#ifndef BATTLESHIPAI_H
#define BATTLESHIPAI_H

#include "playGame.h"

int getAImove(char b[ROWS][COLS], int level);


//easy AI
int hal(char b[ROWS][COLS]);

//standard AI
int wopr(char b[ROWS][COLS]);

//difficult
int viki(char b[ROWS][COLS]);

//impossible
int skynet(char b[ROWS][COLS]);
void skynet_online(char b[ROWS][COLS]);

//general functions
//returns target if it finds one, else -1
int target(char b[ROWS][COLS]);
int attackPos(char b[ROWS][COLS], int attack);

#endif// BATTLESHIPAI_H
