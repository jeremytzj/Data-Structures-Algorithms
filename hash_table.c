/*
Este programa organiza uma base de dados de clientes de uma grande empresa,
dividindo-os em m grupos com base em uma função hash: o nome do cliente é
convertido para um número decimal interpretando a string como base 128 (ASCII),
e o grupo é determinado pelo resto da divisão desse número por m.

Operações suportadas:
1 nome quantidade -> Insere ou atualiza o cliente 'nome' com a nova 'quantidade'.
                     Se o nome já existir, a quantidade é adicionada à lista existente.
2 nome            -> Busca o cliente 'nome' e imprime:
                     - Todos os nomes do mesmo grupo, em ordem alfabética.
                     - A lista de quantidades associadas ao nome, em ordem de inserção.
                     - Se o nome não existir, imprime 0 na segunda linha.
0                 -> Encerra o programa.

Entrada:
- Primeira linha: um inteiro m (2 < m < 1.000.000)
- Demais linhas: uma sequência de operações (1, 2 ou 0) conforme descrito.

Saída:
Para cada operação de busca (operação 2), imprime:
1ª linha: nomes do grupo correspondente em ordem alfabética, separados por espaço.
2ª linha: lista de quantidades do cliente buscado, ou 0 se não encontrado.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define L 15

typedef struct node {
    char name[L];
    int *quantidades;
    int pedidos;
    int altura;
    struct node *r;
    struct node *l;
} node;

typedef struct tree {
    node *root;
    int grupo;
} tree;

typedef struct hash {
    int size;
    tree *grupos;
} hash;


node* createNode(char* nome, int qtd) {
    node *cliente = (node*)malloc(sizeof(node));
    if(cliente) {
        strcpy(cliente->name, nome);
        cliente->pedidos = 1;
        cliente->quantidades = malloc(10 * sizeof(int));
        cliente->quantidades[0] = qtd;
        cliente->r = NULL;
        cliente->l = NULL;
        cliente->altura = 0;
        return cliente;
    }
    return NULL;
}

int altura(node* n) {
    if(n == NULL) {
        return 0;
    }
    return n->altura;
}


node* searchNode(node* root, char* name) {
    if(root == NULL || strcmp(root->name, name) == 0) {
        return root;
    }
    if(strcmp(root->name, name) > 0) {
        return searchNode(root->l, name);
    }
    return searchNode(root->r, name);
}

node* rotateRight(node* y) {
    node* x = y->l;
    node* T2 = x->r;
    x->r = y;
    y->l = T2;
    y->altura = fmax(altura(y->l), altura(y->r)) + 1;
    x->altura = fmax(altura(x->l), altura(x->r)) + 1;
    return x;
}

node* rotateLeft(node* x) {
    node* y = x->r;
    node* T2 = y->l;
    y->l = x;
    x->r = T2;
    x->altura = fmax(altura(x->l), altura(x->r)) + 1;
    y->altura = fmax(altura(y->l), altura(y->r)) + 1;
    return y;
}

int heightDiff(node* n) {
    if(n == NULL) {
        return 0;
    }
    return altura(n->l) - altura(n->r);
}

node* insertNode(node* root, char* name, int qtd) {
    if(root == NULL) {
        return createNode(name, qtd);
    }
    if (strcmp(root->name, name) > 0) {
        root->l = insertNode(root->l, name, qtd);
    } else if (strcmp(root->name, name) < 0) {
        root->r = insertNode(root->r, name, qtd);
    } else {
        if(root->pedidos % 10 == 0) {
            root->quantidades = realloc(root->quantidades, (root->pedidos + 10) * sizeof(int));
        }
        root->quantidades[root->pedidos++] = qtd;
        return root;
    }

    root->altura = 1 + fmax(altura(root->l), altura(root->r));

    int balance = heightDiff(root);
    if(balance > 1 && strcmp(name, root->l->name) < 0) {
        return rotateRight(root);
    }
    if(balance < -1 && strcmp(name, root->r->name) > 0) {
        return rotateLeft(root);
    }
    if(balance > 1 && strcmp(name, root->l->name) > 0) {
        root->l = rotateLeft(root->l);
        return rotateRight(root);
    }
    if(balance < -1 && strcmp(name, root->r->name) < 0) {
        root->r = rotateRight(root->r);
        return rotateLeft(root);
    }

    return root;
}

void printTree(node* root) {
    if(root != NULL) {
        printTree(root->l);
        printf("%s ", root->name);
        printTree(root->r);
    }
}

int hashing(int m, char* name) {
    long long soma = 0;
    for(int i = 0; name[i] != '\0'; i++) {
        soma = (soma * 128 + name[i]) % m;
    }
    return soma;
}

void insertClient(char* name, int qtd, hash* tabela, int index) {
    tabela->grupos[index].root = insertNode(tabela->grupos[index].root, name, qtd);
}

void searchClient(char* name, hash* tabela, int index) {
    node* target = searchNode(tabela->grupos[index].root, name);
    if(target == NULL) {
        printf("\n0\n");
    } else {
        printTree(tabela->grupos[index].root);
        printf("\n");
        for(int i = 0; i < target->pedidos; i++) {
            printf("%d ", target->quantidades[i]);
        }
        printf("\n");
    }
}

int main(void) {
    int m, op, qtd, index;
    char temp[L];
    scanf("%d", &m);

    hash* tabela = (hash*)malloc(sizeof(hash));
    tabela->size = m;
    tabela->grupos = (tree*)malloc(m * sizeof(tree));
    for(int i = 0; i < m; i++) {
        tabela->grupos[i].root = NULL;
    }

    while(1) {
        scanf("%d", &op);
        if(op == 0) {
            
            free(tabela->grupos);
            free(tabela);
            return 0;
        } else if(op == 1) {
            scanf("%s %d", temp, &qtd);
            index = hashing(m, temp);
            insertClient(temp, qtd, tabela, index);
        } else if(op == 2) {
            scanf("%s", temp);
            index = hashing(m, temp);
            searchClient(temp, tabela, index);
        }
    }
}