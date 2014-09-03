// !!! IMPORTANTE ESSE ARQUIVO ESTÁ CODIFICADO EM UTF-8 !!!
// Se você não estiver vendo um símbolo de um cavalo de xadrez aqui (♞) não
// edite esse arquivo nesse editor. Use o Vim :D

// ┏━┳┓
// ┃ ┃┃ Caracteres para novas viadagens :D
// ┣━╋┫
// ┗━┻┛
// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━┓ ┃
// ┃ ┃                                                            ┃         ┃ ┃
// ┃ ┃ Trabalho de Programação de Alto Desempenho - Projeto 2     ┃  ⚈   ⚈  ┃ ┃
// ┃ ┃                                                            ┃         ┃ ┃
// ┃ ┃ Aluno: Gabriel H. M. Saraiva                               ┃         ┃ ┃
// ┃ ┃ Professor: Dr. Aleardo Manacero Jr.      Data: 25/07/2014  ┃    ◡    ┃ ┃
// ┃ ┃                                                            ┃         ┃ ┃
// ┃ ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━┫ ┃
// ┃ ┃                                                                      ┃ ┃
// ┃ ┃ Descrição do projeto solicitado em                                   ┃ ┃
// ┃ ┃ http://www.dcce.ibilce.unesp.br/~aleardo/cursos/hpc/projeto2.php     ┃ ┃
// ┃ ┃                                                                      ┃ ┃
// ┃ ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫ ┃
// ┃ ┃                                                                      ┃ ┃
// ┃ ┃ Escreva um programa para ordenação de valores inteiros armazenados   ┃ ┃
// ┃ ┃ em um vetor. A ordenação deve ser feita usando a estratégia de       ┃ ┃
// ┃ ┃ merge-sort empregada no primeiro projeto, porém agora usando o       ┃ ┃
// ┃ ┃ modelo bag-of-tasks programado em MPI.                               ┃ ┃
// ┃ ┃                                                                      ┃ ┃
// ┃ ┃ O vetor, independente de tamanho e do número de máquinas paralelas,  ┃ ┃
// ┃ ┃ deverá ser particionado em partes com no máximo 5000 elementos na    ┃ ┃
// ┃ ┃ primeira fase (ordenação).                                           ┃ ┃
// ┃ ┃                                                                      ┃ ┃
// ┃ ┃ A ordenação usará o algoritmo BOLHA (obrigatoriamente). O            ┃ ┃
// ┃ ┃ coordenador das tarefas será responsável por, a cada fase de merge,  ┃ ┃
// ┃ ┃ enviar como nova tarefa dois vetores recebidos da fase anterior      ┃ ┃
// ┃ ┃ para uma das máquinas.                                               ┃ ┃
// ┃ ┃                                                                      ┃ ┃
// ┃ ┃ Seu programa deve escrever o resultado final em um arquivo de saída  ┃ ┃
// ┃ ┃ (ordenado.txt). Para análise de desempenho seu sistema deve ser      ┃ ┃
// ┃ ┃ testado com vetores de 200, 400, 800, 1600 mil elementos, usando 1,  ┃ ┃
// ┃ ┃ 2, 4, 8 e 16 máquinas.                                               ┃ ┃
// ┃ ┃                                                                      ┃ ┃
// ┃ ┃ Seu programa deve ser executado no cluster do GSPD. Para isso        ┃ ┃
// ┃ ┃ providencie uma conta para acesso remoto ao cluster.                 ┃ ┃
// ┃ ┃                                                                      ┃ ┃
// ┃ ┃ ENTREGA: 02/06, até 23h59, por e-mail para aleardo@sjrp.unesp.br     ┃ ┃
// ┃ ┃                                                                      ┃ ┃
// ┃ ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫ ┃
// ┃ ┃                                                                      ┃ ┃
// ┃ ┃ Obs:                                                                 ┃ ┃
// ┃ ┃ DATA DE ENTREGA ADIADA PARA: 02/08/2014, até as 23h59 devido a greve ┃ ┃
// ┃ ┃                                                                      ┃ ┃
// ┃ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

// = Definições ================================================================
#define IS_ROOT (RANK == ROOT)

#define ROOT 0
#define BUBBLE_TASK_SIZE 5000
#define NO_TASK -1
#define MAX_RANDOM 1000

