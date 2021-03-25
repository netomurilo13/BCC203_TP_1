#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MM 2 * M
#define M 2
const int TAM_REG_DADOS = 64;
const int T_CHAVE = 9;
int contadordecomparar = 0;

typedef struct Tiporegistro
{
    int Chave;
    long dado1;
    char dado2[500];
} TipoRegistro;

typedef struct TipoPagina *TipoApontador;

typedef struct TipoPagina
{
    short n;
    TipoRegistro r[MM];
    TipoApontador p[MM + 1];
    bool folha;
} TipoPagina;

void Inicializa(TipoApontador *Arvore)
{
    *Arvore = NULL;
}
// função que inicializa meu dicionario/arvore

TipoRegistro *Pesquisa(TipoRegistro *x, TipoApontador *Ap)
{
    long i = 1;

    contadordecomparar++;
    if (*Ap == NULL)
    {
        printf("TipoRegistro nao esta na arvore \n");
        return NULL;
    }
    while (i < (*Ap)->n && x->Chave > (*Ap)->r[i - 1].Chave)
    {
        contadordecomparar++;
        i++;
    }
    contadordecomparar++;
    if (x->Chave == (*Ap)->r[i - 1].Chave)
    {
        *x = (*Ap)->r[i - 1];
        return x;
    }
    contadordecomparar++;
    if (x->Chave < (*Ap)->r[i - 1].Chave)
        Pesquisa(x, &(*Ap)->p[i - 1]);
    else
    {
        Pesquisa(x, &(*Ap)->p[i]);
    }
}
// funçao recursiva que faz uma busca, comparando as chaves até retornar a chave desejada

void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir)
{
    short NaoAchouPosicao;
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);
    while (NaoAchouPosicao)
    {
        if (Reg.Chave >= Ap->r[k - 1].Chave)
        {
            NaoAchouPosicao = false;
            break;
        }
        Ap->r[k] = Ap->r[k - 1];
        Ap->p[k + 1] = Ap->p[k];
        k--;
        if (k < 1)
            NaoAchouPosicao = false;
    }
    Ap->r[k] = Reg;
    Ap->p[k + 1] = ApDir;
    Ap->n++;
}

void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno)
{
    long i = 1;
    long j;
    TipoApontador ApTemp;
    // Serve para verificar se a arvore esta vazia ou quando atingir o no folha
    if (Ap == NULL)
    {
        *Cresceu = true;
        (*RegRetorno) = Reg;
        (*ApRetorno) = NULL;
        return;
    }
    // realizar uma pesquisa
    while (i < Ap->n && Reg.Chave > Ap->r[i - 1].Chave)
    {
        i++;
    }

    // considerando que as chaves são unicas
    // porem no aleatorio entra muito nessa condição
    if (Reg.Chave == Ap->r[i - 1].Chave)
    {
        printf(" Erro : Registro ja esta presente\n");
        *Cresceu = false;
        return;
    }
    if (Reg.Chave < Ap->r[i - 1].Chave)
        i--;

    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);

    if (!*Cresceu)
        return;
    // Pagina tem espaco
    if (Ap->n < MM)
    {
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = false;
        return;
    }
    //Pagina tem que ser dividida, atingiu a capacidade máxima

    ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;

    if (i < M + 1)
    {
        InsereNaPagina(ApTemp, Ap->r[MM - 1], Ap->p[MM]);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    }
    else
        InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    for (j = M + 2; j <= MM; j++)
        InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);
    Ap->n = M;
    ApTemp->p[0] = Ap->p[M + 1];
    *RegRetorno = Ap->r[M];
    *ApRetorno = ApTemp;
}

void Insere(TipoRegistro Reg, TipoApontador *Ap)
{
    short Cresceu;
    // para saber se a arvore vai ou não crescer
    TipoRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;
    //chamada da função recursiva
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno); // saber se alteração em determinado nível implica em um nível anterior
                                                      // Arvore cresce na altura pela raiz

    if (Cresceu)
    {
        ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
        ApTemp->n = 1;
        //printf("entrou na condiçao cresceu");
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;
        *Ap = ApTemp;
    }
}

