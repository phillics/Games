/*

clean exit vs sudden exit
    if game closes suddenly, it should offer to continue upon startup

setup: 
	check bin file to see if 

setup menu:
	create new game
		//choose difficulty level
	continue game
		//store 3 games
	view high scores
	exit
	

main game loop:
    displayBoard

    getuser input
        ask for row col  for next attack
            check if valid move
        check if user wants to save game or quit

    update game
        check if Won

    determine AI move
        //might have different levels of AI
            trivial -> full random
            easy -> full random, if ship is hit, target
            standard ->parity random, if ship is hit target
            hard -> Hunt by probability density, target ships
                create int probability[][] = {0};
                    for each cell, check if each ship remaining fits
                        if fits, increment cell
            impossible -> AI starts cheating if it is loosing

    update game
        check if Won

    ???write updates to file??? - backup game incase sudden close


//how to score - (hits / shots) * 100
	(hits / shots) + (AI level)(ships lost) //pure random: score of 18...
	
	GIVEN: hits, shots, AI Level, AI_shipslost

	AI_LEVELS:
		trivial = 1;
		easy = 2;
		standard = 3;
		difficult = 4;
		impossible = 5;
	
	score = (hits / shots)*100 + AI_LEVEL^2 + AI_LEVEL * AI_SHIPS_LOST;
	
	EXPECTED SCORES:	lowest		expected	highest
	Trivial -> 17/70	10		32		106
	Easy	-> 17/60	4		42		114
	Standard-> 17/50	9		58		124
	Difficul-> 15/42	16		67		136
	Impossib-> 12/35	25		74		150

//AI determinations: GIVEN:  visibleBoard, playerShips
	HAL->EASY:		AVERAGE PLAYER CAN WIN
		check board for hits
		if hit:	
			call target function
		else:
			random guess

	WOPR->STANDARD:	GOOD PLAYER CAN WIN
		check board for hits
		if hit: 
			call target function
		else:
			parity random guess

	VIKI->DIFFICULT:	EXCELLENT PLAYER MIGHT WIN
		check boards for hits
		if hit:
			call target function
		else:
			probability density guess

	SKYNET->IMPOSSIBLE:	//starts off bad, then progressively gets harder
				//never allows player to win, ends game by destroying board
		if Player 1 move from winning
			drop A-Bomb
		if Player winning
			cheat
		if hit:
			call target function
		else:
			probability density guess

	needed functions:
		int target(board)
			returns target if it finds one, else returns -1
		randomGuess(board)
			returns attack location
		randomParity(board)
			returns parity location
		probabilityGuess(board)
			returns most likely location
		dropABomb()
			returns special code -66 
		cheat(playerList)
		

//variations:  SALVO - each turn, players get to shoot once for each of their ships still afloat


scanf("%[a-z ][A-Z ]s", str) //reads in only characters into string



...

CCCCC = carrier
BBBB = battleship sunk
DDD = destroyer sunk
SSS = submarine sunk
PP = Patrol boat sunk

*/

int stringLength(char *string){
	int result = 0;
	while(*string + result != '\0'){
		result++;
		string++;
	}
}
/*



http://www.datagenetics.com/blog/december32011/
*/
