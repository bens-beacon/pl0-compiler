/* Semantikroutine
 * C-Code
 * Code by Ben
 * 
*/

#include "smr.h"

/* Globales														*/
int compid;																	/* Globale Var Condition	*/
extern int LenCode;													/* Codelänge							*/
extern tProc* AktProc;
extern tMorph Morph;												/* Aktuelles Morphem			*/
extern tlist* Constblock;										/* Konstanteliste					*/
extern tlist* Labellist;										/* Labelkeller						*/
tBez* BezConstGlobal;												/* Globaler Bez für Const	*/

/* Ist Konstantenname schon vorhanden */
int bl1()
{
	printf(ANSI_COLOR_CYAN " >> bl1! \n");

	tBez* Bez_tmp = searchBEZ(AktProc, Morph.Val.pStr);
	if(Bez_tmp != NULL)												/* Bez schon vorhanden ?	*/
	{
		printf(ANSI_COLOR_RED " >> Constname \"%s\" is already defined!\n",Morph.Val.pStr);
		exit(EXIT_FAILURE);
	}
	tBez* BezConst = createBezConst(Morph.Val.pStr);/* Erstelle ConstBez*/
	BezConstGlobal = BezConst;
	return OK;
}

/* Bezeichner mit Konstante anlegen	  */
int bl2()
{
	printf(ANSI_COLOR_CYAN " >> bl2! \n");

	tConst* Const_tmp = searchConst(Morph.Val.Num);		
	if(Const_tmp == NULL)											/* Konstenwert vorhanden?	*/
	{																					/* Nicht Vorhanden, erste */
		tBez* BezConst = addValueBezConst(AktProc, BezConstGlobal, Morph.Val.Num);
		insertend(AktProc->pLBez,BezConst);
	}
	else 																			/* Vorhanden, Index übern */
	{
		tBez* BezConst = addValueBezConst(AktProc, BezConstGlobal, Const_tmp->Val);
		insertend(AktProc->pLBez,BezConst);
	}
	return OK;
}

/* Bezeichner mit Variable anlegen 	  */
int bl3()
{
	printf(ANSI_COLOR_CYAN " >> bl3! \n");

	tBez* Bez_tmp = searchBEZ(AktProc, Morph.Val.pStr);
	if(Bez_tmp != NULL)												/* Bez schon vorhanden ?	*/
	{
		printf(ANSI_COLOR_RED " >> Varname \"%s\"  is already defined!\n", Morph.Val.pStr);	
		exit(EXIT_FAILURE);
	}
	else
	{
			tBez* BezVar = createBezVar(AktProc, Morph.Val.pStr);
			insertend(AktProc->pLBez,BezVar);
	}
	return OK;
}

/* Bezeichner mit neuen Procedur 		  */
int bl4()
{
	tBez* Bez_tmp = searchBEZ(AktProc, Morph.Val.pStr);
	if(Bez_tmp == NULL)
	{
		printf(ANSI_COLOR_RED " >> Procedur \"%s\" is already available!\n",Morph.Val.pStr);	
		exit(EXIT_FAILURE);
	}
	else
	{
			tBez * BezProc = createBezProc(AktProc, Morph.Val.pStr);
			insertend(AktProc->pLBez,BezProc);
	}	
	return OK;
}

/* lösche Prozedur 										*/
int bl5()
{
	printf(ANSI_COLOR_CYAN " >> bl5!\n");	

	code(retProc);
	tProc* Proc_tmp = AktProc;
	if( Proc_tmp->pParent != 0)
	{
		AktProc = Proc_tmp->pParent;						/* ElternProzedur wird akt*/
	}	

	clear(Proc_tmp);
	CodeOut();																/* Schreibe i Ausgabedatei*/
}

/* Beginn des Anwesiungsteils					*/
int bl6()
{
	printf(ANSI_COLOR_CYAN " >> bl6!\n");	

	tProc* Proc_tmp = AktProc;
	code(entryProc,0,Proc_tmp->IdxProc,Proc_tmp->SpzzVar); /* entryproc	*/
	return OK;
}

/* Abschlussarbeiten									*/
int pr1()
{
	printf(ANSI_COLOR_CYAN " >> pr1!\n");

	/* Konstanten block hate extra Funktion im Codegen */
	return OK;									
}

