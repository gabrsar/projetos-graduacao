/*
 * listaDuplaGenerica.c
 *
 *  Created on: Nov 3, 2012
 *      Author: gabriel
 */

#include "listaGenerica.h"
#include "../padroes.h"
void listaGenerica_iniciar(listaGenerica_t *lista)
{
	if (!lista)
	{
		return;
	}

	lista->inicio = NULL;
	lista->final = NULL;
	lista->tamanho = 0;
}

void listaGenerica_inserir(listaGenerica_t *lista, void *elemento, int posicao)
{

	__no_lista_simples_t *novo = (__no_lista_simples_t *) malloc(sizeof(__no_lista_simples_t ));

	novo->informacao = elemento;

	if (lista->tamanho == 0)
	{
		posicao = LISTA_GENERICA_INICIO;
	}

	if (posicao == LISTA_GENERICA_INICIO)
	{

		__no_lista_simples_t *tmp;

		tmp = lista->inicio;
		lista->inicio = novo;
		novo->proximo = tmp;

		if (!lista->tamanho)
		{
			lista->final = lista->inicio;
		}
	}
	else if (posicao == LISTA_GENERICA_FINAL || posicao >= lista->tamanho)
	{

		lista->final->proximo = novo;
		lista->final = novo;
		lista->final->proximo = NULL;
	}
	else
	{

		__no_lista_simples_t *tmp = lista->inicio;

		posicao--;

		for (int i = 0; i < posicao; i++)
		{
			tmp = tmp->proximo;

		}

		novo->proximo = tmp->proximo;
		tmp->proximo = novo;

	}

	lista->tamanho++;
}

void* listaGenerica_remover_elemento(listaGenerica_t *lista, void *elemento)
{

	if (!lista || !lista->inicio)
	{
		return NULL ;
	}

	__no_lista_simples_t *tmp = lista->inicio;

	for (int i = 0; i < lista->tamanho; i++)
	{
		if (tmp->informacao == elemento)
		{
			return listaGenerica_remover_posicao(lista, i);
		}

	}
	return NULL ;

}

void* listaGenerica_remover_posicao(listaGenerica_t *lista, int posicao)
{
	if (!lista || !lista->inicio || posicao < LISTA_GENERICA_FINAL || posicao > lista->tamanho)
	{
		return NULL ;
	}

	__no_lista_simples_t *excluir;

	if (lista->tamanho == 1)
	{
		posicao = LISTA_GENERICA_INICIO;
	}

	if (posicao == LISTA_GENERICA_INICIO)
	{

		if (lista->tamanho == 1)
		{
			excluir = lista->inicio;
			lista->inicio = NULL;
			lista->final = NULL;
		}
		else
		{
			excluir = lista->inicio;
			lista->inicio = lista->inicio->proximo;
		}
	}
	else
	{

		int p = posicao;

		if (posicao == LISTA_GENERICA_FINAL)
		{
			p = lista->tamanho;
		}

		__no_lista_simples_t *tmp = lista->inicio;

		for (int i = 0; i < p - 2; i++)
		{
			tmp = tmp->proximo;
		}

		excluir = tmp->proximo;

		tmp->proximo = tmp->proximo->proximo;

		if (posicao == LISTA_GENERICA_FINAL)
		{
			lista->final = tmp;
		}
	}

	lista->tamanho--;

	void * informacao = excluir->informacao;

	free(excluir);

	return informacao;

}

void listaGenerica_destruir(listaGenerica_t *lista)
{

	register __no_lista_simples_t *tmp;
	register __no_lista_simples_t *tmpExcluir;
	tmp = lista->inicio;
	while (tmp)
	{
		free(tmp->informacao);
		tmpExcluir = tmp;
		tmp = tmp->proximo;
		free(tmpExcluir);
	}

}

int listaGenerica_possui_elemento(listaGenerica_t *lista, void *elemento)
{

	if (!lista->inicio || !lista->tamanho)
	{
		return -1;
	}

	register __no_lista_simples_t *tmp1;
	tmp1 = lista->inicio;

	while (tmp1)
	{
		if (tmp1->informacao == elemento)
		{
			return 1;
		}
		tmp1 = tmp1->proximo;
	}
	return 0;
}

void* listaGenerica_obter_posicao(listaGenerica_t *lista, int posicao)
{
	if (!lista->inicio || posicao < LISTA_GENERICA_FINAL || posicao > lista->tamanho-1)
	{
		return NULL ;
	}

	register __no_lista_simples_t *tmp1;

	if (posicao == LISTA_GENERICA_FINAL)
	{
		tmp1 = lista->final;
	}
	else
	{
		tmp1 = lista->inicio;

		for (int i = 0; i < posicao ; i++)
		{
			tmp1 = tmp1->proximo;
		}
	}
	return tmp1->informacao;
}

void listaGenerica_imprimir(listaGenerica_t *lista, void (*funcaoExibir)(void*), char *separador)
{

	register __no_lista_simples_t *tmp;
	tmp = lista->inicio;

	if (!tmp)
	{
		pff("[ LISTA SEM ELEMENTOS ]");
		return;
	}
	else
	{
		pff("[ LISTA: ");
	}

	int i = 0;
	while (tmp)
	{
		printf("(%d,", i++);
		funcaoExibir(tmp->informacao);
		printf(")");
		printf(separador);
		fflush(stdout);

		tmp = tmp->proximo;
	}
	pff("FIM ]\n");
}

void listaGenerica_imprimir_int(void *p)
{
	int *i = (int*) p;

	printf("%d", *i);
	fflush(stdout);
}
