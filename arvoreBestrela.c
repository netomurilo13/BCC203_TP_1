#include <stdio.h>
#include <stdlib.h>

// Definindo a ordem da árvore
#define M1 2
#define MM1 M1 * 2

// Estrutura do registro que está nos arquivos
typedef struct RegistroBestrela
{
    int chave;
    long dado1;
    char dado2[500];
} RegistroBestrela;

// Enumerador para representar se uma página é interna ou externa
typedef enum
{
    Interna,
    Externa
} PaginaInternaOuExterna;

// Estrutura responsável por armazenar o ponteiro de outra página
typedef struct Pagina *ApontadorPagina;

// Estrutura de uma página
typedef struct Pagina
{
    PaginaInternaOuExterna tipoDaPagina;
    union {
        // Sub-estrutura responsável pela arvore de indices (Página interna)
        struct
        {
            int numeroDeChaves;
            int chaves[MM1];
            ApontadorPagina paginas[MM1 + 1];
        } interna;
        // Sub-estrutura responsável pela camada de folha (Página externa)
        struct
        {
            int numeroDeRegistros;
            RegistroBestrela registros[MM1];
        } externa;
    } dados;
} Pagina;

// Função para inicializar a arvore
void InicializaBestrela(ApontadorPagina *Arvore)
{
    *Arvore = NULL;
}

// Função que recebe um registro contendo as informações necessárias para a pesquisa e a página
void PesquisaBestrela(RegistroBestrela *registro, ApontadorPagina *apPagina)
{
    int i;
    ApontadorPagina pagina;
    pagina = *apPagina;

    // Verifica se a página passada como parâmetro é uma página interna ou externa
    if ((*apPagina)->tipoDaPagina == Interna)
    {
        i = 1;
        // Efetua uma pesquisa sequencial nas páginas internas
        while (i < pagina->dados.interna.numeroDeChaves && registro->chave > pagina->dados.interna.chaves[i - 1])
        {
            i++;
        }
        // Condicional para pesquisar de forma recursiva as sub-árvores da página a esquerda ou a direita
        if (registro->chave < pagina->dados.interna.chaves[i - 1])
        {
            PesquisaBestrela(registro, &pagina->dados.interna.paginas[i - 1]);
        }
        else
        {
            PesquisaBestrela(registro, &pagina->dados.interna.paginas[i]);
        }
        return;
    }
    else
    {
        i = 1;
        // Efetua uma pesquisa sequencial nas páginas folhas
        while (i < pagina->dados.externa.numeroDeRegistros && registro->chave > pagina->dados.externa.registros[i - 1].chave)
        {
            i++;
        }
        // Condicional para verificar se a chave do registro foi encontrada
        if (registro->chave == pagina->dados.externa.registros[i - 1].chave)
        {
            *registro = pagina->dados.externa.registros[i - 1];
            printf("\nEncontrou!\n");
        }
        else
        {
            printf("\nNao encontrou!\n");
        }
    }
}

void InsereNaPaginaBestrela(ApontadorPagina apPagina, RegistroBestrela registro, ApontadorPagina ApDir)
{
    short naoAchouPosicao;
    int k = apPagina->dados.interna.numeroDeChaves;
    naoAchouPosicao = (k > 0);
    while (naoAchouPosicao)
    {
        if (registro.chave >= apPagina->dados.interna.chaves[k - 1])
        {
            naoAchouPosicao = 0;
            break;
        }
        apPagina->dados.interna.chaves[k] = apPagina->dados.interna.chaves[k - 1];
        apPagina->dados.interna.paginas[k + 1] = apPagina->dados.interna.paginas[k];
        k--;
        if (k < 1)
        {
            naoAchouPosicao = 0;
        }
    }
    apPagina->dados.interna.chaves[k] = registro.chave;
    apPagina->dados.interna.paginas[k + 1] = ApDir;
    apPagina->dados.interna.numeroDeChaves++;
}

