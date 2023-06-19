#include "game.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>

typedef struct{
    char *name;
    float X;
    float Y;
    float Z;
} originSet_t;

int8_t finInvIt(gameLabEscape_t *game, char *name){
    int8_t i;
    for(i=0; i<CNT_MAX_INVENTORY_ITEMS; ++i){
        if(strcmp(name, game->inventoryObjects[i].name) == 0)
            return i;
    }
    fprintf(stderr, "ERROR: Inventory item named '%s' not found !\n", name);
    exit(100);
}

void game_registerClickEvents(gameLabEscape_t *game){
    int8_t i;
    clickEvent_t tmpDat[CNT_CLICK_EVENTS] = {
                                                {"Door1",1.88797,0.389975,1.8648,0.129},
                                                {"Door2",1.74827,0.389975,1.8648,0.129},
                                                {"Door3",1.88797,0.389975,1.72434,0.129},
                                                {"Door4",1.74827,0.389975,1.72434,0.129},
                                                {"Escape",2.77518,-1.29695,0.725539,2.387},
                                                {"Safe1",3.195,2.40131,0.989651,0.120},
                                                {"Safe2",3.195,2.28225,0.989651,0.120},
                                                {"Safe3",3.195,2.16101,0.989651,0.120},
                                                {"Safe4",3.195,2.0423,0.989651,0.120},
                                                {"SafePapers",3.65299,2.24285,0.647523,0.407},
                                                {"SafeKey",3.35826,2.45627,0.647523,0.218},
                                                {"GasOpen",3.97996,3.18321,1.64326,0.100},
                                                {"GasOutlet",4.01524,3.27484,1.65765,0.150},
                                                {"WaterFloor",3.62974,4.73874,0.741452,0.471},
                                                {"WaterOpen",3.90443,4.74,1.29845,0.120},
                                                {"Substance",1.87547,6.86951,1.66184,0.527},
                                                {"HoseC",1.30104,6.95301,2.03291,0.450},
                                                {"ShelfC",0.164519,5.69704,0.603485,0.205},
                                                {"VR1",0.034266,5.62536,0.089272,0.160},
                                                {"VR2",0.034266,6.52535,0.089272,0.160},
                                                {"Ladder",-0.415734,6.13677,-0.053684,0.997},
                                                {"FireOn",-0.333862,4.73419,1.17012,0.350},
                                                {"FireStand",-0.374511,4.73419,1.45844,0.189413},
                                                {"ScrewdriverC",-0.302,3.78409,1.03832,0.300735},
                                                {"ShelfOpen",-0.544149,3.08,1.76249,0.170},
                                                {"Matches",-0.74762,3.17778,1.51419,0.225012},
                                                {"KadinkaC",-0.766883,3.47559,1.86668,0.209981},
                                                {"PaperC",-0.483552,1.87673,1.82279,0.25032},
                                                {"PaperOpen",-0.159813,1.55357,1.2288,0.2},
                                                {"ComputerC",2.93319,4.54436,-2.47155,0.50951},
                                                {"TapeC",-1.01055,0.629377,-1.70537,0.991976},
                                                {"MouseC",-3.89196,3.35599,-2.05648,1.07965}
                                            };
    for(i=0; i<CNT_CLICK_EVENTS; ++i){
        game->clickEvents[i].name = tmpDat[i].name;
        game->clickEvents[i].X = tmpDat[i].X;
        game->clickEvents[i].Y = tmpDat[i].Z;
        game->clickEvents[i].Z = -tmpDat[i].Y;
        game->clickEvents[i].radius = tmpDat[i].radius/2.0;
    }
}

void game_registerInventory(gameLabEscape_t *game){
    int8_t i;
    game->inventorySelectedItem = 0;
    char *tmpDat_[CNT_MAX_INVENTORY_ITEMS] = {
        "+", "+Hose", "+Reagent", "+Suspicious paper", "+Screwdriver", "+Beaker",
        "+Matches", "+Beaker with solution", "+Key", "+Documents - EVIDENCE", "+Data tape - EVIDENCE", "+Modified mouse - EVIDENCE"
    };
    for(i=0; i<CNT_MAX_INVENTORY_ITEMS; ++i){
        game->inventoryObjects[i].name = tmpDat_[i];
        game->inventoryObjects[i].boolExist = 0;
    }
    game->inventoryObjects[0].boolExist = 1;
}

