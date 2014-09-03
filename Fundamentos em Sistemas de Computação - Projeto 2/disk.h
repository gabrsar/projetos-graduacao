#ifndef DISK_H_
#define DISK_H_

#include "bcp.h"

#define DISK_NUMERO_TRILHAS 200
#define DISK_NUMERO_BLOCOS 32

#define DISK_ALGORITHM_SUBINDO 1
#define DISK_ALGORITHM_DESCENDO -1
#define DISK_ALGORITHM_PARADO 0

#define DISK_READ 1
#define DISK_WRITE 0

#define DISK_SIZE (DISK_NUMERO_TRILHAS * DISK_NUMERO_BLOCOS)

// Tipos de dados =============================================================
typedef struct query
{

	//Todas as listas de requisições são listas com cabeça.
	//A identificação do nó de cabeça é onde o cliente é NULL.

	int addr; /* endereco do bloco em disco (200 trilhas, 32 blocos por trilha) */
	int mem_addr; /* endereco do bloco na memoria */
	int rw; /* identificador operacao de leitura/escrita, 1=leitura, 0=escrita */
	bcp *client; /* ponteiro p/ o processo que fez a solicitacao */
	struct buffer *buf; /* ponteiro p/ o buffer que armazenará os dados do pedido */
	struct query *next; /* proximo pedido da lista */
} lista_query;

typedef struct
{
	int block;
	struct buffer *buff;
	int rw;
	int from;
} argDC;     //Estrutura de parametros para chamada da thread DiskControl.

typedef struct
{
	lista_query *list;
	int cur_pos;
} argALGORITHM;     //Estrutura de parametros para chamada da thread Algorithm.
// ============================================================================

// Semaforos para controlar toda a orquestra :D ===============================
sem_t SEMAFORO_LISTAS_DE_REQUISICOES_DE_DISCO;
sem_t SEMAFORO_DISK;		// Controla o acesso a cabeça de leitura do disco
// ============================================================================

// Threads de controle do disco ===============================================
pthread_t THREAD_DISKCONTROL;
pthread_t THREAD_ALGORITHM;
pthread_t THREAD_PARK;
// ============================================================================

// Atributos das threads de controle do disco =================================
pthread_attr_t THREAD_DISKCONTROL_ATTR;
pthread_attr_t THREAD_ALGORITHM_ATTR;
pthread_attr_t THREAD_PARK_ATTR;
// ============================================================================

// Variaveis GLOBAIS de controle e acesso ao DISCO e requisições ==============
int DISK_HEAD_POS;		// Controladas pelo semaforo
int DISK_STATUS;		// SEMAFORO_DISK
// ============================================================================

// Controladas pelo semaforo SEMAFORO_LISTAS_DE_REQUISICOES_DE_DISCO ==========
lista_query *LISTA_DE_REQUISICOES_DE_DISCO_PENDENTES;
lista_query *REQUISICAO_EM_EXECUCAO;
// ============================================================================

// Prototipos das funções =====================================================
void iniciarDisk();
void lista_REQUEST_iniciar(lista_query **l);
void diskDefinirListas();
void diskDefinirSemaforos();
void diskDefinirThreads();

// Threads de acesso e controle do disco ======================================
void runDiskControl(argDC *p);
void DiskControl(int block, struct buffer *buf, int rw, int from);

void runAlgorithm(argALGORITHM *p);
void Algorithm(lista_query *RW_QUERY, int cur_pos);

void Park();
// ============================================================================

// Função que realmente implementa o algoritmo SCAN ===========================
lista_query * DISK_SCAN(lista_query *l, int cur_pos);
lista_query* gerarRequisicaoDeDisco(bcp *p_gerador);
// ============================================================================

// Funções para manipular listas de requisições ===============================
lista_query * lista_REQUEST_obter_elemento(lista_query *l, lista_query *elemento);
lista_query * lista_REQUEST_obter_primeiro_elemento(lista_query *l);
void lista_REQUEST_imprimir(lista_query *l, char *nome);
void lista_REQUEST_inserir(lista_query *lista, lista_query *requisicao);
// ============================================================================

#endif /* DISK_H_ */
