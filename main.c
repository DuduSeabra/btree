#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct btpage {
    int total_chaves;
    struct btpage *p_filha[7];
    struct btpage *no_pai;
    bool no_folha;
    bool no_raiz;
    int posicao_em_relacao_ao_pai;
    char chaves[5][6];
} BTPAGE;

typedef struct btree { 
    BTPAGE *root;
} BTREE;

int text_compare(char *v0, char *v1) {
    for(int i = 0; i < 5; i++) {
        if(v0[i] < v1[i])
            return 0;
        if(v0[i] > v1[i])
            return 1;
    }
}

void adicionar_no_vetor(BTPAGE *node_aux, char *key) {
    int aux = 0;
        while(1) {
            if(text_compare(key, node_aux->chaves[aux]) == 0) {
                char a[6];
                char b[6];
                strcpy(b, key);
                for(int i = aux; i < 5; i++) {
                    strcpy(a, node_aux->chaves[i]);
                    strcpy(node_aux->chaves[i], b);
                    strcpy(b, a);
                }
                break;
            }
            if(node_aux->chaves[aux][0] == '*') {
                strcpy(node_aux->chaves[aux], key);
                break;
            }
            aux++;
        }
}

int posicao_chave(BTPAGE *node_aux, char *key) {
    int i = 0;
    if(key[0] == '*')
        return -1;
    while(strcmp(node_aux->chaves[i], key) != 0)
        i++;

    return i;
}

void explosao_no_raiz(BTPAGE *node_aux) {
    BTPAGE *new_node1 = (BTPAGE *) malloc(sizeof(BTPAGE));
    BTPAGE *new_node2 = (BTPAGE *) malloc(sizeof(BTPAGE));

    for(int i = 0; i < 3; i++) {
        new_node1->p_filha[i] = node_aux->p_filha[i];
        new_node2->p_filha[i] = node_aux->p_filha[i+3];
        new_node1->p_filha[i]->posicao_em_relacao_ao_pai = i;
        new_node2->p_filha[i]->posicao_em_relacao_ao_pai = i;
        new_node1->p_filha[i]->no_pai = new_node1;
        new_node2->p_filha[i]->no_pai = new_node2;
    }
        

    node_aux->p_filha[0] = new_node1;
    node_aux->p_filha[1] = new_node2;

    for(int i = 0; i < 5; i++) {
        if(i < 2) {
            strcpy(new_node1->chaves[i], node_aux->chaves[i]);
        }
        if(i > 2) {
            strcpy(new_node2->chaves[i-3], node_aux->chaves[i]);
        }
    }
    strcpy(node_aux->chaves[0], node_aux->chaves[2]);
    for(int i = 1; i < 5; i++) {
        strcpy(node_aux->chaves[i], "*");
    }
    for(int i = 2; i < 5; i++) {
        strcpy(new_node1->chaves[i], "*");
        strcpy(new_node2->chaves[i], "*");
    }
    new_node1->total_chaves = 2;
    new_node2->total_chaves = 2;
    node_aux->total_chaves = 1;
    new_node1->no_pai = node_aux;
    new_node2->no_pai = node_aux;
    new_node1->posicao_em_relacao_ao_pai = 0;
    new_node2->posicao_em_relacao_ao_pai = 1;

    node_aux->no_folha = false;
    if(new_node1->p_filha[0] == NULL)
        new_node1->no_folha = true;
    else
        new_node1->no_folha = false;
    
    if(new_node2->p_filha[0] ==NULL)
        new_node2->no_folha = true;
    else
        new_node2->no_folha = false;

    new_node1->no_raiz = false;
    new_node2->no_raiz = false;

    for(int i = 2; i < 5; i++)
        node_aux->p_filha[i] = NULL;
}

