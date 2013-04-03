/*
 *  CS180 Machine Exercise 1 Template
 *  by T.M. Basa
 *
 */

#include<stdio.h>
#include<stdlib.h>

#define SWAP(a,b) temp=(a); (a)=(b); (b)=temp; //when using SWAP, make sure to declare 'int temp' first

typedef struct node {
	int pathcost;
	int fcost;
	int **nboard;
	struct node *child;
	struct node *sibling;
	struct node *parent;
}node;


void display_board(int **board) {
    int i,j;
    
    for (i=0; i<3; i++) {
        for (j=0; j<3; j++) 
            if (board[i][j])
                printf("%i ",board[i][j]);
            else
                printf("  ");
                
        printf("\n");
    }
}

int read_board(int **board,char *fname) { //read initial board from file
    int i;
    FILE *fp;
    
    fp=fopen(fname,"rt");
    if (fp) {
        for (i=0; i<9; i++) 
            fscanf(fp,"%i",&board[(int)i/3][i%3]);
        
        fclose(fp);
        return 1;
    }
    display_board(board);
    return 0;
}

int **init_goal(int **goal) { //initialize goal state
    int i;
    
    for (i=0; i<2; i++) {
        goal[0][i]=i+1;
        goal[i][2]=i+3;
        goal[2][2-i]=i+5;
        goal[2-i][0]=i+7;
    }  
    goal[1][1]=0;
    
    return goal;
}

int mandist(int **a,int **b) {  //returns the manhattan distance between two 8-puzzle boards
    int i,j,dist;          //you can also use this function to determine goal state
    
    dist=0;
    for (i=0; i<9; i++)
        for (j=0; j<9; j++)
            if (a[(int)i/3][i%3]==b[(int)j/3][j%3]) {
                dist+=abs((int)i/3-(int)j/3)+abs(i%3-j%3);
                break;
            }
    
    return dist;
}

int getLeast(node *&rover, node *&least, node *&root, int **goal){
		while(rover->child != NULL) rover = rover->child;
		
		if(rover == root || rover == NULL) return 0;

		if(((rover)->fcost <= (least)->fcost && rover->child == NULL) || mandist(rover->nboard, goal) == 0) { least = rover;}
		
		
		if(rover->sibling != NULL){
			rover = (rover)->sibling;
			getLeast(rover, least, root, goal);	
		}	
		else {
			rover = (rover)->parent;
			if((rover)->sibling != NULL){
				rover = (rover)->sibling;
				getLeast(rover, least, root, goal);	
			}
			if(rover == root) return 0;
			else return 0;
		}	
		return 0;
}


