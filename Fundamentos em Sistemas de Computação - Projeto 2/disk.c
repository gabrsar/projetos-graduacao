#include <stdio.h>
#include <stdlib.h>

//#include "disk.h"
#include "nucleo.h"

void iniciarDisk()
{

	diskDefinirListas();
	diskDefinirSemaforos();
	diskDefinirThreads();

	//Inicializa o disco
	pff("\nInicializando Disco: ");

	DISK_STATUS = DISK_ALGORITHM_PARADO;
	DISK_HEAD_POS = 0;

	pff("OK!");

}

void diskDefinirSemaforos()
{
	sem_init(&SEMAFORO_LISTAS_DE_REQUISICOES_DE_DISCO, 1, 1);
	sem_init(&SEMAFORO_DISK, 1, 1);
}

void diskDefinirThreads()
{
	pthread_attr_init(&THREAD_DISKCONTROL_ATTR);
	pthread_attr_init(&THREAD_ALGORITHM_ATTR);
	pthread_attr_init(&THREAD_PARK_ATTR);

	pthread_attr_setscope(&THREAD_DISKCONTROL_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_ALGORITHM_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_PARK_ATTR, PTHREAD_SCOPE_SYSTEM);
}

void diskDefinirListas()
{
	lista_REQUEST_iniciar(&LISTA_DE_REQUISICOES_DE_DISCO_PENDENTES);
}

lista_query* gerarRequisicaoDeDisco(bcp *processo)
{
	// Gera uma solicitação de acesso de leitura ou escrita ao disco ficticia.
	lista_query *q = (lista_query*) malloc(sizeof(lista_query));
	q->next = NULL;
	q->addr = rand() % DISK_SIZE;
	q->client = processo;
	q->rw = rand() % 2 ? DISK_READ : DISK_WRITE;
	q->mem_addr = 0;
	q->buf = NULL;

	return q;
}

void lista_REQUEST_iniciar(lista_query **l)
{

	lista_query *cabeca = (lista_query*) malloc(sizeof(lista_query));

	cabeca->next = NULL;
	cabeca->client = NULL;
	cabeca->addr = 0;
	cabeca->buf = NULL;
	cabeca->mem_addr = 0;
	cabeca->rw = 0;

	*l = cabeca;

}

lista_query * lista_REQUEST_obter_primeiro_elemento(lista_query *l)
{

	if (!l)
	{
		return NULL ;
	}

	lista_query *primeiro = l->next;
	if (primeiro)
	{
		l->next = primeiro->next;
	}

	return primeiro;

}

void Park()
{
	fflush(stdout);
	_LIGHT_RED_
	pff("\n\n!!! Disk Park !!!\n\n");
	_DEFAULT_

	sem_wait(&SEMAFORO_DISK);
	{
		DISK_HEAD_POS = 0;
	}
	sem_post(&SEMAFORO_DISK);
}

void runDiskControl(argDC *p)
{
// Essa função só faz o meio de campo entre o disparo da thread
// e os parametros da DiskControl

	DiskControl(p->block, NULL, p->rw, p->from);

	free(p);     //Libera o parametro da memória. Já que não vai mais ser utilizado (eu acho :D)
}

