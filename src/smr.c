/* Semantikroutine
 * C-Code
 * Code by Ben
 * 
*/

#include "smr.h"

/* Aktuelles								*/
char* pName;
long Val;

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
}

/* Beginn des Anwesiungsteils					*/
int bl6()
{
	tProc* Proc_tmp = AktProc;
	code(20,0,Proc_tmp->IdxProc,Proc_tmp->SpzzVar);
	return OK;
}


/* Abschlussarbeiten									*/
int pr1()
{
	bl5();																		/* Lösche Aktuelle Proze	*/

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
			if(AktProc->IdxProc==0) code(04,Var_tmp->Dspl);				/* Main 	*/
			else if(AktProc->IdxProc==0) code(04,Var_tmp->Dspl);							// <--- dieser fall ist nicht einleuchtend
			else {code(03,Var_tmp->Dspl);}												/* Local	*/			
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
		code(07);																/* StoreValue							*/
		return OK;
}












