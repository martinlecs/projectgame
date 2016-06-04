/*
 *  Mr marFu.  The King of Games.
 *
 *  Proundly Created by Martin Le and Heng Fu Xiu
 *  Share Freely Creative Commons SA-BY-NC 3.0. 
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Game.h"
#include "mechanicalTurk.h"

#define NUM_OF_DISCIPLINES 6

action buildCampus (Game g, int uniID, action nextAction);
action buildArc (Game g, int uniID, action nextAction);
action buildGO8 (Game g, int uniID, action nextAction);

int possibleMove (Game g, int player, int actionCode);
int hasResource (Game g, int player, int actionCode);
action trade (Game g, int player, int actionCode);

action decideAction (Game g) {

   action nextAction;

   nextAction.actionCode = START_SPINOFF;
   int uniID = getWhoseTurn (g);
   int kpi = getKPIpoints (g, uniID);
   
	if (kpi < 30) {
		if (getARCs (g, uniID) % 2 == 1 && getARCs (g,uniID) <= 6) {
			if (hasResource (g, uniID, OBTAIN_ARC) == TRUE) {
				
				nextAction = buildArc (g, uniID, nextAction);
				
			} else if (possibleMove (g, uniID, OBTAIN_ARC) == TRUE) {
				
				nextAction = trade (g, uniID, OBTAIN_ARC);
				
			}
		} else if (getCampuses (g, uniID) <= 5) {
			if (hasResource (g, uniID, BUILD_CAMPUS) == TRUE) {
				
				nextAction = buildCampus (g, uniID, nextAction);
				
			} else if (possibleMove (g, uniID, BUILD_CAMPUS) == TRUE) {
				
				nextAction = trade (g, uniID, BUILD_CAMPUS);
			}
		}		
	} else if (kpi < 50) {
		if (hasResource (g, uniID, BUILD_GO8) == TRUE) {
			
			nextAction = buildGO8 (g, uniID, nextAction);
			
		} else if (possibleMove (g, uniID, BUILD_GO8) == TRUE) {
			
			nextAction = trade (g, uniID, BUILD_GO8);
			
		} else if (getARCs (g, uniID) % 2 == 1 && getARCs (g,uniID) <= 6) {
			if (hasResource (g, uniID, OBTAIN_ARC) == TRUE) {
				
				nextAction = buildArc (g, uniID, nextAction);
				
			} else if (possibleMove (g, uniID, OBTAIN_ARC) == TRUE) {
				
				nextAction = trade (g, uniID, OBTAIN_ARC);
				
			}			
		} else if (getCampuses (g, uniID) <= 5) {
			if (hasResource (g, uniID, BUILD_CAMPUS) == TRUE) {
				
				nextAction = buildCampus (g, uniID, nextAction);
				
			} else if (possibleMove (g, uniID, BUILD_CAMPUS) == TRUE) {
				
				nextAction = trade (g, uniID, BUILD_CAMPUS);
			}
		}
	} else if (kpi < 80 && getGO8s (g, uniID) <= 1) {
		if (hasResource (g, uniID, BUILD_GO8) == TRUE) {
			
			nextAction = buildGO8 (g, uniID, nextAction);
			
		} else if (possibleMove (g, uniID, BUILD_GO8) == TRUE) {
			
			nextAction = trade (g, uniID, BUILD_GO8);
			
		}
	} 
   /*
   nextAction.actionCode = START_SPINOFF;
     
   int uniID = getWhoseTurn (g);
   int numOfStudents[6] = {3,3,1,1,1,0};
    
   //number of BPS students
   numOfStudents[0] = getStudents (g, uniID, STUDENT_BPS); 
   //number of BQN students
   numOfStudents[1] = getStudents (g, uniID, STUDENT_BQN); 
   //number of MJ students
   numOfStudents[2] = getStudents (g, uniID, STUDENT_MJ); 
   //number of MTV students
   numOfStudents[3] = getStudents (g, uniID, STUDENT_MTV); 
   //number of M$ students
   numOfStudents[4] = getStudents (g, uniID, STUDENT_MMONEY); 
   //number of ThD students
   numOfStudents[5] = getStudents (g, uniID, STUDENT_THD); 

   
   }

   nextAction = buildArc (g, uniID, nextAction);
   nextAction = buildCampus (g, uniID, nextAction);
   
   // Checks if it is legal to start a spinoff
   // If it is not, then the AI passes
   if (isLegalAction (g, nextAction) == FALSE) {
	   nextAction.actionCode = PASS;
   }
   */
   if (isLegalAction (g, nextAction) == FALSE) {
	   nextAction.actionCode = PASS;
   }
   return nextAction;
}

