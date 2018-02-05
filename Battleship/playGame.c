#include "playGame.h"

/*
sets up game (reads from file or starts new game)
contains game loop:
*/
void playGame(char fileName[], int AIlevel){

    int continueGame =1;

    char playerBoard[ROWS][COLS]; //contains all computer strikes and player's sunken ships
    int playerShips[17] = {0}; // contains player ship locations

    char computerBoard[ROWS][COLS]; //contains all user strikes and computer's sunken ships
    int computerShips[17] = {0};


	if(strcmp(fileName, "NEW")){
		readGameFile(fileName, playerShips, playerBoard, computerShips, computerBoard);
	} else {

    	//initialize boards
    	int i, k;
    	for(i = 0; i < ROWS; i++)
    	    for(k = 0; k < COLS; k++){
    	        playerBoard[i][k] = ' ';
    	        computerBoard[i][k] = ' ';
    	    }

    	srand((unsigned)time(NULL));
    	initializeShips(playerShips);
    	initializeShips(computerShips);
	}

    /*//////////////////////////////////////////////////////////////////////////
    GAME LOOP
    *///////////////////////////////////////////////////////////////////////////
    while(continueGame){
        //display board
        CLS;
        displayBoard(computerBoard, "COMPUTER FLEET");
        displayBoard(playerBoard, "PLAYER FLEET");

        //get user input and validate
        int move = 0;
        move = getPlayerMove(computerBoard);

		if(move == -1){//save and continue
			writeGameToFile(playerShips, playerBoard, computerShips, computerBoard);
			continue;	//skip AI	
		}
		if(move == -2){//save and quit
			writeGameToFile(playerShips, playerBoard, computerShips, computerBoard);
			continueGame = 0; //end game
			continue;	//skip AI
		}
		if(move == -3){//quit
			continueGame = 0;
			continue;
		}

        //update game (user input)  returns 0 if unsucessful
        updateGame(computerBoard, computerShips, move);

        if(checkIfWon(computerBoard)){
            //get player score
            getPlayerScore(computerBoard, AIlevel, 1);
            printf("PLAYER WINS\n");
            continueGame = 0;
            FLUSH;	PAUSE;
            break;;//leave game loop
        }

        //determine AI move
        move = getAImove(playerBoard, AIlevel);

        updateGame(playerBoard, playerShips, move);

        if(checkIfWon(playerBoard)){
            printf("COMPUTER WINS\n");
			getPlayerScore(computerBoard, AIlevel, 0);
            continueGame = 0;
			FLUSH;	PAUSE;
            break;//leave game loop
        }
    }//end while
}//end playgame

/*reads all necessary information from file and loads it into relevent variables*/
void readGameFile(char fileName[], int pS[17], char pB[ROWS][COLS], int cS[17], char cB[ROWS][COLS]){
	FILE* fp = fopen(fileName, "r");
	if(!fp){
		printf("FILE COULD NOT BE OPENED\n\n");
		return;
	}

	int r, c, tmp;

	for(r = 0; r < 7; r++){
		fgetc(fp);
	}
	

	//Player ships
	for(r = 0; r < 17; r++){
		fscanf(fp, " %d", &tmp);
		pS[r] = tmp;	
	}
	fgetc(fp);
	fgetc(fp);
	fgetc(fp);

	//Player board
	for(r = 0; r < ROWS; r++){
		for(c = 0; c < COLS; c++)
			pB[r][c] = fgetc(fp);
		fgetc(fp);
	}
	fgetc(fp);
	fgetc(fp);

	for(r = 0; r < 9; r++)
		fgetc(fp);

	//Computer ships
	for(r = 0; r < 17; r++){
		fscanf(fp, " %d", &tmp);
		cS[r] =  tmp;
	}
	fgetc(fp);
	fgetc(fp);
	fgetc(fp);

	//Computer board
	for(r = 0; r < ROWS; r++){
		for(c = 0; c < COLS; c++)
			cB[r][c] = fgetc(fp);
		fgetc(fp);
	}

	fclose(fp);
	
}

/*stores all variables necessary to continue game where it was left off*/
void writeGameToFile(int pS[17], char pB[ROWS][COLS], int cS[17], char cB[ROWS][COLS]){

	char name[] = "saved_game.txt";
	FILE* fp = fopen(name, "w");
	if(!fp){
		printf("FILE COULD NOT BE OPENED\n\n");
		return;
	}

	int r, c;

	//Player ships
	fprintf(fp, "PLAYER\n");
	for(r = 0; r < 17; r++){
		fprintf(fp, "%d ", pS[r]);
		if(r == 4 || r == 8 || r == 11 || r == 14)
			fputc('\n', fp);
	}
	fputc('\n', fp);
	fputc('\n', fp);

	//Player board
	for(r = 0; r < ROWS; r++){
		for(c = 0; c < COLS; c++)
			fputc(pB[r][c], fp);
		fputc('\n', fp);
	}
	fputc('\n', fp);
	fputc('\n', fp);


	//Computer ships
	fprintf(fp, "COMPUTER\n");
	for(r = 0; r < 17; r++){
		fprintf(fp, "%d ", cS[r]);
		if(r == 4 || r == 8 || r == 11 || r == 14)
			fputc('\n', fp);
	}
	fputc('\n', fp);
	fputc('\n', fp);

	//Computer board
	for(r = 0; r < ROWS; r++){
		for(c = 0; c < COLS; c++)
			fputc(cB[r][c], fp);
		fputc('\n', fp);
	}
	fputc('\n', fp);
	fputc('\n', fp);

	fclose(fp);
}

