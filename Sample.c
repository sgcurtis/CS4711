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
char field[100][100];

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

	if(wrappref[0] == 'T' || wrappref[0] == 't')
	{
		wrap = 1;
	}

	


	printf("Enter # of Cycles / Turns \n");
	scanf("%s", cycleBuff);
	numCycles = atoi(cycleBuff);
	printf("Width: %d Height: %d # of Cycles/Turns: %d\n", xSize, ySize, numCycles);
	
	printf("Enter # of Players supports 1,2,3,4: \n");
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
	//start of cut chunk

	//2d array for field use
	//char field[xSize][ySize];
	
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
	

//start super mega cut
	for(int i = 0; i < numCycles; i++)
	{
		//exclude this
		//print cycles
		printf("\nCycle %d\n", i);

//need this
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
//end need

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

//end exclude
			for(int a = 0; a < ySize; a++)
			{

			for(int b = 0; b < xSize; b++)
			{

				//start
				if(field[a][b] == '-')
				{
					liveNeigh = 0;
					//Wraparound if a = 0 and b = 0
					if(wrap == 1 && a == 0 && b == 0){
						if(field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4' || field[a+1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4' || field[a][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
	
						if(field[ySize-1][b] == '1' || field[ySize-1][b] == '2' || field[ySize-1][b] == '3' || field[ySize-1][b] == '4' || field[ySize-1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][xSize-1] == '1' || field[a][xSize-1] == '2' || field[a][xSize-1] == '3' || field[a][xSize-1] == '4' || field[a][xSize-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4' || field[a+1][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[ySize-1][xSize-1] == '1' || field[ySize-1][xSize-1] == '2' || field[ySize-1][xSize-1] == '3' || field[ySize-1][xSize-1] == '4' || field[ySize-1][xSize-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[ySize-1][b+1] == '1' || field[ySize-1][b+1] == '2' || field[ySize-1][b+1] == '3' || field[ySize-1][b+1] == '4' || field[ySize-1][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a+1][xSize-1] == '1' || field[a+1][xSize-1] == '2' || field[a+1][xSize-1] == '3' || field[a+1][xSize-1] == '4' || field[a+1][xSize-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if a = 0 and b = xSize
					}else if(wrap == 1 && a == 0 && b == xSize-1){
						if(field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4' || field[a+1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][0] == '1' || field[a][0] == '2' || field[a][0] == '3' || field[a][0] == '4' || field[a][0] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
	
						if(field[ySize-1][b] == '1' || field[ySize-1][b] == '2' || field[ySize-1][b] == '3' || field[ySize-1][b] == '4' || field[ySize-1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4' || field[a][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a+1][0] == '1' || field[a+1][0] == '2' || field[a+1][0] == '3' || field[a+1][0] == '4' || field[a+1][0] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[ySize-1][b-1] == '1' || field[ySize-1][b-1] == '2' || field[ySize-1][b-1] == '3' || field[ySize-1][b-1] == '4' || field[ySize-1][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[ySize-1][0] == '1' || field[ySize-1][0] == '2' || field[ySize-1][0] == '3' || field[ySize-1][0] == '4' || field[ySize-1][0] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4' || field[a+1][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if a = ySize and b = 0
					}else if(wrap == 1 && a == ySize-1 && b == 0){
						if(field[0][b] == '1' || field[0][b] == '2' || field[0][b] == '3' || field[0][b] == '4' || field[0][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4' || field[a][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
	
						if(field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4' || field[a-1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][xSize-1] == '1' || field[a][xSize-1] == '2' || field[a][xSize-1] == '3' || field[a][xSize-1] == '4' || field[a][xSize-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[0][b+1] == '1' || field[0][b+1] == '2' || field[0][b+1] == '3' || field[0][b+1] == '4' || field[0][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a-1][xSize-1] == '1' || field[a-1][xSize-1] == '2' || field[a-1][xSize-1] == '3' || field[a-1][xSize-1] == '4' || field[a-1][xSize-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4' || field[a-1][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[0][xSize-1] == '1' || field[0][xSize-1] == '2' || field[0][xSize-1] == '3' || field[0][xSize-1] == '4' || field[0][xSize-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if a = ySize and b = xSize
					}else if(wrap == 1 && a == ySize-1 && b == xSize-1){
						if(field[0][b] == '1' || field[0][b] == '2' || field[0][b] == '3' || field[0][b] == '4' || field[0][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][0] == '1' || field[a][0] == '2' || field[a][0] == '3' || field[a][0] == '4' || field[a][0] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
	
						if(field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4' || field[a-1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4' || field[a][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[0][0] == '1' || field[0][0] == '2' || field[0][0] == '3' || field[0][0] == '4' || field[0][0] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4' || field[a-1][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a-1][0] == '1' || field[a-1][0] == '2' || field[a-1][0] == '3' || field[a-1][0] == '4' || field[a-1][0] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[0][b-1] == '1' || field[0][b-1] == '2' || field[0][b-1] == '3' || field[0][b-1] == '4' || field[0][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if a = 0
					}else if(wrap == 1 && a == 0){
						if(field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4' || field[a+1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4' || field[a][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
	
						if(field[ySize-1][b] == '1' || field[ySize-1][b] == '2' || field[ySize-1][b] == '3' || field[ySize-1][b] == '4' || field[ySize-1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4' || field[a][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4' || field[a+1][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[ySize-1][b-1] == '1' || field[ySize-1][b-1] == '2' || field[ySize-1][b-1] == '3' || field[ySize-1][b-1] == '4' || field[ySize-1][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[ySize-1][b+1] == '1' || field[ySize-1][b+1] == '2' || field[ySize-1][b+1] == '3' || field[ySize-1][b+1] == '4' || field[ySize-1][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4' || field[a+1][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if a = ySize
					}else if(wrap == 1 && a == ySize-1){
						if(field[0][b] == '1' || field[0][b] == '2' || field[0][b] == '3' || field[0][b] == '4' || field[0][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4' || field[a][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
	
						if(field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4' || field[a-1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4' || field[a][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[0][b+1] == '1' || field[0][b+1] == '2' || field[0][b+1] == '3' || field[0][b+1] == '4' || field[0][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4' || field[a-1][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4' || field[a-1][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[0][b-1] == '1' || field[0][b-1] == '2' || field[0][b-1] == '3' || field[0][b-1] == '4' || field[0][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if b = 0
					}else if(wrap == 1 && b == 0){
						if(field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4' || field[a+1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4' || field[a][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
	
						if(field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4' || field[a-1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][xSize-1] == '1' || field[a][xSize-1] == '2' || field[a][xSize-1] == '3' || field[a][xSize-1] == '4' || field[a][xSize-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4' || field[a+1][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a-1][xSize-1] == '1' || field[a-1][xSize-1] == '2' || field[a-1][xSize-1] == '3' || field[a-1][xSize-1] == '4' || field[a-1][xSize-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4' || field[a-1][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a+1][xSize-1] == '1' || field[a+1][xSize-1] == '2' || field[a+1][xSize-1] == '3' || field[a+1][xSize-1] == '4' || field[a+1][xSize-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
						//wraparound if b = xSize
					}else if(wrap == 1 && b == xSize-1){
						if(field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4' || field[a+1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][0] == '1' || field[a][0] == '2' || field[a][0] == '3' || field[a][0] == '4' || field[a][0] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
	
						if(field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4' || field[a-1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4' || field[a][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a+1][0] == '1' || field[a+1][0] == '2' || field[a+1][0] == '3' || field[a+1][0] == '4' || field[a+1][0] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4' || field[a-1][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a-1][0] == '1' || field[a-1][0] == '2' || field[a-1][0] == '3' || field[a-1][0] == '4' || field[a-1][0] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4' || field[a+1][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
						//No Wraparound
					}else{
						if(field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4' || field[a+1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4' || field[a][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
	
						if(field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4' || field[a-1][b] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4' || field[a][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4' || field[a+1][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4' || field[a-1][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4' || field[a-1][b+1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}

						if(field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4' || field[a+1][b-1] == '+')
						{
							liveNeigh = liveNeigh + 1;
						}
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


		for(int a = 0; a < ySize; a++)
		{

			for(int b = 0; b < xSize; b++)
			{



				if(field[a][b] == '1' || field[a][b] == '2' || field[a][b] == '3' || field[a][b] == '4' || field[a][b] == '+')
				{
					liveNeigh = 0;
					//Wraparound if a = 0 and b = 0
					if(wrap == 1 && a == 0 && b == 0){
						//check 1
						if(field[a+1][b] == '+' || field[a+1][b] == 'm' || field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 2
						if(field[a+1][b+1] == '+' || field[a+1][b+1] == 'm' || field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 3
						if(field[ySize-1][xSize-1] == '+' || field[ySize-1][xSize-1] == 'm' || field[ySize-1][xSize-1] == '1' || field[ySize-1][xSize-1] == '2' || field[ySize-1][xSize-1] == '3' || field[ySize-1][xSize-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 4
						if(field[a][b+1] == '+' || field[a][b+1] == 'm' || field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 5
						if(field[ySize-1][b] == '+' || field[ySize-1][b] == 'm' || field[ySize-1][b] == '1' || field[ySize-1][b] == '2' || field[ySize-1][b] == '3' || field[ySize-1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 6
						if(field[a][xSize-1] == '+' || field[a][xSize-1] == 'm' || field[a][xSize-1] == '1' || field[a][xSize-1] == '2' || field[a][xSize-1] == '3' || field[a][xSize-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 7
						if(field[ySize-1][b+1] == '+' || field[ySize-1][b+1] == 'm' || field[ySize-1][b+1] == '1' || field[ySize-1][b+1] == '2' || field[ySize-1][b+1] == '3' || field[ySize-1][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 8
						if(field[a+1][xSize-1] == '+' || field[a+1][xSize-1] == 'm' || field[a+1][xSize-1] == '1' || field[a+1][xSize-1] == '2' || field[a+1][xSize-1] == '3' || field[a+1][xSize-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if a = 0 and b = xSize
					}else if(wrap == 1 && a == 0 && b == xSize-1){
						//check 1
						if(field[a+1][b] == '+' || field[a+1][b] == 'm' || field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 2
						if(field[a+1][0] == '+' || field[a+1][0] == 'm' || field[a+1][0] == '1' || field[a+1][0] == '2' || field[a+1][0] == '3' || field[a+1][0] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 3
						if(field[ySize-1][b-1] == '+' || field[ySize-1][b-1] == 'm' || field[ySize-1][b-1] == '1' || field[ySize-1][b-1] == '2' || field[ySize-1][b-1] == '3' || field[ySize-1][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 4
						if(field[a][0] == '+' || field[a][0] == 'm' || field[a][0] == '1' || field[a][0] == '2' || field[a][0] == '3' || field[a][0] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 5
						if(field[ySize-1][b] == '+' || field[ySize-1][b] == 'm' || field[ySize-1][b] == '1' || field[ySize-1][b] == '2' || field[ySize-1][b] == '3' || field[ySize-1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 6
						if(field[a][b-1] == '+' || field[a][b-1] == 'm' || field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 7
						if(field[ySize-1][0] == '+' || field[ySize-1][0] == 'm' || field[ySize-1][0] == '1' || field[ySize-1][0] == '2' || field[ySize-1][0] == '3' || field[ySize-1][0] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 8
						if(field[a+1][b-1] == '+' || field[a+1][b-1] == 'm' || field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if a = ySize and b = 0
					}else if(wrap == 1 && a == ySize-1 && b == 0){
						//check 1
						if(field[0][b] == '+' || field[0][b] == 'm' || field[0][b] == '1' || field[0][b] == '2' || field[0][b] == '3' || field[0][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 2
						if(field[0][b+1] == '+' || field[0][b+1] == 'm' || field[0][b+1] == '1' || field[0][b+1] == '2' || field[0][b+1] == '3' || field[0][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 3
						if(field[a-1][xSize-1] == '+' || field[a-1][xSize-1] == 'm' || field[a-1][xSize-1] == '1' || field[a-1][xSize-1] == '2' || field[a-1][xSize-1] == '3' || field[a-1][xSize-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 4
						if(field[a][b+1] == '+' || field[a][b+1] == 'm' || field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 5
						if(field[a-1][b] == '+' || field[a-1][b] == 'm' || field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 6
						if(field[a][xSize-1] == '+' || field[a][xSize-1] == 'm' || field[a][xSize-1] == '1' || field[a][xSize-1] == '2' || field[a][xSize-1] == '3' || field[a][xSize-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 7
						if(field[a-1][b+1] == '+' || field[a-1][b+1] == 'm' || field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 8
						if(field[0][xSize-1] == '+' || field[0][xSize-1] == 'm' || field[0][xSize-1] == '1' || field[0][xSize-1] == '2' || field[0][xSize-1] == '3' || field[0][xSize-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if a = ySize and b = xSize
					}else if(wrap == 1 && a == ySize-1 && b == xSize-1){
						//check 1
						if(field[0][b] == '+' || field[0][b] == 'm' || field[0][b] == '1' || field[0][b] == '2' || field[0][b] == '3' || field[0][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 2
						if(field[0][0] == '+' || field[0][0] == 'm' || field[0][0] == '1' || field[0][0] == '2' || field[0][0] == '3' || field[0][0] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 3
						if(field[a-1][b-1] == '+' || field[a-1][b-1] == 'm' || field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 4
						if(field[a][0] == '+' || field[a][0] == 'm' || field[a][0] == '1' || field[a][0] == '2' || field[a][0] == '3' || field[a][0] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 5
						if(field[a-1][b] == '+' || field[a-1][b] == 'm' || field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 6
						if(field[a][b-1] == '+' || field[a][b-1] == 'm' || field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 7
						if(field[a-1][0] == '+' || field[a-1][0] == 'm' || field[a-1][0] == '1' || field[a-1][0] == '2' || field[a-1][0] == '3' || field[a-1][0] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 8
						if(field[0][b-1] == '+' || field[0][b-1] == 'm' || field[0][b-1] == '1' || field[0][b-1] == '2' || field[0][b-1] == '3' || field[0][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if a = 0
					}else if(wrap == 1 && a == 0){
						//check 1
						if(field[a+1][b] == '+' || field[a+1][b] == 'm' || field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 2
						if(field[a+1][b+1] == '+' || field[a+1][b+1] == 'm' || field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 3
						if(field[ySize-1][b-1] == '+' || field[ySize-1][b-1] == 'm' || field[ySize-1][b-1] == '1' || field[ySize-1][b-1] == '2' || field[ySize-1][b-1] == '3' || field[ySize-1][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 4
						if(field[a][b+1] == '+' || field[a][b+1] == 'm' || field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 5
						if(field[ySize-1][b] == '+' || field[ySize-1][b] == 'm' || field[ySize-1][b] == '1' || field[ySize-1][b] == '2' || field[ySize-1][b] == '3' || field[ySize-1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 6
						if(field[a][b-1] == '+' || field[a][b-1] == 'm' || field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 7
						if(field[ySize-1][b+1] == '+' || field[ySize-1][b+1] == 'm' || field[ySize-1][b+1] == '1' || field[ySize-1][b+1] == '2' || field[ySize-1][b+1] == '3' || field[ySize-1][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 8
						if(field[a+1][b-1] == '+' || field[a+1][b-1] == 'm' || field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if a = ySize
					}else if(wrap == 1 && a == ySize-1){
						//check 1
						if(field[0][b] == '+' || field[0][b] == 'm' || field[0][b] == '1' || field[0][b] == '2' || field[0][b] == '3' || field[0][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 2
						if(field[0][b+1] == '+' || field[0][b+1] == 'm' || field[0][b+1] == '1' || field[0][b+1] == '2' || field[0][b+1] == '3' || field[0][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 3
						if(field[a-1][b-1] == '+' || field[a-1][b-1] == 'm' || field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 4
						if(field[a][b+1] == '+' || field[a][b+1] == 'm' || field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 5
						if(field[a-1][b] == '+' || field[a-1][b] == 'm' || field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 6
						if(field[a][b-1] == '+' || field[a][b-1] == 'm' || field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 7
						if(field[a-1][b+1] == '+' || field[a-1][b+1] == 'm' || field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 8
						if(field[0][b-1] == '+' || field[0][b-1] == 'm' || field[0][b-1] == '1' || field[0][b-1] == '2' || field[0][b-1] == '3' || field[0][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if b = 0
					}else if(wrap == 1 && b == 0){
						//check 1
						if(field[a+1][b] == '+' || field[a+1][b] == 'm' || field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 2
						if(field[a+1][b+1] == '+' || field[a+1][b+1] == 'm' || field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 3
						if(field[a-1][xSize-1] == '+' || field[a-1][xSize-1] == 'm' || field[a-1][xSize-1] == '1' || field[a-1][xSize-1] == '2' || field[a-1][xSize-1] == '3' || field[a-1][xSize-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 4
						if(field[a][b+1] == '+' || field[a][b+1] == 'm' || field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 5
						if(field[a-1][b] == '+' || field[a-1][b] == 'm' || field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 6
						if(field[a][xSize-1] == '+' || field[a][xSize-1] == 'm' || field[a][xSize-1] == '1' || field[a][xSize-1] == '2' || field[a][xSize-1] == '3' || field[a][xSize-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 7
						if(field[a-1][b+1] == '+' || field[a-1][b+1] == 'm' || field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 8
						if(field[a+1][xSize-1] == '+' || field[a+1][xSize-1] == 'm' || field[a+1][xSize-1] == '1' || field[a+1][xSize-1] == '2' || field[a+1][xSize-1] == '3' || field[a+1][xSize-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//Wraparound if b = xSize
					}else if(wrap == 1 && b == xSize-1){
						//check 1
						if(field[a+1][b] == '+' || field[a+1][b] == 'm' || field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 2
						if(field[a+1][0] == '+' || field[a+1][0] == 'm' || field[a+1][0] == '1' || field[a+1][0] == '2' || field[a+1][0] == '3' || field[a+1][0] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 3
						if(field[a-1][b-1] == '+' || field[a-1][b-1] == 'm' || field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 4
						if(field[a][0] == '+' || field[a][0] == 'm' || field[a][0] == '1' || field[a][0] == '2' || field[a][0] == '3' || field[a][0] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 5
						if(field[a-1][b] == '+' || field[a-1][b] == 'm' || field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 6
						if(field[a][b-1] == '+' || field[a][b-1] == 'm' || field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 7
						if(field[a-1][0] == '+' || field[a-1][0] == 'm' || field[a-1][0] == '1' || field[a-1][0] == '2' || field[a-1][0] == '3' || field[a-1][0] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 8
						if(field[a+1][b-1] == '+' || field[a+1][b-1] == 'm' || field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//No Wraparound
					}else{
						//check 1
						if(field[a+1][b] == '+' || field[a+1][b] == 'm' || field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 2
						if(field[a+1][b+1] == '+' || field[a+1][b+1] == 'm' || field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 3
						if(field[a-1][b-1] == '+' || field[a-1][b-1] == 'm' || field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 4
						if(field[a][b+1] == '+' || field[a][b+1] == 'm' || field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 5
						if(field[a-1][b] == '+' || field[a-1][b] == 'm' || field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 6
						if(field[a][b-1] == '+' || field[a][b-1] == 'm' || field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 7
						if(field[a-1][b+1] == '+' || field[a-1][b+1] == 'm' || field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
						//check 8
						if(field[a+1][b-1] == '+' || field[a+1][b-1] == 'm' || field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4')
						{
							liveNeigh = liveNeigh + 1;
						}
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

//end super mega cut


	return;
}


void twoPlayer()
{
printf("\nwe have 2 players\n");
printf("\nhere are the rules: \n");
printf("\nEach Players Live cell is their player number eg. Player1's live cells are marked 1\n");
printf("\nEnter k to kill a cell and s to spawn one, and v is to view the board\n");
printf("\nYou get 3 moves\n");
printf("\nYou cannot directly kill an opponents cell\n");
printf("\nthe last player standing or the one with the most cells at the end of the game is the winner\n");
printf("\nNote: All other standard rules apply\n");
printf("\nGood Luck\n");

//start 2p
int p1Turn = 0;
int p2Turn = 0;
int p1 = 1;
int p2 = 1;
int p1c = 0;
int p2c = 0;
char* choice[20];
char pChoice = 'v';
char xcoBuff[20];
char ycoBuff[20];
char xcoc;
char ycoc;
int xco = 0;
int yco = 0;


	//2d array for field use
	//char field[xSize][ySize];
	
	//initField(xSize, ySize);

	for(int a = 0; a < ySize; a++){
		for(int b = 0; b < xSize; b++){

			field[a][b] = '-';
		}
	}


while(numCycles > 0)
{

//start of turn
numCycles = numCycles - 1;

p1Turn = 3;
p2Turn = 3;

if(p1 == 0)
{
printf("\nPlayer 1 is dead\n");
printf("\nPlayer 2 wins\n");
p1Turn = 0;
break;
}


if(p2 == 0)
{
printf("\nPlayer 2 is dead\n");
p2Turn = 0;
printf("\nPlayer 1 Wins\n");
break;
}


while(p1Turn != 0)
{
printf("\nMoves Remaining: %d\n", p1Turn);
printf("\nPlayer1's Turn:");
scanf("%s", choice);
pChoice = choice[0];
//display grid

//vcut
if(pChoice == 'v')
{
printf("\nXSIZE: %d\n \nYSIZE: %d\n",xSize,ySize);

for(int a = 0; a < ySize; a++)
{

for(int b = 0; b < xSize; b++)
{

printf("|%c|",field[a][b]);

}
printf("\n");
}

}
//end vcut

//start kcut
//kill one of your cells
if(pChoice == 'k')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{
yco = atoi(ycoBuff);
if((char)field[yco][xco] != '2')
{
printf("\nKilling cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '-';
p1Turn = p1Turn - 1;
}
//end cut
else
{
printf("\nError that is not your Cell\n");
}

}

}



}
//end kcut

//start scut
//spawn cell
if(pChoice == 's')
{
//start cut
printf("\nEnter X-coord of cell to spawn or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to spawn or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{


yco = atoi(ycoBuff);


if(field[xco][yco] != '-')
{
printf("\nError there is already a live cell there\n");
}

if(field[yco][xco] == '-')
{
printf("\nSpawning cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '1';
p1Turn = p1Turn - 1;
}



}

}

}
//end s cut
}

printf("\nPlayer2's Turn:");
while(p2Turn != 0)
{
printf("\nMoves Remaining: %d\n", p2Turn);
printf("\nPlayer2's Turn:");
scanf("%s", choice);
pChoice = choice[0];
//display grid


//vcut
if(pChoice == 'v')
{
printf("\nXSIZE: %d\n \nYSIZE: %d\n",xSize,ySize);

for(int a = 0; a < ySize; a++)
{

for(int b = 0; b < xSize; b++)
{

printf("|%c|",field[a][b]);

}
printf("\n");
}

}
//end vcut

//start kcut
//kill one of your cells
if(pChoice == 'k')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{
yco = atoi(ycoBuff);
if(field[yco][xco] != '1')
{
printf("\nKilling cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '-';
p2Turn = p2Turn - 1;
}
//end cut
else
{
printf("\nError that is not your Cell\n");
}

}

}



}
//end kcut

//start scut
//spawn cell
if(pChoice == 's')
{
//start cut
printf("\nEnter X-coord of cell to spawn or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to spawn or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{


yco = atoi(ycoBuff);


if(field[yco][xco] != '-')
{
printf("\nError their is already a live cell there\n");
}

if(field[yco][xco] == '-')
{
printf("\nSpawning cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '2';
p2Turn = p2Turn - 1;
}


}

}

}
//end s cut


//p2Turn = p2Turn - 1;
}

//code to apply rules goes here




//start super mega cut
	for(int i = 0; i < numCycles; i++)
	{
			for(int a = 0; a < ySize - 1; a++)
			{

			for(int b = 0; b < xSize - 1; b++)
			{

				//start
				if(field[a][b] == '-')
				{
					liveNeigh = 0;

					if(field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4' || field[a+1][b] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4' || field[a][b+1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4' || field[a-1][b] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4' || field[a][b-1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4' || field[a+1][b+1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4' || field[a-1][b-1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4' || field[a-1][b+1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4' || field[a+1][b-1] == '+')
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



				if(field[a][b] == '1' || field[a][b] == '2' || field[a][b] == '3' || field[a][b] == '4' || field[a][b] == '+')
				{
					liveNeigh = 0;
					//check 1
					if(field[a+1][b] == '+' || field[a+1][b] == 'm' || field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 2
					if(field[a+1][b+1] == '+' || field[a+1][b+1] == 'm' || field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 3
					if(field[a-1][b-1] == '+' || field[a-1][b-1] == 'm' || field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 4
					if(field[a][b+1] == '+' || field[a][b+1] == 'm' || field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 5
					if(field[a-1][b] == '+' || field[a-1][b] == 'm' || field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 6
					if(field[a][b-1] == '+' || field[a][b-1] == 'm' || field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 7
					if(field[a-1][b+1] == '+' || field[a-1][b+1] == 'm' || field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 8
					if(field[a+1][b-1] == '+' || field[a+1][b-1] == 'm' || field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4')
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

//end super mega cut


//need this
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
				

				//printf("|%c|",field[a][b]);

			}

		}
//end need
//end actual cut



//code to check for deaths
p1c = 0;
p2c = 0;
	for(int a = 0; a < ySize; a++){
		for(int b = 0; b < xSize; b++){

			if(field[a][b] == '1')
			{
			p1c = p1c + 1;
			}

			if(field[a][b] == '2')
			{
			p2c = p2c + 1;
			}
			
			
		
		}
	}
//printf("\np1c: %d\n",p1c);
//printf("\np2c: %d\n",p2c);
//check for losers
if(p1c == 0)
{
//printf("\nin p1\n");
p1 = 0;
}

if(p2c == 0)
{
//printf("\nin p2\n");
p2 = 0;
}

}
//end 2p

if(p1c >= p2c)
{
printf("\nGame Over Player 1 Wins\n");
}
if(p2c >= p1c)
{
printf("\nGame Over Player 2 Wins\n");
}



}

void threePlayer()
{
//start super cut
printf("\nwe have 3 players\n");
printf("\nhere are the rules: \n");
printf("\nEach Players Live cell is their player number eg. Player1's live cells are marked 1\n");
printf("\nEnter k to kill a cell and s to spawn one\n");
printf("\nYou get 3 moves\n");
printf("\nYou cannot directly kill an opponents cell\n");
printf("\nthe last player standing or the one with the most cells at the end of the game is the winner\n");
printf("\nNote: All other standard rules apply\n");

printf("\nGood Luck\n");

//start 2p
int p1Turn = 0;
int p2Turn = 0;
int p3Turn = 0;
int p1 = 1;
int p2 = 1;
int p3 = 1;
int p1c = 0;
int p2c = 0;
int p3c = 0;
char* choice[20];
char pChoice = 'v';
char xcoBuff[20];
char ycoBuff[20];
char xcoc;
char ycoc;
int xco = 0;
int yco = 0;

	//2d array for field use
	//char field[xSize][ySize];
	
	//initField(xSize, ySize);

	for(int a = 0; a < ySize; a++){
		for(int b = 0; b < xSize; b++){

			field[a][b] = '-';
		}
	}


while(numCycles > 0)
{
//start of turn
numCycles = numCycles - 1;

p1Turn = 3;
p2Turn = 3;
p3Turn = 3;

if(p1 == 0)
{
printf("\nPlayer 1 is dead\n");
p1Turn = 0;
}

if(p2 == 0)
{
printf("\nPlayer 2 is dead\n");
p2Turn = 0;
}


if(p3 == 0)
{
printf("\nPlayer 3 is dead\n");
p3Turn = 0;
}


if(p3 == 0 && p2 == 0)
{
printf("\nPlayer 1 Wins\n");
p3Turn = 0;
break;
}

if(p1 == 0 && p3 == 0)
{
printf("\nPlayer 2 Wins\n");
break;
}


if(p1 == 0 && p2 == 0)
{
printf("\nPlayer 3 Wins\n");
break;
}



while(p1Turn != 0)
{
printf("\nMoves Remaining: %d\n", p1Turn);
printf("\nPlayer1's Turn:");
scanf("%s", choice);
pChoice = choice[0];
//display grid

//vcut
if(pChoice == 'v')
{
printf("\nXSIZE: %d\n \nYSIZE: %d\n",xSize,ySize);

for(int a = 0; a < ySize; a++)
{

for(int b = 0; b < xSize; b++)
{

printf("|%c|",field[a][b]);

}
printf("\n");
}

}
//end vcut

//start kcut
//kill one of your cells
if(pChoice == 'k')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{
yco = atoi(ycoBuff);
if((char)field[yco][xco] != '2' && (char)field[yco][xco] != '3')
{
printf("\nKilling cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '-';
p1Turn = p1Turn - 1;
}
//end cut
else
{
printf("\nError that is not your Cell\n");
}

}

}



}
//end kcut

//start scut
//spawn cell
if(pChoice == 's')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{


yco = atoi(ycoBuff);

if(field[yco][xco] != '-')
{
printf("\nError there is a live cell there already\n");
}


if(field[yco][xco] == '-')
{
printf("\nSpawning cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '1';
p1Turn = p1Turn - 1;
}



}

}

}
//end s cut
}

printf("\nPlayer2's Turn:");
while(p2Turn != 0)
{
printf("\nMoves Remaining: %d\n", p2Turn);
printf("\nPlayer2's Turn:");
scanf("%s", choice);
pChoice = choice[0];
//display grid


//vcut
if(pChoice == 'v')
{
printf("\nXSIZE: %d\n \nYSIZE: %d\n",xSize,ySize);

for(int a = 0; a < ySize; a++)
{

for(int b = 0; b < xSize; b++)
{

printf("|%c|",field[a][b]);

}
printf("\n");
}

}
//end vcut

//start kcut
//kill one of your cells
if(pChoice == 'k')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{
yco = atoi(ycoBuff);
if(field[yco][xco] != '1' && field[yco][xco] != '3')
{
printf("\nKilling cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '-';
p2Turn = p2Turn - 1;
}
//end cut
else
{
printf("\nError that is not your Cell\n");
}

}

}



}
//end kcut

//start s cut
//spawn cell
if(pChoice == 's')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{


yco = atoi(ycoBuff);



if(field[yco][xco] != '-')
{
printf("\nError there is already a live cell there\n");
}


if(field[yco][xco] == '-')
{
printf("\nSpawning cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '2';
p2Turn = p2Turn - 1;
}



}

}

}
//end s cut


//p2Turn = p2Turn - 1;
}

while(p3Turn != 0)
{
printf("\nMoves Remaining: %d\n", p3Turn);
printf("\nPlayer3's Turn:");
scanf("%s", choice);
pChoice = choice[0];

//display grid
//vcut
if(pChoice == 'v')
{
printf("\nXSIZE: %d\n \nYSIZE: %d\n",xSize,ySize);

for(int a = 0; a < ySize; a++)
{

for(int b = 0; b < xSize; b++)
{

printf("|%c|",field[a][b]);

}
printf("\n");
}

}
//end vcut

//start kcut
//kill one of your cells
if(pChoice == 'k')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{
yco = atoi(ycoBuff);

if((char)field[yco][xco] != '1' && (char)field[yco][xco] != '2')
{

//printf("\n%c\n",field[yco][xco]);
printf("\nKilling cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '-';
p3Turn = p3Turn - 1;

}
//end cut
else
{
printf("\nError that is not your Cell\n");
}

}

}



}
//end kcut

//start s cut
//spawn cell
if(pChoice == 's')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{


yco = atoi(ycoBuff);

if(field[yco][xco] != '-')
{
printf("\nError there is already a live cell there\n");
}

if(field[yco][xco] == '-')
{
printf("\nSpawning cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '3';
p3Turn = p3Turn - 1;
}



}

}

}
//end s cut


}



//start super mega cut
	for(int i = 0; i < numCycles; i++)
	{
			for(int a = 0; a < ySize - 1; a++)
			{

			for(int b = 0; b < xSize - 1; b++)
			{

				//start
				if(field[a][b] == '-')
				{
					liveNeigh = 0;

					if(field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4' || field[a+1][b] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4' || field[a][b+1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4' || field[a-1][b] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4' || field[a][b-1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4' || field[a+1][b+1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4' || field[a-1][b-1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4' || field[a-1][b+1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4' || field[a+1][b-1] == '+')
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



				if(field[a][b] == '1' || field[a][b] == '2' || field[a][b] == '3' || field[a][b] == '4' || field[a][b] == '+')
				{
					liveNeigh = 0;
					//check 1
					if(field[a+1][b] == '+' || field[a+1][b] == 'm' || field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 2
					if(field[a+1][b+1] == '+' || field[a+1][b+1] == 'm' || field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 3
					if(field[a-1][b-1] == '+' || field[a-1][b-1] == 'm' || field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 4
					if(field[a][b+1] == '+' || field[a][b+1] == 'm' || field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 5
					if(field[a-1][b] == '+' || field[a-1][b] == 'm' || field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 6
					if(field[a][b-1] == '+' || field[a][b-1] == 'm' || field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 7
					if(field[a-1][b+1] == '+' || field[a-1][b+1] == 'm' || field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 8
					if(field[a+1][b-1] == '+' || field[a+1][b-1] == 'm' || field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4')
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

//end super mega cut


//need this
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
				

				//printf("|%c|",field[a][b]);

			}

		}
//end need
//end actual cut



//code to check for deaths
p1c = 0;
p2c = 0;
p3c = 0;
	for(int a = 0; a < ySize; a++){
		for(int b = 0; b < xSize; b++){

			if(field[a][b] == '1')
			{
			p1c = p1c + 1;
			}

			if(field[a][b] == '2')
			{
			p2c = p2c + 1;
			}


			if(field[a][b] == '3')
			{
			p3c = p3c + 1;
			}
			
			
		
		}
	}
//printf("\np1c: %d\n",p1c);
//printf("\np2c: %d\n",p2c);
//check for losers
if(p1c == 0)
{
//printf("\nin p1\n");
p1 = 0;
}

if(p2c == 0)
{
//printf("\nin p2\n");
p2 = 0;
}


if(p3c == 0)
{
//printf("\nin p2\n");
p3 = 0;
}


}
//end 2p

if(p1c >= p2c && p1c >= p3c)
{
printf("\nGame Over Player 1 Wins\n");
}

if(p2c >= p1c && p2c >= p3c)
{
printf("\nGame Over Player 2 Wins\n");
}

if(p3c >= p1c && p3c >= p2c)
{
printf("\nGame Over Player 3 Wins\n");
}

}


void fourPlayer()
{
printf("\nwe have 4 players\n");
printf("\nhere are the rules: \n");
printf("\nEach Players Live cell is their player number eg. Player1's live cells are marked 1\n");
printf("\nEnter k to kill a cell and s to spawn one\n");
printf("\nYou get 3 moves\n");
printf("\nYou cannot directly kill an opponents cell\n");
printf("\nthe last player standing or the one with the most cells at the end of the game is the winner\n");
printf("\nNote: All other standard rules apply\n");

//start 2p
int p1Turn = 0;
int p2Turn = 0;
int p3Turn = 0;
int p4Turn = 0;
int p1 = 1;
int p2 = 1;
int p3 = 1;
int p4 = 1;
int p1c = 0;
int p2c = 0;
int p3c = 0;
int p4c = 0;
char* choice[20];
char pChoice = 'v';
char xcoBuff[20];
char ycoBuff[20];
char xcoc;
char ycoc;
int xco = 0;
int yco = 0;

	//2d array for field use
	//char field[xSize][ySize];
	
	//initField(xSize, ySize);

	for(int a = 0; a < ySize; a++){
		for(int b = 0; b < xSize; b++){

			field[a][b] = '-';
		}
	}


while(numCycles > 0)
{
//start of turn
numCycles = numCycles - 1;

p1Turn = 3;
p2Turn = 3;
p3Turn = 3;
p4Turn = 3;


if(p1 == 0)
{
printf("\nPlayer 1 is dead\n");
p1Turn = 0;
}

if(p2 == 0)
{
printf("\nPlayer 2 is dead\n");
p2Turn = 0;
}


if(p3 == 0)
{
printf("\nPlayer 3 is dead\n");
p3Turn = 0;
}


if(p4 == 0)
{
printf("\nPlayer 4 is dead\n");
p4Turn = 0;
}


if(p3 == 0 && p2 == 0 && p4 == 0)
{
printf("\nPlayer 1 Wins\n");
p3Turn = 0;
break;
}

if(p1 == 0 && p3 == 0 && p4 == 0)
{
printf("\nPlayer 2 Wins\n");
break;
}


if(p1 == 0 && p2 == 0 && p4 == 0)
{
printf("\nPlayer 3 Wins\n");
break;
}

if(p1 == 0 && p2 == 0 && p3 == 0)
{
printf("\nPlayer 4 Wins\n");
break;
}


while(p1Turn != 0)
{
printf("\nMoves Remaining: %d\n", p1Turn);
printf("\nPlayer1's Turn:");
scanf("%s", choice);
pChoice = choice[0];
//display grid

//vcut
if(pChoice == 'v')
{
printf("\nXSIZE: %d\n \nYSIZE: %d\n",xSize,ySize);

for(int a = 0; a < ySize; a++)
{

for(int b = 0; b < xSize; b++)
{

printf("|%c|",field[a][b]);

}
printf("\n");
}

}
//end vcut

//start kcut
//kill one of your cells
if(pChoice == 'k')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{
yco = atoi(ycoBuff);

if((char)field[yco][xco] != '2' && (char)field[yco][xco] != '3' && field[yco][xco] != '4')
{
printf("\nKilling cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '-';
p1Turn = p1Turn - 1;
}
//end cut
else
{
printf("\nError that is not your Cell\n");
}

}

}



}
//end kcut

//start scut
//spawn cell
if(pChoice == 's')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{
yco = atoi(ycoBuff);


if(field[yco][xco] != '-')
{
printf("Error there is already a live cell there");
}


if(field[yco][xco] == '-')
{
printf("\nSpawning cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '1';
p1Turn = p1Turn - 1;
}



}

}

}
//end s cut
}


//start p2 turn
printf("\nPlayer2's Turn:");
while(p2Turn != 0)
{
printf("\nMoves Remaining: %d\n", p2Turn);
printf("\nPlayer2's Turn:");
scanf("%s", choice);
pChoice = choice[0];
//display grid


//vcut
if(pChoice == 'v')
{
printf("\nXSIZE: %d\n \nYSIZE: %d\n",xSize,ySize);

for(int a = 0; a < ySize; a++)
{

for(int b = 0; b < xSize; b++)
{

printf("|%c|",field[a][b]);

}
printf("\n");
}

}
//end vcut

//start kcut
//kill one of your cells
if(pChoice == 'k')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{
yco = atoi(ycoBuff);
if(field[yco][xco] != '1' && field[yco][xco] != '3' && field[yco][xco] != '4')
{
printf("\nKilling cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '-';
p2Turn = p2Turn - 1;
}
//end cut
else
{
printf("\nError that is not your Cell\n");
}

}

}



}
//end kcut

//start s cut
//spawn cell
if(pChoice == 's')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{


yco = atoi(ycoBuff);



if(field[yco][xco] != '-')
{
printf("\nErrorr there is already a live cell there\n");
}

if(field[yco][xco] == '-')
{
printf("\nSpawning cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '2';
p2Turn = p2Turn - 1;
}



}

}

}
//end s cut


//p2Turn = p2Turn - 1;
}
//end p2


while(p3Turn != 0)
{
printf("\nMoves Remaining: %d\n", p3Turn);
printf("\nPlayer3's Turn:");
scanf("%s", choice);
pChoice = choice[0];

//display grid
//vcut
if(pChoice == 'v')
{
printf("\nXSIZE: %d\n \nYSIZE: %d\n",xSize,ySize);

for(int a = 0; a < ySize; a++)
{

for(int b = 0; b < xSize; b++)
{

printf("|%c|",field[a][b]);

}
printf("\n");
}

}
//end vcut

//start kcut
//kill one of your cells
if(pChoice == 'k')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{
yco = atoi(ycoBuff);

if((char)field[yco][xco] != '1' && (char)field[yco][xco] != '2' && field[yco][xco] != '4')
{

//printf("\n%c\n",field[yco][xco]);
printf("\nKilling cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '-';
p3Turn = p3Turn - 1;

}
//end cut
else
{
printf("\nError that is not your Cell\n");
}

}

}



}
//end kcut

//start s cut
//spawn cell
if(pChoice == 's')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{


yco = atoi(ycoBuff);
printf("\nSpawning cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '3';
p3Turn = p3Turn - 1;

}

}

}
//end s cut


}
printf("\nPlayer4's Turn:");
while(p4Turn != 0)
{
printf("\nMoves Remaining: %d\n", p4Turn);
printf("\nPlayer4's Turn:");
scanf("%s", choice);
pChoice = choice[0];
//display grid


//vcut
if(pChoice == 'v')
{
printf("\nXSIZE: %d\n \nYSIZE: %d\n",xSize,ySize);

for(int a = 0; a < ySize; a++)
{

for(int b = 0; b < xSize; b++)
{

printf("|%c|",field[a][b]);

}
printf("\n");
}

}
//end vcut

//start kcut
//kill one of your cells
if(pChoice == 'k')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{
yco = atoi(ycoBuff);

if(field[yco][xco] != '1' && field[yco][xco] != '3' && field[yco][xco] != '2')
{
printf("\nKilling cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '-';
p4Turn = p4Turn - 1;
}
//end cut
else
{
printf("\nError that is not your Cell\n");
}

}

}



}
//end kcut

//start s cut
//spawn cell
if(pChoice == 's')
{
//start cut
printf("\nEnter X-coord of cell to kill or b to go back:\n");
scanf("%s", xcoBuff);

if((char)xcoBuff[0] != 'b')
{
xco = atoi(xcoBuff);
printf("\n%d\n",xco);

printf("\nEnter Y-coord of cell to kill or b to go back:");
scanf("%s",ycoBuff);


if((char)ycoBuff[0] != 'b')
{


yco = atoi(ycoBuff);


if(field[yco][xco] != '-')
{
printf("\nError there is already a live cell there\n");
}

if(field[yco][xco] == '-')
{
printf("\nSpawning cell at (%d,%d)\n",xco,yco);
field[yco][xco] = '4';
p4Turn = p4Turn - 1;
}



}

}

}
//end s cut


//p4Turn = p4Turn - 1;
}
//end p2



//start super mega cut
	for(int i = 0; i < numCycles; i++)
	{
			for(int a = 0; a < ySize - 1; a++)
			{

			for(int b = 0; b < xSize - 1; b++)
			{

				//start
				if(field[a][b] == '-')
				{
					liveNeigh = 0;

					if(field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4' || field[a+1][b] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4' || field[a][b+1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4' || field[a-1][b] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4' || field[a][b-1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4' || field[a+1][b+1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4' || field[a-1][b-1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4' || field[a-1][b+1] == '+')
					{
						liveNeigh = liveNeigh + 1;
					}

					if(field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4' || field[a+1][b-1] == '+')
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



				if(field[a][b] == '1' || field[a][b] == '2' || field[a][b] == '3' || field[a][b] == '4' || field[a][b] == '+')
				{
					liveNeigh = 0;
					//check 1
					if(field[a+1][b] == '+' || field[a+1][b] == 'm' || field[a+1][b] == '1' || field[a+1][b] == '2' || field[a+1][b] == '3' || field[a+1][b] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 2
					if(field[a+1][b+1] == '+' || field[a+1][b+1] == 'm' || field[a+1][b+1] == '1' || field[a+1][b+1] == '2' || field[a+1][b+1] == '3' || field[a+1][b+1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 3
					if(field[a-1][b-1] == '+' || field[a-1][b-1] == 'm' || field[a-1][b-1] == '1' || field[a-1][b-1] == '2' || field[a-1][b-1] == '3' || field[a-1][b-1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 4
					if(field[a][b+1] == '+' || field[a][b+1] == 'm' || field[a][b+1] == '1' || field[a][b+1] == '2' || field[a][b+1] == '3' || field[a][b+1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 5
					if(field[a-1][b] == '+' || field[a-1][b] == 'm' || field[a-1][b] == '1' || field[a-1][b] == '2' || field[a-1][b] == '3' || field[a-1][b] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 6
					if(field[a][b-1] == '+' || field[a][b-1] == 'm' || field[a][b-1] == '1' || field[a][b-1] == '2' || field[a][b-1] == '3' || field[a][b-1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 7
					if(field[a-1][b+1] == '+' || field[a-1][b+1] == 'm' || field[a-1][b+1] == '1' || field[a-1][b+1] == '2' || field[a-1][b+1] == '3' || field[a-1][b+1] == '4')
					{
						liveNeigh = liveNeigh + 1;
					}
					//check 8
					if(field[a+1][b-1] == '+' || field[a+1][b-1] == 'm' || field[a+1][b-1] == '1' || field[a+1][b-1] == '2' || field[a+1][b-1] == '3' || field[a+1][b-1] == '4')
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

//end super mega cut


//need this
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
				

				//printf("|%c|",field[a][b]);

			}

		}
//end need
//end actual cut



//code to check for deaths
p1c = 0;
p2c = 0;
p3c = 0;
	for(int a = 0; a < ySize; a++){
		for(int b = 0; b < xSize; b++){

			if(field[a][b] == '1')
			{
			p1c = p1c + 1;
			}

			if(field[a][b] == '2')
			{
			p2c = p2c + 1;
			}


			if(field[a][b] == '3')
			{
			p3c = p3c + 1;
			}
			
			
		
		}
	}

//apply rules here


}
//end 2p


if(p1c >= p2c && p1c >= p3c && p1c >= p4c)
{
printf("\nGame Over Player 1 Wins\n");
}

if(p2c >= p1c && p2c >= p3c && p2c >= p4c)
{
printf("\nGame Over Player 2 Wins\n");
}

if(p3c >= p1c && p3c >= p2c && p3c >= p4c)
{
printf("\nGame Over Player 3 Wins\n");
}

if(p4c >= p1c && p4c >= p2c && p4c >= p3c)
{
printf("\nGame Over Player 4 Wins\n");
}




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

if(players == 4)
{
fourPlayer();
}


}


void save(){ 
       //work for save and load 11/6
        char* saveStr[100];
        printf("\n");
        printf("Would you like to save field? Type yes or no.");
	printf("\n");
	scanf("%s", saveStr);
	if(strcmp(saveStr, "no") == 0)
	{
	  printf("Goodbye\n");
	}
	if(strcmp(saveStr, "yes") == 0)
	{
	  printf("Saved\n");
	  FILE *f = fopen("sample.data", "wb");
	  fprintf(f, "%d\n", xSize);
	  fprintf(f, "%d\n", ySize);
	  fwrite(field, sizeof(char), sizeof(field), f);
	  fclose(f);
	}
}

void load(){
	char* loadStr[100];
	printf("\n");
        printf("Would you like to load a field? Type yes or no.");
	printf("\n");
	scanf("%s", loadStr);
	if(strcmp(loadStr, "no") == 0)
	{
	  printf("Goodbye\n");
	}
	if(strcmp(loadStr, "yes") == 0)
	{
	  FILE *file;
	  file = fopen("sample.data", "r");

	  if (file == NULL)
	    {
	      printf("no such file\n");
	      return 0;
	    }
	  
	  //read file into array
	  char* xSize[100];
	  char* ySize[100];
	  
	  fscanf(file, "%s", &xSize);
	  fscanf(file, "%s", &ySize);

	  int x = atoi(xSize);
	  int y = atoi(ySize);
	  int size = x*y + 1;
	  //char buf[size];
	  char buf[10000];

	  fscanf(file, "%s", &buf);

	  int xDim;
	  int yDim;

	  int counter = 0;
	  int rowCounter = 0;
	  for (yDim = 0; yDim < 100; yDim++)
	    {
	      for (xDim = 0; xDim < 100; xDim++)
		{
		  if ((buf[counter] == '+') || (buf[counter] == '-'))
		    {
		      //rowCounter++;
		      if (rowCounter == x)
			{
			putchar('\n');
			rowCounter = 0;
			}
		      printf("|");
		      printf("%c", buf[counter]);
		      printf("|");
		      rowCounter++;
		      counter++;
		    }
		  else if (buf[counter] == 'm')
		    {
		      if (rowCounter == x)
			{
			putchar('\n');
			rowCounter = 0;
			}
		      printf("|");
		      printf("+");
		      printf("|");
		      rowCounter++;
		      counter++;
		    }
		  else if (buf[counter] == 'p')
		    {
		      if (rowCounter == x)
			{
			putchar('\n');
			rowCounter = 0;
			}
		      printf("|");
		      printf("-");
		      printf("|");
		      rowCounter++;
		      counter++;
		    }
		  else 
		    {
		      //printf("|");
		      //printf("%c", buf[counter]);
		      // printf("|");
		      counter++;
		    }
		}
	      //putchar('\n');
	    }
	  putchar('\n');


	}

}



int main(void){
getInput();

performCycle(numPlayers);

 save();
 load();
//cycleWork();

}
