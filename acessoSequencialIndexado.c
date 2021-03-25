#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ITENSPAGINA 4

// Estrutura do registro que está nos arquivos
typedef struct
{
    int chave;
    long dado1;
    char dado2[500];
} registro;

int pesquisaSequencialIndexado(int indiceDePaginas[], int qtdPaginas, registro *item, FILE *arq, int *numeroLeituras, int *numeroDeComparacoesEntreChaves, int situacao)
{
    registro *pagina = malloc(sizeof(registro) * ITENSPAGINA); // Pagina com a quatidade de registros
    int i, qtdItens;
    long desloc;
    i = 0;

    // Procura a pagina que está o chave e retorna o numero da proxima pagina.
    if (situacao == 1)
    {
        while (i < qtdPaginas && indiceDePaginas[i] <= item->chave)
        {
            i++;
        }
        printf("\nIndice da pagina onde o valor pode estar: %d", i - 1);
    }
    else if (situacao == 2)
    {
        while (i < qtdPaginas && indiceDePaginas[i] >= item->chave)
        {
            i++;
        }
        printf("\nIndice da pagina onde o valor pode estar: %d", i - 1);
    }
    else if (situacao == 3)
    {
        fseek(arq, 0, SEEK_SET);
        // pesquisa sequencial na página lida
        while (fread(pagina, sizeof(registro), 1, arq) == 1)
        {
            (*numeroLeituras)++;
            (*numeroDeComparacoesEntreChaves)++;
            if (pagina[i].chave == item->chave)
            {
                *item = pagina[i];
                return 1;
            }
        }
    }

    if ((i == 0 && situacao == 1) || (i == 0 && situacao == 2))
    {
        return 0;
    }
    else
    {
        // a ultima página pode não estar completa
        if (i < qtdPaginas)
        {
            qtdItens = ITENSPAGINA;
            printf("\nQuantidade de itens na pagina: %d", qtdItens);
        }
        else
        {
            fseek(arq, 0, SEEK_END);
            if ((ftell(arq) / sizeof(Registro)) % ITENSPAGINA == 0)
            {
                qtdItens = ITENSPAGINA;
            }
            else
            {
                qtdItens = (ftell(arq) / sizeof(registro)) % ITENSPAGINA;
            }
        }
        if (situacao == 1 || situacao == 2)
        {
            desloc = (i - 1) * ITENSPAGINA * sizeof(registro);
            printf("\nDeslocamento no arquivo: %ld", desloc);
            fseek(arq, desloc, SEEK_SET);
            fread(pagina, sizeof(registro), qtdItens, arq);
            (*numeroLeituras)++;
            // pesquisa sequencial na página lida
            for (i = 0; i < qtdItens; i++)
            {
                (*numeroDeComparacoesEntreChaves)++;
                if (pagina[i].chave == item->chave)
                {
                    *item = pagina[i];
                    return 1;
                }
            }
        }
        return 0;
    }
}

int AcessoSequencialIndexado(FILE *arq, int chavePesquisa, int maxTabela, clock_t *tempoTotalDaPesquisa, int *numeroDeLeituras, int *numeroDeComparacoesEntreChaves, int situacao)
{
    // Volta o ponteiro do arquivo para a posição inicial
    rewind(arq);

    // Vetor de inteiros para respresentar a tabela de indices
    int *indiceDePaginas = malloc(sizeof(int) * maxTabela);

    // Registro utilizado para realizar a leitura do arquivo e efetuar a pesquisa
    registro *reg = malloc(sizeof(registro) * 1);

    int qtdPaginas = 0, qtdRegistros = 0;

    // Efetuando a leitura do arquivo e criando a tabela de indices com as chaves dos registros
    while (fread(reg, sizeof(registro), 1, arq) == 1)
    {
        (*numeroDeLeituras)++;
        qtdRegistros++;
        if (qtdRegistros % ITENSPAGINA == 1)
        {
            indiceDePaginas[qtdPaginas] = reg->chave;
            // printf("\nA pagina de numero %d comeca com a chave de valor: %d", qtdPaginas, reg->chave);
            qtdPaginas++;
        }
    }

    // Setando a chave de pesquisa no registro que será passado para a função de pesquisa
    reg->chave = chavePesquisa;

    // Parte responsável por realizar a pesquisa e calcular o tempo gasto para efetuar a mesma
    clock_t tempoInicialDaPesquisa = clock();
    (*tempoTotalDaPesquisa) = clock();
    if (pesquisaSequencialIndexado(indiceDePaginas, qtdPaginas, reg, arq, numeroDeLeituras, numeroDeComparacoesEntreChaves, situacao))
    {
        printf("\n\nLivro de nome [%s] e codigo [%d] foi localizado!\n", reg->dado2, reg->chave);
        (*tempoTotalDaPesquisa) = clock() - tempoInicialDaPesquisa;
        return 1;
    }
    else
    {
        printf("\n\nLivro de codigo %d nao foi localizado\n", reg->chave);
        (*tempoTotalDaPesquisa) = clock() - tempoInicialDaPesquisa;
        return 0;
    }

    return 0;
}