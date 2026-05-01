#include "OverlapDetector.h"
#include "Octree.h"
#include "Component/PrimitiveComponent.h"

#include <functional>

void FOverlapDetector::Tick(const FOctree* Octree)
{
    CurrentTickCollisionPairs = GetOverlappingPair(Octree);

    int CurrentSize = static_cast<int>(CurrentTickCollisionPairs.size());
    for (int i=0; i<CurrentSize; i++)
    {
        if (!ContainsCollisionPair(PreviousTickCollisionPairs, CurrentTickCollisionPairs.at(i)))
        {
            BroadcastBeginOverlap(CurrentTickCollisionPairs.at(i));
        }
    }

    int PreviousSize = static_cast<int>(PreviousTickCollisionPairs.size());
    for (int i=0; i<PreviousSize; i++)
    {
        if (!ContainsCollisionPair(CurrentTickCollisionPairs, PreviousTickCollisionPairs.at(i)))
        {
            BroadcastEndOverlap(PreviousTickCollisionPairs.at(i));
        }
    }

    PreviousTickCollisionPairs = CurrentTickCollisionPairs;
    CurrentTickCollisionPairs.clear();
}

void FOverlapDetector::Clear()
{
    int Size = static_cast<int>(CollisionComponents.size());
    for (int i=0; i<Size; i++)
    {
        if (CollisionComponents.at(i))
        {
            CollisionComponents.at(i)->SetRegisteredToOverlapDetector(false);
        }
    }
    
    CollisionComponents.clear();
    CurrentTickCollisionPairs.clear();
    PreviousTickCollisionPairs.clear();
}

TArray<CollisionPair> FOverlapDetector::GetOverlappingPair(const FOctree* Octree)
{
    TArray<CollisionPair> OverlapPairs;
    if (!Octree) return OverlapPairs;
    
    int Size = static_cast<int>(CollisionComponents.size());
    for (int i=0; i<Size; i++)
    {
        UPrimitiveComponent* CurrentComponent = CollisionComponents.at(i);
        if (!CurrentComponent) continue;
        
        TArray<UPrimitiveComponent*> OverlapComponents;
        Octree->QueryAABB(CurrentComponent->GetWorldBoundingBox(), OverlapComponents);
        int OverlapSize = static_cast<int>(OverlapComponents.size());
        for (int j=0; j<OverlapSize; j++)
        {
            UPrimitiveComponent* OtherComponent = OverlapComponents.at(j);
            if (!OtherComponent) continue;
            if (CurrentComponent == OtherComponent) continue;
            if (!OtherComponent->IsRegisteredToOverlapDetector()) continue;
            if (CurrentComponent->GetOwner() == OtherComponent->GetOwner()) continue;
            if (std::less<UPrimitiveComponent*>()(OtherComponent, CurrentComponent)) continue;
            
            CollisionPair Pair;
            Pair.Prim1 = CurrentComponent;
            Pair.Prim2 = OtherComponent;
            OverlapPairs.push_back(Pair);
        }
    }
    return OverlapPairs;
}

bool FOverlapDetector::RegisterComponent(UPrimitiveComponent* Component)
{
    if (!Component) return false;
    //If Already Registered, Ignore
    if (Component->IsRegisteredToOverlapDetector()) return true; 
    CollisionComponents.push_back(Component);
    Component->SetRegisteredToOverlapDetector(true);
    return true;
}

bool FOverlapDetector::UnregisterComponent(UPrimitiveComponent* Component)
{
    if (!Component) return false;
    //If Not Registered, No Need to Traverse Array 
    if (!Component->IsRegisteredToOverlapDetector()) return false;
    
    int Size = static_cast<int>(CollisionComponents.size());
    for (int i=0; i<Size; i++)
    {
        if (CollisionComponents.at(i) == Component)
        {
            CollisionComponents.at(i) = CollisionComponents.back();
            CollisionComponents.pop_back();
            Component->SetRegisteredToOverlapDetector(false);
            RemoveCollisionPair(Component);
            return true;
        }
    }
    Component->SetRegisteredToOverlapDetector(false);
    RemoveCollisionPair(Component);
    return false;
}

bool FOverlapDetector::ContainsCollisionPair(const TArray<CollisionPair>& Pairs, const CollisionPair& Pair) const
{
    int Size = static_cast<int>(Pairs.size());
    for (int i=0; i<Size; i++)
    {
        if (Pairs.at(i) == Pair)
        {
            return true;
        }
    }
    return false;
}

void FOverlapDetector::BroadcastBeginOverlap(const CollisionPair& Pair)
{
    if (!Pair.Prim1 || !Pair.Prim2) return;
    
    Pair.Prim1->BroadcastComponentBeginOverlap(Pair.Prim2->GetOwner());
    Pair.Prim2->BroadcastComponentBeginOverlap(Pair.Prim1->GetOwner());
}

void FOverlapDetector::BroadcastEndOverlap(const CollisionPair& Pair)
{
    if (!Pair.Prim1 || !Pair.Prim2) return;
    
    Pair.Prim1->BroadcastComponentEndOverlap(Pair.Prim2->GetOwner());
    Pair.Prim2->BroadcastComponentEndOverlap(Pair.Prim1->GetOwner());
}

void FOverlapDetector::RemoveCollisionPair(UPrimitiveComponent* Component)
{
    for (int i=static_cast<int>(PreviousTickCollisionPairs.size())-1; i>=0; i--)
    {
        CollisionPair Pair = PreviousTickCollisionPairs.at(i);
        if (Pair.Prim1 == Component || Pair.Prim2 == Component)
        {
            BroadcastEndOverlap(Pair);
            PreviousTickCollisionPairs.erase(PreviousTickCollisionPairs.begin()+i);
        }
    }

    for (int i=static_cast<int>(CurrentTickCollisionPairs.size())-1; i>=0; i--)
    {
        CollisionPair Pair = CurrentTickCollisionPairs.at(i);
        if (Pair.Prim1 == Component || Pair.Prim2 == Component)
        {
            CurrentTickCollisionPairs.erase(CurrentTickCollisionPairs.begin()+i);
        }
    }
}
