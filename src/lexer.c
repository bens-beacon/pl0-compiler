/* LEXER
 * C-Code 
 * Code by Ben
 * 
*/

#include "lexer.h"

/* Lexer									*/
static int    Z;														/* Zustand 								*/
static tMorph MorphInit;										/* ??? */
extern tMorph Morph;												/* Aktuelles Morphem			*/
static char   vBuf[1024+1];									/* Ausgabebuffer 					*/
static char*  pBuf;													/* Pointer Ausgabebuffer 	*/
int    line,col;										      	/* Zeilen, Spaltennr Dok.	*/
static int 		End;

/* ---- Zeichenklassenvector -----------------------------------------*/
static char vZKl[128]=
/*      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F     */
/*­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­*/
 /* 0*/{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 10, 7, 7, 7, 7, 7,/* 0*/
 /*10*/ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,/*10*/
 /*20*/ 7, 0, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,/*20*/
 /*30*/ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 5, 4, 6, 0,/*30*/
 /*40*/ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,/*40*/
 /*50*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2,/*50*/
 /*60*/ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,/*60*/
 /*70*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,11}/*70*/;

/* ---- Automatentabelle ---------------------------------------------*/
struct vsCon vSMatrx[][12] = {
/*        So    	  Z      	 Bu         ':'      '='    	 '<'    	'>'   	Space  	  '"'        '#'        'LF'     EOF   */
/*­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­ 0 */{{ 0,fslb},{ 1,fsl },{ 2,fgl },{ 3,fsl },{ 0,fslb},{ 4,fsl },{ 5,fsl },{ 0,fl },{ 9,fl  },{11,fl  },{ 0,fl  },{0,bb }},
/* 1 */{{ 0,fb  },{ 1,fsl },{10,fgl },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb },{ 0,fb  },{ 0,fb  },{ 0,fb  },{0,bb }},
/* 2 */{{ 0,fb  },{ 2,fsl },{10,fgl },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb },{ 0,fb  },{ 0,fb  },{ 0,fb  },{0,bb }},
/* 3 */{{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 6,fsl },{ 0,fb  },{ 0,fb  },{ 0,fb },{ 0,fb  },{ 0,fb  },{ 0,fb  },{0,bb }},
/* 4 */{{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 7,fsl },{ 0,fb  },{ 0,fb  },{ 0,fb },{ 0,fb  },{ 0,fb  },{ 0,fb  },{0,bb }},
/* 5 */{{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 8,fsl },{ 0,fb  },{ 0,fb  },{ 0,fb },{ 0,fb  },{ 0,fb  },{ 0,fb  },{0,bb }},
/* 6 */{{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb },{ 0,fb  },{ 0,fb  },{ 0,fb  },{0,bb }},
/* 7 */{{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb },{ 0,fb  },{ 0,fb  },{ 0,fb  },{0,bb }},
/* 8 */{{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb },{ 0,fb  },{ 0,fb  },{ 0,fb  },{0,bb }},
/* 9 */{{ 9,fsl },{ 9,fsl },{ 9,fsl },{ 9,fsl },{ 9,fsl },{ 9,fsl },{ 9,fsl },{ 9,fsl},{ 0,flb },{ 0,fb  },{ 0,fb  },{0,bb }},
/*10 */{{ 0,fb  },{10,fgl },{10,fgl },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb  },{ 0,fb },{ 0,fb  },{ 0,fb  },{ 0,fb  },{0,bb }},
/*11 */{{11,fsl },{11,fsl },{11,fsl },{11,fsl },{11,fsl },{11,fsl },{11,fsl },{11,fsl},{11,fsl },{11,fsl },{ 0,flb },{0,bb }}
};

/*­­­­ ---- Initialisierung der lexiaklischen Analyse ------------------- ­­­­*/
static FILE  *pIF;
static int    X;														/* erstes Zeichen Datei 	*/

int initLex(char* fname)
{
	char vName[128+1];
  /* Kopiere Filename und teste auf .pl0 */
  strcpy(vName,fname);
  if (strstr(vName,".pl0")==NULL) strcat(vName,".pl0");
  /* Öffne File und hole erstes Zeichen (fgetc) */
  pIF=fopen(vName,"r+t");
  if (pIF!=NULL) 
  {
    X=fgetc(pIF); 
    //printf(ANSI_COLOR_GREEN " >> Valid File! \n"); 
    return OK;
  }
  printf(ANSI_COLOR_RED " >> Invalid File! \n");
  exit(EXIT_FAILURE);
  return FAIL;
}

