/*
Este código ordena strings utilizando os métodos Radix Sort e Counting Sort

Entrada: 
A primeira linha da entrada contém o número N (2 ≤ N ≤ 100.000) de nomes a serem ordenados. Nas próximas N linhas são apresentados os nomes, um nome em cada linha. A linha seguinte contém dois números P (1 ≤ P ≤ N) da posição do primeiro nome a ser impresso e o número M (1 ≤ M ≤ N-P+1) de números a partir do P-ésimo nome.

Saída:
Para cada “dígito” i do Radix sort, o código imprime uma linha com os 27 valores do vetor C do
Counting sort. Nas próximas M linhas, imprime os M nomes presentes a partir da P-ésima
posição da lista ordenada, um nome a cada linha sem os caracteres adicionais.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//a struct word representa uma palavra
typedef struct {
    char palavra[15];
} word;


int cindex(char ch) {
    //retorna o index do caractere na lista c
    if (ch == '\0') {
        return 0; 
    }
    return ch - 'a' + 1; 
}


void countSort(word *vet, int size, int pos) {
    // c é a lista de contagem e b é a lista temporária para ordenação
    int *c = calloc(27, sizeof(int)), i;
    word *b = malloc(size * sizeof(word));
    char ch;


    //adiciona 1 ao index do caractere na lista de contagem
    for (i = 0; i < size; i++) {
        int mx = strlen(vet[i].palavra) / sizeof(char);
        if(pos < mx ){
            ch = vet[i].palavra[pos];
        }
        else{
            ch = '\0';
        }
        c[cindex(ch)]++;
    }

    for (i = 1; i < 27; i++) {
        c[i] += c[i - 1];
    }
    
    //prof pediu pra printar a lista c antes de ordenar
    for(i = 0; i < 27; i++){
        printf("%d ", c[i]);
    }
    printf("\n");

    for (i = size - 1; i >= 0; i--) {
        if(pos < strlen(vet[i].palavra)){
            ch = vet[i].palavra[pos];
        }
        else{
            ch = '\0';
        }
        b[c[cindex(ch)] - 1] = vet[i];
        c[cindex(ch)]--;
    }

    //copia a lista ordenada para a lista original
    for (int i = 0; i < size; i++) {
        strcpy(vet[i].palavra, b[i].palavra);
    }

    free(b);
    free(c);
}



void radixSort(word *vet, int size) {
    int maxsize = 0, i, len;
    //verifica qual é o tamanho da maior palavra
    for (i = 0; i < size; i++) {
        len = strlen(vet[i].palavra);
        if (len > maxsize) {
            maxsize = len;
        };
    }
    // comeca a ordenar a partir da ultima letra
    for (int pos = maxsize - 1; pos >= 0; pos--) {
        countSort(vet, size, pos);
    }
}

int main(void) {
    int size, p, m;
    
    scanf("%d", &size);
    // vet é o vetor de structs
    word *vet = malloc(size * sizeof(word));
    
    for (int i = 0; i < size; i++) {
        scanf("%s", vet[i].palavra);
        for (int j = 0; j < strlen(vet[i].palavra); j++) {
            vet[i].palavra[j] = tolower(vet[i].palavra[j]);
        }
    }
    
    scanf("%d %d",&p, &m);
    radixSort(vet, size);

    for (int i = p-1; i < p - 1 + m; i++) {
        printf("%s\n", vet[i].palavra);
    }

    free(vet);
    return 0;
}