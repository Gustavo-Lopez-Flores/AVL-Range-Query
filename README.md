# Implementação de AVL para Suporte a Range Query

Este trabalho consiste em adaptar uma árvore AVL para suportar range queries utilizando uma base de dados de cidades brasileiras fornecida pelo IBGE no [JSON de municípios brasileiros](https://github.com/kelvins/municipios-brasileiros/blob/main/json/municipios.json).. O objetivo é criar uma estrutura de dados eficiente para realizar buscas por intervalos em diferentes campos das cidades. As regras e tarefas para o trabalho são descritas a seguir.

## Tarefa 1: Adaptação da AVL

### Descrição
1. **Adaptação da AVL**
    1.1. **Adição de ponteiro para o pai:** Modifique o código da árvore AVL para que cada nó possua um ponteiro para o seu nó pai.
    1.2. **Lista encadeada em cada nó:** Cada nó da árvore deve ter uma lista encadeada que armazena os registros com chaves iguais. Isso permitirá que múltiplos registros com a mesma chave sejam armazenados em um único nó.
    1.3. **Implementação da função de sucessor:** Implemente a função de sucessor de um nó que consiga olhar para os ancestrais, conforme descrito no livro do Cormen. Essa funcionalidade permitirá a realização das range queries.

## Tarefa 2: Construção das AVLs

### Descrição
Construa uma AVL para cada um dos seguintes campos como chave de busca:
- `nome`
- `latitude`
- `longitude`
- `codigo_uf`
- `ddd`

Você terá que instanciar cinco árvores AVL, uma para cada campo. O registro armazenado deve conter a chave de busca e o `codigo_ibge`. Na AVL adaptada, as chaves replicadas são armazenadas em um mesmo nó. Por exemplo, todas as cidades com DDD 67 serão armazenadas em um mesmo nó.

## Tarefa 3: Combinação de Range Queries

### Descrição
Considere as seguintes três queries:
1. Cidades com `latitude > 50`
2. `20 < longitude < 30`
3. `DDD == 67`

Cada query irá retornar um conjunto de códigos IBGE. A combinação consiste em fazer a intersecção desses conjuntos.

## Tarefa 4: Interface

### Descrição
Desenvolva uma interface para permitir a realização de range queries e combinações de range queries em qualquer um dos cinco campos definidos na Tarefa 2. A busca deve retornar todos os campos das cidades fornecidas na base de dados. Para a busca das cidades pelo código IBGE, utilize uma tabela hash.

## Observações

   - Execute na raiz do projeto para compilar, depois execute `./saida`

         gcc -I./cJSON -I./include src/avl.c src/compare.c src/main.c cJSON/cJSON.c -o saida

---

**Nota:** Este projeto foi desenvolvido como parte de um trabalho acadêmico na disciplina de Estrutura de Dados. As funcionalidades foram implementadas com base em conceitos de estruturas de dados e algoritmos estudados em sala de aula.