void explosao_outro_no(BTPAGE *node_aux, BTPAGE *node_pai, int posicao_filho) {
    BTPAGE *new_node = (BTPAGE *) malloc(sizeof(BTPAGE));

    node_pai->p_filha[posicao_filho+1] = new_node;

    strcpy(new_node->chaves[0], node_aux->chaves[3]);
    strcpy(new_node->chaves[1], node_aux->chaves[4]);

    adicionar_no_vetor(node_pai, node_aux->chaves[2]);

    for(int i = 2; i < 5; i++) {
        strcpy(node_aux->chaves[i], "*");
        strcpy(new_node->chaves[i], "*");
    }
    node_aux->total_chaves = 2;
    new_node->total_chaves = 2;
    new_node->no_folha = true;
    new_node->no_raiz = false;
    node_pai->total_chaves++;
    new_node->no_pai = node_pai;
}

void insertion(BTREE *arvore, char *key) {
    if(arvore->root == NULL) {
        BTPAGE *new_node = (BTPAGE *) malloc(sizeof(BTPAGE));
        arvore->root = new_node;
        new_node->total_chaves = 0;
        new_node->no_folha = true;
        new_node->no_raiz = true;
        for(int i = 0; i < 5; i++)
            new_node->p_filha[i] = NULL;
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++)
                new_node->chaves[i][j] = '*';
        }
        strcpy(new_node->chaves[0], key);
        new_node->no_pai == NULL;
        new_node->total_chaves++;
        return;
    }
    int aux;
    int posicao_filho;
    BTPAGE *node_aux = arvore->root;
    BTPAGE *node_pai;

    while(node_aux->no_folha == false) { // vai achar o nó que será inserido
        if(text_compare(key, node_aux->chaves[0]) == 0) {
            node_pai = node_aux;
            node_aux = node_aux->p_filha[0];
        }
        else {
            for(int i = 0; i < 4 && node_aux->chaves[i][0] != '*'; i++) {
                if(text_compare(key, node_aux->chaves[i]) == 1)
                    aux = i+1;
            }
            node_pai = node_aux;
            posicao_filho = aux;
            node_aux = node_aux->p_filha[aux];
        }
    }
    //printf("%d\n", node_aux->posicao_em_relacao_ao_pai);
    if(node_aux->total_chaves < 4) {
        adicionar_no_vetor(node_aux, key);
        node_aux->total_chaves++;
    }
    else {
        if(node_aux->no_raiz == true) {
            BTPAGE *new_node1 = (BTPAGE *) malloc(sizeof(BTPAGE));
            BTPAGE *new_node2 = (BTPAGE *) malloc(sizeof(BTPAGE));

            adicionar_no_vetor(node_aux, key);

            for(int i = 0; i < 3; i++) {
                new_node1->p_filha[i] = node_aux->p_filha[i];
                new_node2->p_filha[i] = node_aux->p_filha[i+3];
            }

            node_aux->p_filha[0] = new_node1;
            node_aux->p_filha[1] = new_node2;
            new_node1->posicao_em_relacao_ao_pai = 0;
            new_node2->posicao_em_relacao_ao_pai = 1;
            new_node1->no_pai = node_aux;
            new_node2->no_pai = node_aux;

            for(int i = 0; i < 5; i++) {
                if(i < 2) {
                    strcpy(new_node1->chaves[i], node_aux->chaves[i]);
                }
                if(i > 2) {
                    strcpy(new_node2->chaves[i-3], node_aux->chaves[i]);
                }
            }
            strcpy(node_aux->chaves[0], node_aux->chaves[2]);

            for(int i = 1; i < 5; i++) {
                strcpy(node_aux->chaves[i], "*");
            }
            for(int i = 2; i < 5; i++) {
                strcpy(new_node1->chaves[i], "*");
                strcpy(new_node2->chaves[i], "*");
            }
            new_node1->total_chaves = 2;
            new_node2->total_chaves = 2;
            node_aux->total_chaves = 1;

            node_aux->no_folha = false;
            new_node1->no_folha = true;
            new_node2->no_folha = true;

            new_node1->no_raiz = false;
            new_node2->no_raiz = false;

            for(int i = 2; i < 5; i++)
                node_aux->p_filha[i] = NULL;
        }
        else {
            BTPAGE *new_node = (BTPAGE *) malloc(sizeof(BTPAGE));
            char *auxiliar;
            BTPAGE *ajudante;

            adicionar_no_vetor(node_aux, key);

            for(int i = 5; i >= node_aux->posicao_em_relacao_ao_pai+1; i--) {
                node_pai->p_filha[i] = node_pai->p_filha[i-1];
            }
            node_pai->p_filha[node_aux->posicao_em_relacao_ao_pai+1] = new_node;
            for(int i = node_aux->posicao_em_relacao_ao_pai+1; node_pai->p_filha[i] != NULL; i++) {
                node_pai->p_filha[i]->posicao_em_relacao_ao_pai = i;
            }

            strcpy(new_node->chaves[0], node_aux->chaves[3]);

            strcpy(new_node->chaves[1], node_aux->chaves[4]);

            adicionar_no_vetor(node_pai, node_aux->chaves[2]);

            auxiliar = node_aux->chaves[2];
            for(int i = 2; i < 5; i++) {
                strcpy(node_aux->chaves[i], "*");
                strcpy(new_node->chaves[i], "*");
            }
            new_node->posicao_em_relacao_ao_pai = node_aux->posicao_em_relacao_ao_pai+1;
            node_aux->total_chaves = 2;
            new_node->total_chaves = 2;
            new_node->no_folha = true;
            new_node->no_raiz = false;
            new_node->no_pai = node_pai;
            node_pai->total_chaves++;

            while(node_pai->total_chaves == 5) {
                if(node_pai->no_raiz == true) {
                    explosao_no_raiz(node_pai);
                }
                else {
                    explosao_outro_no(node_pai, node_aux->no_pai, node_aux->posicao_em_relacao_ao_pai);
                }
                node_pai = node_pai->no_pai;
                if(node_pai == NULL)
                    break;
            }
        }
    }
}

