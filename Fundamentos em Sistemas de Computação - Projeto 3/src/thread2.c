/*
 * thread2.h
 *
 *  Created on: 09/11/2012
 *      Author: gabriel
 */

#include "thread2.h"

/* Função que sorteia o K */
void thread2_run()
{
	configurarThread2();

	TPFF(2, "Iniciada :)", 0);

	while (1)
	{

		TPFF(2, "<<< Thread 2 ESPERANDO >>>", 0);
		FUNCAO_SLEEP(THREAD2_TEMPO_SLEEP);
		// Dorme por 1 segundo.
		TPFF(2, "<<< Thread 2    PRONTA >>>", 0);

		gerenciarACKsAgendados();

		pacote_t pacote;

		sem_wait(&SEMAFORO_COMUNICACAO);
		pacote = PONTO_DE_TRANSMISSAO;
		sem_post(&SEMAFORO_COMUNICACAO);

		if (pacote.tipo == PACOTE_TIPO_DADOS)
		{
			// Retira o pacote do canal.
			sem_wait(&SEMAFORO_COMUNICACAO);
			PONTO_DE_TRANSMISSAO.tipo = PACOTE_TIPO_VAZIO;
			sem_post(&SEMAFORO_COMUNICACAO);

			tratarPacoteDeDados(pacote);
		}
	}
}

void tratarPacoteDeDados(pacote_t pacote)
{

	TPFF(2, "Encontrado pacote #%4d de dados no canal", 1, pacote.numero);

	if (!deveDevolverACK())
	{
		return; // Considera como pacote perdido na transmissão.
	}

	// Exibe o conteúdo do pacote
	TPFF(2, "Conteúdo do pacote #%4d: [\"%s\"]", 2, pacote.numero, pacote.dados);

	if (isRepetido(pacote))
	{
		// Pacote já foi recebido e processado. Descarta. =/
		// O Motivo pelo qual o pacote veio repeido pode ser por perda do ACK no meio do caminho, ou um time_out do ack do outro lado.
		TPFF(2, "O pacote #%4d é REPETIDO. Ele será descartado mas seu ACK será enviado novamente", 1, pacote.numero);
	}
	else
	{

		// Aloca o pacote e insere na lista de pacotes recebidos.
		pacote_t *p = (pacote_t*) malloc(sizeof(pacote_t));

		// Copia as informações do pacote temporario para o pacote persistente
		strcpy(p->dados, pacote.dados);
		p->numero = pacote.numero;
		p->tipo = PACOTE_TIPO_DADOS;

		// Armazena o pacote para ser enviado em ordem para a camada de rede (não implementado nesse projeto)
		listaGenerica_inserir(&LISTA_DE_PACOTES_RECEBIDOS, p, LISTA_GENERICA_FINAL);

	}

	agendarEnvioDeACK(pacote);
}

void configurarThread2()
{
	listaGenerica_iniciar(&LISTA_ACKS_ENVIAR);
	listaGenerica_iniciar(&LISTA_DE_PACOTES_RECEBIDOS);
}

int deveDevolverACK()
{

	float k = rand() % 10;
	k /= 10;

	int retorna = k > K_DEVOLVER_ACK ? 1 : 0;

	TPFF(2, "K = %.1f [%sDEVOLVER ACK]%s", 3, retorna ? "" : "NÃO ", retorna ? "" : " - Descarta o pacote");

	return retorna;
}

int sortearT()
{
	return rand() % (TEMPO_MAXIMO_DEVOLVER_ACK + 1);
}

void enviarACK(pacote_t *ack)
{
	//sem_wait(&SEMAFORO_COMUNICACAO);
	PONTO_DE_TRANSMISSAO = *ack;
	//sem_post(&SEMAFORO_COMUNICACAO);

	//Retira o ACK da lista
	listaGenerica_remover_elemento(&LISTA_ACKS_ENVIAR, ack);
}

void gerenciarACKsAgendados()
{

	if (LISTA_ACKS_ENVIAR.tamanho <= 0)
	{
		return;
	}

	int enviado = 0;

	for (int i = 0; i < LISTA_ACKS_ENVIAR.tamanho; i++)
	{

		pacote_t *ack = (pacote_t*) listaGenerica_obter_posicao(&LISTA_ACKS_ENVIAR, i);

		if (ack->tempoDeVida == 0)
		{
			if (!enviado)
			{
				TPFF(2, "Enviando ACK #%4d", 1, ack->numero);

				enviarACK(ack);

				enviado++;
			}
			else
			{
				//Como como só pode despachar 1 ACK por vez...
				TPFF(2, "Não foi possivel enviar ACK #%4d pois o canal já possui um ACK trafegando", 1, ack->numero);

			}
		}
		else
		{
			TPFF(2, "ACK #%4d será enviado em %2ds", 2, ack->numero, ack->tempoDeVida);
			ack->tempoDeVida--;
		}
	}

}

/* Cria o ACK e realiza o processo de sortear o tempo para disparar o ACK e colocá-lo na lista de aguardo */
void agendarEnvioDeACK(pacote_t p)
{

	pacote_t *ack = (pacote_t*) malloc(sizeof(pacote_t));

	// Identifica o ACK
	ack->numero = p.numero;

	// Usa o tempo de vida como um timer para disparar o ACK.
	ack->tempoDeVida = sortearT();

	ack->tipo = PACOTE_TIPO_ACK;

	TPFF(2, "Agendado para devolver ACK #%4d em T = %2ds", 2, ack->numero, ack->tempoDeVida);

	listaGenerica_inserir(&LISTA_ACKS_ENVIAR, ack, LISTA_GENERICA_FINAL);
}

/* Verifica se o pacote já foi recebido e aceito. */
int isRepetido(pacote_t p)
{

	int repetido = 0;

	for (int i = 0; i < LISTA_DE_PACOTES_RECEBIDOS.tamanho; i++)
	{
		pacote_t *tmp = listaGenerica_obter_posicao(&LISTA_DE_PACOTES_RECEBIDOS, i);

		if (tmp->numero == p.numero)
		{
			repetido = 1;
			break;
		}
	}

	return repetido;
}