#define MSG_REQUEST_TASK 1000  // trabalhadore solicita nova tarefa
#define MSG_TASK_ID 1001       // Número da tarefa para controle (ROOT)
#define MSG_TASK_PAYLOAD 1002  // Conteúdo da tarefa.
#define MSG_TASK_DONE 1003     // Sinaliza que a tarefa foi concluida
#define MSG_TASK_RESULT 1004   // Resultado de uma tarefa executada
#define MSG_NO_TASK 1005       // Não existem mais tarefas.
#define MSG_STAGE_DONE 1006    // trabalhador desconectado. sem tarefas
#define MSG_TASK_SIZE 1007     // Tamanho da tarefa (merge sort)

#define BUBBLE_TASK 1          // Tarefas de ordenação por bubble
#define MERGE_TASK 2           // Tarefas de ordenação por merge

#define BILLION 1000000000L    // Usado pelo clock_gettime
// =============================================================================

// = Instrumentação para análise de tempo ======================================
typedef struct timespec TIME;

// = Tabela de cores para o terminal ===========================================

// Mudar isso para 0 para não usar cores :(
#define USE_COLORS 1

#ifdef USE_COLORS
char WORKERS_COLORS[][20] = {
    {"\033[0m"}, //Padrão
    {"\033[1;31m"}, // Vermelho
    {"\033[1;32m"}, // Verde
    {"\033[1;33m"}, // Amarelo
    {"\033[1;34m"}, // Azul
    {"\033[1;35m"}, // Magenta
    {"\033[1;36m"}, // Cyan
    {"\033[1;37m"}, // Branco
    {"\033[1;41m"}, // Fundo vermelho
    {"\033[1;42m"}, // Fundo verde
    {"\033[1;43m"}, // Fundo Amarelo
    {"\033[1;44m"}, // Fundo Azul
    {"\033[1;45m"}, // Fundo Magenta
    {"\033[1;46m"}, // Fundo Cyan
    {"\033[1;47m"}, // Fundo Branco
    {"\033[1;34;46m"}, // Fundo cyan texto azul
    {"\033[1;34;42m"} // Fundo verde texto azul
};
#endif
// =============================================================================

// = Globais ===================================================================
// Embora eu seja totalmente contra o uso de variáveis globais, elas tem um mo-
// tivo para existir. Então, vamos usá-las :) Mas com cuidado.

// Uma vez que essas variaveis são definidas na main não são mais alteradas.
int PROCESS_NUMBER = -1;    // Número total de processos.
int RANK = -1;              // Rank do processo.
FILE *PROFILE;              // Utilizado para fazer o PROFILE do sistema.
double timeComunicacaoBubble = 0;
double timeComunicacaoMerge = 0;
//  ============================================================================


// = Prototipos ================================================================
void erro(int erro);
void help();
int gprintff(char *format, ...);

void createRandomVector(int vector[], int vectorSize);

int isSorted(int vector[], int size);

void bubbleSort(int vector[], int size);

void mergeSort(int vector[], int size);
void merge(int vector[], int size);

void master(char inputFile[], char outputFile[]);
void writeVector(int vector[], int vectorSize, FILE *file);

void client();
void clientBubbleSort();


void clientMergeSort();
int receiveTaskMerge(int **vector, int *size);
int sendMergeTaskResult(int taskId, int vector[], int size);

// Message Handlers Bubble
int hmTDB(int rank, int vector[]);
int hmRTB(int rank, int vector[], int *tasksSended, int taskNumber);

// Message Handlers Merge
int hmTDM(int rank, int vector[], int size);
int hmRTM(int rank, int taskId, int vector[], int size);

int tasksDone(int rank);
int loadVector( int **vector, FILE *file);


// Funções utilizadas para fazer a instrumentação necessária para o relatório

TIME gettime();
double diff(TIME start, TIME end);
// =============================================================================

double diff(TIME start, TIME end)
{
    TIME temp;

    temp.tv_sec = (end.tv_sec - start.tv_sec);
    temp.tv_nsec = (end.tv_nsec - start.tv_nsec);


    double t;
    t = temp.tv_sec * BILLION;
    t += temp.tv_nsec;

    return t / BILLION;
}

TIME gettime()
{
    TIME t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t;
}


