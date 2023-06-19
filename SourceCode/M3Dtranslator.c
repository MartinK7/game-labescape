
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct{
    uint32_t vertexes;
    uint32_t normals;
    uint32_t faces;
} M3D_counter;

typedef struct{
    float x;
    float y;
    float z;
} M3D_vertex, M3D_normal;

typedef struct{
    uint32_t vertex1;
    uint32_t vertex2;
    uint32_t vertex3;
    uint32_t normal;
} M3D_face;

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

//return last read character
int getContent(FILE *f, char *output){
    int c = fgetc(f);
    int i = 0;
    while(c != ' ' && c != '\n' && c != EOF && c != '/'){
        output[i++] = (char)c;
        c = fgetc(f);
    }
    output[i++] = 0;
    return c;
}

void newObject(M3D_head_t *head, M3D_object_t *obj, char *name, M3D_counter *counter){
    int i;

    /* Reset */
    obj->cntVertexes = 0;
    obj->cntNormals = 0;
    obj->cntFaces = 0;

    /* Set additional data */
    obj->secOffsetVertexes = counter->vertexes;
    obj->secOffsetNormals = counter->normals;
    obj->secOffsetFaces = counter->faces;

    /* Increment list*/
    ++head->cntObjects;

    /* Set name*/
    strcpy(obj->name, name);

    /* Default data */
    for(i=0; i<3; ++i){
        obj->rotation[i] = 0.0;
        obj->origin[i] = 0.0;
        obj->translation[i] = 0.0;
        obj->color[i] = 0.0;
    }
    obj->color[3] = 0.0;
}

void addObject(M3D_object_t **objarray, M3D_head_t *head, M3D_object_t *obj){
    /* Allocate more data */
    M3D_object_t *temparray = (M3D_object_t*)malloc(sizeof(M3D_object_t)*head->cntObjects);
    memcpy(temparray, (*objarray), sizeof(M3D_object_t)*(head->cntObjects-1));
    free((*objarray));
    (*objarray) = temparray;

    /* Get pointer */
    M3D_object_t *ob = &(*objarray)[head->cntObjects-1];

    /* Copy all data to obj file*/
    memcpy(ob, obj, sizeof(M3D_object_t));
}

void addVertex(M3D_vertex **vertexes, M3D_counter *counter, float x, float y, float z){
    M3D_vertex *temporary = (M3D_vertex*)malloc(sizeof(M3D_vertex)*((*counter).vertexes+1));
    if(temporary == NULL){
        fprintf(stderr, "ERROR: Allocation memory failed !\n");
        exit(105);
    }
    memcpy(temporary, (*vertexes), sizeof(M3D_vertex)*(*counter).vertexes);
    temporary[(*counter).vertexes].x = x;
    temporary[(*counter).vertexes].y = y;
    temporary[(*counter).vertexes].z = z;
    (*counter).vertexes++;
    free((*vertexes));
    (*vertexes) = temporary;
}

void addNormal(M3D_normal **normals, M3D_counter *counter, float x, float y, float z){
    M3D_normal *temporary = (M3D_normal*)malloc(sizeof(M3D_normal)*((*counter).normals+1));
    if(temporary == NULL){
        fprintf(stderr, "ERROR: Allocation memory failed !\n");
        exit(105);
    }
    memcpy(temporary, (*normals), sizeof(M3D_normal)*(*counter).normals);
    temporary[(*counter).normals].x = x;
    temporary[(*counter).normals].y = y;
    temporary[(*counter).normals].z = z;
    (*counter).normals++;
    free((*normals));
    (*normals) = temporary;
}

void addFace(M3D_face **faces, M3D_counter *counter, uint32_t *facedata){
    M3D_face *temporary = (M3D_face*)malloc(sizeof(M3D_face)*((*counter).faces+1));
    if(temporary == NULL){
        fprintf(stderr, "ERROR: Allocation memory failed !\n");
        exit(105);
    }
    memcpy(temporary, (*faces), sizeof(M3D_face)*(*counter).faces);
    temporary[(*counter).faces].vertex1 = facedata[0];
    temporary[(*counter).faces].vertex2 = facedata[1];
    temporary[(*counter).faces].vertex3 = facedata[2];
    temporary[(*counter).faces].normal = facedata[3];
    (*counter).faces++;
    free((*faces));
    (*faces) = temporary;
}

