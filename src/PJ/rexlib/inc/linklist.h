/*****************************************************************************
 *
 * linklist.h - defines, typedefs, and prototypes for linklist.c.
 *
 * MAINTENANCE
 *	08/22/90	(Ian)
 *				Updated prototypes to match ANSI/C++ changes in LINKLIST.C.
 *				The old protos are still in, surrounded with a #ifdef, so
 *				that I can refer to them if something breaks for Jim.
 *  08/22/90	(Jim)
 *				Put in a few new protos: reverse_slist, name_in_list, new_name.
 ****************************************************************************/


#ifndef LINKLIST_H
#define LINKLIST_H

#ifndef STDTYPES_H
#include "stdtypes.h"
#endif


#define DLL_SAFETY /* if this defined doubly linked calls will set node->next
					* to NULL when removed. also the call safe_rem_node() will
					* be compiled in and some calls will check for NULL */

/* Slnode -- single linked list "NODE" */

typedef struct slnode {
	struct slnode *next;
} Slnode;


/* Names -- just a linked list of strings */

typedef struct names {
	struct names *next;
	char *name;
} Names;
Names *array_to_names(char **array, int size);


/* Dlnode -- double linked list node */

typedef struct dlnode {
	struct dlnode *next;   /* points to next node */
	struct dlnode *prev;   /* points to previous node */
} Dlnode;

/* Dlheader -- double linked list header */

typedef struct dl_header {
	Dlnode *head;
	Dlnode *tail;        /* initialized to 0 */
	Dlnode *tails_prev;
} Dlheader;

/* macro for static declaration of pre-initialized list headers
 *
 * ie: declared as,
 *
 * Dlheader name = DLHEADER_INIT(name);
 *
 */

#define DLHEADER_INIT(name) \
 {(Dlnode *)(&((name).tail)),NULL,(Dlnode *)(&((name).head))}

#define is_head(n) ((n)->prev->prev == NULL)
#define is_tail(n) ((n)->next->next == NULL)

/* function prototypes */

void 	*reverse_slist(void *l);

Names *name_in_list(char *name, Names *list);
Names *text_in_list(char *name, Names *list);
Errcode new_name(Names **pname, char *s, Names **plist);

void 	safe_rem_node(Dlnode *node);
LONG 	listlen(Dlheader *list);
Dlheader *find_header(Dlnode *node);
Boolean isin_list(Dlnode *testnode,Dlheader *list);
void 	insert_compare(Dlnode *node, Dlheader *list, FUNC compare);
void 	isort_list(Dlheader *list, FUNC compare);
void 	sort_indarray(void **array, LONG count, FUNC cmp,...);
void 	*sort_slist(Slnode *list, FUNC cmp);
Names 	*sort_names(Names *list);

void swap_dl_list(Dlheader *a, Dlheader *b);
Errcode clone_dl_list(Dlheader *source, Dlheader *dest, int node_size);

#endif /* LINKLIST_H */

