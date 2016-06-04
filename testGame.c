// Jonathan Charles, z5115933
// Jeremy Gillen, z5118358
// fri09-sitar
// dd-mm-yy
// testGame.c file for the COMP1917 16s1 Project Knowledge Island

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

void jeremyTest(void);
void fuXiuTest(void);
void martinTest (void);
void jonathanTest(void);
void pennyTest(void);

typedef struct _game {
	int discipline[NUM_REGIONS];
	int currentTurn;
	int kpi;
	int patents;
	int papers;
	int numofBQN;
	int numofMMONEY;
	int numofMJ;
	int numofBPS;
	int numofMTV;
	int numofTHD;
	int numofCampuses;
} * Game;

typedef char path[PATH_LIMIT];

int main (int argc, char * argv[]) {

    pennyTest();	
    printf("pennyTest passed\n");
    fuXiuTest();
    printf("fuXiuTest passed\n");
    jonathanTest();
    printf("jonathanTest passed\n");
    martinTest();
    printf("martinTest passed\n");
    jeremyTest();
    printf("JeremyTest passed\n");

    printf("All tests passed, you are Awesome!\n");
    return EXIT_SUCCESS;
}

void jonathanTest (void) {
    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = DEFAULT_DICE;
    Game g = newGame(disciplines, dice);

    assert(g != NULL);
    assert(getDiscipline(g, 0) == STUDENT_BQN);
    assert(getDiscipline(g, 1) == STUDENT_MMONEY);
    assert(getDiscipline(g, 2) == STUDENT_MJ);
    assert(getDiscipline(g, 4) == STUDENT_MJ);
    assert(getDiscipline(g, 17) == STUDENT_BQN);
    assert(getDiscipline(g, 18) == STUDENT_BPS);

    // More tests could include if player A has a campus at a tile and gets a student from rolling that number?? 

    printf("Passed tests for getDiscipline!\n");

    assert(getDiceValue(g, 0) == 9);
    assert(getDiceValue(g, 1) == 10);
    assert(getDiceValue(g, 6) == 3);
    assert(getDiceValue(g, 8) == 3);
    assert(getDiceValue(g, 17) == 10);
    assert(getDiceValue(g, 18) == 5);

    printf("Passed tests for getDiceValue!\n");

    action a;
    a.actionCode = OBTAIN_PUBLICATION;
    throwDice(g,5); // increments turn
    if (getWhoseTurn(g) == UNI_A) {
        makeAction(g, a);
        assert(getPublications (g, UNI_A) == 1);
        assert(getPublications (g, UNI_B) == 0);
        assert(getPublications (g, UNI_C) == 0);
    }
    throwDice(g,5);
    if (getWhoseTurn(g) == UNI_B) {
        makeAction(g, a);
        assert(getPublications (g, UNI_A) == 1);
        assert(getPublications (g, UNI_B) == 1);
        assert(getPublications (g, UNI_C) == 0);
    }
    throwDice(g,5);    
    if (getWhoseTurn(g) == UNI_C) {
        makeAction(g, a);
        assert(getPublications (g, UNI_A) == 1);
        assert(getPublications (g, UNI_B) == 1);
        assert(getPublications (g, UNI_C) == 1);
    }
    disposeGame(g);

    printf("Passed tests for getPublication!\n");

    g = newGame(disciplines, dice);
    
    // Testing base cases before moving onto different cases
    
    // testing if the array is being accessed correctly and if player ID's are consistent
    assert(getStudents (g, UNI_A, disciplines[0]) == getStudents (g, UNI_A, disciplines[10]));
    assert(getStudents (g, UNI_A, disciplines[2]) == getStudents (g, UNI_B, disciplines[4]));
    assert(getStudents (g, UNI_A, disciplines[6]) == getStudents (g, UNI_B, disciplines[7]));
    assert(getStudents (g, UNI_A, disciplines[2]) == getStudents (g, UNI_B, disciplines[15]));
    assert(getStudents (g, UNI_A, disciplines[18]) == getStudents (g, UNI_B, disciplines[5]));

    assert(getStudents (g, UNI_B, disciplines[0]) == getStudents (g, UNI_C, disciplines[10]));
    assert(getStudents (g, UNI_B, disciplines[2]) == getStudents (g, UNI_C, disciplines[4]));
    assert(getStudents (g, UNI_B, disciplines[6]) == getStudents (g, UNI_C, disciplines[7]));
    assert(getStudents (g, UNI_B, disciplines[2]) == getStudents (g, UNI_C, disciplines[15]));
    assert(getStudents (g, UNI_B, disciplines[18]) == getStudents (g, UNI_C, disciplines[5]));

    assert(getStudents (g, UNI_C, disciplines[0]) == getStudents (g, UNI_A, disciplines[10]));
    assert(getStudents (g, UNI_C, disciplines[2]) == getStudents (g, UNI_A, disciplines[4]));
    assert(getStudents (g, UNI_C, disciplines[6]) == getStudents (g, UNI_A, disciplines[7]));
    assert(getStudents (g, UNI_C, disciplines[2]) == getStudents (g, UNI_A, disciplines[15]));
    assert(getStudents (g, UNI_C, disciplines[18]) == getStudents (g, UNI_A, disciplines[5]));

    // are players starting with right amount of students?
    assert(getStudents (g, UNI_A, disciplines[0]) == 3);
    assert(getStudents (g, UNI_A, disciplines[1]) == 1);
    assert(getStudents (g, UNI_A, disciplines[2]) == 1);
    assert(getStudents (g, UNI_A, disciplines[5]) == 3);
    assert(getStudents (g, UNI_A, disciplines[6]) == 1);
    assert(getStudents (g, UNI_A, disciplines[13]) == 0);
    
    assert(getStudents (g, UNI_B, disciplines[0]) == 3);
    assert(getStudents (g, UNI_B, disciplines[1]) == 1);
    assert(getStudents (g, UNI_B, disciplines[2]) == 1);
    assert(getStudents (g, UNI_B, disciplines[5]) == 3);
    assert(getStudents (g, UNI_B, disciplines[6]) == 1);
    assert(getStudents (g, UNI_B, disciplines[13]) == 0);

    assert(getStudents (g, UNI_C, disciplines[0]) == 3);
    assert(getStudents (g, UNI_C, disciplines[1]) == 1);
    assert(getStudents (g, UNI_C, disciplines[2]) == 1);
    assert(getStudents (g, UNI_C, disciplines[5]) == 3);
    assert(getStudents (g, UNI_C, disciplines[6]) == 1);
    assert(getStudents (g, UNI_C, disciplines[13]) == 0);
    
    disposeGame(g);
    g = newGame(disciplines, dice);

    printf("Passed tests for getStudents!\n");
    
    
    // Testing base cases before gameplay occurs

    assert(getExchangeRate (g, UNI_A, STUDENT_BQN, STUDENT_THD) == 3);
    assert(getExchangeRate (g, UNI_B, STUDENT_MMONEY, STUDENT_BQN) == 3);
    assert(getExchangeRate (g, UNI_B, STUDENT_MTV, STUDENT_MMONEY) == 3);
    assert(getExchangeRate (g, UNI_C, STUDENT_MJ, STUDENT_MTV) == 3);
    assert(getExchangeRate (g, UNI_C, STUDENT_BPS, STUDENT_MJ) == 3);
    
    // Testing that training centres can't stack in beneficial value
    assert(getExchangeRate (g, UNI_A,STUDENT_BQN, STUDENT_MMONEY) >= 2); 
    assert(getExchangeRate (g, UNI_A,STUDENT_BQN, STUDENT_MMONEY) <= 3);
    assert(getExchangeRate (g, UNI_A,STUDENT_MTV, STUDENT_MJ) >= 2); 
    assert(getExchangeRate (g, UNI_A,STUDENT_MTV, STUDENT_MJ) <= 3);
    assert(getExchangeRate (g, UNI_A,STUDENT_BPS, STUDENT_THD) >= 2); 
    assert(getExchangeRate (g, UNI_A,STUDENT_BPS, STUDENT_THD) <= 3);

    printf("Tests passed for getExchangeRate!\n");

    disposeGame(g);
}


