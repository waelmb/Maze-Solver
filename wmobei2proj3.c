/*
* File name: wmobei2proj3.c
* Project 3
* CS211, Spring 2019
* Author: Wael Abdulelah F Mobeirek (wmobei2)
* Description: A maze solver program using linked lists to store the solution path and dynamic arrays for the maze
***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //for the boolean data type

bool debugMode = false; //debugMode gobal variable

// room struct definition
typedef struct roomStruct
{
	int row, col;
	bool isVisited;
	bool isPushed;
	char status;
		
} room;

// maze struct definition 
typedef struct mazeStruct
{
 room** roomsArray;  /* allows for a maze of size 30x30 plus outer walls */
 int xsize, ysize;
 int xstart, ystart;
 int xend, yend;
} maze;

//node struct definition
typedef struct nodeStruct 
{
 room rNode;
 struct nodeStruct* pNext;
} node;

/* Initializes the stack
* @param {node*} a head of a linked list
*******************************/
node* initNode (node* hd)
{
	hd = NULL;
	return hd;
}

/* Pushes a new value to a linked list 
* @param {node**} a pointer to a head of a linked list
* @param {room} an instance of room
*******************************/
void pushNode (node** hd, room r) {
	node* ptr = (node*) malloc (sizeof(node));
 	ptr->rNode = r;
 	ptr->pNext = *hd;       // note that hd must be "de-referenced" when used 
 	*hd = ptr;             //   the unary * operator is the "de-reference" operator 
	
	if(debugMode) {
		printf("(%d, %d) has been pushed to the stack.\n", r.row, r.col);	
	} 	
}

/* Pops the element at the top of the linked list
* @param {node**} a pointer to a head of a linked list
*******************************/
void popNode (node** hd)
{
 node* ptr = *hd;

 if (ptr != NULL)
   {
    *hd = ptr->pNext;
    if(debugMode) {
    	room r = ptr->rNode;
		printf("(%d, %d) has been popped of the stack.\n", r.row, r.col);
		//printf("The linked list is now: \n"); //DEBUG
		//printSolution(*hd);	
	} 
    free (ptr);
   }
}

/* Prints a linked lists recursively to show the solution path
* @param {node*} a head of a linked list
*******************************/
void printSolution (node* hd)
{
  if(hd == NULL) {
 	return;
  }
  else {
  	printSolution(hd->pNext);
  	printf ("(%d, %d)\n", hd->rNode.row,hd->rNode.col);   // access the value at the node 
  	return;
  }
 printf ("\n");
}

/* Returns a linked lists length
* @param {node*} a head of a linked list
*******************************/
int listLength (node* hd)
{
 int length = 0;
 while (hd != NULL)
   {
    length++;
    hd = hd->pNext;
   }
 return length;
}

/* Checks if the list is empty or not
* @param {node*} a head of a linked list
*******************************/
bool isEmpty (node* hd)
{
 if (hd == NULL)
   return true;
 else
   return false;
}

/* Returns the first room the stack
* @param {node*} a head of a linked list
*******************************/
room getFirstValue (node* hd)
{
	return hd->rNode;	
}

/* Resets a stack of nodes
* @param {node*} a head of a linked list
*******************************/
void resetNode(node** hd) 
{
   while (*hd != NULL)        
   {
   	 popNode(hd);		
   }
   free(*hd);
}

