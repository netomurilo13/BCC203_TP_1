#include <stdio.h>
#include <stdlib.h>
#include <string.h>
clock_t tempo;
long int comparacoes = 0;
long int numeroLeituras = 0;
#define CADEIA_DE_CARACTERES 500
#define ITENSPAGINA 4

typedef struct conteudo
{
    //Estrutura de conteudo.
    int chave;
    long conteudo;
    char conteudo2[CADEIA_DE_CARACTERES];
} Conteudo;

typedef struct conteudoArvore
{
    //Estrutura que representa o nó da arvore.
    long int filhoEsq, filhoDir;
    Conteudo *conteudo;
} ConteudoArvore;
//Estrutura que será retornada na função de busca.
typedef struct tipoRetorno
{
    //Posição do pai do nó pesquisado. Será utilizada para alterar o valor dos apontadores.
    int posPai;
    //Essa variavel receberá a posição do objeto pesquisado.
    int objetoPesquisado;
    //Essa variavel recebe o pai do objeto pesquisado. Será usada para ser reescrita no arquivo com os valores novos dos apontadores.
    ConteudoArvore *pai;
} TipoRetorno;

TipoRetorno *pesquisaNaArvore(FILE *arquivoArvore, int chaveBuscada, int quantidade)
{
    ConteudoArvore *aux = malloc(sizeof(ConteudoArvore));
    /*Variavel que será retornada
     Caso essa função seja chamada pela função insere, a variavel de retorno não possuirá valor no campo "objetoPesquisado",
     exceto se ja existir um dado de valor igual na arvore.
    */
    TipoRetorno *retorno = malloc(sizeof(TipoRetorno));
    //Atribuindo valor -1 ao objetoPesquisado. Esse valor será substituido caso o objeto seja encontrado na arvore.
    //Essa substituição é necessaria para diferenciar o retorno da função nos casos de busca para pesquisa, e busca para inserção.
    retorno->objetoPesquisado = -1;
    rewind(arquivoArvore);
    //Variavel que retornará a posição do pai do nó que será inserido, ou posição em que o ponteiro do arquivo se encontrava em uma interação anterior.
    //Ela começa com o valor 0, referente á primeira posição do arquivo.
    int posPai = ftell(arquivoArvore);
    for (int i = 0; i < quantidade; i++)
    {
        if (fread(aux, sizeof(ConteudoArvore), 1, arquivoArvore))
        {
            numeroLeituras++;
            //Verificando se o valor analisado é o procurado.
            if (chaveBuscada == aux->conteudo->chave)
            {
                comparacoes++;
                /*Atribuindo o pai do nó ao objeto retornado. Esse retorno será importante para substituir 
                  o valor de seus apontadores para a posição do novo nó que sera inserido;
                */
                retorno->pai = aux;
                /*Atribuindo ao retorno a posição do pai do nó que será inserido. Essa atribuição é importante,
                pois será necessário reescrever sobre ela, o pai do nó inserido com os valores de seus apontadores alterados.
                */

                retorno->posPai = posPai;
                /*Quando um objeto é encontrado, o ponteiro do arquivo se encontra em uma posição a frente da do item buscado.
                  Nesse caso, atribui à variavel objetoPesquisado, o valor da variavel posição do pai, que ira ter exatamente o valor do item
                  que estamos buscando.
                */
                retorno->objetoPesquisado = posPai;
                //Retornando objeto encontrado;
                return retorno;
            }
            else if (chaveBuscada > aux->conteudo->chave)
            {
                comparacoes++;
                retorno->pai = aux;
                retorno->posPai = posPai;
                //Verifica se o item atual, possui espaço para inserir um filho à direita.
                if (aux->filhoDir == -1)
                {
                    //Retorna os valores necessarios para inserir o novo nó.
                    return retorno;
                }
                //Caso não possua espaço, o ponteiro do arquivo avança para o proximo nodo da arvore para fazer as mesmas verificações.
                rewind(arquivoArvore);
                fseek(arquivoArvore, aux->filhoDir, SEEK_CUR);
            }
            else if (chaveBuscada < aux->conteudo->chave)
            {
                comparacoes++;
                retorno->pai = aux;
                retorno->posPai = posPai;
                //Verifica se o item atual, possui espaço para inserir um filho à esquerda.
                if (aux->filhoEsq == -1)
                {
                    return retorno;
                }
                //Caso não possua espaço, o ponteiro do arquivo avança para o proximo nodo da arvore para fazer as mesmas verificações.
                rewind(arquivoArvore);
                fseek(arquivoArvore, aux->filhoEsq, SEEK_CUR);
            }
            //Salvando posição atual do cursor na variavel posPai.
            posPai = ftell(arquivoArvore);
        }
        else
        {
            i = quantidade;
        }
    }
    return NULL;
}