uint8_t getColorFromMTLLIB(FILE **mtllib, char *mtlname, float *red, float *green, float *blue){
    /* Text buffer */
    char txtbuf[100];

    /* Result value */
    int result;

    /* Rewind to inception */
    rewind((*mtllib));

    /* Find flag */
    uint8_t found = 0;

    /* Initialize read */
    result = getContent((*mtllib), txtbuf);

    /* Find texture name */
    while(result != EOF){
        if(strcmp("newmtl", txtbuf) == 0){
            result = getContent((*mtllib), txtbuf); //name
            if(result != '\n' && result != EOF){
                fprintf(stderr, "ERROR: Expected new line !\n");
                exit(101);
            }
            if(strcmp(txtbuf, mtlname) == 0){
                //Found!
                found = 1;
            }
        }else if(strcmp("Kd", txtbuf) == 0 && found == 1){
            result = getContent((*mtllib), txtbuf);
            if(result != ' '){
                fprintf(stderr, "ERROR: Expected char:' ' !\n");
                exit(101);
            }
            (*red) = strtof(txtbuf, NULL);
            result = getContent((*mtllib), txtbuf);
            if(result != ' '){
                fprintf(stderr, "ERROR: Expected char:' ' !\n");
                exit(101);
            }
            (*green) = strtof(txtbuf, NULL);
            result = getContent((*mtllib), txtbuf);
            if(result != '\n' && result != EOF){
                fprintf(stderr, "ERROR: Expected new line!\n");
                exit(101);
            }
            (*blue) = strtof(txtbuf, NULL);

            return 0; //Success
        }

        result = getContent((*mtllib), txtbuf);
    }

    return 1; //Fail! Texture not found!
}

