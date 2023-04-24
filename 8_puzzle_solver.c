//******************************************************************************************************************//
//			This program asks the user to input unique numbers from 0-8 in a 3x3 grid, that will serve				//
//				as the input for the 8- puzzle. The program will then utilize IDS and A* search to					//
//				find the solution, solution path, solution cost, number of nodes expanded, and the running time.	//
//																													//
//			MEMBERS:		BALANA, Renmar																			//
//							BORRERO, Jan Lance																		//
//							SABAS, Jessa Lorenza																	//
//																													//
//******************************************************************************************************************//



#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXSIZE 40000
#define SIZE 125

//IDS only needs a maximum of 125 nodes. 
//MAXSIZE is chosen to be 40000, to be able to finish an A* search

int easy[9] 	= {1, 3, 4, 8, 6, 2, 7, 0, 5};
int medium[9] 	= {2, 8, 1, 0, 4, 3, 7, 6, 5};
int hard[9] 	= {2, 8, 1, 4, 6, 3, 7, 5, 0};
int worst[9] 	= {5, 6, 7, 4, 0, 8, 3, 2, 1};
int goal[9] 	= {1, 2, 3, 8, 0, 4, 7, 6, 5};				//stores value of goal state as an array			GOAL


typedef struct treenode{
	unsigned long long int node_id;		//64 bits allocated to keep track of id of nodes
	int *config;						//arrangement of numbers as array of int
	
	struct treenode *parent;
	struct treenode *up;
	struct treenode *down;
	struct treenode *left;
	struct treenode *right;
	
	int hn;							//our h(n) or manhattan distance
	int gn;			 				//cost so far to reach node				same as DEPTH. because all moves cost 1.
	int fvalue;						//f value
	char prevAction;				//action done in previous node to get to this node 
	
}node;							//alias for struct treenode
typedef node* nodePtr;			//alias for node pointer

//nodePtr tree[MAXSIZE] = {NULL};
//define a pointer to (a pointer to a node).
//this will be like an array of (pointers to a node)


typedef struct st {
  int items[MAXSIZE];				//stores index of tree[] that is OPEN
  int top;
  char mode;
  
}stack;

stack *OPEN;
stack *CLOSED;			//NOT REALLY USED


//OPTIMIZATION FOR A*: use closed list to avoid repeated states

//linked list for closed list
typedef struct closed_l {
	int board_config[9];
	int node_id;	//not sure if needed, but ok
	struct closed_l *nxtPtr;
}list;

list* CLOSED_LIST;
int UNIQUE_STATES_EXPANDED;		//remembers how many unique config have been expanded



unsigned long long int OPEN_count;						//
unsigned long long int CLOSED_count;					//tracks number of closed nodes (visited, and/or expanded)

unsigned long long int glob_count;						//keeps track of number nodes generated
unsigned long long int expanded_nodes_count;

double time_spent;


nodePtr defaultPtr;						//this will serve as the default pointer to newly created nodes.
										//helps distinguish useless nodes.	





//function prototypes

int main_menu();						//these prints various menu screens
int easy_menu();
int medium_menu();
int hard_menu();
int worst_menu();
int user_initial_menu();
int user_goal_menu();
int debug_menu();


int IDS(int *initial);
int IDS_debug(int *initial);
int DLS(nodePtr tree[], int depth);
int DLS_debug(nodePtr tree[], int depth);

int Astar(int *intial);

nodePtr allocate_node();
int* arrayAllocate(int* array);
void blankMove(int config[], char move);
int blankPosition(int config[]);
void createEmptyStack(stack *s, char mod);
void deallocate_mem(nodePtr tree[]);

void expandNode(nodePtr tree[], int node_index, int mode);
int find_vacant_ptr(nodePtr tree[]);
int goalTest(nodePtr nPtr);

void insert(nodePtr tree[], int parent_index, int *array, char action_taken);
void sort_stack(nodePtr tree[], stack *OPEN);
int isempty(stack *s); 
int isfull(stack *s);
int isSolvable(int *arrayPtr);
int  mnhtan_dist(int curr[], int goal[]);
int pop(stack *s);
void prepare_node(nodePtr n);
int  prepare_tree(nodePtr n[]);
void printStack(stack *s);
void prune_node(nodePtr n);
void push(stack *s, int newitem);
void nodeCheck(nodePtr n);
void print_tree(nodePtr tree[]);
void printGame(int n[], int accept);
void printGrid(int n[]);
void printGrid_(int n[], int curr);
void print_solution_path(nodePtr tree[], int goal_index);
void print_hborder();
void print_vborder();
void free_tree(nodePtr tree[], int max);
void print_tree(nodePtr tree[]);
void trim_tree(nodePtr tree[]);

void title_screen();
void closing_screen();
void clear_screen();

int inputGrid(int n[]);
int inputGoal(int n[]);
void updateGrid(int n[], int pos, int value);
void enter_checkpoint();
void delay(int seconds);



void reset_Goal();

int static xcount;
int static debugcount;


void enqueue(nodePtr tree[], int index, list* head);
int compare_board(int a[], int b[]);
void freeList(list* head);
int is_state_unique(int config[], list* head);


void print_queue(list* head);

int IDS(int *initial){

	nodePtr tree[SIZE] = {NULL};
	
	time_spent = 0.0;
	clock_t begin = clock();
	
	OPEN_count = 0;
	CLOSED_count = 0;
	glob_count = 0;
	
	int depth_limit = 0;
	int result, i, index;
	
			//tree is empty
			
	OPEN 	= (stack *)malloc(sizeof(stack));
	CLOSED 	= (stack *)malloc(sizeof(stack));
	createEmptyStack(OPEN, 'o');
	createEmptyStack(CLOSED, 'c');		
	
	
	do{
		insert(tree, -1, initial, '_');
		expanded_nodes_count = 0;
		
		
		
		result = DLS(tree, depth_limit);
		
		if(result<0){
			//then reset the tree and everything else
			depth_limit++;
			free_tree(tree, SIZE);
			free(OPEN);
			free(CLOSED);
			
			OPEN = NULL;
			CLOSED = NULL;
			glob_count 	= 0;
			OPEN_count 	= 0;
			CLOSED_count = 0;
			expanded_nodes_count = 0;
			
			
			OPEN 	= (stack *)malloc(sizeof(stack));
			CLOSED 	= (stack *)malloc(sizeof(stack));
				if (OPEN == NULL){
					printf("DEBUG - ERROR. Not enough memory to allocate the OPEN stack\n");
				}
				if (CLOSED == NULL){
					printf("DEBUG - ERROR. Not enough memory to allocate the CLOSED stack\n");
				}
			createEmptyStack(OPEN, 'o');
			createEmptyStack(CLOSED, 'c');	
		}
		
	}
	while (result < 0);
	
	clock_t end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	print_solution_path(tree, result);
	
			free_tree(tree, SIZE);
			
			free(OPEN);
			free(CLOSED);
			
			OPEN = NULL;
			CLOSED = NULL;
			glob_count 	= 0;
			OPEN_count 	= 0;
			CLOSED_count= 0;
			expanded_nodes_count = 0;

	//returns index of goal node
	return result;
}

//returns index of goal node
int DLS(nodePtr tree[], int depth){
	
	int resultFound = 0;		//flag, gets 1 once goal node is found
	int i=0;
	int goalNode;
	nodePtr currPtr;
	int leaf_reached;
	int index;
		
	do{
		index = OPEN->items[OPEN->top];
		currPtr = tree[index];						//currPtr gets address of top of OPEN List
	
		if(currPtr->gn <= depth){							//currPtr depth is less than depthLimit
			if(goalTest(currPtr)){							//if node is goal
				//return SOLUTION
				resultFound = 1;
				goalNode = index;
			}
			else if (currPtr->gn == depth){
				pop(OPEN);
				CLOSED_count++;
				leaf_reached = 1;			//marks that an "edge" has been reached. (node with same depth as depthLimit)
				
				if(currPtr->gn > 0){		//that node is "cutoff"
					currPtr->up = NULL;
					currPtr->down = NULL;
					currPtr->left = NULL;
					currPtr->right = NULL;
					}
			}
			else{							//node is not goal, so expand:
				expandNode(tree, index, 1);		//once expanded, node gets moved to CLOSED
				expanded_nodes_count++;
			}
			
			if(leaf_reached == 1){
				trim_tree(tree);
			} else;
		}
		else{										//currPtr depth shouldn't be larger than depthLimit. not expanded yet
			printf("DEBUG ----- ERROR!!! you shouldn't have reached this!!!\n");
		}
	}
	while(resultFound != 1 && (isempty(OPEN) == 0)); 
	
	if(resultFound == 1){
	//	printf("GOAL WAS FOUND\n");
	//	nodeCheck(tree[goalNode]);
		return goalNode;}
	else{
		//printf("GOAL WAS NOT found\n");
		return -1;
	}		//goal not yet found	
}

