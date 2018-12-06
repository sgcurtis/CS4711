/*
   ============================================================================
Name        : Conway.c
Author      :WalkersRangers 
Version     :
Copyright   : Your copyright notice
Description : Hello World in C, Ansi-style
============================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//variables for recording user's input on size of field, cell placement, etc.
//names should be self explanitonway's Game of Life\n\n");
int xSize = 0;
int ySize = 0;
int numCycles = 1;
int xStart = 0;
int yStart = 0;
int xStart2 = 0;
int yStart2 = 0;
int xStart3 = 0;
int yStart3 = 0;
char widthBuff[8];
char heightBuff[8];
char cycleBuff[8];
char xStartBuff[8];
char yStartBuff[8];
//char* xStartBuff2[8];
//char* yStartBuff2[8];
//char* xStartBuff3[8];
//char* yStartBuff3[8];
char* path[10000];
char coordList[1000];
int xList[10000];
int yList[10000];
FILE* fp;
char* wrappref[8];
int defSize = 40;
int infinite = 0;
int wrap = 0;
int liveNeigh = 0;
char sleepBuf[8];
int sleepTime = 0;
char* fieldpref[8];
int time = 0;
int step = 0;
char* stepbuff[8];
char* stepper[8];
int stepping = 0;
char* playerBuff[20];
int numPlayers = 1;

//2d array for field use, starts at size of 2x2, changed later
//char field[2][2];


struct Node  {
	int xCoord;
	int yCoord;
	struct Node* next;
	struct Node* prev;
};

struct Node* head; // global variable - pointer to head node.

struct Node* GetNewNode(int x, int y) {
	struct Node* newNode
		= (struct Node*)malloc(sizeof(struct Node));
	newNode->xCoord = x;
	newNode->yCoord = y;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

//Inserts a Node at head of doubly linked list
void InsertAtHead(int x, int y) {
	struct Node* newNode = GetNewNode(x,y);
	if(head == NULL) {
		head = newNode;
		return;
	}

	head->prev = newNode;
	newNode->next = head; 
	head = newNode;
}

//Inserts a Node at tail of Doubly linked list
void InsertAtTail(int x, int y) {
	struct Node* temp = head;
	struct Node* newNode = GetNewNode(x,y);
	if(head == NULL) {
		head = newNode;
		return;
	}
	while(temp->next != NULL) temp = temp->next; // Go To last Node
	temp->next = newNode;
	newNode->prev = temp;
}

//Prints all the elements in linked list in forward traversal order
void Print() {
	struct Node* temp = head;
	printf("Forward: ");
	while(temp != NULL) {
		printf("(%d,%d) ",temp->xCoord,temp->yCoord);
		temp = temp->next;
	}
	printf("\n");
}

//Prints all the elements in linked list in backward traversal order
void ReversePrint() {
	struct Node* temp = head;
	if(temp == NULL) return; // empty list, exit
	// Going to last Node
	while(temp->next != NULL) {
		temp = temp->next;
	}
	// Traversing backward using prev pointer
	printf("Reverse: ");
	while(temp != NULL) {
		printf("(%d,&d) ",temp->xCoord,temp->yCoord);
		temp = temp->prev;
	}
	printf("\n");
}



//int main(void) {
//variables for recording user's input on size of field, cell placement, etc.
//names should be self explanitonway's Game of Life\n\n");
/*	int xSize = 0;
	int ySize = 0;
	int numCycles = 1;
	int xStart = 0;
	int yStart = 0;
	int xStart2 = 0;
	int yStart2 = 0;
	int xStart3 = 0;
	int yStart3 = 0;
	char* widthBuff[8];
	char* heightBuff[8];
	char* cycleBuff[8];
	char* xStartBuff[8];
	char* yStartBuff[8];
	char* xStartBuff2[8];
	char* yStartBuff2[8];
	char* xStartBuff3[8];
	char* yStartBuff3[8];
	char* path[10000];
	char coordList[1000];
	int xList[10000];
	int yList[10000];
	FILE* fp;
	char* fieldpref[8];
	char* wrappref[8];
	int defSize = 40;
	int infinite = 0;
	int wrap = 0;*/