void game_setObjectOrigins(gameLabEscape_t *game){
    int8_t i;
    originSet_t origins[32] =    {
                                    {"Click1",1.88797,0.404905,1.8648},{"Click2",1.74797,0.404905,1.8648},
                                    {"Click3",1.88797,0.404905,1.7248},{"Click4",1.74797,0.404905,1.7248},
                                    {"DoorA_1",-0.197333,2.07333,0.084},{"DoorA_2",-0.197333,2.07333,0.084},
                                    {"DoorB_1",-0.584833,3.59,1.49249},{"DoorB_2",-0.584833,3.59,1.49249},
                                    {"DoorC_1",0.138033,6.63139,0.058886},{"DoorC_2",-0.415734,5.58215,0.09942},
                                    {"DoorD_1",3.98158,3.38643,1.64326},{"DoorD_2",3.98158,3.38643,1.64326},
                                    {"DoorE_0",3.195,1.96225,0.991523},{"DoorE_1",3.195,1.96225,0.991523},
                                    {"DoorE_2",3.195,1.96225,0.991523},{"DoorE_3",3.195,1.96225,0.991523},
                                    {"Door_1",3.59517,0.310058,0.288239},{"Door_2",3.59517,0.310058,0.288239},
                                    {"Door_3",3.59517,0.310058,0.288239},{"TapeA_1",2.61445,0.656377,-1.70537},
                                    {"TapeA_2",2.61445,0.656377,-1.70537},{"TapeB_1",1.96445,0.656377,-1.70537},
                                    {"TapeB_2",1.96445,0.656377,-1.70537},{"TapeC_1",0.914446,0.656377,-1.70537},
                                    {"TapeC_2",0.914446,0.656377,-1.70537},{"TapeD_1",0.264446,0.656377,-1.70537},
                                    {"TapeD_2",0.264446,0.656377,-1.70537},{"DoorE_4",3.195,1.96225,0.991523},
                                    {"MouseBody", -3.82523, 3.42511, -2.20357}, {"MouseEyes", -3.82523, 3.42511, -2.20357},
                                    {"MouseBody.001", -0.204294, 6.20296, 1.20776}, {"MouseEyes.001", -0.204294, 6.20296, 1.20776}
                                };
    float *p;
    for(i=0; i<32; ++i){
        p = M3D_gOriXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, origins[i].name));
        p[0] = origins[i].X;
        p[1] = origins[i].Z;
        p[2] = -origins[i].Y;
    }
}

void game_hideObjects(gameLabEscape_t *game){
    int8_t i;
    float *p;
    char *hide_names[35] =    {
                                "Kadinka","WaterModify2","FireModel","Paper2",
                                "pr1","pr2","pr3","pr4","Hose2","Trash2",
                                "WaterAnim","WaterModify","Kadinka2","Water2",
                                "Cursor", "Text1", "Text2", "Text3",
                                "TextA", "TextB", "TextC", "TextD",
                                "TextA.001", "TextB.001", "TextC.001", "TextD.001",
                                "TextA.002", "TextB.002", "TextC.002", "TextD.002",
                                "TextA.003", "TextB.003", "TextC.003", "TextD.003", "EndText"
                            };
    for(i=0; i<35; ++i){
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, hide_names[i]));
        p[3] = 0.0;
    }
}