int M3DTranslator_run(char *filename, char *outputFileName){
    /* Text buffer */
    char txtbuf[1000];

    /* Result value */
    int result;

    /* M3D file construction */
    M3D_object_t *objectarray = (M3D_object_t*)malloc(sizeof(M3D_object_t));
    M3D_head_t head;
    head.conststr[0] = 'M';
    head.conststr[1] = '3';
    head.conststr[2] = 'D';
    head.filever = 2;
    head.cntObjects = 0;
    head.offsetVertexes = 0;
    head.offsetNormals = 0;
    head.offsetVertexes = 0;

    /* Object building */
    M3D_object_t object;
    object.cntVertexes = 0;
    object.cntNormals = 0;
    object.cntFaces = 0;
    M3D_counter stuffcounter;
    stuffcounter.vertexes = 0;
    stuffcounter.normals = 0;
    stuffcounter.faces = 0;
    M3D_vertex *vertexes = (M3D_vertex*)malloc(sizeof(M3D_vertex));
    M3D_normal *normals = (M3D_normal*)malloc(sizeof(M3D_normal));;
    M3D_face *faces = (M3D_face*)malloc(sizeof(M3D_face));

    /* Load OBJ file */
    FILE *file = fopen(filename, "rb");
    if(file == NULL){
        fprintf(stderr, "ERROR: No input file with name '%s' !\n", filename);
        exit(1);
    }

    /* MTL file */
    FILE *mtlfile = NULL;

    /* Initialize read */
    result = getContent(file, txtbuf);

    /* Read whole file*/
    while(result != EOF){
        if(strcmp("o", txtbuf) == 0){
            printf("Object\n");

            if(head.cntObjects != 0)
                addObject(&objectarray, &head, &object);

            result = getContent(file, txtbuf);
            if(result != '\n' && result != EOF){
                fprintf(stderr, "ERROR: Expected new line!\n");
                exit(101);
            }
            printf("\tAdded new object, name of this object is '%s'\n", txtbuf);
            txtbuf[15] = '\0';
            newObject(&head, &object, txtbuf, &stuffcounter);
        }else if(strcmp("v", txtbuf) == 0){
            float x, y, z;
            printf("Vertex\n");
            result = getContent(file, txtbuf);
            if(result != ' '){
                fprintf(stderr, "ERROR: Expected char:' ' !\n");
                exit(101);
            }
            x = strtof(txtbuf, NULL);
            result = getContent(file, txtbuf);
            if(result != ' '){
                fprintf(stderr, "ERROR: Expected char:' ' !\n");
                exit(101);
            }
            y = strtof(txtbuf, NULL);
            result = getContent(file, txtbuf);
            if(result != '\n' && result != EOF){
                fprintf(stderr, "ERROR: Expected new line!\n");
                exit(101);
            }
            z = strtof(txtbuf, NULL);
            ++object.cntVertexes;
            addVertex(&vertexes, &stuffcounter, x, y, z);
            printf("\tAdded new vertex, coordinates is x=%.6f, y=%.6f, z=%.6f\n", x, y, z);
        }else if(strcmp("vn", txtbuf) == 0){
            float x, y, z;
            printf("Normal\n");
            result = getContent(file, txtbuf);
            if(result != ' '){
                fprintf(stderr, "ERROR: Expected char:' ' !\n");
                exit(101);
            }
            x = strtof(txtbuf, NULL);
            result = getContent(file, txtbuf);
            if(result != ' '){
                fprintf(stderr, "ERROR: Expected char:' ' !\n");
                exit(101);
            }
            y = strtof(txtbuf, NULL);
            result = getContent(file, txtbuf);
            if(result != '\n' && result != EOF){
                fprintf(stderr, "ERROR: Expected new line!\n");
                exit(101);
            }
            z = strtof(txtbuf, NULL);
            ++object.cntNormals;
            addNormal(&normals, &stuffcounter, x, y, z);
            //objectAddNormal(&normals, &FilecntNormals, x, y, z);
            //++FilecntNormals;
            printf("\tAdded new normal, coordinates is x=%.6f, y=%.6f, z=%.6f\n", x, y, z);
        }else if(strcmp("f", txtbuf) == 0){
            uint32_t face[3+1];
            int i;
            printf("Face\n");

            for(i=0; i<3; ++i){
                result = getContent(file, txtbuf); //vertex
                if(result != '/'){
                    fprintf(stderr, "ERROR: Expected char:'/' !\n");
                    exit(101);
                }
                face[i] = (uint32_t)strtoul(txtbuf, NULL, 0) - 1;
                if(getContent(file, txtbuf) != '/'){
                    fprintf(stderr, "ERROR: Expected char:'/' !\n");
                    exit(101);
                }
                result = getContent(file, txtbuf);
                if(result != ' ' && result != '\n'){
                    fprintf(stderr, "ERROR: Expected new line!\n");
                    exit(101);
                }
            }
            face[3] = (uint32_t)strtoul(txtbuf, NULL, 0) - 1;
            ++object.cntFaces;
            addFace(&faces, &stuffcounter, face);
            //objectAddFace(&faces, &FilecntFaces, face);
            //++FilecntFaces;
            printf("\tAdded new face, vertex is '%d:%d:%d' normal '%d'\n", face[0], face[1], face[2], face[3]);
        }else if(strcmp("#", txtbuf) == 0){
            printf("Comment '#'\n");
            do{
                result = getContent(file, txtbuf);
            }while(result != '\n' && result != EOF);
        }else if(strcmp("mtllib", txtbuf) == 0){
            printf("Material library\n");
            if(mtlfile == NULL){
                result = getContent(file, txtbuf); //vertex
                if(result != '\n' && result != EOF){
                    fprintf(stderr, "ERROR: Expected new line!\n");
                    exit(101);
                }
                
                // 2023 hack:
                snprintf(txtbuf, sizeof(txtbuf), filename);
                if(strlen(txtbuf) >= 3) {
                	txtbuf[strlen(txtbuf) - 3] = 'm';
                	txtbuf[strlen(txtbuf) - 2] = 't';
                	txtbuf[strlen(txtbuf) - 1] = 'l';
                }
                
                printf("\tMaterial library is not open, opening file '%s'\n", txtbuf);
                mtlfile = fopen(txtbuf, "rb");
                if(mtlfile == NULL){
                    fprintf(stderr, "ERROR: No material library with name '%s' !\n", txtbuf);
                    exit(100);
                }
                printf("\tLibrary loaded\n");
            }
        }else if(strcmp("usemtl", txtbuf) == 0){
            float red, green, blue;
            printf("Use material\n");
            if(mtlfile != NULL){
                result = getContent(file, txtbuf);
                if(result != '\n' && result != EOF){
                    fprintf(stderr, "ERROR: Expected new line!\n");
                    exit(101);
                }
                printf("\tName of this material is '%s'\n", txtbuf);
                if(!getColorFromMTLLIB(&mtlfile, txtbuf, &red, &green, &blue)){
                    printf("\tApplying material, color is red=%.6f green=%.6f blue=%.6f'\n", red, green, blue);
                    object.color[0] = red;
                    object.color[1] = green;
                    object.color[2] = blue;
                    object.color[3] = 1.0;
                }else{
                    fprintf(stderr, "\tWARNING: Material not found! Applying black color.\n");
                    object.color[0] = 0.0;
                    object.color[1] = 0.0;
                    object.color[2] = 0.0;
                    object.color[3] = 1.0;
                }
            }else{
                    fprintf(stderr, "\tWARNING: Material library not loaded! Applying black color.\n");
                    object.color[0] = 0.0;
                    object.color[1] = 0.0;
                    object.color[2] = 0.0;
                    object.color[3] = 1.0;
            }
        }

        result = getContent(file, txtbuf);
    }

    /* Add last object */
    if(head.cntObjects != 0)
        addObject(&objectarray, &head, &object);

    /* Write output file*/
    FILE *outfile = fopen(outputFileName, "wb");
    fwrite(&head, sizeof(M3D_head_t), 1, outfile);
    fwrite(objectarray, sizeof(M3D_object_t)*head.cntObjects, 1, outfile);
    head.offsetVertexes = (uint32_t)ftell(outfile);
    fwrite(vertexes, sizeof(M3D_vertex)*stuffcounter.vertexes, 1, outfile);
    head.offsetNormals = (uint32_t)ftell(outfile);
    fwrite(normals, sizeof(M3D_normal)*stuffcounter.normals, 1, outfile);
    head.offsetFaces = (uint32_t)ftell(outfile);
    fwrite(faces, sizeof(M3D_face)*stuffcounter.faces, 1, outfile);
    rewind(outfile);
    fwrite(&head, sizeof(M3D_head_t), 1, outfile);

    /* Free memory */
    fclose(outfile);
    fclose(mtlfile);
    fclose(file);
    free(objectarray);
    free(vertexes);
    free(normals);
    free(faces);

    /* Everything was ok */
    return 0;
}