// Oi. Esse é o main :D
int main( int argc, char *argv[])
{
    TIME totalTimeStart, totalTimeEnd;

	totalTimeStart = gettime();

    // Aqui um resumo básico:
    // Trata os parâmetros.
    // Define quem é quem e quantos são (RANK, PROCESS_NUMBER)
    // Root executa a função master.
    // Outros ranks executam client.


    srand(time(NULL)); // Isso vem aqui para eu não esquecer.

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &RANK);
    MPI_Comm_size(MPI_COMM_WORLD, &PROCESS_NUMBER);

    if (IS_ROOT) {
        PROFILE = fopen("./profile.txt", "a");
        if(!PROFILE)
        {
        	printf("Não foi possivel criar o PROFILE!\n");
        	fflush(stdout);
        }
        fprintf(PROFILE,"Workers: %d; ",PROCESS_NUMBER-1);
    }


    // = Verifica se o programa foi chamado corretamente :) ====================
    // psiu, ow... - não vale. Tem que passar os parâmetros.

    char *arg1, *arg2;

    if (argc != 2 && argc != 3) {
        gprintff("Número de parâmetros inválido!");
        erro(1);
    }

    arg1 = argv[1];

    if (argc == 3) {
        arg2 = argv[2];
    } else {
        arg2 = malloc(sizeof(char) * 32);
        strcpy(arg2, "ORDENADO.txt");
    }

    // =========================================================================

    // Exibe que o processo começou. Útil para fazer debug no "zóio"
    gprintff("iniciado!");

    // Rapaz... num é que ficou bonito isso. Cada um pro seu canto. #CHUPA_MPI
    IS_ROOT ? master(arg1, arg2) : client();

    totalTimeEnd = gettime();

    if (IS_ROOT) {

        double totalTime = diff(totalTimeStart, totalTimeEnd);

        fprintf(PROFILE,"Comunicação Bubble: %f; ", timeComunicacaoBubble);
        fprintf(PROFILE,"Comunicação Merge: %f; ", timeComunicacaoMerge);
        fprintf(PROFILE,"Total: %f\n",totalTime);

		fclose(PROFILE);
    }

    MPI_Finalize();
    return 0;
}

// = ROOT MSG HANDLING =========================================================

// Trata o recebimento do resultado de uma tarefa enviada a um nó, armazena
// o resultado no local correto do vetor, e atualiza o contador de tarefas
// prontas
int hmTDB(int rank, int vector[])
{
    TIME comStart,comEnd;
    comStart=gettime();
    int taskId;
    MPI_Recv(&taskId, 1, MPI_INT, rank, MSG_TASK_ID, MPI_COMM_WORLD, NULL);

    int index = taskId * BUBBLE_TASK_SIZE;

    MPI_Recv(vector + index, BUBBLE_TASK_SIZE, MPI_INT, rank, MSG_TASK_RESULT,
             MPI_COMM_WORLD, NULL);
    
    comEnd=gettime();
    timeComunicacaoBubble+=diff(comStart,comEnd);

    return 1;
}

// Trata a requisição de uma tarefa por um trabalhadore. Caso não existam mais
// tarefas retorna 0 para o caller tratar. Envia uma msg de aviso com o ID da
// tarefa e depois outra msg com o payload da tarefa, por fim atualiza o total
// de tarefas enviadas.
int hmRTB(int rank, int vector[], int *tasksSended,
          int taskNumber)
{
    
    TIME comStart,comEnd;

    
    int task = *tasksSended;



    if (task == taskNumber) {
        return 0; // no more tasks =(
    }

    //gprintff("Enviando tarefa %d para RANK %d", task + 1, rank);
    comStart = gettime();
    MPI_Send(&task, 1, MPI_INT, rank, MSG_TASK_ID, MPI_COMM_WORLD);

    MPI_Send(vector + task * BUBBLE_TASK_SIZE,
             BUBBLE_TASK_SIZE, MPI_INT, rank, MSG_TASK_PAYLOAD,
             MPI_COMM_WORLD);

	comEnd=gettime();
	timeComunicacaoBubble+=diff(comStart,comEnd);

    (*tasksSended)++;
    return 1;
}