/*
//simple memory bounded approach to trim nodes that are obviously suboptimal
//because their heuristic got worse than starting point
void Astar_trim(nodePtr tree[], int cutoff_threshold){
	int i, j;
	int temp;
	
	for(i=0; i<glob_count; i++){
		if(tree[i] != NULL){
			if(tree[i]->hn > cutoff_threshold){
				free(tree[i]->config);
				free(tree[i]);
				tree[i] = NULL;
				printf("\tDEBUG trimmed node: %d\n", i);
					//search OPEN stack of that node, remove it from open
					for(j=0; j<(OPEN->top)+1; j++){
						if( OPEN->items[j] == i ){			//first, put it on top of stack, then pop
							OPEN->items[j] = OPEN->items[OPEN->top];
							OPEN->items[OPEN->top] = i;
							printf("\tDEBUG popped from trim: %d\n", i);
							pop(OPEN);
							break;
						}
					}
			}
		}
	}
	return;
}*/


int Astar(int *initial){
	
	nodePtr tree[MAXSIZE] = {NULL};
	
	time_spent = 0.0;
	clock_t begin = clock();
	
	OPEN_count = 0;
	CLOSED_count = 0;
	glob_count = 0;
	
	int result, i, index, n;

	OPEN 	= (stack *)malloc(sizeof(stack));
	CLOSED 	= (stack *)malloc(sizeof(stack));
	createEmptyStack(OPEN, 'o');
	createEmptyStack(CLOSED, 'c');		
		//start with tree is empty
		
	CLOSED_LIST = (list *)malloc(sizeof(list));	
	UNIQUE_STATES_EXPANDED = 0;
	
	
	insert(tree, -1, initial, '_');									//STEP 1
//	int cutoff_threshold = tree[0]->hn;
	
	do{
		if(isempty(OPEN)){
			printf("ERROR! OPEN is empty. No nodes inserted.\n");	//STEP 2
			break;
		}
		
	//	Astar_trim(tree, cutoff_threshold);						//additional step for better space complexity. DISREGARDED
		
		sort_stack(tree, OPEN);											//STEP 3
		n = pop(OPEN);
		
		//push(CLOSED, n);

//		print_tree(tree);
//		enter_checkpoint();
	
		if(goalTest(tree[n])){										//STEP 4
		
			clock_t end = clock();
			time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			
			print_solution_path(tree, n);
//			print_tree(tree);
			
			free_tree(tree, MAXSIZE);
				free(OPEN);		OPEN = NULL;
				free(CLOSED);	CLOSED = NULL;
				glob_count 	= 0;
				OPEN_count 	= 0;
				CLOSED_count= 0;
				expanded_nodes_count = 0;
				
				UNIQUE_STATES_EXPANDED = 0;
				freeList(CLOSED_LIST);
				
			break;
		}
		else{														//STEP 5
			expandNode(tree, n, 2);
			expanded_nodes_count++;									//once expanded, node gets moved to CLOSED
		}
	}while(1);
	return n;
}

int Astar_debug(int *initial){
	
	nodePtr tree[MAXSIZE] = {NULL};
	time_spent = 0.0;
	clock_t begin = clock();
	
	OPEN_count = 0;
	CLOSED_count = 0;
	glob_count = 0;
	
	int result, i, index, n;

	OPEN 	= (stack *)malloc(sizeof(stack));
	CLOSED 	= (stack *)malloc(sizeof(stack));
	createEmptyStack(OPEN, 'o');
	createEmptyStack(CLOSED, 'c');		
		//start with tree is empty
	CLOSED_LIST = (list *)malloc(sizeof(list));	
	UNIQUE_STATES_EXPANDED = 0;
	insert(tree, -1, initial, '_');									//STEP 1
	//int cutoff_threshold = tree[0]->hn;
	
	do{	
		if(isempty(OPEN)){
			printf("ERROR! OPEN is empty. No nodes inserted.\n");	//STEP 2
		}
		
		sort_stack(tree, OPEN);											//STEP 3
		n = pop(OPEN);
		enqueue(tree, n, CLOSED_LIST);
		//push(CLOSED, n);

//		print_tree(tree);
//		enter_checkpoint();
	
		if(goalTest(tree[n])){										//STEP 4
			clock_t end = clock();
			time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			
			print_solution_path(tree, n);
			
			free_tree(tree, MAXSIZE);
				
				free(OPEN);		OPEN = NULL;
//				free(CLOSED);	CLOSED = NULL;
				glob_count 	= 0;
				OPEN_count 	= 0;
				CLOSED_count= 0;
				expanded_nodes_count = 0;
				UNIQUE_STATES_EXPANDED = 0;
			
				freeList(CLOSED_LIST);
			break;
		}
		else{														//STEP 5
			expandNode(tree, n, 2);
			expanded_nodes_count++;
			printf("\texpanded node id: [%5llu] \thn():[%2d] \tgn():[%2d]\t fvalue:[%2d] \t total nodes generated: [%d] \n", tree[n]->node_id, tree[n]->hn, tree[n]->gn, tree[n]->fvalue, glob_count); //debugger
		}
	}while(1);
	return n;
}


int main (void){
	int game_mode;
	int algo_choice;
	int input_accept=0;
	
	defaultPtr = (nodePtr)malloc(sizeof(node));			//default ptr for newly inserted nodes
	

	printf("\t---- DEBUG MODE ----\n");
	
	do{
		title_screen();
		game_mode = main_menu();
		
		switch(game_mode){
			case 0:
				closing_screen();
				break;
			case 1:							//EASY MODE
				do{
					title_screen();
					algo_choice = easy_menu();
					
					if (algo_choice > -1){
						input_accept = 1;
					}
					else{
						printf("\t\t\tERROR! Your input was not among the valid choices. Please Try again\n");
						input_accept = 0;
						delay(1);
					}
				}while (input_accept == 0);
				break;
			case 2:							//MEDIUM MODE
				do{
					title_screen();
					algo_choice = medium_menu();
					
					if (algo_choice > -1){
						input_accept = 1;
					}
					else{
						printf("\t\t\tERROR! Your input was not among the valid choices. Please Try again\n");
						input_accept = 0;
						delay(2);
					}
				}while (input_accept == 0);
				break;
			case 3:							//HARD MODE
				do{
					title_screen();
					algo_choice = hard_menu();
					
					if (algo_choice > -1){
						input_accept = 1;
					}
					else{
						printf("\t\t\tERROR! Your input was not among the valid choices. Please Try again\n");
						input_accept = 0;
						delay(2);
					}
				}while (input_accept == 0);
				break;
			case 4:							//WORST MODE
				do{
					title_screen();
					algo_choice = worst_menu();
					
					if (algo_choice > -1){
						input_accept = 1;
					}
					else{
						printf("\t\t\tERROR! Your input was not among the valid choices. Please Try again\n");
						input_accept = 0;
						delay(2);
					}
					
				}while (input_accept == 0);
				break;
				
			case 5:									//user INPUT
				title_screen();
				algo_choice = user_initial_menu();
				break;	
			case 6:									//user INTIAL & GOAL INPUT
				title_screen();
				algo_choice = user_goal_menu();
				break;
			case 7:									//DEBUG mode
				title_screen();
				algo_choice = debug_menu();
				break;
			default:
				printf("\t\t\tThat is not an integer between 0-6, silly!!!\n\t\t\tPlease Try Again.\n");
				delay(2);
				break;
		
		}//end switch
		
	}while(game_mode != 0);
	

	return 0;
}



int IDS_debug(int *initial){
	nodePtr tree[SIZE] = {NULL};
	
	time_spent = 0.0;
	clock_t begin = clock();
	
	OPEN_count = 0;
	CLOSED_count = 0;
	glob_count = 0;
	
	int depth_limit = 0;
	int result, i, index;
	
			//tree is empty
			
	OPEN 	= (stack *)malloc(sizeof(stack));
	CLOSED 	= (stack *)malloc(sizeof(stack));
	createEmptyStack(OPEN, 'o');
	createEmptyStack(CLOSED, 'c');		
	
	insert(tree, -1, initial, '_');
	
	do{
		expanded_nodes_count = 0;
		
		result = DLS_debug(tree, depth_limit);

		if(result<0){
			//then reset the tree
			depth_limit++;
			
			free_tree(tree, SIZE);
			free(OPEN);
			free(CLOSED);
			
			OPEN = NULL;
			CLOSED = NULL;
			glob_count 	= 0;
			OPEN_count 	= 0;
			CLOSED_count = 0;
			expanded_nodes_count = 0;
			
			
			OPEN 	= (stack *)malloc(sizeof(stack));
			CLOSED 	= (stack *)malloc(sizeof(stack));
				if (OPEN == NULL){
					printf("DEBUG - ERROR. Not enough memory to allocate the OPEN stack\n");
				}
				if (CLOSED == NULL){
					printf("DEBUG - ERROR. Not enough memory to allocate the CLOSED stack\n");
				}
			createEmptyStack(OPEN, 'o');
			createEmptyStack(CLOSED, 'c');		
			
			insert(tree, -1, initial, '_');
		}
		
	}
	while (result < 0);
	
	clock_t end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	print_solution_path(tree, result);
	
//remove the comment, if you wish to see the remaining tree, after finding goal
//print_tree(tree);

			free_tree(tree, SIZE);
			free(OPEN);
			free(CLOSED);
			
			OPEN = NULL;
			CLOSED = NULL;
			glob_count 	= 0;
			OPEN_count 	= 0;
			CLOSED_count= 0;
			expanded_nodes_count = 0;

	//returns index of goal node
	return result;
}

