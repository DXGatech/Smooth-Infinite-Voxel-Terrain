#include "octree.h"

octree::octree(FIntVector position, int depth, int id) : m_position(position), m_depth(depth), m_id(id), m_bFather(false)
{
    check(depth <= 19);
}

int octree::size()
{
    return 16 << m_depth;
}

FIntVector octree::getMin(int x, int y, int z)
{
    return position - FIntVector(size() / 2, size() / 2, size() / 2); // low left corner
}

FIntVector octree::getMax(int x, int y, int z)
{
    return position + FIntVector(size() / 2, size() / 2, size() / 2); // top right corner
}

bool octree::bLeaf()
{
    return !m_bFather;
}

bool octree::bInTree(int x, int y, int z)
{
    return position.X - size() / 2 <= x && x < position.X + size() / 2 && position.X - size() / 2 <= x && x < position.X + size() / 2 && position.X - size() / 2 <= x && x < position.X + size() / 2; // from gamedev.net
}

FIntVector octree::glconvert(int x, int y, int z)
{
    return FIntVector(x - (position.X - size() / 2), y - (position.Y - size() / 2), z - (position.Z - size() / 2));
}

FIntVector octree::lgconvert(int x, int y, int z)
{
    return FIntVector(x + (position.X + size() / 2), y + (position.Y - size() / 2), z + (position.Z - size() / 2));
}

int octree::depth_id(int depth)
{
    check(depth <= 19);
    int result_id = 1;
    for(int i = 0; i < depth; i++){
        result *= 9;
    }
    return result_id;
}
