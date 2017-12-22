/* LISTE
 * C-Code
 * Code by Ben
 *
*/

#include "list.h"

/* ---- functions for list------------------------------------------- */
/* create a list					*/
tlist *createlist(void)																					
{																												
	tlist *manlist=malloc(sizeof(tlist));															
	if(manlist)																							
	{																											
		manlist->first=manlist->last=manlist->curr=NULL;										
		return manlist;																					
	}																											
	else printf("createlist -> malloc error -> no memory");									
}																												

/* insertend 							*/
int insertend(tlist *manlist, void *manitem)														
{																												
	tcon *mantmp=malloc(sizeof(tcon));																
	if(mantmp)																								
	{																										
		mantmp->data=manitem;																		
		mantmp->next=NULL;							
		if(manlist->last)	
		{
			manlist->last->next=mantmp;
			mantmp->prev=manlist->last;																
		}	
		else	manlist->first=mantmp;																	
		manlist->last=mantmp;																			
		manlist->curr=mantmp;																			
	}																											
	else printf("insertend -> malloc error -> no memory");	
	manlist->length++;									
	return (int)(long)mantmp;																			
}																												

/* removeitem 	 					*/
tlist* removeitem(tlist *manlist)																		
{																												
	if(manlist->curr==manlist->last)			// if remove last item														
	{																											
		manlist->curr=manlist->last->prev;		
		manlist->curr->next=NULL;
		free(manlist->last);
		manlist->last=manlist->curr;
	}																											
	else if(manlist->curr==manlist->first)// if remove first item
	{
		manlist->curr=manlist->first->next;
		manlist->curr->prev=NULL;
		free(manlist->first);
		manlist->first=manlist->curr;
	}
	else 																	// if remove current/middle item
	{
		manlist->curr=manlist->curr->prev;		
		free(manlist->curr->next);
		manlist->curr->next=manlist->curr->next->next;
		manlist->curr->next->prev=manlist->curr->prev;	
	}								
	return manlist;																										
}																																															

/* removelast 	 					*/
tlist* removelast(tlist *manlist)																		
{		
	if(manlist->last->prev == NULL)
	{
		manlist->last=NULL;
		free(manlist->last);
	}
	else
	{																						
		manlist->curr=manlist->last->prev;		
		manlist->curr->next=NULL;
		free(manlist->last);
		manlist->last=manlist->curr;			
	}
	return manlist;																										
}		

/* getfirst								*/
void *getfirst(tlist *manlist)																		
{																												
	tcon *mantmp;																							
	if(manlist->first)																					
	{																											
		mantmp=manlist->first;																			
		manlist->curr=mantmp;																			
		return mantmp->data;																				
	}																											
	else printf("getfirst -> (manlist->first) -> no content");								
	return NULL;																							
}																												

/* getlast								*/
void *getlast(tlist *manlist)																		
{																												
	tcon *mantmp;																							
	if(manlist->last)																					
	{																											
		mantmp=manlist->last;																			
		manlist->curr=mantmp;																			
		return mantmp->data;																				
	}																											
	else printf("getlast -> (manlist->last) -> no content");								
	return NULL;																							
}																												

/* getnext								*/
void *getnext(tlist *manlist)																			
{																												
	if(manlist->curr)																						
	{																											
		manlist->curr=manlist->curr->next;															
		if(manlist->curr) return manlist->curr->data;											
	}																											
	else printf("getnext -> (manlist->next) -> no content");									
	return NULL;																							
}																												

/* getprev								*/
void *getprev(tlist *manlist)																			
{																												
	if(manlist->curr)																						
	{																											
		manlist->curr=manlist->curr->prev;															
		if(manlist->curr) return manlist->curr->data;											
	}																											
	else printf("getprev -> (manlist->prev) -> no content");									
	return NULL;																							
}																												

/* getcurr								*/
void *getcurr(tlist *manlist)																			
{																												
	if(manlist->curr)																						
	{																																										
		if(manlist->curr) return manlist->curr->data;											
	}																											
	else printf("getcurr -> (manlist->curr) -> no content");									
	return NULL;																							
}	