void jeremyTest(void){

   // Tests finished but not comprehensive
   int disciplines[] = DEFAULT_DISCIPLINES;
   int dice[] = DEFAULT_DICE;
   Game g = newGame(disciplines, dice);


   //isLegalAction   **************************
   printf("testing isLegalAction\n");
   action a;
   a.actionCode = 8;
   assert(!(isLegalAction(g,a)));
   a.actionCode = -1;
   assert(!(isLegalAction(g,a)));
   strcpy(a.destination, "RRLRLL");
   assert(!(isLegalAction(g,a)));
   a.actionCode = 3;
   assert(!(isLegalAction(g,a)));
   
   //getKPIpoints    ***************************
   assert(getKPIpoints(g,1) == 20);
   assert(getKPIpoints(g,2) == 20);
   assert(getKPIpoints(g,3) == 20);
   
   //getARCs         ***************************
   assert(getARCs(g,1) == 0);
   assert(getARCs(g,2) == 0);
   assert(getARCs(g,3) == 0);
   //get some arcs and test again.
   
   //put down some arcs and a campus for player 1
   //test the legaliy of actions
   strcpy(a.destination, "RLRLRLRLRLL");
   throwDice(g,1);
   assert(isLegalAction(g,a));
   makeAction(g,a);                   //put down arc next to bottom campus A

   //test whether path that goes outside is illegal
   //test whether you can build a new campus where you already have a campus
   strcpy(a.destination, "BBRLRLRLRLRLR");
   assert(!(isLegalAction(g,a)));
   a.actionCode = 1;
   strcpy(a.destination, "");
   assert(!(isLegalAction(g,a)));
   
   a.actionCode = 3;
   strcpy(a.destination, "RLRLRLRLRL");
   assert(isLegalAction(g,a));
   makeAction(g,a);                  //put down another arc next to the other one.

   strcpy(a.destination, "RLRLRLRLR");
   a.actionCode = 1;
   assert(isLegalAction(g,a));
   makeAction(g,a);                  //put down a campus

   assert(!(isLegalAction(g,a)));
   
   
   //Test KPIpoints again

   assert(getKPIpoints(g,1) == 34);
   assert(getKPIpoints(g,2) == 20);
   assert(getKPIpoints(g,3) == 20);
   
   //Test getARCs again
   
   assert(getARCs(g,1) == 2);
   assert(getARCs(g,2) == 0);
   assert(getARCs(g,3) == 0);


   

   //testing the transfer of MTV and M$ students to THD
   throwDice(g,7);
   assert(getStudents(g,1,4) == 0);
   assert(getStudents(g,3,5) == 0);
   disposeGame(g);
}

