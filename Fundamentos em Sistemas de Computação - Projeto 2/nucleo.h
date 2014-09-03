/* nucleo.h
 *
 * Definições e cabeçalhos de das funções responsáveis por gerenciar o núcleo
 * do sistema.
 *
 *
 *
 * Detalhes a serem observados:
 *
 * 1) O projeto não trabalha com um QUANTUM definido, uma vez que existe o
 * "PROCESS_INTERRUPT" que marca o final do quantum time de um processo.
 *
 *
 */

#ifndef NUCLEO_H_
#define NUCLEO_H_

// Bibliotecas incluidas ======================================================
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>		//Utilizada devido ao comando sleep
#include "lista.h"
#include "bcp.h"
#include "syscall.h"
#include "exibicao.h"
#include "disk.h"
// ============================================================================

// MACROS =====================================================================
#define _MS_ 		1000
#define _SLEEP_		usleep(1000*_MS_); //Sleep de 500 milisegundos
// ============================================================================

// Constantes =================================================================
#define NIVEIS_DE_PRIORIDADE 				5 	// Quantidade de niveis de prioridades
#define PID_INICIAL 						1000// Pid do primeiro processo do sistema. Os Pids mais baixos são para os processos do sistema :)
#define PORCENTAGEM_DE_INTERRUPCOES 		1 	// Constante que marca a porcentagem de interrupções no sistema
#define CHANCE_DE_CRIAR_PROCESSOS			2	// Constante que marca a chance de disparar a chamada para criar novo processo
#define TIPOS_DE_INTERRUPCAO 				11	// Número de interrupções definidas pelo trabalho.
#define SIZE_MEMORIA						100 // Tamanho da memória ficticia.
// Flags de Tipos de interrupção ==============================================
#define NAO_ABORTAR	 		 0	// Nenhum problema. Continue processando :) Definida por necessidade e não contabilizada.
#define PROCESS_INTERRUPT 	 1	// interrupção gerada pelo final do quantum-time de um processo
#define PROCESS_CREATE 		 2	// chamada para iniciar a criação de um processo no BCP
#define PROCESS_FINISH 	 	 3	// chamada para terminar a existência de um processo no BCP
#define IO_REQUEST 			 4	// chamada de operação de entrada/saída
#define IO_FINISH 			 5	// sinalização de final de E/S
#define MEM_LOAD_REQ 		 6	// chamada de operação de carregamento na memória
#define MEM_LOAD_FINISH 	 7	// sinalização de final de carregamento
#define FS_REQUEST 			 8	// chamada para operação no sistema de arquivos
#define FS_FINISH 			 9	// sinalização de final de operação no SA
#define SEMAPHORE_P 		10	// tratamento de bloqueio de processo
#define SEMAPHORE_V 		11	// tratamento de desbloqueio de processo
// ============================================================================

// Variaveis globais do núcleo ================================================
int PID;							// Contador do PID atual.

// Variavel que controla quando deve ser liberado o processador ===============
int ABORTAR_PROCESSAMENTO;     // Controlada pelo semaforo SEMAFORO_PROCESSADOR
bcp *PROCESSO_NA_CPU;		   // Controlada pelo semaforo SEMAFORO_PROCESSADOR
// ============================================================================

// Listas de processos ========================================================
lista *LISTAS_DE_PROCESSOS;							// Acesso controlado
lista *LISTA_DE_BLOQUEADOS_POR_SEMAFORO;			// pelo semaforo
lista *LISTA_DE_BLOQUEADOS_POR_ENTRADA_E_SAIDA;     // SEMAFORO_LISTA_DE_PROCESSOS
// ============================================================================

// Threads das interrupções
pthread_t THREAD_PROCESS_INTERRUPT;
pthread_t THREAD_PROCESS_CREATE;
pthread_t THREAD_PROCESS_FINISH;
pthread_t THREAD_SEMAPHORE_P;
pthread_t THREAD_SEMAPHORE_V;
pthread_t THREAD_ESCALONADOR;
pthread_t THREAD_IO_REQUEST;
pthread_t THREAD_IO_FINISH;

// Atributos das threads
pthread_attr_t THREAD_PROCESS_INTERRUPT_ATTR;
pthread_attr_t THREAD_PROCESS_CREATE_ATTR;
pthread_attr_t THREAD_PROCESS_FINISH_ATTR;
pthread_attr_t THREAD_SEMAPHORE_P_ATTR;
pthread_attr_t THREAD_SEMAPHORE_V_ATTR;
pthread_attr_t THREAD_ESCALONADOR_ATTR;
pthread_attr_t THREAD_IO_REQUEST_ATTR;
pthread_attr_t THREAD_IO_FINISH_ATTR;

// Semaforos
sem_t SEMAFORO_LISTA_DE_PROCESSOS;
sem_t SEMAFORO_PROCESSADOR;
sem_t SEMAFORO_ESCALONADOR;
sem_t SEMAFORO_SEMAFOROS;

// ========================================================

// Estruturas de dados ====================================
struct buffer
{
	/* Sem uso ainda, inserindo apenas a estrutura */
	int mem_addr; /* endereco do bloco na memoria */
	struct buffer *next; /* proximo pedido da lista */
};
// ========================================================

// Cabeçalhos de funções ==================================
void iniciarSistema();					// Função que inicia o sistema, todo mundo na ordem correta. :)
void definirThreads();
void definirSemaforos();
void definirListasDePrioridades();
void definirConfiguracoesIniciais();

int processar(bcp *processo);			// Função que simula o processamento de um processo
void escalonador();						// Função que implementa o escalonamento do sistema

bcp *criarProcesso();					// Função que literalmente cria um processo
void matarProcesso(bcp *processo);		// Função que mata o processo
int interruptControl();					// Função que avisa quando houve interrupção
int sysCall();							// Função que define qual interrupção foi recebida

void gerenciarInterrupcoes();			// Função que verifica se houve e inicia o disparo de uma interrupção
void dispararInterrupcao(int call);		// Função que efetivamente dispara a interrupção

void imprimirNomeDaInterrupcao(int call);
// Funções das interrupções ===============================
void processInterrupt();
void processCreate();
void processFinish();
void ioRequest();						// !!! IMPLEMENTADAS NO SEGUNDO PROJETO	!!!
void ioFinish();						// !!! IMPLEMENTADAS NO SEGUNDO PROJETO	!!!
void memLoadReq();						// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
void memLoadFinish();					// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
void fsRequest();						// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
void fsFinish();						// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
void semaphoreP();
void semaphoreV();
// ========================================================

#endif /* NUCLEO_H_ */

