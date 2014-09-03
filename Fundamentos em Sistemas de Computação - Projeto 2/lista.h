
#ifndef LISTASIMPLES_C_
#define LISTASIMPLES_C_

#include <stdio.h>
#include <stdlib.h>
#include "bcp.h"
#include "exibicao.h"

/* Implementação de lista encadeada quase genérica
 * Dessa maneira que a lista está implementada é possivel alterar
 * o tipo de dados apenas mudando o conteúdo do macro __TIPO_LISTA
 */

#define __TIPO_LISTA bcp

#define LISTA_INICIO 0
#define LISTA_FIM -1

/* Definição de um nó da lista. Esse nó só armazena um endereço para um BCP. */
typedef struct __noLista
{
	__TIPO_LISTA *informacao;
	struct __noLista *proximo;
} __no;


typedef struct
{
	__no *inicio;
	__no *final;
	int tamanho;

} lista;


bcp* lista_remover_elemento(lista *l, bcp *processo);


void lista_iniciar(lista *l);

void lista_inserir(lista *l, __TIPO_LISTA *valor, int posicao);

int lista_obter_elemento(lista *l, __TIPO_LISTA **elemento, int posicao);

int lista_retorna_posicao(lista *l, bcp *processo);

void lista_remover_elemento_por_posicao(lista *l, int posicao);

void lista_imprimir(lista *l, char *nome);
#endif /* LISTASIMPLES_C_ */