//void initField(int x, int y){
//field[x][y];

//}






void getInput(){
	//rules and input section
	printf("Welcome to Conway's Game of Life\n");
	printf("Rules:\n");
	printf("The game is played on a 2D orthogonal grid of square cells.\n");
	printf("The cells are either alive or dead.\n");
	printf("Every cell has eight neighbors adjacent horizontally, vertically, and diagonally\n");
	printf("Every turn allows for the following events:\n");
	printf("Any live cell with less than 2 live neighbors dies\n");
	printf("Any live cell with 2 or 3 live neighbors lives\n");
	printf("Any live cell wih 3 or more live neighbors dies\n");
	printf("Any dead cell with exactly 3 live neighbors becomes alive\n");
	printf("The rules are applied each turn until the game ends\n\n");

	printf("Enter preferred field type: I = infinite, B = bounded\n");
	scanf("%s", fieldpref);
	
	//if they go for infinite field set it to true
	if(fieldpref[0] == 'I' || fieldpref[0] == 'i')
	{
	infinite = 1;

	}

	//if we dont use infinite field do this	
	if(infinite == 0)
	{
		printf("Enter Width: \n");
		scanf("%s", widthBuff);
		xSize = atoi(widthBuff);
		printf("Enter Height: \n");
		scanf("%s", heightBuff);
		ySize = atoi(heightBuff);
	}

	//if we have an infinite field do this
	if(infinite == 1)
	{
		//set grid dimensions to some that are very big but fit on screen
		xSize = defSize;
		ySize = defSize;

	}


	printf("Enter preferred field wrap style: T = toroidal, S = standard\n");
	scanf("%s", wrappref);

	if(wrappref[0] == "T" || wrappref[0] == "t")
	{
		wrap = 1;
	}

	


	printf("Enter # of Cycles / Turns \n");
	scanf("%s", cycleBuff);
	numCycles = atoi(cycleBuff);
	printf("Width: %d Height: %d # of Cycles/Turns: %d\n", xSize, ySize, numCycles);
	
	printf("Enter # of Players supports 1,2,3: \n");
	scanf("%s", playerBuff);
	numPlayers = atoi(playerBuff);
	

	if(numPlayers == 1)
	{
	printf("Cycle by step or by time? (s for step t for time): \n");
	scanf("%s", stepbuff);
	

	if(stepbuff[0] == 's' || stepbuff[0] == 'S')
	{
	step = 1;
	}

	if(stepbuff[0] == 't' || stepbuff[0] == 'T')
	{
	time = 1;
	}
	
	if(time == 1)
	{
	//Get cycle delay
	printf("Enter cycle delay (in seconds): ");
	scanf("%s", sleepBuf);
	sleepTime = atoi(sleepBuf);
  	}

	if(step == 1)
	{
	sleepTime = 0;
	}

	}


}



	void cycleWork(){
	//2d array for field use
	char field[xSize][ySize];
	
	//initField(xSize, ySize);

	for(int a = 0; a < ySize; a++){
		for(int b = 0; b < xSize; b++){

			field[a][b] = '-';
		}
	}
	int exit = 0;

	//allows the user to input the coordinates they want cells at
	while(exit == 0)
	{
		printf("Enter Next X Coordinate or type exit to exit: ");
		scanf("%s",xStartBuff);
		xStart = atoi(xStartBuff);
		if(strcmp(xStartBuff,"exit") == 0)
		{
			exit = 1;
			break;
		}
		printf("Enter Next Y Coordinate or type exit to exit: ");
		scanf("%s",yStartBuff);
		yStart = atoi(yStartBuff);

		if(strcmp(yStartBuff,"exit") == 0)
		{
			exit = 1;
			break;
		}

		InsertAtHead(xStart,yStart);
		//field[yStart][xStart] = '+';
	}
	struct Node* temp = head;

	while(temp != NULL)
	{
		//insert into array version
		field[((temp->yCoord)%ySize)][((temp->xCoord)%xSize)] = '+';
		//printf("\ngoing\n");
		temp = temp->next;

	}	

	//int liveNeigh = 0;
	//end of first section
	

	for(int i = 0; i < numCycles; i++)
	{

		//print cycles
		printf("\nCycle %d\n", i);
		for(int a = 0; a < ySize; a++)
		{

			for(int b = 0; b < xSize; b++)
			{

				
				if(field[a][b] == 'm')
				{
				field[a][b] = '-';
				}

				if(field[a][b] == 'p')
				{
				field[a][b] = '+';
				}
				

				printf("|%c|",field[a][b]);
				

			}

			printf("\n");
		}
		//printf("\nxStart:%d yStart:%d \n",xStart,yStart);
		printf("\n\n");
	
		// amount of delay between cycles (in seconds)
		if(sleepTime != 0) {
			sleep(sleepTime);
		}


			//if we are stepping through cycles do this else use timer below
			if(step == 1)
			{
			stepping = 0;

			while(stepping == 0)
			{
			printf("\n enter n for next cycle: ");
			scanf("%s",stepper);
	
			if(stepper[0] == 'n' || stepper[0] == 'N')
			{
			stepping = 1;
			}

			}

			}


		//do checking of cells in each cycle
		//currently unimplemented need to get rid of -1 on for loop replace with checks on ifs for exceding boundary

		//new
		/*
			for(int a = 0; a < ySize; a++)
			{

			for(int b = 0; b < xSize; b++)
			{
			
				
				if(field[a][b] == 'm')
				{
				field[a][b] = '-';
				}

				if(field[a][b] == 'p')
				{
				field[a][b] = '+';
				}
				

			}

			
			}
			//end new
		*/	
			for(int a = 0; a < ySize - 1; a++)
			{

			for(int b = 0; b < xSize - 1; b++)
			{

				//start
				if(field[a][b] == '-')
				{
					liveNeigh = 0;

					if(field[a+1][b] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a][b+1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a-1][b] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a][b-1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a+1][b+1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a-1][b-1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a-1][b+1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a+1][b-1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(liveNeigh == 3)
					{
						//did this for important reasons
						field[a][b] = 'p';
				printf("\nNeighbors: %d  Coords:(%d,%d)\n",liveNeigh,b,a);

					}

				}

			//end add phase
		}
		}


		for(int a = 0; a < ySize - 1; a++)
		{

			for(int b = 0; b < xSize - 1; b++)
			{



				if(field[a][b] == '+')
				{
					liveNeigh = 0;
					//check 1
					if(field[a+1][b] == '+' || field[a+1][b] == 'm')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 2
					if(field[a+1][b+1] == '+' || field[a+1][b+1] == 'm')
					{
						liveNeigh = liveNeigh + 1;
					}
					//chck 3
					if(field[a-1][b-1] == '+' || field[a-1][b-1] == 'm')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 4
					if(field[a][b+1] == '+' || field[a][b+1] == 'm')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 5
					if(field[a-1][b] == '+' || field[a-1][b] == 'm')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 6
					if(field[a][b-1] == '+' || field[a][b-1] == 'm')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 7
					if(field[a-1][b+1] == '+' || field[a-1][b+1] == 'm')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 8
					if(field[a+1][b-1] == '+' || field[a+1][b-1] == 'm')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(liveNeigh > 3 || liveNeigh <=1)
					{

						field[a][b] = 'm';

					}
					printf("\nNeighbors: %d  Coords:(%d,%d)\n",liveNeigh,b,a);


				}//end of first loop


			}
/*
			for(int x = 0; x < ySize; x++)
			{
				for(int y = 0; y < xSize; y++)
				{
					if(field[x][y] == 'p')
					{
						field[x][y] = '+';
					}

					if(field[x][y] == 'm')
					{
						field[x][y] = '-';
					}

				}
			}

			*/
		}



	}




	return;
}


void twoPlayer()
{
printf("\nwe have 2 players\n");
}



void threePlayer()
{
printf("\nwe have 3 players\n");
}




void performCycle(int players)
{

if(players == 1)
{
cycleWork();
}

if(players == 2)
{

twoPlayer();

}

if(players == 3)
{
threePlayer();
}

}






int main(void){
getInput();

performCycle(numPlayers);

//cycleWork();

}
