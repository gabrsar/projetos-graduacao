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
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>		//Utilizada devido ao comando sleep
#include "lista.h"
#include "bcp.h"
#include "syscall.h"
#include "exibicao.h"
// ============================================================================

// Para desativar ou ativar a espera do comando sleep colocar basta trocar o
// comentário dessas linhas

#define _SLEEP_			sleep(1);
//#define _SLEEP_		//NADA

// Constantes =================================================================
#define NIVEIS_DE_PRIORIDADE 				5 	// Quantidade de niveis de prioridades
#define PID_INICIAL 						0 	// Pid do primeiro processo do sistema.
#define PORCENTAGEM_DE_INTERRUPCOES 		1 	// Constante que marca a porcentagem de interrupções no sistema
#define TIPOS_DE_INTERRUPCAO 				11	// Número de interrupções definidas pelo trabalho.
// Flags de Tipos de interrupção ==============================================
#define PROCESSADOR_SUCESSO	 0	// Nenhum problema. Continue processando :) Definida por necessidade e não contabilizada.
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
int PROCESSOS_ATIVOS;
int ABORTAR_PROCESSAMENTO;     		// Variavel que controla quando deve ser liberado o processador.

lista *LISTAS_DE_PROCESSOS;   		// Array de Lista de processos por prioridade
lista *LISTA_DE_BLOQUEADOS;			// Lista de processos bloqueados

// Threads das interrupções
pthread_t THREAD_PROCESS_INTERRUPT;
pthread_t THREAD_PROCESS_CREATE;
pthread_t THREAD_PROCESS_FINISH;
pthread_t THREAD_SEMAPHORE_P;
pthread_t THREAD_SEMAPHORE_V;
pthread_t THREAD_ESCALONADOR;

// Atributos das threads
pthread_attr_t THREAD_PROCESS_INTERRUPT_ATTR;
pthread_attr_t THREAD_PROCESS_CREATE_ATTR;
pthread_attr_t THREAD_PROCESS_FINISH_ATTR;
pthread_attr_t THREAD_SEMAPHORE_P_ATTR;
pthread_attr_t THREAD_SEMAPHORE_V_ATTR;
pthread_attr_t THREAD_ESCALONADOR_ATTR;

// Semaforos

sem_t SEMAFORO_LISTA_DE_PROCESSOS;
sem_t SEMAFORO_PROCESSADOR;
sem_t SEMAFORO_ESCALONADOR;
sem_t SEMAFORO_SEMAFOROS;
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
void ioRequest();						// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
void ioFinish();						// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
void memLoadReq();						// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
void memLoadFinish();					// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
void fsRequest();						// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
void fsFinish();						// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
void semaphoreP();
void semaphoreV();

// ========================================================

#endif /* NUCLEO_H_ */

