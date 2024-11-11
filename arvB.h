#include <stdlib.h>

typedef struct no{
    int total;
    int *chaves;
    struct no **filhos;
    struct no *pai;
}NoB;

typedef struct arvB{
    NoB *raiz;
    int ordem;
} ArvB;

/*
Operacoes:
    Add chave 
    Remove chave
    Localiza chave
    Percorrer árvore
*/ 


ArvB * criaArvB(int ordem);
NoB * criaNo(ArvB *arv);
void percorreArvB(NoB *no, void(visita)(int chave)); // o qu eesse "void(visita)(int chave)"
int localizaChave(ArvB * arv, int chave);
int pesquisaBinaria(NoB * no, int chave);
void adicionaChaveNoB(NoB * no, NoB * direita, int chave);
NoB * localizaNoB(ArvB* arvB, int chave);
NoB * divideNoB(ArvB * arv, NoB * no);
int transbordoB (ArvB *arvB, NoB *noB);
void addChaveBRecursivo(ArvB* arvB, NoB* noB, NoB* novo, int chave);
void removeChave(ArvB* arvB, int chave);