void martinTest (void) {
    // Create a new game
    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = DEFAULT_DICE;
    Game g = newGame(disciplines, dice);

    assert (getWhoseTurn (g) == NO_ONE);
    throwDice (g, 5);

    int i = 0;

    // Runs a loop to check that a player turn order is correct
    while (i < 10) {
        assert (getWhoseTurn (g) == UNI_A);
        throwDice (g, 5);
        assert (getWhoseTurn (g) == UNI_B);
        throwDice (g, 5);
        assert (getWhoseTurn (g) == UNI_C);
        throwDice (g, 5);
        i++;
    }
    printf ("You passed the getWhoseTurn test!!!\n");

    //getCampus test
    
    //Check if starting points at beginning of the game are campuses
    assert (getCampus (g, "") == CAMPUS_A);
    assert (getCampus (g, "LRLRLRRLRLRRLRL") == CAMPUS_A);
    assert (getCampus (g, "RRLRL") == CAMPUS_B);
    assert (getCampus (g, "LRLRLRRLRL") == CAMPUS_B);
    assert (getCampus (g, "LRLRL") == CAMPUS_C);
    assert (getCampus (g, "RRLRLLRLRL") == CAMPUS_C);
    assert (getCampus (g, "LRRLR") == VACANT_VERTEX);
    assert (getCampus (g, "R") == VACANT_VERTEX);

    printf ("You passed getCampus tests!\n");

    //getARC test
    //Builds arcs using the resources given at the beginning of the game
    //The initial board state begins with no arcs,
	// Turn 0 - uni A places two arcs that are adjacent to each other
    
    disposeGame(g);
    g = newGame(disciplines, dice);
    //Initialise an action to obtain arc

    action obtainARC;

    obtainARC.actionCode = OBTAIN_ARC;

    //Turn 0 - UNI_A places its first two arcs adjacent to each other
    throwDice(g, 5);
    assert(getWhoseTurn(g) == UNI_A);
    strcpy (obtainARC.destination, "R");
    assert(isLegalAction(g, obtainARC) == TRUE);
    makeAction (g, obtainARC);
    strcpy (obtainARC.destination, "RL");
    assert(isLegalAction(g, obtainARC) == TRUE);
    makeAction (g, obtainARC);

    // Turn 1 - UNI_B places its first two arcs adjacent to each other
    throwDice(g, 5);
    assert(getWhoseTurn(g) == UNI_B);
    strcpy (obtainARC.destination, "RRLRLL");
    assert(isLegalAction(g, obtainARC) == TRUE);
    makeAction (g, obtainARC);
    strcpy (obtainARC.destination, "RRLRLLL");
    assert(isLegalAction(g, obtainARC) == TRUE);
    makeAction (g, obtainARC);

    // Turn 2 - UNI_C places its first two arcs adjacent to each other
    throwDice(g, 5);
    assert(getWhoseTurn(g) == UNI_C);
    strcpy (obtainARC.destination, "RLRLRLRRL");
    assert(isLegalAction(g, obtainARC) == TRUE);
    makeAction (g, obtainARC);
    strcpy (obtainARC.destination, "RLRLRLRRLR");
    assert(isLegalAction(g, obtainARC) == TRUE);
    makeAction (g, obtainARC);

    //Checks that the arcs we just built are there. Also tests edges that we know should be empty.
    assert (getARC (g, "R") == ARC_A);
    assert (getARC (g, "RL") == ARC_A);
    assert (getARC (g, "RRLRLL") == ARC_B);
    assert (getARC (g, "RRLRLLL") == ARC_B);
    assert (getARC (g, "RLRLRLRRL") == ARC_C);
    assert (getARC (g, "RLRLRLRRLR") == ARC_C);
    assert (getARC (g, "L") == VACANT_ARC);
    assert (getARC (g, "LRRLLR") == VACANT_ARC);

    printf("You have passed the 1st getARC test!\n");
    disposeGame(g);

    g = newGame(disciplines, dice);

    obtainARC.actionCode = OBTAIN_ARC;

    //Turn 0 - UNI_A places its first two arcs adjacent to each other
    throwDice(g, 5);
    assert(getWhoseTurn(g) == UNI_A);
    strcpy (obtainARC.destination, "R");
    assert(isLegalAction(g, obtainARC) == TRUE);
    makeAction (g, obtainARC);
    strcpy (obtainARC.destination, "RL");
    assert(isLegalAction(g, obtainARC) == TRUE);
    makeAction (g, obtainARC);

    // Turn 1 - UNI_B places its first two arcs adjacent to each other
    throwDice(g, 5);
    assert(getWhoseTurn(g) == UNI_B);
    strcpy (obtainARC.destination, "RRLRLL");
    assert(isLegalAction(g, obtainARC) == TRUE);
    makeAction (g, obtainARC);
    strcpy (obtainARC.destination, "RRLRLLL");
    assert(isLegalAction(g, obtainARC) == TRUE);
    makeAction (g, obtainARC);

    // Turn 2 - UNI_C places its first two arcs adjacent to each other
    throwDice(g, 5);
    assert(getWhoseTurn(g) == UNI_C);
    strcpy (obtainARC.destination, "RLRLRLRRL");
    assert(isLegalAction(g, obtainARC) == TRUE);
    makeAction (g, obtainARC);
    strcpy (obtainARC.destination, "RLRLRLRRLR");
    assert(isLegalAction(g, obtainARC) == TRUE);
    makeAction (g, obtainARC);

    disposeGame(g);
}