//returns index of goal node
int DLS_debug(nodePtr tree[], int depth){
	
clock_t dls_start_t = clock();	
double dls_total_t;
	
	int resultFound = 0;		//flag, gets 1 once goal node is found
	int i=0;
	int goalNode;
	nodePtr currPtr;
	int leaf_reached;
	int index;
	do{
		index = OPEN->items[OPEN->top];
		currPtr = tree[index];						//currPtr gets address of top of OPEN List

		if(currPtr->gn <= depth){							//currPtr depth is less than depthLimit
			if(goalTest(currPtr)){							//if node is goal
				//return SOLUTION
				resultFound = 1;
				goalNode = index;
			}
			else if (currPtr->gn == depth){
				pop(OPEN);
				CLOSED_count++;
				leaf_reached =1;			//marks that an "edge" has been reached. (node with same depth as depthLimit)
				
				if(currPtr->gn > 0){		//that node is "cutoff"
					currPtr->up = NULL;
					currPtr->down = NULL;	
					currPtr->left = NULL;
					currPtr->right = NULL;
					}
		/*	//debugging. checking trimmed leaves		
			printf("leaf TRIMMED. depth[%d] current:",depth);
				nodeCheck(currPtr);
			printf("parent:");
				nodeCheck(currPtr->parent);
		*/		
			}
			else{							//node is not goal, so expand:
				expandNode(tree, index, 1);		//once expanded, node gets moved to CLOSED
				expanded_nodes_count++;
			}
			
			if(leaf_reached == 1){
				trim_tree(tree);
			} else;
		}
		else{										//currPtr depth shouldn't be larger than depthLimit. not expanded yet
			printf("DEBUG ----- ERROR!!! you shouldn't have reached this!!!\n");
		}
	}
	while(resultFound != 1 && (isempty(OPEN) == 0)); 

clock_t dls_end_t = clock();
dls_total_t = (double)(dls_end_t - dls_start_t) / CLOCKS_PER_SEC;

printf("\tDEBUG --end of DLS-- depth: [%2d]\t", depth);
printf("time taken: %f sec\t",dls_total_t);
printf("nodes generated: %9llu\t", glob_count);

	if(resultFound == 1){
		printf("GOAL WAS FOUND\n");
		return goalNode;}
	else{
		printf("GOAL WAS NOT found\n");
		return -1;}				//goal not yet found
		
}

//finds any free element in array of pointers, and gives it a fresh new memory
void insert(nodePtr tree[], int parent_index, int *array, char action_taken){
	//where, tree is array of pointers,
	//array is the board config
	//action_taken is the move
	
	
	int curr_index = prepare_tree(tree);			//get vacant index
	int i;
	nodePtr nPtr = tree[curr_index];
	nPtr->config = array;							//config of node gets value of argument entered.
	prepare_node(nPtr);								//intitalize node values
	nPtr->prevAction = action_taken;
	
	nPtr->hn = mnhtan_dist(nPtr->config, goal);
	
	if (parent_index < 0 ){				//if inserting root node,
		nPtr->gn = 0;					//gn is depth, aka cost to get here
	}
	else{
		nodePtr parent = tree[parent_index];		//first, find the parent address
		nPtr->parent = parent;
		nPtr->gn = (parent->gn)+1;
		
		//connect the new node to its parent:
		switch(action_taken){
				case 'u':		//for up
					parent->up = nPtr; break;
				case 'd':		//for down
					parent->down = nPtr; break;
				case 'l':		//for left
					parent->left = nPtr; break;
				case 'r':		//for right
					parent->right = nPtr; break;
				default:		//for root node, which does not have an action. only for DEBUGGING.
					printf("\nERROR! how the heck did we even get here. shouldn't be possible!\n\n");
					break;
			}//end switch
	}
	
	nPtr->fvalue = nPtr->hn + nPtr->gn;	
	glob_count++;						//tick 1 on glob_count, that counts total number of nodes
	
	return;
}


//returns the first index of the array of ptrs, that points to NULL
int find_vacant_ptr(nodePtr tree[]){
	int i;
	int vacant = -1;
	
	for(i=0; i<MAXSIZE; i++){
		if (tree[i] == NULL){
			vacant = i;
			break;
		}
	}
	if(vacant == -1){
		printf("ERROR! the array of Pointers was full. No more vacant ptr, so no new nodes can be created");
	}
	return vacant;
}

//allocatest the member config of a node
int* arrayAllocate(int* array){
	int *result = malloc(9 * sizeof(int));
	if(result == NULL){
		printf("ERROR. Not enough memory to allocate a new Board.\n");
	}
	return result;
}

//function allocates memory on the heap, to store a node
nodePtr allocate_node(){
	nodePtr n = (nodePtr)malloc(sizeof (node));
	if ( n == NULL){
		printf("Critical ERROR!!! Not enough memory to allocate a new node\n");
	}
	return n;
}

//function returns pointer to chosen vacant node
int prepare_tree(nodePtr n[]){
	int vacant, i;
	vacant = find_vacant_ptr(n);		//first, find index of element that still points to NULL (therefore vacant)
	n[vacant] = allocate_node();		//allocate memory to that element
	
	push(OPEN, vacant);
	
	return vacant;
}

//this function intializes the node members
void prepare_node(nodePtr n){
	n->node_id 	= glob_count;
	
	//new nodes are redirected by default to defaultPtr
	//NULL values are only for childless nodes (can be deallocated safely)
	
	n->parent 	= defaultPtr;
	//need switch
	int pos = blankPosition(n->config);
	switch(pos){
		case 0://only legal moves: right, down
			n->up		= NULL;
			n->down		= defaultPtr;
			n->left		= NULL;
			n->right	= defaultPtr;
			break;
		case 1://down, left, right
			n->up		= NULL;
			n->down		= defaultPtr;
			n->left		= NULL;
			n->right	= defaultPtr;
			break;
		case 2://down, left
			n->up		= NULL;
			n->down		= defaultPtr;
			n->left		= defaultPtr;
			n->right	= NULL;
			break;
		case 3://up, down, right
			n->up		= defaultPtr;
			n->down		= defaultPtr;
			n->left		= NULL;
			n->right	= defaultPtr;
			break;
		case 4://up, down, left, right
			n->up		= defaultPtr;
			n->down		= defaultPtr;
			n->left		= defaultPtr;
			n->right	= defaultPtr;
			break;
		case 5://up, down, left
			n->up		= defaultPtr;
			n->down		= defaultPtr;
			n->left		= defaultPtr;
			n->right	= NULL;
			break;
		case 6://up, right
			n->up		= defaultPtr;
			n->down		= NULL;
			n->left		= NULL;
			n->right	= defaultPtr;
			break;
		case 7://up, left, right
			n->up		= defaultPtr;
			n->down		= NULL;
			n->left		= defaultPtr;
			n->right	= defaultPtr;
			break;
		case 8://up, left
			n->up		= defaultPtr;
			n->down		= NULL;
			n->left		= defaultPtr;
			n->right	= NULL;
			break;
		default:
			printf("debugger: SOMEHOW SOMETHING WENT WRONG IN SWITCH CASE IN prepare_node\n");
			break;
	}
	n->hn		= 0;
	n->gn		= 0;
	n->fvalue	= 0;
	n->prevAction = '_';
	return;
}

//free and set to null, the array element that holds address of that node. So memory can be used again.
void prune_node(nodePtr n){
		free(n);
		n = NULL;
	return;
}

//searches tree for any useless nodes, and ("trim") frees it to reuse memory
void trim_tree(nodePtr tree[]){
	int i;
	int index;
	for(i=0; i<SIZE; i++){		//loop through array of ptrs
		if(tree[i] != NULL){			//if current element has a node,	
			//and if that node's successor ptrs are all NULL
			if((tree[i]->up == NULL) && (tree[i]->down == NULL) && (tree[i]->left == NULL) && (tree[i]->right == NULL)){
				if ((tree[i]->gn) > 0){
					//remove the trace of this useless node from the parent.
			//		nodePtr currPtr = tree[i];
					char move = (tree[i])->prevAction; 	
					switch (move){
						case 'u':
							(tree[i]->parent)->up = NULL;
							break;
						case 'd':
							(tree[i]->parent)->down = NULL;
							break;
						case 'l':
							(tree[i]->parent)->left = NULL;
							break;
						case 'r':
							(tree[i]->parent)->right = NULL;
							break;
					}
					
					free(tree[i]->config);
					free(tree[i]);
					tree[i] = NULL;
				}
			}
		}
		else{
		}
	}
	return;
}

//deallocates all nodes, and all nodes' config
void free_tree(nodePtr tree[], int max){
	int x;
	for(x=0; x<max; x++){
		if(tree[x] != NULL){
		//	prune_node(tree[x]);
			free((tree[x]));
			tree[x] = NULL;
		}
	}
	return;
}

