/*
 * thread1.c
 *
 *  Created on: 08/11/2012
 *      Author: gabriel
 */

#include "thread1.h"

void thread1_run()
{

	TPFF(1, "Iniciada :)", 0);

	configurarThread1();

	gerenciarComunicacao();

}

void configurarThread1()
{

	NUMERO_DE_PACOTES = 0;
	sem_init(&SEMAFORO_COMUNICACAO, 1, 1);
	listaGenerica_iniciar(&LISTA_PACOTES_AGUARDANDO_ACK);

}

void gerenciarComunicacao()
{
	while (1)
	{

		pacote_t tmp;

		sem_wait(&SEMAFORO_COMUNICACAO);
		// Lê pacote no canal.
		tmp = PONTO_DE_TRANSMISSAO;
		sem_post(&SEMAFORO_COMUNICACAO);

		if (tmp.tipo == PACOTE_TIPO_ACK)
		{

			sem_wait(&SEMAFORO_COMUNICACAO);
			//Remove pacote do canal.
			PONTO_DE_TRANSMISSAO.tipo = PACOTE_TIPO_VAZIO;
			sem_post(&SEMAFORO_COMUNICACAO);

			TPFF(1, "ACK #%4d Recebido.", 1, tmp.numero);

			tratarACK(tmp);

		}

		gerenciarPacotesAguardandoACK();

		// Verifica se é possivel enviar mais algum pacote...
		if (LISTA_PACOTES_AGUARDANDO_ACK.tamanho < JANELA_TAMANHO)
		{
			// Ainda pode enviar pacotinhos felizes :D uhu!!!
			pacote_t *novo = obterPacoteDeDados();

			enviarPacote(novo);

		}

		TPFF(1, "<<< Thread 1 ESPERANDO >>>", 0);
		FUNCAO_SLEEP(THREAD1_TEMPO_SLEEP);
		TPFF(1, "<<< Thread 1    PRONTA >>>", 0);
	}

}

/* Valida o ACK, verificando se existe algum pacote aguardando por ele e concluindo-o */
void tratarACK(pacote_t ack)
{

	int tamanhoLista = LISTA_PACOTES_AGUARDANDO_ACK.tamanho;

	TPFF(1, "Procurando pacote correspondente ao ACK #%4d.", 1, ack.numero);

	pacote_t *pacoteConfirmado = NULL;

	int posicao = 0;
	for (int i = 0; i < tamanhoLista; i++)
	{

		pacote_t *p = (pacote_t*) listaGenerica_obter_posicao(&LISTA_PACOTES_AGUARDANDO_ACK, i);

		if (p->numero == ack.numero)
		{
			// Achou o dono do ACK :) hauhuahreaehuaeh
			pacoteConfirmado = p;
			posicao = i;
			break;

		}
	}

	if (pacoteConfirmado)
	{

		TPFF(1, "%sPacote #%4d chegou ao seu destino com sucesso! :) !!!", 1, _COR_T1_SUCESSO_, pacoteConfirmado->numero);

		pacote_t *p = listaGenerica_remover_posicao(&LISTA_PACOTES_AGUARDANDO_ACK, posicao);

		// Adeus pacotinho...
		free(p);

	}
	else
	{
		TPFF(1, "ACK #%4d não possui pacote correspondente. Descartado.", 1, ack.numero);

	}

}

void gerenciarPacotesAguardandoACK()
{

	int tamanhoLista = LISTA_PACOTES_AGUARDANDO_ACK.tamanho;

	if (tamanhoLista <= 0)
	{
		return;
	}

	for (int i = 0; i < tamanhoLista; i++)
	{

		pacote_t *tmp = (pacote_t*) listaGenerica_obter_posicao(&LISTA_PACOTES_AGUARDANDO_ACK, i);

		if (tmp->tempoDeVida > 0)
		{

			TPFF(1, "Aguardando ACK #%4d. Tempo restante: %2ds", 2, tmp->numero, tmp->tempoDeVida);

			tmp->tempoDeVida--;

		}
		else
		{

			/* Tempo de espera pelo pacote esgotado.
			 * Remove o pacote da lista de espera e o envia novamente.
			 */

			TPFF(1, "Tempo de espera por ACK #%4d esgotado. Tentando mais uma vez.\n", 1, tmp->numero);

			pacote_t *p = (pacote_t*) listaGenerica_remover_posicao(&LISTA_PACOTES_AGUARDANDO_ACK, i);

			enviarPacote(p);

			//Como o pacote foi removido da lista, é necessário voltar o laço um passo para sincronizar com a lista.
			i--;
		}
	} // Final do for
}

/* Cria os pacotes (alocando-os) e retorna um ponteiro para o novo pacote */
pacote_t* obterPacoteDeDados()
{
	static size_t posicaoDaMensagem = 0;

// Tempo de vida só é definido quando o pacote é enviado.

	pacote_t *novo = (pacote_t*) malloc(sizeof(pacote_t));

	novo->numero = NUMERO_DE_PACOTES++;

	novo->tipo = PACOTE_TIPO_DADOS;

	for (int i = 0; i < PACOTE_BYTES - 1; i++)
	{
		if (MENSAGEM[posicaoDaMensagem] == '\0')
		{
			posicaoDaMensagem = 0;
		}

		novo->dados[i] = MENSAGEM[posicaoDaMensagem++];

	}
	novo->dados[PACOTE_BYTES] = '\0';
	return novo;
}

/* Envia o pacote, iniciando seu tempo de vida e inserindo-o na lista de espera pelo ACK */
void enviarPacote(pacote_t *p)
{
	TPFF(1, "Enviando pacote de dados #%4d. Conteúdo: [\"%s\"]", 2, p->numero, p->dados);
	// Marca que o tempo que deve esperar pelo ACK do pacote.
	p->tempoDeVida = TEMPO_DE_VIDA;

	// Coloca pacote no canal.
	sem_wait(&SEMAFORO_COMUNICACAO);
	PONTO_DE_TRANSMISSAO = *p;
	sem_post(&SEMAFORO_COMUNICACAO);

	// Coloca pacote para esperar o ACK no final da lista.
	listaGenerica_inserir(&LISTA_PACOTES_AGUARDANDO_ACK, (void*) p, LISTA_GENERICA_FINAL);

}