int main (int argc, char **argv)
{
  maze m1;
  m1.xstart, m1.xsize, m1.xend, m1.ystart, m1.ysize, m1.yend = 0;
  node* stack;
  stack = initNode(stack);
  int xpos, ypos;
  int i,j;

  FILE *src;

  /* verify the proper number of command line arguments were given */
  if(argc < 2 || argc > 3) {
     printf("Usage: %s <input file name>\n", argv[0]);
     exit(-1);
  }
  
  /* Try to open the input file. */
  if ( ( src = fopen( argv[1], "r" )) == NULL && ( src = fopen( argv[2], "r" )) == NULL )
  {
    printf ( "Can't open input file: %s and %s", argv[1], argv[2] );
    exit(-1);
  }
  
  //Setup debugMode
   for(i = 0; i < argc; i++) {
        if(strcmp(argv[i], "-d") == 0) {
           debugMode = true;
           printf ("Debug Mode is ON\n");
        }
    }

  /* read in the size, starting and ending positions in the maze */
  //Verify that its a valid input. A vaild input is more than zero.
  do {
  	fscanf (src, "%d %d", &m1.xsize, &m1.ysize);
  	if(m1.xsize <= 0 || m1.ysize <= 0) {
  		printf("Inputs %d, %d are out of range for the size of the maze. They will be ignored.\n", m1.xsize, m1.ysize);
	  }
  }while(m1.xsize <= 0 || m1.ysize <= 0);
  
  //Verify that its a valid input. A vaild input is more than zero and equal to or less than the size
  do {
  	fscanf (src, "%d %d", &m1.xstart, &m1.ystart);
  	if(m1.xstart <= 0 || m1.ystart <= 0 || m1.xstart > m1.xsize || m1.ystart > m1.ysize) {
  		printf("Inputs %d, %d are out of range for the start of the maze. They will be ignored.\n", m1.xstart, m1.ystart);
	  }
  }while(m1.xstart <= 0 || m1.ystart <= 0 || m1.xstart > m1.xsize || m1.ystart > m1.ysize);
  
  //Verify that its a valid input. A vaild input is more than zero and equal to or less than the size
  do {
  	fscanf (src, "%d %d", &m1.xend, &m1.yend);
  	if(m1.xend <= 0 || m1.yend <= 0 || m1.xend > m1.xsize || m1.yend > m1.ysize || (m1.xend == m1.xstart && m1.yend == m1.ystart)) {
  		printf("Inputs %d, %d are out of range for the end of the maze. They will be ignored.\n", m1.xend, m1.yend);
	  }
  }while(m1.xend <= 0 || m1.yend <= 0 || m1.xend > m1.xsize || m1.yend > m1.ysize || (m1.xend == m1.xstart && m1.yend == m1.ystart));
  
  //Check if valid input ended before necessary information has been provided
  if(m1.xend == 0 || m1.xsize == 0 || m1.xstart == 0 || m1.yend == 0 || m1.ysize == 0 || m1.ystart == 0) {
  	printf("No sufficient valid input. Exiting...\n");
  	exit(-1);
  }
  
  /* print them out to verify the input */
  printf ("size: %d, %d\n", m1.xsize, m1.ysize);
  printf ("start: %d, %d\n", m1.xstart, m1.ystart);
  printf ("end: %d, %d\n", m1.xend, m1.yend);
  
  //Initialize the dynamic array inside the maze instance
  m1.roomsArray = (room**) malloc (sizeof(room*) * (m1.xsize+2) );
  for (i = 0; i < m1.xsize+2; i++) { 
  	m1.roomsArray[i] = (room*) malloc (sizeof(room) * (m1.ysize+2) );
  }
  
  /* initialize the maze to empty */
  for (i = 0; i < m1.xsize+2; i++) {
  	for (j = 0; j < m1.ysize+2; j++) {
  		m1.roomsArray[i][j].status = '.';
		m1.roomsArray[i][j].isVisited = false;
		m1.roomsArray[i][j].isPushed = false;
		m1.roomsArray[i][j].row = i;
		m1.roomsArray[i][j].col = j;
	  }
       
  }
     

  /* mark the borders of the maze with *'s */
  for (i=0; i < m1.xsize+2; i++)
    {
     m1.roomsArray[i][0].status = '*';
     m1.roomsArray[i][m1.ysize+1].status = '*';
    }
  for (i=0; i < m1.ysize+2; i++)
    {
     m1.roomsArray[0][i].status = '*';
     m1.roomsArray[m1.xsize+1][i].status = '*';
    }

  /* mark the starting and ending positions in the maze */
  m1.roomsArray[m1.xstart][m1.ystart].status = 's';
  m1.roomsArray[m1.xstart][m1.ystart].isVisited = true;
  m1.roomsArray[m1.xend][m1.yend].status = 'e';
		  
  /* mark the blocked positions in the maze with *'s */
  while (fscanf (src, "%d %d", &xpos, &ypos) != EOF)
    {
     	//Check if its a starting or ending position
		 if(m1.roomsArray[xpos][ypos].status != 's' && m1.roomsArray[xpos][ypos].status != 'e') {
     		m1.roomsArray[xpos][ypos].status = '*';	
		 }
    }

  /* print out the initial maze */
  for (i = 0; i < m1.xsize+2; i++)
    {
     for (j = 0; j < m1.ysize+2; j++)
       printf ("%c", m1.roomsArray[i][j].status);
     printf("\n");
    }
  
  /*Searching algorithm begins*/
  //set row and col to the starting position  
  int row = m1.xstart;
  int col = m1.ystart;
  
  //loop until the previously set variables equal to the ending position
  while(true)
   {  
		//printf("row and col are (%d, %d)\n", row, col);
		//Go down if not blocked or visited
  		if(m1.roomsArray[row+1][col].status != '*' && !m1.roomsArray[row+1][col].isVisited) 
		  {
  			//push roomsArray[row][col] to the stack
  			if(!m1.roomsArray[row][col].isPushed) {
  				pushNode(&stack, m1.roomsArray[row][col]);
				m1.roomsArray[row][col].isPushed = true;	
			  }
			//Icrement y by 1 to go down
			row++;
			//Make the new position visited
			m1.roomsArray[row][col].isVisited = true;
		  }
		//Go up if not blocked or visited
  		else if(m1.roomsArray[row-1][col].status != '*' && !m1.roomsArray[row-1][col].isVisited) 
		  {
  			//push roomsArray[row][col] to the stack
  			if(!m1.roomsArray[row][col].isPushed) {
  				pushNode(&stack, m1.roomsArray[row][col]);
				m1.roomsArray[row][col].isPushed = true;	
			  }
			//Decrement row by 1 to go up
			row--;
			//Make the new position visited
			m1.roomsArray[row][col].isVisited = true;
		  }
  		//Go right if not blocked or visited
  		else if(m1.roomsArray[row][col+1].status != '*' && !m1.roomsArray[row][col+1].isVisited) 
		  {
  			//push roomsArray[row][col] to the stack
  			if(!m1.roomsArray[row][col].isPushed) {
  				pushNode(&stack, m1.roomsArray[row][col]);
				m1.roomsArray[row][col].isPushed = true;	
			  }
			//Increment col by 1 to go to the right
			col++;
			//Make the new position visited
			m1.roomsArray[row][col].isVisited = true;
		  }
  		
  		//Go left if not blocked or visited
  		else if(m1.roomsArray[row][col-1].status != '*' && !m1.roomsArray[row][col-1].isVisited) 
		  {
  			//push roomsArray[row][col] to the stack
  			if(!m1.roomsArray[row][col].isPushed) {
  				pushNode(&stack, m1.roomsArray[row][col]);
				m1.roomsArray[row][col].isPushed = true;	
			  }
			//Decrement x by 1 to go to the left
			col--;
			//Make the new position visited
			m1.roomsArray[row][col].isVisited = true;
		  }
  		//Else, pop the stack and take a step back
  		else {
  			//If the stack is empty, then there is no solution
  			if(isEmpty(stack)) 
  			 {
  				printf("There is no solution to this maze.\n");
				break;	
			 }
  			//Else, make row and col be the position at the element at the top of the stack
  			else 
			{
  				//pop the stack
  				popNode(&stack);
				//Update row and col
				if(!isEmpty(stack)) {
					room updatePos = getFirstValue (stack);
					row = updatePos.row;
					col = updatePos.col;	
				}
			}
		  }
		  //break the loop when the end has been reached
		  if(row == m1.xend && col == m1.yend) {
		  	 //Print solution path
   			 printf("\nThe solution of the maze is as follows:\n");
   			 printSolution(stack);
   		     resetNode(&stack);	
			 break;	
		  }
   }
   //free the roomsArray inside of the maze instance
  	for (i = 0; i < m1.xsize+2; i++) { 
  		free(m1.roomsArray[i]);
  	}
  	free(m1.roomsArray);
  	fclose(src);
}
