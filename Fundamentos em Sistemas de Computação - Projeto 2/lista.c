/* Implementação de lista encadeada simples quase genérica*/

#include "lista.h"

void lista_iniciar(lista *l)
{
	if (!l)
	{
		return;
	}

	l->inicio = NULL;
	l->final = NULL;
	l->tamanho = 0;

}

int lista_obter_elemento(lista *l, __TIPO_LISTA **elemento, int posicao)
{
	// Verifica se a lista é nula ou não.
	if (!l)
	{
		return 0;
	}

	//Verifica se existem elementos para serem retornados.
	if (l->tamanho > 0)
	{

		__no *excluir;
		if (posicao == LISTA_INICIO)
		{
			*elemento = l->inicio->informacao;

			excluir = l->inicio;
			l->inicio = l->inicio->proximo;
		}
		else
		{
			__no *tmp1;
			__no *tmp2 = l->inicio;

			if (posicao == LISTA_FIM)
			{
				posicao = l->tamanho;
				posicao--;
			}

			for (int i = 0; i < posicao; i++)
			{
				if (tmp2)
				{
					tmp1 = tmp2;
					tmp2 = tmp2->proximo;
				}
				else
				{
					return -1;
				}

			}
			if (!tmp2) return -1;

			*elemento = tmp2->informacao;
			excluir = tmp2;

			if (posicao + 1 == l->tamanho)
			{
				tmp1->proximo = NULL;

			}
			else
			{
				tmp1->proximo = tmp2->proximo;
			}
		}

		free(excluir);
		l->tamanho--;
	}
	return 0;
}

void lista_inserir(lista *l, __TIPO_LISTA *valor, int posicao)
{

//Verifica se a lista existe.
	if (!l)
	{
		return;
	}

//Novo elemento
	__no *novo = (__no *) malloc(sizeof(__no ));

//Amarra valor ao novo elemento.
	novo->informacao = valor;

//Se não houver elemento na lista o final vira o inicio.
	if (!(l->inicio))
	{
		posicao = LISTA_INICIO;
	}

	if (posicao == LISTA_INICIO)
	{

		// Amarra o primeiro elemento no novo e amarra o novo no inicio da lista.
		if (l->inicio)
		{
			novo->proximo = l->inicio;
			l->tamanho++;
		}
		else
		{
			novo->proximo = NULL;
			l->final = novo;
			l->tamanho = 1;
		}

		l->inicio = novo;

	}
	else if (posicao == LISTA_FIM)
	{
		l->final->proximo = novo;
		l->tamanho++;
		l->final->proximo->proximo = NULL;
		l->final = novo;

	}
	else
	{
		__no *tmp1;
		__no *tmp2 = l->inicio;

		for (int i = 0; i < posicao; i++)
		{
			if (tmp2)
			{
				tmp1 = tmp2;
				tmp2 = tmp2->proximo;
			}
			else
			{
				break;
			}
		}

		tmp1->proximo = novo;
		novo->proximo = tmp2;
		l->tamanho++;
	}
}

void lista_imprimir(lista *l, char *nome)
{

	if (!l)
	{
		return;
	}

	_LIGHT_PURPLE_
	pff(nome);
	_DEFAULT_
	pff(":");

	__no *tmp = l->inicio;

	if (!tmp)
	{
		_LIGHT_CYAN_
		pff("VAZIA.");
	}
	else
	{
		while (tmp)
		{
			_LIGHT_CYAN_
			printf("%d", tmp->informacao->pid);
			fflush(stdout);
			_DEFAULT_
			pff(",");

			tmp = tmp->proximo;
		}

		pff("\b.");
	}

}

void lista_remover_elemento_por_posicao(lista *l, int posicao)
{

	if (!l)
	{
		return;
	}

	if (!l->tamanho)
	{
		return;
	}

	lista_imprimir(l, "LISTA_IO");

	__no *tmp1, *tmp2, *excluir;

	tmp2 = NULL;
	tmp1 = l->inicio;

	for (int i = 0; i < posicao - 1; i++)
	{
		tmp2 = tmp1;
		tmp1 = tmp1->proximo;
	}

	if (!posicao)
	{

		//Primeiro elemento
		excluir = l->inicio;
		l->inicio = l->inicio->proximo;

	}
	else if (tmp1->proximo)
	{

		//Elemento do meio da lista
		excluir = tmp1;
		tmp2->proximo = tmp1->proximo;

	}
	else
	{

		//Ultimo elemento
		excluir = tmp1;
		tmp2->proximo = NULL;

	}

	free(excluir);

}

int lista_retorna_posicao(lista *l, bcp *processo)
{

	if (!l)
	{
		return -1;
	}

	int i = 0;
	__no *tmp = l->inicio;

	while (tmp)
	{
		if (tmp->informacao == processo)
		{
			return i;
		}
		tmp = tmp->proximo;
		i++;
	}

	return -2;
}

bcp* lista_remover_elemento(lista *l, bcp *processo)
{

	if (!l)
	{
		return NULL ;
	}

	int i = 0;

	bcp *elemento = NULL;
	__no *tmp1 = l->inicio;
	__no *tmp2 = NULL;

	while (tmp1)
	{
		if (tmp1->informacao == processo)
		{
			if (!tmp2)
			{
				l->inicio = tmp1->proximo;
			}
			else
			{
				tmp2->proximo = tmp1->proximo;
			}

			elemento = tmp1->informacao;
			free(tmp1);
			return elemento;
		}
		tmp2 = tmp1;
		tmp1 = tmp1->proximo;
		i++;
	}

	return NULL ;
}
