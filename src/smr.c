/* Semantikroutine
 * C-Code
 * Code by Ben
 * 
*/

#include "smr.h"

/* Globales														*/
char* pName;
long Val;
int compid;																	/* Globale Var Condition	*/
extern int LenCode;													/* Codelänge							*/

/* Ist Konstantenname schon vorhanden */
int bl1()
{
	tBez* Bez_tmp = searchBEZ(AktProc, pName);
	if(Bez_tmp == NULL)												/* Bez schon vorhanden ?	*/
	{
		printf(ANSI_COLOR_RED " >> Constname is already available!\n");
		exit(EXIT_FAILURE);
	}
	return OK;
}

/* Bezeichner mit Konstante anlegen	  */
int bl2()
{
	tConst* Const_tmp = searchConst( Val);		
	if(Const_tmp == NULL)											/* Konstenwert vorhanden?	*/
	{																					/* Nicht Vorhanden, erste */
		tBez* BezConst = createBezConst(AktProc,Val, pName);
		insertend(AktProc->pLBez,BezConst);
	}
	else 																			/* Vorhanden, Index übern */
	{
		tBez* BezConst = createBezConstIdx(AktProc,Val,pName,Const_tmp->Idx);
		insertend(AktProc->pLBez,BezConst);
	}
	return OK;
}

/* Bezeichner mit Variable anlegen 	  */
int bl3()
{
	tBez* Bez_tmp = searchBEZ(AktProc, pName);
	if(Bez_tmp == NULL)												/* Bez schon vorhanden ?	*/
	{
		printf(ANSI_COLOR_RED " >> Varname is already available!\n");	
		exit(EXIT_FAILURE);
	}
	else
	{
			tBez * BezVar = createBezVar(AktProc, pName);
			insertend(AktProc->pLBez,BezVar);
	}
	return OK;
}

/* Bezeichner mit neuen Procedur 		  */
int bl4()
{
	tBez* Bez_tmp = searchBEZ(AktProc, pName);
	if(Bez_tmp == NULL)
	{
		printf(ANSI_COLOR_RED " >> Procedur is already available!\n");	
		exit(EXIT_FAILURE);
	}
	else
	{
			tBez * BezProc = createBezProc(AktProc, pName);
			insertend(AktProc->pLBez,BezProc);
	}	
	return OK;
}

/* lösche Prozedur 										*/
int bl5()
{
	tProc* Proc_tmp = AktProc;
	AktProc = Proc_tmp->pParent;							/* ElternProzedur wird akt*/
	clear(Proc_tmp);
	CodeOut();																/* Schreibe i Ausgabedatei*/
}

/* Beginn des Anwesiungsteils					*/
int bl6()
{
	printf(ANSI_COLOR_CYAN " >> bl6!\n");	
	tProc* Proc_tmp = AktProc;
	code(20,0,Proc_tmp->IdxProc,Proc_tmp->SpzzVar); /* 20 = entryProc		*/
	return OK;
}


/* Abschlussarbeiten									*/
int pr1()
{
	CodeOut();
	printf(ANSI_COLOR_CYAN " >> pr1!\n");	
	//bl5();																		/* Löschen								*/

	/* Schreibe Constantenblock 				*/
	if(Constblock->first != NULL)
	{	
		tConst* Const_tmp = Constblock->first->data;
		for (;Constblock->curr->next!=NULL; Const_tmp = Constblock->curr->next->data)
		{
			wr2ToCode(Const_tmp->Val);
		}
		/* Codelänge nachtragen */
		//wr2ToCodeAtP(short x,char*pD)
	}
	return OK;									
}

/* Wertzuweisung 	LS		PushAdrVar...	*/
int st1()
{
	tBez * Bez_tmp = globalsearchBEZ(AktProc,pName);
	if( Bez_tmp == NULL)
	{
		printf(ANSI_COLOR_RED " >> Variable is not available!\n");	
		exit(EXIT_FAILURE);			
	}
	else
	{
		if(Bez_tmp->Kz == KzVar)
		{		
			tVar* Var_tmp = Bez_tmp->pObj;
			if(AktProc->IdxProc==0) code(4,Var_tmp->Dspl);												/* Main 	*/
			else if(AktProc->IdxProc > 0) code(2,Var_tmp->Dspl,AktProc->IdxProc);	/* Global */
			else {code(3,Var_tmp->Dspl);}																					/* Local	*/			
		}
		else if(Bez_tmp->Kz == KzConst)
		{
			printf(ANSI_COLOR_RED " >> Its a Const not a Variable!\n");	
			exit(EXIT_FAILURE);					
		}
		else if(Bez_tmp->Kz == KzProc)
		{
			printf(ANSI_COLOR_RED " >> Its a Proc not a Variable!\n");	
			exit(EXIT_FAILURE);				
		}		
	} 	
	return OK;
}

