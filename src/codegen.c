/* Codegenerieren
 * C-Code
 * Code by Ben
 * 
 * Statt dem pCode könnte man auch eine Relativadresse nehmen!
 * 
*/

#include "codegen.h"

char*  vCode;																/* Anfangsadresse im Speicher	*/
char*  pCode;    														/* pCode wandert durch als Zeiger */
short  IdxProc;  														/* Zähler für Prozedur		*/
int    LenCode;  														/* Länge Codeausgabeberei	*/
FILE*  pOFile;															/* Ausgabe Datei					*/
extern tProc* AktProc;
extern tlist* Constblock;
tMorph Morph;																/* Aktuelles Morphem			*/

/* Schreibe aktuelle Programmcounter	*/
void wr2ToCode(short x)
{
  *pCode++=(unsigned char)(x & 0xff);
  *pCode++=(unsigned char)(x >> 8);
}

/* Schreibe an Stelle char *pD				*/
void wr2ToCodeAtP(short x,char*pD)
{
  * pD   =(unsigned char)(x & 0xff);
  *(pD+1)=(unsigned char)(x >> 8);
}

/* Schreibe Befehl mit Parameter			*/
/* Dieser Funktion werden die Funktionen der Virtuellen Maschine  
 * übergeben. Also die Funktionsnummer und die dazugehörigen Parameter.
 * Danach wird die Funktionen mit ihren enthaltenen Parametern hinten 
 * ans File geschrieben. 
*/ 
int code(tCode Code,...)
{
  va_list ap;																/* Paramterliste 					*/
  short sarg;																/* Parametercount					*/

  if (pCode-vCode+MAX_LEN_OF_CODE>=LenCode) /* Wenn kein Speicher	 		*/
  {
		char* xCode=realloc(vCode,(LenCode+=1024));
		if (xCode==NULL)
		{
			printf(ANSI_COLOR_RED " codegen >> realloc >> cant get memory!\n");
			exit(EXIT_FAILURE);	
		}
    pCode=xCode+(pCode-vCode);
    vCode=xCode;
  }
  *pCode++=(char)Code;
  va_start(ap,Code);												/* initialisiere Paramlist*/
  
  switch (Code)
  {
    /* Befehle mit 3 Parametern */
    case entryProc: 	sarg=va_arg(ap,int);	/* bekomme nächsten Param */
											wr2ToCode(sarg);
    /* Befehle mit 2 Parametern */
    case puValVrGlob:
    case puAdrVrGlob: sarg=va_arg(ap,int);	/* bekomme nächsten Param */
											wr2ToCode(sarg);
    /* Befehle mit 1 Parameter */
    case puValVrMain:
    case puAdrVrMain:
    case puValVrLocl:
    case puAdrVrLocl:
    case puConst:
    case jmp :
    case jnot:
    case call:				sarg=va_arg(ap,int); /* Prozedurnummer               */
											wr2ToCode(sarg);
											break;

     /* keine Parameter 			*/
    case putStrg:
											if ((int)(pCode-vCode+strlen(Morph.Val.pStr)+1)>=LenCode)
											{
												char* xCode=realloc(vCode,(LenCode+=1024));
												if (xCode==NULL)
												{
													printf(ANSI_COLOR_RED " codegen >> realloc >> cant get memory!\n");
													exit(EXIT_FAILURE);	
												}		
												pCode=xCode+(pCode-vCode);
												vCode=xCode;
											}
											strcpy(pCode,Morph.Val.pStr);
											pCode+=strlen(pCode)+1;
											break;
     default: 				break;
 }
  va_end (ap);															/* Lösche Parameterliste	*/
  return OK;
}

/* Schreibe den Code ins File 				*/
int CodeOut(void)
{
  unsigned short Len=(unsigned short)(pCode-vCode);
  wr2ToCodeAtP((short)Len,vCode+1);							/* Schreibe Code     	*/
  wr2ToCodeAtP((short)AktProc->SpzzVar,vCode+5);/* Schreibe Variable 	*/
  if (Len==fwrite(vCode,sizeof(char),Len,pOFile)) return OK;
  else return FAIL;
}

/* Öffne das Codefile 								*/
int openOFile(char* arg)
{
  long  i=0L;
  char vName[128+1];
  strcpy(vName,arg);												/* Erstelle File mit Name */
  if (strstr(vName,".pl0")==NULL) strcat(vName,".cl0");
  else *(strchr(vName,'.')+1)='c';

  if ((pOFile=fopen(vName,"w"))!=NULL) 		  /* Öffne es								*/
  {
    fwrite(&i,sizeof(int32_t),1,pOFile);		/* Fülle es mit nullen		*/
    return OK;
  }
  else return FAIL;
}

/* Schließe das Codefile							*/
int closeOFile(void)
{
  writeConstblock();

  char vBuf2[2];                            /* Für big/Little Endian */
  fseek(pOFile,0,SEEK_SET);									/* setzte Cursor an start */
  wr2ToCodeAtP(IdxProc,vBuf2);							/* Schreibe Anzahl Proc		*/

  if (fwrite(vBuf2,2,1,pOFile)==2) return OK;
  else return FAIL;
}

/* Schreibe den Konstantenblock       */
int writeConstblock(void)
{
  fseek(pOFile,0,SEEK_END);                 /* Cursor ans Ende setzten*/

  /* Schreibe Constantenblock 				*/
	if(Constblock->first != NULL)
	{	
		Constblock->curr = Constblock->first;
		for(;; Constblock->curr = Constblock->curr->next)
		{
			tConst* Const_tmp = Constblock->curr->data;
			//wr2ToCode(Const_tmp->Val);
      fwrite(&Const_tmp->Val,sizeof(int16_t),1,pOFile);

			if(Constblock->curr==Constblock->last) break;	/* Abbruch		*/
		}
	}
  return OK;

}