int removal(BTREE *arvore, char *key) {
    BTPAGE *node_aux = arvore->root;
    int aux = 0;

    while(node_aux != NULL) {  //encontra o nó da chave que será removido
        while(node_aux->chaves[aux][0] != '*' && aux != 4) {
            if(strcmp(node_aux->chaves[aux], key) == 0)
                break;
            aux++;
        }
        if(strcmp(node_aux->chaves[aux], key) == 0)
                break;
        for(int i = 0; i < 5; i++) {
            if(text_compare(key, node_aux->chaves[i]) == 0) {
                node_aux = node_aux->p_filha[i];
                break;
            }
            if(node_aux->chaves[i+1][0] == '*') {
                node_aux = node_aux->p_filha[i+1];
                break;
            }
        }
        aux = 0;
    }

    if(node_aux == NULL) {
        return 0;
    }

    if(node_aux->no_raiz == true && node_aux->p_filha[0] == NULL) {
        for(int i = aux; i < 4; i++) {
            strcpy(node_aux->chaves[i], node_aux->chaves[i+1]);
        }
        return 1;
    }

    if(node_aux->no_raiz == true && node_aux->p_filha[0] != NULL && node_aux->p_filha[1] != NULL) {
        if(node_aux->p_filha[0]->p_filha[0] == NULL && node_aux->p_filha[1]->p_filha[0] == 0 && node_aux->p_filha[0]->total_chaves == 2 && node_aux->p_filha[1]->total_chaves == 2) {
            strcpy(node_aux->chaves[0], node_aux->chaves[1]);
            adicionar_no_vetor(node_aux, node_aux->p_filha[0]->chaves[0]);
            adicionar_no_vetor(node_aux, node_aux->p_filha[0]->chaves[1]);
            adicionar_no_vetor(node_aux, node_aux->p_filha[1]->chaves[0]);
            adicionar_no_vetor(node_aux, node_aux->p_filha[1]->chaves[1]);
            node_aux->p_filha[0] = NULL;
            node_aux->p_filha[1] = NULL;
            return 1;
        }
    }

    if(node_aux->total_chaves >= 3 && node_aux->no_folha == true) { //caso 1: o nó folha ter mais que 2 chaves
        for(int i = aux; i < 4; i++) {
            strcpy(node_aux->chaves[i], node_aux->chaves[i+1]);
        }
        node_aux->total_chaves--;
    }

    else if(node_aux->total_chaves == 2 && node_aux->no_folha == true) { //caso2: o nó folha ter 2 chaves
        int posicao_no = node_aux->posicao_em_relacao_ao_pai;
        
        if(posicao_no > 0 && posicao_no < node_aux->no_pai->total_chaves) {
            if(node_aux->no_pai->p_filha[posicao_no-1]->total_chaves > 2) { //RR
                for(int i = aux; i < 4; i++) {
                    strcpy(node_aux->chaves[i], node_aux->chaves[i+1]);
                }

                adicionar_no_vetor(node_aux, node_aux->no_pai->chaves[posicao_no-1]);

                if(node_aux->no_pai->p_filha[posicao_no-1]->total_chaves == 3) {
                    strcpy(node_aux->no_pai->chaves[posicao_no-1], node_aux->no_pai->p_filha[posicao_no-1]->chaves[2]);
                    strcpy(node_aux->no_pai->p_filha[posicao_no-1]->chaves[2], "*");
                }
                else {
                    strcpy(node_aux->no_pai->chaves[posicao_no-1], node_aux->no_pai->p_filha[posicao_no-1]->chaves[3]);
                    strcpy(node_aux->no_pai->p_filha[posicao_no-1]->chaves[3], "*");
                }
                node_aux->no_pai->p_filha[posicao_no-1]->total_chaves--;
            }
            else if(node_aux->no_pai->p_filha[posicao_no-1]->total_chaves == 2 && node_aux->no_pai->p_filha[posicao_no+1]->total_chaves > 2) { //LL
                for(int i = aux; i < 4; i++) {
                    strcpy(node_aux->chaves[i], node_aux->chaves[i+1]);
                }
                adicionar_no_vetor(node_aux, node_aux->no_pai->chaves[posicao_no]);
                strcpy(node_aux->no_pai->chaves[posicao_no], node_aux->no_pai->p_filha[posicao_no+1]->chaves[0]);

                for(int i = 0; i < 4; i++) {
                    strcpy(node_aux->no_pai->p_filha[posicao_no+1]->chaves[i], node_aux->no_pai->p_filha[posicao_no+1]->chaves[i+1]);
                }
                node_aux->no_pai->p_filha[posicao_no+1]->total_chaves--;
            }
            else if(node_aux->no_pai->p_filha[posicao_no+1]->total_chaves == 2 && node_aux->no_pai->p_filha[posicao_no-1]->total_chaves == 2) {
                if(posicao_no == 1) {
                    for(int i = aux; i < 4; i++) {
                        strcpy(node_aux->chaves[i], node_aux->chaves[i+1]);
                    }
                    adicionar_no_vetor(node_aux->no_pai->p_filha[posicao_no-1], node_aux->no_pai->chaves[0]);

                    for(int i = 0; i < 4; i++) {
                        strcpy(node_aux->no_pai->chaves[i], node_aux->no_pai->chaves[i+1]);
                    }
                    adicionar_no_vetor(node_aux->no_pai->p_filha[posicao_no-1], node_aux->chaves[0]);

                    for(int i = 1; i < 5; i++) {
                        node_aux->no_pai->p_filha[i] = node_aux->no_pai->p_filha[i+1];
                    }
                    node_aux->no_pai->total_chaves--;
                    node_aux->no_pai->p_filha[posicao_no-1]->total_chaves = 4;
                }
                else {
                    for(int i = aux; i < 4; i++) {
                        strcpy(node_aux->chaves[i], node_aux->chaves[i+1]);
                    }
                    adicionar_no_vetor(node_aux->no_pai->p_filha[posicao_no-1], node_aux->no_pai->chaves[posicao_no-1]);

                    for(int i = 0; i < 4; i++) {
                        strcpy(node_aux->no_pai->chaves[i], node_aux->no_pai->chaves[i+1]);
                    }
                    adicionar_no_vetor(node_aux->no_pai->p_filha[posicao_no-1], node_aux->chaves[0]);

                    for(int i = posicao_no; i < 5; i++) {
                        node_aux->no_pai->p_filha[i] = node_aux->no_pai->p_filha[i+1];
                    }
                    node_aux->no_pai->total_chaves--;
                    node_aux->no_pai->p_filha[posicao_no-1]->total_chaves = 4;
                }
            }
        }

        else if(posicao_no == 0) {
            if(node_aux->no_pai->p_filha[posicao_no+1]->total_chaves > 2) {
                for(int i = aux; i < 4; i++) {
                    strcpy(node_aux->chaves[i], node_aux->chaves[i+1]);
                }
                adicionar_no_vetor(node_aux, node_aux->no_pai->chaves[posicao_no]);
                strcpy(node_aux->no_pai->chaves[posicao_no], node_aux->no_pai->p_filha[posicao_no+1]->chaves[0]);

                for(int i = 0; i < 4; i++) {
                    strcpy(node_aux->no_pai->p_filha[posicao_no+1]->chaves[i], node_aux->no_pai->p_filha[posicao_no+1]->chaves[i+1]);
                }
                node_aux->no_pai->p_filha[posicao_no+1]->total_chaves--;
            }
            else if(node_aux->no_pai->p_filha[posicao_no+1]->total_chaves == 2) {
                for(int i = aux; i < 4; i++) {
                    strcpy(node_aux->chaves[i], node_aux->chaves[i+1]);
                }
                adicionar_no_vetor(node_aux, node_aux->no_pai->chaves[0]);

                for(int i = 0; i < 4; i++) {
                    strcpy(node_aux->no_pai->chaves[i], node_aux->no_pai->chaves[i+1]);
                }
                adicionar_no_vetor(node_aux, node_aux->no_pai->p_filha[posicao_no+1]->chaves[0]);
                adicionar_no_vetor(node_aux, node_aux->no_pai->p_filha[posicao_no+1]->chaves[1]);

                for(int i = 1; i < 5; i++) {
                    node_aux->no_pai->p_filha[i] = node_aux->no_pai->p_filha[i+1];
                }
                node_aux->no_pai->total_chaves--;
                node_aux->total_chaves = 4;
            }
        }

        else if(posicao_no == node_aux->no_pai->total_chaves) {
            if(node_aux->no_pai->p_filha[posicao_no-1]->total_chaves > 2) { //RR
                for(int i = aux; i < 4; i++) {
                    strcpy(node_aux->chaves[i], node_aux->chaves[i+1]);
                }
                adicionar_no_vetor(node_aux, node_aux->no_pai->chaves[posicao_no-1]);

                if(node_aux->no_pai->p_filha[posicao_no-1]->total_chaves == 3) {
                    strcpy(node_aux->no_pai->chaves[posicao_no-1], node_aux->no_pai->p_filha[posicao_no-1]->chaves[2]);
                    strcpy(node_aux->no_pai->p_filha[posicao_no-1]->chaves[2], "*");
                }
                else {
                    strcpy(node_aux->no_pai->chaves[posicao_no-1], node_aux->no_pai->p_filha[posicao_no-1]->chaves[3]);
                    strcpy(node_aux->no_pai->p_filha[posicao_no-1]->chaves[3], "*");
                }
                node_aux->no_pai->p_filha[posicao_no-1]->total_chaves--;
            }
            else if(node_aux->no_pai->p_filha[posicao_no-1]->total_chaves == 2) {
                for(int i = aux; i < 4; i++) {
                    strcpy(node_aux->chaves[i], node_aux->chaves[i+1]);
                }
                adicionar_no_vetor(node_aux, node_aux->no_pai->chaves[0]);

                for(int i = 0; i < 4; i++) {
                    strcpy(node_aux->no_pai->chaves[i], node_aux->no_pai->chaves[i+1]);
                }
                adicionar_no_vetor(node_aux, node_aux->no_pai->p_filha[posicao_no-1]->chaves[0]);

                adicionar_no_vetor(node_aux, node_aux->no_pai->p_filha[posicao_no-1]->chaves[1]);

                for(int i = posicao_no; i < 5; i++) {
                    node_aux->no_pai->p_filha[i] = node_aux->no_pai->p_filha[i+1];
                }
                node_aux->no_pai->total_chaves--;
                node_aux->total_chaves = 4;
            }
        }
    }

    else {
        BTPAGE *a = node_aux;

        for(int i = aux; i < 4; i++) {
            strcpy(node_aux->chaves[i], node_aux->chaves[i+1]);
        }

        a = a->p_filha[aux];
        while(a->no_folha != true) {
            a = a->p_filha[a->total_chaves];
        }
        adicionar_no_vetor(node_aux, a->chaves[a->total_chaves-1]);

        a->chaves[a->total_chaves-1][4] = '*';
        removal(arvore, a->chaves[a->total_chaves-1]);
    }
    return 1;
}

