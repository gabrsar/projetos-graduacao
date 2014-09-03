#ifndef _LISTA_ENCADEADA_GENERICA_H
#define _LISTA_ENCADEADA_GENERICA_H

#include <stdio.h>
#include <stdlib.h>

#include "../exibicao.h"
#define LISTA_GENERICA_INICIO 0
#define LISTA_GENERICA_FINAL -1

#define LISTA_GENERICA_CRESCENTE 0
#define LISTA_GENERICA_DECRESCENTE 1

typedef struct __no_lista_simples
{
	void *informacao;
	struct __no_lista_simples *proximo;
} __no_lista_simples_t;

typedef struct
{
	__no_lista_simples_t *inicio;
	__no_lista_simples_t *final;
	int tamanho;
} listaGenerica_t;

/* Inicia uma lista para poder ser utilizada */
void listaGenerica_iniciar(listaGenerica_t *lista);

/* Insere um elemento em uma lista, na posição indicada */
void listaGenerica_inserir(listaGenerica_t *lista, void *elemento, int posicao);

/* Insere um elemento em uma lista, em ordem (crescente ou decrescente), para isso utiliza uma função comparativa que decida quem é maior que quem*/
//void listaGenerica_inserir_ordenado(listaGenerica_t *lista, void *elemento, int ordenacao, int (*funcaoComparativa)(void*, void*));
/* Remove um elemento da lista. Esse elemento é encontrado apartir do endereço passado na variavel "elemento".
 * Um ponteiro para o elemento é retornado*/
void* listaGenerica_remover_elemento(listaGenerica_t *lista, void *elemento);

/* Remove um elemento da lista. Esse elemento é encontrado apartir de sua posição apartir do inicio da lista.
 * Um ponteiro para o elemento removido é retornado */
void* listaGenerica_remover_posicao(listaGenerica_t *lista, int posicao);

/* Libera uma lista da memória, liberando todos os os seus dados e seus nós */
void listaGenerica_destruir(listaGenerica_t *lista);

/* Verifica se a lista possui um elemento */
int listaGenerica_possui_elemento(listaGenerica_t *lista, void *elemento);

/* Retorna um ponteiro para um elemento na lista. Esse elemento é encontrado pela posição apartir do inicio da lista.
 * O elemento retornado não é removido da lista!
 */
void* listaGenerica_obter_posicao(listaGenerica_t *lista, int posicao);

/* Imprime uma lista utilizando uma função especifica para o tipo de dados desejado. */
void listaGenerica_imprimir(listaGenerica_t *lista, void (*funcaoExibir)(void*), char *separador);
void listaGenerica_imprimir_int(void *p);
#endif
