#include <iostream>
#include <random>                                                                                               // to generate random number
#include <conio.h>                                                                                              // getch() function-->display the output screen until any key is pressed, and to use kbhit function -->this func checks if the key is hit or not
#include <windows.h>                                                                                            // header file for gotoxy
#include <stdio.h>                                                                                              // header file for stdin and stdout
#include <time.h>                                                                                               // for srand()
using namespace std;


COORD coord = {0, 0};                                                                                           // center of axis is set to top left corner of the screen

void gotoxy(int x, int y) {                                                                                     // gotoxy function
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

int getRandom() {                                                                                               // random function
	int random;
	srand(time(0));
	random = rand()%7;                                                                                            // gives random number between 0-6
	return random;
}

//VARIABLE DECLARATION
int block[][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
int stage[32][21] = {};
int field[32][21] = {};

int x = 8, y = 0;                                                                                               // coordinate
bool gameover = 0;                                                                                              // check whether game is over or not
long long int GAMESPEED = 25000;                                                                                // determines gamespeed
int score = 0, oldScore = 0;                                                                                    // calculate score
int highScore = 0;                                                                                              // to maintain high score


int block_list[7][4][4] = {
	
	 {{ 0, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 }},                         // line

    {{ 0, 0, 0, 0 },
     { 0, 1, 1, 0 },
     { 0, 1, 0, 0 },
     { 0, 1, 0, 0 }},                          // inverted L

    {{ 0, 0, 1, 0 },
     { 0, 1, 1, 0 },
     { 0, 1, 0, 0 },
     { 0, 0, 0, 0 }},                          // rotated Z

    {{ 0, 1, 0, 0 },
     { 0, 1, 1, 0 },
     { 0, 0, 1, 0 },
     { 0, 0, 0, 0 }},                          // rotated S

    {{ 0, 0, 0, 0 },
     { 0, 1, 0, 0 },
     { 1, 1, 1, 0 },
     { 0, 0, 0, 0 }},                          // inverted T

    {{ 0, 0, 0, 0 },
     { 0, 1, 1, 0 },
     { 0, 1, 1, 0 },
     { 0, 0, 0, 0 }},                          // block

    {{ 0, 0, 0, 0 },
     { 0, 1, 1, 0 },
     { 0, 0, 1, 0 },
     { 0, 0, 1, 0 }}                           // mirrored of inverted L
};
 
//FUNCTION USED

int gameOver();                                // displays gameover
void title();                                  // displays title TETRIS
void gameLoop();                               // runs game
void display();                                // display game board
bool makeBlocks();                             // make Blocks
void initGame();                               // initializes game
void moveBlock(int, int);                      // moves block
void collidable();                             // fix the block
bool isCollide(int, int);                      // check collisions
void userInput();                              // takes input from user
bool rotateBlock();                            // rotates the block
void spawnBlock();                             // moves block down
void speed();                                  // increases the speed

//MAIN FUNCTION
int main() {
	title();
	gameLoop();
	return 0;
}

void title() {
	system("cls");                                                 // clears screen
     cout <<"\n\n\n\n"
            "\t\t####### ####### ####### ######    ###    #####\n"    
            "\t\t   #    #          #    #     #    #    #     #\n"
            "\t\t   #    #          #    #     #    #    #\n"
            "\t\t   #    #####      #    ######     #     #####\n"
            "\t\t   #    #          #    #   #      #          #\n"
            "\t\t   #    #          #    #    #     #    #     #\n"
      		"\t\t   #    #######    #    #     #   ###    #####\n"
            "\n\n\n\n";
    cout <<"Press any key to continue...";      
           getch();                            // displays tetris till user press any key

}

int gameOver() {
	system("cls");                                                       // clears screen
    cout << "\n\n\n\n"
            "\t #####     #    #     # ####### ####### #     # ####### ######\n"
            "\t#     #   # #   ##   ## #       #     # #     # #       #     #\n"
            "\t#        #   #  # # # # #       #     # #     # #       #     #\n"
            "\t#  #### #     # #  #  # #####   #     # #     # #####   ######\n"
            "\t#     # ####### #     # #       #     #  #   #  #       #   #\n"
            "\t#     # #     # #     # #       #     #   # #   #       #    #\n"
            "\t #####  #     # #     # ####### #######    #    ####### #     #\n"
            "\n\n\n\n";
            cout <<"\t\t\t\tYour Score is: "<< score;                    // displays score
            cout <<"\n\n\n";
            char ch;
            cout<<"\t\t\tWant to start new game {y/n}: ";
            cin >> ch;
            if(ch == 'y') {                                              // starts new game
                gameover = false;
                score = 0;
                GAMESPEED = 25000;
                oldScore = 0;
                gameLoop();
        }
}

void gameLoop() {
	int time = 0;
	initGame();
	while(!gameover) {                                               // check whether game is over or not
		if(kbhit())                                                    // function to determine if the key is pressed or not
			userInput();                                                 // function to take user input
		if(time < GAMESPEED)
			time++;
		else {
			spawnBlock();                                                // moves block down
			time = 0;
		}
	}
}

void initGame() {                                                  // initGame function
	for(int i=0 ; i<=30 ; i++) {                                     // loop for number of rows
		for(int j=0 ; j<=19 ; j++) {                                   // loop for number of columns
			if((j==0)||(j==19)||(i==30))                                 // check for border and base
				field[i][j] = stage[i][j] = 9;
			else                                                         // remaining part of border
				field[i][j] = stage[i][j] = 0;
		}
	}
	makeBlocks();
	display();
}

void display() {
	system("cls");                                                                          // clears screen
	cout<<"\n\n\n\n\t\t Your Score : "<<score<<"\t\t Highest Score : "<<highScore;          // displays score & high_score
	int a = 10;
	for(int i=0 ; i<31 ; i++) {
		gotoxy(32, a);                                                                        // for starting position of rows
		a++;                                                                                  // update row
		for(int j=0 ; j<20 ; j++) {
			switch(field[i][j]) {
				case 0: cout <<" "<< flush;                                                       // for empty space
                    		break;
	            case 9: cout <<"X"<< flush;                                                 // for border and base
	                		break;
	            default: cout <<"#"<< flush;                                                // for filled part
	                		break;
			}
		}
		cout << endl;
	}
	cout << "\n\n\n\t A: Left\t S: Down\t D: Right\t Rotation[Space]";                // for instructions
    if(gameover)                                                                          // check for game over
        gameOver();  
}

bool makeBlocks() {
	x = 8;                                                                                  // for middle of game
	y = 0;                                                                                  // for random block
	int blockType = getRandom();
	for(int i=0 ; i<4 ; i++) {
		for(int j=0 ; j<4 ; j++) {                                                            // loop for block
			block[i][j] = 0;
			block[i][j] = block_list[blockType][i][j];
		}
	}
	for(int i=0 ; i<4 ; i++) {
		for(int j=0 ; j<4 ; j++) {
			field[i][j + x] = stage[i][j + x] + block[i][j];                                    // copying blocks to field
			if(field[i][j + x] > 1) {                                                           // collision occurs
				gameover = 1;                                                                     // denotes gameover
				return 1;
			}
		}
	}
	return 0;
}

void moveBlock(int x2, int y2) {                                                          // move block function
	for(int i=0 ; i<4 ; i++)
		for(int j=0 ; j<4 ; j++)
			field[y + i][x + j] -= block[i][j];                                                 // remove block
	x = x2;                                                                                 // update coordinates
	y = y2;
	for(int i=0 ; i<4 ; i++)                                                                // assign block with updated value
		for(int j=0 ; j<4 ; j++)
			field[y + i][x + j] += block[i][j];
		
	display();
}

void collidable() {                                                                       // collidable function
	int c = 1, k = 29;
	for(int i=29 ; i>=0 ; i--) {                                                            // for each row
		c=1;
		for(int j=0 ; j<20 ; j++) {                                                           // for each column
			if(field[i][j]==0) {                                                                // check for empty space in line
				c=0;
				break;
			}
		}
		if(c==0) {                                                                            // if empty space found then copy as it is else don't
			for(int j=0 ; j<20 ; j++)
				stage[k][j]=field[i][j];                                                          // copying after eliminating filled lines
			k--;
		}
	}
	for(int i=0 ; i<31 ; i++)
		for(int j=0 ; j<20 ; j++)
			field[i][j]=stage[i][j];                                                            // copying final field 
	score+=10;                                                                              // increase of 10 for fixing each block
	score+=(k+1)*10;                                                                        // increase of 10 for removal of each line
	if(k>=1)                                                                                // if lines removed are more than 1 then increase score by 20
		score+=20;
	if(score>highScore)                                                                     // update highscore if score id greater than it
		highScore=score;
	speed();
}

bool isCollide(int x2, int y2) {                                                          // check for collision
	for(int i=0 ; i<4 ; i++)
		for(int j=0 ; j<4 ; j++)
			if(block[i][j] && stage[y2+i][x2+j]!=0)
				return true;                                                                      // if collision then return true else false
	return false;                                      
}

void spawnBlock() {                                                                       // spawnblock function
	if(!isCollide(x, y+1))                                                                  // check for collision
		moveBlock(x, y+1);                                                                    // if no collision then move the block else fix it
	else {
		collidable();                                                                         // fix the block
		makeBlocks();
		display();
	}
}

void speed() {                                                                             // speed function
	if(score - oldScore >=200) {
		oldScore+=200;                                                    
		GAMESPEED-=5000;                                                                       // increase speed
	}
}

void userInput() {
	char key;
	key=getch();
	switch(key) {
		case 'd': if(!isCollide(x+1, y))                                          // if no collision, move right
					moveBlock(x+1, y);
			break;
		case 'a': if(!isCollide(x-1, y))                                          // if no collision, move left
					moveBlock(x-1, y);
			break;
		case 's': if(!isCollide(x, y+1))                                          // if no collision, move down
					moveBlock(x, y+1);
			break;
		case ' ': rotateBlock();                                                  // rotate block
	}
}

bool rotateBlock() {                                                            // rotate block function
	int tmp[4][4];
	for(int i=0 ; i<4 ; i++)                                                      // save block temporarily
		for(int j=0 ; j<4 ; j++)
			tmp[i][j]=block[i][j];                                                
	for(int i=0 ; i<4 ; i++)                                                      // rotate clockwise
		for(int j=0 ; j<4 ; j++)                  
			block[i][j]=tmp[3-j][i];
	if(isCollide(x, y)) {                                                         // stop if overlap
		for(int i=0 ; i<4 ; i++)
			for(int j=0 ; j<4 ; j++)
				block[i][j]=tmp[i][j];
		return 1;
	}
	for(int i=0 ; i<4 ; i++)
		for(int j=0 ; j<4 ; j++) {
			field[y+i][x+j]-=tmp[i][j];                                              // removes original block
			field[y+i][x+j]+=block[i][j];                                            // adds rotated block
		}
	display();                                                                   // calls display function
	return 0;
}
