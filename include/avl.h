#ifndef __AVL__
#define __AVL__

typedef struct {
    int codigo_ibge;
    char *nome;
    double latitude;
    double longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char *fuso_horario;
} titem;

typedef struct _listnode {
    titem item;
    struct _listnode *next;
} listnode;

typedef struct _node {
    titem item;
    listnode *lista;
    struct _node *esq;
    struct _node *dir;
    struct _node *pai;
    int h;
} tnode;

void avl_insere(tnode **parv, titem reg, int (*cmp)(titem, titem));
void avl_remove(tnode **parv, titem reg, int (*cmp)(titem, titem));
void avl_destroi(tnode *parv);
tnode* avl_sucessor(tnode *no);

void _rd(tnode **pparv);
void _re(tnode **pparv);
void _avl_rebalancear(tnode **pparv);

void consulta_latitude_maior_que(tnode *parv, double valor, int *resultado, int *tamanho);
void consulta_longitude_intervalo(tnode *parv, double min, double max, int *resultado, int *tamanho);
void consulta_ddd_igual(tnode *parv, int valor, int *resultado, int *tamanho);

int* intersecao(int *a, int tamanho_a, int *b, int tamanho_b, int *tamanho_intersecao);

#endif