void astar(int **board,int **goal) {
    int i, j, x, y, temp, count = 0;
    int numOfMoves = 0;
    node *currentNode = NULL, *parentNode = NULL, *rover = NULL, *least = NULL, *holder = NULL;
    node *root;
    node *stack[31];
    node *visited[500];
    
	for(i=0; i<31; i++) stack[i] = NULL;
 	for(i=0; i<500; i++) visited[i] = NULL;
 	
 	//initialize root node   
	root = (node*)malloc(sizeof(node));
	root->nboard = (int**)malloc(sizeof(int*)*3); 
			
 	//copy contents of board to current board
	for(i=0; i<3; i++){
		root->nboard[i]=(int*)malloc(sizeof(int)*3);
		for(j=0; j<3; j++){
				root->nboard[i][j] = board[i][j];	
		}	
	}
	
	root->pathcost = 0;
	root->fcost = root->pathcost + mandist(board,goal);
	root->child = NULL;
	root->parent = NULL;
	root->sibling = NULL;
	
	parentNode = root;
	least = (node*)malloc(sizeof(node));
	holder = least;
	visited[count] = root;
	
	while(mandist(parentNode->nboard,goal) != 0){
		least = holder;
		least->fcost = 2147483647;
		
		//find 0
		for(i=0; i<=2; i++){
			for(j=0; j<=2; j++){
				if(parentNode->nboard[i][j] == 0) {y=i; x=j;};
			}
		}
	
	//	printf("PARENT:\n");
	//	display_board(parentNode->nboard);
		
		//if 0  is not on the first row
		if(y!=0){ 
			//printf("MOVE DOWN\n");
			
			currentNode = (node*)malloc(sizeof(node));
			currentNode->nboard = (int**)malloc(sizeof(int*)*3); 
			
 			//copy contents of board to current board
			for(i=0; i<3; i++){
			currentNode->nboard[i]=(int*)malloc(sizeof(int)*3);
				for(j=0; j<3; j++){
						currentNode->nboard[i][j] = parentNode->nboard[i][j];	
				}	
			}
			
			//printf("\n");
			SWAP(currentNode->nboard[y][x], currentNode->nboard[y-1][x]);
			//display_board(currentNode->nboard);
			
			//put field contents in new node
			currentNode->pathcost = (parentNode->pathcost) + 1;
			numOfMoves = currentNode->pathcost;
			currentNode->fcost = currentNode->pathcost + mandist(currentNode->nboard, goal);
			currentNode->parent = parentNode;
			currentNode->child = NULL;
			currentNode->sibling = NULL;
			
			if(mandist(currentNode->nboard,goal) == 0){
				parentNode = currentNode; break;
			}
			
			parentNode->child = currentNode; 
		}
		
		//if 0 is not on the last row
		if(y!=2){ 
			//printf("MOVE UP\n");
			
			currentNode = (node*)malloc(sizeof(node));
			currentNode->nboard = (int**)malloc(sizeof(int*)*3); 
 			//copy contents of board to current board
			for(i=0; i<3; i++){
			currentNode->nboard[i]=(int*)malloc(sizeof(int)*3);
				for(j=0; j<3; j++){
						currentNode->nboard[i][j] = parentNode->nboard[i][j];	
				}	
			}
			//printf("\n");
			SWAP(currentNode->nboard[y][x], currentNode->nboard[y+1][x]);
			//display_board(currentNode->nboard);
			
			currentNode->parent = parentNode;
			currentNode->child = NULL;
			currentNode->sibling = NULL;
		
			//put field contents in new node
			currentNode->pathcost = (parentNode->pathcost) + 1;
			numOfMoves = currentNode->pathcost;
			currentNode->fcost = currentNode->pathcost + mandist(currentNode->nboard, goal);
			
			if(mandist(currentNode->nboard,goal) == 0){
				parentNode = currentNode; break;
			}
			
			//pointing of child
			if(parentNode->child == NULL){
				parentNode->child = currentNode;
			}
			else parentNode->child->sibling = currentNode;
			
		}
				//if 0  is not on the first column
		if(x!=0){ 
			//printf("MOVE RIGHT\n");
			
			currentNode = (node*)malloc(sizeof(node));
			currentNode->nboard = (int**)malloc(sizeof(int*)*3); 
						
 			//copy contents of board to current board
			for(i=0; i<3; i++){
			currentNode->nboard[i]=(int*)malloc(sizeof(int)*3);
				for(j=0; j<3; j++){
						currentNode->nboard[i][j] = parentNode->nboard[i][j];	
				}	
			}
			
			//printf("\n");
			SWAP(currentNode->nboard[y][x], currentNode->nboard[y][x-1]);
			//display_board(currentNode->nboard);
			
			//put field contents in new node
			currentNode->pathcost = (parentNode->pathcost) + 1;
			numOfMoves = currentNode->pathcost;
			currentNode->fcost = currentNode->pathcost + mandist(currentNode->nboard, goal);
			currentNode->parent = parentNode;
			currentNode->child = NULL;
			currentNode->sibling = NULL;

			if(mandist(currentNode->nboard,goal) == 0){
				parentNode = currentNode; break;
			}

			//pointing of child
			if(parentNode->child == NULL){
				parentNode->child = currentNode; 
			}
			else if(parentNode->child->sibling == NULL){
				parentNode->child->sibling = currentNode;
			}
			else 
				parentNode->child->sibling->sibling = currentNode;
			
		
		}
		
	
		//if 0  is not on the last column
			if(x!=2){ 
			//printf("MOVE LEFT\n");
			
			currentNode = (node*)malloc(sizeof(node));
			currentNode->nboard = (int**)malloc(sizeof(int*)*3); 
			
 			//copy contents of board to current board
			for(i=0; i<3; i++){
			currentNode->nboard[i]=(int*)malloc(sizeof(int)*3);
				for(j=0; j<3; j++){
						currentNode->nboard[i][j] = parentNode->nboard[i][j];	
				}	
			}
			
		//	printf("\n");
			SWAP(currentNode->nboard[y][x], currentNode->nboard[y][x+1]);
			//display_board(currentNode->nboard);
			
			
			
			//put field contents in new node
			currentNode->pathcost = (parentNode->pathcost) + 1;
			numOfMoves = currentNode->pathcost;
			currentNode->fcost = currentNode->pathcost + mandist(currentNode->nboard, goal);
			currentNode->parent = parentNode;
			currentNode->child = NULL;
			currentNode->sibling = NULL;
			
			if(mandist(currentNode->nboard,goal) == 0){
				parentNode = currentNode; break;
			}
			
			//pointing of child
			if(parentNode->child == NULL){
				parentNode->child = currentNode;
			}
			else if(parentNode->child->sibling == NULL){
				parentNode->child->sibling = currentNode;	
			}
			else if(parentNode->child->sibling->sibling == NULL){
				parentNode->child->sibling->sibling = currentNode;
			}
			else parentNode->child->sibling->sibling->sibling = currentNode;
		}
		

		
		//traverse tree
		rover = root;
		
		//while(j == 1){
			j=0;
		
   		getLeast(rover, least, root, goal);
			parentNode = least;
			//printf("\n %d %d \n", parentNode->fcost, parentNode->pathcost);
			
		//}
		//break;
		//display_board(parentNode->nboard);
		
	}
	rover = parentNode;
	i=30;
	while(rover != root){
		stack[i] = rover; i--;
		rover = rover->parent;
	}
	stack[i] = root;
	
	for(i=0; i<31; i++){
		if(stack[i] != NULL){
			printf("\n");
			display_board(stack[i]->nboard);	}
		
	}
	
		
   printf("\nTOTAL NUMBER OF MOVES: %d", numOfMoves); 
}

int main(int argc,char *argv[]) {
    int i;
    int **board,**goal;
    
    if (argc>1) {
        
        board=(int**)malloc(sizeof(int*)*3);
        goal=(int**)malloc(sizeof(int*)*3);
        for (i=0; i<3; i++) {
            board[i]=(int*)malloc(sizeof(int)*3);
            goal[i]=(int*)malloc(sizeof(int)*3);
        }
        
        if (read_board(board,argv[1])) {
            goal=init_goal(goal);       
            astar(board,goal);
        
        }
        else
            printf("\n\nERROR: Can't open file '%s'.\n\n",argv[1]);
            
        //free allocated variables
        for (i=0; i<3; i++) {
            free(board[i]);
            free(goal[i]);
        }
        free(board);    
        free(goal);
    }
    else 
        printf("\nUsage: %s <inputfile>\n\n",argv[0]);

    return 0;
}