void fuXiuTest(void){
	// Initialise disciplines and a dice with only one possible output
	int disciplines[] = DEFAULT_DISCIPLINES;
        int dice[] = {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};
	
	// Testing GetMostARCS() **********
	// Initialise game board where all region have same ID
	Game g = newGame(disciplines, dice);

	// Initialise an action to obtain ARC
	action obtainARC;
	obtainARC.actionCode = OBTAIN_ARC;
	
	// Turn -1
	assert(getMostARCs(g) == NO_ONE);
	
	// Turn 0 - uni A obtain 1st ARC
	throwDice(g, 5);
	assert(getWhoseTurn(g) == UNI_A);
	strcpy (obtainARC.destination, "L");
	assert(isLegalAction(g, obtainARC) == TRUE);
	makeAction (g, obtainARC);
	
	// Turn 1 - uni B obtain 1st ARC
	throwDice(g, 5);
	assert(getMostARCs(g) == UNI_A);
	assert(getWhoseTurn(g) == UNI_B);
	strcpy (obtainARC.destination, "RRLRL");
	assert(isLegalAction(g, obtainARC) == TRUE);
	makeAction (g, obtainARC);
	
	// Turn 2 - uni C obtain 1st ARC
	throwDice(g, 5);
	assert(getMostARCs(g) == UNI_A);
	assert(getWhoseTurn(g) == UNI_C);
	strcpy (obtainARC.destination, "LRLRL");
	assert(isLegalAction(g, obtainARC) == TRUE);
	makeAction (g, obtainARC);
	assert(getMostARCs(g) == UNI_A);
	
	// Turn 3 - uni A didnt obtain ARC
	throwDice(g, 5);
	assert(getWhoseTurn(g) == UNI_A);
	assert(getMostARCs(g) == UNI_A);
	
	// Turn 4 - uni B obtain 2nd ARC
	throwDice(g, 5);
	assert(getWhoseTurn(g) == UNI_B);
	strcpy (obtainARC.destination, "RRLR");
	assert(isLegalAction(g, obtainARC) == TRUE);
	makeAction (g, obtainARC);
	
	//Turn 5 - uni C obtain 2nd ARC
	throwDice(g, 5);
	assert(getMostARCs(g) == UNI_B);
	assert(getWhoseTurn(g) == UNI_C);
	strcpy (obtainARC.destination, "LRLR");
	assert(isLegalAction(g, obtainARC) == TRUE);
	makeAction (g, obtainARC);
	assert(getMostARCs(g) == UNI_B);
	
	disposeGame(g);
	
	printf("You passed the getMostARCs test!\n");
	
	// Testing getMostPublications() **********
	g = newGame(disciplines, dice);
	
	// Initialise an action to obtain a publication
	action obtainPublication;
	obtainPublication.actionCode = OBTAIN_PUBLICATION;
	
	// Turn -1
	assert(getMostPublications(g) == NO_ONE);
	
	// Turn 0 - uni A obtain 1st Publication
	throwDice(g, 5);
	assert(getWhoseTurn(g) == UNI_A);
	assert(isLegalAction(g, obtainPublication) == FALSE);
	makeAction (g, obtainPublication);
	
	// Turn 1 - uni B obtain 1st Publication
	throwDice(g, 5);
	assert(getMostPublications(g) == UNI_A);
	assert(getWhoseTurn(g) == UNI_B);
	assert(isLegalAction(g, obtainPublication) == FALSE);
	makeAction (g, obtainPublication);
	
	// Turn 2 - uni C obtain 1st Publication
	throwDice(g, 5);
	assert(getMostPublications(g) == UNI_A);
	assert(getWhoseTurn(g) == UNI_C);
	assert(isLegalAction(g, obtainPublication) == FALSE);
	makeAction (g, obtainPublication);
	
	// Turn 3 - uni A didn't obtain publication
	throwDice(g, 5);
	assert(getMostPublications(g) == UNI_A);
	assert(getWhoseTurn(g) == UNI_A);
	
	// Turn 4 - uni B didn't obtain Publication
	throwDice(g, 5);
	assert(getMostPublications(g) == UNI_A);
	assert(getWhoseTurn(g) == UNI_B);
	assert(isLegalAction(g, obtainPublication) == FALSE);
	
	// Turn 5 - uni C obtain 2nd Publication
	throwDice(g, 5);
	assert(getMostPublications(g) == UNI_A);
	assert(getWhoseTurn(g) == UNI_C);
	assert(isLegalAction(g, obtainPublication) == FALSE);
	makeAction (g, obtainPublication);

	throwDice(g, 5);
	assert(getMostPublications(g) == UNI_C);
	
	disposeGame(g);
	
	printf("You passed the getMostPublications test!\n");
	
	// Testing GetTurnNumber() **********
	g = newGame(disciplines, dice);
	
	assert(getTurnNumber(g) == -1);
	throwDice(g, 5);
	assert(getTurnNumber(g) == 0);
	throwDice(g, 5);
	assert(getTurnNumber(g) == 1);
	throwDice(g, 5);
	assert(getTurnNumber(g) == 2);
	throwDice(g, 5);
	assert(getTurnNumber(g) == 3);
	
	disposeGame(g);	
	printf("You passed the getTurnNumber test!\n");	
}

