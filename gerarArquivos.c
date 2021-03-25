#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int chave;
    long dado1;
    char dado2[500];
} Registro;

FILE *ascendente(char *nome, int size)
{
    FILE *arq;
    char livro[7] = "Livro ";
    char *codigoEmChar = malloc(sizeof(char));

    Registro *reg = malloc(sizeof(Registro) * 1);
    reg->chave = 0;
    reg->dado1 = 0;
    strcpy(reg->dado2, livro);
    itoa(reg->chave, codigoEmChar, 10);
    strcpy(reg->dado2, strcat(reg->dado2, codigoEmChar));

    if ((arq = fopen(nome, "w+b")) == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        return 0;
    }

    for (int i = 0; i < size; i++)
    {
        reg->chave = i + 1;
        reg->dado1 = reg->chave + 1;
        strcpy(reg->dado2, livro);
        itoa(reg->chave, codigoEmChar, 10);
        strcpy(reg->dado2, strcat(reg->dado2, codigoEmChar));
        fwrite(reg, sizeof(Registro), 1, arq);
    }

    return arq;
}

FILE *decrescente(char *nome, int size)
{
    FILE *arq;
    char livro[7] = "Livro ";
    char *codigoEmChar = malloc(sizeof(char));

    Registro *reg = malloc(sizeof(Registro) * 1);
    reg->chave = 0;
    reg->dado1 = 0;
    strcpy(reg->dado2, livro);
    itoa(reg->chave, codigoEmChar, 10);
    strcpy(reg->dado2, strcat(reg->dado2, codigoEmChar));

    if ((arq = fopen(nome, "w+b")) == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        return 0;
    }

    for (int i = size; i > 0; i--)
    {
        reg->chave = i;
        reg->dado1 = reg->chave + 1;
        strcpy(reg->dado2, livro);
        itoa(reg->chave, codigoEmChar, 10);
        strcpy(reg->dado2, strcat(reg->dado2, codigoEmChar));
        fwrite(reg, sizeof(Registro), 1, arq);
    }

    return arq;
}

// Algoritmo de Fisher-Yates para embaralhar um array
void shuffle(int *array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
}

FILE *aleatorio(char *nome, int size)
{
    FILE *arq;
    char livro[7] = "Livro ";
    char *codigoEmChar = malloc(sizeof(char));

    Registro *reg = malloc(sizeof(Registro) * 1);
    reg->chave = 0;
    reg->dado1 = 0;
    strcpy(reg->dado2, livro);
    itoa(reg->chave, codigoEmChar, 10);
    strcpy(reg->dado2, strcat(reg->dado2, codigoEmChar));

    if ((arq = fopen(nome, "w+b")) == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        return 0;
    }

    // Criando um vetor de numero e embaralhando para gerar um vetor randomico
    int *numeros = malloc(size * sizeof *numeros);
    for (int k = 0; k < size; k++)
    {
        numeros[k] = k+1;
    }
    shuffle(numeros, size);

    for (int i = 0; i < size; i++)
    {
        reg->chave = numeros[i];
        reg->dado1 = numeros[i] + 1;
        strcpy(reg->dado2, livro);
        itoa(reg->chave, codigoEmChar, 10);
        strcpy(reg->dado2, strcat(reg->dado2, codigoEmChar));
        fwrite(reg, sizeof(Registro), 1, arq);
    }

    return arq;
}

FILE *gerarArquivo(int opcao, char *nome, int tamanho)
{
    switch (opcao)
    {
    case 1:
        return ascendente(nome, tamanho);
        break;
    case 2:
        return decrescente(nome, tamanho);
        break;
    case 3:
        return aleatorio(nome, tamanho);
        break;
    }
    return NULL;
}