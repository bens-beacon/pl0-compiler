/* Semantikroutine
 * Headerfile
 * Code by Ben
 * 
 * FUNKTION:
 * 	Die Semantikroutine schaut ob Variabledeklaration noch nicht 
 * 	verwendet wurden. Sie erstellt außerdem den Baum aus Listen zu dem
 * 	aktuellen Programm. Also Prozedur, Constanten, Variablen. 
 * 
 * 	Wichtig es gibt eine globale Variable die die aktuelle Prozedur
 * 	kennzeichnet.
 * 
*/

#ifndef SMR_H
#define SMR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>	

/* Includes 				*/
#include "codegen.h"
#include "namelist.h"
#include "color.h"

/* ---- Prototypen -------------------------------------------------- */
int bl1();																	/* Konstantenbezeichner		*/
int bl2();																	/* Konstante eintragen		*/
int bl3();																	/* Variablenbezeichner 		*/
int bl4();																	/* Procedurbezeichner			*/
int bl5();																	/* Procedur Ende					*/
int bl6();																	/* Anweisungsteil					*/

int pr1();

int st1();
int st2();
int st3();
int st4();
int st5();
int st6();
int st7();
int st8();
int st9();
int st10();



#endif
