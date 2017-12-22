/* NAMELIST
 * Headerfile
 * Code by Ben
 * 
 * FUNKTION
 * 	Für jede Prozedur wird eine Liste erstellt. Jede Liste besteht aus
 * 	Bezeichnern. Bezeichnern können verschiedene Objekte angehängt 
 * 	werden. Variable, Konstante, Prozedur. Konstanten werden in einer 
 * 	extra Liste gespeichert. 
 * 
 *  Constblock = createlist();
 *  tBez * co_1 = createBezConst(Main,12,"CON");
 *  insertend(Main->pLBez,co_1);
 *  tBez * fr_1 = createBezVar(Main,"A");
 *	insertend(Main->pLBez,fr_1);	 
*/
#ifndef NAMELIST_H
#define NAMELIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>	

/* Includes 				*/
#include "list.h"
#include "color.h"

typedef enum tKZ{KzBez=0, KzProc, KzConst, KzVar}tKz;

/* ---- Baum-Typen -------------------------------------------------- */
typedef struct tBEZ
{
	tKz   Kz;																	/* Kennzeichen						*/
  short IdxProc;														/* Prozedurnummer					*/
  void* pObj;																/* Objekt Var, Const, Proc*/
  int   Len;
  char* pName;															/* Name 									*/
}tBez;

typedef struct tPROC			
{
	tKz			Kz;
	short 	IdxProc;													/* Prozedurnummer	0->Main */
	struct 	tPROC* pParent;										/* Pointer auf Parentproc */
	tlist 	*pLBez;														/* Namensliste der Proc		*/
	int 		SpzzVar;													/* Speicherplatzzuordnung, beginnt bei 0 wird von Var hochgezeählt */
}tProc;

typedef struct tCONST
{
	tKz		Kz;
	long 	Val;																/* Wert der Konstante			*/
	int 	Idx;																/* Index der Konstanten		*/
}tConst;

typedef struct tVAR
{
	tKz Kz;
	int Dspl;																	/* RealativAdresse 0 4 8	*/
}tVar;

typedef struct tLABL
{
  tKz Kz;																		/* Kennzeichen 						*/
  long iJmp;																/* Pointer in Codeausgabe */
}tLabl;

tlist* Constblock;													/* Konstanteliste					*/

/* ---- FunktionsPrototyp	------------------------------------------- */
void    clear(tProc* Proc);																							/* Lösche aktuellen Baum	*/
tBez*   globalsearchBEZ(tProc* Proc, char *pName);											/* Globale Suche 					*/
tBez*   searchBEZ(tProc* Proc, char *pName);														/* Suche in akt. Prozedur */
tConst* searchConst(long Val);																					/* Suche Konst in Konstbl */
tBez*   createBezConst(tProc* Proc,long Val,char *pName);								/* */
tBez*   createBezConstIdx(tProc* Proc,long Val,char *pName,int Idx);
tBez*   createBezVar(tProc* Proc,char *pName);
tBez*   createBezProc(tProc* Proc, char *pName);
tProc*  createProc(short IdxProc, tProc* pParent);

int pushLabel(void);												/* Label	 								*/
int popAndInsLabl(int x);
int popLabl(void);

#endif



