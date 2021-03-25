#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gerarArquivos.c"
#include "acessoSequencialIndexado.c"
#include "arvoreBinaria.c"
#include "arvoreB.c"
#include "arvoreBestrela.c"

int main(int argc, char *argv[])
{
    // 1 = Sequencial Indexada, 2 = Arvore Binaria, 3 = Arvore B, 4 = Arvore B*
    int metodo = atoi(argv[1]);
    // Quantidade de registros do arquivo
    int quantidade = atoi(argv[2]);
    // Situação de ordem do arquivo, 1 = Ascendente, 2 = Decrescente, 3 = Aleatorio
    int situacao = atoi(argv[3]);
    // Chave a ser pesquisada no arquivo
    int chave = atoi(argv[4]);

    // Variável responsavel para receber o arquivo gerado
    FILE *arq;
    if (quantidade > 0 && (situacao >= 1 && situacao <= 3) && (metodo >= 1 && metodo <= 4))
    {
        clock_t inicioCriacaoDoArquivo = clock();
        arq = gerarArquivo(situacao, "registros.bin", quantidade);
        clock_t tempoTotalParaCriarArquivo = clock() - inicioCriacaoDoArquivo;
        printf("\n---------------------------------------------------------------\n");
        printf("\nTempo usado para criar o arquivo de registros: %.4lf segundos!\n", ((double)tempoTotalParaCriarArquivo) / CLOCKS_PER_SEC);
        printf("\n---------------------------------------------------------------\n\n");
    }
    else if (metodo < 1 || metodo > 4)
    {
        printf("Informe um método válido!\n");
        return 0;
    }
    else if (quantidade <= 0)
    {
        printf("Informe uma quantide de registros valida!\n");
        return 0;
    }
    else if (situacao < 1 || situacao > 3)
    {
        printf("Informe uma situacao valida!\n");
        return 0;
    }

    // Atraves do primeiro paramento da linha comando é selecionado qual metodo executar
    switch (metodo)
    {
    case 1:
        printf("> Pesquisa Sequencial Indexada - Chave pesquisada: %d\n", chave);
        clock_t tempoTotalDaPesquisa;
        int numeroDeLeituras = 0;
        int numeroDeComparacoesEntreChaves = 0;
        AcessoSequencialIndexado(arq, chave, quantidade, &tempoTotalDaPesquisa, &numeroDeLeituras, &numeroDeComparacoesEntreChaves, situacao);
        printf("\n----------------------------------------------------------------------------\n");
        printf("\nNumero total de leituras na memoria secundaria: %d vezes!\n", numeroDeLeituras);
        printf("\nNumero total de comparacoes entre as chaves: %d vezes!\n", numeroDeComparacoesEntreChaves);
        printf("\nTempo gasto para realizar a pesquisa: %.4lf segundos!\n", ((double)tempoTotalDaPesquisa) / CLOCKS_PER_SEC);
        printf("\n----------------------------------------------------------------------------\n");
        break;
    case 2:
        printf("> Pesquisa Arvore Binaria - Chave pesquisada: %d\n", chave);
        ArvoreBinaria(arq, chave, quantidade);
        break;
    case 3:
        printf("> Pesquisa Arvore B - Chave pesquisada: %d\n", chave);
        ArvoreB(arq, chave);
        break;
    case 4:
        printf("> Pesquisa Arvore B* - Chave pesquisada: %d\n", chave);
        ArvoreBestrela(arq, chave);
        break;
    }

    printf("\n");

    // Fecha o arquivo que foi aberto dentro da função 'gerarArquivo'
    fclose(arq);

    return 0;
}