// Avisa trabalhador que não existem mais tarefas e decremena contador de
// trabalhadores ativos
int tasksDone(int rank)
{
    //gprintff("Sinalizando para %d que não existem mais tarefas.", rank);
    int task = -1;
    MPI_Send(&task, 1, MPI_INT, rank, MSG_NO_TASK, MPI_COMM_WORLD);
    return 1;
}
// =============================================================================


// ... faz isso mesmo que ta no nome. O terceiro parâmetro ajusta quantos
// elementos serão exibidos por linha. Use 0 para exibir tudo na mesma linha
void printvector(int vector[], int vectorSize, int lineSize)
{
    if (lineSize == 0) {
        for (int i = 0; i < vectorSize; i++) {
            printf("%d, ", vector[i]);
        }
    } else {
        vectorSize--; // Redução de esforço =D
        for (int i = 0; i < vectorSize; i++) {
            if (i % lineSize == 0) {
                printf("\n");
            }
            printf("%5d, ", vector[i]);
        }
        printf("%5d", vector[vectorSize]);
    }

    printf("\n");
}

// Faz a ordenação !!crescente!! de um vetor por bubble sort. Simples assim =D
void bubbleSort(int vector[], int size)
{

    if (size == 1) {
        // Vetor de tamanho 1 é ordenado por definição
        return; //huehue :D
    }

    int aux;

    // Utilizado para diminuir o esforço. Simplifica o teste do for e para caso
    // não haja mais o que ser ordenado.
    int nonstop = 1;

    for (int i = size - 1; nonstop; i--) {

        nonstop = 0;

        for (int j = 0; j < i; j++) {

            if (vector[j] > vector[j + 1]) {
                aux = vector[j + 1];
                vector[j + 1] = vector[j];
                vector[j] = aux;
                nonstop = 1;
            }

        }
    }
}

// Verifica se um vetor está ordenado de forma crescente!
int isSorted(int vector[], int size)
{

    size--;
    for (int i = 0; i < size; i++) {
        if (vector[i] > vector[i + 1]) {
            return 0;
        }
    }

    return 1;
}

// Função que controla a execução do mestre.
void masterBubble(int vector[], int vectorSize)
{

    printf("\n");
    gprintff("==== Fase 1 - Bubble Sort =====");

    // 1ª Fase - Pré ordenação com Bubble Sort por Bag-of-Tasks

    // Tarefas com tamanhos constantes.
    int bubbleTasksNumber = vectorSize / BUBBLE_TASK_SIZE;

    // Tarefas que foram concluidas (recebidas dos trabalhadores).
    int bubbleTasksDone = 0;

    // Tarefas enviadas aos trabalhadores.
    int bubbleTasksSended = 0;

    // Trabalhadores ativos fazendo pré sort.
    int bubbleActiveWorkers = PROCESS_NUMBER - 1;

    while (bubbleActiveWorkers > 0) {

        float pct = (bubbleTasksDone * 100.0 / bubbleTasksNumber ) ;

        printf("\rTarefas (E/R/T): %d/%d/%i (%.2f%%) - Workers: %d",
               bubbleTasksSended, bubbleTasksDone, bubbleTasksNumber,
               pct,
               bubbleActiveWorkers

              );

        // Tratamento das mensagens. Verifica o tipo da mensagem pela TAG

        MPI_Status status;

        int signal;
        MPI_Recv(&signal, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);

        // = MSG_REQUEST_TASK ==================================================

        if (status.MPI_TAG == MSG_REQUEST_TASK) {

            int s = hmRTB(status.MPI_SOURCE, vector, &bubbleTasksSended,
                          bubbleTasksNumber);

            if (!s) {

                if (tasksDone(status.MPI_SOURCE) == 1) {
                    bubbleActiveWorkers--;
                }
            }
        }

        // = MSG_TASK_DONE =====================================================

        else if (status.MPI_TAG == MSG_TASK_DONE) {

            int ret = hmTDB(status.MPI_SOURCE, vector);
            bubbleTasksDone += ret;

        }

        // = OUTRAS MSGS =======================================================

        else {
            gprintff("ERRO! MENSAGEM DE SINALIZAÇÃO COM TAG DESCONHECIDA!");
            gprintff("TAG: %d\n", status.MPI_TAG);
        }

        // = Final do recebmento de mensagens ==================================

    }

    printf("\n");

    // Final da primeira fase =D

    // Até aqui temos um vetor pré ordenado por bubble sort em subvetores de
    // tamanho BUBBLE_TASK_SIZE
    //
    // Aqui começa a parte de ordenação por merge sort

}

