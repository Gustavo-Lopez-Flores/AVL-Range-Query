#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/avl.h"
#include "../include/compare.h"
#include "../include/hash.h"
#include "../cJSON/cJSON.h"

char *get_key_codigo_ibge(void *registro) {
    titem *item = (titem *)registro;
    char *key = (char *)malloc(12);
    sprintf(key, "%d", item->codigo_ibge);
    return key;
}

void load_json_file(const char *filename, cJSON **json) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';

    *json = cJSON_Parse(data);
    free(data);
    fclose(file);
}

void build_avl_from_json(tnode **root, cJSON *json, int (*cmp)(titem, titem)) {
    cJSON *municipio;
    cJSON_ArrayForEach(municipio, json) {
        titem *item = (titem *)malloc(sizeof(titem));
        item->codigo_ibge = cJSON_GetObjectItem(municipio, "codigo_ibge")->valueint;
        item->nome = cJSON_GetObjectItem(municipio, "nome")->valuestring;
        item->latitude = cJSON_GetObjectItem(municipio, "latitude")->valuedouble;
        item->longitude = cJSON_GetObjectItem(municipio, "longitude")->valuedouble;
        item->codigo_uf = cJSON_GetObjectItem(municipio, "codigo_uf")->valueint;
        item->ddd = cJSON_GetObjectItem(municipio, "ddd")->valueint;

        avl_insere(root, *item, cmp);
    }
}

void build_hash_from_json(thash *hash, cJSON *json) {
    cJSON *municipio;
    cJSON_ArrayForEach(municipio, json) {
        titem *item = (titem *)malloc(sizeof(titem));
        item->codigo_ibge = cJSON_GetObjectItem(municipio, "codigo_ibge")->valueint;
        item->nome = cJSON_GetObjectItem(municipio, "nome")->valuestring;
        item->latitude = cJSON_GetObjectItem(municipio, "latitude")->valuedouble;
        item->longitude = cJSON_GetObjectItem(municipio, "longitude")->valuedouble;
        item->codigo_uf = cJSON_GetObjectItem(municipio, "codigo_uf")->valueint;
        item->ddd = cJSON_GetObjectItem(municipio, "ddd")->valueint;

        hash_insere(hash, item);
    }
}

void print_resultado(thash hash, int *codigos_ibge, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        char key[12];
        sprintf(key, "%d", codigos_ibge[i]);
        titem *item = (titem *)hash_busca(hash, key);
        if (item != NULL) {
            printf("Codigo IBGE: %d\n", item->codigo_ibge);
            printf("Nome: %s\n", item->nome);
            printf("Latitude: %lf\n", item->latitude);
            printf("Longitude: %lf\n", item->longitude);
            printf("Codigo UF: %d\n", item->codigo_uf);
            printf("DDD: %d\n", item->ddd);
            printf("\n");
        }
    }
}

int main(void) {
    cJSON *json;
    load_json_file("municipios.json", &json);

    tnode *avl_nome = NULL;
    tnode *avl_latitude = NULL;
    tnode *avl_longitude = NULL;
    tnode *avl_codigo_uf = NULL;
    tnode *avl_ddd = NULL;

    build_avl_from_json(&avl_nome, json, compare_nome);
    build_avl_from_json(&avl_latitude, json, compare_latitude);
    build_avl_from_json(&avl_longitude, json, compare_longitude);
    build_avl_from_json(&avl_codigo_uf, json, compare_codigo_uf);
    build_avl_from_json(&avl_ddd, json, compare_ddd);

    thash hash;
    hash_constroi(&hash, 1000, get_key_codigo_ibge);
    build_hash_from_json(&hash, json);

    int resultado_latitude[1000], tamanho_latitude = 0;
    int resultado_longitude[1000], tamanho_longitude = 0;
    int resultado_ddd[1000], tamanho_ddd = 0;

    consulta_latitude_maior_que(avl_latitude, 50.0, resultado_latitude, &tamanho_latitude);
    consulta_longitude_intervalo(avl_longitude, 20.0, 30.0, resultado_longitude, &tamanho_longitude);
    consulta_ddd_igual(avl_ddd, 67, resultado_ddd, &tamanho_ddd);

    int resultado_interseccao[1000], tamanho_interseccao = 0;
    for (int i = 0; i < tamanho_latitude; i++) {
        for (int j = 0; j < tamanho_longitude; j++) {
            if (resultado_latitude[i] == resultado_longitude[j]) {
                for (int k = 0; k < tamanho_ddd; k++) {
                    if (resultado_latitude[i] == resultado_ddd[k]) {
                        resultado_interseccao[tamanho_interseccao++] = resultado_latitude[i];
                    }
                }
            }
        }
    }

    printf("Resultados das interseções:\n");
    print_resultado(hash, resultado_interseccao, tamanho_interseccao);

    cJSON_Delete(json);
    avl_destroi(avl_nome);
    avl_destroi(avl_latitude);
    avl_destroi(avl_longitude);
    avl_destroi(avl_codigo_uf);
    avl_destroi(avl_ddd);
    hash_apaga(&hash);

    return 0;
}