void search(char *key) {
    FILE *file_dados;
    file_dados = fopen("produtos.dat", "r+");
    if(file_dados == NULL) {
        printf("Arquivo de dados não encontrado.\n");
    }

    else {
        int count = 0, j = 6;
        fpos_t position;
        char dados[200];

        fseek(file_dados, 0, SEEK_SET);
        fgetpos(file_dados, &position);
        while(fgets(dados, 192, file_dados) != NULL) {
            fsetpos(file_dados, &position);
            for(int i = 0; i < 5; i++) {
                if(dados[i] == key[i])
                    count++;
            }
            if(count == 5) {
                printf("Nome: ");
                while(dados[j] != '@') {
                    printf("%c", dados[j]);
                    j++;
                }
                j++;
                printf("\n");
                printf("Marca: ");
                while(dados[j] != '@') {
                    printf("%c", dados[j]);
                    j++;
                }
                j++;
                printf("\n");
                printf("Descrição: ");
                while(dados[j] != '@') {
                    printf("%c", dados[j]);
                    j++;
                }
                j++;
                printf("\n");
                printf("Ano: ");
                while(dados[j] != '@') {
                    printf("%c", dados[j]);
                    j++;
                }
                j++;
                printf("\n");
                printf("Preço: ");
                while(dados[j] != '@') {
                    printf("%c", dados[j]);
                    j++;
                }
                printf("\n");
                return;
            }
            fseek(file_dados, 192, SEEK_CUR);
            fgetpos(file_dados, &position);
            count = 0;
        }
        printf("Chave não encontrada.\n");
    }
} 

