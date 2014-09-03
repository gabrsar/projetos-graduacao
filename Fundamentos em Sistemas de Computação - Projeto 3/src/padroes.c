/*
 * pacote.c
 *
 *  Created on: 09/11/2012
 *      Author: gabriel
 */

#include "padroes.h"
void imprimir_pacote(void *p)
{
	pacote_t *pacote = (pacote_t*) p;

	printf(" [#%d,", pacote->numero);

	imprimir_tipo_pacote(pacote->tipo);
	printf(",%ds] ", pacote->tempoDeVida);

}
void imprimir_tipo_pacote(pacote_tipo_enum e)
{
	switch (e)
	{
	case PACOTE_TIPO_VAZIO:
		pff("VAZIO");
		break;
	case PACOTE_TIPO_DADOS:
		pff("DADOS");
		break;
	case PACOTE_TIPO_ACK:
		pff("ACK");
		break;
	}

}

/* Retorna a Diferença entre o # do pacote 1 e do pacote 2 (#1 - #2)
 int compararNumeroDePacote(pacote_t *p1, pacote_t *p2)
 {
 return p1->numero - p2->numero;
 }
 */
