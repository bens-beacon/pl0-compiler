/* Maschienenbefehle
 * Headerfile
 * Code by Beck
 * 
 * FUNKTION:
 * 	Es beinhaltet alle wichtigen Befehle für die virtuelle Maschine.
 *  Diese Befehle spielen im Zwischencode eine wichtige Rolle. Mit 
 * 	Va_list gibt es eine Parameterliste die alle übergebenen Parameter
 * 	enthält.
 * 
*/

#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>	
#include <stdarg.h>

/* Includes 				*/
#include "namelist.h"
#include "color.h"
#include "lexer.h"
#include "code.h"

#define OK 1
#define FAIL 0

/* ---- Prototypen -------------------------------------------------- */
int code(tCode Code,...);										/* Schreib Befe in Codefi */
int CodeOut(void);													/* Schreiben Code raus 		*/
int openOFile(char* arg);										/* Öffne Ausgabefile			*/
int closeOFile(void);												/* Schließe Ausgabefile		*/
void wr2ToCode(short x);
void wr2ToCodeAtP(short x,char*pD);
int writeConstblock(void);
#endif
