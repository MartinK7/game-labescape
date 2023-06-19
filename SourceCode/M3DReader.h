#include <stdint.h>

typedef struct{
    char conststr[3];
    uint8_t filever;
    uint32_t cntObjects;
    uint32_t offsetVertexes;
    uint32_t offsetNormals;
    uint32_t offsetFaces;
} M3D_head_t;

typedef struct{
    char name[16];
    uint32_t cntVertexes;
    uint32_t cntNormals;
    uint32_t cntFaces;
    float origin[3];
    float rotation[3];
    float translation[3];
    float color[4];
    uint32_t secOffsetVertexes;
    uint32_t secOffsetNormals;
    uint32_t secOffsetFaces;
} M3D_object_t;

typedef struct{
    float x;
    float y;
    float z;
} M3D_vertex_t, M3D_normal_t;

typedef struct{
    uint32_t vertex1;
    uint32_t vertex2;
    uint32_t vertex3;
    uint32_t normal;
} M3D_face_t;

typedef struct{
    uint8_t *rawdata;
    M3D_head_t *head;
    M3D_object_t *objects;
    M3D_vertex_t *vertexes;
    M3D_normal_t *normals;
    M3D_face_t *faces;
} M3DFILE_t;

/* Main commands */
uint8_t M3D_load(M3DFILE_t *file, char *name);
void M3D_free(M3DFILE_t *file);

/* Get functions about M3D file */
uint32_t M3D_gCntObj(M3DFILE_t *file);
uint32_t M3D_gCntVer(M3DFILE_t *file, uint32_t object);
uint32_t M3D_gCntNor(M3DFILE_t *file, uint32_t object);
uint32_t M3D_gCntFac(M3DFILE_t *file, uint32_t object);

/* Get functions for objects */
float* M3D_gClrRGB(M3DFILE_t *file, uint32_t object);
float* M3D_gRotXYZ(M3DFILE_t *file, uint32_t object);
float* M3D_gTraXYZ(M3DFILE_t *file, uint32_t object);
float* M3D_gOriXYZ(M3DFILE_t *file, uint32_t object);

/* Get functions for 3D data inside object */
M3D_vertex_t* M3D_gVer(M3DFILE_t *file, uint32_t object, uint32_t vertex);
M3D_normal_t* M3D_gNor(M3DFILE_t *file, uint32_t object, uint32_t normal);
M3D_face_t* M3D_gFac(M3DFILE_t *file, uint32_t object, uint32_t face);
M3D_vertex_t* M3D_gOVer(M3DFILE_t *file, uint32_t vertex);
M3D_normal_t* M3D_gONor(M3DFILE_t *file, uint32_t normal);
M3D_face_t* M3D_gOFac(M3DFILE_t *file, uint32_t face);

/* Searching function */
uint32_t M3D_gObjInx(M3DFILE_t *file, char *name);
