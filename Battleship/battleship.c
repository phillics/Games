/*
TO DO:
    make better display for GAME WON
*/
#include "playGame.h"



//menus
void displayMainMenu(int *userChoice);
void displayControlsAndInfo();
void chooseAIlevel();
void viewHighscores();

int main(){
    //setup
        //check bin file for active games

    //main menu
    int userChoice;
    do {
        userChoice = 0;
        displayMainMenu(&userChoice);

        switch (userChoice) {
        case 1: // Start New Game
   			chooseAIlevel("NEW");
            break;
        case 2: // Continue Game, load game from file
			playGame("saved_game.txt", 0);
            break;
        case 3: // View High Scores
			viewHighscores();
            break;
        case 4: // Controls and Game info
			displayControlsAndInfo();
            break;
		case 5: //quit
			break;
        default:
            printf("Invalid Selection\n");
            PAUSE;
            break;
        } //
    } while (userChoice != 5);

    return 0;
}

/*each record is on seperate line NAME: Score: %d AIlevel: %d\n*/
void viewHighscores(){
	CLS;

	char name[100] = {'\0'};
	int nameCtr = 0;
	int score;
	int AIlevel;

	//get stuff from file
	FILE* fp = fopen("highscores.txt", "r");
	char c;
	int i;

	while((c = fgetc(fp)) != EOF){ //for each c in file
		if(c == '\n')
			continue;

		while(c != ':'){//read name
			if(c == '\n')
				c = fgetc(fp);
			else{
				name[nameCtr++] = c;
				c = fgetc(fp);
			}
		}//end read name

		//ignore stuff, get score
		for(i = 0; i < 8; i++)
			fgetc(fp);
		fscanf(fp," %d", &score);

		//ignore stuff, get AI
		for(i = 0; i < 10; i++)
			fgetc(fp);
		fscanf(fp," %d", &AIlevel);

		
		printf("%s %d %d\n", name, score, AIlevel);

		for(i = 0; i < 100; i++)
			name[i] = '\0';
		nameCtr = 0;
	}
	fclose(fp);
	printf("\n\n");
	FLUSH; PAUSE;
}

void chooseAIlevel(char fileName[]){
	int AIlevel;
	printf("Choose your Adversary...\n\n");
	printf("(1) HAL\n(2) WOPR\n(3) VIKI\n(4) SKYNET\n\nEnter Choice: ");
	scanf(" %d", &AIlevel);
	if(AIlevel >= 1 && AIlevel <= 4)
		playGame(fileName, AIlevel);
	else
		printf("Invalid selection\n\n");
		PAUSE;
}

void displayControlsAndInfo(){
	CLS;
	//basic explination of rules
	char rules[] = "This Battleship game is a single player game (you vs the computer).\nThe goal is to guess the location of the computer's ships before the computer finds yours.\nEach of you has 5 ships of various sizes.\n\n";
	printf("%s", rules);

	//controls of the game
	char controls[] = "On each turn the player will be asked to enter the column and row they wish to attack\nColumns are entered as uppercase letters:\t A,B,C,D,E,F,G,H,I,J\nRows are entered as single digit integers:\t 0,1,2,3,4,5,6,7,8,9\nIf ship is hit, an X will appear.\nIf the ship is sunk, the X's will be replaced with the ships letter\nIf no ship was hit, a O will appear\n\nInstead of entering column to attack, Player can enter\n\tS to save the game\n\tQ to quit with or without saving\n\n";
	printf("%s", controls);

	//hints
	char hints[] = "Before a new game starts, you can select the computer difficulty.\nHAL\teasy\nWOPR\tmedium\nVIKI\tdifficult\nSKYNET\timpossible\n\n";
	printf("%s", hints);

	//authors and revisions
	char authors[] = "Authors: Christian Phillips, Michael Gambetta, Derek Lu\nlast revised: Oct 23, 2016\n";
	printf("%s", authors);
	FLUSH;	PAUSE;
}

void displayMainMenu(int *userChoice) {
	CLS;
	printf("***** BattleShip *****\n");
	printf("(1) Start New Game\n");
	printf("(2) Continue Game\n");
	printf("(3) View High Scores\n");
	printf("(4) Controls and Game Info\n");
	printf("(5) Exit\n");
	printf("Enter Choice: ");
	scanf(" %d", userChoice);
} // end displayMainMenu*/
