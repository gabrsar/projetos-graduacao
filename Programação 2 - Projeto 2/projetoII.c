#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>

// Prototipos =================================================================

// Principais
int  buscaseq       (int *, int, int);
int  buscabinaria   (int *, int, int);
void bolha          (int *, int);
void selecao        (int *, int);
void insercao       (int *, int);
void shellsort      (int *, int);
void mergesort      (int *, int, int);
void quicksort      (int *, int, int);

// Auxiliares
void vetor_rand     (int *, int);
void exibirVetor    (int *, int);
int *criarVetorOrdenado(int, int, int);
double tempo();

// ============================================================================

// Variaveis Fixas ============================================================
char nomes[6][10] = {   "Bolha",    "Sele��o",  "Inser��o",
                        "ShellSort", "MergeSort", "QuickSort"
                    };
// ============================================================================

// Constantes =================================================================
#define TAMANHO_INICIAL 2000
#define TAMANHO_INCREMENTO 2000
#define ARQUIVO_SAIDA "./saida.txt"
// ============================================================================

double tempo()
{
    return (double) clock() / CLOCKS_PER_SEC;
}

int getAleatorio()
{
    return (double) INT_MAX * rand() / (RAND_MAX + 1.0);
}

void vetor_rand(int *v, int n)
{

    int i;
    for (i = 0; i < n; i++) {
        v[i] = getAleatorio();
    }
}

int buscaseq(int *v, int n, int x)
{

    int i;

    for (i = 0; i < n; i++) {
        if ( v[i] == x ) return i;
    }

    return -1;
}

// Busca bin�ria, implementada pela Professora
int buscabinaria(int *v, int n, int x)
{

    int e, d, m;

    e = 0;
    d = n - 1;

    while (e <= d) {
        m = (e + d) / 2;

        if (v[m] == x) return m;
        else if (v[m] < x) e = m + 1;

        else if (v[m] > x) d = m - 1;
    }
    return -1;
}

// Impementa��o ''da professora de ordena��o por bolha
void bolha (int *v, int n)
{
    int i, j, aux;

    for (i = 0; i < n; i++) {
        for (j = n; j > i; j--) {

            if (v[j - 1] > v[j]) {

                aux = v[j - 1];
                v[j - 1] = v[j];
                v[j] = aux;

            }

        }

    }
}


// Implementa��o da professora de ordena��o por sele��o
void selecao(int *v, int t)
{

    int i, j, menor, troca, ind_menor;

    for (i = 0; i < t; i++) {
        ind_menor = i;
        menor = v[i];
        troca = 0;

        for (j = i + 1; j < t; j++) {
            if (v[j] < menor) {
                ind_menor = j;
                menor = v[j];
                troca = 1;
            }
        }
        if (troca) {
            v[ind_menor] = v[i];
            v[i] = menor;
        }
    }
}

void insercao(int *v, int t)
{

    int i, j, x;
    for (i = 1; i < t; i++) {
        x = v[i];
        for ( j = i - 1; j >= 0 && v[j] > x; j--) {
            v[j + 1] = v[j];

        }
        v[j + 1] = x;
    }
}

void shellsort(int *v, int t)
{

    int i, j, incr, k, x;

    int passos[] = {t / 2, t / 3, t / 4, 1};

    for (k = 0; k < 4; k++) {
        incr = passos[k];

        for (i = 0; i < t; i++) {

            x = v[i];

            for (j = i - incr; j >= 0 && v[j] > x; j -= incr) {
                v[j + incr] = v[j];
            }
            v[j + incr] = x;
        }
    }
}

void quicksort(int *v, int inicio, int fim)
{
    int x, i, j, aux;
    i = inicio;
    j = fim;
    x = v[(i + j) / 2];

    do {
        while (v[i] < x)   i++;
        while (v[j] > x)   j--;

        if (i < j) {
            aux = v[i];
            v[i] = v[j];
            v[j] = aux;
            i++;
            j--;
        } else if (i == j) {
            i++; j--;
        }
    } while (i < j);

    if (inicio < j)    quicksort(v, inicio, j);
    if (fim > i)   quicksort(v, i, fim);
}

void intercala(int *v, int p, int q, int r)
{

    int i, j, k, *w;

    w = (int *)malloc((r - p) * sizeof(int));
    i = p;
    j = q;
    k = 0;
    while (i < q && j < r) {

        if (v[i] <= v[j]) w[k++] = v[i++];
        else w[k++] = v[j++];
    }

    while (i < q) w[k++] = v[i++];
    while (j < r) w[k++] = v[j++];

    for (i = p; i < r; i++) v[i] = w[i - p];

    free(w);
}

void mergesort (int *v, int inicio, int fim)
{

    int q;
    if (inicio < (fim - 1)) {
        q = (inicio + fim) / 2;
        mergesort(v, inicio, q);
        mergesort(v, q, fim);
        intercala(v, inicio, q, fim);
    }
}