void Reconstitui(TipoApontador ApPag, TipoApontador ApPai, int PosPai, short *Diminuiu)
{
    TipoPagina *Aux;
    long DispAux, j;
    /// Aux = TipoPagina a direita de ApPag
    if (PosPai < ApPai->n)
    {
        Aux = ApPai->p[PosPai + 1];
        DispAux = (Aux->n - M + 1) / 2;
        ApPag->r[ApPag->n] = ApPai->r[PosPai];
        ApPag->p[ApPag->n + 1] = Aux->p[0];
        ApPag->n++;
        // Existe folga : transfere de Aux para ApPag
        if (DispAux > 0)
        {
            for (j = 1; j < DispAux; j++)
            {
                InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j]);
            }
            ApPai->r[PosPai] = Aux->r[DispAux - 1];
            Aux->n -= DispAux;
            for (j = 0; j < Aux->n; j++)
                Aux->r[j] = Aux->r[j + DispAux];
            for (j = 0; j <= Aux->n; j++)
                Aux->p[j] = Aux->p[j + DispAux];
            *Diminuiu = false;
        }
        // Fusao: intercala Aux em ApPag e libera Aux
        else
        {
            for (j = 1; j <= M; j++)
            {
                InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j]);
            }
            free(Aux);
            for (j = PosPai + 1; j < ApPai->n; j++)
            {
                ApPai->r[j - 1] = ApPai->r[j];
                ApPai->p[j] = ApPai->p[j + 1];
            }
            ApPai->n--;
            if (ApPai->n >= M)
                *Diminuiu = false;
        }
    }

    //Aux = TipoPagina a esquerda de ApPag
    else
    {
        Aux = ApPai->p[PosPai - 1];
        DispAux = (Aux->n - M + 1) / 2;
        for (j = ApPag->n; j >= 1; j--)
        {
            ApPag->r[j] = ApPag->r[j - 1];
        }
        ApPag->r[0] = ApPai->r[PosPai - 1];
        for (j = ApPag->n; j >= 0; j--)
        {

            ApPag->p[j + 1] = ApPag->p[j];
        }
        ApPag->n++;
        // Existe espaço : transferencia de Aux para ApPag

        if (DispAux > 0)
        {
            for (j = 1; j < DispAux; j++)
            {
                InsereNaPagina(ApPag, Aux->r[Aux->n - j], Aux->p[Aux->n - j + 1]);
            }
            ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
            ApPai->r[PosPai - 1] = Aux->r[Aux->n - DispAux];
            Aux->n -= DispAux;
            *Diminuiu = false;
        }
        // Fusao: intercala ApPag em Aux e libera ApPag
        else
        {
            for (j = 1; j <= M; j++)
            {

                InsereNaPagina(Aux, ApPag->r[j - 1], ApPag->p[j]);
            }
            free(ApPag);
            ApPai->n--;

            if (ApPai->n >= M)
                *Diminuiu = false;
        }
    }
}

int comparar()
{
    return contadordecomparar;
}

int ArvoreB(FILE *arq, int chavePesquisa)
{
    rewind(arq);

    TipoApontador *Arvore = malloc(sizeof(TipoApontador));     // alocando o apontador inicial
    Inicializa(Arvore);                                        // inicializando
    TipoRegistro *registro = malloc(sizeof(TipoRegistro) * 1); // criando uma variavel do tipo registro para fazer uma leitura no arquivo bin
    int leituraexterna = 0;
    clock_t tInsereInicial = clock(); // função para calcular o tempo da função insere
    while (fread(registro, sizeof(TipoRegistro), 1, arq) == 1)
    {
        leituraexterna++;
        Insere((*registro), Arvore);
    }
    clock_t tInsereFinal = (clock() - tInsereInicial); //Faz a operação tempo inicial menos tempo final de execução da função insere
    //printf("FINAL");
    TipoRegistro *regparapesquisa = malloc(sizeof(TipoRegistro)); // inicializando uma variavel do tipo registro para realizar uma pesquisa

    regparapesquisa->Chave = chavePesquisa;

    clock_t ttPesquisaInicial = clock(); // função para calcular o tempo da função insere
    TipoRegistro *cat = Pesquisa(regparapesquisa, Arvore);
    clock_t ttPesquisaFinal = (clock() - ttPesquisaInicial); //Faz a operação tempo inicial menos tempo final de execução da função insere
    printf("\nVoce achou o livro de nome [%s] e codigo [%d] foi localizado!\n", cat->dado2, cat->Chave);
    int contador = comparar();
    printf("O numero de comparacao e %d\n", contador);
    printf("O tempo de execucao do metodo insere e %.4lf segundos\n", ((double)tInsereFinal) / CLOCKS_PER_SEC);
    printf("O tempo de execucao do pesquisa e %.4lf segundos\n", ((double)ttPesquisaFinal) / CLOCKS_PER_SEC);
    printf("O numero de leituras na memoria externa e %d\n", leituraexterna);

    free(cat);
    free(Arvore);
    free(regparapesquisa);

    return 0;
}