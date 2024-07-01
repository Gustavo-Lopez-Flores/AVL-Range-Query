#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/avl.h"
#include "../include/compare.h"
#include "../include/cJSON/cJSON.h"

void load_json_file(const char *filename, cJSON **json) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir");
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
        item.capital = cJSON_GetObjectItem(municipio, "capital")->valueint;
        item.codigo_uf = cJSON_GetObjectItem(municipio, "codigo_uf")->valueint;
        item.siafi_id = cJSON_GetObjectItem(municipio, "siafi_id")->valueint;
        item.ddd = cJSON_GetObjectItem(municipio, "ddd")->valueint;
        item.fuso_horario = cJSON_GetObjectItem(municipio, "fuso_horario")->valuestring;

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

    cJSON_Delete(json);

    return 0;
}