void insereNoArvore(FILE *arquivoArvore, ConteudoArvore *noArvore, int quantidade)
{
    //Variavel do tipo retorno que recebera os valores retornados pela função pesquisaNaArvore
    //Como essa é uma função de inserção, os valores retornados aqui serão apenas a posicão do PAI do nó que sera inserido, e o PAI do nó que será inserido
    TipoRetorno *resultBusca = pesquisaNaArvore(arquivoArvore, noArvore->conteudo->chave, quantidade);
    //Condições para verificar se o dado é maior ou menor que o pai, para saber qual apontador do pai(Esquerdo ou direito), deve ser substituido pela posição do nó que está sendo inserido
    if ((noArvore->conteudo->chave) > (resultBusca->pai->conteudo->chave))
    {
        rewind(arquivoArvore);
        fseek(arquivoArvore, ftell(arquivoArvore), SEEK_END);
        resultBusca->pai->filhoDir = ftell(arquivoArvore);
        fwrite(noArvore, sizeof(ConteudoArvore), 1, arquivoArvore);
        rewind(arquivoArvore);
        fseek(arquivoArvore, resultBusca->posPai, SEEK_CUR);
        fwrite(resultBusca->pai, sizeof(ConteudoArvore), 1, arquivoArvore);
    }
    if ((noArvore->conteudo->chave) < (resultBusca->pai->conteudo->chave))
    {
        rewind(arquivoArvore);
        fseek(arquivoArvore, ftell(arquivoArvore), SEEK_END);
        resultBusca->pai->filhoEsq = ftell(arquivoArvore);
        fwrite(noArvore, sizeof(ConteudoArvore), 1, arquivoArvore);
        rewind(arquivoArvore);
        fseek(arquivoArvore, resultBusca->posPai, SEEK_CUR);
        fwrite(resultBusca->pai, sizeof(ConteudoArvore), 1, arquivoArvore);
    }
}

void buscaItem(FILE *arquivoArvore, int chaveBuscada, int quantidade)
{
    //Variavel que possuirá o retorno da função de busca.
    TipoRetorno *resultado = pesquisaNaArvore(arquivoArvore, chaveBuscada, quantidade);
    ConteudoArvore *aux = malloc(sizeof(ConteudoArvore));
    //Caso a função retorno -1 no objetoBuscado, significa que o objeto não existe na arvore.
    if (resultado->objetoPesquisado == -1)
    {
        printf("Dado nao existe ano arquivo!\n");
    }
    //Lendo o arquivo na posição que se encontra com o dado desejado e imprimindo valores.
    else
    {
        rewind(arquivoArvore);
        fseek(arquivoArvore, resultado->objetoPesquisado, SEEK_CUR);
        fread(aux, sizeof(ConteudoArvore), 1, arquivoArvore);
    }
}

void ArvoreBinaria(FILE *arq, int chavePesquisa, int quantidade)
{
    rewind(arq);
    Conteudo *conteudo = malloc(sizeof(Conteudo));
    ConteudoArvore *conteudoArvore = malloc(sizeof(ConteudoArvore));
    fread(conteudo, sizeof(Conteudo), 1, arq);
    conteudoArvore->conteudo = conteudo;
    conteudoArvore->filhoDir = -1;
    conteudoArvore->filhoEsq = -1;
    FILE *arquivoArvore = fopen("arquivoArvore.bin", "w+b");
    //PRENCHENDO PRIMEIRA POSIÇÃO DA ARVORE COM O DADO RAIZ
    fwrite(conteudoArvore, sizeof(ConteudoArvore), 1, arquivoArvore);
    //Laço que chama a função para inserir os nós na arvore
    //Optei por fazer de maneira interativa
    for (int i = 1; i < quantidade; i++)
    {
        Conteudo *conteudo = malloc(sizeof(Conteudo));
        ConteudoArvore *conteudoArvore = malloc(sizeof(ConteudoArvore));
        //Condição para garantir que a função inserção só seja executada enquanto o arquivo binario possuir dados não lidos
        //Essa condição nao era necessaria, pois o laço só permite a leitura até o tamanho final do arquivo, mas eu coloquei por garantia rs
        if (fread(conteudo, sizeof(Conteudo), 1, arq))
        {
            conteudoArvore->conteudo = conteudo;
            conteudoArvore->filhoDir = -1;
            conteudoArvore->filhoEsq = -1;
            //Função de inserção
            insereNoArvore(arquivoArvore, conteudoArvore, quantidade);
        }
        else
        {
            i = quantidade;
        }
    }
    numeroLeituras = 0;
    comparacoes = 0;
    tempo = clock();
    buscaItem(arquivoArvore, chavePesquisa, quantidade);
    printf("\n----------------------------------------------------------------------------\n");
    printf("\nNumero total de leituras na memoria secundaia: %ld vezes!\n", numeroLeituras);
    printf("\nNumero total de comparacoes entre chaves: %ld vezes!\n", comparacoes);
    printf("\nTempo gasto para realizar a pesquisa: %.4lf segundos!\n", ((double)tempo) / CLOCKS_PER_SEC);
    printf("\n----------------------------------------------------------------------------\n");
    fclose(arquivoArvore);
}
