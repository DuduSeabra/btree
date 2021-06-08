#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct lista {
    int valor_no;
    struct lista *prox;
} LISTA;

typedef struct fila {
    int indice;
    struct fila *prox;
} FILA;

typedef struct cabeca {
    FILA *head;
} CABECA;

void inicializar(LISTA *grafo, int n_nos) {
    for(int i = 0; i < n_nos; i++) {
        grafo[i].prox = NULL;
    }
}

void inserir(LISTA *grafo, int n_arestas, int n_instancia) {
    int v1, v2;
    LISTA *aux;

    if(n_instancia == 0) {
        for(int i = 0; i < n_arestas; i++) {
            //printf("Digite os grafos conectados: ");
            scanf("%d%d", &v1, &v2);
            aux = grafo[v1-1].prox;

            if(v1 != v2) {
                LISTA *new_node1 = (LISTA *) malloc(sizeof(LISTA));
                grafo[v1-1].prox = new_node1;
                new_node1->valor_no = v2-1;
                new_node1->prox = aux;

                aux = grafo[v2-1].prox;
                LISTA *new_node2 = (LISTA *) malloc(sizeof(LISTA));
                grafo[v2-1].prox = new_node2;
                new_node2->valor_no = v1-1;
                new_node2->prox = aux;
            }
            else {
                LISTA *new_node1 = (LISTA *) malloc(sizeof(LISTA));
                grafo[v1-1].prox = new_node1;
                new_node1->valor_no = v2-1;
                new_node1->prox = aux;
            }
        }
    }
    else if(n_instancia == 1) {
        for(int i = 0; i < n_arestas; i++) {
            scanf("%d%d", &v1, &v2);
            aux = grafo[v1-1].prox;

            LISTA *new_node1 = (LISTA *) malloc(sizeof(LISTA));
            grafo[v1-1].prox = new_node1;
            new_node1->valor_no = v2-1;
            new_node1->prox = aux;
        }
    }
}

int all_blacks(char *cor, int n_nos) {
    for(int i = 0; i < n_nos; i++) {
        if(cor[i] != 'B')
            return 0;
    }
    return 1;
}

