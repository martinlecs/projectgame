/*
 *  Game.c 1917 v1.07
 *  Created By Fu Xiu Heng, Jonathan Charles, Martin Le, Penny Ou, Jeremy Gillen
 *  Licensed under Creative Commons SA-BY-NC 3.0.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Game.h"

#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}

// Directions
# define NORTH 0
# define EAST 1
# define SOUTH_EAST 2
# define SOUTH 3
# define WEST 4
# define NORTH_WEST 5

typedef struct _vertex * Vertex;
typedef struct _region * Region;
typedef struct _player * Player;
typedef struct _point * Point;

struct _vertex {
   
   // This integer represents the contents of this vertex, -1 if no contents
   int contents;

   // This int indicates whether there is a retraining center adjacent, -1 if none
   // uses standard integer disipline codes
   int retraining;

   // This integer array stores whether there is an arc in the direction of the index of the array.
   // the int in each array element is the ARC CONTENTS as defined in Game.h
   int arc[6];
   
};

struct _region {
   
   // the discipline of this region
   int discipline;

   // the dice number of this region
   int dicenumber;

};


struct _player {
   int kpi;
   int patents;
   int publications;
   
   int numofBQN;
   int numofMMONEY;
   int numofMJ;
   int numofBPS;
   int numofMTV;
   int numofTHD;
   
   int numofCampuses;
   int numofGO8s;
   int numofARCs;
   
};



struct _game {

   int currentTurn;
   Player players[4];
   Vertex allCampus[11][11];
   Region allRegions[11][11];
   int discipline[19];
   int dice[19];
   int mostPublications;
   int mostARCs;
    	

};

//this data structure stores the output of the getCoordinates function, which converts
//path strings to coordinates
struct _point {
    int x;
    int y;
    int direction;
    int previousX;
    int previousY;
};

Point getCoordinates(path pathToVertex);
Point makePoint(int x, int y);
Point makeEdge(int x, int y, int direction);

// checks if a coordinate is in the playable board Points
int isInBoard (Point coordinate);

// checks if a coordinate is in the playable board points
int pointInBoard (Point coordinate);
int edgeInBoard (Point coordinate);

// checks if there are any adjacent campuses/GO8 to a given point
int adjacentCampus (Game g, Point coordinate);
int adjacentARC (Game g, Point coordinate);


/*
int main (int argc, char * argv[]) {

    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = DEFAULT_DICE;
    Game g = newGame(disciplines, dice);
    printf("newGame seems to have worked\n");
    disposeGame(g);
    Point coordinates = getCoordinates("RL");
    printf("x:%d,y:%d\n",coordinates->x,coordinates->y);
    printf("All tests passed, you are Awesome!\n");
    return EXIT_SUCCESS;
}
*/

