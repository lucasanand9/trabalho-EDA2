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

        if (i < no->total && no->chaves[i] == chave){
            return 1; //encontrou
        }else{
            no = no->filhos[i];
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

void adicionaChave(ArvB * arvB, int chave){
    NoB * no = localizaNoB(arvB, chave);
    addChaveBRecursivo(arvB, no, NULL, chave);
}

void adicionaChaveNoB(NoB * no, NoB * direita, int chave){
    int i = pesquisaBinaria(no, chave);

    for (int j = no->total-1; j >= i; j++){
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

void addChaveBRecursivo(ArvB* arvore, NoB* no, NoB* novo, int chave) {
   adicionaChaveNoB(no, novo, chave);
    if (transbordoB(arvore, no)) {
        int promovido = no->chaves[arvore->ordem];
        NoB* novo = divideNoB(arvore, no);
        if (no->pai == NULL) {
            NoB* raiz = criaNo(arvore);
            raiz->filhos[0] = no;
            adicionaChaveNoB(no->pai, novo, promovido); // pq pai esta dando errado
            no->pai = raiz;
            novo->pai = raiz;
            arvore->raiz = raiz;
        } else {
            addChaveBRecursivo(arvore, no->pai, novo, promovido);
        }
    }
}


void removeChave(ArvB* arvB, int chave) {
    NoB* no = localizaNoB(arvB, chave);
    if (no == NULL) {
        return; // Chave não encontrada
    }

    int idx = pesquisaBinaria(no, chave);

    if (no->filhos[0] == NULL) {
        // Caso 1: Remoção em nó folha
        for (int i = idx; i < no->total - 1; i++) {
            no->chaves[i] = no->chaves[i + 1];
        }
        no->total--;

        if (no->pai != NULL && no->total < (arvB->ordem - 1) / 2) {
            ajustaSubfluxo(arvB, no);
        }
    } else {
        // Caso 2: Remoção em nó interno
        NoB* predecessor = no->filhos[idx];
        while (predecessor->filhos[predecessor->total] != NULL) {
            predecessor = predecessor->filhos[predecessor->total];
        }

        int chavePredecessor = predecessor->chaves[predecessor->total - 1];
        no->chaves[idx] = chavePredecessor;
        removeChave(arvB, chavePredecessor);
    }

    // Caso 3: Ajuste da raiz se necessário
    if (arvB->raiz->total == 0) {
        NoB* antigaRaiz = arvB->raiz;
        arvB->raiz = arvB->raiz->filhos[0];
        if (arvB->raiz != NULL) {
            arvB->raiz->pai = NULL;
        }
        free(antigaRaiz);
    }
}

void ajustaSubfluxo(ArvB* arvB, NoB* no) {
    NoB* pai = no->pai;
    int idxFilho = 0;

    while (pai->filhos[idxFilho] != no) {
        idxFilho++;
    }

    NoB* irmaoEsq = (idxFilho > 0) ? pai->filhos[idxFilho - 1] : NULL;
    NoB* irmaoDir = (idxFilho < pai->total) ? pai->filhos[idxFilho + 1] : NULL;

    if (irmaoEsq != NULL && irmaoEsq->total > (arvB->ordem - 1) / 2) {
        // Redistribuição: Pegar chave do irmão esquerdo
        for (int i = no->total; i > 0; i--) {
            no->chaves[i] = no->chaves[i - 1];
        }
        no->chaves[0] = pai->chaves[idxFilho - 1];
        no->filhos[no->total + 1] = no->filhos[no->total];
        for (int i = no->total; i > 0; i--) {
            no->filhos[i] = no->filhos[i - 1];
        }
        no->filhos[0] = irmaoEsq->filhos[irmaoEsq->total];
        pai->chaves[idxFilho - 1] = irmaoEsq->chaves[irmaoEsq->total - 1];
        irmaoEsq->total--;
        no->total++;
    } else if (irmaoDir != NULL && irmaoDir->total > (arvB->ordem - 1) / 2) {
        // Redistribuição: Pegar chave do irmão direito
        no->chaves[no->total] = pai->chaves[idxFilho];
        no->filhos[no->total + 1] = irmaoDir->filhos[0];
        pai->chaves[idxFilho] = irmaoDir->chaves[0];
        for (int i = 0; i < irmaoDir->total - 1; i++) {
            irmaoDir->chaves[i] = irmaoDir->chaves[i + 1];
            irmaoDir->filhos[i] = irmaoDir->filhos[i + 1];
        }
        irmaoDir->filhos[irmaoDir->total - 1] = irmaoDir->filhos[irmaoDir->total];
        irmaoDir->total--;
        no->total++;
    } else {
        // Fusão com irmão
        if (irmaoEsq != NULL) {
            for (int i = 0; i < no->total; i++) {
                irmaoEsq->chaves[irmaoEsq->total + i] = no->chaves[i];
                irmaoEsq->filhos[irmaoEsq->total + i + 1] = no->filhos[i];
            }
            irmaoEsq->total += no->total;
            irmaoEsq->filhos[irmaoEsq->total] = no->filhos[no->total];
            for (int i = idxFilho - 1; i < pai->total - 1; i++) {
                pai->chaves[i] = pai->chaves[i + 1];
                pai->filhos[i + 1] = pai->filhos[i + 2];
            }
            pai->total--;
            free(no);
        } else if (irmaoDir != NULL) {
            for (int i = 0; i < irmaoDir->total; i++) {
                no->chaves[no->total + i] = irmaoDir->chaves[i];
                no->filhos[no->total + i + 1] = irmaoDir->filhos[i];
            }
            no->chaves[no->total] = pai->chaves[idxFilho];
            no->total += irmaoDir->total + 1;
            for (int i = idxFilho; i < pai->total - 1; i++) {
                pai->chaves[i] = pai->chaves[i + 1];
                pai->filhos[i + 1] = pai->filhos[i + 2];
            }
            pai->total--;
            free(irmaoDir);
        }

        if (pai->pai == NULL && pai->total == 0) {
            arvB->raiz = no;
            no->pai = NULL;
            free(pai);
        }
    }
}
