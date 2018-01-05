# Smooth-Infinite-Voxel-Terrain
A voxel terrain generator that allows artists to easily create endless, diverse game worlds.

dependencies: [transvoxel](www.transvoxel.org) and [polyvox](www.volumesoffun.com)

octree: 

The original version of polyvox used virtual memory to page in & out terrain chunks.

For this project, we use octree instead, as it allows us to easily implement chunk LOD.

Built-in multi-threading supports memory-consuming components such as A.I. units and surface vegetation.

# Chunk management with octrees
Each leaf node contains a single chunk, whose LOD is updated by the tree. The player carries a voxel modification component integrated with polyvox, and each octree calculates the distance between the component and its central location per frame. The variables mentioned above are used to determine a chunk's LOD, which tremendously improves the overall performance during runtime. Here are some pictures comparing a terrain segment generated with the conventional method that loads chunk with virtual memory paging, and another one generated with the maneuver mentioned above.

Generated with conventional method:

A small-sized, limited with virtual memory paging:

![alt text](https://i2.wp.com/cplusplusgametricks.files.wordpress.com/2017/12/screen-shot-2017-12-19-at-10-38-31-pm.png?ssl=1&w=450)

Generated with octrees:

Huge terrain segment with vegetation and dynamic LOD

Please note that smooth shading is supported. The lowpoly effect is intentionally created by the terrain material with DDX and DDY nodes.

![alt text](https://i1.wp.com/cplusplusgametricks.files.wordpress.com/2018/01/screen-shot-2018-01-02-at-4-36-03-pm.png?ssl=1&w=450)
![alt text](https://i0.wp.com/cplusplusgametricks.files.wordpress.com/2018/01/screen-shot-2017-12-28-at-10-15-13-am.png?ssl=1&w=450)
![alt text](https://i1.wp.com/cplusplusgametricks.files.wordpress.com/2018/01/screen-shot-2018-01-02-at-4-36-54-pm.png?ssl=1&w=450)

Each octree calculates the distance between the player and its central position to determine its LOD. A lower LOD indicates a larger octree with a single chunk in the same size.

# Drawbacks of virtual memory paging:
When used with a large-scale polygon engine, chunks must be paged out of virtual memory in a constructor. As a result, engine-specific multi-threading functions can not access terrain data during construction, which is probably the reason why the conventional method is so slow.