/* Wertzuweisung 	LS		PushAdrVar...	*/
int st1()
{
	printf(ANSI_COLOR_CYAN " >> st1!\n");

	tBez * Bez_tmp = globalsearchBEZ(AktProc,Morph.Val.pStr);
	if( Bez_tmp == NULL)
	{
		printf(ANSI_COLOR_RED " >> Variable \"%s\" is not defined!\n",Morph.Val.pStr);	
		exit(EXIT_FAILURE);			
	}
	else
	{
		if(Bez_tmp->Kz == KzVar)
		{		
			tVar* Var_tmp = Bez_tmp->pObj;
			if(Bez_tmp->IdxProc==0) code(puAdrVrMain,Var_tmp->Dspl);	/* Mai*/
			else if(AktProc->IdxProc == Bez_tmp->IdxProc) 
															code(puAdrVrLocl,Var_tmp->Dspl);	/* Loc*/
			else {code(puValVrGlob,Var_tmp->Dspl,AktProc->IdxProc);	}	/* Glo*/																		
		}
		else if(Bez_tmp->Kz == KzConst)
		{
			printf(ANSI_COLOR_RED " >> \"%s\" is a Const not a Variable!\n",Bez_tmp->pName);	
			exit(EXIT_FAILURE);					
		}
		else if(Bez_tmp->Kz == KzProc)
		{
			printf(ANSI_COLOR_RED " >> \"%s\" is a Proc not a Variable!\n",Morph.Val.pStr);	
			exit(EXIT_FAILURE);				
		}		
	} 	
	return OK;
}

/* Wertzuweisung RS										*/
int st2()
{
	printf(ANSI_COLOR_CYAN " >> st2!\n");

	code(storeVal);														/* StoreValue							*/
	return OK;
}

/* Rufe Funktion auf */
int st8()
{
	printf(ANSI_COLOR_CYAN " >> st8!\n");

	tBez * Bez_tmp = globalsearchBEZ(AktProc,Morph.Val.pStr);
	if( Bez_tmp == NULL)
	{
		printf(ANSI_COLOR_RED " >> Procedure \"%s\" is not defined!\n",Morph.Val.pStr);	
		exit(EXIT_FAILURE);			
	}
	else
	{
		if(Bez_tmp->Kz == KzVar)
		{			
			printf(ANSI_COLOR_RED " >> \"%s\" is a Var not a Proc!\n",Bez_tmp->pName);	
			exit(EXIT_FAILURE);																		
		}
		else if(Bez_tmp->Kz == KzConst)
		{
			printf(ANSI_COLOR_RED " >> \"%s\" is a Const not a Proc!\n",Bez_tmp->pName);	
			exit(EXIT_FAILURE);					
		}
		else if(Bez_tmp->Kz == KzProc)
		{
			code(call,Bez_tmp->IdxProc);			
		}		
	} 	
	return OK;
}

/* Eingabe	 													*/
int st9()
{
	tBez * Bez_tmp = globalsearchBEZ(AktProc,Morph.Val.pStr);
	if( Bez_tmp == NULL)
	{
		printf(ANSI_COLOR_RED " >> Variable \"%s\" is not defined!\n",Morph.Val.pStr);	
		exit(EXIT_FAILURE);			
	}
	else
	{
		if(Bez_tmp->Kz == KzVar)
		{		
			tVar* Var_tmp = Bez_tmp->pObj;
			if(Bez_tmp->IdxProc==0) code(puAdrVrMain,Var_tmp->Dspl);	/* Mai*/
			else if(AktProc->IdxProc == Bez_tmp->IdxProc) 
															code(puAdrVrLocl,Var_tmp->Dspl);	/* Loc*/
			else {code(puAdrVrGlob,Var_tmp->Dspl,AktProc->IdxProc);	}	/* Glo*/			
			code(getVal);																					/* GETVAL */		
		}
		else if(Bez_tmp->Kz == KzConst)
		{
			printf(ANSI_COLOR_RED " >> \"%s\" is a Const not a Variable!\n",Bez_tmp->pName);	
			exit(EXIT_FAILURE);					
		}
		else if(Bez_tmp->Kz == KzProc)
		{
			printf(ANSI_COLOR_RED " >> \"%s\" is a Proc not a Variable!\n",Morph.Val.pStr);
			exit(EXIT_FAILURE);				
		}		
	}
	return OK;
	
}