void masterMerge(int vector[], int vectorSize)
{

    gprintff("\n\n MERGE SORT \n\n");

    int workers = (PROCESS_NUMBER - 1);

    // No merge sort como o número de partes diminúi pela metade a cada
    // interação, aqui vai acontecer o mesmo. Como cada parte vai para um
    // processo diferente, aqui os processos serão reduzidos pela metade
    // a cada passo.

    int passo = 1;

    while (workers > 0 ) {

        printf("\n\n");
        gprintff("===========================================================");
        gprintff("Passo %i", passo++);

        int size = vectorSize / workers;
        int mergeTasks = workers; // # de tarefas = # de trabalhadores
        int mergeTasksSended = 0;
        int mergeTasksDone = 0;

        while (mergeTasksDone < mergeTasks) {


            // Recebe uma mensagem, verifica o tipo e repassa ao handler correto
            MPI_Status status;

            // Essa variavel não é utilizada a principio. Só está aqui por que
            // tenho que enviar algo. Mas todo tratamento é feito com TAGS
            int signalMessage;

            // Recebe a belezura...
            MPI_Recv(&signalMessage, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG
                     , MPI_COMM_WORLD, &status);

            // Usada para tratar o retorno do handler. Está fora dos IFs por
            // limpeza de código.
            int ret;

            //Desnecessário, mas por motivos de clareza de código dentro dos IFs
            int source = status.MPI_SOURCE;

            // Eu odeio abreviar nomes, mas eu abreviei aqui por causa do
            // tamanho dos nomes:
            // hm = handler messages
            // RTM = Request Task Merge
            // TDM = Task Done Merge

            // Verifica a TAG para descobrir o tipo. Fiz o que pude... =(

            // = MSG_REQUEST_TASK ==============================================

            if (status.MPI_TAG == MSG_REQUEST_TASK) {

                // Não existem mais tarefas para serem enviadas.
                if (mergeTasksSended == mergeTasks) {
                    tasksDone(source);
                }
                // Existem tarefas para enviar
                else {
                    ret = hmRTM(source , mergeTasksSended, vector, size);
                    mergeTasksSended += (ret == 1 ? 1 : 0);
                }
            }

            // = MSG_TASK_DONE =================================================

            else if (status.MPI_TAG == MSG_TASK_DONE) {
                ret = hmTDM(source , vector, size);
                mergeTasksDone += (ret == 1 ? 1 : 0);
            }

            // = OUTRAS MSGS ===================================================

            else {
                gprintff("ERRO! MENSAGEM DE SINALIZAÇÃO COM TAG DESCONHECIDA!");
                gprintff("TAG: %d\n", status.MPI_TAG);
            }

            // = FINAL DESSE TRECHO CHATO ======================================

            gprintff("Tarefas (E/R/T): %d/%d/%d - Workers: %d",
                     mergeTasksSended, mergeTasksDone, mergeTasks, workers);
        }

        gprintff("===========================================================");
        workers /= 2;
    }

    printf("\n\n");
    gprintff("Ordenação concluida! Notificando trabalhadores");
    for (int rank = 1; rank < PROCESS_NUMBER; rank++) {
        tasksDone(rank);
    }
}


