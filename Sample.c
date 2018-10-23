/*
 ============================================================================
 Name        : Conway.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int xSize = 0;
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
	printf("Enter Width: ");
	scanf("%s", widthBuff);
	xSize = atoi(widthBuff);
	printf("\nEnter Height: ");
	scanf("%s", heightBuff);
	ySize = atoi(heightBuff);
	printf("\nEnter # of Cycles ");
	scanf("%s", cycleBuff);
	numCycles = atoi(cycleBuff);
	printf("\nWidth: %d Height: %d # of Cycles: %d", xSize, ySize, numCycles);

	char field[xSize][ySize];

	for(int a = 0; a < ySize; a++)
	{

	for(int b = 0; b < xSize; b++)
	{

	field[a][b] = '-';
	}

	}


	int exit = 0;

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

	
	field[yStart][xStart] = '+';
	}



for(int i = 0; i < numCycles; i++)
{
	//do checking of cells in each cycle
	//currently unimplemented need to get rid of -1 on for loop replace with checks on ifs for exceding boundary
	for(int a = 0; a < ySize - 1; a++)
	{

	for(int b = 0; b < xSize - 1; b++)
	{

	if(field[a][b] == '+')
	{
	int liveNeigh = 0;

	if(field[a+1][b] == '+')
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

	if(field[a-1][b+1] == '+')
	{
	liveNeigh = liveNeigh + 1;
	}

	if(field[a+1][b-1] == '+')
	{
	liveNeigh = liveNeigh + 1;
	}

	if(!(liveNeigh == 2 || liveNeigh == 3))
	{
	field[a][b] == '-';
	}


	}

	if(field[a][b] == '-')
	{
	int liveNeigh = 0;

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
	field[a][b] = '+';
	}
	}
	
	}

	}

	//print cycles
	printf("\nCycle %d\n", i);
	for(int a = 0; a < ySize; a++)
	{

	for(int b = 0; b < xSize; b++)
	{
	printf("|%c|",field[a][b]);
	}
	printf("\n");
	}
	printf("\nxStart:%d yStart:%d \n",xStart,yStart);
}
	return EXIT_SUCCESS;
}

