#ifndef __AVL__
#define __AVL__

typedef int titem;

typedef struct _listnode {
    titem item;
    struct _listnode *next;
} listnode;

typedef struct _node {
    titem item;
    struct _listnode *lista;
    struct _node *esq;
    struct _node *dir;
    struct _node *pai;
    int h;
} tnode;

void avl_insere(tnode **parv, titem reg);
void avl_remove(tnode **parv, titem reg);
void avl_destroi(tnode *parv);

void _rd(tnode **pparv);
void _re(tnode **pparv);
void _avl_rebalancear(tnode **pparv);

#endif