void game_showComputerCode(gameLabEscape_t *game){
    float *p;
    if(game->doorCode[0]==0){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextA"));p[3]=2.0;}
    if(game->doorCode[0]==1){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextB"));p[3]=2.0;}
    if(game->doorCode[0]==2){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextC"));p[3]=2.0;}
    if(game->doorCode[0]==3){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextD"));p[3]=2.0;}
    if(game->doorCode[1]==0){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextA.001"));p[3]=2.0;}
    if(game->doorCode[1]==1){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextB.001"));p[3]=2.0;}
    if(game->doorCode[1]==2){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextC.001"));p[3]=2.0;}
    if(game->doorCode[1]==3){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextD.001"));p[3]=2.0;}
    if(game->doorCode[2]==0){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextA.002"));p[3]=2.0;}
    if(game->doorCode[2]==1){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextB.002"));p[3]=2.0;}
    if(game->doorCode[2]==2){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextC.002"));p[3]=2.0;}
    if(game->doorCode[2]==3){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextD.002"));p[3]=2.0;}
    if(game->doorCode[3]==0){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextA.003"));p[3]=2.0;}
    if(game->doorCode[3]==1){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextB.003"));p[3]=2.0;}
    if(game->doorCode[3]==2){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextC.003"));p[3]=2.0;}
    if(game->doorCode[3]==3){p=M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "TextD.003"));p[3]=2.0;}
}

void game_gameOwer(gameLabEscape_t *game){
    if(game->grabbedPapers==1 && game->grabbedTape==1 && game->grabbedMouse==1){
        game->infoTextTimer = -1200;
    }else{
        game->infoTextTimer = 1200;
    }
}

void game_checkDoors(gameLabEscape_t *game){
    float *p;
    int8_t i, a=0;
    for(i=0; i<4; ++i){
        if(game->doorCode[i] != game->actualDoorCode[i])a=1;
    }
    if(a!=1){
        game->mainDoorCorrect = 2;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "LockShaft"));
        p[3] = 0.0;
    }
}

void game_safeCodeShow(gameLabEscape_t *game){
    float *p;
    p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorE_1"));
    p[0] = game->safeCodeColors[game->actualSafeCode[0]*4+0];
    p[1] = game->safeCodeColors[game->actualSafeCode[0]*4+1];
    p[2] = game->safeCodeColors[game->actualSafeCode[0]*4+2];
    p[3] = game->safeCodeColors[game->actualSafeCode[0]*4+3];
    p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorE_2"));
    p[0] = game->safeCodeColors[game->actualSafeCode[1]*4+0];
    p[1] = game->safeCodeColors[game->actualSafeCode[1]*4+1];
    p[2] = game->safeCodeColors[game->actualSafeCode[1]*4+2];
    p[3] = game->safeCodeColors[game->actualSafeCode[1]*4+3];
    p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorE_3"));
    p[0] = game->safeCodeColors[game->actualSafeCode[2]*4+0];
    p[1] = game->safeCodeColors[game->actualSafeCode[2]*4+1];
    p[2] = game->safeCodeColors[game->actualSafeCode[2]*4+2];
    p[3] = game->safeCodeColors[game->actualSafeCode[2]*4+3];
    p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorE_4"));
    p[0] = game->safeCodeColors[game->actualSafeCode[3]*4+0];
    p[1] = game->safeCodeColors[game->actualSafeCode[3]*4+1];
    p[2] = game->safeCodeColors[game->actualSafeCode[3]*4+2];
    p[3] = game->safeCodeColors[game->actualSafeCode[3]*4+3];

    int i, a;
    a = 0;
    for(i=0; i<4; ++i){
        if(game->safeCode[i] != game->actualSafeCode[i])a=1;
    }
    if(a==0){
        game->safeDoorCorrect = 1;
    }
}

void game_generateCodes(gameLabEscape_t *game){
    int8_t i;
    float *p;
    float tmp[6*4] = {1.0, 1.0, 1.0, 1.0,
                      1.0, 0.0, 0.0, 1.0,
                      0.0, 1.0, 0.0, 1.0,
                      0.0, 0.0, 1.0, 1.0,
                      1.0, 1.0, 0.0, 1.0,
                      0.0, 1.0, 1.0, 1.0};
    for(i=0; i<6*4; ++i){
        game->safeCodeColors[i] = tmp[i];
    }
    srand(time(NULL));
    for(i=0; i<4; ++i){
        game->doorCode[i] = rand()%4;
        game->safeCode[i] = rand()%6;
        game->actualSafeCode[i] = 0;
        game->actualDoorCode[i] = 0;
    }
    p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "pr1"));
    p[0] = game->safeCodeColors[game->safeCode[0]*4+0];
    p[1] = game->safeCodeColors[game->safeCode[0]*4+1];
    p[2] = game->safeCodeColors[game->safeCode[0]*4+2];
    //p[3] = game->safeCodeColors[game->safeCode[0]*4+3];
    p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "pr2"));
    p[0] = game->safeCodeColors[game->safeCode[1]*4+0];
    p[1] = game->safeCodeColors[game->safeCode[1]*4+1];
    p[2] = game->safeCodeColors[game->safeCode[1]*4+2];
    //p[3] = game->safeCodeColors[game->safeCode[1]*4+3];
    p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "pr3"));
    p[0] = game->safeCodeColors[game->safeCode[2]*4+0];
    p[1] = game->safeCodeColors[game->safeCode[2]*4+1];
    p[2] = game->safeCodeColors[game->safeCode[2]*4+2];
    //p[3] = game->safeCodeColors[game->safeCode[2]*4+3];
    p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "pr4"));
    p[0] = game->safeCodeColors[game->safeCode[3]*4+0];
    p[1] = game->safeCodeColors[game->safeCode[3]*4+1];
    p[2] = game->safeCodeColors[game->safeCode[3]*4+2];
    //p[3] = game->safeCodeColors[game->safeCode[3]*4+3];
    game_safeCodeShow(game);
}

void game_init(gameLabEscape_t *game, char *gameMapFileName){
    uint8_t i;

    /* Initial elementary data for OPENGL engine */
    for(i=0; i<3; ++i){
        game->cameraPosition[i] = 0.0;
        game->cameraRotation[i] = 0.0;
    }
    // Rotation
    game->cameraRotPitch = -0.5;
    game->cameraRotYaw = -3.2;
    // Position
    game->cameraPosition[0] = 1.1826;
    game->cameraPosition[1] = 1.8;
    game->cameraPosition[2] = -0.626849;
    // Camera lock area
    game->cameraXStart = 0.3;
    game->cameraXEnd = 3.0;
    game->cameraZStart = -5.92;
    game->cameraZEnd = -0.6;
    /*game->cameraXStart = -2.73;
    game->cameraXEnd = 2.3;
    game->cameraZStart = -6.082;
    game->cameraZEnd = -1.09;*/
    // Light 0 color
    game->light0_color[0] = 1.0;
    game->light0_color[1] = 1.0;
    game->light0_color[2] = 1.0;
    game->light0_color[3] = 1.0;
    // Light 0 position
    game->light0_position[0] = 1.0;
    game->light0_position[1] = 1.0;
    game->light0_position[2] = 1.0;
    // Info
    game->infoText = "";
    game->infoTextTimer = 0;
    // Game map load
    if(M3D_load(&game->gameMap, gameMapFileName)){
        fprintf(stderr, "ERROR: No game map file '%s' !\n", gameMapFileName);
        exit(100);
    }
    //Keyboard
    for(i=0; i<=254; ++i){
        game->keymap[i] = 0;
    }

    //grabbedData
    game->grabbedHose = 0;
    game->grabbedSubstance = 0;
    game->grabbedScrewdriver = 0;
    //opening cabinet
    game->stateDoorA_1 = 0;
    game->stateDoorB_1 = 0;
    game->stateDoorC_1 = 0;
    game->stateDoorD_1 = 0;
    game->safeDoorCorrect = 0;
    game->mainDoorCorrect = 0;
    game->grabbedMatches = 0;
    game->grabbedKadinka = 0;
    game->grabbedPaperC = 0;
    game->grabbedKey = 0;
    game->grabbedPapers = 0;
    game->grabbedVR1 = 0;
    game->grabbedVR2 = 0;
    game->stepsKadinka = 0;
    game->waterAnim = 0;
    game->fireTimer = 0;
    game->stepsLadder = 0;
    game->transportSystem = 0;
    game->stepsHacking = 0;
    game->cursorTimer = 50;
    game->grabbedTape = 0;
    game->grabbedMouse = 0;
    game->hoseAdded = 0;
    //Animations
    game->speed1 = 0.0;
    game->speed2 = 0.0;
    game->timer1 = 10;
    game->timer2 = 10;
    game->mouseVector1 = 0.0;
    game->mouseVector2 = 0.0;

    game_registerInventory(game);
    game_registerClickEvents(game);
    game_setObjectOrigins(game);
    game_hideObjects(game);
    game_generateCodes(game);
}

void game_free(gameLabEscape_t *game){
    M3D_free(&game->gameMap);
}

void game_physicsTick(gameLabEscape_t *game){
    float *p;
    //Static
    if(--game->timer1 <= 0.0){
        game->speed1 = (rand()%1000)/100.0-5.0;
        game->timer1 = rand()%300;
    }
    p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "TapeA_1"));p[2] += game->speed1;
    p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "TapeA_2"));p[2] += game->speed1;
    p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "TapeB_1"));p[2] += game->speed1;
    p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "TapeB_2"));p[2] += game->speed1;
    if(--game->timer2 <= 0.0){
        game->speed2 = (rand()%1000)/100.0-5.0;
        game->timer2 = rand()%300;
        //And also Mouse!
        game->mouseVector1 = (rand()%200)/100.0-1.0;
        game->mouseVector2 = (rand()%200)/100.0-1.0;
    }
    p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "TapeC_1"));p[2] += game->speed2;
    p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "TapeC_2"));p[2] += game->speed2;
    p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "TapeD_1"));p[2] += game->speed2;
    p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "TapeD_2"));p[2] += game->speed2;
    if(++game->mouseCounter >= 10000.0){
        game->mouseCounter = 0;
    }
    //Static Mouse
    p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "MouseBody"));
    p[0] = sin(game->mouseCounter/5.0);
    p[1] = cos(game->mouseCounter/7.0);
    p[2] = sin(game->mouseCounter/6.0+1.5);
    p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "MouseBody"));
    p[1] += game->mouseVector1*5.0;
    p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "MouseEyes"));
    p[1] += game->mouseVector1*5.0;
    p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "MouseBody.001"));
    p[1] += game->mouseVector1*5.0;
    p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "MouseEyes.001"));
    p[1] += game->mouseVector1*5.0;



    //Dynamic
    if(game->stateDoorA_1==1){
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorA_1"));
        p[1] += 4.0;
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorA_2"));
        p[1] += 4.0;
        if(p[1] >= 114.0){
            game->stateDoorA_1 = 2;
        }
    }
    if(game->stateDoorB_1==1){
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorB_1"));
        p[1] += 4.0;
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorB_2"));
        p[1] += 4.0;
        if(p[1] >= 80.0){
            game->stateDoorB_1 = 2;
        }
    }if(game->stateDoorD_1==1){
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorD_1"));
        p[1] -= 4.0;
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorD_2"));
        p[1] -= 4.0;
        if(p[1] <= -90.0){
            game->stateDoorD_1 = 2;
        }
    }if(game->waterAnim > 0){
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "WaterAnim"));
        p[3] = 2.0;
        --game->waterAnim;
        if(game->stepsKadinka == 1)++game->stepsKadinka;
    }else{
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "WaterAnim"));
        p[3] = 0.0;
        if(game->stepsKadinka == 2){
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Water2"));
            p[3] = 2.0;
            ++game->stepsKadinka;
        }
    }if(game->fireTimer > 0){
        --game->fireTimer;
    }else{
        if(game->stepsKadinka==12){
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "pr1"));p[3] = 2.0;
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "pr2"));p[3] = 2.0;
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "pr3"));p[3] = 2.0;
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "pr4"));p[3] = 2.0;
        }
    }if(game->safeDoorCorrect==1){
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorE_0"));
        p[1] += 4.0;
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorE_1"));
        p[1] += 4.0;
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorE_2"));
        p[1] += 4.0;
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorE_3"));
        p[1] += 4.0;
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorE_4"));
        p[1] += 4.0;
        if(p[1] >= 90.0){
            game->safeDoorCorrect = 2;
        }
    }if(game->stateDoorC_1==1){
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorC_1"));
        p[1] += 4.0;
        if(p[1] >= 90.0){
            game->stateDoorC_1 = 2;
        }
    }if(game->stepsLadder==2){
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "DoorC_2"));
        p[0] += 4.0;
        if(p[0] >= 89.0){
            game->stepsLadder = 3;
            game->transportSystem = 2;
        }
    }if(game->stepsHacking==0){
        if(game->cursorTimer > 25){
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Cursor"));
            p[3] = 2.0;
        }else if(game->cursorTimer > 0){
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Cursor"));
            p[3] = 0.0;
        }
        if(--game->cursorTimer==0)game->cursorTimer=50;
    }if(game->mainDoorCorrect == 2){
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "Door_1"));
        p[1] -= 1.0;
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "Door_2"));
        p[1] -= 1.0;
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "Door_3"));
        p[1] -= 1.0;
        if(p[1] <= -90.0){
            game->mainDoorCorrect = 3;
        }
    }if(game->infoTextTimer > 0){
        game->inventorySelectedItem = 0;
        game->infoText = "}You do not have all three of EVIDENCE!";
        --game->infoTextTimer;
    }else if(game->infoTextTimer < 0){
        game->inventorySelectedItem = 0;
        game->infoText = "}###Congratulations! Game over!###";
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "EndText"));
        p[3] = 2.0;
        if(++game->infoTextTimer > -5)exit(0);
    }else{
        game->infoText = "";
    }
}

