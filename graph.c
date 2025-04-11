/*
Este programa simula e analisa a cadeia de comando de uma facção criminosa, com base em
relações de comando direto entre seus membros e o tempo total que cada um passou preso.

A facção é representada como um grafo direcionado:
- Cada vértice representa um membro.
- Uma aresta do membro X para o membro Y indica que X comanda Y diretamente.
- Cada membro possui um tempo de prisão associado.
- A hierarquia pode ser modificada dinamicamente por instruções de troca de posição entre membros.

O programa responde a consultas que buscam identificar o maior tempo de prisão entre os chefes (diretos
ou indiretos) de um membro específico.

--------------------------
ENTRADA:
--------------------------
- A primeira linha contém três inteiros:
  N → número de membros (1 ≤ N ≤ 500)
  M → número de relações de comando direto (0 ≤ M ≤ 60.000)
  I → número de instruções (1 ≤ I ≤ 500)

- A segunda linha contém N inteiros T_i (1 ≤ T_i ≤ 100), onde T_i representa o tempo total que o membro i passou preso.

- As próximas M linhas contêm dois inteiros X e Y (1 ≤ X, Y ≤ N, X ≠ Y), indicando que o membro X comanda diretamente o membro Y.

- As próximas I linhas representam instruções, que podem ser:
  * T A B → instrução de troca de posição entre os membros A e B.
  * P E   → consulta que solicita o maior tempo de prisão entre os chefes (diretos ou indiretos) do membro E.

--------------------------
SAÍDA:
--------------------------
- Para cada instrução de consulta (P E), o programa imprime uma linha:
  * O maior tempo de prisão entre todos os membros que comandam direta ou indiretamente E.
  * Se o membro E não possui nenhum chefe, imprime-se um asterisco '*' (sem aspas).

O programa termina sempre com uma instrução de consulta.
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    int years;
    int index;
    int*adj;
    int auts;
    struct node*bot;
}node;

typedef struct stack {
    int size;
    int * pilha;
}stack;

node* initGraph(int N, int*indexes){
    node*graph = (node*)malloc(N*sizeof(node));
    for(int i = 0; i < N; i ++){
        scanf("%d", &graph[i].years);
        graph[i].index = i;
        graph[i].auts = 0;
        graph[i].adj= NULL;
        graph[i].bot = NULL;
        indexes[i] = i;
    }
    return graph;
}


void swapNodes(node*graph, int swap1, int swap2, int *indexes){
    int temp, tempAge;
    tempAge = graph[indexes[swap1-1]].years;
    graph[indexes[swap1-1]].years = graph[indexes[swap2-1]].years;
    graph[indexes[swap2-1]].years = tempAge;
    temp = indexes[swap1-1];
    indexes[swap1-1] = indexes[swap2-1];
    indexes[swap2-1] = temp;
}

void pushNode(stack* toVisit, int value){
    toVisit->size++;
    toVisit->pilha = realloc(toVisit->pilha, toVisit->size*sizeof(int));
    toVisit->pilha[toVisit->size -1] = value;
}

int popStack(stack* toVisit){
    int popped = toVisit -> pilha[toVisit->size-1];
    toVisit->size--;
    toVisit->pilha = realloc(toVisit->pilha, toVisit->size*sizeof(int));
    return popped;
}

// DFS (Depth First Search)
int searchMaxNode(node* graph, int search, int *visited, int *indexes){
    int maxYears = -1;
    stack *toVisit = (stack*)malloc(sizeof(stack));
    toVisit->size = 0;
    toVisit -> pilha = NULL;
    
    for(int i = 0; i < graph[search].auts; i++){
        pushNode(toVisit, graph[search].adj[i]);
    }

    while (toVisit -> size > 0){
        int current = popStack(toVisit);
        if(visited[current] == 0){
            visited[current] = 1;
            if(graph[current].years > maxYears){
                maxYears = graph[current].years;
            }

            for(int i =0; i < graph[current].auts; i++){
                int next = graph[current].adj[i];
                if(visited[next]== 0){
                    pushNode(toVisit, next);
                }
            }
        }
        
    }
    free(toVisit->pilha);
    free(toVisit);
    return maxYears;
}


int main(void){
    int N,M, I;
    scanf("%d %d %d",&N,&M,&I);
    int *indexes = (int*)malloc(N*sizeof(int));
    // indexes guarda o index de cada node no grafo
    node* graph = initGraph(N, indexes);


    for(int i = 0; i < M; i++){
        int aut, sub;
        scanf("%d %d", &aut, &sub);
        graph[sub-1].auts++;
        if(graph[sub-1].adj==NULL){
            graph[sub-1].adj = malloc(sizeof(int));
        }
        else{
            graph[sub-1].adj = realloc(graph[sub-1].adj, sizeof(int)*graph[sub-1].auts);
        }
        graph[sub-1].adj[graph[sub-1].auts - 1] = aut-1; 

        
    }
    // cria as relacoes do grafo 

    for(int i = 0; i < I; i++){
        char typeOp;
        scanf(" %c", &typeOp);

        if(typeOp == 'P'){
            int searched;
            scanf("%d" , &searched);
            searched = indexes[searched-1];
            int visited[N];
            memset(visited, 0, sizeof(visited));  
            int maxYears = searchMaxNode(graph, searched, visited, indexes);
            if (maxYears == -1){
                printf("*\n");
            }
            else{
                printf("%d\n",maxYears);
            }
        }
        else if(typeOp == 'T'){
            int swap1, swap2;
            scanf("%d %d", &swap1, &swap2);
            swapNodes(graph, swap1,swap2, indexes);
        }
    }
    
}