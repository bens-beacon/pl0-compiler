/* LEXER 
 * Headerfile
 * Code by Ben
 * 
 * FUNKTION:
 * 	Abbruch durch Extrazzeichen (9 -> EOF).
 * 	Schlüsselworterkennung durch Binäre Suche.
 * 	Schlüsselworterkennung erst ab Zeichen 2. Extrazustand 9.
 * 	Alles über 128 beendet und gibt Fehler aus.
 * 	Einzelne Buchstaben werden Ignoriert.
 * 
 */
 
#ifndef LEXER_H 
#define LEXER_H 

/* Includes 				*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "color.h"

#define OK 1
#define FAIL 0

/* ---- Morphemcodes Symbol, Zahl, Sonstiges ------------------------ */
/* Morphemtyp 						*/
typedef enum T_MC				
{
	mcEmpty, mcSymb, mcNum, mcIdent, mcStrin
}tMC;
/* Schlüsselwörter 				*/
typedef enum T_ZS									
{
	zNIL,
  zERG=128,zLE,zGE,zBGN,zCLL,zCST,zDO,zEND,zIF,zODD,zPRC,zTHN,zVAR,zWHL
  /* Start bei 128, zLE ist 129,... */
}tZS;
/* Morphem 								*/
typedef struct					
{
	tMC MC;																		/* Morphemtyp 						*/
	int PosLine;															/* Zeile 									*/
	int PosCol;																/* Spalte 								*/
	union VAL
	{
		long Num;
		char*pStr;
		int Symb;
	}Val;
	int MLen;																	/* Morphemlänge 					*/
}tMorph;

/* Prototypen:						*/
static void fl  (void);
static void bb  (void);
static void fb  (void);
static void fgl (void);
static void fsl (void);
static void fslb(void);
static void checkKeyw(void);
int binary_search( const char** M, int n, const char* X) ;

/* Automatentabelle				*/
struct vsCon {
	int Zstd;
  void (*FX)(void);
};

/* Prototypen							*/
tMorph* Lex(void);
int initLex(char* fname);

/* Global									*/
extern tMorph Morph;			
extern const char*  Keyw[];

#endif
