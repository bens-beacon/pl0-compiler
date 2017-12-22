/* LISTE
 * Headerfile
 * Code by Ben
 *
*/

#include <stdlib.h>
#include <stdio.h>

/* Aufbaue der Liste			*/
typedef struct tcon																							
{																												
	struct tcon *prev;																				
	struct tcon *next;																					
	void 				*data;																					
}tcon;																										
																												
typedef struct tlist 																											
{																												
			 tcon *first;																					
			 tcon *last;																					
			 tcon *curr;
			 int  length;																					
}tlist;																										

/* FunktionsPrototyp			*/
tlist *createlist(void);
int 	 insertend(tlist *manlist, void *manitem);	
tlist *removeitem(tlist *manlist);		
tlist *removelast(tlist *manlist);	
void 	*getfirst(tlist *manlist);
void 	*getlast(tlist *manlist);	
void 	*getnext(tlist *manlist);		
void 	*getprev(tlist *manlist);		
void 	*getcurr(tlist *manlist);		