void master(char inputFile[], char outputFile[])
{

    TIME bubbleTimeStart, bubbleTimeEnd;
    TIME mergeTimeStart, mergeTimeEnd;
    TIME inputTimeStart, inputTimeEnd;
    TIME outputTimeStart, outputTimeEnd;

    // Para não ficar bagunçado quebrei essa função em mais duas funções.
    // masterBubble - Faz o tratamento da primeira fase (bubbleSort)
    // masterMerge - Faz o tratamento da segunda fase (mergeSort)
    // Ficando assim a cargo dessa função apenas alocar o vetor e preenchê-lo
    // com valores aleatórios, e então chamar as duas funções em ordem.

    FILE *input = fopen(inputFile, "r");
    FILE *output = fopen(outputFile, "w");

    if (input == NULL) {
        gprintff("Impossivel abrir arquivo '%s'\n", inputFile);
        erro(2);
    }

    if (output == NULL) {
        gprintff("Impossivel abrir arquivo '%s'\n", inputFile);
        erro(3);
    }

    int *vector;


    inputTimeStart = gettime();
    int vectorSize = loadVector(&vector, input);
    fclose(input);
    inputTimeEnd = gettime();

    gprintff("Vetor de %d elementos carregado do arquivo '%s'", vectorSize,
             inputFile);

    // 1ª Fase.
    bubbleTimeStart = gettime();
    masterBubble(vector, vectorSize);
    bubbleTimeEnd = gettime();

    MPI_Barrier(MPI_COMM_WORLD);

    // 2ª Fase.
    mergeTimeStart = gettime();
    masterMerge(vector, vectorSize);
    mergeTimeEnd = gettime();

    // Grava o vetor em um arquivo

    gprintff("Gravando resultados em %s.", outputFile);
    outputTimeStart = gettime();
    writeVector(vector, vectorSize, output);
    fclose(output);
    outputTimeEnd = gettime();

    //Cálculos dos times e gravação no PROFILE
    double inputTime	= diff(inputTimeStart, inputTimeEnd);
    double outputTime	= diff(outputTimeStart, outputTimeEnd);
    double bubbleTime	= diff(bubbleTimeStart, bubbleTimeEnd);
    double mergeTime	= diff(mergeTimeStart, mergeTimeEnd);

	fprintf(PROFILE,"Size: %d; Input: %f; Output: %f; Bubble: %f; Merge: %f; ",
			vectorSize,inputTime,outputTime,bubbleTime,mergeTime);
}

int loadVector( int **vector, FILE *file)
{

    if (file == NULL) {
        return -1;
    }

    if (vector == NULL) {
        return -2;
    }

    // Conta quantos elementos existem no arquivo
    size_t size = 0;

    int c;
    while (!feof(file)) {
        fscanf(file, "%d", &c);
        size++;
    }
    size--; // Desconta o EOF

    rewind(file);

    // Aloca espaço para armazenar o arquivo
    int *v = malloc(sizeof(int) * size);

    for (size_t i = 0; i < size; i++) {
        fscanf(file, "%d", &v[i]);
    }


    (*vector) = v;

    return size;

}

// Grava um vetor de tamanho conhecido em um arquivo. Um valor por linha
void writeVector( int vector[], int vectorSize, FILE *file)
{
    vectorSize--;
    for (int i = 0; i < vectorSize; i++) {
        fprintf(file, "%d\n", vector[i]);
    }

    fprintf(file, "%d", vector[vectorSize]);
}

int hmTDM(int rank, int vector[], int size)
{

    TIME comStart,comEnd;
    comStart=gettime();
    int taskId;

    MPI_Recv(&taskId, 1, MPI_INT, rank, MSG_TASK_ID, MPI_COMM_WORLD, NULL);

    int index = size * taskId;
    MPI_Recv(vector + index, size, MPI_INT, rank, MSG_TASK_RESULT
             , MPI_COMM_WORLD, NULL);
    comEnd=gettime();
    timeComunicacaoMerge+=diff(comStart,comEnd);

    return 1;
}

// Trata a requisição de tarefas para merge-sort enviando uma msg com o ID da
// tarefa e outra msg com a carga a ser processada. É necessário enviar o ID
// para poder fazer o balanceamento de carga de trabalho e assim realmente
// fazer um Bag-Of-Tasks
int hmRTM(int rank, int taskId, int vector[], int size)
{

    TIME comStart,comEnd;
    comStart=gettime();
    // Manda o ID para saber onde guardar essa tarefa na hora de montar o vetor
    MPI_Send(&taskId, 1, MPI_INT, rank, MSG_TASK_ID, MPI_COMM_WORLD);

    // Manda o tamanho da tarefa.
    MPI_Send(&size, 1, MPI_INT, rank, MSG_TASK_SIZE, MPI_COMM_WORLD);

    // Calcula a posição de inicio do envio (onde começa a tarefa)
    int index = size * (taskId);
    MPI_Send(vector + index, size, MPI_INT, rank, MSG_TASK_PAYLOAD,
             MPI_COMM_WORLD);

    comEnd=gettime();
    timeComunicacaoMerge+=diff(comStart,comEnd);

    return 1;
}



