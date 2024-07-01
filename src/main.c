#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/avl.h"
#include "../include/compare.h"
#include "../include/hash.h"
#include "../cJSON/cJSON.h"

void build_avl_from_json(tnode **root, cJSON *json, int (*cmp)(titem, titem));

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
        titem item;
        item.codigo_ibge = cJSON_GetObjectItem(municipio, "codigo_ibge")->valueint;
        item.nome = strdup(cJSON_GetObjectItem(municipio, "nome")->valuestring);
        item.latitude = cJSON_GetObjectItem(municipio, "latitude")->valuedouble;
        item.longitude = cJSON_GetObjectItem(municipio, "longitude")->valuedouble;
        item.codigo_uf = cJSON_GetObjectItem(municipio, "codigo_uf")->valueint;
        item.ddd = cJSON_GetObjectItem(municipio, "ddd")->valueint;

        avl_insere(root, item, cmp);
    }
}

void print_resultado(thash hash, int *codigos_ibge, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        char key[12];
        sprintf(key, "%d", codigos_ibge[i]);
        titem item;
        item.codigo_ibge = codigos_ibge[i];
        
        titem *resultado = (titem *) hash_busca(hash, key);
        
        if (resultado != NULL) {
            printf("Codigo IBGE: %d\n", resultado->codigo_ibge);
            printf("Nome: %s\n", resultado->nome);
            printf("Latitude: %lf\n", resultado->latitude);
            printf("Longitude: %lf\n", resultado->longitude);
            printf("Codigo UF: %d\n", resultado->codigo_uf);
            printf("DDD: %d\n", resultado->ddd);
            printf("\n");
        }
    }
}

int main(void) {
    cJSON *json;
    load_json_file("municipios.json", &json);

    tnode *avl_latitude = NULL;
    tnode *avl_longitude = NULL;
    tnode *avl_ddd = NULL;

    build_avl_from_json(&avl_latitude, json, compare_latitude);
    build_avl_from_json(&avl_longitude, json, compare_longitude);
    build_avl_from_json(&avl_ddd, json, compare_ddd);

    int resultado_latitude[5570], tamanho_latitude = 0;
    int resultado_longitude[5570], tamanho_longitude = 0;
    int resultado_ddd[5570], tamanho_ddd = 0;

    consulta_latitude_maior_que(avl_latitude, -200000.0, resultado_latitude, &tamanho_latitude);
    consulta_longitude_intervalo(avl_longitude, -400000.0, -600000.0, resultado_longitude, &tamanho_longitude);
    consulta_ddd_igual(avl_ddd, 67, resultado_ddd, &tamanho_ddd);

    int *resultado_interseccao = intersecao(resultado_latitude, tamanho_latitude, resultado_longitude, tamanho_longitude, &tamanho_latitude);
    resultado_interseccao = intersecao(resultado_interseccao, tamanho_latitude, resultado_ddd, tamanho_ddd, &tamanho_latitude);

    printf("Resultados das interseções:\n");
    thash hash;
    hash_constroi(&hash, 5570, NULL); 
    print_resultado(hash, resultado_interseccao, tamanho_latitude);

    cJSON_Delete(json);
    avl_destroi(avl_latitude);
    avl_destroi(avl_longitude);
    avl_destroi(avl_ddd);
    hash_apaga(&hash);

    return 0;
}