void DiskControl(int block, struct buffer *buf, int rw, int from)
{

	_CYAN_
	fflush(stdout);
	pff("\nDisk Control\t = ");
	_LIGHT_BLUE_
	pff("Chamado por ");
	_YELLOW_

	if (from == 1)
	{
		pff("IO_REQUEST.");
		_DEFAULT_
	}
	if (from == 2)     //IO_FINISH
	{

		pff("IO_FINISH.");

		_DEFAULT_

		// Caso receba uma request de IO_FINISH
		// Finaliza a requisição que estava em andamento,
		// e começa a executar as proximas até acabar, dai ativa o Park.

		lista_query *req;

		sem_wait(&SEMAFORO_LISTAS_DE_REQUISICOES_DE_DISCO);
		{
			req = REQUISICAO_EM_EXECUCAO;
			REQUISICAO_EM_EXECUCAO = NULL;

		}
		sem_post(&SEMAFORO_LISTAS_DE_REQUISICOES_DE_DISCO);

		if (req)
		{

			_CYAN_
			fflush(stdout);
			pff("\nDisk Control\t = ");
			_LIGHT_BLUE_
			printf("Requisição do processo %d, concluida.", req->client->pid);
			_DEFAULT_
			//Remove o processo da fila de bloqueados e coloca como prioritário para o escalonador

			sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);
			{
				bcp *processo;

				sem_wait(&SEMAFORO_DISK);
				{
					processo = lista_remover_elemento(LISTA_DE_BLOQUEADOS_POR_ENTRADA_E_SAIDA, req->client);
				}
				sem_post(&SEMAFORO_DISK);

				if (processo)
				{
					_CYAN_
					pff("\nDisk Control\t = ");
					_LIGHT_BLUE_
					printf("Processo %d que estava bloqueado por ", processo->pid);
					_YELLOW_
					pff("IO_REQUEST");
					_LIGHT_BLUE_
					pff(" foi liberado.");
					_DEFAULT_
					lista_inserir(LISTAS_DE_PROCESSOS, req->client, LISTA_INICIO);
				}

			}
			sem_post(&SEMAFORO_LISTA_DE_PROCESSOS);
			argALGORITHM *parametroAlg = (argALGORITHM*) malloc(sizeof(argALGORITHM));

			sem_wait(&SEMAFORO_DISK);
			{
				if (!REQUISICAO_EM_EXECUCAO)
				{
					parametroAlg->cur_pos = DISK_HEAD_POS;
					parametroAlg->list = LISTA_DE_REQUISICOES_DE_DISCO_PENDENTES;

					pthread_create(&THREAD_ALGORITHM, &THREAD_ALGORITHM_ATTR, (void*) &runAlgorithm, parametroAlg);
				}

			}
			sem_post(&SEMAFORO_DISK);

		}

	}

	sem_wait(&SEMAFORO_LISTAS_DE_REQUISICOES_DE_DISCO);
	{
		// Exibe lista de requisições pendentes
		_CYAN_
		pff("\nDisk Control\t = ");
		_LIGHT_PURPLE_
		lista_REQUEST_imprimir(LISTA_DE_REQUISICOES_DE_DISCO_PENDENTES, "Lista de requisições pendentes");

		_DEFAULT_

		if (!REQUISICAO_EM_EXECUCAO)
		{
			//Verifica se existem requisições não atendidas...
			if (LISTA_DE_REQUISICOES_DE_DISCO_PENDENTES->next)
			{

				argALGORITHM *parametroAlg = (argALGORITHM*) malloc(sizeof(argALGORITHM));
				parametroAlg->cur_pos = DISK_HEAD_POS;
				parametroAlg->list = LISTA_DE_REQUISICOES_DE_DISCO_PENDENTES;

				pthread_create(&THREAD_ALGORITHM, &THREAD_ALGORITHM_ATTR, (void*) &runAlgorithm, parametroAlg);

			}
			else
			{
				pthread_create(&THREAD_PARK, &THREAD_PARK_ATTR, (void*) &Park, NULL );
			}
		}
	}
	sem_post(&SEMAFORO_LISTAS_DE_REQUISICOES_DE_DISCO);

}

void runAlgorithm(argALGORITHM *p)
{
// Essa função só faz o meio de campo entre o disparo da thread
// e os parametros da Algorithm

	Algorithm(p->list, p->cur_pos);

	free(p);
}

void Algorithm(lista_query *list, int cur_pos)
{

	lista_query *requisicao = DISK_SCAN(list, cur_pos);

	if (requisicao)
	{

		//Leva a cabeça do disco até o ponto desejado e realiza a leitura.
		sem_wait(&SEMAFORO_DISK);
		{

			DISK_HEAD_POS = requisicao->addr;

			//Como essa estrutura ainda não funciona apenas altera o valor para "simular" a realização da operação de I/O
			if (requisicao->rw == DISK_READ)
			{
				// Lê a informação no disco.
				;
			}
			else
			{
				// Grava informação do disco
				;
			}

			// Marca que existe uma operação de disco em execução
			REQUISICAO_EM_EXECUCAO = requisicao;
		}

		sem_post(&SEMAFORO_DISK);

		_CYAN_
		pff("\nAlgorithm\t = ");
		_LIGHT_BLUE_
		printf("Executando requisição de acesso do processo %d.", requisicao->client->pid);
		fflush(stdout);

	}
}

