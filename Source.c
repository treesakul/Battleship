//Source code
#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#define KEY_LEFT 75							/* Move left, left arrow */
#define KEY_UP 72							/* Move up, up arrow */ 
#define KEY_RIGHT 77						/* Move right, right arrow */
#define KEY_DOWN 80							/* Move down, down arrow */
#define KEY_ENTER 13						/*Enter key*/
#define BOARDSIZE 8							/*size of the board*/
#define BATTLESHIPSIZE 5					/*size of the biggest ship*/
#define CRUISERSIZE 4						/*size of the second biggest ship*/
#define FRIGATESIZE 3						/*size of the second smallest ship*/
#define MINESWEEPERSIZE 2					/*size of the smallest ship*/
#define SHIPNUMBER 4						/*number of all the ship*/
#define WATER 176							/*character for water*/
#define SHIP 219							/*character for ship(while selecting)*/
#define BOMB 1								/*character for ship that has been hit (smiley face)*/
#define NOTBOMB 248							/*character for water that is empty (tiny circle)*/
#define PLAYER 219							/*character when the player is selecting the position of the ships*/
#define BLANK -52							/*black character*/
#define PLAYERNO 2							/*number of player*/

struct board {
	char ship[SHIPNUMBER][BOARDSIZE][BOARDSIZE];
	int score;	//contain positions of the 2-block ship
	int mainboard[BOARDSIZE][BOARDSIZE];	/*8-by-8 board*/
	char openbomb[BOARDSIZE][BOARDSIZE];	/*the 8-by-8 board contains the position of the block that a bomb has been placed*/
	char openwater[BOARDSIZE][BOARDSIZE];	/*the 8-by-8 board contains the position of the block that has been opened*/
};
typedef struct board Board;
struct info {				/*store the winner's information*/
	char name[100];
	int score;
	char date[80];

};
typedef struct info Info;
struct dt {					/*Information for the present date, year and month*/
	int date;
	int month;
	int year;
};
//Introduction to the game 
void intro() {
	int user;
	while (1) {
		printf("=========================================================");
		printf("\n=		   Welcome to Battleship  		=");
		printf("\n=========================================================");
		printf("\n");
		printf("                    ()\n");
		printf("                    ||q',,'\n");
		printf("                    ||d,~\n");
		printf("         (,---------------------,)\n");
		printf("          ',       q888p       ,'\n");
		printf("            \\       986       /\n");
		printf("             \\  8p, d8b ,q8  /\n");
		printf("              ) 888a888a888 (\n");
		printf("             /  8b` q8p `d8  \\              O\n");
		printf("            /       689       \\             |','\n");
		printf("           /       d888b       \\      (,---------,)\n");
		printf("         ,'_____________________',     \\   ,8,   /\n");
		printf("         (`__________|__________`)      ) a888a (  \n");
		printf("         [___________|___________]     /___`8`___\\   }*{\n");
		printf("           }:::|:::::}::|::::::{      (,=========,)  -=-\n");
		printf("            '|::::}::|:::::{:|'  .,.    \\:::|:::/    ~`~\n");
		printf("--=~(@)~=-- ' |}:::::|::{:::|'           ~\"., .\"~`~\n");
		printf("               '|:}::|::::|'~`~\"., .\" \n");
		printf("         ~`~\"., .\"~`~\"., \"~`~\"., .\"~\n\n");
		printf("		Press enter to play\n");
		user = _getch();
		if (user == KEY_ENTER)	//will only go to the next window if the enter button is pressed
			break;
		system("cls");
	}
	system("cls");
}
//display the board that shows the position of the ships
void displayBoard(Board* ptrBoard) {
	int option = WATER;
	printf("\nYour current board: \n\n");
	int i, j;
	for (i = 0; i < BOARDSIZE; i++) {
		printf("\t    ");
		for (j = 0; j < BOARDSIZE; j++) {
			printf(" %c ", ptrBoard->mainboard[i][j]);	//display the current character in main board 
		}
		printf("\n\n");
	}
}
//gameCore function is for assigning the position from all the ships to the mainboard or refresh the main board
void gameCore(Board* ptrBoard) {
	int i, j, k;
	int option = WATER;
	for (i = 0; i < BOARDSIZE; i++) {
		for (j = 0; j < BOARDSIZE; j++) {
			for (k = 0; k < SHIPNUMBER; k++) {
				if (ptrBoard->ship[k][i][j] != BLANK) {
					ptrBoard->mainboard[i][j] = ptrBoard->ship[k][i][j];	//assign the position of the placed ship to the main board
				}
				if (ptrBoard->ship[0][i][j] == BLANK && ptrBoard->ship[1][i][j] == BLANK && ptrBoard->ship[2][i][j] == BLANK && ptrBoard->ship[3][i][j] == BLANK)
					ptrBoard->mainboard[i][j] = option;						//assign WATER character to the main board
			}
		}
	}
}
//Check if the ships are placed overlapped
int isOverlapped(Board* ptrBoard, int m, int n, int shipnum, int choice) { //m and n are the row and column respectively
	int k, l, i;
	for (i = 0; i < shipnum; i++) {
		for (k = 0; k < SHIPNUMBER; k++) {
			if (ptrBoard->ship[k][m][n] != BLANK)			//if the current position already exists in current ship array, return 1
				return 1;
		}
		if (choice == 1)		//the direction that the ship is facing to move forward the position in the loop
			n = n + 1;
		else if (choice == 2)	//1 for left and right and 2 for up and down
			m = m + 1;
	}
	return 0;	//no conditions above have been applied then the function returns 0
}
// Function fixPos is used to move the position of the ship  around while selectiing
void fixPos(Board* ptrBoard, int m, int n, int shipnum, int choice, int enter) {
	// if the enter button is pressed, the value of enter is 1, otherwise it is 0;
	int option = SHIP, k, l, list[] = { BATTLESHIPSIZE, CRUISERSIZE, FRIGATESIZE, MINESWEEPERSIZE };

	for (l = 0; l < shipnum; l++) {
		ptrBoard->mainboard[m][n] = option;
		for (k = 0; k < SHIPNUMBER; k++) {
			if (enter == 1 && shipnum == list[k])							//enter is pressed and the selected ship is battleship
				ptrBoard->ship[k][m][n] = ptrBoard->mainboard[m][n];		//the selected position on the main board will be assigned to the ship array
		}
		if (choice == 1)		//the direction that the ship is facing to move forward the position in the loop
			n++;				//1 for left and right and 2 for up and down
		else if (choice == 2)
			m++;

	}
}
//selectionWindows is a window for the users(players) to place the ships.
Board selectionWindow(Board board, Board* ptrBoard) {
	int i, ships[SHIPNUMBER] = { BATTLESHIPSIZE, CRUISERSIZE, FRIGATESIZE, MINESWEEPERSIZE }, j, user = 1, m = 0, n = 0;
	int count = 0, user2, enter = 0, player = 1, edge;
	// i and j = used in a loop// user = the decision of the ship direction that the user made// m = // n = column
	// count = used to change the kind of ship to the next one //user2 = key that user entered// player = player's number
	char choice; //direction of the ship
	ptrBoard = &board;
	ptrBoard->score = 0;
	system("cls");
	for (i = 0; i < SHIPNUMBER; i++) {
		m = n = 0;
		enter = 0;
		while (1) {									//loop for selecting the direction that the ship will be faciing
			printf("What direction will the ship be facing?\n");
			printf("\n\t");
			for (j = 0; j < ships[i]; j++) {			//Display ship facing right and left
				printf("%c ", SHIP);
			}
			printf("\t %c\n", SHIP);
			for (j = 0; j < ships[i] - 1; j++) {		//Display ship facing up and down*/
				if (ships[i] < 4)
					printf("\n\t\t %c\n", SHIP);
				else
					printf("\n\t\t\t %c\n", SHIP);
			}
			//Number (1) and (2) to be picked
			if (ships[i] < 4)						//organize the format for the ship that is larger that 3 blocks
				printf("\n\t(1)\t(2)");				//display choices 
			else
				printf("\n\t(1)\t\t(2)\n");			//display choices  

			if (user == 1) {
				printf("\n\t ^");					//display the position for the curser for the first choice
			}
			else if (user == 2) {					//display the position for the curser for the second choice
				if (ships[i] < 4)					//organize the format for the ship that is smaller that 4 blocks
					printf("\n\t\t ^");				//display the position for the curser for the second choice
				else                                //organize the format for the ship that is larger that 3 blocks
					printf("\n\t\t\t ^");			//display the position for the curser for the second choice
			}

			choice = _getch();						//input from the user
			if (choice == KEY_LEFT) {				//left button is pressed
				user = 1;
			}
			else if (choice == KEY_RIGHT) {			//right button is pressed
				user = 2;
			}
			else if (choice == KEY_ENTER) {			//enter button is pressed
				if (user == 1 || user == 2) {		//assign user value to the variable user
					break;
				}
			}
			system("cls");
		}
		gameCore(ptrBoard);							//call gameCore fuction to initialize the main board
		fixPos(ptrBoard, m, n, ships[count], user, enter);
		system("cls");
		while (1) {									//loop for plaing the ship in the board
			printf("Row = %d Column = %d\n", m, n);
			displayBoard(ptrBoard);
			user2 = _getch();						//keyboard input from the user
			if (user2 == KEY_RIGHT) {				//right button is pressed
				if (user == 1)
					edge = BOARDSIZE - ships[count];	//initialize the value of the edge 
				else if (user == 2)
					edge = BOARDSIZE - 1;			//initialize the value of the edge 
				if (n < edge) {						//apply the following commands if the position is not beyond the edge
					n++;
					gameCore(ptrBoard);
					fixPos(ptrBoard, m, n, ships[count], user, enter);
				}
				else {								//the selected position is beyond the edge and display the message to the user
					printf("\nOut of the board!!!\n");
					Sleep(300);
				}
			}
			else if (user2 == KEY_LEFT) {			//left button is pressed
				if (n > 0) {							//apply the following commands if the position is not beyond the edge
					n--;
					gameCore(ptrBoard);
					fixPos(ptrBoard, m, n, ships[count], user, enter);
				}
				else {								//the selected position is beyond the edge and display the message to the user
					printf("\nOut of the board!!!\n");
					Sleep(300);
				}
			}
			else if (user2 == KEY_UP) {				//up button is pressed
				if (m > 0) {							//apply the following commands if the position is not beyond the edge
					m--;
					gameCore(ptrBoard);
					fixPos(ptrBoard, m, n, ships[count], user, enter);
				}
				else {								//the selected position is beyond the edge and display the message to the user
					printf("\nOut of the board!!!\n");
					Sleep(300);
				}
			}
			else if (user2 == KEY_DOWN) {			//down button is pressed
				if (user == 2)						//intialize the value of the edge
					edge = BOARDSIZE - ships[count];
				else if (user == 1)
					edge = BOARDSIZE - 1;
				if (m < edge) {						//apply the following commands if the position is not beyond the edge
					m++;
					gameCore(ptrBoard);
					fixPos(ptrBoard, m, n, ships[count], user, enter);
				}
				else {								//the selected position is beyond the edge and display the message to the user
					printf("\nOut of the board!!!\n");
					Sleep(300);
				}
			}
			else if (user2 == KEY_ENTER) {			//enter button is pressed
				if (isOverlapped(ptrBoard, m, n, ships[count], user) == 0) {	//do the following commands if the ships are not overlapped
					fixPos(ptrBoard, m, n, ships[count], user, 1);
					gameCore(ptrBoard);
					system("cls");
					break;
				}
				else if (isOverlapped(ptrBoard, m, n, ships[count], user) == 1) { //'the ships are overlapped' is told the user
					printf("\nThey're overlapped!!!\n");
					Sleep(750);
				}
			}
			system("cls");
		}
		count++;

	}
	displayBoard(ptrBoard);
	Sleep(1000);
	system("cls");
	return board;		//return structure board
}
//displayGame is used to display the openbomb board which is surface of the water and display the number of remaining ships
void displayGame(Board* ptrBoard) {
	char shipname[SHIPNUMBER][15] = { "Battleship", "Cruiser", "Frigate", "Minesweeper" };
	int count = 0;
	printf("\nPick the spot\n\n");
	int i, j, k;
	for (i = 0; i < BOARDSIZE; i++) {
		printf("\t    ");
		for (j = 0; j < BOARDSIZE; j++) {
			printf(" %c ", ptrBoard->openbomb[i][j]);	//display the surface board
		}
		printf("\n\n");
	}
	for (k = 0; k < SHIPNUMBER; k++) {					//display the remaining ships 
		for (i = 0; i < BOARDSIZE; i++) {
			for (j = 0; j < BOARDSIZE; j++) {
				if (ptrBoard->ship[k][i][j] != BLANK)
					count++;
			}
		}
		printf("%s : %d\n", shipname[k], count);
		count = 0;
	}
}
//refreshBoard function is for assigning the position from all the ships to the mainboard or refresh the main board( similar to gameCore function)
void refreshBoard(Board* ptrBoard) {
	int i, j;
	for (i = 0; i < BOARDSIZE; i++) {
		for (j = 0; j < BOARDSIZE; j++) {
			if (ptrBoard->openwater[i][j] == BLANK) {	//if the unerneath water position is empty, assign Water to the surface position
				ptrBoard->openbomb[i][j] = WATER;
			}
			if (ptrBoard->openwater[i][j] == BOMB) {		//if the unerneath water position is bomb, assign bomb to the surface position
				ptrBoard->openbomb[i][j] = BOMB;
			}
			if (ptrBoard->openwater[i][j] != BOMB && ptrBoard->openwater[i][j] != BLANK) { //otherwise assign NOTBOMB to the surface position
				ptrBoard->openbomb[i][j] = NOTBOMB;
			}
		}
	}
}
//check if any user has found all the ships and become the winner
int checkWinner(Board* ptrBoard, int player) {
	int i, j, check = 1;
	//following loop is checking whether all the position of all the 4 ships are empty
	for (i = 0; i < BOARDSIZE; i++) {
		for (j = 0; j < BOARDSIZE; j++) {
			if (ptrBoard->ship[1][i][j] == BLANK && ptrBoard->ship[2][i][j] == BLANK && ptrBoard->ship[3][i][j] == BLANK && ptrBoard->ship[0][i][j] == BLANK)
				check = 1;
			else {
				return 0;
			}
		}
	}
	if (check == 1) {	//all the position of all the 4 ships are empty
		printf("Number of shots you've missed: %d", ptrBoard->score);
		return 1;
	}
}
//placeBomb is for placing the bomb to the board but yet assign the value to the board
int placeBomb(Board* ptrBoard, int m, int n, int enter) {
	int k;
	if (enter == 1 && ptrBoard->openwater[m][n] != BLANK) {	//check if the position has already been selected before
		printf("You've already selected the spot!");
		Sleep(500);
		return 0;
	}
	else {
		ptrBoard->openbomb[m][n] = PLAYER;						//assign PLAYER character to the surface board
		if (enter == 1 && ptrBoard->mainboard[m][n] != WATER) {	//the position is not water, inform the user they've found a ship
			printf("Bomb!");
			ptrBoard->openbomb[m][n] = BOMB;
			ptrBoard->openwater[m][n] = BOMB;
			for (k = 0; k < SHIPNUMBER; k++) {
				if (ptrBoard->ship[k][m][n] != BLANK)
					ptrBoard->ship[k][m][n] = BLANK;
			}
			Sleep(500);
			return 2;
		}
		else if (enter == 1 && ptrBoard->mainboard[m][n] == WATER) {	//the selected position is WATER and inform it to the user
			printf("Missed!");
			ptrBoard->score += 1;
			ptrBoard->openbomb[m][n] = NOTBOMB;			//assign not bomb to both surface and underwater board
			ptrBoard->openwater[m][n] = NOTBOMB;
			Sleep(500);
		}
	}
	return 1;
}
//Main function of te game for hitting the ships and finding the winner
Board mainGame(Board board, Board* ptrBoard, int player) {
	ptrBoard = &board;
	int m = 0, n = 0, user, enter, end;
	refreshBoard(ptrBoard);
	placeBomb(ptrBoard, m, n, 0);
	while (1) {
		enter = 0;
		printf("Player %d's turn ", player + 1);		//display each player's turn
		printf("\nRow = %d Column = %d", m, n);
		displayGame(ptrBoard);						//show the board
		user = _getch();							//recieve input from the user
		if (user == KEY_RIGHT) {						//the user press left button, n(column) will be moved to the right 1 block
			if (n < BOARDSIZE - 1) {					//check if n is not beyond the edge
				n++;
				refreshBoard(ptrBoard);
				placeBomb(ptrBoard, m, n, enter);
			}
			else {
				printf("Out of the board!");		//inform the user that the selected position is beyond the edge
				Sleep(500);
			}
		}
		else if (user == KEY_LEFT) {					//the user press right button, n(column) will be moved to the left 1 block
			if (n > 0) {								//check if n is not beyond the edge
				n--;
				refreshBoard(ptrBoard);
				placeBomb(ptrBoard, m, n, enter);
			}
			else {
				printf("Out of the board!");		//check if n is not beyond the edge
				Sleep(500);
			}
		}
		else if (user == KEY_UP) {					//the user press up button, n(column) will be moved up 1 block
			if (m > 0) {								//check if n is not beyond the edge
				m--;
				refreshBoard(ptrBoard);
				placeBomb(ptrBoard, m, n, enter);
			}
			else {
				printf("Out of the board!");		//check if n is not beyond the edge
				Sleep(500);
			}
		}
		else if (user == KEY_DOWN) {					//the user press down button, n(column) will be moved down 1 block
			if (m < BOARDSIZE - 1) {					//check if n is not beyond the edge
				m++;
				refreshBoard(ptrBoard);
				placeBomb(ptrBoard, m, n, enter);
			}
			else {
				printf("Out of the board!");		//check if n is not beyond the edge
				Sleep(500);
			}
		}
		else if (user == KEY_ENTER) {				//the user press enter
			if (placeBomb(ptrBoard, m, n, 1) == 1) {	//the selected position didn't hit the ship
				break;
			}
			if (checkWinner(ptrBoard, player) == 1) {//check if any player has found all the ships
				system("cls");
				return board;
			}
		}
		system("cls");
	}
	system("cls");
	displayGame(ptrBoard);	//display the board before leaving
	system("cls");
	return board;
}
//record function is responsible for keeping up with the highest score
void record(Board* ptrBoard, Info* info) {
	//Info *info;
	FILE* outFile;
	char name[100], name1[100];
	char* s;
	int score, score1 = -1000;
	outFile = fopen("record.txt", "r");
	char date1[80];
	time_t rawDate;
	struct dt* date;
	char buffer[80];
	char a;

	time(&rawDate);
	date = localtime(&rawDate);
	strftime(buffer, 80, "%x", date);			//store the date into the string buffer

	printf("\n\nEnter your name: ");			//ask the user to type in a name
	gets(name);
	//system("cls");
	//printf("\nDate: %s ", buffer);				//display the current date
	score = ptrBoard->score;
	if (outFile == NULL) {						//check if the record file exists, if not, the file will be created
		outFile = fopen("record.txt", "w");
		fprintf(outFile, "name: %s score: %d Date: %s", name, score, buffer); //set the first winner
	}
	else {
		fscanf(outFile, "name: %s score: %d Date: %s", name1, &score1, date1);	//in case the file already exists
		if (score < score1) {													//the score will be compared to find the smaller score
			fclose(outFile);
			outFile = fopen("record.txt", "w");								//if the new score is lower, the file will be overwritten 
			printf("\n\nCongraturation!\n\nYou've made a new top score");
			fprintf(outFile, "name: %s score: %d Date: %s", name, score, buffer);
		}
	}
	printf("\n\nThe winner of all time (has fewest missed shots):\n\n"); //final announcement of top player
	fclose(outFile);
	outFile = fopen("record.txt", "r");
	while (fscanf(outFile, "%c", &a) != EOF)							//display the winner
		printf("%c", a);
	fclose(outFile);
}
void main() {
	Info info;
	Info* ptrinfo;
	Board board[PLAYERNO];
	Board* ptrBoard[PLAYERNO];
	ptrinfo = &info;
	ptrBoard[0] = &board[0];
	ptrBoard[1] = &board[1];
	int player = 0, round, user, order = 1;
	intro();

	for (round = 0; round < 2; round++) {
		while (1) {
			printf("\n\n\n\n\tPlayer %d's turn...\n", round + 1);				//display a message telling whose turn is starting
			printf("\n\n\tPress enter to go on...");						//user must press enter to go to the selection window
			if (user = _getch() == KEY_ENTER)
				break;
			system("cls");
		}
		board[order] = selectionWindow(board[order], ptrBoard[order]);	//selecting the positions for the ships
		displayBoard(ptrBoard[order]);									//display the complete board filled with ships
		system("cls");
		order--;
		/*because each player must place the ships for the others, so the fitst turn is for the second player*/
	}
	while (1) {
		board[player % 2] = mainGame(board[player % 2], ptrBoard[player % 2], player % 2);	//main part of the game is running
		displayGame(ptrBoard[player % 2]);													//display  the board 
		system("cls");
		if (checkWinner(ptrBoard[player % 2], player % 2) == 1) {							//check for the winner and announce it
			printf("\nPlayer %d wins!!\n", player % 2 + 1);
			break;
		}
		player++;
	}
	printf("\nHere where all the winner's ships are... ");
	displayBoard(ptrBoard[player % 2]);
	record(ptrBoard[player % 2], ptrinfo);							//call the record function to show the highest score

	_getch();
}