//deallocates any non-goal leaf (node at edge of tree)
void deallocate_mem(nodePtr tree[]){
	int x;
	//loop to find which node can be deallocated
	for(x=0; x<MAXSIZE; x++){
		//if node has no child
		if((tree[x]->up == NULL) && (tree[x]->down == NULL) && (tree[x]->left == NULL) && (tree[x]->right == NULL)){
			//and node is not goal yet,
			if (tree[x]->hn != 0){
				//printf("DEBUG - node pruned: %llu\n", tree[x]->node_id);
				prune_node(tree[x]);
			}
		}
	}
}

//DEBUGGING Function Prints elements of stack
void printStack(stack *s) {

  int i;
  
  if(s->mode == 'o'){
  	  printf("OPEN Stack: \t");
	  for (i = 0; i < OPEN_count; i++) {
	    printf("%3d ", s->items[i]);
	  }
  }
  else{
  	  printf("CLOSED Stack: \t");
  	  for (i = 0; i < CLOSED_count; i++) {
	    printf("%d ", s->items[i]);
	  }
  }
  	  printf("\t <-----(top)\n");
}

//function to set top of new stack to -1
void createEmptyStack(stack *s, char mod) {
  s->top = -1;
  if (mod== 'o')
  	  s->mode = 'o';
  else
  	  s->mode = 'c';
}

//check if the stack is full
int isfull(stack *s) {				//be careful. "isFull", uppercase F is apparently a reserved keyword.
  if (s->top == MAXSIZE - 1)
    return 1;
  else
    return 0;
}
// Check if the stack is empty
int isempty(stack *s) {				//be careful. "isEmpty", uppercase E is apparently a reserved keyword.
  if (s->top == -1)
    return 1;
  else
    return 0;
}

// Add elements into stack
void push(stack *s, int newitem) {
  if (isfull(s)) {
  	if(s->mode == 'o'){
	  	printf("\n OPEN STACK FULL \n");}
	  else{
	  	printf("\n CLOSED STACK FULL \n");}
  } else {
  	
    s->top++;
    s->items[s->top] = newitem;
 //   printf("DEBUG --- Item pushed= %d\n", s->items[s->top]);
      if(s->mode == 'o'){
	  	OPEN_count++;}
	  else{
	  	CLOSED_count++;}
  }
}
// Remove element from stack
int pop(stack *s) {
  if (isempty(s)) {
  //	if(s->mode == 'o'){
  //  	printf("\n OPEN STACK EMPTY \n");}
  // 	else{
  //		printf("\n CLOSED STACK EMPTY \n");}
  } else {
 	int popped = (s->items[s->top]);
 	   //printf("DEBUG --- Item popped= %d\n", popped);
    s->items[s->top] = -1;
    s->top--;
    
    if(s->mode == 'o'){
    	OPEN_count--;}
    else{
  		CLOSED_count--;}
  	return popped;
  }
}


//this function Inserts into the tree, the children of a node
void expandNode(nodePtr tree[], int node_index, int mode){
	//root points to root of array of struct
	//nPtr should be the node to be expanded
	
	if (mode == 1)
		pop(OPEN);						//remove the top of OPEN list, because we just expanded
	else
		enqueue(tree, node_index, CLOSED_LIST);//put node in CLOSED list
		
	CLOSED_count++;

	int i, pos;

	int *upNode;
	int *downNode;
	int *leftNode;
	int *rightNode;

	nodePtr nPtr = tree[node_index];
	
	pos = blankPosition(nPtr->config);							//get position of blank tile

	switch(pos){
		case 0:
			//move is right, down
			downNode = arrayAllocate(downNode);
			rightNode = arrayAllocate(rightNode);
			
			for(i=0; i<9; i++){
				rightNode[i] = nPtr->config[i];					//copy first the config of parent node
				downNode[i] = nPtr->config[i];
			}
			blankMove(rightNode, 'r');							//do the appropriate moves or swapping
			blankMove(downNode, 'd');
				insert(tree, node_index, rightNode, 'r');				//insert into the tree
				insert(tree, node_index, downNode, 'd');
			break;
		case 1:
			//left, right, down
			downNode = arrayAllocate(downNode);
			leftNode = arrayAllocate(leftNode);
			rightNode = arrayAllocate(rightNode);
			for(i=0; i<9; i++){
				leftNode[i] = nPtr->config[i];
				rightNode[i] = nPtr->config[i];
				downNode[i] = nPtr->config[i];
			}
			blankMove(leftNode, 'l');
			blankMove(rightNode, 'r');
			blankMove(downNode, 'd');
			
				insert(tree, node_index, rightNode, 'r');				//insert into the tree
				insert(tree, node_index, leftNode, 'l');
				insert(tree, node_index, downNode, 'd');
			break;
		case 2:
			//left, down
			downNode = arrayAllocate(downNode);
			leftNode = arrayAllocate(leftNode);
			for(i=0; i<9; i++){
				leftNode[i] = nPtr->config[i];
				downNode[i] = nPtr->config[i];
			}
			blankMove(leftNode, 'l');
			blankMove(downNode, 'd');
			
				insert(tree, node_index, leftNode, 'l');
				insert(tree, node_index, downNode, 'd');
			
			break;
		case 3:
			//up, right, down
			upNode = arrayAllocate(upNode);
			downNode = arrayAllocate(downNode);
			rightNode = arrayAllocate(rightNode);
			for(i=0; i<9; i++){
				upNode[i] = nPtr->config[i];
				rightNode[i] = nPtr->config[i];
				downNode[i] = nPtr->config[i];
			}
			blankMove(upNode, 'u');
			blankMove(rightNode, 'r');
			blankMove(downNode, 'd');
				insert(tree, node_index, rightNode, 'r');
				insert(tree, node_index, downNode, 'd');
				insert(tree, node_index, upNode, 'u');
			break;
		case 4:
			//up, right, down, left
			upNode = arrayAllocate(upNode);
			downNode = arrayAllocate(downNode);
			leftNode = arrayAllocate(leftNode);
			rightNode = arrayAllocate(rightNode);
			for(i=0; i<9; i++){
				upNode[i] = nPtr->config[i];
				leftNode[i] = nPtr->config[i];
				rightNode[i] = nPtr->config[i];
				downNode[i] = nPtr->config[i];
			}
			blankMove(upNode, 'u');
			blankMove(rightNode, 'r');
			blankMove(downNode, 'd');
			blankMove(leftNode, 'l');
			
				insert(tree, node_index, rightNode, 'r');
				insert(tree, node_index, leftNode, 'l');
				insert(tree, node_index, downNode, 'd');
				insert(tree, node_index, upNode, 'u');

			break;
		case 5:
			//up, left, down
			upNode = arrayAllocate(upNode);
			downNode = arrayAllocate(downNode);
			leftNode = arrayAllocate(leftNode);
			for(i=0; i<9; i++){
				upNode[i] = nPtr->config[i];
				leftNode[i] = nPtr->config[i];
				downNode[i] = nPtr->config[i];
			}
			blankMove(upNode, 'u');
			blankMove(leftNode, 'l');
			blankMove(downNode, 'd');
			
		
				insert(tree, node_index, leftNode, 'l');
				insert(tree, node_index, downNode, 'd');
				insert(tree, node_index, upNode, 'u');

			break;
		case 6:
			//up, right
			upNode = arrayAllocate(upNode);
			rightNode = arrayAllocate(rightNode);
			for(i=0; i<9; i++){
				upNode[i] = nPtr->config[i];
				rightNode[i] = nPtr->config[i];
			}
			blankMove(upNode, 'u');
			blankMove(rightNode, 'r');

				insert(tree, node_index, rightNode, 'r');
				insert(tree, node_index, upNode, 'u');

			break;
		case 7:
			//up, left, right
			upNode = arrayAllocate(upNode);
			leftNode = arrayAllocate(leftNode);
			rightNode = arrayAllocate(rightNode);
			for(i=0; i<9; i++){
				upNode[i] = nPtr->config[i];
				leftNode[i] = nPtr->config[i];
				rightNode[i] = nPtr->config[i];
			}
			blankMove(upNode, 'u');
			blankMove(rightNode, 'r');
			blankMove(leftNode, 'l');

				insert(tree, node_index, rightNode, 'r');
				insert(tree, node_index, leftNode, 'l');
				insert(tree, node_index, upNode, 'u');
			break;
		case 8:
			//up, left
			upNode = arrayAllocate(upNode);
			leftNode = arrayAllocate(leftNode);
			for(i=0; i<9; i++){
				upNode[i] = nPtr->config[i];
				leftNode[i] = nPtr->config[i];
			}
			blankMove(upNode, 'u');
			blankMove(leftNode, 'l');

				insert(tree, node_index, leftNode, 'l');
				insert(tree, node_index, upNode, 'u');
			break;
	}
	
	return;
}