action buildGO8 (Game g, int uniID, action nextAction) {
	
	action buildGO8;
	buildGO8.actionCode = BUILD_GO8;
	
	int i = 0;
	const int numGO8 = 5;
	path go8[numGO8];
	
	// Campus location in building order
	if (uniID == UNI_A) {
		strcpy (go8[0], "LRRLRLRLRLR");
		strcpy (go8[1], "");
		strcpy (go8[2], "LRRLRLRLR");
		strcpy (go8[3], "LRRLRLR");
		strcpy (go8[4], "LRRLR");
	} else if (uniID == UNI_B) {
		strcpy (go8[0], "RRLRL");
		strcpy (go8[1], "LRLRLRRLRL");
		strcpy (go8[2], "RRLRLLL");
		strcpy (go8[3], "RRLRLLLRL");
		strcpy (go8[4], "RRLRLLLRLRL");		
	} else if (uniID == UNI_C) {
		strcpy (go8[0], "RLRLRLRRLR");
		strcpy (go8[1], "LRLRL");
		strcpy (go8[2], "RLRLRLRR");
		strcpy (go8[3], "RLRLRL");
		strcpy (go8[4], "RLRL");
	}
	
	while (i < numGO8) {
		strcpy (buildGO8.destination, go8[i]);	
	
		if (isLegalAction(g, buildGO8) == TRUE ) {
			nextAction = buildGO8;
		}
		
		i++;
	}
	
	return nextAction;	
}

action buildCampus (Game g, int uniID, action nextAction) {
	
	action buildCampus;
	buildCampus.actionCode = BUILD_CAMPUS;
	
	int i = 0;
	const int numCampus = 3;
	path campus[numCampus];
	
	// Campus location in building order
	if (uniID == UNI_A) {
		strcpy (campus[0], "LRRLRLRLR");
		strcpy (campus[1], "LRRLRLR");
		strcpy (campus[2], "LRRLR");
	} else if (uniID == UNI_B) {
		strcpy (campus[0], "RRLRLLL");
		strcpy (campus[1], "RRLRLLLRL");
		strcpy (campus[2], "RRLRLLLRLRL");		
	} else if (uniID == UNI_C) {
		strcpy (campus[0], "RLRLRLRR");
		strcpy (campus[1], "RLRLRL");
		strcpy (campus[2], "RLRL");
	}
	
	while (i < numCampus) {
		strcpy (buildCampus.destination, campus[i]);	
	
		if (isLegalAction(g, buildCampus) == TRUE ) {
			nextAction = buildCampus;
		}
		
		i++;
	}
	
	return nextAction;	
}

action buildArc (Game g, int uniID, action nextAction) {

	action buildArc;
	buildArc.actionCode = OBTAIN_ARC;
	
	int i = 0;
	const int numArc = 6;
	path arc[numArc];
	
	// Arc location in building order
	if (uniID == UNI_A) {
		strcpy (arc[0], "LRRLRLRLRLR");
		strcpy (arc[1], "LRRLRLRLRL");
		strcpy (arc[2], "LRRLRLRLR");
		strcpy (arc[3], "LRRLRLRL");
		strcpy (arc[4], "LRRLRLR");
		strcpy (arc[5], "LRRLRL");
	} else if (uniID == UNI_B) {
		strcpy (arc[0], "RRLRLL");
		strcpy (arc[1], "RRLRLLL");
		strcpy (arc[2], "RRLRLLLR");
		strcpy (arc[3], "RRLRLLLRL");
		strcpy (arc[4], "RRLRLLLRLR");		
		strcpy (arc[5], "RRLRLLLRLRL");		
	} else if (uniID == UNI_C) {
		strcpy (arc[0], "RLRLRLRRLR");
		strcpy (arc[0], "RLRLRLRRL");
		strcpy (arc[1], "RLRLRLRR");
		strcpy (arc[1], "RLRLRLR");
		strcpy (arc[2], "RLRLRL");
		strcpy (arc[2], "RLRLR");
	}
	
	while (i < numArc) {
		strcpy (buildArc.destination, arc[i]);	
	
		if (isLegalAction(g, buildArc) == TRUE ) {
			nextAction = buildArc;
		}
		
		i++;
	}
	
	return nextAction;		
}

