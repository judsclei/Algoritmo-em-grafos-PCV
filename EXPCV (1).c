#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0

struct matriz {   //matriz de adjacencias
    int numero_elementos; //numeros de elementos
    int** elementos; //elementos ou conteudo dos elementos
};

struct nodo {     //vertice
    int indice;  //numero dos vertices
    int valor;   //valor da aresta
};

void ler_arquivo(struct matriz*, char*); // le e carrega arquivo com matriz de vertices e arestas e povoa a matriz de adjacencia
int calcular_custo(struct matriz, int*); // calcula o custo da soluçao
void construir_caminho(struct matriz, int*); //constroi caminho de maneira "gulosa" utilizando a heuristica vizinho mais proximo

void realizar_movimento_troca(struct matriz, int*, int*); //metodo que faz a troca da soluçao por uma menor
void copiar_caminho(struct matriz, int*, int*); //procedimento auxiliar que coipia a suluçao gerada e compara a fim de encontrar a menor possivel

void imprimir_caminho(int, int*);

void imprimir_matriz(struct matriz);
void linha();

int main(int argc, char *argv[]) {
    struct matriz m;

    ler_arquivo(&m, "M1.txt"); //funçao q la ariquivo com matriz no arquivo esta contido primeiro o numero de vertices (elementos) e a matriz com as arestas
    imprimir_matriz(m);

    int *solucao_inicial = malloc((m.numero_elementos + 1) * sizeof(int));
    construir_caminho(m, solucao_inicial);
    printf("Solucao inicial gulosa vertices: ");
    imprimir_caminho(m.numero_elementos + 1, solucao_inicial);
    int custo_solucao_inicial = calcular_custo(m, solucao_inicial);
    printf("Custo solucao inicial gulosa: %d em metros\n", custo_solucao_inicial);

    int* melhor_vizinho = malloc((m.numero_elementos + 1) * sizeof(int));
    realizar_movimento_troca(m, solucao_inicial, melhor_vizinho);
    printf("Solucao melhor vizinho vertices: ");
    imprimir_caminho(m.numero_elementos + 1, melhor_vizinho);
    int custo_solucao_melhor_vizinho = calcular_custo(m, melhor_vizinho);
    printf("Custo solucao melhor vizinho: %d em metros\n", custo_solucao_melhor_vizinho);
}

//-----------------------------------------------------------------------------

void copiar_caminho(struct matriz m, int* origem, int* destino) {
    int i;

    for(i = 0; i <= m.numero_elementos; i++) {
        destino[i] = origem[i];
    }
}

//-----------------------------------------------------------------------------

void realizar_movimento_troca(struct matriz m, int* solucao_inicial, int* melhor_vizinho) {
    int* solucao_tmp = malloc((m.numero_elementos + 1) * sizeof(int));
    int custo_referencia = calcular_custo(m, solucao_inicial);

    copiar_caminho(m, solucao_inicial, melhor_vizinho);

    for(int i = 1; i < m.numero_elementos; i++) {
        copiar_caminho(m, solucao_inicial, solucao_tmp);

        for(int j = i + 1; j < m.numero_elementos; j++) {

            int tmp = solucao_tmp[i];
            solucao_tmp[i] = solucao_tmp[j];
            solucao_tmp[j] = tmp;

            int custo_solucao_tmp = calcular_custo(m, solucao_tmp);

            if(custo_solucao_tmp < custo_referencia) {
                custo_referencia = custo_solucao_tmp;
                copiar_caminho(m, solucao_tmp, melhor_vizinho);
            }
        }
    }
}

//-----------------------------------------------------------------------------

void ler_arquivo(struct matriz* m, char* arquivo) {
    FILE* fp = fopen(arquivo, "r");

    fscanf(fp, "%d\n", &m->numero_elementos);

    m->elementos = malloc(m->numero_elementos * sizeof(int*));

    for(int i = 0; i < m->numero_elementos; i++) {
        m->elementos[i] = malloc(m->numero_elementos * sizeof(int));
        for(int j = 0; j < m->numero_elementos; j++) {
            fscanf(fp, "%d ", &m->elementos[i][j]);
        }
    }


    fclose(fp);
}

//-----------------------------------------------------------------------------

int calcular_custo(struct matriz m, int* caminho) { //calcula ocusto das arestas
    int custo = 0;

    for(int i = 0; i < m.numero_elementos; i++) {
        custo = custo + m.elementos[caminho[i]][caminho[i + 1]]; //soma o valor do nodo da posiçao atual com o proximo
    }

    return custo; //retorna o custo apos todas as interaçoes
}

//-----------------------------------------------------------------------------

void construir_caminho(struct matriz m, int* caminho) {
    int *inseridos = malloc(m.numero_elementos * sizeof(int));//inicializa vetor de inseridos que vai receber os vertices ja analizados

    for(int i = 0; i < m.numero_elementos; i++) {
        inseridos[i] = FALSE;
    }

    caminho[0] = 0; //institui o vertice inicial ou seja o primeiro ser iniciado
    inseridos[0] = TRUE; //insere o vertice inicial no vetor de analizados

    for(int i = 0; i < m.numero_elementos; i++) { //a naliza os demais vertices e constroi caminho
        int valor_referencia = INT_MAX; //cria valor de referencia que vai separas ordenar achando assim as aresta e vertices de menor peso
        int vizinho_selecionado = caminho[0];

        for(int j = 0; j < m.numero_elementos; j++) {
            if(!inseridos[j] && valor_referencia > m.elementos[i][j]) {//verifica se o vertice ja foi analizado e esta no vetor de inseridos
                vizinho_selecionado = j; //se vertice  foi selecionado
                valor_referencia = m.elementos[i][j];
            }
        }

        caminho[i + 1] = vizinho_selecionado;
        inseridos[vizinho_selecionado] = TRUE;//isere vertice ja analizado no vetor de inseridos
    }

    caminho[m.numero_elementos] = caminho[0]; //retorna ao vertice inicial depois de ter analizado todos os outros e tercriado o caminho

    free(inseridos);
}



//-----------------------------------------------------------------------------


void imprimir_matriz(struct matriz m) {
    linha();
    printf("Matriz\n\n");

    for(int i = 0; i < m.numero_elementos; i++) {
        for(int j = 0; j < m.numero_elementos; j++) {
            printf("%d ", m.elementos[i][j]);
        }

        printf("\n");
    }

    linha();
}

//-----------------------------------------------------------------------------

void imprimir_caminho(int n, int* caminho) {
    int i;

    for(i = 0; i < n; i++) {
        printf("%d ", caminho[i]);
    }
    printf("\n");
}

//-----------------------------------------------------------------------------

void linha() {
    int i;
    printf("\n");
    for(i = 0; i < 80; i++) printf("_");
    printf("\n");
}
