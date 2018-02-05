/*contains all the functions involved with playing a game of battleship*/

#ifndef PLAYGAME_H
#define PLAYGAME_H

//needed libraries:
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#ifdef linux //for CHRISTIAN'S computer
    #define CLS     system("clear");
    #define PAUSE   printf("Continue...");getchar();
    #define FLUSH   while(getchar() != '\n'){;}
#else  //for everyone else...
    #define CLS     system("cls");
    #define PAUSE   system("pause");
    #define FLUSH   system("flush");
#endif //switch for linux

#define ROWS 10
#define COLS 10


#include "battleshipAI.h"



/*creates and runs a new game*/
void playGame(char fileName[], int AIlevel);//(char *fileName, int AIlevel);

//setup game
void initializeShips(int ships[]);
//functions in playGame
void displayBoard(char b[ROWS][COLS], char player[]);
int getPlayerMove(char b[ROWS][COLS]);


int updateGame(char b[ROWS][COLS], int ships[], int attack);
void removeSunkenShips(char b[ROWS][COLS], int ships[]);
int checkIfWon(char b[ROWS][COLS]);
void getPlayerScore(char b[ROWS][COLS], int AIlevel, int win);

//save game to file
void writeGameToFile(int pS[17], char pB[ROWS][COLS], int cS[17], char cB[ROWS][COLS]);
void readGameFile(char fileName[], int pS[17], char pB[ROWS][COLS], int cS[17], char cB[ROWS][COLS]);


#endif//PLAYGAME_H