/*calculates player score based on accuracy and AI level...
then appends it to highscore file*/
void getPlayerScore(char b[ROWS][COLS],int AIlevel, int win){
    //need score = (hits / shots)*100 + AI_LEVEL^2 + AI_LEVEL * AI_SHIPS_LOST;
    int score, hits = 0, shots = 0;
    int r, c;
    for(r = 0; r < ROWS; r++)
        for(c = 0; c < COLS; c++){
            if(b[r][c] != ' '){
                shots++;
                if(b[r][c] != 'O')
                    hits++;
            }
        }
    printf("hits: %d, misses: %d, AI %d\n", hits, shots-hits, AIlevel);
    score = (hits * 100) / shots + AIlevel * AIlevel + (AIlevel * hits) / 3 + AIlevel * win;
    printf("Your Score: %d\n", score);

	//write score to file
	char name[50] = {'\0'};
	printf("Name: ");
	scanf("%s", &name);
	FILE* fp = fopen("highscores.txt", "a");
	fprintf(fp, "%s: Score: %d AI level: %d\n", name, score, AIlevel);
	fclose(fp);
}

void displayBoard(char b[ROWS][COLS], char player[]){
    printf("\t\t%s\n", player);
    printf("    A   B   C   D   E   F   G   H   I   J\n");
    printf("  +---+---+---+---+---+---+---+---+---+---+\n");

    int i, k;
    for(i = 0; i < ROWS; i++){
        printf("%d |", i);
        for(k = 0; k < COLS; k++)
            printf(" %c |", b[i][k]);
        printf("\n  +---+---+---+---+---+---+---+---+---+---+\n");
    }
    printf("\n\n");
}

/*ask for row and col for next attack and check if valid...
if player enters 
'q'	ask if player wants to save, no: return -3, yes: return -2
's' save	return -1
*/
int getPlayerMove(char b[ROWS][COLS]){
    int row;
    char col;
    int valid = 0;

    while(!valid){
        printf("Enter Target:\nCOLUMN \tA...J >>> ");
        scanf(" %c", &col);

        //check if player wants to quit game or save game
		if(col == 'q' || col == 'Q'){
			printf("\nAre you sure you want to quit? (y/n): ");
			scanf(" %c", &col);
			if(col != 'y' && col != 'Y')
				continue;
			else{
				printf("Do you want to save the game? (y/n): ");
				scanf(" %c", &col);
				if(col == 'y' || col == 'Y')
					return -2;
				return -3;
			}
		}//end Quit
		
		if(col == 's' || col == 'S'){
			return -1;
		}

        printf("ROW \t0...9 >>> ");
        scanf(" %d", &row);

        //check if selection is in board
        if((int) col >= 65 && (int) col <=74) //'A' is 65: 'J' is 74
            if(row >= 0 && row <= 9)
                valid = 1;
        if(!valid){
            printf("INVALID INPUT: TRY AGAIN\n\n");
            FLUSH;
            PAUSE;
            continue;
        }

        //check if board cell is occupied
        if(b[row][(int) col - 65] != ' '){
            printf("CELL ALREADY ATTACKED: TRY AGAIN\n\n");
            valid = 0;
            FLUSH;
            PAUSE;
        }
        else
            valid = 1;
    }

    return (row * 10 + ((int) col) - 65);
}//end getPlayerMove

//given board, ships, and attack: if board[attack] is ' ', check if any ships at location
int updateGame(char b[ROWS][COLS], int ships[], int attack){ //return 1 if sucessful, 0 otherwise
	if(attack < 0)
		return 0;

    int r = (attack / 10) % 10;//incase attack is greater than 100... avoid array out of bounds
    int c = attack % 10;

    if(b[r][c] == ' '){//if no previous attack on this cell
        int i, pos = r*10 + c;
        for(i = 0; i < 17; i++) //for each ship
            if(ships[i] == pos){ //if ship was hit
                b[r][c] = 'X';
                printf("SHIP WAS HIT!!!\n\n");

                removeSunkenShips(b, ships);
                return 1;
            }
        b[r][c] = 'O'; //miss
        printf("MISS\n\n");

        removeSunkenShips(b, ships);
        return 1;
    }
    //position already attacked
    return 0;
}

