/* PARSER
 * Headerfile
 * Code by Ben
 * 
 * FUNKTION: 
 *	Pro Fall gibt es einen Graphen (gBlock, gState). Diese haben Bögen
 * 	und diese werden im Parser abgearbeitet. Es gibt einen Folge und 
 * 	einen Alternativbogen. Falls der Folgebogen 0 ist, dann wird die
 * 	Alternative gewählt wenn diese auch 0 ist. Gibt es einen Fehler, da
 * 	der Parser nicht bis zum Ende kommt. 
 * 
 * 	Damit sich die Graphen gegenseitig aufrufen können, wird ein Index
 * 	gespeichert und Array am Ende der die Graphenadressen besitzt. Mit 
 * 	dem Index kann man nun die Graphenadresse ausdem Array lesen. 
 * 
 */

#ifndef PARSER_H
#define PARSER_H

/* Includes 				*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>	

#include "color.h"
#include "lexer.h"
#include "parser.h"
#include "smr.h"
#include "codegen.h"


/* ---- BOGEN - INIT ------------------------------------------------ */
/* Bogentyp 							*/
typedef enum BOGEN_DESC
{
	BgNl= 0,  // NIL     
  BgSy= 1,  // Symbol  
  BgMo= 2,  // Morphem -> Ident
  BgGr= 4,  // Graph   
  BgEn= 8,  // Graphende 
}tBg;

/* Graphenindex						*/
typedef enum T_IdxGr
{
	iProg,iBlock,iState,iExpr,iTerm,iFact,iCond
}tIdxGr;

/* Bogen									*/
typedef struct BOGEN
{
	tBg BgD;  																/* Bogentyp 							*/
  union BGX 
  {
    unsigned long X; 												/* muss der Restreinpasse */
    int           S;												/* Symboladresse 					*/
    tMC           M;												/* Mophemcode 						*/
    tIdxGr        G; 												/* Adresse Graphindex			*/
  }BgX;
  int (*fx)(void); 													/* Aktion									*/
  int iNext; 																/* Nachfolgebogen 				*/
  int iAlt; 																/* Alternativbogen				*/	
}tBog;

/* Bogen									*/
typedef unsigned long ul;

#endif