int isOrdenado(int *v, int t)
{

    int i = 0;
    for (; i < t - 1; i++) if (v[i] > v[i + 1]) return 0;
    return 1;

}

//Fun��o que faz a compara��o de desempenho entre os m�todos de orden��o.
//Fun��o reescrita para otimizar o c�digo, manuten��o, flexibilidade e legibilidade e tamanho.
void compararOrdenacao()
{

    FILE *arq = fopen(ARQUIVO_SAIDA, "w");
    if (arq == NULL) {
        printf("Erro! O Arquivo de resultados n�o pode ser criado.\n");
        return;
    }

    double tTrabalho;
    printf("Entre com o tempo que as fun��es ter�o para trabalhar:");
    scanf("%lf", &tTrabalho);

    fprintf(arq, "Arquivo de Resultados - Compara��o de Ordena��o de Vetores\n\n");
    fprintf(arq, "LEGENDA: N.E.O. = N�mero de Elementos Ordenados\n\n");

    fprintf(arq, "TEMPO DE TRABALHO DAS FUN��ES: %.2fs\n\n", tTrabalho);
    fprintf(arq, "Testes:\n\n");

    int i = 0;
    char nomes[6][10] = {"Bolha", "Sele��o", "Inser��o", "ShellSort", "MergeSort", "QuickSort"};

    int tamanho = TAMANHO_INICIAL, acomulador = 0, incremento;

    printf("Iniciando testes. Legenda: '.' = 1 incremento '>' = 100 incrementos\n");


    do {
        double tempoTotal, tempoInicial, tempoFinal;

        printf("\nM�todo: %s. Executando", nomes[i]);
        fflush(stdout);

        fprintf(arq, "\nM�TODO:%s\n", nomes[i]);

        tempoInicial = tempo();

        double tInicial, tFinal, tTotal;

        do {

            int *vetor = (int *) malloc(sizeof(int) * tamanho);
            vetor_rand(vetor, tamanho);

            tInicial = tempo();

            char m;

            switch (i) {
            case 0: bolha(vetor, tamanho);       incremento = 2000;  m = '.';  break;
            case 1: selecao(vetor, tamanho);     incremento = 2000;      m = '.';  break;
            case 2: insercao(vetor, tamanho);    incremento = 2000;      m = '.';  break;
            case 3: shellsort(vetor, tamanho);   incremento = 2000;      m = '.';  break;

            //Nesses casos foram usados incrementos maiores para agilizar o processo de teste
            //j� que esses dois metodos s�o muito mais eficientes e o uso de incrementos pequenos
            //iria fazer com que fossem feitos muitos testes.

            case 4: mergesort(vetor, 0, tamanho); incremento = 200000;    m = '>';  break;
            case 5: quicksort(vetor, 0, tamanho); incremento = 200000;    m = '>';  break;
            }

            tFinal = tempo();

            printf("%c", m);
            fflush(stdout);


            tTotal = tFinal - tInicial;
            free(vetor);
            fprintf(arq, "N.E.O.: %-10d Tempo:%3.2fs\n", tamanho, tTotal);

            acomulador += tamanho;
            tamanho += incremento;

        } while (tTotal < tTrabalho);

        tempoFinal = tempo();

        tempoTotal = tempoFinal - tempoInicial;

        tamanho -= TAMANHO_INCREMENTO; //Corrige o n�mero de elementos ordenados.

        printf("OK\n");
        printf( "O m�todo %s ordenou %d elementos em %.2fs. Tempo total do teste:%.2fs\n",
                nomes[i],
                tamanho,
                tTotal,
                tempoTotal
              );

        fprintf(arq,
                "N�mero total de elementos (aloca��o + criar elementos aleat�rios + ordena��o): %d em %3.2fs\n",
                acomulador,
                tempoTotal
               );

        i++;//Corre para proxima fun��o

    } while (i < 6);

    printf("Testes concluidos. Para analisar com mais detalhes verificar arquivo %s.\n",
           ARQUIVO_SAIDA);

    fclose(arq);
}