/* Wertzuweisung RS										*/
int st2()
{
	code(7);																	/* StoreValue							*/
	return OK;
}

/* Eingabe	 													*/
int st9()
{
	tBez * Bez_tmp = globalsearchBEZ(AktProc,pName);
	if( Bez_tmp == NULL)
	{
		printf(ANSI_COLOR_RED " >> Variable is not available!\n");	
		exit(EXIT_FAILURE);			
	}
	else
	{
		if(Bez_tmp->Kz == KzVar)
		{
			
			tVar* Var_tmp = Bez_tmp->pObj;
			if(AktProc->IdxProc==0) code(4,Var_tmp->Dspl);											/* Main 	*/
			else if(AktProc->IdxProc>0) code(2,Var_tmp->Dspl,AktProc->IdxProc);	/* Global */
			else {code(3,Var_tmp->Dspl);}																				/* Local	*/	
			code(9);																														/* GETVAL */		
		}
		else if(Bez_tmp->Kz == KzConst)
		{
			printf(ANSI_COLOR_RED " >> Its a Const not a Variable!\n");	
			exit(EXIT_FAILURE);					
		}
		else if(Bez_tmp->Kz == KzProc)
		{
			printf(ANSI_COLOR_RED " >> Its a Proc not a Variable!\n");	
			exit(EXIT_FAILURE);				
		}		
	}
	return OK;
	
}

/* Ausgabe 														*/
int st10()
{
	code(8);																	/* PUTVAL									*/									
	return OK;
}

/* Vorzeichen Minus										*/
int ex1()
{
	code(10);																	/* vzMinus								*/
	return OK;
}

/* Addieren														*/
int ex2()
{
	code(12);																	/* opAdd									*/
	return OK;
}

/* Subtrahieren												*/
int ex3()
{
	code(13);																	/* opSub									*/
	return OK;
}

/* Mulitplizieren											*/
int te1()
{
	code(14);																	/* opMud									*/
	return OK;
}

/* Dividieren													*/
int te2()
{
	code(15);																	/* opDiv									*/
	return OK;
}

/* Faktor Konstante										*/
int fa1()
{
	tConst* Const_tmp = searchConst(Val);		
	if(Const_tmp == NULL)											/* Konstenwert vorhanden?	*/
	{																					/* Nicht Vorhanden, erste */
		tBez* BezConst = createBezConst(AktProc,Val, pName);
		insertend(AktProc->pLBez,BezConst);
		Const_tmp = BezConst->pObj;
		code(6,Const_tmp->Idx);									/* Konstante anlegen 			*/
	}
	else 																			/* Vorhanden, Index übern */
	{

	}
	return OK;
}

/* Faktor anlegen											*/
int fa2()
{
	tBez * Bez_tmp = globalsearchBEZ(AktProc,pName);
	if( Bez_tmp == NULL)
	{
		printf(ANSI_COLOR_RED " >> Variable is not available!\n");	
		exit(EXIT_FAILURE);			
	}
	else
	{
		if(Bez_tmp->Kz == KzVar)
		{
			
			tVar* Var_tmp = Bez_tmp->pObj;
			if(AktProc->IdxProc==0) code(4,Var_tmp->Dspl);											/* Main 	*/
			else if(AktProc->IdxProc>0) code(2,Var_tmp->Dspl,AktProc->IdxProc);	/* Global */
			else {code(3,Var_tmp->Dspl);}																				/* Local	*/			
		}
		else if(Bez_tmp->Kz == KzConst)
		{
			tConst* Const_tmp = Bez_tmp->pObj;
			code(6,Const_tmp->Idx);								/* Konstante anlegen 			*/
		}
		else if(Bez_tmp->Kz == KzProc)
		{
			printf(ANSI_COLOR_RED " >> Its a Proc not a Variable!\n");	
			exit(EXIT_FAILURE);				
		}		
	} 
	return OK;
}

/* Condition 													*/
int co1()
{
	code(11);																	/* odd										*/
	return OK;
}

/* = 																	*/
int co2()
{
	compid = 16;															/* cmpEQ									*/
	return OK;
}

/* # 																	*/
int co3()
{
	compid = 17;															/* cmpNE									*/
	return OK;
}

/* < 																	*/
int co4()
{
	compid = 18;															/* cmpLT									*/
	return OK;
}

/* <= 																*/
int co5()
{
	compid = 20;															/* cmpLE									*/
	return OK;
}

/* >																	*/
int co6()
{
	compid = 19;															/* cmpGT									*/
	return OK;
}

/* >= 																	*/
int co7()
{
	compid = 21;															/* cmpGE									*/
	return OK;
}

/* Codegenerierung											*/
int co8()
{
	code(compid);
	return OK;
}