lista_query* DISK_SCAN(lista_query *l, int cur_pos)
{

	static int sentido = DISK_ALGORITHM_SUBINDO;

	if (!l)
	{
		return NULL ;
	}

//Função que realmente implementa o algoritmo SCAN
//Retorna a posição da proxima requisição na lista de Requisicoes

	int achou = 0;

	lista_query *tmp = l->next;

// Primeiro testa no sentido em que o disco está trabalhando, depois testa no outro sentido.
	for (int i = 0; i < 2; i++)
	{
		while (tmp && !achou)
		{

			if (sentido == DISK_ALGORITHM_SUBINDO)
			{

				if (tmp->addr >= cur_pos)
				{
					achou = 1;
					break;
				}

			}
			else
			{

				if (tmp->addr <= cur_pos)
				{
					achou = 1;
					break;
				}
			}

			tmp = tmp->next;
		}

		if (!achou)
		{
			//Inverte o sentido do DISCO
			if (sentido == DISK_ALGORITHM_SUBINDO)
			{
				tmp = l->next;
				sentido = DISK_ALGORITHM_DESCENDO;
			}
			else
			{
				tmp = l->next;
				sentido = DISK_ALGORITHM_SUBINDO;
			}
		}
		// E tenta encontrar a requisição no outro sentido.

	}

	if (achou)
	{
		//Remove o elemento da lista e retorna a requisição para o ALGORITHM executa-la
		lista_query *elemento = lista_REQUEST_obter_elemento(LISTA_DE_REQUISICOES_DE_DISCO_PENDENTES, tmp);
		return elemento;
	}

//Não encontrou a soliciação
	return NULL ;

}

void lista_REQUEST_imprimir(lista_query *l, char *nome)
{

	if (!l)
	{
		return;
	}

//Pula o nó cabeça.
	l = l->next;

	_LIGHT_PURPLE_
	pff(nome);
	_DEFAULT_
	pff(":");

	lista_query *tmp = l;
	if (!tmp)
	{
		_LIGHT_CYAN_
		pff("VAZIA");
		_DEFAULT_
		pff(".");

	}
	else
	{
		;

		while (tmp)
		{
			_LIGHT_CYAN_
			printf("\n\t\t\tProcesso %d: %s Trilha: %3d Bloco: %2d.", tmp->client->pid,
					tmp->rw ? "LEITURA" : "ESCRITA",
					tmp->addr / DISK_NUMERO_BLOCOS, tmp->addr % DISK_NUMERO_BLOCOS);
			_DEFAULT_

			fflush(stdout);

			tmp = tmp->next;
		}

		_LIGHT_PURPLE_
		pff("\n\t\t   Final da lista.");
	}
}

void lista_REQUEST_inserir(lista_query *l, lista_query *requisicao)
{

//Insere as requisições na ordem que chegam. Ou seja, sempre no final.
	lista_query *primeiroElemento = l->next;

	if (!primeiroElemento)
	{
		l->next = requisicao;
		requisicao->next = NULL;
		return;
	}

	lista_query *tmp1 = primeiroElemento;
	lista_query *tmp2 = primeiroElemento;

//Corre até o final da lista.
	while (tmp1)
	{
		tmp2 = tmp1;
		tmp1 = tmp1->next;
	}

	tmp2->next = requisicao;

	requisicao->next = NULL;

}

lista_query* lista_REQUEST_obter_elemento(lista_query *l, lista_query *elemento)
{

	if (!l)
	{
		return NULL ;
	}

	if (!elemento)
	{
		return NULL ;
	}

	lista_query *encontrado;

	lista_query *tmp1, *tmp2;

	tmp1 = l->next;     //Primeiro elemento.
	tmp2 = l;

	while (tmp1 && tmp1 != elemento)
	{
		tmp2 = tmp1;
		tmp1 = tmp1->next;

	}

	if (tmp1 == elemento)
	{
		encontrado = tmp1;
		tmp2->next = tmp1->next;
		return encontrado;
	}
	return NULL ;

}

