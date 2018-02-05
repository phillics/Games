#include "battleshipAI.h"

/*idea:: use macros to help condense code in viki */

int getAImove(char b[ROWS][COLS], int level){
    printf("AI move\n");

    int move;
    switch(level){
        case 1:
            move = hal(b);
            break;
        case 2:
            move = wopr(b);
            break;
        case 3:
            move = viki(b);
            break;
        case 4:
            move = skynet(b);
            break;
        default:
            move = hal(b);
            break;
    }

    return move;
}

//easy AI
/*returns random location on board*/
int hal(char b[ROWS][COLS]){
    int valid = 0, move;
    while(!valid){
        move = rand()%100;

        if(b[move/10][move%10] == ' ')
            valid = 1;
    }
    return move;
}

/*standard AI
if ship has been hit, target
else, guess random*/
int wopr(char b[ROWS][COLS]){
    int attack;
    if((attack = target(b)) > 0)
        return attack;
    return hal(b);
}

//difficult
/*increment 2D array of counters based on likelyhood of ship at that position
attack most likely position*/
int viki(char b[ROWS][COLS]){

    int r, c, s, f;
    int prob[ROWS][COLS] = {0};

    //increment counters around any 'X' 
    for(r = 0; r < ROWS; r++){
		for(c = 0; c < COLS; c++){
			if(b[r][c] != ' ')
				continue;

			//check left right up and down			
			if(c > 0)//left
				if(b[r][c-1] == 'X')
					prob[r][c] += 50;
			if(c < COLS-1)//right
				if(b[r][c+1] == 'X')
					prob[r][c] += 50;
			if(r > 0)//up
				if(b[r-1][c] == 'X')
					prob[r][c] += 50;
			if(r < ROWS-1)
				if(b[r+1][c] == 'X')
					prob[r][c] += 50;
		}
	}

    //determine possible ships in each cell right and down
    int shipLens[5] = {5,4,3,3,2};

    for(r = 0; r < ROWS; r++){
        for(c = 0; c < COLS; c++){
            for(s = 0; s < 5; s++){
                //fits right?
                int fits = 0;
                if(COLS - c >= shipLens[s]){
                    //check for obstacles
                    for(f = 0; f < shipLens[s]; f++)
                        if(b[r][c+f] == ' ')
                            fits++;

                    if(fits == shipLens[s])
                        for(f = 0; f < shipLens[s]; f++)
                            prob[r][c+f]++;
                }
                //fits down?
                fits = 0;
                if(ROWS - r >= shipLens[s]){
                    for(f = 0; f < shipLens[s]; f++)
                        if(b[r+f][c] == ' ')
                            fits++;

                    if(fits == shipLens[s])
                        for(f = 0; f < shipLens[s]; f++)
                            prob[r+f][c]++;
                }
                //fits left
                fits = 0;
                if(c >= shipLens[s] - 1){
                    for(f = 0; f < shipLens[s]; f++)
                        if(b[r][c-f] == ' ')
                            fits++;

                    if(fits == shipLens[s])
                        for(f = 0; f < shipLens[s]; f++)
                            prob[r][c-f]++;
                }
                //fits up
                fits = 0;
                if(r >= shipLens[s]-1){
                    for(f = 0; f < shipLens[s]; f++)
                        if(b[r-f][c] == ' ')
                            fits++;

                    if(fits == shipLens[s])
                        for(f = 0; f < shipLens[s]; f++)
                            prob[r-f][c]++;
                }
            }
        }

    }
    int highest = 0;
	int location = -1;

    //display board and find highest
    for(r = 0; r < ROWS; r++)
        for(c = 0; c < COLS; c++)
            if(prob[r][c] > highest){
                highest = prob[r][c];
                location = r*10+c;
            }

    if(location >= 0)
        return location;

    return hal(b);
}

//impossible
/*
uses viki's probability distrobution until turn 20, then skynet comes online...
call atomic bomb function...
*/
int skynet(char b[ROWS][COLS]){
	//find out which turn it is by counting non ' ' characters
	int r, c, count = 0;
	for(r = 0; r < ROWS; r++)
		for(c = 0; c < ROWS; c++)
			if(b[r][c] != ' ')
				count++;

	if(count >= 29){
		skynet_online(b);
		return -1;
	}
	return viki(b);
}


//destruction animation...
void skynet_online(char b[ROWS][COLS]){
	//attack position
	int attack = viki(b);
	
	//replace attack with #
	int r = attack / 10;
	int c = attack % 10;
	b[r][c] = '#';

	char temp[ROWS][COLS];//stores position of bomb for next round
	for(r = 0; r < ROWS; r++)
		for(c = 0; c < ROWS; c++)
			temp[r][c] = b[r][c];

	int boardFilled = 0;	
	struct timespec delay, tim2;
	delay.tv_sec = 1;
	delay.tv_nsec = 500; //can set delay in sec or nsec

	while(!boardFilled){
		/*FRAME RATE CONTROL*/
		nanosleep(&delay, &tim2);		

		//display board
		CLS;
		displayBoard(b, "SKYNET ONLINE");

		//check for empty space in four corners...
		if(b[0][0] == '#' && b[0][COLS-1] == '#' && b[ROWS-1][0] == '#' && b[ROWS-1][COLS-1] == '#'){
			boardFilled = 1;
			break;
		}

		//fill in next pieces on temp board (
		for(r = 0; r < ROWS; r++)
			for(c = 0; c < COLS; c++)
				if(b[r][c] != '#'){ //check cells around for #
					if(r > 0)
						if(b[r-1][c] == '#')
							temp[r][c] = '#';
					if(r < ROWS -1)
						if(b[r+1][c] == '#')
							temp[r][c] = '#';
					if(c > 0)
						if(b[r][c-1] == '#')
							temp[r][c] = '#';
					if(c < COLS-1)
						if(b[r][c+1] == '#')
							temp[r][c] = '#';
				}
	
		//swap boards 
		for(r = 0; r < ROWS; r++)
			for(c = 0; c < ROWS; c++)
				b[r][c] = temp[r][c];



		
	}//end while
}

//attack areas around 'X', return -1 if not found
int target(char b[ROWS][COLS]){

    int i, j;
    int attack = 0;
    int r = ROWS, c = COLS;

    printf("\tlooking for Target\n");

    for(r = 0; r < ROWS; r++){
        for(c = 0; c < COLS; c++){
            if(b[r][c] == 'X'){
                attack = r*10+c;
                attack = attackPos(b, attack);
                if(attack > 0)
                    return attack;
            }
        }
    }
    printf("\tno targets found\n");
    return -1;
}

//go through each position in board and see if 'X'
int attackPos(char b[ROWS][COLS], int attack){

	int r = attack / 10;
	int c = attack % 10;

    //printf("checking corrdinates: (%d, %d)\n", r, c);

    if(c < COLS-1){ //check right
        if(b[r][c+1] == ' ')
            return attack + 1;
    }
    if(r < ROWS-1){ //check down
        if(b[r+1][c] == ' ')
            return attack + 10;
    }
    if(c > 0){ //check left
        if(b[r][c-1] == ' ')
            return attack - 1;
    }
    if(r > 0){ //check up
        if(b[r-1][c] == ' ')
            return attack - 10;
    }

    return - 1;
}