//this function returns the position of blank tile on the Board
int blankPosition(int config[]){
	int i, pos;
	for(i=0; i<9; i++){
		if(config[i] == 0){				//finds the index of zero aka blank tile
			pos = i;
		}
	}
	return pos;
}

//this function changes the configuration of the Board, depending on the specified move
void blankMove(int config[], char move){
	int pos = blankPosition(config);
	
		switch(move){			//connect the last node to the new node
			case 'u':		//for up
				config[pos] = config[pos-3];
				config[pos-3] = 0;
				break;
			case 'd':		//for down
				config[pos] = config[pos+3];
				config[pos+3] = 0;
				break;
			case 'l':		//for left
				config[pos] = config[pos-1];
				config[pos-1] = 0;
				break;
			case 'r':		//for right
				config[pos] = config[pos+1];
				config[pos+1] = 0;
				break;
			default:
				break;
		}//end switch
}

//this function returns 1, if goalnode, 0 otherwise
int goalTest(nodePtr nPtr){
	if (nPtr->hn == 0)
		return 1;
	else
		return 0;
}

//returns 0 if unsolvable. 1 if it is (at most 31 moves)
int isSolvable(int *arrayPtr){

	int i, j;
	int inversion_count;
	
	for(i=0, inversion_count=0; i<8; i++){					//counts inversion count of goal node
		for(j=i+1; j<9; j++){
			if(goal[i] > goal[j]){
				inversion_count++;
			}
		}
	}
	int goal_inversion = inversion_count - blankPosition(goal);			//ignores the blank tile by subtracting it
	goal_inversion = goal_inversion %2;					//1 if odd, 0 if even
	
	for(i=0, inversion_count=0; i<8; i++){					//counts inversion count of input node
		for(j=i+1; j<9; j++){
			if(arrayPtr[i] > arrayPtr[j]){
				inversion_count++;
			}
		}
	}
	int starting_inversion = inversion_count - blankPosition(arrayPtr);
	starting_inversion = starting_inversion %2;			//1 if odd, 0 if even
	
	if(starting_inversion == goal_inversion){
		//printf("THIS IS SOLVABLE!!!\n");
		return 1;
	}
	else
		return 0;
}

//this function solves for h(n) using manhattan distance for A* search
int mnhtan_dist(int curr[], int goal[]){
	int result=0;														//curr for current node, goal is goal state, pos is evaluated position
	int i, j, k;
	int coords[9] = {0,10,20,1,11,21,2,12,22};						//assign coordinates to the grid. coords[0] being [0,0] and n[8] being [2,2]
	int h[9];
	//stores h(n) corresponding to the current state, in an array
	k=0;
	for(i=0; i<9; i++){
		for(j=0; j<9; j++){
			if(curr[i] == goal[j]){
				h[k] = abs((coords[i]/10) - (coords[j])/10) + abs((coords[i]%10) - (coords[j])%10) ;
				k++;
			}	
		}
	}
	for(i=0; i<9; i++){
		result += h[i];
	}
	
	return result;
}

//DEBUGGING function. prints out the values of specified node
void nodeCheck(nodePtr n){
	printf("\n\nnodeCheck results:\n");
	printf("ADDRESS of this node: %p\n", n);
	printf("node_id: %d\n", n->node_id);
	printf("config: \n");
	printGrid(n->config);
	
	printf("parent: %p\n", n->parent);
	printf("up: \t%p\n", n->up);
	printf("down: \t%p\n", n->down);
	printf("left: \t%p\n", n->left);
	printf("right: \t%p\n", n->right);
	
	printf("hn: %d\n", n->hn);
	printf("gn: %d\n", n->gn);
	printf("fvalue: %d\n", n->fvalue);
	printf("prevAction: %c\n", n->prevAction);
	printf("nodeCheck DONE\n\n");
	return;
}

//this function traces solution path, from the goal_index
void print_solution_path(nodePtr tree[], int goal_index){
	int debug_counter, i=0, j;
	nodePtr currPtr = tree[goal_index];
	printf("\n");
	print_hborder();
	printf("\t|\tSOLUTION PATH:\t");
	char move;
	char sol_path[31];
	
	do{
		move = currPtr->prevAction;
		
		sol_path[i] = move;		
		
		i++;
		
		currPtr = currPtr->parent;
		debug_counter++;	
			
	}while (currPtr != defaultPtr);
	
	for (j=0; i>=0; i--){
		move = sol_path[i];
		switch (move){
			case 'u':
				printf("-> UP ");
				j++;
				break;
			case 'd':
				printf("-> DOWN ");
				j++;
				break;
			case 'l':
				printf("-> LEFT ");
				j++;
				break;
			case 'r':
				printf("-> RIGHT ");
				j++;
				break;
		}
		if(j == 10){
			printf("\n\t|\t\t\t");
			j=0;
		}
		
	}
	printf("\n\t|\tSOLUTION COST:\t %d", tree[goal_index]->gn);
	printf("\n\t|\tRUNNING TIME:\t %f seconds", time_spent);
	printf("\n\t|\tNODES EXPANDED BEFORE REACHING GOAL:\t %llu \n", expanded_nodes_count);
	print_hborder();
	printf("\n\n");
	return;
}

//prints the nodes of the tree
void print_tree(nodePtr tree[]){
	int i;
	
	for(i=0; i<MAXSIZE; i++){
		if(tree[i] != NULL){
			printf("\nINDEX FROM TREE: [%d]", i);	//DEBUG
			nodeCheck(tree[i]);
		}
	}
}

void printGame(int n[], int accept){					//prints grid of initial and final state
		
		int i, j, k, x;									//counters
		
		printf("\t|\t\t\tINITIAL STATE:\t\t\t\t GOAL STATE:\t\t\t\t|\n");
		printf("\t|\t\t\t-------------\t\t\t\t-------------\t\t\t\t|\n");
		for(x=0; x<3; x++){
			printf("\t|\t\t\t");
				for(i=0; i<3; i++){
					if (n[x*3 +i] == 0)									//print 0 as '_'
						printf("| %c ", '_');
					else
						printf("| %d ", n[x*3 +i]);
				}
				if(x == 1){
					if(accept > 0){
						printf("|\t\t----->\t\t");}
					else{
						printf("|\t\t--X-->\t\t");}
				}else{
					printf("| \t\t\t\t");
				}
				
				for(i=0; i<3; i++){
					if (goal[x*3 +i] == 0)
						printf("| %c ", '_');
					else
						printf("| %d ", goal[x*3 +i]);
				}
				printf("| \t\t\t\t|\n");
			printf("\t|\t\t\t-------------\t\t\t\t-------------\t\t\t\t|\n");
		}
	return;
}


//this function prints the array (state configuration) in a formatted Board
void printGrid(int n[]){					//simply prints the grid
		printf("\t|\t\t\t -------------\t\t\t\t\t\t\t\t\t|\n");
		int i, j, k;									//counters
		for (i=0,j=0; j<3; j++){
			printf("\t|\t\t\t");
			for(k=0; k<3; k++){
				if (n[i] == 0)							//print 0 as '_'
					printf(" | %c", '_');
				else
					printf(" | %d", n[i]);
				i++;
			}
			printf(" |\t\t\t\t\t\t\t\t\t|\n");								//grid design
			printf("\t|\t\t\t -------------\t\t\t\t\t\t\t\t\t|\n");
	}
	return;
}
//this function is specially made for inputGrid. Prints Grid, shows current tile being edited
void printGrid_(int n[], int curr){
	
		int i, j, k;									//counters
		printf("\t|\t\t\t -------------\t\t\t\t\t\t\t\t\t|\n");
		for (i=0,j=0; j<3; j++){
			printf("\t|\t\t\t");
			for(k=0; k<3; k++){
				if (i == curr)
					printf(" | %c", '?');				// 0 as '_'
				else if ( n[i] == 0)
					printf(" | %c", '_');
				else if (n[i] == 9)
					printf(" | %c", ' ');				// 9 as 'X'
				else
					printf(" | %d", n[i]);				//numbers 1-8, print as normal
				i++;
			}
			printf(" |\t\t\t\t\t\t\t\t\t|\n");								//grid design
			printf("\t|\t\t\t -------------\t\t\t\t\t\t\t\t\t|\n");
	}
	return;
}

