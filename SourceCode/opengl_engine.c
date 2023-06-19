#include <GL/glut.h>

#include "opengl_engine.h"
#include "game.h"
#include <math.h>
#include <stdio.h>

/* Global variables :-( */
gameLabEscape_t game;

void opengl_init(void){
    /* Default camera position and rotation*/
    game_init(&game, "map.m3d");

    /* Cursor not warped yet */
    game.cursorNotWarped = 1;

    /* Enable a single OpenGL light. */
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, game.light0_color);
    glLightfv(GL_LIGHT0, GL_POSITION, game.light0_position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    /* Use depth buffering for hidden surface elimination. */
    glEnable(GL_DEPTH_TEST);

    /* Setup view of scene. */
    glMatrixMode(GL_PROJECTION);
    gluPerspective( /* field of view in degree */ 40.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 0.01, /* Z far */ 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void opengl_mouseMotion(int x, int y){
    static uint8_t warped = 0;

    glutSetCursor(GLUT_CURSOR_NONE);

    if(game.cursorNotWarped){
        glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/2, glutGet(GLUT_WINDOW_HEIGHT)/2);
        game.cursorNotWarped = 0;
        return;
    }

    if(warped) {
        warped = 0;
        return;
    }

    int dx = x - glutGet(GLUT_WINDOW_WIDTH)/2;
    int dy = y - glutGet(GLUT_WINDOW_HEIGHT)/2;

    if(dx) {
        game.cameraRotYaw += 0.005*dx;
    }

    if(dy) {
        game.cameraRotPitch -= 0.005*dy;
        if(game.cameraRotPitch > 1.5)game.cameraRotPitch = 1.5;
        if(game.cameraRotPitch < -1.5)game.cameraRotPitch = -1.5;
    }

    glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/2, glutGet(GLUT_WINDOW_HEIGHT)/2);

    warped = 1;
}

void opengl_drawGameObjects(void){
    /* Initialize variables */
    uint32_t i, a;
    float *ptrfloat;
    M3D_normal_t *ptrnormal;
    M3D_face_t *ptrface;
    M3D_vertex_t *ptrvertex;

    /* Draw every object in map file */
    for(i=0; i<M3D_gCntObj(&game.gameMap); ++i){
        ptrfloat = M3D_gClrRGB(&game.gameMap, i);
        if(ptrfloat[3] > 0.1){
            glPushMatrix();

                /* Translate */
                ptrfloat = M3D_gTraXYZ(&game.gameMap, i);
                glTranslatef(ptrfloat[0], ptrfloat[1], ptrfloat[2]);

                /* Rotate around origin */
                ptrfloat = M3D_gOriXYZ(&game.gameMap, i);
                glTranslatef(ptrfloat[0], ptrfloat[1], ptrfloat[2]);
                ptrfloat = M3D_gRotXYZ(&game.gameMap, i);
                glRotatef(ptrfloat[0], 1.0, 0.0, 0.0);
                glRotatef(ptrfloat[1], 0.0, 1.0, 0.0);
                glRotatef(ptrfloat[2], 0.0, 0.0, 1.0);
                ptrfloat = M3D_gOriXYZ(&game.gameMap, i);
                glTranslatef(-ptrfloat[0], -ptrfloat[1], -ptrfloat[2]);

                /* Set current material color */
                glBegin(GL_TRIANGLES);
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, M3D_gClrRGB(&game.gameMap, i));
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, M3D_gClrRGB(&game.gameMap, i));
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0F);
                glEnable(GL_COLOR_MATERIAL);
                glEnd();

                /* Draw all polygons in object */
                glBegin(GL_TRIANGLES);
                for(a = 0; a < M3D_gCntFac(&game.gameMap, i); ++a){
                    ptrface = M3D_gFac(&game.gameMap, i, a);

                    ptrnormal = M3D_gONor(&game.gameMap, ptrface->normal);
                    glNormal3f(ptrnormal->x, ptrnormal->y, ptrnormal->z);
                    ptrvertex = M3D_gOVer(&game.gameMap, ptrface->vertex1);
                    glVertex3f(ptrvertex->x, ptrvertex->y, ptrvertex->z);
                    ptrvertex = M3D_gOVer(&game.gameMap, ptrface->vertex2);
                    glVertex3f(ptrvertex->x, ptrvertex->y, ptrvertex->z);
                    ptrvertex = M3D_gOVer(&game.gameMap, ptrface->vertex3);
                    glVertex3f(ptrvertex->x, ptrvertex->y, ptrvertex->z);
                }
                glEnd();
            glPopMatrix();
        }
    }
}

