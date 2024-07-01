#include <string.h>
#include "../include/compare.h"

int compare_codigo_ibge(titem a, titem b) {
    return a.codigo_ibge - b.codigo_ibge;
}

int compare_nome(titem a, titem b) {
    return strcmp(a.nome, b.nome);
}

int compare_latitude(titem a, titem b) {
    if (a.latitude < b.latitude) return -1;
    if (a.latitude > b.latitude) return 1;
    return 0;
}

int compare_longitude(titem a, titem b) {
    if (a.longitude < b.longitude) return -1;
    if (a.longitude > b.longitude) return 1;
    return 0;
}

int compare_capital(titem a, titem b) {
    return a.capital - b.capital;
}

int compare_codigo_uf(titem a, titem b) {
    return a.codigo_uf - b.codigo_uf;
}

int compare_siafi_id(titem a, titem b) {
    return a.siafi_id - b.siafi_id;
}

int compare_ddd(titem a, titem b) {
    return a.ddd - b.ddd;
}

int compare_fuso_horario(titem a, titem b) {
    return strcmp(a.fuso_horario, b.fuso_horario);
}