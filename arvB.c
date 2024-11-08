#include "arvB.h"

ArvB * criaArvB(int ordem){
    ArvB *a = malloc(sizeof(ArvB));
    a->ordem = ordem;
    a->raiz = criaNo(a);
    
    return a;
}

NoB * criaNo(ArvB *arv){
    int max = arv->ordem * 2;
    NoB * no = malloc(sizeof(NoB));
    no->pai = NULL;
    no->chaves = malloc(sizeof(int) * (max + 1)); // quantidade max de chaves eh max+1
    no->filhos = malloc(sizeof(NoB) * (max +2)); // nao sei o pq a qtd max de filhos eh max+2 (acho q eh para ter uma sobra para dps balancear??)
    no->total = 0;
    for (int i = 0; i < max+2; i++){
        no->filhos[i] = NULL;  
    }
    return no;
}

void percorreArvB(NoB *no, void(visita)(int chave)){
    if (no != NULL){
        for (int i = 0; i < no->total; i++){
            percorreArvB(no->filhos[i], visita);
            visita(no->chaves[i]);
        }
        percorreArvB(no->filhos[no->total], visita);
    }
}

int localizaChave(ArvB * arv, int chave){
    NoB * no = arv->raiz;

    while (no != NULL){
        int i  = pesquisaBinaria(no, chave);

        if (i < no->total &&  no->chaves[i] == chave){
            return 1; //encontrou
        }else{
            no = no->filhos;
        }
    }
    return 0;
}

int pesquisaBinaria(NoB * no, int chave){
    int inicio = 0, fim = no->total-1, meio;
    while (inicio<=fim){
        meio = (inicio+fim)/2;
        if (no->chaves[meio] == chave){
            return meio; // encontrou
        } else if(no->chaves[meio] > chave){
            fim = meio -1;
        } else{
            inicio = meio +1;
        }
    }
    return -1; // n encontrou
}

void adicionaChaveNoB(NoB * no, NoB * direita, int chave){
    int i = pesquisaBinaria(no, chave);

    for (int j = no->total -1; j >= i; j++){
        no->chaves[j+1] = no->chaves[j];
        no->filhos[j+2] = no->filhos[j+1];
    }

    no->chaves[i] = chave;
    no->filhos[i+1] = direita;

    no->total++;
}

NoB * localizaNoB(ArvB* arvB, int chave) {
    NoB *noB = arvB->raiz;
    while (noB != NULL) {
        int i = pesquisaBinaria(noB, chave);
        if (noB->filhos[i] == NULL) {
            return noB;
        } else {
            noB = noB->filhos[i];
        }
        
        return NULL;
    }
}

int transbordoB (ArvB *arvB, NoB *noB) {
    return noB->total > arvB->ordem * 2;
}

NoB * divideNoB(ArvB * arv, NoB * no){
    int meio = no->total/2;
    NoB * novo = criaNo(arv);
    novo->pai = no->pai;
    for (int i = meio+1; i < no->total; i++){
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        if (novo->filhos[novo->total] != NULL){
            novo->filhos[novo->total] ->pai = novo;
        }
        novo->total++;
    }

    novo->filhos[novo->total] = no->filhos[no->total];
    if(novo->filhos[novo->total] != NULL){
        novo->filhos[novo->total]->pai = novo;
    }
    no->total= meio;
    return novo;
}

void addChaveBRecursivo(ArvB* arvB, NoB* noB, NoB* novo, int chave) {
    adicionaChaveNoB(noB, novo, chave);
    if (transbordoB(arvB, noB)) {
        int promovido = noB->chaves[arvB->ordem];
        NoB* novo = divideNo(arvB, noB);
        if (noB->pai == NULL) {
            NoB* raiz = criaNo(arvB);
            raiz->filhos[0] = noB;
            adicionaChaveNoB(pai, novo, promovido); // pq pai esta dando errado
            noB->pai = raiz;
            novo->pai = raiz;
            arvB->raiz = raiz;
               
        } else {
        addChaveBRecursivo(arvB, noB->pai, novo, promovido);
        }
    }
}