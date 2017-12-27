# Smooth-Infinite-Voxel-Terrain
A voxel terrain generator that allows artists to easily create endless, diverse game worlds.

octree: the original version of polyvox used virtual memory to page in & out terrain chunks.

For this project, we use octree instead, as it allows us to easily implement chunk LOD.

Built-in multi-threading supports memory-consuming components such as A.I. units and surface vegetation.

# Chunk management with octrees
Each leaf node contains a single chunk, whose LOD is updated by the tree. The player carries a voxel modification component integrated with polyvox, and each octree calculates the distance between the component and its central location per frame. The variables mentioned above are used to determine a chunk's LOD, which tremendously improves the oevrall performance during runtime. Here are some pictures comparing a terrain segment generated with the conventional method that loads chunk with virtual memory paging, and another one generated with the maneuver mentioned above.

Generated with conventional method:

![alt text]()

Generated with octrees:

![alt text]()
![alt text]()