void InsBestrela(RegistroBestrela registro, ApontadorPagina Ap, short *cresceu, RegistroBestrela *registroRetorno, ApontadorPagina *paginaRetorno)
{
    long i = 1;
    long j;
    ApontadorPagina paginaTemp;
    // Serve para verificar se a arvore esta vazia ou quando atingir o no folha
    if (Ap == NULL)
    {
        *cresceu = 1;
        (*registroRetorno) = registro;
        (*paginaRetorno) = NULL;
        return;
    }

    // realizar uma pesquisa
    while (i < Ap->dados.interna.numeroDeChaves && registro.chave > Ap->dados.interna.chaves[i - 1])
    {
        i++;
    }
    if (registro.chave == Ap->dados.interna.chaves[i - 1])
    {
        printf(" Erro : Registro ja esta presente\n");
        *cresceu = 0;
        return;
    }
    if (registro.chave < Ap->dados.interna.chaves[i - 1])
    {
        i--;
    }

    InsBestrela(registro, Ap->dados.interna.paginas[i], cresceu, registroRetorno, paginaRetorno);

    if (!*cresceu)
    {
        return;
    }
    if (Ap->dados.interna.numeroDeChaves < MM1)
    {
        InsereNaPaginaBestrela(Ap, *registroRetorno, *paginaRetorno);
        *cresceu = 0;
        return;
    }
    // Overflow: Pagina tem que ser dividida

    paginaTemp = (ApontadorPagina)malloc(sizeof(Pagina));
    paginaTemp->dados.interna.numeroDeChaves = 0;
    paginaTemp->dados.interna.paginas[0] = NULL;
    if (i < M1 + 1)
    {
        InsereNaPaginaBestrela(paginaTemp, Ap->dados.externa.registros[MM1 - 1], Ap->dados.interna.paginas[MM1]);
        Ap->dados.interna.numeroDeChaves--; // Ver AQUI
        InsereNaPaginaBestrela(Ap, *registroRetorno, *paginaRetorno);
    }
    else
    {
        InsereNaPaginaBestrela(paginaTemp, *registroRetorno, *paginaRetorno);
    }
    for (j = M1 + 2; j <= MM1; j++)
    {
        InsereNaPaginaBestrela(paginaTemp, Ap->dados.externa.registros[j - 1], Ap->dados.interna.paginas[j]);
    }
    Ap->dados.interna.numeroDeChaves = M1;
    paginaTemp->dados.interna.paginas[0] = Ap->dados.interna.paginas[M1 + 1];
    *registroRetorno = Ap->dados.externa.registros[M1];
    *paginaRetorno = paginaTemp;
}

void InsereBestrela(RegistroBestrela registro, ApontadorPagina *apPagina)
{
    short cresceu;
    RegistroBestrela registroRetorno;
    Pagina *paginaRetorno, *paginaTemp;
    InsBestrela(registro, *apPagina, &cresceu, &registroRetorno, &paginaRetorno);
    if (cresceu)
    {
        paginaTemp = (Pagina *)malloc(sizeof(Pagina));
        paginaTemp->dados.interna.numeroDeChaves = 1;
        paginaTemp->dados.externa.registros[0] = registroRetorno;
        paginaTemp->dados.interna.paginas[1] = paginaRetorno;
        paginaTemp->dados.interna.paginas[0] = *apPagina;
        *apPagina = paginaTemp;
    }
}

void Imprime(Pagina *arvore)
{
    int i = 0;
    if (arvore == NULL)
        return;
    while (i <= arvore->dados.interna.numeroDeChaves)
    {
        Imprime(arvore->dados.interna.paginas[i]);
        if (i != arvore->dados.interna.numeroDeChaves)
        {
            printf("Chave: %d\n", arvore->dados.externa.registros[i].chave);
        }
        i++;
    }
}

int ArvoreBestrela(FILE *arq, int chavePesquisa)
{
    rewind(arq);

    // Criando a árvore com a página raiz
    Pagina *arvore = malloc(sizeof(Pagina));
    InicializaBestrela(&arvore);
    RegistroBestrela registro;

    // Faz a leitura de um em um registro do arquivo
    while (fread(&registro, sizeof(RegistroBestrela), 1, arq) == 1)
    {
        // Chama a função Insere passando o registro lido para efetuar a construção da árvore
        InsereBestrela(registro, &arvore);
    }

    // Imprime(arvore);

    // Registro utilizado para efetuar a busca
    RegistroBestrela *registroBusca = malloc(sizeof(RegistroBestrela));
    
    registroBusca->chave = chavePesquisa;

    PesquisaBestrela(registroBusca, &arvore);

    // if (Pesquisa(registroBusca, &arvore))
    // {
    //     printf("Registro foi encontrado!");
    // }
    // else
    // {
    //     printf("Registro não foi encontrado!");
    // }

    return 0;
}