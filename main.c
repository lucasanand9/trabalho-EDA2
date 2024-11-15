#include <stdio.h>
#include <stdlib.h>
#include "arvB.h"

// Função para imprimir uma chave (usada em percorreArvB)
void imprimeChave(int chave) {
    printf("%d ", chave);
}

int main() {
    // Cria a Árvore B com ordem 3
    int ordem = 3;
    ArvB* arvore = criaArvB(ordem);
    if (arvore == NULL) {
        printf("Erro ao criar a Árvore B.\n");
        return 1;
    }

    printf("Árvore B criada com ordem %d.\n", ordem);

    // Inserindo chaves na árvore
    adicionaChave(arvore, 10);
    adicionaChave(arvore, 20);
    adicionaChave(arvore, 5);
    adicionaChave(arvore, 6);
    adicionaChave(arvore, 12);
    adicionaChave(arvore, 30);
    adicionaChave(arvore, 7);
    adicionaChave(arvore, 17);

    // Percorrendo e exibindo a árvore após inserções
    printf("Árvore após inserções (percorrendo):\n");
    percorreArvB(arvore->raiz, imprimeChave);
    printf("\n");

    // Removendo chaves da árvore
    int chavesParaRemover[] = {6, 13, 7, 4, 10};
    int nRemocoes = sizeof(chavesParaRemover) / sizeof(chavesParaRemover[0]);
    printf("Removendo chaves: ");
    for (int i = 0; i < nRemocoes; i++) {
        printf("%d ", chavesParaRemover[i]);
        removeChave(arvore, chavesParaRemover[i]);
    }
    printf("\n");

    // Percorrendo e exibindo a árvore após remoções
    printf("Árvore após remoções (percorrendo):\n");
    percorreArvB(arvore->raiz, imprimeChave);
    printf("\n");

    printf("Programa finalizado.\n");

    return 0;
}