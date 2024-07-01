#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/avl.h"
#include "../include/compare.h"

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

void avl_insere(tnode **parv, titem item, int (*cmp)(titem, titem)) {
    if (*parv == NULL) {
        *parv = (tnode *) malloc(sizeof(tnode));
        (*parv)->item = item;
        (*parv)->esq = NULL;
        (*parv)->dir = NULL;
        (*parv)->pai = NULL;
        (*parv)->h = 0;

        listnode *new_node = (listnode *) malloc(sizeof(listnode));
        new_node->item = item;
        new_node->next = NULL;
        (*parv)->lista = new_node;
    } else if (cmp(item, (*parv)->item) == 0) {
        listnode *new_node = (listnode *) malloc(sizeof(listnode));
        new_node->item = item;
        new_node->next = (*parv)->lista;
        (*parv)->lista = new_node;
    } else if (cmp(item, (*parv)->item) < 0) {
        avl_insere(&(*parv)->esq, item, cmp);
        if ((*parv)->esq != NULL) {
            (*parv)->esq->pai = *parv;
        }
    } else {
        avl_insere(&(*parv)->dir, item, cmp);
        if ((*parv)->dir != NULL) {
            (*parv)->dir->pai = *parv;
        }
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

void avl_remove(tnode **parv, titem reg, int (*cmp)(titem, titem)) {
    if (*parv != NULL) {
        int comp = cmp(reg, (*parv)->item);
        if (comp < 0) {
            avl_remove(&((*parv)->esq), reg, cmp);
        } else if (comp > 0) {
            avl_remove(&((*parv)->dir), reg, cmp);
        } else {
            if ((*parv)->esq == NULL && (*parv)->dir == NULL) {
                listnode *curr = (*parv)->lista;
                while (curr != NULL) {
                    listnode *tmp = curr;
                    curr = curr->next;
                    free(tmp);
                }
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
                listnode *curr = aux->lista;
                while (curr != NULL) {
                    listnode *tmp = curr;
                    curr = curr->next;
                    free(tmp);
                }
                free(aux);
            } else {
                tnode **sucessor = percorre_esq(&(*parv)->dir);
                (*parv)->item = (*sucessor)->item;
                listnode *old_lista = (*parv)->lista;
                (*parv)->lista = (*sucessor)->lista;
                (*sucessor)->lista = old_lista;
                avl_remove(&(*parv)->dir, (*sucessor)->item, cmp);
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
        listnode *curr = parv->lista;
        while (curr != NULL) {
            listnode *tmp = curr;
            curr = curr->next;
            free(tmp);
        }
        free(parv);
    }
}

tnode* avl_sucessor(tnode *no) {
    if (no->dir != NULL) {
        no = no->dir;
        while (no->esq != NULL) {
            no = no->esq;
        }
        return no;
    }

    tnode *pai = no->pai;
    while (pai != NULL && no == pai->dir) {
        no = pai;
        pai = pai->pai;
    }
    return pai;
}

void consulta_latitude_maior_que(tnode *parv, double valor, int *resultado, int *tamanho) {
    if (parv != NULL) {
        if (parv->item.latitude > valor) {
            listnode *curr = parv->lista;
            while (curr != NULL) {
                resultado[(*tamanho)++] = curr->item.codigo_ibge;
                curr = curr->next;
            }
            consulta_latitude_maior_que(parv->esq, valor, resultado, tamanho);
        }
        consulta_latitude_maior_que(parv->dir, valor, resultado, tamanho);
    }
}

void consulta_longitude_intervalo(tnode *parv, double min, double max, int *resultado, int *tamanho) {
    if (parv != NULL) {
        if (parv->item.longitude > min) {
            consulta_longitude_intervalo(parv->esq, min, max, resultado, tamanho);
        }
        if (parv->item.longitude >= min && parv->item.longitude <= max) {
            listnode *curr = parv->lista;
            while (curr != NULL) {
                resultado[(*tamanho)++] = curr->item.codigo_ibge;
                curr = curr->next;
            }
        }
        if (parv->item.longitude < max) {
            consulta_longitude_intervalo(parv->dir, min, max, resultado, tamanho);
        }
    }
}

void consulta_ddd_igual(tnode *parv, int valor, int *resultado, int *tamanho) {
    if (parv != NULL) {
        consulta_ddd_igual(parv->esq, valor, resultado, tamanho);
        if (parv->item.ddd == valor) {
            listnode *curr = parv->lista;
            while (curr != NULL) {
                resultado[(*tamanho)++] = curr->item.codigo_ibge;
                curr = curr->next;
            }
        }
        consulta_ddd_igual(parv->dir, valor, resultado, tamanho);
    }
}

int* intersecao(int *a, int tamanho_a, int *b, int tamanho_b, int *tamanho_intersecao) {
    int *resultado = (int *) malloc(sizeof(int) * (tamanho_a < tamanho_b ? tamanho_a : tamanho_b));
    int i = 0, j = 0, k = 0;
    while (i < tamanho_a && j < tamanho_b) {
        if (a[i] < b[j]) {
            i++;
        } else if (a[i] > b[j]) {
            j++;
        } else {
            resultado[k++] = a[i];
            i++;
            j++;
        }
    }
    *tamanho_intersecao = k;
    return resultado;
}