void compararBuscas()
{

    FILE *arq = fopen(ARQUIVO_SAIDA, "w");
    if (arq == NULL) {
        printf("Erro! O Arquivo de resultados n�o pode ser criado.\n");
        return;
    }

    printf("Compara��o de m�todos de Busca.\n\n");

    double tTrabalho;
    printf("Entre com o tempo que as fun��es ter�o para trabalhar:");

    scanf(" %lf", &tTrabalho);

    int algoritmo = 0;
    printf("Entre com o algoritmo de ir� ordenar os n�meros para a busca bin�ria:\n");

    int i = 0;
    for (; i < 6; i++) {
        printf(" %d - %s\n", i, nomes[i]);
    }
    scanf(" %d", &algoritmo);

    int b;
    printf("Entre com o n�mero de buscas que ser�o feitas por teste:");
    scanf(" %d", &b);

    fprintf(arq, "Arquivo de Resultados - Compara��o de M�todos de Buscas\n\n");

    fprintf(arq, "TEMPO DE TRABALHO DA(S) FUN��O/FUN��ES: %.2fs\n\n", tTrabalho);
    fprintf(arq, "Testes:\n\n");

    int tamanho = 2000, incremento;

    double tbInicial, tbFinal, tbTotal, tsInicial, tsFinal, tsTotal;

    int totalDeTestes = 0;
    int seq = 0, bin = 0;
    do {
        int *vetorAmostra = (int *) malloc(sizeof(int) * tamanho);

        vetor_rand(vetorAmostra, tamanho);


        // Valor a ser buscado.

        tsInicial = tempo();
        int x;
        for (x = 0; x < b; x++) buscaseq(vetorAmostra, tamanho, getAleatorio());

        tsFinal = tempo();

        tsTotal = tsFinal - tsInicial;



        tbInicial = tempo();


        switch (algoritmo) {
        case 0: bolha(vetorAmostra, tamanho);        incremento = 2000;  break;
        case 1: selecao(vetorAmostra, tamanho);      incremento = 2000;  break;
        case 2: insercao(vetorAmostra, tamanho);     incremento = 2000;  break;
        case 3: shellsort(vetorAmostra, tamanho);    incremento = 2000;  break;
        case 4: mergesort(vetorAmostra, 0, tamanho);  incremento = 100000;    break;
        case 5: quicksort(vetorAmostra, 0, tamanho);  incremento = 100000;    break;
        }

        for (x = 0; x < b; x++) buscabinaria(vetorAmostra, tamanho, getAleatorio());

        tbFinal = tempo();

        tbTotal = tbFinal - tbInicial;

        char vencedor[10];
        if (tbTotal <  tsTotal) {
            bin++;
            strcpy(vencedor, "Bin�ria");
        }
        if (tsTotal <  tbTotal) {
            seq++;
            strcpy(vencedor, "Sequencial");
        }
        if (tsTotal == tbTotal)  strcpy(vencedor, "Empate");

        totalDeTestes++;
        printf("Tamanho: %15d - Buscas:%d - Sequencial: %3.2fs - Bin�ria(%s): %3.2fs - Melhor:%s\n",
               tamanho, b, tsTotal, nomes[algoritmo], tbTotal, vencedor
              );

        fprintf(arq,
                "Tamanho: %15d - Buscas:%d - Sequencial: %3.2fs - Bin�ria(%s): %3.2fs - Melhor:%s\n",
                tamanho, b, tsTotal, nomes[algoritmo], tbTotal, vencedor
               );


        tamanho += incremento;

        free(vetorAmostra);

    } while (tbTotal < tTrabalho && tsTotal < tTrabalho);

    printf("Testes concluidos.\n\n");

    printf("Pesquisa Bin�ria foi mais rapida em: %d de %d dos testes. %.2f%%\n\n",
           bin, totalDeTestes, (((double)bin) / (double)totalDeTestes) * 100);
    fprintf(arq,
            "Pesquisa Bin�ria foi mais rapida em: %d de %d dos testes. %.2f%%\n\n", bin,
            totalDeTestes, (((double)bin) / (double)totalDeTestes) * 100);

    printf("Pesquisa Sequencial foi mais rapida em: %d de %d dos testes. %.2f%%\n\n",
           seq, totalDeTestes, (((double)seq) / (double)totalDeTestes) * 100);
    fprintf(arq,
            "Pesquisa Sequencial foi mais rapida em: %d de %d dos testes. %.2f%%\n\n", seq,
            totalDeTestes, (((double)seq) / (double)totalDeTestes) * 100);

    int empate = totalDeTestes - (bin + seq);
    printf("Empate em: %d de %d dos testes. %.2f%%\n\n", empate, totalDeTestes,
           (((double)empate) / (double)totalDeTestes) * 100);
    fprintf(arq, "Empate em: %d de %d dos testes. %.2f%%\n\n", empate,
            totalDeTestes, (((double)empate) / (double)totalDeTestes) * 100);

    printf("Os dados foram salvos em %s\n\n\n", ARQUIVO_SAIDA);
    fclose(arq);
}

int main()
{

    int opc;
    do {
        printf("Benchmark de Ordena��o e Busca\n");
        printf("1 - Comparar fun��es de ordena��o\n");
        printf("2 - Comparar fun��es de buscas\n");
        printf("0 - Sair\n");
        scanf("%d", &opc);

        switch (opc) {
        case 1: compararOrdenacao();    break;
        case 2: compararBuscas();   break;
        }
    } while (opc != 0);
    return 0;
}

