
typedef struct{
    int argc;
    char** argv;
} arguments_t;

void *opengl_thread(void *argumentsPtr);
