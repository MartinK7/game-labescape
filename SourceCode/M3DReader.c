
#include "M3DReader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t M3D_load(M3DFILE_t *file, char *name){
    /* As first load whole file into RAM*/
    uint32_t lenght;
    FILE *f = fopen(name, "rb");
    if(f == NULL)return 1;
    fseek(f, 0, SEEK_END);
    lenght = ftell(f);
    file->rawdata = (uint8_t*)malloc(lenght);
    rewind(f);
    fread(file->rawdata, lenght, 1, f);
    fclose(f);

    /* Lets make pointers */
    file->head = (M3D_head_t*)(file->rawdata);
    file->objects = (M3D_object_t*)(file->rawdata + sizeof(M3D_head_t));
    file->vertexes = (M3D_vertex_t*)(file->rawdata + file->head->offsetVertexes);
    file->normals = (M3D_normal_t*)(file->rawdata + file->head->offsetNormals);
    file->faces = (M3D_face_t*)(file->rawdata + file->head->offsetFaces);

    return 0;
}

void M3D_free(M3DFILE_t *file){
    free(file->rawdata);
}

uint32_t M3D_gCntObj(M3DFILE_t *file){
    return file->head->cntObjects;
}

uint32_t M3D_gCntVer(M3DFILE_t *file, uint32_t object){
    return file->objects[object].cntVertexes;
}

uint32_t M3D_gCntNor(M3DFILE_t *file, uint32_t object){
    return file->objects[object].cntNormals;
}

uint32_t M3D_gCntFac(M3DFILE_t *file, uint32_t object){
    return file->objects[object].cntFaces;
}

float* M3D_gClrRGB(M3DFILE_t *file, uint32_t object){
    return file->objects[object].color;
}

float* M3D_gRotXYZ(M3DFILE_t *file, uint32_t object){
    return file->objects[object].rotation;
}

float* M3D_gTraXYZ(M3DFILE_t *file, uint32_t object){
    return file->objects[object].translation;
}

float* M3D_gOriXYZ(M3DFILE_t *file, uint32_t object){
    return file->objects[object].origin;
}

M3D_vertex_t* M3D_gVer(M3DFILE_t *file, uint32_t object, uint32_t vertex){
    return &file->vertexes[file->objects[object].secOffsetVertexes + vertex];
}

M3D_normal_t* M3D_gNor(M3DFILE_t *file, uint32_t object, uint32_t normal){
    return &file->normals[file->objects[object].secOffsetNormals + normal];
}

M3D_face_t* M3D_gFac(M3DFILE_t *file, uint32_t object, uint32_t face){
    return &file->faces[file->objects[object].secOffsetFaces + face];
}

M3D_vertex_t* M3D_gOVer(M3DFILE_t *file, uint32_t vertex){
    return &file->vertexes[vertex];
}

M3D_normal_t* M3D_gONor(M3DFILE_t *file, uint32_t normal){
    return &file->normals[normal];
}

M3D_face_t* M3D_gOFac(M3DFILE_t *file, uint32_t face){
    return &file->faces[face];
}

uint32_t M3D_gObjInx(M3DFILE_t *file, char *name){
    uint32_t i;
    for(i=0; i<file->head->cntObjects; ++i){
        if(strcmp(file->objects[i].name, name) == 0)
            return i;
    }

    fprintf(stderr, "ERROR: Object '%s' not found!\n", name);
    exit(111);

    return 0;
}