void drawstring(float x,float y,float z,char *string)
{
 char *c;
 glRasterPos3f(x,y,z);
 for(c=string;*c!='\0';c++)
 { glColor3f(0.0,0.0,0.0);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*c);
 }
}

void opengl_display(void){
    glClearColor (0.0,0.0,0.0,1.0); //clear the screen to black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	game.cameraRotation[0] = cos(game.cameraRotYaw) * cos(game.cameraRotPitch);
	game.cameraRotation[1] = sin(game.cameraRotPitch);
	game.cameraRotation[2] = sin(game.cameraRotYaw) * cos(game.cameraRotPitch);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(game.cameraPosition[0],
           game.cameraPosition[1],
           game.cameraPosition[2],
           game.cameraPosition[0] + game.cameraRotation[0],
           game.cameraPosition[1] + game.cameraRotation[1],
           game.cameraPosition[2] + game.cameraRotation[2],
           0.0, 1.0, 0.0);

    /* Draw all */
    opengl_drawGameObjects();

    /* Draw info text */
    char temporaryText[64];
    sprintf(temporaryText, "%s %s", game.inventoryObjects[game.inventorySelectedItem].name, game.infoText);
    glPushMatrix();
        float colorWhite[4] = {1.0, 1.0, 1.0, 1.0};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorWhite);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorWhite);
        drawstring(game.cameraPosition[0]+0.2*game.cameraRotation[0], game.cameraPosition[1]+0.2*game.cameraRotation[1], game.cameraPosition[2]+0.2*game.cameraRotation[2], temporaryText);
    glPopMatrix();

    /*glPushMatrix();
    glBegin(GL_LINES);
    glVertex3f(1.0, 0.0, 1.0);
    glVertex3f(-1.0, 0.0, -1.0);
    glVertex3f(-1.0, 0.0, 1.0);
    glVertex3f(1.0, 0.0, -1.0);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, -1.0, 0.0);
    glEnd();
    glPopMatrix();*/

    /* Show it! */
    glutSwapBuffers(); //swap the buffers
}

void opengl_reshape(int w, int h){
    /* Transform drawing */
    glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
    glMatrixMode (GL_PROJECTION); //set the matrix to projection
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w/(GLfloat)h, 0.1 , 100.0); //set the perspective (angle of sight, width, height, ,depth)
    glMatrixMode(GL_MODELVIEW); //set the matrix back to model
}

void opengl_keyPress(unsigned char key, int x, int y){
    if(key == 'p')glutDestroyWindow(1);
    game.keymap[key] = 1;
}

void opengl_keyUnpress(unsigned char key, int x, int y){
    game.keymap[key] = 0;
}

void opnegl_mouseClick(int buttonID, int boolUnPress, int x, int y){
    int16_t i;
    float distance, distanceTest;
    float distanceStart = 0.01, distanceStop = 3.0, distanceStep = 0.01;
    if(buttonID == GLUT_LEFT_BUTTON && boolUnPress == 1){
        for(i=0; i<CNT_CLICK_EVENTS; ++i){
            for(distanceTest=distanceStart; distanceTest<=distanceStop; distanceTest+=distanceStep){
                distance = 0.0;
                    distance += pow(game.cameraPosition[0]+distanceTest*game.cameraRotation[0]-game.clickEvents[i].X, 2);
                    distance += pow(game.cameraPosition[1]+distanceTest*game.cameraRotation[1]-game.clickEvents[i].Y, 2);
                    distance += pow(game.cameraPosition[2]+distanceTest*game.cameraRotation[2]-game.clickEvents[i].Z, 2);
                distance = sqrt(distance);
                if(distance < game.clickEvents[i].radius){
                    game_objectClick(&game, distance, game.clickEvents[i].name);
                    return;
                }
            }
        }
    }else if(buttonID == GLUT_RIGHT_BUTTON && boolUnPress == 1){
        game.inventorySelectedItem = 0;
    }else if(buttonID == 3 && boolUnPress == 1){
        i = game.inventorySelectedItem;
        while(1){
            if(++i >= CNT_MAX_INVENTORY_ITEMS)i=0;
            if(game.inventoryObjects[i].boolExist == 1){
                game.inventorySelectedItem = i;
                break;
            }
        }
    }else if(buttonID == 4 && boolUnPress == 1){
        i = game.inventorySelectedItem;
        while(1){
            if(--i < 0)i=CNT_MAX_INVENTORY_ITEMS-1;
            if(game.inventoryObjects[i].boolExist == 1){
                game.inventorySelectedItem = i;
                break;
            }
        }
    }
}

