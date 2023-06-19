#include "M3DReader.h"

#define CNT_CLICK_EVENTS 32
#define CNT_MAX_INVENTORY_ITEMS 12

typedef struct{
    char *name;
    uint8_t boolExist;
} inventoryObject_t;

typedef struct{
    char *name;
    float X;
    float Y;
    float Z;
    float radius;
} clickEvent_t;

typedef struct{
    /* Elementary data for OPENGL engine */
    //Game map
    M3DFILE_t gameMap;
    //Camera
    float cameraRotYaw;
    float cameraRotPitch;
    float cameraRotation[3];
    float cameraPosition[3];
    uint8_t cursorNotWarped;
    //Event text
    char *infoText;
    int16_t infoTextTimer;
    //Camera lock area
    float cameraXStart, cameraXEnd;
    float cameraZStart, cameraZEnd;
    //Light
    float light0_color[4];
    float light0_position[3];
    //Keyboard
    uint8_t keymap[256];
    //Inventory register
    inventoryObject_t inventoryObjects[CNT_MAX_INVENTORY_ITEMS];
    uint8_t inventorySelectedItem;
    //Click event register
    clickEvent_t clickEvents[CNT_CLICK_EVENTS];



    /* Game data */
    //Animations
    float speed1, speed2;
    int16_t timer1, timer2;
    int32_t mouseCounter;
    float mouseVector1, mouseVector2;
    //Codes
    uint8_t doorCode[4];
    uint8_t actualDoorCode[4];
    float safeCodeColors[6*4];
    uint8_t actualSafeCode[4];
    uint8_t safeCode[4];
    //Grabbed objects
    uint8_t grabbedHose, hoseAdded, grabbedSubstance, grabbedScrewdriver; //Standart
    uint8_t grabbedMatches, grabbedKadinka, grabbedPaperC;
    uint8_t grabbedKey, grabbedVR1, grabbedVR2;
    uint8_t grabbedPapers, grabbedTape, grabbedMouse;
    //Opening cabinet
    uint8_t stateDoorA_1, stateDoorB_1, stateDoorC_1, stateDoorD_1, safeDoorCorrect, mainDoorCorrect;
    //Steps
    uint8_t stepsKadinka, waterAnim, fireTimer, stepsLadder, transportSystem, stepsHacking, cursorTimer;
} gameLabEscape_t;

void game_init(gameLabEscape_t *game, char *gameMapFileName);
void game_free(gameLabEscape_t *game);

void game_physicsTick(gameLabEscape_t *game);
void game_objectClick(gameLabEscape_t *game, float distance, char *name);