void bfs(LISTA *grafo, int n_nos, int n_arestas, int n_instancia, int n_inicial) {
    char d[n_nos+1][5];
    char *cor = (char *) malloc((n_nos+1) * sizeof(char));
    int *pi = (int *) malloc((n_nos+1) * sizeof(int));
    CABECA *a = (CABECA *) malloc(sizeof(CABECA));
    int aux;
    LISTA *node_aux;

    char c[3];
    
    a->head = NULL;
    for(int i = 0; i < n_nos; i++)
        pi[i] = -1;
    for(int i = 0; i < n_nos; i++)
        strcpy(d[i], "inf");
    for(int i = 0; i < n_nos; i++)
        cor[i] = 'W';
    printf("Noh");
    for(int i = 0; i < n_nos; i++)
        printf(" %d", i+1);
    printf("\n");
    printf("d[]");
    for(int i = 0; i < n_nos; i++)
        printf(" %s", d[i]);
    printf("\n");
    printf("pi[]");
    for(int i = 0; i < n_nos; i++)
        printf(" %d", pi[i]);
    printf("\n");
    printf("cor");
    for(int i = 0; i < n_nos; i++)
        printf(" %c", cor[i]);
    printf("\n");
    printf("Fila\n//");

    int pai = -1;
    FILA *auxiliar;
    FILA *inicio = (FILA *) malloc(sizeof(FILA));
    a->head = inicio;
    inicio->indice = n_inicial-1;
    inicio->prox = NULL;

    while(all_blacks(cor, n_nos) == 0) {
        auxiliar = a->head;
        if(auxiliar != NULL) {
            if(auxiliar->indice == n_inicial-1) {
                c[0] = 0 + '0';
                strcpy(d[auxiliar->indice], c);
                pi[auxiliar->indice] = pai;
                cor[auxiliar->indice] = 'G';
            }
        }
        else break;
        FILA *x1 = a->head;

        printf("\n\n");
        printf("Noh");
        for(int i = 0; i < n_nos; i++)
            printf(" %d", i+1);
        printf("\n");
        printf("d[]");
        for(int i = 0; i < n_nos; i++) {
            if(d[i][0] == 'i')
                printf(" %s", d[i]);
            else
                printf(" %c", d[i][0]);
        }
        printf("\n");
        printf("pi[]");
        for(int i = 0; i < n_nos; i++)
            printf(" %d", pi[i]);
        printf("\n");
        printf("cor");
        for(int i = 0; i < n_nos; i++)
            printf(" %c", cor[i]);
        printf("\n");
        printf("Fila\n");
        while(x1 != NULL) {
            printf("%d->", x1->indice+1);
            x1 = x1->prox;
        }
        printf("//");

        pai = a->head->indice+1;
        aux = a->head->indice;
        node_aux = grafo[aux].prox;
        cor[aux] = 'B';
        a->head = a->head->prox;
        auxiliar = a->head;


        if(auxiliar != NULL) {
            while(auxiliar->prox != NULL)
                auxiliar = auxiliar->prox;
            while(node_aux != NULL) {
                if(cor[node_aux->valor_no] == 'W') {
                    FILA *new_node1 = (FILA *) malloc(sizeof(FILA));
                    auxiliar->prox = new_node1;
                    new_node1->indice = node_aux->valor_no;
                    int n1 = d[aux][0]+1 - '0';
                    c[0] = n1 + '0';
                    strcpy(d[node_aux->valor_no], c);
                    pi[node_aux->valor_no] = pai;
                    cor[node_aux->valor_no] = 'G';
                    auxiliar = auxiliar->prox;
                }

                node_aux = node_aux->prox;
            }
        }
        else {
            if(cor[node_aux->valor_no] == 'W') {
                FILA *new_node = (FILA *) malloc(sizeof(FILA));
                a->head = new_node;
                new_node->indice = node_aux->valor_no;
                int n1 = d[aux][0]+1 - '0';
                c[0] = n1 + '0';
                strcpy(d[node_aux->valor_no], c);
                pi[node_aux->valor_no] = pai;
                cor[node_aux->valor_no] = 'G';
                node_aux = node_aux->prox;
                auxiliar = new_node;
            }
            while(node_aux != NULL) {
                if(cor[node_aux->valor_no] == 'W') {
                    FILA *new_node1 = (FILA *) malloc(sizeof(FILA));
                    auxiliar->prox = new_node1;
                    new_node1->indice = node_aux->valor_no;
                    int n1 = d[aux][0]+1 - '0';
                    c[0] = n1 + '0';
                    strcpy(d[node_aux->valor_no], c);
                    pi[node_aux->valor_no] = pai;
                    cor[node_aux->valor_no] = 'G';
                    auxiliar = auxiliar->prox;
                }

                node_aux = node_aux->prox;
            }
        }
    }
    printf("\n\n");
    printf("Noh");
    for(int i = 0; i < n_nos; i++)
        printf(" %d", i+1);
    printf("\n");
    printf("d[]");
    for(int i = 0; i < n_nos; i++) {
        if(d[i][0] == 'i')
            printf(" %s", d[i]);
        else
            printf(" %c", d[i][0]);
    }
    printf("\n");
    printf("pi[]");
    for(int i = 0; i < n_nos; i++)
        printf(" %d", pi[i]);
    printf("\n");
    printf("cor");
    for(int i = 0; i < n_nos; i++)
        printf(" %c", cor[i]);
    printf("\n");
    printf("Fila\n");
    printf("//");
    
}

int main() {

    int n_nos;
    int n_arestas;
    int n_instancia;
    int n_inicial;
    
    //printf("Números de nós: ");
    scanf("%d", &n_nos);
    //printf("Números de vértices: ");
    scanf("%d", &n_arestas);
    //printf("Qual instância? (0-não orientado, 1-orientado, 2-não orientado e ponderado, 3-orientado e ponderado)\n");
    scanf("%d", &n_instancia);
    scanf("%d", &n_inicial);

    LISTA *grafo = (LISTA *) malloc((n_nos+1) * sizeof(LISTA));

    inicializar(grafo, n_nos);

    inserir(grafo, n_arestas, n_instancia);

    bfs(grafo, n_nos, n_arestas, n_instancia, n_inicial);    

    return 0;
}