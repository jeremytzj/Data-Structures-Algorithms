/*
Este é um programa que, dada uma lista com palavras em uma língua estrangeira e suas respectivas traduções no português, consiga traduzir uma sequência de palavras qualquer
nesse idioma para o português.

Entrada:
A entrada inicia-se com uma linha contendo dois inteiros M e N ,referente ao número de termos com traduções conhecidas e o número de linhas da letra a ser traduzida,respectivamente. Nos próximos M pares de linhas devem ser apresentados os termos originais e suas traduções em português sucessivamente, ou seja, a cada par de linha, a primeira linha contém o termo (palavra simples) na língua de origem e a segunda linha contém a sua tradução em português (pode ter uma ou mais palavras). Todas as palavras (na língua estrangeira e no português) devem ser compostas apenas por letras minúsculas (a-z). As próximas N linhas devem conter a sequência a ser traduzida. Cada linha pode conter vários termos na língua de origem, sendo que os termos são separados por espaços. Todos os termos devem ser compostos apenas por letras minúsculas (a-z).

Saída:
Imprime N linhas contendo a tradução da sequência passada na entrada. Os termos da letra que sem tradução conhecida são apresentados em sua forma original.
*/






#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 83

typedef struct word {
    char *palavra;
    char **traducao;
    int traducao_count;
    struct word *next;
} word;

word* hash_table[HASH_SIZE] = {NULL};

unsigned int hashing(const char *palavra) {
    unsigned int hash = 5381;
    int c;
    while ((c = *palavra++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % HASH_SIZE;
}

void insert_word(const char *palavra, const char *traducao) {
    unsigned int index = hashing(palavra);

    word *new_word = (word*) malloc(sizeof(word));
    new_word->palavra = strdup(palavra);

    // Processa tradução
    char *token = strtok(strdup(traducao), " ");
    new_word->traducao_count = 0;
    new_word->traducao = NULL;
    while (token) {
        new_word->traducao = realloc(new_word->traducao, (new_word->traducao_count + 1) * sizeof(char *));
        new_word->traducao[new_word->traducao_count++] = strdup(token);
        token = strtok(NULL, " ");
    }

    // Insere na hash table
    new_word->next = hash_table[index];
    hash_table[index] = new_word;
}

word* find_word(const char *palavra) {
    unsigned int index = hashing(palavra);
    word *current = hash_table[index];
    while (current) {
        if (strcmp(current->palavra, palavra) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

void translate_and_print(char *text) {
    char *token = strtok(text, " ");
    while (token) {
        if (strcmp(token, "@") == 0) {
            printf("\n");  // Imprime uma quebra de linha ao encontrar '@'
        } else {
            word *w = find_word(token);
            if (w) {
                for (int i = 0; i < w->traducao_count; i++) {
                    printf("%s ", w->traducao[i]);
                }
            } else {
                printf("%s ", token);  // Palavra não encontrada
            }
        }
        token = strtok(NULL, " ");
    }
    printf("\n");
}


int main(void) {
    int m, n;
    scanf("%d %d", &m, &n);
    getchar();

    for (int i = 0; i < m; i++) {
        char p[101], t[101];
        scanf("%s", p);
        getchar();
        fgets(t, sizeof(t), stdin);
        t[strcspn(t, "\n")] = 0;  // Remove newline
        insert_word(p, t);
    }

    char *original = malloc(104 * n * sizeof(char));
    original[0] = '\0';

    for (int i = 0; i < n; i++) {
        char line[104];
        fgets(line, sizeof(line), stdin);
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n'){
            line[len - 1] = ' ';
            line[len] = '@';
            line[len+1] = ' ';
            line[len+2] = '\0';
        }

        strcat(original, line);
    }

    translate_and_print(original);

    // Libera memória
    for (int i = 0; i < HASH_SIZE; i++) {
        word *current = hash_table[i];
        while (current) {
            word *temp = current;
            current = current->next;
            for (int j = 0; j < temp->traducao_count; j++) {
                free(temp->traducao[j]);
            }
            free(temp->traducao);
            free(temp->palavra);
            free(temp);
        }
    }
    free(original);

    return 0;
}