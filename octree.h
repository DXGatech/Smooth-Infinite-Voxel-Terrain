#pragma once
#include "CoreMinimal.h"

class octree
{
    
public:
    octree(FIntVector position, int depth, int id);
    
    bool operator==(octree agent) const;
    bool operator<(octree agent) const;
    bool operator>(octree agent) const;
    
    FIntVector m_position;
    int m_depth;
    int m_id;
    bool m_bFather;
    
    // we use force inline to maximize performance
    
    FORCEINLINE int size();
    
    FORCEINLINE FIntVector getMin();
    FORCEINLINE FIntVector getMax();
    
    FORCEINLINE bool bLeaf();
    FORCEINLINE bool bInTree(int x, int y, int z);
    
    FORCEINLINE FIntVector glconvert(int x, int y, int z); // convert from global coord to local coord
    FORCEINLINE FIntVector lgconvert(int x, int y, int z); // vice versa
    
    FORCEINLINE int depth_id(int depth);
    
    //TODO: implement region
    // region getBound();
    
    
};
