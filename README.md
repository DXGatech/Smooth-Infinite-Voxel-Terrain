# Smooth-Infinite-Voxel-Terrain
A voxel terrain generator that allows artists to easily create endless, diverse game worlds.

dependencies: [transvoxel](transvoxel.org) and [polyvox](volumesoffun.com)

octree: 

The original version of polyvox used virtual memory to page in & out terrain chunks.

For this project, we use octree instead, as it allows us to easily implement chunk LOD.

Built-in multi-threading supports memory-consuming components such as A.I. units and surface vegetation.

# Chunk management with octrees
Each leaf node contains a single chunk, whose LOD is updated by the tree. The player carries a voxel modification component integrated with polyvox, and each octree calculates the distance between the component and its central location per frame. The variables mentioned above are used to determine a chunk's LOD, which tremendously improves the oevrall performance during runtime. Here are some pictures comparing a terrain segment generated with the conventional method that loads chunk with virtual memory paging, and another one generated with the maneuver mentioned above.

Generated with conventional method:

A small-sized, limited terrain:

![alt text](https://i2.wp.com/cplusplusgametricks.files.wordpress.com/2017/12/screen-shot-2017-12-19-at-10-38-31-pm.png?ssl=1&w=450)

Generated with octrees:

Huge terrain segment with vegetation and dynamic LOD

![alt text](https://i0.wp.com/cplusplusgametricks.files.wordpress.com/2017/12/screen-shot-2017-12-27-at-10-11-12-pm.png?ssl=1&w=450)
![alt text](https://i0.wp.com/cplusplusgametricks.files.wordpress.com/2017/12/screen-shot-2017-12-27-at-10-11-47-pm.png?ssl=1&w=450)
