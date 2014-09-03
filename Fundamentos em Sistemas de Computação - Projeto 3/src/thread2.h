/*
 * thread2.c
 *
 *  Created on: 09/11/2012
 *      Author: gabriel
 */

#ifndef THREAD2_H_
#define THREAD2_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "padroes.h"
#include "exibicao.h"
#include "listaGenerica/listaGenerica.h"

#define TEMPO_MAXIMO_DEVOLVER_ACK 15
#define K_DEVOLVER_ACK 0.5

float THREAD2_TEMPO_SLEEP;

listaGenerica_t LISTA_DE_PACOTES_RECEBIDOS;
listaGenerica_t LISTA_ACKS_ENVIAR;
int PACOTE_DE_MAIOR_NUMERO_RECEBIDO;

/* Responsável por cuidar do tempo de vida de cada pacote */
void gerenciarACKsAgendados();
/* Responsável por configurar e definir as variaveis, threads e listas da Thread 1*/
void configurarThread2();
/* Main da thread1 */
void thread2_run();

void enviarACK(pacote_t *p);

void tratarPacoteDeDados(pacote_t dados);

void agendarEnvioDeACK(pacote_t p);

int deveDevolverACK();

int sortearT();

/* Verifica se o pacote já existe na lista de pacotes recebidos.
 * Embora a biblioteca listaGenerica já possua uma função quase isso.
 * Ela só compara (por enquanto...) os ponteiros para o objeto e não suas caracteristicas.
 * Por isso fiz essa função e não aproveitei a da lista */
int isRepetido(pacote_t p);

#endif /* THREAD2_H_ */
