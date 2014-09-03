/*
 * padroes.h
 *
 *  Created on: 08/11/2012
 *      Author: gabriel
 */

#ifndef PADROES_H_
#define PADROES_H_
#define PACOTE_BYTES 16

#define FUNCAO_SLEEP(a) sleep(a);
#define UM_SEGUNDO	1

#include <semaphore.h>
#include "exibicao.h"
typedef char byte;

typedef enum
{
	PACOTE_TIPO_VAZIO, PACOTE_TIPO_DADOS, PACOTE_TIPO_ACK
} pacote_tipo_enum;

typedef struct
{

	/* Tempo de vida:
	 * Esse campo pode ser utilizado para 3 coisas distintas.
	 *
	 * 1º - Ao transmitir o pacote, atua como o  TTL (Time To Live)	[ !!! não implementado nesse projeto !!! ]
	 * 2º - Na Thread 1, serve para contar o tempo de espera pelo ACK desse pacote.
	 * 3º - Na Thread 2, serve para contar o tempo de espera para disparar o ACK
	 */
	short tempoDeVida;
	int numero;
	pacote_tipo_enum tipo;
	byte dados[PACOTE_BYTES];

} pacote_t;

/* Pacote que representa o meio de transmissão dos pacotes.
 * Para representar que não existe nenhum pacote sendo transmitido o tipo
 * do pacote é marcado como PACOTE_TIPO_NENHUM, assim o pacote é interpretado como "já transmitido".
 * Caso contrário o pacote ainda não foi pego por ninguem.
 */
pacote_t PONTO_DE_TRANSMISSAO;
sem_t SEMAFORO_COMUNICACAO;

void imprimir_pacote(void *p);
void imprimir_tipo_pacote(pacote_tipo_enum e);
//int compararNumeroDePacote(pacote_t *p1, pacote_t *p2);

#endif /* PADROES_H_ */
