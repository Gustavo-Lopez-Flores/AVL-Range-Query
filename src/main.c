#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/avl.h"
#include "../include/compare.h"
#include "../cJSON/cJSON.h"

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
        item.nome = cJSON_GetObjectItem(municipio, "nome")->valuestring;
        item.latitude = cJSON_GetObjectItem(municipio, "latitude")->valuedouble;
        item.longitude = cJSON_GetObjectItem(municipio, "longitude")->valuedouble;
        item.codigo_uf = cJSON_GetObjectItem(municipio, "codigo_uf")->valueint;
        item.ddd = cJSON_GetObjectItem(municipio, "ddd")->valueint;

        avl_insere(root, item, cmp);
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

    int resultado_latitude[1000], tamanho_latitude = 0;
    int resultado_longitude[1000], tamanho_longitude = 0;
    int resultado_ddd[1000], tamanho_ddd = 0;

    consulta_latitude_maior_que(avl_latitude, 50.0, resultado_latitude, &tamanho_latitude);
    consulta_longitude_intervalo(avl_longitude, 20.0, 30.0, resultado_longitude, &tamanho_longitude);
    consulta_ddd_igual(avl_ddd, 67, resultado_ddd, &tamanho_ddd);

    int tamanho_intersecao1;
    int *intersecao1 = intersecao(resultado_latitude, tamanho_latitude, resultado_longitude, tamanho_longitude, &tamanho_intersecao1);

    int tamanho_intersecao_final;
    int *intersecao_final = intersecao(intersecao1, tamanho_intersecao1, resultado_ddd, tamanho_ddd, &tamanho_intersecao_final);

    printf("Códigos IBGE das cidades que atendem às três queries:\n");
    for (int i = 0; i < tamanho_intersecao_final; i++) {
        printf("%d\n", intersecao_final[i]);
    }

    free(intersecao1);
    free(intersecao_final);
    cJSON_Delete(json);

    avl_destroi(avl_nome);
    avl_destroi(avl_latitude);
    avl_destroi(avl_longitude);
    avl_destroi(avl_codigo_uf);
    avl_destroi(avl_ddd);

    return 0;
}