void game_objectClick(gameLabEscape_t *game, float distance, char *name){
    float *p;
    if(strcmp(name, "HoseC")==0 && game->grabbedHose==0){
        game->inventoryObjects[finInvIt(game, "+Hose")].boolExist = 1;
        game->inventorySelectedItem = finInvIt(game, "+Hose");
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Hose"));
        p[3] = 0.0;
        game->grabbedHose = 1;
    }else if(strcmp(name, "Substance")==0 && game->grabbedSubstance==0){
        game->inventoryObjects[finInvIt(game, "+Reagent")].boolExist = 1;
        game->inventorySelectedItem = finInvIt(game, "+Reagent");
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Sub1"));
        p[3] = 0.0;
        game->grabbedSubstance = 1;
    }else if(strcmp(name, "ScrewdriverC")==0 && game->grabbedScrewdriver==0){
        game->inventoryObjects[finInvIt(game, "+Screwdriver")].boolExist = 1;
        game->inventorySelectedItem = finInvIt(game, "+Screwdriver");
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Screw"));
        p[3] = 0.0;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Screw.001"));
        p[3] = 0.0;
        game->grabbedScrewdriver = 1;
    }else if(strcmp(name, "PaperOpen")==0 && game->stateDoorA_1==0){
        game->stateDoorA_1 = 1;//Start opening
    }else if(strcmp(name, "ShelfOpen")==0 && game->stateDoorB_1==0){
        game->stateDoorB_1 = 1;//Start opening
    }else if(strcmp(name, "Matches")==0 && game->stateDoorB_1==2 && game->grabbedMatches==0){
        game->inventoryObjects[finInvIt(game, "+Matches")].boolExist = 1;
        game->inventorySelectedItem = finInvIt(game, "+Matches");
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "kr1"));p[3] = 0.0;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "kr2"));p[3] = 0.0;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "kr3"));p[3] = 0.0;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "kr4"));p[3] = 0.0;
        game->grabbedMatches = 1;
    }else if(strcmp(name, "KadinkaC")==0 && game->stateDoorB_1==2 && game->grabbedKadinka==0){
        game->inventoryObjects[finInvIt(game, "+Beaker")].boolExist = 1;
        game->inventorySelectedItem = finInvIt(game, "+Beaker");
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Kadinka1"));p[3] = 0.0;
        game->grabbedKadinka = 1;
    }else if(strcmp(name, "PaperC")==0 && game->stateDoorA_1==2 && game->grabbedPaperC==0){
        game->inventoryObjects[finInvIt(game, "+Suspicious paper")].boolExist = 1;
        game->inventorySelectedItem = finInvIt(game, "+Suspicious paper");
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Paper"));p[3] = 0.0;
        game->grabbedPaperC = 1;
    }else if(strcmp(name, "GasOpen")==0 && game->inventorySelectedItem==finInvIt(game, "+Screwdriver")){
        game->stateDoorD_1 = 1;
    }else if(strcmp(name, "WaterFloor")==0 && game->inventorySelectedItem==finInvIt(game, "+Beaker")){
        game->inventorySelectedItem = 0;
        game->inventoryObjects[finInvIt(game, "+Beaker")].boolExist = 0;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Kadinka2"));p[3] = 2.0;
        game->stepsKadinka = 1;
    }else if(strcmp(name, "WaterOpen")==0){
        game->waterAnim = 100;
    }else if(strcmp(name, "WaterFloor")==0 && game->inventorySelectedItem==finInvIt(game, "+Reagent") && game->stepsKadinka==3){
        game->stepsKadinka = 4;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "WaterModify"));p[3] = 2.0;
        game->inventorySelectedItem = 0;
        game->inventoryObjects[finInvIt(game, "+Reagent")].boolExist = 0;
    }else if(strcmp(name, "WaterFloor")==0 && game->stepsKadinka==4){
        game->inventoryObjects[finInvIt(game, "+Beaker with solution")].boolExist = 1;
        game->inventorySelectedItem = finInvIt(game, "+Beaker with solution");
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Kadinka2"));p[3] = 0.0;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "WaterModify"));p[3] = 0.0;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Water2"));p[3] = 0.0;
        game->stepsKadinka = 5;
    }else if(strcmp(name, "GasOutlet")==0 && game->inventorySelectedItem==finInvIt(game, "+Hose") && game->stateDoorD_1==2){
        game->inventorySelectedItem = 0;
        game->inventoryObjects[finInvIt(game, "+Hose")].boolExist = 0;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Hose2"));p[3] = 2.0;
        game->hoseAdded = 1;
    }else if(strcmp(name, "FireStand")==0 && game->inventorySelectedItem==finInvIt(game, "+Beaker with solution")){
        game->inventorySelectedItem = 0;
        game->inventoryObjects[finInvIt(game, "+Beaker with solution")].boolExist = 0;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "WaterModify2"));p[3] = 2.0;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Kadinka"));p[3] = 2.0;
        game->stepsKadinka = 10;
    }else if(strcmp(name, "FireStand")==0 && game->inventorySelectedItem==finInvIt(game, "+Suspicious paper") && game->stepsKadinka==10){
        game->inventorySelectedItem = 0;
        game->inventoryObjects[finInvIt(game, "+Suspicious paper")].boolExist = 0;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Paper2"));p[3] = 2.0;
        game->stepsKadinka = 11;
    }else if(strcmp(name, "FireOn")==0 && game->inventorySelectedItem==finInvIt(game, "+Matches") && game->stepsKadinka==11 && game->hoseAdded==1){
        game->fireTimer = 200;
        game->stepsKadinka = 12;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "FireModel"));p[3] = 2.0;
    }
    /**/
    if(strcmp(name, "Safe1")==0){
        if(++game->actualSafeCode[0] >= 6)game->actualSafeCode[0] = 0;
        game_safeCodeShow(game);
    }else if(strcmp(name, "Safe2")==0){
        if(++game->actualSafeCode[1] >= 6)game->actualSafeCode[1] = 0;
        game_safeCodeShow(game);
    }else if(strcmp(name, "Safe3")==0){
        if(++game->actualSafeCode[2] >= 6)game->actualSafeCode[2] = 0;
        game_safeCodeShow(game);
    }else if(strcmp(name, "Safe4")==0){
        if(++game->actualSafeCode[3] >= 6)game->actualSafeCode[3] = 0;
        game_safeCodeShow(game);
    }else if(strcmp(name, "SafeKey")==0 && game->grabbedKey==0 && game->safeDoorCorrect==2){
        game->inventoryObjects[finInvIt(game, "+Key")].boolExist = 1;
        game->inventorySelectedItem = finInvIt(game, "+Key");
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Key"));
        p[3] = 0.0;
        game->grabbedKey = 1;
    }else if(strcmp(name, "SafePapers")==0 && game->grabbedPapers==0 && game->safeDoorCorrect==2){
        game->inventoryObjects[finInvIt(game, "+Documents - EVIDENCE")].boolExist = 1;
        game->inventorySelectedItem = finInvIt(game, "+Documents - EVIDENCE");
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Papers"));
        p[3] = 0.0;
        game->grabbedPapers = 1;
    }else if(strcmp(name, "ShelfC")==0 && game->inventorySelectedItem==finInvIt(game, "+Key")){
        game->inventorySelectedItem = 0;
        game->inventoryObjects[finInvIt(game, "+Key")].boolExist = 0;
        game->stateDoorC_1 = 1;
    }else if(strcmp(name, "VR1")==0 && game->inventorySelectedItem==finInvIt(game, "+Screwdriver") && game->grabbedVR1==0){
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "vr1"));
        p[3] = 0.0;
        game->grabbedVR1 = 1;
    }else if(strcmp(name, "VR2")==0 && game->inventorySelectedItem==finInvIt(game, "+Screwdriver") && game->grabbedVR2==0){
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "vr3"));
        p[3] = 0.0;
        game->grabbedVR2 = 1;
    }else if(strcmp(name, "Ladder")==0 && game->stateDoorC_1==2 && game->stepsLadder==0){
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Trash"));
        p[3] = 0.0;
        p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Trash2"));
        p[3] = 2.0;
        game->stepsLadder = 1;
    }else if(strcmp(name, "Ladder")==0 && game->stateDoorC_1==2 && game->stepsLadder==1 && game->grabbedVR1==1 && game->grabbedVR2==1){
        game->stepsLadder = 2;
    }else if((strcmp(name, "Ladder")==0 || strcmp(name, "ShelfC")==0) && game->transportSystem==1){
        //transport up
        game->cameraXStart = 0.3;
        game->cameraXEnd = 3.0;
        game->cameraZStart = -5.92;
        game->cameraZEnd = -0.6;
        game->cameraPosition[1] = 1.8;
        game->transportSystem = 2;
    }else if((strcmp(name, "Ladder")==0 || strcmp(name, "ShelfC")==0) && game->transportSystem==2){
        //transport down
        game->cameraXStart = -2.73;
        game->cameraXEnd = 2.3;
        game->cameraZStart = -6.082;
        game->cameraZEnd = -1.09;
        game->cameraPosition[1] = -1.5;
        game->transportSystem = 1;
    }
    /**/
    if(game->transportSystem==1){
        if(strcmp(name, "ComputerC")==0){
            ++game->stepsHacking;
            if(game->stepsHacking==1){
                p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Cursor"));
                p[3] = 0.0;
                p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Text1"));
                p[3] = 2.0;
            }else if(game->stepsHacking==2){
                p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Text2"));
                p[3] = 2.0;
            }else if(game->stepsHacking==3){
                p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Text3"));
                p[3] = 2.0;
                game_showComputerCode(game);
            }
        }else if(strcmp(name, "TapeC")==0 && game->grabbedTape==0){
            game->inventoryObjects[finInvIt(game, "+Data tape - EVIDENCE")].boolExist = 1;
            game->inventorySelectedItem = finInvIt(game, "+Data tape - EVIDENCE");
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Tape.002"));
            p[3] = 0.0;

            game->grabbedTape = 1;
        }else if(strcmp(name, "MouseC")==0 && game->grabbedMouse==0){
            game->inventoryObjects[finInvIt(game, "+Modified mouse - EVIDENCE")].boolExist = 1;
            game->inventorySelectedItem = finInvIt(game, "+Modified mouse - EVIDENCE");
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "MouseBody"));
            p[3] = 0.0;
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "MouseEyes"));
            p[3] = 0.0;
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Wires"));
            p[3] = 0.0;
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "MouseP"));
            p[3] = 0.0;
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Pit"));
            p[3] = 0.0;
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Pit.001"));
            p[3] = 0.0;
            p = M3D_gClrRGB(&game->gameMap, M3D_gObjInx(&game->gameMap, "Msk"));
            p[3] = 0.0;
            game->grabbedMouse = 1;
        }
    }
    /**/
    if(strcmp(name, "Door1")==0){
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "Click1"));
        p[2] += 90.0;
        if(++game->actualDoorCode[0] >= 4)game->actualDoorCode[0] = 0;
        game_checkDoors(game);
    }else if(strcmp(name, "Door2")==0){
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "Click2"));
        p[2] += 90.0;
        if(++game->actualDoorCode[1] >= 4)game->actualDoorCode[1] = 0;
        game_checkDoors(game);
    }else if(strcmp(name, "Door3")==0){
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "Click3"));
        p[2] += 90.0;
        if(++game->actualDoorCode[2] >= 4)game->actualDoorCode[2] = 0;
        game_checkDoors(game);
    }else if(strcmp(name, "Door4")==0){
        p = M3D_gRotXYZ(&game->gameMap, M3D_gObjInx(&game->gameMap, "Click4"));
        p[2] += 90.0;
        if(++game->actualDoorCode[3] >= 4)game->actualDoorCode[3] = 0;
        game_checkDoors(game);
    }else if((strcmp(name, "ExitC")==0 || strcmp(name, "Escape")==0) && game->mainDoorCorrect==3){
        game_gameOwer(game);
    }
}






