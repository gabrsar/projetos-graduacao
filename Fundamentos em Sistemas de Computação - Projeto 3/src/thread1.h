/*
 * thread1.h
 *
 *  Created on: 08/11/2012
 *      Author: gabriel
 */

#ifndef THREAD1_H_
#define THREAD1_H_

#include "listaGenerica/listaGenerica.h"
#include "padroes.h"
#include "exibicao.h"

#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define JANELA_TAMANHO 5
#define TEMPO_DE_VIDA 10

int NUMERO_DE_PACOTES; //Número do pacote atual.

float THREAD1_TEMPO_SLEEP;

char *MENSAGEM;

listaGenerica_t LISTA_PACOTES_AGUARDANDO_ACK;

pthread_t THREAD_TIME_OUT;
pthread_attr_t THREAD_TIME_OUT_ATTR;

/* Main da thread1 */
void thread1_run();

/* Responsável por configurar e definir as variaveis, threads e listas da Thread 1*/
void configurarThread1();

/* Controla a orquestra :) */
void gerenciarComunicacao();

/* Responsável por cuidar do tempo de vida de cada pacote */
void gerenciarPacotesAguardandoACK();

/* Recebe o ack e verifica qual pacote deve ser liberado, e libera o meliante :) */
void tratarACK(pacote_t ack);

/* Envia o pacote para a thread2 */
void enviarPacote(pacote_t *p);

pacote_t* obterPacoteDeDados();

#endif /* THREAD1_H_ */
