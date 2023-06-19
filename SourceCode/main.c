#include "opengl_engine.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "M3Dtranslator.h"

//#include "demo.h"

/* Main OpenGL GLUT function */
int main(int argc, char** argv) {
    //runme(argc, argv);
    //while(1);
    int i;
    for(i=1; i<argc; ++i){
        if(strcmp(argv[i], "-translate")==0){
            if(i+2 < argc){
                printf("Running M3D Translator !\n");
                M3DTranslator_run(argv[i+1], argv[i+2]);
                printf("Translating file '%s' to M3D binary file '%s' was SUCCESSFUL!\n", argv[i+1], argv[i+2]);
                exit(0);
            }
        }else{
            fprintf(stderr, "ERROR: Unknown commands\n");
            printf("Aviaible is only command:\n\t-translate [InputFileName] [OutputFileName]\n");
            exit(1);
        }
    }
    arguments_t arguments;
    arguments.argc = argc;
    arguments.argv = argv;

    pthread_t openglThr;

    /* Create OPENGL thread */
    if(pthread_create(&openglThr, NULL, opengl_thread, &arguments)){
        fprintf(stderr, "ERROR: Creating OPENGL thread failed!\n");
        return 200;
    }

    /* Join to thread */
    if(pthread_join(openglThr, NULL)){
        fprintf(stderr, "ERROR: Joining in OPENGL thread failed!\n");
        return 201;
    }

    /* The end of GLUT window and program */
    return 0;
}