/* Ausgabe 														*/
int st10()
{
	code(putVal);															/* PUTVAL									*/									
	return OK;
}

/* Vorzeichen Minus										*/
int ex1()
{
	code(vzMinus);														/* vzMinus								*/
	return OK;
}

/* Addieren														*/
int ex2()
{
	code(OpAdd);															/* opAdd									*/
	return OK;
}

/* Subtrahieren												*/
int ex3()
{
	code(OpSub);															/* opSub									*/
	return OK;
}

/* Mulitplizieren											*/
int te1()
{
	code(OpMult);															/* opMud									*/
	return OK;
}

/* Dividieren													*/
int te2()
{
	code(OpDiv);															/* opDiv									*/
	return OK;
}

/* Faktor Konstante										*/
int fa1()
{
	tConst* Const_tmp = searchConst(Morph.Val.Num);		
	if(Const_tmp == NULL)											/* Konstenwert vorhanden?	*/
	{																					/* Nicht Vorhanden, erste */
		// Konstante anlegen
		tConst *newConst 	= malloc(sizeof(tConst)); 
		newConst->Kz			= KzConst;
		newConst->Idx			= Constblock->length;	/* Index ist akt. Länge		*/
		newConst->Val			= Morph.Val.Num;
		insertend(Constblock,newConst);					/* Füge an Konstantenblock*/
		code(puConst,newConst->Idx);						/* Konstante anlegen 			*/
	}
	else 																			/* Vorhanden, Index übern */
	{
		code(puConst,Const_tmp->Idx);						/* Konstante anlegen 			*/
	}
	return OK;
}

/* Faktor anlegen											*/
int fa2()
{
	printf(ANSI_COLOR_CYAN " >> fa2!\n");

	tBez * Bez_tmp = globalsearchBEZ(AktProc,Morph.Val.pStr);
	if( Bez_tmp == NULL)
	{
		printf(ANSI_COLOR_RED " >> Variable \"%s\" is not defined!\n",Morph.Val.pStr);	
		exit(EXIT_FAILURE);			
	}
	else
	{
		if(Bez_tmp->Kz == KzVar)
		{		
			tVar* Var_tmp = Bez_tmp->pObj;
			if(Bez_tmp->IdxProc==0) code(puValVrMain,Var_tmp->Dspl);	/* Mai*/
			else if(AktProc->IdxProc == Bez_tmp->IdxProc) 
															code(puValVrLocl,Var_tmp->Dspl);	/* Loc*/
			else {code(puValVrGlob,Var_tmp->Dspl,AktProc->IdxProc);	}	/* Glo*/				
		}
		else if(Bez_tmp->Kz == KzConst)
		{
			tConst* Const_tmp = Bez_tmp->pObj;
			code(puConst,Const_tmp->Idx);					/* Konstante anlegen 			*/
		}
		else if(Bez_tmp->Kz == KzProc)
		{
			printf(ANSI_COLOR_RED " >> \"%s\" is a Proc not a Variable!\n",Morph.Val.pStr);	
			exit(EXIT_FAILURE);				
		}		
	} 
	return OK;
}

/* Condition 													*/
int co1()
{
	code(odd);																/* odd										*/
	return OK;
}

/* = 																	*/
int co2()
{
	compid = cmpEQ;														/* cmpEQ									*/
	return OK;
}

/* # 																	*/
int co3()
{
	compid = cmpNE;														/* cmpNE									*/
	return OK;
}

/* < 																	*/
int co4()
{
	compid = cmpLT;														/* cmpLT									*/
	return OK;
}

/* <= 																*/
int co5()
{
	compid = cmpLE;														/* cmpLE									*/
	return OK;
}

/* >																	*/
int co6()
{
	compid = cmpGT;														/* cmpGT									*/
	return OK;
}

/* >= 																	*/
int co7()
{
	compid = cmpGE;														/* cmpGE									*/
	return OK;
}

/* Codegenerierung											*/
int co8()
{
	code(compid);
	return OK;
}