void removeSunkenShips(char b[ROWS][COLS], int ships[]){
    //check each ship in ships to see if sunken
    int i, hits = 0;
    int r, c;

	////CARRIER////
    for(i = 0; i < 5; i++){ //check carrier hits
		r = ships[i] / 10;
        c = ships[i] % 10;
        if(b[r][c] == 'X')
            hits++;
    }
    if(hits == 5) //if carrier has 5 hits
        for(i = 0; i < 5; i++){
            r = ships[i] / 10;
            c = ships[i] % 10;
            b[r][c] = 'C';
        }
    hits = 0;

	////BATTLESHIP////
    for(i = 5; i < 9; i++){ //check battleship hits
		r = ships[i] / 10;
        c = ships[i] % 10;
        if(b[r][c] == 'X')
            hits++;
	}
    if(hits == 4) //if battleship has 5 hits
        for(i = 5; i < 9; i++){
            r = ships[i] / 10;
            c = ships[i] % 10;
            b[r][c] = 'B';
        }
    hits = 0;


	////DESTROYER////
    for(i = 9; i < 12; i++){ //check Destroyer hits
		r = ships[i] / 10;
        c = ships[i] % 10;
        if(b[r][c] == 'X')
            hits++;
	}
    if(hits == 3) //if Destroyer has 5 hits
        for(i = 9; i < 12; i++){
            r = ships[i] / 10;
            c = ships[i] % 10;
            b[r][c] = 'D';
        }
    hits = 0;


	////SUBMARINE////
    for(i = 12; i < 15; i++){ //check Submarine hits
		r = ships[i] / 10;
        c = ships[i] % 10;
        if(b[r][c] == 'X')
            hits++;
	}
    if(hits == 3) //if submarine has 5 hits
        for(i = 12; i < 15; i++){
            r = ships[i] / 10;
            c = ships[i] % 10;
            b[r][c] = 'S';
        }
    hits = 0;


	////PT BOAT////
    for(i = 15; i < 17; i++){ //check PT Boat hits
		r = ships[i] / 10;
        c = ships[i] % 10;
        if(b[r][c] == 'X')
            hits++;
	}
    if(hits == 2) //if submarine has 5 hits
        for(i = 15; i < 17; i++){
            r = ships[i] / 10;
            c = ships[i] % 10;
            b[r][c] = 'P';
        }
}

//given board, counts number of X:  if num >= 17, return 1
int checkIfWon(char b[ROWS][COLS]){
    int i, k, count = 0;
    for(i = 0; i < ROWS; i++)
        for(k = 0; k < COLS; k++)
            if(b[i][k] != ' ' && b[i][k] != 'O')
                count++;
    if(count >= 17){
        return 1;
    }
    return 0;
}

/*int playerShips[17];
0 to 4:     carrier     //initialize longest to shortest
5 to 8:     BattleShip
9 to 11:    Destroyer
12 to 14:   Submarine
15 to 16:   PT Boat

each int is a location,
posR = playerShips[0] / 10;
posC = playerShips[0] % 10;
*/
int tryShip(int ships[17], int index, int len){
    int r, d; //r is posXY and d is direction
    int i, k, ctr; //i is increment, ctr counts conflicts
    int fits = 0;

    int tries = 0;
    while(!fits && tries < 200){
        int try = 1;
        r = rand()%100;
        d = rand()%2; //right or down

        //check if fits
        if(d == 1){//right
            if(r%10 < len){
                ctr = 0;
                for(i = 0; i < len; i++){//for each position to right
                    //check if position is already in use
                    for(k = 0; k < 17; k++)
                        if(r+i == ships[k])
                            ctr++;
                }
                if(ctr == 0) //if no conflicts
                    fits = 1;
            }
        }//right

        if(d == 0)//down
            if(r / 10 < len){
                printf("LOC 2: %d\n", r);
                for(i = 0; i < len; i++){
                    //check if position is already in use
                    for(k = 0; k < 17; k++)
                        if(r+(i*10) == ships[k]){
                            printf("LOC 3\n");
                            ctr++;}
                }
                if(ctr == 0) //if no conflicts
                    fits = 1;
            }
        tries++;
    }//end while

    if(fits){
        if(d)
            for(i = 0; i < len; i++)
                ships[i+index] = r+i;
        else
            for(i = 0; i < len; i++)
                ships[i+index] = r+(i*10);
        return len;
    }
    printf("could not place\n");
    return 0;
}

void initializeShips(int ships[17]){
    int len, index = 0;

    len = 5; //CARRIER
    index += tryShip(ships, index, len);

    len = 4; //BATTLESHIP
    index += tryShip(ships, index, len);

    len = 3; //DESTROYER
    index += tryShip(ships, index, len);

    len = 3; //SUBMARINE
    index += tryShip(ships, index, len);

    len = 2; //PT BOAT
    index += tryShip(ships, index, len);

    int i;
    for(i = 0; i < 17; i++)
        printf("%d, ", ships[i]);
    printf("\n");
}
