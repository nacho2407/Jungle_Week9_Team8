#pragma once
#include "Core/CoreTypes.h"

class FOctree;
class UPrimitiveComponent;

struct CollisionPair
{
    UPrimitiveComponent* Prim1;
    UPrimitiveComponent* Prim2;

    bool operator==(const CollisionPair& Other) const
    {
        return Prim1 == Other.Prim1 && Prim2 == Other.Prim2;
    }
};

class FOverlapDetector
{
private:
public:
    void Tick(const FOctree* Octree);
    void Clear();
    TArray<CollisionPair> GetOverlappingPair(const FOctree* Octree);
    bool RegisterComponent(UPrimitiveComponent* Component);
    bool UnregisterComponent(UPrimitiveComponent* Component);
private:
    bool ContainsCollisionPair(const TArray<CollisionPair>& Pairs, const CollisionPair& Pair) const;
    void BroadcastBeginOverlap(const CollisionPair& Pair);
    void BroadcastEndOverlap(const CollisionPair& Pair);
    void RemoveCollisionPair(UPrimitiveComponent* Component);
    
    TArray<UPrimitiveComponent*> CollisionComponents;
    TArray<CollisionPair> CurrentTickCollisionPairs;
    TArray<CollisionPair> PreviousTickCollisionPairs;
};
