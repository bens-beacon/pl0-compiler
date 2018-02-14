/* LEXER 
 * Headerfile
 * Code by Ben
 * 
 * FUNKTION:
 *  initLex() liest erstes Zeichen und speichert dies in Variable X. Als 
 *  nächstes wird LEX() aufgerufen. Das aktuelle Zeichen ist eine Zahl
 *  zwischen 0 und 127. Im Zeichenklassenvektor bekommt man den 
 *  richtigen Zustand zurück. Mit diesem Zustand kann nun aus der 
 *  Automatentabelle, der Folgezeile und die benötigte Funktion aus-
 *  gelesen werden. Je nach Funktion wird nun das Zeichen in den 
 *  Speicher geschrieben, ein neues gelesen oder beendet. Wenn beendet
 *  wird, dann ist die Zahl nicht die folge Zeile sondern der Fall in 
 *  der Beendenfunktion fb(). Dort wird das aktuelle Morphem 
 *  mit den wichtigen Parametern gesetzt. 
 * 
 * INFO:
 * 	Abbruch durch Zeichen 127 -> EOF.
 * 	Schlüsselworterkennung durch Binäre Suche.
 * 	Schlüsselworterkennung erst ab Zeichen 2.
 * 	Alles über 128 beendet und gibt Fehler aus.
 * 	Einzelne Buchstaben werden Ignoriert.
 *  Ident kann '_' und Zahlen beinhalten.
 *  Kommentare über eine Zeile sind möglich mit '#'
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
  zERG=128,zLE,zGE,zBGN,zCLL,zCST,zDO,zELSE,zEND,zIF,zODD,zPRC,zTHN,zVAR,zWHL
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
static void flb  (void);
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