// Cria vetores aleatórios. Essa versão não faz o teste se o vetor criado
// realmente é aleatório. No projeto 1 eu testava.
//
// Essa função não é mais usada! Mas vou deixar aqui por que eu gosto dela S2
void createRandomVector(int vector[], int vectorSize)
{

    // Primeira tentativa de desenrolar o laço :D Vamos ver se da certo depois.
    //  int a = vectorSize % 4 ? 1 : 0;
    //  vectorSize/=4;
    //
    //  for (int i = 0; i < vectorSize; i++) {
    //      vector[i] = rand() % MAX_RANDOM;
    //      vector[i+1] = rand() % MAX_RANDOM;
    //      vector[i+2] = rand() % MAX_RANDOM;
    //      vector[i+3] = rand() % MAX_RANDOM;
    //  }
    //
    //  for (int i = vectorSize*4; i < a; i++) {
    //      vector[i] = rand() % MAX_RANDOM;
    //  }

    // Fica assim por enquanto
    for (int i = 0; i < vectorSize; i++) {
        vector[i] = rand() % MAX_RANDOM;
    }
}

// Exibe um HELP. É, eu sei. Esse comentário foi desnecessário.
void help(char arg0[])
{
    printf("Uso: ./%s [ARQUIVO ENTRADA] (ARQUIVO SAIDA)\n", arg0);
    printf("[ARQUIVO ENTRADA] obrigatório fornecer o nome do arquivo\n");
    printf("(ARQUIVO SAIDA) opcional. Vetor ordenado salvo em ORDENADO.txt ");
    printf("se omitido\n");
    printf("\n\n");
    printf("Tenha um bom dia :D\n\n");
}


// Gabriel Printf Flush, :D
int gprintff(char *format, ...)
{
    if (RANK) {
        return 0;
    }

    // Printf que utiliza cores (depende do macro lá em cima USE_COLORS) e das
    char buffer[1024];

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    int b;

#if USE_COLORS == 1
    b = printf("%sProcesso %2d :: %s %s\n",
               WORKERS_COLORS[(RANK % 16) + 1],
               RANK,
               buffer,
               WORKERS_COLORS[0]);
#else
    b = printf("Processo %2d :: %s\n",
               RANK,
               buffer);
#endif

    fflush(stdout);

    return b;
}

void erro(int erro)
{
    MPI_Finalize();
    exit (erro);
}


// Função para fazer a parte do bubble sort do cliente. Solicita tarefas,
// executa e devolve os resultados
void clientBubbleSort()
{
    while (1) {


        // Solicita nova tarefa ================================================
        int signal;
        MPI_Send(&signal, 1, MPI_INT, ROOT, MSG_REQUEST_TASK, MPI_COMM_WORLD);
        // =====================================================================

        // Recebe nova tarefa ==================================================
        int taskId;
        MPI_Status status;

        MPI_Recv(&taskId, 1, MPI_INT, ROOT, MPI_ANY_TAG, MPI_COMM_WORLD,
                 &status);

        // Sem tarefas. Cliente vai sai do while e encerra a função.
        if (status.MPI_TAG == MSG_NO_TASK) {
            break;
        }

        // Todas as tarefas de bubble sort sempre vão ter o mesmo tamanho.
        int vector[BUBBLE_TASK_SIZE];

        MPI_Recv(vector, BUBBLE_TASK_SIZE, MPI_INT, ROOT,
                 MSG_TASK_PAYLOAD,
                 MPI_COMM_WORLD, NULL);

        // =====================================================================

        // Executa a tarefa ====================================================
        //printvector(vector,BUBBLE_TASK_SIZE,8);
        bubbleSort(vector, BUBBLE_TASK_SIZE);
        //printvector(vector,BUBBLE_TASK_SIZE,8);
        // =====================================================================

        // Envia resultado =====================================================
        MPI_Send(&signal, 1, MPI_INT, ROOT, MSG_TASK_DONE, MPI_COMM_WORLD);

        MPI_Send(&taskId, 1, MPI_INT, ROOT, MSG_TASK_ID, MPI_COMM_WORLD);
        MPI_Send(vector, BUBBLE_TASK_SIZE, MPI_INT, ROOT, MSG_TASK_RESULT,
                 MPI_COMM_WORLD);
        // =====================================================================
    }
}