//functions to print a design border
void print_hborder(){
	printf("\t----------------------------------------------------");
	printf("-----------------------------------------------------\n");
	return;	
}
void print_vborder(){
	printf("\t|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
	return;
}

//returns what is preferred game mode
int main_menu(){
	
	int input;
	print_hborder();
		print_vborder();
		printf("\t|\t\t\t\t\t\t_MAIN MENU_\t\t\t\t\t\t|\n");
		print_vborder();
		printf("\t|\tWelcome to 8-Puzzle Solver!!! Please enter the GameMode you want to play...\t\t\t|\n");
		print_vborder();
		printf("\t|\t\t [1] - Easy mode \t\t\t\t\t\t\t\t\t|\n");
		printf("\t|\t\t [2] - Medium mode \t\t\t\t\t\t\t\t\t|\n");
		printf("\t|\t\t [3] - Hard mode \t\t\t\t\t\t\t\t\t|\n");
		printf("\t|\t\t [4] - Worst mode \t\t\t\t\t\t\t\t\t|\n");
		print_vborder();
		printf("\t|\t\t [5] - Preferred Initial Configuration mode \t\t\t\t\t\t|\n");
		printf("\t|\t\t [6] - Preferred Initial & Final State \t\t\t\t\t\t\t|\n");
		printf("\t|\t\t [7] - Debug mode \t\t\t\t\t\t\t\t\t|\n");
		print_vborder();
		printf("\t|\t\t [0] - Quit Game \t\t\t\t\t\t\t\t\t|\n");
		print_vborder();
	print_hborder();

	printf("\n\t\tYour INPUT: ");
	scanf("%d", &input);
	
	return input;
}

//different menu screens for different game modes
	int easy_menu(){
	
	int input;
	do{
		print_hborder();
			print_vborder();
			printf("\t|\tYou have chosen : [1] - Easy Mode.\t\t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t EASY mode Initial board Configuration: \t\t\t\t\t\t|\n");
			print_vborder();
			
			printGame(easy, 1);
			print_vborder();
			printf("\t|\tWhat Search Algorithm do you want to use? \t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t [1] - IDS (Iterative Depth Search) \t\t\t\t\t\t\t|\n");
			printf("\t|\t\t [2] - A* Search  \t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t [0] - CANCEL \t\t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
		print_hborder();
			
		printf("\n\t\tYour INPUT: ");
		scanf("%d", &input);
		
		if (input == 0){
			break;
		}
		else if (input == 1){
			printf("\n\t\tYou chose : [1] - IDS (Iterative Depth Search) to solve this puzzle.\n");
			input = IDS(easy);
			enter_checkpoint();
		}
		else if (input == 2){
			printf("\n\t\tYou chose : [2] - A* to solve this puzzle.\n");
			input = Astar(easy);
			enter_checkpoint();
		}
		else {
			input = -1;					//defaults invalid input to negative one
			printf("\t\tINVALID input! Please try again...\n");
			enter_checkpoint();
			clear_screen();
			title_screen();
		}
	} while (input < 0);
	
	return input;
}
int medium_menu(){
	
	int input;
	do{
		print_hborder();
			print_vborder();
			printf("\t|\tYou have chosen : [2] - Medium Mode.\t\t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t MEDIUM mode Initial board Configuration: \t\t\t\t\t\t|\n");
			print_vborder();
			
			printGame(medium, 1);
			print_vborder();
			printf("\t|\tWhat Search Algorithm do you want to use? \t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t [1] - IDS (Iterative Depth Search) \t\t\t\t\t\t\t|\n");
			printf("\t|\t\t [2] - A* Search  \t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t [0] - CANCEL \t\t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
		print_hborder();
			
		printf("\n\t\tYour INPUT: ");
		scanf("%d", &input);
		
		if (input == 0){
			break;
		}
		else if (input == 1){
			printf("\n\t\tYou chose : [1] - IDS (Iterative Depth Search) to solve this puzzle.\n");
			input = IDS(medium);
			enter_checkpoint();
		}
		else if (input == 2){
			printf("\n\t\tYou chose : [2] - A* to solve this puzzle.\n");
			input = Astar(medium);
			enter_checkpoint();
		}
		else {
			input = -1;					//defaults invalid input to negative one
			printf("\t\tINVALID input! Please try again...\n");
			enter_checkpoint();
			clear_screen();
			title_screen();
		}
	} while (input < 0);
	
	return input;
}
int hard_menu(){
	
	int input;
	do{
		print_hborder();
			print_vborder();
			printf("\t|\tYou have chosen : [3] - Hard Mode.\t\t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t HARD mode Initial board Configuration: \t\t\t\t\t\t|\n");
			print_vborder();
			
			printGame(hard, 1);
			print_vborder();
			printf("\t|\tWhat Search Algorithm do you want to use? \t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t [1] - IDS (Iterative Depth Search) \t\t\t\t\t\t\t|\n");
			printf("\t|\t\t [2] - A* Search  \t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t [0] - CANCEL \t\t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
		print_hborder();
			
		printf("\n\t\tYour INPUT: ");
		scanf("%d", &input);
		
		if (input == 0){
			break;
		}
		else if (input == 1){
			printf("\n\t\tYou chose : [1] - IDS (Iterative Depth Search) to solve this puzzle.\n");
			printf("\t\t\tProcessing...\n\n");
			input = IDS(hard);
			enter_checkpoint();
		}
		else if (input == 2){
			printf("\n\t\tYou chose : [2] - A* to solve this puzzle.\n");
			input = Astar(hard);
			enter_checkpoint();
		}
		else {
			input = -1;					//defaults invalid input to negative one
			printf("\t\tINVALID input! Please try again...\n");
			enter_checkpoint();
			clear_screen();
			title_screen();
		}
	} while (input < 0);
		
	return input;
}
int worst_menu(){
	
	int input;
	do{
		print_hborder();
			print_vborder();
			printf("\t|\tYou have chosen : [4] - Worst Mode.\t\t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t WORST mode Initial board Configuration: \t\t\t\t\t\t|\n");
			print_vborder();
			
			printGame(worst, 1);
			print_vborder();
			printf("\t|\tWhat Search Algorithm do you want to use? \t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t [1] - IDS (Iterative Depth Search) \t\t\t\t\t\t\t|\n");
			printf("\t|\t\t [2] - A* Search  \t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t [0] - CANCEL \t\t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
		print_hborder();
			
		printf("\n\t\tYour INPUT: ");
		scanf("%d", &input);
		
		if (input == 0){
			break;
		}
		else if (input == 1){
			printf("\n\t\tYou chose : [1] - IDS (Iterative Depth Search) to solve this puzzle.\n");
			printf("\t\t\tProcessing... ... ...\n\n");
			input = IDS(worst);
			enter_checkpoint();
		}
		else if (input == 2){
			printf("\n\t\tYou chose : [2] - A* to solve this puzzle.\n");
			printf("\t\t\tProcessing... ... ...\n\n");
			input = Astar(worst);
			enter_checkpoint();
		}
		else {
			input = -1;					//defaults invalid input to negative one
			printf("\t\tINVALID input! Please try again...\n");
			enter_checkpoint();
			clear_screen();
			title_screen();
		}
	} while (input < 0);
	
	return input;
}
int user_initial_menu(){
	
	int input, i;
	int *initial = malloc(9 * sizeof(int));
	int choice;

	clear_screen();
	do{
		
		for(i=0; i<9; i++){									//defaults the value of each tile as 9, meaning its unset
			initial[i] = 9;
		}
		
		choice = inputGrid(initial);							//prompt user to input BOARD CONFIGURATION
		
		if (choice < 0){			//user chooses to go back
			free(initial);			//discard array
			return input;
		}
		
		if( (isSolvable(initial)) == 0){					// if not solvable...
			print_hborder();
				print_vborder();
				printf("\t|\tWHOOPS! This initial configuration is NOT SOLVABLE with the given goal state.\t\t\t|\n");
				print_vborder();
				printGame(initial, -1);
				print_vborder();
				printf("\t|\tPlease Try again...\t\t\t\t\t\t\t\t\t\t|\n");
				print_vborder();
			print_hborder();
			enter_checkpoint();
			input = -1;
		}
		else break;
		
	}while (input < 0);
	
	input = -1;
	
	do {
		title_screen();
		print_hborder();
			print_vborder();
			printf("\t|\tWhat Search Algorithm would you like to use to solve this puzzle? \t\t\t\t|\n");
			print_vborder();
			printGame(initial, 1);
			print_vborder();
			printf("\t|\t\t [1] - IDS (Iterative Depth Search) \t\t\t\t\t\t\t|\n");
			printf("\t|\t\t [2] - A* Search  \t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t [0] - CANCEL \t\t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
		print_hborder();
		
		printf("\n\t\tYour INPUT: ");
		scanf("%d", &input);
		
		if (input == 0){
			break;
		}
		else if (input == 1){
			printf("\n\t\tYou chose : [1] - IDS (Iterative Depth Search) to solve this puzzle.\n");
			printf("\t\t\tProcessing...\n\n");
			input = IDS(initial);
			enter_checkpoint();
		}
		else if (input == 2){
			printf("\n\t\tYou chose : [2] - A* to solve this puzzle.\n");
			printf("\t\t\tProcessing...\n\n");
			input = Astar(initial);
			enter_checkpoint();
		}
		else {				//invalid input, default to -1
			printf("\t\tYour INPUT was not among the valid choices, silly!\n\t\tPlease try again...\n");
			delay(2);
			input = -1;
		}
		
	}while (input < 0);
	return input;
}
int user_goal_menu(){
	
	int input, i;
	int *initial = malloc(9 * sizeof(int));
	int choice;

	clear_screen();
	do{
		
		for(i=0; i<9; i++){									//defaults the value of each tile as 9, meaning its unset
			initial[i] = 9;
			goal[i] = 9;
		}
		
		choice = inputGoal(goal);
		if (choice < 0){			//user chooses to go back
			free(initial);			//discard array
			reset_Goal();
			return input;
		}
		
		choice = inputGrid(initial);							//prompt user to input BOARD CONFIGURATION
		if (choice < 0){			//user chooses to go back
			free(initial);			//discard array
			reset_Goal();
			return input;
		}
		
		if( (isSolvable(initial)) == 0){					// if not solvable...
			print_hborder();
				print_vborder();
				printf("\t|\tWHOOPS! This initial configuration is NOT SOLVABLE with the given goal state.\t\t\t|\n");
				print_vborder();
				printGame(initial, -1);
				print_vborder();
				printf("\t|\tPlease Try again...\t\t\t\t\t\t\t\t\t\t|\n");
				print_vborder();
			print_hborder();
			enter_checkpoint();
			input = -1;
		}
		else break;
		
	}while (input < 0);
	
	input = -1;
	
	do {
		title_screen();
		print_hborder();
			print_vborder();
			printf("\t|\tWhat Search Algorithm would you like to use to solve this puzzle? \t\t\t\t|\n");
			print_vborder();
			printGame(initial, 1);
			print_vborder();
			printf("\t|\t\t [1] - IDS (Iterative Depth Search) \t\t\t\t\t\t\t|\n");
			printf("\t|\t\t [2] - A* Search  \t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t [0] - CANCEL \t\t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
		print_hborder();
		
		printf("\n\t\tYour INPUT: ");
		scanf("%d", &input);
		
		if (input == 0){
			break;
		}
		else if (input == 1){
			printf("\n\t\tYou chose : [1] - IDS (Iterative Depth Search) to solve this puzzle.\n");
			printf("\t\t\tProcessing...\n\n");
			input = IDS(initial);
			enter_checkpoint();
		}
		else if (input == 2){
			printf("\n\t\tYou chose : [2] - A* to solve this puzzle.\n");
			printf("\t\t\tProcessing...\n\n");
			input = Astar(initial);
			enter_checkpoint();
		}
		else {				//invalid input, default to -1
			printf("\t\tYour INPUT was not among the valid choices, silly!\n\t\tPlease try again...\n");
			delay(2);
			input = -1;
		}
		
	}while (input < 0);
	reset_Goal();
	return input;
}
int debug_menu(){
	
	int input, i;
	int *initial = malloc(9 * sizeof(int));
	int choice;
	int flag;
	clear_screen();
	
	do{
		
		do{
			title_screen();
			print_hborder();
				print_vborder();
				printf("\t|\tYou have chosen : [7] - Debug Mode.\t\t\t\t\t\t\t\t|\n");
				print_vborder();
				printf("\t|\t\tDo you have a goal node in mind? \t\t\t\t\t\t\t|\n");
				print_vborder();
				printf("\t|\t\t [1] - NO. Just use the default one. \t\t\t\t\t\t\t|\n");
				printf("\t|\t\t [2] - YES. I want to try my own goal state. \t\t\t\t\t\t|\n");
				print_vborder();
				printf("\t|\t\t [0] - CANCEL \t\t\t\t\t\t\t\t\t\t|\n");
				print_vborder();
			print_hborder();
			
			printf("\n\t\tYour INPUT: ");
			scanf("%d", &input);
			
			if(input<0 || input>2){
				printf("\t\tYour INPUT was not among the valid choices, silly!\t\tPlease try again...\n");
				enter_checkpoint();
				flag = -1;
			}
			else flag=1;
			
		} while (flag<0);
		
		switch (input){
			case 0:
				return input;
			case 1:
				for(i=0; i<9; i++){									//defaults the value of each tile as 9, meaning its unset
					initial[i] = 9;
				}
				
				choice = inputGrid(initial);									//prompt user to input BOARD CONFIGURATION
				
				if (choice < 0){			//user chooses to go back
					free(initial);			//discard array
					return input;
				}
				break;
			case 2:
				for(i=0; i<9; i++){									//defaults the value of each tile as 9, meaning its unset
					initial[i] = 9;
					goal[i] = 9;
				}
				
				choice = inputGoal(goal);
				if (choice < 0){			//user chooses to go back
					free(initial);			//discard array
					reset_Goal();
					return input;
				}
				
				choice = inputGrid(initial);							//prompt user to input BOARD CONFIGURATION
				if (choice < 0){			//user chooses to go back
					free(initial);			//discard array
					reset_Goal();
					return input;
				}
				break;
			default:
				printf("debugger: shouldn't have reached this default!!! problem in function debug_menu();\n");
		}

		
		
		if( (isSolvable(initial)) == 0){					// if not solvable...
			print_hborder();
				print_vborder();
				printf("\t|\tWHOOPS! This initial configuration is NOT SOLVABLE with the given goal state.\t\t\t|\n");
				print_vborder();
				printGame(initial, -1);
				print_vborder();
				printf("\t|\tPlease Try again...\t\t\t\t\t\t\t\t\t\t|\n");
				print_vborder();
			print_hborder();
			enter_checkpoint();
			input = -1;
		}
		else break;
		
	}while (input < 0);
	
	input = -1;
	
	do {
		title_screen();
		print_hborder();
			print_vborder();
			printf("\t|\tWhat Search Algorithm would you like to use to solve this puzzle? \t\t\t\t|\n");
			print_vborder();
			printGame(initial, 1);
			print_vborder();
			printf("\t|\t\t [1] - IDS (Iterative Depth Search) \t\t\t\t\t\t\t|\n");
			printf("\t|\t\t [2] - A* Search  \t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
			printf("\t|\t\t [0] - CANCEL \t\t\t\t\t\t\t\t\t\t|\n");
			print_vborder();
		print_hborder();
		
		printf("\n\t\tYour INPUT: ");
		scanf("%d", &input);
		
		if (input == 0){
			break;
		}
		else if (input == 1){
			printf("\n\t\tYou chose : [1] - IDS (Iterative Depth Search) to solve this puzzle.\n\n");
			printf("\t\t\tProcessing...\n\n");
			
			input = IDS_debug(initial);
			enter_checkpoint();
		}
		else if (input == 2){
			printf("\n\t\tYou chose : [2] - A* to solve this puzzle.\n\n");
			printf("\t\t\tProcessing...\n\n");
			input = Astar_debug(initial);
			enter_checkpoint();
		}
		else {				//invalid input, default to -1
			printf("\t\tYour INPUT was not among the valid choices, silly!\n\t\tPlease try again...\n");
			delay(2);
			input = -1;
		}
		reset_Goal();
	}while (input < 0);
	return input;
}


void enter_checkpoint(){					//wait for user to input enter before proceeding
	printf("Press enter to continue...");
	while (getchar() != '\n');
	char enter = '0';
	while (enter != '\r' && enter !='\n'){
		enter = getchar();
	}
	return;
}
void delay(int seconds){					//delays program by the specified argument in seconds.
	int milli_sec = 1000*seconds;
	clock_t start_time = clock();
	while (clock() < start_time + milli_sec);
	return;	
}



//function to print what numbers are still available for the user to input
void print_available(int n[]){
	int i, j, x;
	int temp[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	print_vborder();
	printf("\t|\tAvailable INPUTS:\t");
	//1st for loop, remembers which numbers are already taken
	for(i=0; i<9; i++){
		if (n[i] != 9){
			temp[n[i]] = 9;
		}
	}
	//2nd loop prints all remainding numbers
	for(i=0, j=0; i<9; i++){
		if (temp[i] != 9){
			printf("[ %d ]\t", temp[i]);
			j++;
		}
	}
	
	for(;j<9;j++){
		printf("\t");
	}
		printf("|");
		printf("\n");
	print_vborder();
return;	
}

//this function prompts user to input the configuration of the board (n[0] to n[8])
int inputGrid(int n[]){
	
	int i=0, j=0, flag=0, num;
	char term;
	do{
		clear_screen();
		title_screen();
		print_hborder();
		print_vborder();
		printf("\t|\tPlease Input your desired INITIAL grid configuration:\t\t\t\t\t\t|\n");
		print_vborder();
		
		printGrid_(n, i);											//print the grid
		print_available(n);
		
		printf("\t|\t\t\t\t\t\t\t\t\t(enter 10 to cancel...)\t\t|\n");
		print_hborder();
		printf("\n\t\tPlease enter the value for n[%d]: ", i+1);
		
		if(scanf("%d%c", &num, &term) != 2 || term != '\n'){
			printf("ERROR! this is not an integer!\n");				//input validation
			flag = 1;												//flag 0 is accept. 1 is deny
			scanf("%*[^\n]");										//discard data in the buffer
			//delay(3);
			enter_checkpoint();
		}
		else if (num == 10){
			return -1;
		}
		else if (num<0 || num>8){
			printf("ERROR! Only numbers from 0-8 are accepted.");		//case if integer is out of range
			flag = 1;
			delay(2);
		}
		else{						//input is accepted
			if(i > 0){				//if board contains at least one number:
				//check if input already exists on the board
				for(j=0; j<(i+1); j++){
					if (num == n[j]){
						printf("ERROR! The number [%d] is already on the board. Please choose another number between 0-9.",num);
						delay(2);
						flag = 1;
						break;
					}
					else{
						flag = 0;
					}
				}
			}
			else{
				flag = 0;
			}
			if (flag != 1){
				updateGrid(n, i, num);				//update the array if input is not on board yet
				i++;
			}
		}
		flag = 1;
		clear_screen();
	}while (i<9);
	return 1;
}
//this function inserts the input into the array. Used exclusively by inputgrid
void updateGrid(int n[], int pos, int value){
	n[pos] = value;
	return;
}

//add parameter tree
void sort_stack(nodePtr tree[], stack *OPEN){
	
	int step, i, temp;
	int size = (OPEN->top)+1;
	
	//we don't want to sort the actual values of the stack (OPEN)
	//because OPEN holds index (of nodes) from the array of Ptrs "tree[]"
	//We want to sort it according to their fvalues, accessed by: tree[(OPEN->items[])]->fvalue
	
	// loop to access each array element
	
	  for (step = 0; step<size-1; ++step) {
	      
	    // loop to compare array elements bubble sort
	    for (i = 0; i<size-step-1; ++i) {
	      
	      // compare two adjacent elements
	      // < to sort in descending order
	      if ( (tree[(OPEN->items[i])]->fvalue) < (tree[(OPEN->items[i+1])]->fvalue) ) {			//we want smallest fvalue to be at top of stack
	        // swapping occurs if elements are not in intended order
	        temp = (OPEN->items[i]);
	        (OPEN->items[i]) = (OPEN->items[i+1]);
	        (OPEN->items[i+1]) = temp;
	      }
	    }
	  }
	  
	  //prioritize goal node, if ever there is a goal node
	  for(i=0; i<size; i++){
	  	if((tree[(OPEN->items[i])]->hn) == 0 ){
	  		//swap with top
	  		temp = (OPEN->items[OPEN->top]);
	  		OPEN->items[OPEN->top] = (OPEN->items[i]);
	  		(OPEN->items[i]) = temp;
		  }
	  }
	
	
	return;
}


//resets the value of Goal Array to default;
void reset_Goal(){
	goal[0] = 1;	goal[1] = 2;	goal[2] = 3;
	goal[3] = 8;	goal[4] = 0;	goal[5] = 4;
	goal[6] = 7;	goal[7] = 6;	goal[8] = 5;
	
	return;
}

int inputGoal(int n[]){
	int i=0, j=0, flag=0, num;
	char term;
	do{
		clear_screen();
		title_screen();
		print_hborder();
		print_vborder();
		printf("\t|\tPlease Input your desired GOAL configuration:\t\t\t\t\t\t\t|\n");
		print_vborder();
		
		printGrid_(n, i);											//print the grid
		print_available(n);
		
		printf("\t|\t\t\t\t\t\t\t\t\t(enter 10 to cancel...)\t\t|\n");
		print_hborder();
		printf("\n\t\tPlease enter the value for n[%d]: ", i+1);
		
		if(scanf("%d%c", &num, &term) != 2 || term != '\n'){
			printf("ERROR! this is not an integer!\n");				//input validation
			flag = 1;												//flag 0 is accept. 1 is deny
			scanf("%*[^\n]");										//discard data in the buffer
			//delay(3);
			enter_checkpoint();
		}
		else if (num == 10){
			return -1;
		}
		else if (num<0 || num>8){
			printf("ERROR! Only numbers from 0-8 are accepted.");		//case if integer is out of range
			flag = 1;
			delay(2);
		}
		else{						//input is accepted
			if(i > 0){				//if board contains at least one number:
				//check if input already exists on the board
				for(j=0; j<(i+1); j++){
					if (num == n[j]){
						printf("ERROR! The number [%d] is already on the board. Please choose another number between 0-9.",num);
						delay(2);
						flag = 1;
						break;
					}
					else{
						flag = 0;
					}
				}
			}
			else{
				flag = 0;
			}
			if (flag != 1){
				updateGrid(n, i, num);				//update the array if input is not on board yet
				i++;
			}
		}
		flag = 1;
		clear_screen();
	}while (i<9);
	return 1;
}

//this function draws title screen, from a .c file
void title_screen() {
	char *filename = "image.c";
    FILE *fptr = NULL;

    if((fptr = fopen(filename,"r")) == NULL) {				//FILE error handling
        fprintf(stderr,"error opening %s\n",filename);
    }
    char read_string[SIZE];										//read file
    clear_screen();
    while(fgets(read_string,sizeof(read_string),fptr) != NULL)		//print line by line
        printf("%s",read_string);
        
      fclose(fptr);
      return;
}

//this function draws closing screen
void closing_screen(){
	clear_screen();
	title_screen();
	
	print_hborder();
	print_vborder();
	printf("\t\t\t\t\t\t----THANKS FOR PLAYING!!!----\n");
	printf(" \n\t\t\t\t\t\t ...Terminating Program...\n");
	print_vborder();
	print_hborder();
	delay(1);
	return;
}

//this function clears the screen
void clear_screen(){
	//manually choose base on OS:
	/*									//50 newline method (not OS dependent, however a bit ugly)
	int i;
	for (i=0; i<10; i++)
		printf("\n\n\n\n\n");
	*/
	
	system("cls");						//for WINDOWS
//	system("clear");					//for gcc/g++ compiler in UNIX based systems
	return;
}

//function to insert to tail of list
void enqueue(nodePtr tree[], int index, list* head){
	list* currPtr;
	list* nextPtr;
//	list* newPtr;

	int i;
	
	
	if (glob_count == 1){	//check first if CLOSED_LIST is empty
		currPtr = head;
	}
	else {					//else, find tail of list
		nextPtr = head;
		do{
			currPtr  = nextPtr;
			nextPtr  = currPtr->nxtPtr;
		}while(nextPtr != NULL); 		// while there is still node in closed list
		
		
		currPtr->nxtPtr = (list*)malloc(sizeof(list));
		currPtr = currPtr->nxtPtr;
	}
	
	//assign values to the members of end of list
	for (i=0; i<9; i++){
		currPtr->board_config[i] = tree[index]->config[i];
	}
	currPtr->nxtPtr = NULL;
	currPtr->node_id = index;
	
	UNIQUE_STATES_EXPANDED++;
	
	return;
}


//function to remove head of list
void dequeue(nodePtr tree[], int index, list*head){
	list* temp = head;
	head = head->nxtPtr;
	free(temp);
}



//inserts into list already sorted , will replace push(OPEN) for A*
//removes need to sort the stack later
void insert_list(nodePtr tree[], int index, list* head){
	int i, inserted=0;
	list* currPtr = head;
	list* prevPtr = NULL;
	list* nextPtr = currPtr->nxtPtr;
	list* newPtr  = (list *)malloc(sizeof(list));
	
	newPtr->node_id = index;
	for (i=0; i<9; i++){
		newPtr->board_config[i] = tree[index]->config[i];
	}
	
	if(glob_count == 0){
		head = newPtr;
	}
	else if(goalTest(tree[index])){				//check if goal node,
		newPtr->nxtPtr = currPtr;			//insert at head of list
		head = newPtr;
	}
	else{	//find position to insert in O(n) time
	
		while(currPtr != NULL){
			if((tree[currPtr->node_id]->fvalue) < (tree[index]->fvalue)){
				//move to next item in list
				prevPtr = currPtr;
				currPtr = nextPtr;
				nextPtr = currPtr->nxtPtr;
				
			}else{
				//insert here
				prevPtr->nxtPtr = newPtr;
				newPtr->nxtPtr = currPtr;
				inserted = 1;
			}
		}
		//reached end of list, insert at end if not yet
		if(inserted != 1){
			prevPtr->nxtPtr = newPtr;
			newPtr->nxtPtr = currPtr;
			inserted = 1;
		}
	}
	
	
	
	
}

void freeList(list* head){
   list* tmp;
   while (head != NULL){
       tmp = head;
       head = head->nxtPtr;
       free(tmp);
    }
}

//searches closed list if the config already exists.
int is_state_unique(int config[], list* head){
	
	list* currPtr = head;
	list* nextPtr;
	
	do{
		nextPtr = currPtr->nxtPtr;
		
			if( compare_board(config, currPtr->board_config) ){ //check 
				//printf("DEBUG identical configs found!");
				return 0;	//not unique
			}
		currPtr = nextPtr;
	}while(currPtr != NULL);
	
	currPtr = head;

	
	
	//the config is unique.
	return 1;	
	}


int compare_board(int a[], int b[]){
	int i =0;
	for (i=0; i<9; i++){	//simple for loop for checking if each element of the 2 arrays matches
		if(a[i]!=b[i])
			return 0;			//return early once inequality is found
	}
	return 1;				//1 is returned if both arrays are identical
}


void print_queue(list* head){
	
	list* currPtr = head;
	list* nextPtr;
	
	do{
		nextPtr = currPtr->nxtPtr;
		printf("\t\t\t----DEBUG closed id: %d\n",currPtr->node_id);
		currPtr = nextPtr;
	}while(currPtr!= NULL);
	
}