int possibleMove (Game g, int player, int actionCode) {
	int possibleMove = FALSE;
	
	// Establish required number of students
	int required[6];
	
	if (actionCode == OBTAIN_ARC) {
		required[STUDENT_THD] = 0;
		required[STUDENT_BPS] = 1;
		required[STUDENT_BQN] = 1;
		required[STUDENT_MJ] = 0;
		required[STUDENT_MTV] = 0;
		required[STUDENT_MMONEY] = 0;
	} else if (actionCode == BUILD_CAMPUS) {
		required[STUDENT_THD] = 0;
		required[STUDENT_BPS] = 1;
		required[STUDENT_BQN] = 1;
		required[STUDENT_MJ] = 1;
		required[STUDENT_MTV] = 1;
		required[STUDENT_MMONEY] = 0;
	} else if (actionCode == BUILD_GO8) {
		required[STUDENT_THD] = 0;
		required[STUDENT_BPS] = 0;
		required[STUDENT_BQN] = 0;
		required[STUDENT_MJ] = 2;
		required[STUDENT_MTV] = 0;
		required[STUDENT_MMONEY] = 3;
	}
	
	int i = 1; //Start at 1 to ignore THDs
	
	int students[6];
	int countInsufficient = 0;
	int countTraded = 0; //Counts the amount of students obtainable from trade
	
	while (i < 6) {
		students[i] = getStudents (g, player, i);
		
		if (students[i] >= required[i]) {
			students[i] -= required[i];
		} else {
			countInsufficient++;
		}
		
		i++;
	}

	i = 1;
	
	while (i < 6) {
		countTraded += (students[i] / 3);
		i++;
	}
	
	if (countTraded >= countInsufficient) { 
		possibleMove = TRUE;
	}
	
	return possibleMove;
}

int hasResource (Game g, int player, int actionCode) {
	int hasResource = FALSE;
	
	if (actionCode == OBTAIN_ARC) {
		if (getStudents (g, player, STUDENT_BPS) == 1 &&
			getStudents (g, player, STUDENT_BQN) == 1) {
				
			hasResource = TRUE;			
			
		}
	} else if (actionCode == BUILD_CAMPUS) {
		if (getStudents (g, player, STUDENT_BPS) == 1 &&
			getStudents (g, player, STUDENT_BQN) == 1 &&
			getStudents (g, player, STUDENT_MJ) == 1 &&
			getStudents (g, player, STUDENT_MTV) == 1) {
			
			hasResource = TRUE;
			
		}
	} else if (actionCode == BUILD_GO8) {
		if (getStudents (g, player, STUDENT_MJ) == 2 &&
			getStudents (g, player, STUDENT_MMONEY) == 3) {
				
			hasResource = TRUE;			
			
		}
	}
	
	return hasResource;
}

action trade (Game g, int player, int actionCode) { 
	action trade;
	
	int required[6];
	
	if (actionCode == OBTAIN_ARC) {
		required[STUDENT_THD] = 0;
		required[STUDENT_BPS] = 1;
		required[STUDENT_BQN] = 1;
		required[STUDENT_MJ] = 0;
		required[STUDENT_MTV] = 0;
		required[STUDENT_MMONEY] = 0;
	} else if (actionCode == BUILD_CAMPUS) {
		required[STUDENT_THD] = 0;
		required[STUDENT_BPS] = 1;
		required[STUDENT_BQN] = 1;
		required[STUDENT_MJ] = 1;
		required[STUDENT_MTV] = 1;
		required[STUDENT_MMONEY] = 0;
	} else if (actionCode == BUILD_GO8) {
		required[STUDENT_THD] = 0;
		required[STUDENT_BPS] = 0;
		required[STUDENT_BQN] = 0;
		required[STUDENT_MJ] = 2;
		required[STUDENT_MTV] = 0;
		required[STUDENT_MMONEY] = 3;
	}
	
	int i = 1;

	int students[6];
	int needsTrade[6] = {0,0,0,0,0,0}; // denotes whether needs trading, 1 for yes, 0 for no
	
	while (i < 6) {
		students[i] = getStudents (g, player, i);
		
		if (students[i] >= required[i]) {
			students[i] -= required[i]; // students available for trade
		} else {
			needsTrade[i] = 1;
		}
		
		i++;
	}
	
	int tradeFromIndex = 0;
	int tradeToIndex = 0;
	
	i = 1; //ignoring ThDs

	while (i < 6) {
		if (needsTrade[i] == 1) {
			tradeToIndex = i;
		}
		i++;
	}
	
	i = 1;
	
	while (i < 6) {
		if (students[i] >= getExchangeRate(g, player, i, tradeToIndex)) {
			tradeFromIndex = i;
		}	
		i++;
	}
	
	trade.actionCode = RETRAIN_STUDENTS;
	trade.disciplineFrom = tradeFromIndex;
	trade.disciplineTo = tradeToIndex;
	
	return trade;
}