/* ---- Lexer ------------------------------------------------------- */
tMorph* Lex(void)
{
  Z=0; 																			/* Anfangszeile   				*/
	int naeZ;																	/* Folgezustand 					*/
	End=0;
	
	/* Setze aktuelle Morph */
  Morph 				= MorphInit;
  Morph.PosLine = line;
  Morph.PosCol 	= col;
  pBuf 					= vBuf;

  /* Rufe Funktion auf 		*/
	do
  {
		
		if(X>=128)															/* Abbruch bei Ä,Ü,.. 		*/
		{
			printf(ANSI_COLOR_RED " >> Invalid Character: %c!\n",X);
			exit(EXIT_FAILURE);
		}

		/* &0x7f schneidet alles über 8 bit weg, um nicht über 127 zu kom */
		naeZ = (vSMatrx[Z][vZKl[X&0x7f]]).Zstd;	/* aktueller Zustand 			*/
		(vSMatrx[Z][vZKl[X&0x7f]]).FX();				/* aktuelle function 			*/
		Z=naeZ;
	}while (End==0);
  return &Morph; 
}

/* ---- Funktionen für Automaten ------------------------------------ */
/* lesen 									*/
void fl()
{
  X=fgetc(pIF);															/* nimm erstes Zeichen 		*/
  if (X=='\n') line++,col=0;
  else col++;
}
/* lesen beenden							*/
void flb()
{
  fl();
  fb();
}
/* schreiben (Groß), lesen*/
void fgl()
{
	*pBuf=(char)toupper(X); 									/* *pBuf=(char)X&0xdf;		*/
	*(++pBuf)=0;
	fl();
}
/* schreiben, lesen				*/
void fsl()
{
	*pBuf=(char)X;														/* gehe ein Zeichen weiter*/
	*(++pBuf)=0;																			
	fl();
}
/* schreiben, lesen, been */
void fslb()
{
	fsl();fb();
}
/* beenden 								*/
void fb()
{
  switch (Z)
  {
    /* Symbol -------			*/
    case 3: // :
    case 4: // <
    case 5: // >
    case 0:Morph.Val.Symb=vBuf[0]; 		Morph.MC =mcSymb;
		break;
    /* Zahl --------- 		*/
    case 1:Morph.Val.Num=atol(vBuf); 	Morph.MC =mcNum;
		break;
		/* IstGleich ----			*/
    case 6:Morph.Val.Symb=(long)zERG;	Morph.MC =mcSymb;
    break;
		/* KleinerGleich 			*/
    case 7:Morph.Val.Symb=(long)zLE; 	Morph.MC =mcSymb;
    break;
		/* GrösserGleich 			*/
    case 8:Morph.Val.Symb=(long)zGE; 	Morph.MC =mcSymb;
    break;
    /* Buchstabe ---- 		*/
    case 2:Morph.Val.pStr=vBuf;			  Morph.MC =mcIdent;
    break;
    /* Wort --------- 		*/
    case 10: checkKeyw(); 
    break; 
    /* String --------    */
    case 9: Morph.Val.pStr=vBuf;			Morph.MC =mcStrin;
    break;
	}
  End=1;  																	/* Morphem / Token fertig */
}
/* beenden --- ohne token	*/
void bb()
{
	End = 1;
}

/* ---- Schlüsselworterkennung ---------------------------------------*/
/* Schküsselwörter */
/* Wenn checkKey aufgerufen wird, dann wird binäre Suche gestartet.
 * Wenn das Wort gefunden wird, gibt er die Stelle in dem KeywordArray
 * zurück. Dann addiert man diese mit dem Erstenzeichen in tZS. So 
 * bekommt man den Keywordcode raus. Falls es nicht gefunden wurde, ist
 * es ein Ident.
*/
const char*  Keyw[]= 
{
	"BEGIN","CALL","CONST","DO","ELSE","END","IF","ODD",
  "PROCEDURE","THEN","VAR","WHILE"
};
/* Teste Zeiket Keyword 	*/
void checkKeyw()
{
	int n=sizeof Keyw/sizeof(char*);
	int ret;
		
	ret = binary_search(Keyw,n,vBuf);
	if(ret==-1)																
	{	
		/* Ident 							*/
		Morph.Val.pStr=vBuf; 										/* Setzte Ident						*/
		Morph.MC =mcIdent;
	}
	else
	{
		/* Keyword 						*/
		Morph.Val.Symb=(long)zBGN+ret;					/* Keywordcodes						*/
    Morph.MC =mcSymb;
	}
}
/* binäre Suche 					*/
int binary_search( const char** M, int n, const char* X) 
{
	unsigned mitte;
  unsigned links = 0; 									
  unsigned rechts = n - 1; 									/* Arrays sind 0­basiert 	*/
  int ret = -1;
  int bool;
 
	do
  {
		if (n<=0) break;
    mitte = links + ((rechts - links) / 2); /* Bereich halbieren 			*/ 
    if (rechts < links) break; 							/* alles durchsucht				*/
    bool=strcmp(M[mitte],X);								/* gibt -1 0 1 zurück			*/
    if (bool==0) ret=mitte;   							 
    else 
			if (bool >0) rechts = mitte; 					/* linker Abschnitt 			*/
			else  															
				links = mitte + 1;    						
    n=(n)/2;
	}while (bool!=0);
  return ret;
}