void desalocar(BTPAGE *node_aux) {

    if(node_aux == NULL)
        return;
    
    desalocar(node_aux->p_filha[0]);
    desalocar(node_aux->p_filha[1]);
    desalocar(node_aux->p_filha[2]);
    desalocar(node_aux->p_filha[3]);
    desalocar(node_aux->p_filha[4]);

    free(node_aux);
}

void listar_produtos(BTPAGE *node_aux) {
    int i = 0;

    if(node_aux == NULL)
        return;
    
    listar_produtos(node_aux->p_filha[0]);
    listar_produtos(node_aux->p_filha[1]);
    listar_produtos(node_aux->p_filha[2]);
    listar_produtos(node_aux->p_filha[3]);
    listar_produtos(node_aux->p_filha[4]);

    while(i < 5) {
        if(node_aux->chaves[i][0] == '*')
            break;
        else {
            printf("Chave: %s\n", node_aux->chaves[i]);
            search(node_aux->chaves[i]);
            printf("--------------------\n");
        }
        i++;
    }
}

int main() {
    BTREE *a = (BTREE *) malloc(4 * sizeof(BTREE));
    int comando;
    char aux[6], dados[200];
    FILE *file_dados;
    file_dados = fopen("produtos.dat", "w");
    fclose(file_dados);
    
    while(1) {
        printf("Digite:\n(1) para inserir um item\n(2) para remover um item\n(3) para buscar um item\n(4) para modificar o preço de um produto\n(5) para listar todos os produtos\n(0) para encerrar o programa\n");
        scanf("%d", &comando);

        if(comando == 1) {
            FILE *file_indices;
            file_indices = fopen("ibtree.idx", "w");
            fclose(file_indices);

            char nome[10], marca[15], descricao[100], ano[5], preco[7];
            strcpy(dados, "");

            printf("Digite os dados que quer inserir:\n");

            printf("nome: ");
            scanf("%s", nome);
            printf("marca: ");
            scanf("%s%*c", marca);
            printf("descrição: ");
            scanf("%[^\n]%*c", descricao);
            printf("ano: ");
            scanf("%s%*c", ano);
            printf("preço: ");
            scanf("%s", preco);
            for(int i = 0; i < 5; i++) {
                if(i < 3)
                    aux[i] = marca[i];
                else
                    aux[i] = ano[i-1];
            }

            insertion(a, aux);

            file_dados = fopen("produtos.dat", "a");
            if(file_dados == NULL) {
                printf("Arquivo de dados não encontrado.\n");
            }
            else {
                char *x = "@";
                char *y = "*";
                strncat(dados, aux, 5);
                strncat(dados, x, 1);
                strncat(dados, nome, strlen(nome));
                strncat(dados, x, 1);
                strncat(dados, marca, strlen(marca));
                strncat(dados, x, 1);
                strncat(dados, descricao, strlen(descricao));
                strncat(dados, x, 1);
                strncat(dados, ano, 4);
                strncat(dados, x, 1);
                strncat(dados, preco, strlen(preco));
                strncat(dados, x, 1);
                for(int i = strlen(dados); i < 192; i++)
                    strncat(dados, y, 1);

                fprintf(file_dados, "%s", dados);
                fclose(file_dados);
                strcpy(dados, "");
            }

            file_indices = fopen("ibtree.idx", "r+");
            if(file_indices == NULL) {
                printf("Arquivo de dados não encontrado.\n");
            }
            else {
                int count = 0, ajuda = 0;
                BTPAGE *node_aux = a->root;
                BTPAGE *node_aux1;
                while(node_aux != NULL) {
                    for(int i = 0; i < 4; i++) {
                        if(node_aux->p_filha[i] != NULL) {
                            fprintf(file_indices, "%d", i);
                        }
                        else {
                            fprintf(file_indices, "-1");
                        }
                        fprintf(file_indices, " <%s, %d> ", node_aux->chaves[i], posicao_chave(node_aux, node_aux->chaves[i]));
                    }
                    fprintf(file_indices, "-1\n");
                    if(node_aux->no_raiz == true) {
                    node_aux = node_aux->p_filha[count];
                    }
                    else {
                        count++;
                        node_aux1 = node_aux;
                        if(count != 5)
                            node_aux = node_aux->no_pai->p_filha[count];
                        if(count != 5 && node_aux == NULL) {
                            count = 0;
                            if(node_aux1->no_pai->no_raiz == true)
                                node_aux = node_aux1->no_pai->p_filha[ajuda]->p_filha[count];
                            else {
                                if(node_aux1->no_pai->no_pai->p_filha[ajuda] != NULL) {
                                    node_aux = node_aux1->no_pai->no_pai->p_filha[ajuda]->p_filha[count];
                                    if(node_aux == NULL)
                                        break;
                                }
                            }
                            ajuda++;
                            if(node_aux == NULL)
                                break;
                        }
                    }
                    if(count == 5) {
                        count = 0;
                        node_aux = node_aux->no_pai->p_filha[ajuda]->p_filha[count];
                        ajuda++;
                        if(node_aux == NULL)
                            break;
                    }
                    if(node_aux == NULL)
                        break;
                }
                fclose(file_indices);
            }
        }
        else if(comando == 2) {
            FILE *file_indices;
            file_indices = fopen("ibtree.idx", "w");
            fclose(file_indices);

            printf("Digite a chave que quer remover: ");
            scanf("%s", aux);
            int d = removal(a, aux);

            file_dados = fopen("produtos.dat", "r+");
            if(file_dados == NULL) {
                printf("Arquivo de dados não encontrado.\n");
            }
            else if(d == 0) {
                printf("Chave não encontrada.\n");
            }
            else {
                printf("Chave %s removida com sucesso!\n", aux);

                int count = 0;
                fpos_t position;
                char *x = "#-";

                fseek(file_dados, 0, SEEK_SET);
                fgetpos(file_dados, &position);
                while(fgets(dados, 192, file_dados) != NULL) {
                    fsetpos(file_dados, &position);
                    for(int i = 0; i < 5; i++) {
                        if(dados[i] == aux[i])
                            count++;
                    }
                    if(count == 5) {
                        fprintf(file_dados, "%s", x);
                        fclose(file_dados);
                        break;
                    }
                    fseek(file_dados, 192, SEEK_CUR);
                    fgetpos(file_dados, &position);
                    count = 0;
                }
            }

            file_indices = fopen("ibtree.idx", "r+");
            if(file_indices == NULL) {
                printf("Arquivo de dados não encontrado.\n");
            }
            else {
                int count = 0, ajuda = 0;
                BTPAGE *node_aux = a->root;
                BTPAGE *node_aux1;
                while(node_aux != NULL) {
                    for(int i = 0; i < 4; i++) {
                        if(node_aux->p_filha[i] != NULL) {
                            fprintf(file_indices, "%d", i);
                        }
                        else {
                            fprintf(file_indices, "-1");
                        }
                        fprintf(file_indices, " <%s, %d> ", node_aux->chaves[i], posicao_chave(node_aux, node_aux->chaves[i]));
                    }
                    fprintf(file_indices, "-1\n");
                    if(node_aux->no_raiz == true) {
                    node_aux = node_aux->p_filha[count];
                    }
                    else {
                        count++;
                        node_aux1 = node_aux;
                        if(count != 5)
                            node_aux = node_aux->no_pai->p_filha[count];
                        if(count != 5 && node_aux == NULL) {
                            count = 0;
                            if(node_aux1->no_pai->no_raiz == true)
                                node_aux = node_aux1->no_pai->p_filha[ajuda]->p_filha[count];
                            else {
                                if(node_aux1->no_pai->no_pai->p_filha[ajuda] != NULL) {
                                    node_aux = node_aux1->no_pai->no_pai->p_filha[ajuda]->p_filha[count];
                                    if(node_aux == NULL)
                                        break;
                                }
                            }
                            ajuda++;
                            if(node_aux == NULL)
                                break;
                        }
                    }
                    if(count == 5) {
                        count = 0;
                        node_aux = node_aux->no_pai->p_filha[ajuda]->p_filha[count];
                        ajuda++;
                        if(node_aux == NULL)
                            break;
                    }
                    if(node_aux == NULL)
                        break;
                }
                fclose(file_indices);
            }
        }
        else if(comando == 3) {
            printf("Digite a chave que quer buscar: ");
            scanf("%s", aux);
            search(aux);
        }
        else if(comando == 4) {
            char editar[6];
            char novo_preco[7];
            char comparar[2];

            printf("Digite a chave que quer editar o preço: ");
            scanf("%s", editar);

            file_dados = fopen("produtos.dat", "r+");
            if(file_dados == NULL) {
                printf("Arquivo de dados não encontrado.\n");
            }
            else {
                int count = 0;
                fpos_t position;

                fseek(file_dados, 0, SEEK_SET);
                fgetpos(file_dados, &position);
                while(fgets(dados, 192, file_dados) != NULL) {
                    fsetpos(file_dados, &position);
                    for(int i = 0; i < 5; i++) {
                        if(dados[i] == editar[i])
                            count++;
                    }
                    if(count == 5) {
                        count = 0;
                        while(count != 5) {
                            fgets(comparar, 2, file_dados);
                            if(comparar[0] == '@')
                                count++;
                        }
                        printf("Digite o novo preço: ");
                        scanf("%s", novo_preco);
                        fprintf(file_dados, "%s", novo_preco);
                        fprintf(file_dados, "@");
                        for(int i = 0; i < 3; i++)
                            fprintf(file_dados, "*");
                        printf("Preço alterado com sucesso!\n");
                        fclose(file_dados);
                        break;
                    }
                    fseek(file_dados, 192, SEEK_CUR);
                    fgetpos(file_dados, &position);
                    count = 0;
                }

            }
        }
        else if(comando == 5) {
            listar_produtos(a->root);
        }
        else if(comando == 0)
            break;
        else
            printf("Comando não reconhecido.\n");
    }
    
    desalocar(a->root);

    return 0;
}