Game newGame (int discipline[], int dice[]) {

    Game g = (Game) malloc(sizeof(struct _game));
    if (g == NULL) {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    g->currentTurn = -1;
    g->mostPublications = 0;
    g->mostARCs = 0;
    memcpy(g->discipline, discipline, sizeof(int)*19);
    memcpy(g->dice, dice, sizeof(int)*19);
    //printf("initialised game\n");
    //set up each player with the right number of students
    int i = 0;
    while(i<4){
        g->players[i] = (Player)malloc(sizeof(struct _player));
        i++;
    }
    i = 0;
    while(i<4){
        g->players[i]->kpi = 20;
        g->players[i]->patents = 0;
        g->players[i]->publications = 0;
        g->players[i]->numofBQN = 3;
        g->players[i]->numofMMONEY = 1;
        g->players[i]->numofMJ = 1;
        g->players[i]->numofBPS = 3;
        g->players[i]->numofMTV = 1;
        g->players[i]->numofTHD = 0;
        g->players[i]->numofCampuses = 2;
        g->players[i]->numofGO8s = 0;
        g->players[i]->numofARCs = 0;
        i++;
    }

    
    int array[6] = {0};

    //set up each vertex with the initial board
    
    int arrayOfVertexContents[] = {-1,-1,-1,-1,-1,-1,-1, 0, 2,-1,-1,\
                                   -1,-1,-1,-1,-1, 0, 0,-1, 0, 0,-1,\
                                   -1,-1,-1, 1, 0,-1, 0, 0,-1, 0, 0,\
                                   -1,-1, 0,-1, 0, 0,-1, 0, 0,-1, 3,\
                                   -1,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1,\
                                   -1, 0,-1, 0, 0,-1, 0, 0,-1, 0,-1,\
                                   -1, 0, 0,-1, 0, 0,-1, 0, 0,-1,-1,\
                                    3,-1, 0, 0,-1, 0, 0,-1, 0,-1,-1,\
                                    0, 0,-1, 0, 0,-1, 0, 1,-1,-1,-1,\
                                   -1, 0, 0,-1, 0, 0,-1,-1,-1,-1,-1,\
                                   -1,-1, 2, 0,-1,-1,-1,-1,-1,-1,-1 };
    i = 0;
    int k = 0;
    int j = 0;
    while(i<11){
        j = 0;
        while(j<11){
            g->allCampus[j][i] = (Vertex)malloc(sizeof(struct _vertex));
            memcpy(g->allCampus[j][i]->arc,  array, sizeof(int)*6);
            g->allCampus[j][i]->contents = arrayOfVertexContents[k];
            g->allCampus[j][i]->retraining = -1;
//            printf("%d,",arrayOfVertexContents[k]);
            k++;
            j++;
        }
//        printf("\n");
        i++;

    }
    
    
    g->allCampus[1][6]->retraining = STUDENT_BPS;
    g->allCampus[1][5]->retraining = STUDENT_BPS;
    g->allCampus[6][1]->retraining = STUDENT_MJ;
    g->allCampus[5][1]->retraining = STUDENT_MJ;
    g->allCampus[8][1]->retraining = STUDENT_BQN;
    g->allCampus[9][1]->retraining = STUDENT_BQN;
    g->allCampus[5][9]->retraining = STUDENT_MTV;
    g->allCampus[6][8]->retraining = STUDENT_MTV;
    g->allCampus[8][6]->retraining = STUDENT_MMONEY;
    g->allCampus[9][5]->retraining = STUDENT_MMONEY;


//    printf("allCampus[2][10]->contents : %d,allCampus[5][7]->arc[3] : %d\n",
//            g->allCampus[2][10]->contents,g->allCampus[0][0]->arc[3]);

//    printf("looks like the campuses/verticies are initialised\n");
    
    //set up each region with disciplines and dice
    i = 0;
    j = 0;
    while(i<11){
        j = 0;
        while(j<11){
            g->allRegions[j][i] = (Region)malloc(sizeof(struct _region));
            g->allRegions[j][i]->discipline = -1;
            g->allRegions[j][i]->dicenumber = -1;
            j++;
        }
 //       printf("\n");
        i++;

    }       

    // I will forever feel bad about how terrible the following code is.
    g->allRegions[3][9]->discipline = discipline[0];
    g->allRegions[2][8]->discipline = discipline[1];
    g->allRegions[1][7]->discipline = discipline[2];
    g->allRegions[5][8]->discipline = discipline[3];
    g->allRegions[4][7]->discipline = discipline[4];
    g->allRegions[3][6]->discipline = discipline[5];
    g->allRegions[2][5]->discipline = discipline[6];
    g->allRegions[7][7]->discipline = discipline[7];
    g->allRegions[6][6]->discipline = discipline[8];
    g->allRegions[5][5]->discipline = discipline[9];
    g->allRegions[4][4]->discipline = discipline[10];
    g->allRegions[3][3]->discipline = discipline[11];
    g->allRegions[8][5]->discipline = discipline[12];
    g->allRegions[7][4]->discipline = discipline[13];
    g->allRegions[6][3]->discipline = discipline[14];
    g->allRegions[5][2]->discipline = discipline[15];
    g->allRegions[9][3]->discipline = discipline[16];
    g->allRegions[8][2]->discipline = discipline[17];
    g->allRegions[7][1]->discipline = discipline[18];

    g->allRegions[3][9]->dicenumber = dice[0];
    g->allRegions[2][8]->dicenumber = dice[1];
    g->allRegions[1][7]->dicenumber = dice[2];
    g->allRegions[5][8]->dicenumber = dice[3];
    g->allRegions[4][7]->dicenumber = dice[4];
    g->allRegions[3][6]->dicenumber = dice[5];
    g->allRegions[2][5]->dicenumber = dice[6];
    g->allRegions[7][7]->dicenumber = dice[7];
    g->allRegions[6][6]->dicenumber = dice[8];
    g->allRegions[5][5]->dicenumber = dice[9];
    g->allRegions[4][4]->dicenumber = dice[10];
    g->allRegions[3][3]->dicenumber = dice[11];
    g->allRegions[8][5]->dicenumber = dice[12];
    g->allRegions[7][4]->dicenumber = dice[13];
    g->allRegions[6][3]->dicenumber = dice[14];
    g->allRegions[5][2]->dicenumber = dice[15];
    g->allRegions[9][3]->dicenumber = dice[16];
    g->allRegions[8][2]->dicenumber = dice[17];
    g->allRegions[7][1]->dicenumber = dice[18];



    return g;
}

void disposeGame (Game g) {
    int i = 0;
    int j = 0;
    //free all Points on the coordinate grid
    while(i<11){
        j = 0;
        while(j<11){
            free(g->allCampus[i][j]);
            g->allCampus[i][j] = NULL;
            free(g->allRegions[i][j]);
            g->allRegions[i][j] = NULL;
            j++;
        }
        i++;
    }
    //free each player
    i = 0;
    while(i<4){
        free(g->players[i]);
        g->players[i] = NULL;
        i++;
    }
    //free the game struct
    free(g);
    g = NULL;
//    printf("game disposed\n");
}

void makeAction (Game g, action a) {
    int currentTurn = getWhoseTurn(g);
    if(a.actionCode == PASS){
        //does nothing 
    }else if(a.actionCode == BUILD_CAMPUS){
        //builds a campus (remember normal campus ints will match up with currentTurn ints)
        Point coordinates = getCoordinates(a.destination);
        g->allCampus[coordinates->x][coordinates->y]->contents = currentTurn;
        g->players[currentTurn]->kpi += 10;
        g->players[currentTurn]->numofCampuses += 1;
        g->players[currentTurn]->numofBPS -= 1;
        g->players[currentTurn]->numofBQN -= 1;
        g->players[currentTurn]->numofMJ -= 1;
        g->players[currentTurn]->numofMTV -= 1;
		free (coordinates);
    }else if(a.actionCode == BUILD_GO8){
        //puts a GO8 coded campus (normal campus +3 as in .h file) in struct
        //adds kpi Points and subtracts relevant students
        Point coordinates = getCoordinates(a.destination);
        g->allCampus[coordinates->x][coordinates->y]->contents = currentTurn + 3;
        g->players[currentTurn]->kpi += 10;
        g->players[currentTurn]->numofCampuses -= 1;
        g->players[currentTurn]->numofGO8s += 1;
        g->players[currentTurn]->numofMJ -= 2;
        g->players[currentTurn]->numofMMONEY -= 3;
		free (coordinates);
    }else if(a.actionCode == OBTAIN_ARC){
        Point coordinates = getCoordinates(a.destination);
        //adds arc reference on last Point of path
        g->allCampus[coordinates->x][coordinates->y]->arc[coordinates->direction] = currentTurn;
        //adds arc reference on second last Point
        g->allCampus[coordinates->previousX][coordinates->previousY]\
            ->arc[(coordinates->direction+3)%6] = currentTurn;
        g->players[currentTurn]->numofARCs += 1;
        g->players[currentTurn]->numofBPS -= 1;
        g->players[currentTurn]->numofBQN -= 1;
        g->players[currentTurn]->kpi += 2;
		free (coordinates);
    }else if(a.actionCode == OBTAIN_PUBLICATION){

        g->players[currentTurn]->publications += 1;
        g->players[currentTurn]->numofMJ -= 1;
        g->players[currentTurn]->numofMTV -= 1;
        g->players[currentTurn]->numofMMONEY -= 1;
    }else if(a.actionCode == OBTAIN_IP_PATENT){

        g->players[currentTurn]->patents += 1;
        g->players[currentTurn]->kpi += 10;
        g->players[currentTurn]->numofMJ -= 1;
        g->players[currentTurn]->numofMTV -= 1;
        g->players[currentTurn]->numofMMONEY -= 1;
    }else if(a.actionCode == RETRAIN_STUDENTS){
        int exchange = getExchangeRate(g, currentTurn, a.disciplineFrom, a.disciplineTo);
        if(a.disciplineFrom == STUDENT_BPS){
            g->players[currentTurn]->numofBPS -= exchange;
        }else if(a.disciplineFrom == STUDENT_BQN){
            g->players[currentTurn]->numofBQN -= exchange;
        }else if(a.disciplineFrom == STUDENT_MJ){
            g->players[currentTurn]->numofMJ -= exchange;
        }else if(a.disciplineFrom == STUDENT_MTV){
            g->players[currentTurn]->numofMTV -= exchange;
        }else if(a.disciplineFrom == STUDENT_MMONEY){
            g->players[currentTurn]->numofMMONEY -= exchange;
        }
        if(a.disciplineTo == STUDENT_BPS){
            g->players[currentTurn]->numofBPS += 1;
        }else if(a.disciplineTo == STUDENT_BQN){
            g->players[currentTurn]->numofBQN += 1;
        }else if(a.disciplineTo == STUDENT_MJ){
            g->players[currentTurn]->numofMJ += 1;
        }else if(a.disciplineTo == STUDENT_MTV){
            g->players[currentTurn]->numofMTV += 1;
        }else if(a.disciplineTo == STUDENT_MMONEY){
            g->players[currentTurn]->numofMMONEY += 1;
        }

    }

}

void throwDice (Game g, int diceScore) {
    //update mostPublications and mostARCs
    int p = 1;
    while(p<4){
        if(g->players[p]->publications > g->players[g->mostPublications]->publications){
            g->players[p]->kpi += 10;
            g->players[g->mostPublications]->kpi -=10;
            g->mostPublications = p;
        }
        if(g->players[p]->numofARCs > g->players[g->mostARCs]->numofARCs){
            g->players[p]->kpi += 10;
            g->players[g->mostARCs]->kpi -=10;
            g->mostARCs = p;
        }
        p++;
    }
    g->currentTurn++;
    //update student numbers
    if(diceScore == 7){
        p = 1;
        while(p<4){
            g->players[p]->numofTHD += g->players[p]->numofMMONEY;
            g->players[p]->numofTHD += g->players[p]->numofMTV;
            g->players[p]->numofMTV = 0;
            g->players[p]->numofMMONEY = 0;
            p++;
        }
    }
    int i = 0;
    int j = 0;
    int k = 0;
    int numAdded;
    int studentNumber;
    int *adjacentCampuses = malloc(sizeof(int)*2*6);
    while(i<11){

        j = 0;
        while(j<11){
            if(g->allRegions[i][j]->dicenumber == diceScore){
                //check each campus in this region
                k = 0;
                adjacentCampuses[0] = i;  
                adjacentCampuses[1] = j+1;
                adjacentCampuses[2] = i+1;
                adjacentCampuses[3] = j;
                adjacentCampuses[4] = i+1;
                adjacentCampuses[5] = j-1;
                adjacentCampuses[6] = i;
                adjacentCampuses[7] = j-1;
                adjacentCampuses[8] = i-1;
                adjacentCampuses[9] = j;
                adjacentCampuses[10] = j-1;
                adjacentCampuses[11] = j+1;

                while(k<12){
                    if(g->allCampus[adjacentCampuses[k]][adjacentCampuses[k+1]]->contents > 0){
                        if(g->allCampus[adjacentCampuses[k]][adjacentCampuses[k+1]]->contents <= 3){
                            numAdded = 1;
                            studentNumber = g->allCampus[adjacentCampuses[k]][adjacentCampuses[k+1]]->contents;
                        }else if(g->allCampus[adjacentCampuses[k]][adjacentCampuses[k+1]]->contents > 3){
                            numAdded = 2;
                            studentNumber = g->allCampus[adjacentCampuses[k]][adjacentCampuses[k+1]]->contents - 3;
                        }
                        if(g->allRegions[i][j]->discipline == STUDENT_THD){
                            g->players[studentNumber]->numofTHD += numAdded;
                        }else if(g->allRegions[i][j]->discipline == STUDENT_BPS){
                            g->players[studentNumber]->numofBPS += numAdded;
                        }else if(g->allRegions[i][j]->discipline == STUDENT_BQN){
                            g->players[studentNumber]->numofBQN += numAdded;
                        }else if(g->allRegions[i][j]->discipline == STUDENT_MJ){
                            g->players[studentNumber]->numofMJ += numAdded;
                        }else if(g->allRegions[i][j]->discipline == STUDENT_MTV){
                            g->players[studentNumber]->numofMTV += numAdded;
                        }else if(g->allRegions[i][j]->discipline == STUDENT_MMONEY){
                            g->players[studentNumber]->numofMMONEY += numAdded;
                        }
                    }
                    k += 2;
                }

            }
        j++;
        }
    i++;
    }
    free(adjacentCampuses);


}

int getDiscipline (Game g, int regionID) {
    
    return g->discipline[regionID];
}

int getDiceValue (Game g, int regionID) {
    
    return g->dice[regionID];
}

int getMostARCs (Game g) {
	
    return g->mostARCs;
}

int getMostPublications (Game g) {
	
    return g->mostPublications;
	
}

int getTurnNumber (Game g) {
    return (g->currentTurn);
}

int getWhoseTurn (Game g) {

    int currentplayer = 0;
    if (g->currentTurn == -1) {
        currentplayer = NO_ONE;
    } else if (g->currentTurn % 3 == 0) {
        currentplayer = UNI_A;
    } else if (g->currentTurn % 3 == 1) {
        currentplayer = UNI_B;
    } else if (g->currentTurn % 3 == 2) {
        currentplayer = UNI_C;
    }
    return currentplayer;
}

int getCampus(Game g, path pathToVertex) {
	
	Point campusPoint = getCoordinates (pathToVertex);
	
	int campusX = campusPoint->x;
	int campusY = campusPoint->y;	
	
	free (campusPoint);
	
    return (g->allCampus[campusX][campusY]->contents);
}

int getARC(Game g, path pathToEdge) {
    
	Point arcPoint = getCoordinates (pathToEdge);
	
	int arcX = arcPoint->x;
	int arcY = arcPoint->y;	
	int arcDir = arcPoint->direction;
	
	free (arcPoint);
	
    return (g->allCampus[arcX][arcY]->arc[arcDir]);
}

int isLegalAction (Game g, action a) {
    
	int isLegal=FALSE;
	int player = getWhoseTurn (g);
	
	// used to store paths as coordinates
	Point coordinate;
	int x;
	int y;
	int d;
	
	// used to determine if an action is illegal in some ways
	// and therefore collectively determine the if its legal
	int semiLegal1;
	int semiLegal2;
	int semiLegal3;
	int semiLegal4;
	
	// loop counter
	int i;
	
	if (a.actionCode == PASS) {
		
		isLegal = TRUE;
		
	} else if (a.actionCode == BUILD_CAMPUS) {
		
		/* isLegalAction BUILD_CAMPUS checklist
		1. check enough resources	
		2. check if that vertex is empty and in the board
		3. there is the player's arc connected to that vertex
		4. check there is no campus on six adjacent vertices
		*/
		
		// 1. check if enough resources
		if ((getStudents (g, player, STUDENT_BPS) >= 1) &&
			(getStudents (g, player, STUDENT_BQN) >= 1) &&
			(getStudents (g, player, STUDENT_MJ) >= 1) &&
			(getStudents (g, player, STUDENT_MTV) >= 1)) {
			semiLegal1 = TRUE;
		} else {
			semiLegal1 = FALSE;
		}
		
		// 2. check if Point is in the board and is vacant
		coordinate = getCoordinates (a.destination);
		x = coordinate->x;
		y = coordinate->y;

		if (pointInBoard (coordinate) == FALSE) {
			semiLegal2 = FALSE;
		} else if (g->allCampus[x][y]->contents != NO_ONE) {
			semiLegal2 = FALSE;
		} else {
			semiLegal2 = TRUE;
		}
		
		// 3. there is the player's arc connected to that vertex
		i = 0;
		semiLegal3 = FALSE;
		
		while (i < 6) {
			if (g->allCampus[x][y]->arc[i] == player) {
				semiLegal3 = TRUE;
			}
			i++;
		}
		
		// 4. check there is no campus/GO8 on six adjacent vertices
		if (adjacentCampus (g, coordinate) == TRUE) {
			semiLegal4 = TRUE;
		} else {
			semiLegal4 = FALSE;
		}
		
		// checking collectively whether isLegal
		if ((semiLegal1 == TRUE) && 
			(semiLegal2 == TRUE) && 
			(semiLegal3 == TRUE) && 
			(semiLegal4 == TRUE)) {
			isLegal = TRUE;
		} else {
			isLegal = FALSE;
		}
		
		free (coordinate);
		
	} else if (a.actionCode == BUILD_GO8) {

		/* isLegalAction BUILD_GO8 checklist
		1. check enough resources
		2. check if the player's campus is on that vertex
		3. check if the player does not already  have 8 GO8s
		*/
		
		// 1. check enough resources 
		if ((getStudents (g, player, STUDENT_MJ) >= 2) &&
			(getStudents (g, player, STUDENT_MMONEY) >= 3)) {
			semiLegal1 = TRUE;
		} else {
			semiLegal1 = FALSE;
		}
		
		// 2. check if the player's campus is on that vertex
		coordinate = getCoordinates (a.destination);
		x = coordinate->x;
		y = coordinate->y;
		
		if (g->allCampus[x][y]->contents == player) {
			semiLegal2 = TRUE;
		} else {
			semiLegal2 = FALSE;
		}
		
		// 3. check if the player does not already  have 8 GO8s
		if (getGO8s (g, player) < 8) {
			semiLegal3 = TRUE;
		} else {
			semiLegal3 = FALSE;
		}
		
		// checking collectively whether isLegal
		if ((semiLegal1 == TRUE) && 
			(semiLegal2 == TRUE) &&
			(semiLegal3 == TRUE)) {
			isLegal = TRUE;
		} else {
			isLegal = FALSE;
		}
		
		free (coordinate);
		
	} else if (a.actionCode == OBTAIN_ARC) {
		
		/* isLegalAction OBTAIN_ARC checklist
		1. check if enough resources
		2. check if that edge is empty and in the board
		3. check if there an arc or campus or GO8 connected to that edge
		4. check if direction is legal direction
		5. check if path lenght is less than PATH_LIMIT
		*/
		
		// 1. check if enough resources
		if ((getStudents (g, player, STUDENT_BQN) >= 1) &&
			(getStudents (g, player, STUDENT_BPS) >= 1)) {
			semiLegal1 = TRUE;
		} else {
			semiLegal1 = FALSE;
		}
		
		// 2. check if that edge is empty and in the board
		coordinate = getCoordinates (a.destination);
		x = coordinate->x;
		y = coordinate->y;
		d = coordinate->direction;
		
		if (edgeInBoard (coordinate) == FALSE) {
			semiLegal2 = FALSE;
		} else if (g->allCampus[x][y]->arc[d] != NO_ONE) { 
			semiLegal2 = FALSE;
		} else {
			semiLegal2 = TRUE;
		}
		
		// 3. check if there an arc, campus or GO8 connected to that edge
		if (adjacentCampus (g, coordinate) == TRUE) {
			semiLegal3 = TRUE;
		} else if (adjacentARC (g, coordinate) == TRUE){
			semiLegal3 = TRUE;
		} else {
			semiLegal3 = FALSE;
		}
		
		// 4. check if direction is legal direction
		if (d >= 0 && d <= 5) {
			semiLegal4 = TRUE;
		} else {
			semiLegal4 = FALSE;
		}
		
		// checking collectively whether isLegal
		if ((semiLegal1 == TRUE) && 
			(semiLegal2 == TRUE) && 
			(semiLegal3 == TRUE) &&
			(semiLegal4 == TRUE)) {
			isLegal = TRUE;
		} else {
			isLegal = FALSE;
		}
		
		free (coordinate);
		
	} else if (a.actionCode == START_SPINOFF) {
		
		//check if enough resources
		if ((getStudents (g, player, STUDENT_MJ) >= 1) &&
			(getStudents (g, player, STUDENT_MTV) >= 1) &&
			(getStudents (g, player, STUDENT_MMONEY) >= 1)) {
			isLegal = TRUE;
		} else {
			isLegal = FALSE;
		}
		
	} else if (a.actionCode == OBTAIN_PUBLICATION) {
		
		isLegal = FALSE;
		
	} else if (a.actionCode == OBTAIN_IP_PATENT) {
		
		isLegal = FALSE;
		
	} else if (a.actionCode == RETRAIN_STUDENTS) {
		
		/* isLegalAction RETRAIN_STUDENTS checklist
		1. check if discipline numbers are valid
		2. check if enough resources
		*/
		
		// 1. check if discipline number is valid
		if ((a.disciplineFrom < 0) || (a.disciplineTo > 5) ||
			(a.disciplineTo < 0) || (a.disciplineTo > 5)){
			semiLegal1 = FALSE;
		} else {
			semiLegal1 = TRUE;
		}
		
		// 2. check if enough resources
		int rate = getExchangeRate (g, player, a.disciplineFrom, a.disciplineTo);
		
		if (getStudents (g, player, a.disciplineFrom) >= rate) {
			semiLegal2 = TRUE;
		} else {
			semiLegal2 = FALSE;
		}
		
		// checking collectively whether isLegal
		if ((semiLegal1 == TRUE) && 
			(semiLegal2 == TRUE)) {
			isLegal = TRUE;
		} else {
			isLegal = FALSE;
		}
		
	}
	
    return isLegal;
}

int getKPIpoints (Game g, int player) {
    return (g->players[player]->kpi);
}

int getARCs (Game g, int player) {
    return (g->players[player]->numofARCs);
}

int getGO8s (Game g, int player) {
    return (g->players[player]->numofGO8s);
}

int getCampuses (Game g, int player) {
    return (g->players[player]->numofCampuses);   
}

int getIPs (Game g, int player) {
    return (g->players[player]->patents);;
}

int getPublications (Game g, int player) {
    
    // Need to understand our ADT so know how we track diff players data...
    int pubs= 0;
    if (player == UNI_A || player == UNI_B || player == UNI_C) {
        pubs = g->players[player]->publications;
    } else {
        pubs = 0;
    }
    return pubs;
}

int getStudents (Game g, int player, int discipline) {
  
    int students = 0;
    if (player == UNI_A || player == UNI_B || player == UNI_C) {
        if (discipline == STUDENT_THD) {
            students = g->players[player]->numofTHD;
        } else if (discipline == STUDENT_MTV) {
            students = g->players[player]->numofMTV;
        } else if (discipline == STUDENT_BPS) {
            students = g->players[player]->numofBPS;
        } else if (discipline == STUDENT_BQN) {
            students = g->players[player]->numofBQN;
        } else if (discipline == STUDENT_MJ) {
            students = g->players[player]->numofMJ;
        } else if (discipline == STUDENT_MMONEY) {
            students = g->players[player]->numofMMONEY;
        } else {
            students = 0;
        }
    } else {
        students = 0; // If player doesn't exist i.e. max 3 players
    }  
    return students;
}

int getExchangeRate (Game g, int player,
                     int disciplineFrom, int disciplineTo) {

    int rate = 3;
	
	int campusPlayer = NO_ONE;
	int go8Player = NO_ONE;
    
	if (player == UNI_A) {
		campusPlayer = CAMPUS_A;
		go8Player = GO8_A;
	} else if (player == UNI_B) {
		campusPlayer = CAMPUS_B;
		go8Player = GO8_B;
	} else if (player == UNI_C) {
		campusPlayer = CAMPUS_C;
		go8Player = GO8_C;
	}
	
	if (disciplineFrom == STUDENT_BPS) {
		if ((g->allCampus [1][5]->contents == campusPlayer) ||
			(g->allCampus [1][5]->contents == go8Player) ||
			(g->allCampus [1][6]->contents == campusPlayer) ||
			(g->allCampus [1][6]->contents == go8Player)) {
			
			rate = 2;
		}
	} else if (disciplineFrom == STUDENT_MJ) {
		if ((g->allCampus [6][1]->contents == campusPlayer) ||
			(g->allCampus [6][1]->contents == go8Player) ||
			(g->allCampus [5][1]->contents == campusPlayer) ||
			(g->allCampus [5][1]->contents == go8Player)) {
			
			rate = 2;
		}
	} else if (disciplineFrom == STUDENT_BQN) {
		if ((g->allCampus [8][1]->contents == campusPlayer) ||
			(g->allCampus [8][1]->contents == go8Player) ||
			(g->allCampus [9][1]->contents == campusPlayer) ||
			(g->allCampus [9][1]->contents == go8Player)) {
			
			rate = 2;
		}
	} else if (disciplineFrom == STUDENT_MTV) {
		if ((g->allCampus [5][9]->contents == campusPlayer) ||
			(g->allCampus [5][9]->contents == go8Player) ||
			(g->allCampus [6][8]->contents == campusPlayer) ||
			(g->allCampus [6][8]->contents == go8Player)) {
			
			rate = 2;
		}
	} else if (disciplineFrom == STUDENT_MMONEY) {
		if ((g->allCampus [8][6]->contents == campusPlayer) ||
			(g->allCampus [8][6]->contents == go8Player) ||
			(g->allCampus [9][5]->contents == campusPlayer) ||
			(g->allCampus [9][5]->contents == go8Player)) {
			
			rate = 2;
		}
	}
	
	return rate;
}

//this function converts between path strings and our coordinate system
Point getCoordinates(path pathToVertex){
    Point coordinates = makePoint(7,8);
    int direction = 3;
    char character;
    int i = 0;
    //traverse the path array
    while(pathToVertex[i] != '\0'){
        character = pathToVertex[i];
        if(character == 'L'){
            direction = (direction -1 + 6)%6;
        } else if(character == 'R'){
            direction = (direction +1)%6;
        } else if(character == 'B'){
            direction = (direction +3)%6;
        } 
        
        //finds the direction toward the arc that was just traversed
        coordinates->direction = (direction+3)%6;
        
        coordinates->previousY = coordinates->y;
        coordinates->previousX = coordinates->x;

        //finds the coordinates
        if(direction == 0){
            coordinates->y += 1;
        } else if(direction == 1){
            coordinates->x += 1;
        } else if(direction == 2){
            coordinates->x += 1;
            coordinates->y -= 1;
        } else if(direction == 3){
            coordinates->y -= 1;
        } else if(direction == 4){
            coordinates->x -= 1;
        } else if(direction == 5){
            coordinates->x -= 1;
            coordinates->y += 1;
        }

        i++;
    }
    return coordinates;
}

// isInBoard function checks if a Point lies in the playable board
// returns TRUE if so and FALSE otherwise
// coordinates for defining regions eg {3,3} returns FALSE;
int pointInBoard (Point coordinate) {
	
	const int totalValidPoints = 54;
	
	 //creates an array of all 54 valid Points
	Point validPoint[totalValidPoints];
	
	validPoint[0] = makePoint (0,7);
	validPoint[1] = makePoint (0,8);
	validPoint[2] = makePoint (1,8);
	validPoint[3] = makePoint (1,9);
	validPoint[4] = makePoint (2,9);
	validPoint[5] = makePoint (2,10);
	validPoint[6] = makePoint (3,10);
	
	validPoint[7] = makePoint (1,5);
	validPoint[8] = makePoint (1,6);
	validPoint[9] = makePoint (2,6);
	validPoint[10] = makePoint (2,7);
	validPoint[11] = makePoint (3,7);
	validPoint[12] = makePoint (3,8);
	validPoint[13] = makePoint (4,8);
	validPoint[14] = makePoint (4,9);
	validPoint[15] = makePoint (5,9);
	
	validPoint[16] = makePoint (2,3);
	validPoint[17] = makePoint (2,4);
	validPoint[18] = makePoint (3,4);
	validPoint[19] = makePoint (3,5);
	validPoint[20] = makePoint (4,5);
	validPoint[21] = makePoint (4,6);
	validPoint[22] = makePoint (5,6);
	validPoint[23] = makePoint (5,7);
	validPoint[24] = makePoint (6,7);
	validPoint[25] = makePoint (6,8);
	validPoint[26] = makePoint (7,8);
	
	validPoint[27] = makePoint (3,2);
	validPoint[28] = makePoint (4,2);
	validPoint[29] = makePoint (4,3);
	validPoint[30] = makePoint (5,3);
	validPoint[31] = makePoint (5,4);
	validPoint[32] = makePoint (6,4);
	validPoint[33] = makePoint (6,5);
	validPoint[34] = makePoint (7,5);
	validPoint[35] = makePoint (7,6);
	validPoint[36] = makePoint (8,6);
	validPoint[37] = makePoint (8,7);
	
	validPoint[38] = makePoint (5,1);
	validPoint[39] = makePoint (6,1);
	validPoint[40] = makePoint (6,2);
	validPoint[41] = makePoint (7,2);
	validPoint[42] = makePoint (7,3);
	validPoint[43] = makePoint (8,3);
	validPoint[44] = makePoint (8,4);
	validPoint[45] = makePoint (9,4);
	validPoint[46] = makePoint (9,5);

	validPoint[47] = makePoint (7,0);
	validPoint[48] = makePoint (8,0);
	validPoint[49] = makePoint (8,1);
	validPoint[50] = makePoint (9,1);
	validPoint[51] = makePoint (9,2);
	validPoint[52] = makePoint (10,2);
	validPoint[53] = makePoint (10,3);

	int inBoard = FALSE;
	int i = 0;
	
	// Compares input coordinate with all valid Points
	while (i < totalValidPoints) {
		if ((coordinate->x == validPoint[i]->x) &&
			(coordinate->y == validPoint[i]->y)) {
			
			inBoard = TRUE;
			
		}
		
        free(validPoint [i]);
		i++;
	}
	
	return inBoard;
}


// this function checks if there is a campus adjacent to a given Point
// isInBoard function checks if an edge lies in the playable board
// returns TRUE if so and FALSE otherwise
int edgeInBoard (Point coordinate) {
	
	const int totalValidEdges = 144;
	
	 //creates an array of all 54 valid points
	Point validEdge[totalValidEdges];
	
	validEdge[0] = makeEdge (0,7, NORTH);
	validEdge[1] = makeEdge (0,8, SOUTH);
	validEdge[2] = makeEdge (0,8, EAST);
	validEdge[3] = makeEdge (1,8, WEST);
	validEdge[4] = makeEdge (1,8, NORTH);
	validEdge[5] = makeEdge (1,9, SOUTH);
	validEdge[6] = makeEdge (1,9, EAST);
	validEdge[7] = makeEdge (2,9, WEST);
	validEdge[8] = makeEdge (2,9, NORTH);
	validEdge[9] = makeEdge (2,10, SOUTH);
	validEdge[10] = makeEdge (2,10, EAST);
	validEdge[11] = makeEdge (3,10, WEST);
	
	validEdge[12] = makeEdge (0,7, SOUTH_EAST);
	validEdge[13] = makeEdge (1,8, SOUTH_EAST);
	validEdge[14] = makeEdge (2,9, SOUTH_EAST);
	validEdge[15] = makeEdge (3,10, SOUTH_EAST);
	validEdge[16] = makeEdge (1,6, NORTH_WEST);
	validEdge[17] = makeEdge (2,7, NORTH_WEST);
	validEdge[18] = makeEdge (3,8, NORTH_WEST);
	validEdge[19] = makeEdge (4,9, NORTH_WEST);
	
	validEdge[20] = makeEdge (1,5, NORTH);
	validEdge[21] = makeEdge (1,6, SOUTH);
	validEdge[22] = makeEdge (1,6, EAST);
	validEdge[23] = makeEdge (2,6, WEST);
	validEdge[24] = makeEdge (2,6, NORTH);
	validEdge[25] = makeEdge (2,7, SOUTH);
	validEdge[26] = makeEdge (2,7, EAST);
	validEdge[27] = makeEdge (3,7, WEST);
	validEdge[28] = makeEdge (3,7, NORTH);
	validEdge[29] = makeEdge (3,8, SOUTH);
	validEdge[30] = makeEdge (3,8, EAST);
	validEdge[31] = makeEdge (4,8, WEST);
	validEdge[32] = makeEdge (4,8, NORTH);
	validEdge[33] = makeEdge (4,9, SOUTH);
	validEdge[34] = makeEdge (4,9, EAST);
	validEdge[35] = makeEdge (5,9, WEST);
	
	validEdge[36] = makeEdge (1,5, SOUTH_EAST);
	validEdge[37] = makeEdge (2,6, SOUTH_EAST);
	validEdge[38] = makeEdge (3,7, SOUTH_EAST);
	validEdge[39] = makeEdge (4,8, SOUTH_EAST);
	validEdge[40] = makeEdge (5,9, SOUTH_EAST);
	validEdge[41] = makeEdge (2,4, NORTH_WEST);
	validEdge[42] = makeEdge (3,5, NORTH_WEST);
	validEdge[43] = makeEdge (4,6, NORTH_WEST);
	validEdge[44] = makeEdge (5,7, NORTH_WEST);
	validEdge[45] = makeEdge (6,8, NORTH_WEST);
	
	validEdge[46] = makeEdge (2,3, NORTH);
	validEdge[47] = makeEdge (2,4, SOUTH);
	validEdge[48] = makeEdge (2,4, EAST);
	validEdge[49] = makeEdge (3,4, WEST);
	validEdge[50] = makeEdge (3,4, NORTH);
	validEdge[51] = makeEdge (3,5, SOUTH);
	validEdge[52] = makeEdge (3,5, EAST);
	validEdge[53] = makeEdge (4,5, WEST);
	validEdge[54] = makeEdge (4,5, NORTH);
	validEdge[55] = makeEdge (4,6, SOUTH);
	validEdge[56] = makeEdge (4,6, WEST);
	validEdge[57] = makeEdge (5,6, EAST);
	validEdge[58] = makeEdge (5,6, NORTH);
	validEdge[59] = makeEdge (5,7, SOUTH);
	validEdge[60] = makeEdge (5,7, EAST);
	validEdge[61] = makeEdge (6,7, WEST);
	validEdge[62] = makeEdge (6,7, NORTH);
	validEdge[63] = makeEdge (6,8, SOUTH);
	validEdge[64] = makeEdge (6,8, EAST);
	validEdge[65] = makeEdge (7,8, WEST);
	
	validEdge[66] = makeEdge (2,3, SOUTH_EAST);
	validEdge[67] = makeEdge (3,4, SOUTH_EAST);
	validEdge[68] = makeEdge (4,5, SOUTH_EAST);
	validEdge[69] = makeEdge (5,6, SOUTH_EAST);
	validEdge[70] = makeEdge (6,7, SOUTH_EAST);
	validEdge[71] = makeEdge (7,8, SOUTH_EAST);
	validEdge[72] = makeEdge (3,2, NORTH_WEST);
	validEdge[73] = makeEdge (4,3, NORTH_WEST);
	validEdge[74] = makeEdge (5,4, NORTH_WEST);
	validEdge[75] = makeEdge (6,5, NORTH_WEST);
	validEdge[76] = makeEdge (7,6, NORTH_WEST);
	validEdge[77] = makeEdge (8,7, NORTH_WEST);
	
	validEdge[78] = makeEdge (3,2, EAST);
	validEdge[79] = makeEdge (4,2, WEST);
	validEdge[80] = makeEdge (4,2, NORTH);
	validEdge[81] = makeEdge (4,3, SOUTH);
	validEdge[82] = makeEdge (4,3, EAST);
	validEdge[83] = makeEdge (5,3, WEST);
	validEdge[84] = makeEdge (5,3, NORTH);
	validEdge[85] = makeEdge (5,4, SOUTH);
	validEdge[86] = makeEdge (5,4, EAST);
	validEdge[87] = makeEdge (6,4, WEST);
	validEdge[88] = makeEdge (6,4, NORTH);
	validEdge[89] = makeEdge (6,5, SOUTH);
	validEdge[90] = makeEdge (6,5, EAST);
	validEdge[91] = makeEdge (7,5, WEST);
	validEdge[92] = makeEdge (7,5, NORTH);
	validEdge[93] = makeEdge (7,6, SOUTH);
	validEdge[94] = makeEdge (7,6, EAST);
	validEdge[95] = makeEdge (8,6, WEST);
	validEdge[96] = makeEdge (8,6, NORTH);
	validEdge[97] = makeEdge (8,7, SOUTH);
	
	validEdge[98] = makeEdge (4,2, SOUTH_EAST);
	validEdge[99] = makeEdge (5,3, SOUTH_EAST);
	validEdge[100] = makeEdge (6,4, SOUTH_EAST);
	validEdge[101] = makeEdge (7,5, SOUTH_EAST);
	validEdge[102] = makeEdge (8,6, SOUTH_EAST);
	validEdge[103] = makeEdge (5,1, NORTH_WEST);
	validEdge[104] = makeEdge (6,2, NORTH_WEST);
	validEdge[105] = makeEdge (7,3, NORTH_WEST);
	validEdge[106] = makeEdge (8,4, NORTH_WEST);
	validEdge[107] = makeEdge (9,5, NORTH_WEST);

	validEdge[108] = makeEdge (5,1, EAST);
	validEdge[109] = makeEdge (6,1, WEST);
	validEdge[110] = makeEdge (6,1, NORTH);
	validEdge[111] = makeEdge (6,2, SOUTH);
	validEdge[112] = makeEdge (6,2, EAST);
	validEdge[113] = makeEdge (7,2, WEST);
	validEdge[114] = makeEdge (7,2, NORTH);
	validEdge[115] = makeEdge (7,3, SOUTH);
	validEdge[116] = makeEdge (7,3, EAST);
	validEdge[117] = makeEdge (8,3, WEST);
	validEdge[118] = makeEdge (8,3, NORTH);
	validEdge[119] = makeEdge (8,4, SOUTH);
	validEdge[120] = makeEdge (8,4, EAST);
	validEdge[121] = makeEdge (9,4, WEST);
	validEdge[122] = makeEdge (9,4, NORTH);
	validEdge[123] = makeEdge (9,5, SOUTH);
	
	validEdge[124] = makeEdge (6,1, SOUTH_EAST);
	validEdge[125] = makeEdge (7,2, SOUTH_EAST);
	validEdge[126] = makeEdge (8,3, SOUTH_EAST);
	validEdge[127] = makeEdge (9,4, SOUTH_EAST);
	validEdge[128] = makeEdge (7,0, NORTH_WEST);
	validEdge[129] = makeEdge (8,1, NORTH_WEST);
	validEdge[130] = makeEdge (9,2, NORTH_WEST);
	validEdge[131] = makeEdge (10,3, NORTH_WEST);
	
	validEdge[132] = makeEdge (7,0, EAST);
	validEdge[133] = makeEdge (8,0, WEST);
	validEdge[134] = makeEdge (8,0, NORTH);
	validEdge[135] = makeEdge (8,1, SOUTH);
	validEdge[136] = makeEdge (8,1, EAST);
	validEdge[137] = makeEdge (9,1, WEST);
	validEdge[138] = makeEdge (9,1, NORTH);
	validEdge[139] = makeEdge (9,2, SOUTH);
	validEdge[140] = makeEdge (9,2, EAST);
	validEdge[141] = makeEdge (10,2, WEST);
	validEdge[142] = makeEdge (10,2, NORTH);
	validEdge[143] = makeEdge (10,3, SOUTH);
	
	int inBoard = FALSE;
	int i = 0;
	
	// Compares input coordinate with all valid points
	while (i < totalValidEdges) {
		if ((coordinate->x == validEdge[i]->x) &&
			(coordinate->y == validEdge[i]->y) &&
			(coordinate->direction == validEdge[i]->direction)) {
			
			inBoard = TRUE;
			
		}
		
		
        free(validEdge [i]);
		i++;
	}
	
	return inBoard;
}

// this function checks if there is a campus/GO8 adjacent to a given point
// returns TRUE if there is and FALSE otherwise.
int adjacentCampus (Game g, Point coordinate) {
	
	int x = coordinate->x;
	int y = coordinate->y;
	
	int isAdjacent = TRUE;
	
	if (x == 0) {
		
		if ((g->allCampus[x][y+1]->contents == NO_ONE) &&
			(g->allCampus[x][y-1]->contents == NO_ONE) &&
			(g->allCampus[x+1][y]->contents == NO_ONE) &&
			(g->allCampus[x+1][y-1]->contents == NO_ONE)) {
		
			isAdjacent = FALSE;
		
		}
		
	} else if (x == 10) {
		
		if ((g->allCampus[x][y+1]->contents == NO_ONE) &&
			(g->allCampus[x][y-1]->contents == NO_ONE) &&
			(g->allCampus[x-1][y]->contents == NO_ONE) &&
			(g->allCampus[x-1][y+1]->contents == NO_ONE)) {
		
			isAdjacent = FALSE;
		
		}
		
	} else if (y == 0) {
		
		if ((g->allCampus[x][y+1]->contents == NO_ONE) &&
			(g->allCampus[x+1][y]->contents == NO_ONE) &&
			(g->allCampus[x-1][y]->contents == NO_ONE) &&
			(g->allCampus[x-1][y+1]->contents == NO_ONE)) {
		
			isAdjacent = FALSE;
		
		}
		
	} else if (y == 10) {
		
		if ((g->allCampus[x][y-1]->contents == NO_ONE) &&
			(g->allCampus[x+1][y]->contents == NO_ONE) &&
			(g->allCampus[x-1][y]->contents == NO_ONE) &&
			(g->allCampus[x+1][y-1]->contents == NO_ONE)) {
		
			isAdjacent = FALSE;	
		}
		
	} else {
		
		if ((g->allCampus[x][y+1]->contents == NO_ONE) &&
			(g->allCampus[x][y-1]->contents == NO_ONE) &&
			(g->allCampus[x+1][y]->contents == NO_ONE) &&
			(g->allCampus[x-1][y]->contents == NO_ONE) &&
			(g->allCampus[x+1][y-1]->contents == NO_ONE) &&
			(g->allCampus[x-1][y+1]->contents == NO_ONE)) {
		
			isAdjacent = FALSE;
		}
	}
	
	return isAdjacent;
	
}

// this function checks if there is an ARC,belonging to 
// the current player adjacent to a given point
// returns TRUE if there is and FALSE otherwise.
int adjacentARC (Game g, Point coordinate) {

	int x = coordinate->x;
	int y = coordinate->y;
	
	int player = getWhoseTurn (g);
	
	int isAdjacent = FALSE;
	
	if (x == 0) {
		
		if ((g->allCampus[x][y]->arc[NORTH] == player) ||
			(g->allCampus[x][y]->arc[SOUTH] == player) ||
			(g->allCampus[x][y]->arc[EAST] == player) ||
			(g->allCampus[x][y]->arc[SOUTH_EAST] == player)) {
		
			isAdjacent = TRUE;
		
		}
		
	} else if (x == 10) {
		
		if ((g->allCampus[x][y]->arc[NORTH] == player) ||
			(g->allCampus[x][y]->arc[SOUTH] == player) ||
			(g->allCampus[x][y]->arc[WEST] == player) ||
			(g->allCampus[x][y]->arc[NORTH_WEST] == player)) {
		
			isAdjacent = TRUE;
			
		}
		
	} else if (y == 0) {
		
		if ((g->allCampus[x][y]->arc[NORTH] == player) ||
			(g->allCampus[x][y]->arc[EAST] == player) ||
			(g->allCampus[x][y]->arc[WEST] == player) ||
			(g->allCampus[x][y]->arc[NORTH_WEST] == player)) {
		
			isAdjacent = TRUE;
			
		}
		
	} else if (y == 10) {
		
		if ((g->allCampus[x][y]->arc[SOUTH] == player) ||
			(g->allCampus[x][y]->arc[EAST] == player) ||
			(g->allCampus[x][y]->arc[WEST] == player) ||
			(g->allCampus[x][y]->arc[SOUTH_EAST] == player)) {
		
			isAdjacent = TRUE;
			
		}
		
	} else {
		
		if ((g->allCampus[x][y]->arc[NORTH] == player) ||
			(g->allCampus[x][y]->arc[SOUTH] == player) ||
			(g->allCampus[x][y]->arc[EAST] == player) ||
			(g->allCampus[x][y]->arc[WEST] == player) ||
			(g->allCampus[x][y]->arc[NORTH_WEST] == player) ||
			(g->allCampus[x][y]->arc[SOUTH_EAST] == player)) {
		
			isAdjacent = TRUE;
			
		}
		
	}
	
	return isAdjacent;
}

Point makePoint(int x, int y) {
   Point p = malloc(sizeof(struct _point));
   p->x = x;
   p->y = y;
   return p;
}

Point makeEdge(int x, int y, int direction) {
   Point p = malloc(sizeof(struct _point));
   p->x = x;
   p->y = y;
   p->direction = direction;
   return p;
}