// Essa função é chamada quando o cliente deve solicitar uma nova tarefa ao
// master. Assim essa função faz:
// Solicita a tarefa, recebe o ID da tarefa (>0),recebe o tamanho e o conteúdo
int receiveTaskMerge(int **vector, int *size)
{
    int *lvector;
    int lsize;

    int taskType = MERGE_TASK;
    MPI_Send(&taskType, 1, MPI_INT, ROOT, MSG_REQUEST_TASK, MPI_COMM_WORLD);

    // Recebe tarefa ou sinal de que não existem mais tarefas
    int taskId;
    MPI_Status status;
    MPI_Recv(&taskId, 1, MPI_INT, ROOT, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    if (status.MPI_TAG == MSG_NO_TASK || taskId < 0) {
        return -1;
    }

    // Recebe o tamanho da tarefa (quantos elementos o vetor tem)
    MPI_Recv(&lsize, 1, MPI_INT, ROOT, MSG_TASK_SIZE, MPI_COMM_WORLD, NULL);

    // Aloca espaço para armazenar o vetor que será recebido e preenche com 0
    lvector = (int *) malloc(sizeof(int) * lsize);


    MPI_Recv(lvector, lsize, MPI_INT, ROOT, MSG_TASK_PAYLOAD, MPI_COMM_WORLD,
             NULL);

    (*size) = lsize;
    (*vector) = lvector;


    return taskId;
}

int sendMergeTaskResult(int taskId, int vector[], int size)
{
    //Não é utilizada pelo master. Mas já que tem que mandar, manda algo válido.
    int taskType = MERGE_TASK;

    MPI_Send(&taskType, 1, MPI_INT, ROOT, MSG_TASK_DONE, MPI_COMM_WORLD);

    MPI_Send(&taskId, 1, MPI_INT, ROOT, MSG_TASK_ID, MPI_COMM_WORLD);

    MPI_Send(vector, size, MPI_INT, ROOT,
             MSG_TASK_RESULT, MPI_COMM_WORLD);

    return 1;
}

void clientMergeSort()
{
    while (1) {

        int size;
        int *vector;


        int taskId = receiveTaskMerge(&vector, &size);


        if (taskId == -1) {
            break; // No more tasks!
        }

        mergeSort(vector, size);

        sendMergeTaskResult(taskId, vector, size);
    }

}


void client()
{
    clientBubbleSort();
    MPI_Barrier(MPI_COMM_WORLD);
    clientMergeSort();
}

// Esse é realmente o Sorter, mas é o outro que leva o crédito :(
// Ah, eu kibei esse código do projeto 1. :) Mas mudei algumas partes :)
void merge(int vector[], int size)
{

    int mid; // Mid of vector (size/2)
    int i, j, k;
    int *tmp; //Use this to help sort vector

    tmp = (int *) malloc(size * sizeof(int));

    if (tmp == NULL) {

        // O certo aqui era fazer um return -1 e subir esse erro até quem
        // chamou a mergeSort. Mas isso vai comprometer todo o desempenho
        // do sistema. Então vou usar essa tatica que é menos bonita. :(
        printf("\nNão foi possivel alocar memória para fazer o merge sort.\n");
        fflush(stdout);
        exit(1);
    }

    mid = size / 2;

    i = 0;      // Index begining of vector
    j = mid;    // Index mid of vector
    k = 0;      // Index to new vector free position;

    /* Enquanto estiver dentro dos limites do dos sub vetores copia dos dados
     * de vector ordenando em TMP */
    while (i < mid && j < size) {
        if (vector[i] <= vector[j]) {
            tmp[k] = vector[i];
            i++;
        } else {
            tmp[k] = vector[j];
            j++;
        }
        k++;
    }

    /* Copia o que faltou (a outra metade)*/
    if (i == mid) {
        while (j < size) {
            tmp[k++] = vector[j++];
        }
    } else {
        while (i < mid) {
            tmp[k++] = vector[i++];

        }
    }

    for (i = 0; i < size; ++i) {
        vector[i] = tmp[i];
    }
}

// Esse pilantra aqui na verde só chama o coitado que trabalha e leva
// todo o crédito.
void mergeSort(int vector[], int size)
{

    int mid;

    if (size > 1) {

        mid = size / 2;

        mergeSort(vector, mid);

        mergeSort(vector + mid, size - mid);

        merge(vector, size);
    }
}






