void pennyTest (void) {
    
    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};
    Game g = newGame(disciplines, dice);
    throwDice(g,5);
    throwDice(g,5);
    throwDice(g,5);
    throwDice(g,5);
    int player = getWhoseTurn(g);
 
    // test getGO8s
    
    // ensure everyone starts with no GO8s!
    assert(getGO8s(g,UNI_A) == 0);
    assert(getGO8s(g,UNI_B) == 0);
    assert(getGO8s(g,UNI_C) == 0);
    
    // do GO8s increment correctly for each player
    action a;
    a.actionCode = BUILD_GO8;
    assert(player == UNI_A);
    makeAction(g,a);
    assert(getGO8s(g,UNI_A) == 1);
    assert(getGO8s(g,UNI_B) == 0);
    assert(getGO8s(g,UNI_C) == 0);
    throwDice(g,5);
    player = getWhoseTurn(g);
    assert(player == UNI_B);
    makeAction(g,a);
    assert(getGO8s(g,UNI_A) == 1);
    assert(getGO8s(g,UNI_B) == 1);
    assert(getGO8s(g,UNI_C) == 0);
    throwDice(g,5);
    player = getWhoseTurn(g);
    assert(player == UNI_C);
    makeAction(g,a);
    assert(getGO8s(g,UNI_A) == 1);
    assert(getGO8s(g,UNI_B) == 1);
    assert(getGO8s(g,UNI_C) == 1);
    
    disposeGame(g);
    
    printf("Passed tests for getGO8s!\n");
    
    // test getCampuses
    // game setup
    g = newGame(disciplines,dice);
    throwDice(g,5);
    throwDice(g,5);
    throwDice(g,5);
    throwDice(g,5);
    player = getWhoseTurn(g); 
   
    // ensure everyone starts with 2 Campuses!
    assert(getCampuses(g,UNI_A) == 2);
    assert(getCampuses(g,UNI_B) == 2);
    assert(getCampuses(g,UNI_C) == 2);
    
    // do Campuses increment correctly for each player
    a.actionCode = BUILD_CAMPUS;
    assert(player == UNI_A);
    makeAction(g,a);
    assert(getCampuses(g,UNI_A) == 3);
    assert(getCampuses(g,UNI_B) == 2);
    assert(getCampuses(g,UNI_C) == 2);
    throwDice(g,5);
    player = getWhoseTurn(g);
    assert(player == UNI_B);
    makeAction(g,a);
    assert(getCampuses(g,UNI_A) == 3);
    assert(getCampuses(g,UNI_B) == 3);
    assert(getCampuses(g,UNI_C) == 2);
    throwDice(g,5);
    player = getWhoseTurn(g);
    assert(player == UNI_C);
    makeAction(g,a);
    assert(getCampuses(g,UNI_A) == 3);
    assert(getCampuses(g,UNI_B) == 3);
    assert(getCampuses(g,UNI_C) == 3);

    disposeGame(g);
    
    printf("Passed tests for getCampuses!\n");
    
    // test getIPs
    // game setup
    g = newGame(disciplines,dice);
    throwDice(g,5);
    throwDice(g,5);
    throwDice(g,5);
    throwDice(g,5);
    player = getWhoseTurn(g);
    
    // check all players start with correct amount of data
    assert(getIPs(g,UNI_A) == 0);
    assert(getIPs(g,UNI_B) == 0);
    assert(getIPs(g,UNI_C) == 0);
    
    // does it increment corectly
    a.actionCode = OBTAIN_IP_PATENT;
    assert(player == UNI_A);
    makeAction(g,a);
    assert(getIPs(g,UNI_A) == 1);
    assert(getIPs(g,UNI_B) == 0);
    assert(getIPs(g,UNI_C) == 0);
    throwDice(g,5);
    player = getWhoseTurn(g);
    assert(player == UNI_B);
    makeAction(g,a);
    assert(getIPs(g,UNI_A) == 1);
    assert(getIPs(g,UNI_B) == 1);
    assert(getIPs(g,UNI_C) == 0);
    throwDice(g,5);
    player = getWhoseTurn(g);
    assert(player == UNI_C);
    makeAction(g,a);
    assert(getIPs(g,UNI_A) == 1);
    assert(getIPs(g,UNI_B) == 1);
    assert(getIPs(g,UNI_C) == 1);
    
    disposeGame(g);
    
    printf("Passed tests for getIPs!\n");
}
