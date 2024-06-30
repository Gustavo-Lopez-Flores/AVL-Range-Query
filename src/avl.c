#include <stdio.h>
#include <stdlib.h>
#include "../include/avl.h"

int max(int a, int b) {
    return a > b ? a : b;
}

int altura(tnode *arv) {
    if (arv == NULL) {
        return -1;
    } else {
        return arv->h;
    }
}

void insere_lista(listnode **lista, titem item) {
    listnode *novo = (listnode *) malloc(sizeof(listnode));
    novo->item = item;
    novo->next = *lista;
    *lista = novo;
}

void remove_lista(listnode **lista, titem item) {
    listnode *aux = *lista;
    listnode *anterior = NULL;
    while (aux != NULL && aux->item != item) {
        anterior = aux;
        aux = aux->next;
    }
    if (aux != NULL) {
        if (anterior == NULL) {
            *lista = aux->next;
        } else {
            anterior->next = aux->next;
        }
        free(aux);
    }
}

void avl_insere(tnode **parv, titem item) {
    if (*parv == NULL) {
        *parv = (tnode *) malloc(sizeof(tnode));
        (*parv)->item = item;
        (*parv)->lista = NULL;
        insere_lista(&(*parv)->lista, item);
        (*parv)->esq = NULL;
        (*parv)->dir = NULL;
        (*parv)->pai = NULL;
        (*parv)->h = 0;
    } else if (item < (*parv)->item) {
        avl_insere(&(*parv)->esq, item);
        if ((*parv)->esq != NULL) {
            (*parv)->esq->pai = *parv;
        }
    } else if (item > (*parv)->item) {
        avl_insere(&(*parv)->dir, item);
        if ((*parv)->dir != NULL) {
            (*parv)->dir->pai = *parv;
        }
    } else {
        insere_lista(&(*parv)->lista, item);
    }
    (*parv)->h = max(altura((*parv)->esq), altura((*parv)->dir)) + 1;
    _avl_rebalancear(parv);
}

void _rd(tnode **parv) {
    tnode *y = *parv;
    tnode *x = y->esq;
    tnode *B = x->dir;

    y->esq = B;
    if (B != NULL) {
        B->pai = y;
    }

    x->dir = y;
    x->pai = y->pai;
    y->pai = x;

    *parv = x;
    y->h = max(altura(y->esq), altura(y->dir)) + 1;
    x->h = max(altura(x->esq), altura(y)) + 1;
}

void _re(tnode **parv) {
    tnode *x = *parv;
    tnode *y = x->dir;
    tnode *B = y->esq;

    x->dir = B;
    if (B != NULL) {
        B->pai = x;
    }

    y->esq = x;
    y->pai = x->pai;
    x->pai = y;

    *parv = y;
    x->h = max(altura(x->esq), altura(x->dir)) + 1;
    y->h = max(altura(y->esq), altura(x)) + 1;
}

void _avl_rebalancear(tnode **parv) {
    int fb = altura((*parv)->esq) - altura((*parv)->dir);

    if (fb == -2) {
        if (altura((*parv)->dir->esq) <= altura((*parv)->dir->dir)) {
            _re(parv);
        } else {
            _rd(&(*parv)->dir);
            _re(parv);
        }
    } else if (fb == 2) {
        if (altura((*parv)->esq->esq) >= altura((*parv)->esq->dir)) {
            _rd(parv);
        } else {
            _re(&(*parv)->esq);
            _rd(parv);
        }
    }
}

tnode **percorre_esq(tnode **arv) {
    tnode *aux = *arv;
    if (aux->esq == NULL) {
        return arv;
    } else {
        while (aux->esq->esq != NULL)
            aux = aux->esq;
        return &(aux->esq);
    }
}

void avl_remove(tnode **parv, titem reg) {
    if (*parv != NULL) {
        int cmp = (*parv)->item - reg;
        if (cmp > 0) {
            avl_remove(&((*parv)->esq), reg);
        } else if (cmp < 0) {
            avl_remove(&((*parv)->dir), reg);
        } else {
            remove_lista(&(*parv)->lista, reg);
            if ((*parv)->lista == NULL) {
                if ((*parv)->esq == NULL && (*parv)->dir == NULL) {
                    free(*parv);
                    *parv = NULL;
                } else if ((*parv)->esq == NULL || (*parv)->dir == NULL) {
                    tnode *aux = *parv;
                    if ((*parv)->esq == NULL) {
                        *parv = (*parv)->dir;
                        (*parv)->pai = aux->pai;
                    } else {
                        *parv = (*parv)->esq;
                        (*parv)->pai = aux->pai;
                    }
                    free(aux);
                } else {
                    tnode **sucessor = percorre_esq(&(*parv)->dir);
                    (*parv)->item = (*sucessor)->item;
                    avl_remove(&(*parv)->dir, (*sucessor)->item);
                }
            }
        }
        if (*parv != NULL) {
            (*parv)->h = max(altura((*parv)->esq), altura((*parv)->dir)) + 1;
            _avl_rebalancear(parv);
        }
    }
}

void avl_destroi(tnode *parv) {
    if (parv != NULL) {
        avl_destroi(parv->esq);
        avl_destroi(parv->dir);
        listnode *current = parv->lista;
        while (current != NULL) {
            listnode *temp = current;
            current = current->next;
            free(temp);
        }
        free(parv);
    }
}