void opengl_walkTimer(int value){
    float prescaller = 0.025;
    if(game.keymap['w'] == 1){
        game.cameraPosition[0] += prescaller*cos(game.cameraRotYaw);
        game.cameraPosition[2] += prescaller*sin(game.cameraRotYaw);
    }else if(game.keymap['s'] == 1){
        game.cameraPosition[0] -= prescaller*cos(game.cameraRotYaw);
        game.cameraPosition[2] -= prescaller*sin(game.cameraRotYaw);
    }
    if(game.keymap['a'] == 1){
        game.cameraPosition[0] += prescaller*cos(game.cameraRotYaw-M_PI/2.0);
        game.cameraPosition[2] += prescaller*sin(game.cameraRotYaw-M_PI/2.0);
    }else if(game.keymap['d'] == 1){
        game.cameraPosition[0] -= prescaller*cos(game.cameraRotYaw-M_PI/2.0);
        game.cameraPosition[2] -= prescaller*sin(game.cameraRotYaw-M_PI/2.0);
    }
    if(game.keymap['*'] == 1){
        game.cameraPosition[1] -= prescaller;
    }

    //Lock camera constrol
    if(game.cameraPosition[0] < game.cameraXStart)
        game.cameraPosition[0] = game.cameraXStart;
    if(game.cameraPosition[0] > game.cameraXEnd)
        game.cameraPosition[0] = game.cameraXEnd;
    if(game.cameraPosition[2] < game.cameraZStart)
        game.cameraPosition[2] = game.cameraZStart;
    if(game.cameraPosition[2] > game.cameraZEnd)
        game.cameraPosition[2] = game.cameraZEnd;

    //Start this thread again after 10ms
    glutTimerFunc(10, opengl_walkTimer, 0);
}

void opnegl_physicsTimer(int value){
    game_physicsTick(&game);
    glutTimerFunc(10, opnegl_physicsTimer, 0);
}

/* this function is run by the second thread */
void *opengl_thread(void *argumentsPtr){
    arguments_t *arg = argumentsPtr;

    /* Main initialization */
    glutInit(&arg->argc, arg->argv);

    /* Render type */
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* GLUT Window size */
    glutInitWindowSize(600, 600);

    /* Create GLUT window with name */
    glutCreateWindow("LabEscape Author: Martin Krásl, martin.contact@email.cz");

    /* Register all handler functions */
    glutMotionFunc(opengl_mouseMotion);
    glutPassiveMotionFunc(opengl_mouseMotion);
    glutDisplayFunc(opengl_display);
    glutIdleFunc(opengl_display);
    glutReshapeFunc(opengl_reshape);
    glutKeyboardFunc(opengl_keyPress);
    glutKeyboardUpFunc(opengl_keyUnpress);
    glutMouseFunc(opnegl_mouseClick);
    ///glutPassiveMotionFunc(mouseMove);

    /* Set another initialize */
    opengl_init();

    /* Run walk thread */
    opengl_walkTimer(0);

    /* Run physics thread */
    opnegl_physicsTimer(0);

    /* Full screen ! */
    glutFullScreen();

    /* Run main GLUT loop */
    glutMainLoop();

    /* Memory free */
    game_free(&game);

    /* Return null */
    return NULL;
}
