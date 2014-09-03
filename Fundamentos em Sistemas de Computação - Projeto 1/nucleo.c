#include "nucleo.h"

void iniciarSistema()
{

	srand(time(NULL ));
	_LIMPAR_TELA_

	pff("+------ NÚCLEO GERENCIADOR DE PROCESSOS ----+\n");
	pff("|                                           |\n");
	pff("| Desenvolvido por:                         |\n");
	pff("|   Gabriel Henrique Martinez Saraiva       |\n");
	pff("|   Igor Stefani Buttarello                 |\n");
	pff("|                                           |\n");
	pff("+-------------------------------------------+\n");

	/* Inicia o sistema, preparando as listas de processos,
	 * definindo as threads, os semaforos, e da o start no
	 * processo de geração de interrupções.
	 */

	// Define as configurações iniciais do sistema
	pff("\n\n");

	definirConfiguracoesIniciais();
	definirThreads();
	definirSemaforos();
	definirListasDePrioridades();

	pff("Iniciando Escalonador:");
	//Inicia o escalonador de tarefas.
	pthread_create(&THREAD_ESCALONADOR, &THREAD_ESCALONADOR_ATTR, (void*) &escalonador, NULL );

	pff("OK!\n");
	//Fica escutando as interrupções

	gerenciarInterrupcoes();

}

void definirConfiguracoesIniciais()
{

	pff("Definindo configurações do sistema:");

	PID = 0;
	PROCESSOS_ATIVOS = 0;
	ABORTAR_PROCESSAMENTO = 0;

	pff("OK!\n");
}

void definirListasDePrioridades()
{

	pff("Definindo Lista de Prioridades:");
	// Cria um array com as listas de prioridades.
	LISTAS_DE_PROCESSOS = (lista*) malloc(sizeof(lista) * NIVEIS_DE_PRIORIDADE);

	// Inicia a lista de processos do sistema.
	for (int i = 0; i < NIVEIS_DE_PRIORIDADE; i++)
	{
		lista_iniciar(LISTAS_DE_PROCESSOS + i);

		printf("%d", i);
		fflush(stdout);

		printf(",");
		fflush(stdout);

	}

	pff("\b:");

	pff("OK!\n");

	// Aproveita e inicia a lista de processos bloqueados.

	LISTA_DE_BLOQUEADOS = (lista*) malloc(sizeof(lista));
	lista_iniciar(LISTA_DE_BLOQUEADOS);

}

void definirSemaforos()
{
	pff("Definindo Semaforos:");
	sem_init(&SEMAFORO_LISTA_DE_PROCESSOS, 1, 1);
	sem_init(&SEMAFORO_PROCESSADOR, 1, 1);

	sem_init(&SEMAFORO_SEMAFOROS, 1, 1);

	pff("OK!\n");
}

void definirThreads()
{

	pff("Definindo threads:");
	// Deixa as threads prontas pra briga :D
	pthread_attr_init(&THREAD_PROCESS_INTERRUPT_ATTR);
	pthread_attr_init(&THREAD_PROCESS_CREATE_ATTR);
	pthread_attr_init(&THREAD_PROCESS_FINISH_ATTR);
	pthread_attr_init(&THREAD_SEMAPHORE_P_ATTR);
	pthread_attr_init(&THREAD_SEMAPHORE_V_ATTR);
	pthread_attr_init(&THREAD_ESCALONADOR_ATTR);

	pthread_attr_setscope(&THREAD_PROCESS_INTERRUPT_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_PROCESS_CREATE_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_PROCESS_FINISH_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_SEMAPHORE_P_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_SEMAPHORE_V_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_ESCALONADOR_ATTR, PTHREAD_SCOPE_SYSTEM);

	pff("OK!\n");
}

void gerenciarInterrupcoes()
{

	/* Casos de processamento testados com sucesso:
	 * Para utilizar esses casos basta comentar as linhas entre
	 * os MARCADORES 1 e 2 e descomentar as linhas referente as
	 * interrupções do caso desejado.
	 */

	//Caso 1 ===============================================
	/*
	 dispararInterrupcao(PROCESS_CREATE);
	 dispararInterrupcao(PROCESS_CREATE);
	 dispararInterrupcao(PROCESS_CREATE);
	 dispararInterrupcao(PROCESS_FINISH);
	 dispararInterrupcao(PROCESS_INTERRUPT);
	 dispararInterrupcao(PROCESS_INTERRUPT);
	 dispararInterrupcao(PROCESS_INTERRUPT);
	 */
	//======================================================
	//Caso 2 ===============================================
	/*
	 dispararInterrupcao(PROCESS_CREATE);
	 dispararInterrupcao(PROCESS_CREATE);
	 dispararInterrupcao(PROCESS_CREATE);
	 dispararInterrupcao(SEMAPHORE_P);
	 dispararInterrupcao(PROCESS_CREATE);
	 dispararInterrupcao(PROCESS_INTERRUPT);
	 dispararInterrupcao(SEMAPHORE_P);
	 dispararInterrupcao(PROCESS_INTERRUPT);
	 dispararInterrupcao(SEMAPHORE_V);
	 dispararInterrupcao(SEMAPHORE_V);
	 dispararInterrupcao(PROCESS_INTERRUPT);
	 dispararInterrupcao(PROCESS_INTERRUPT);
	 dispararInterrupcao(PROCESS_INTERRUPT);
	 dispararInterrupcao(PROCESS_INTERRUPT);
	 dispararInterrupcao(PROCESS_INTERRUPT);
	 dispararInterrupcao(PROCESS_INTERRUPT);
	 */
	//======================================================
	//MARCADOR 1 ==========================================
	pff("Aguardando Interrupções.\n------------------------\n");

	// Cria o primeiro processo para dar sentido no sistema...
	dispararInterrupcao(PROCESS_CREATE);

	while (1)     // Ao infinito e além :D
	{

		if (interruptControl() == 0)
		{
			sleep(1);
			dispararInterrupcao(sysCall());
		}
	}
	//MARCADOR 2 ==========================================

}

void dispararInterrupcao(int call)
{

	if (call == 1 || call == 2 || call == 3 || call == 10 || call == 11)
	{
		_LIGHT_RED_
	}
	else
	{

		_RED_
	}

	pff("\n");
	imprimirNomeDaInterrupcao(call);
	pff("\n");

	_DEFAULT_

	switch (call)
	{

	case PROCESS_INTERRUPT:

		pthread_create(&THREAD_PROCESS_INTERRUPT, &THREAD_PROCESS_INTERRUPT_ATTR, (void*) &processInterrupt,
				NULL );

		break;

	case PROCESS_CREATE:

		pthread_create(&THREAD_PROCESS_CREATE, &THREAD_PROCESS_CREATE_ATTR, (void*) &processCreate,
				NULL );

		break;

	case PROCESS_FINISH:

		pthread_create(&THREAD_PROCESS_FINISH, &THREAD_PROCESS_FINISH_ATTR, (void*) &processFinish,
				NULL );

		break;

	case SEMAPHORE_P:

		pthread_create(&THREAD_SEMAPHORE_P, &THREAD_SEMAPHORE_P_ATTR, (void*) &semaphoreP,
				NULL );

		break;
	case SEMAPHORE_V:

		pthread_create(&THREAD_SEMAPHORE_V, &THREAD_SEMAPHORE_V_ATTR, (void*) &semaphoreV,
				NULL );

		break;

	}

}

void escalonador()

{

	// Ponteiro para armazenar o processo escolhido pelo escalonador
	bcp * eleito;

	while (1)
	{

		eleito = NULL;

		//Escolhe dentre todos os processos o que vai ocupar a CPU.
		//Escolha depende do campo PRIORIDADE da BCP e também da posição
		//do processo em uma lista de processos de mesma prioridade.

		//Trava a lista de processos para pegar o processo a ser computado.

		sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);

		//Varre toda a lista de procbesso e pega o primeiro processo de maior prioridade

		for (int i = 0; i < NIVEIS_DE_PRIORIDADE; i++)
		{

			if ((LISTAS_DE_PROCESSOS + i)->tamanho > 0)
			{

				//Existem processos nessa lista.
				//Pega o primeiro.

				lista_obter_elemento(LISTAS_DE_PROCESSOS + i, &eleito, LISTA_INICIO);

				_CYAN_
				pff("Escalonador = ");
				_LIGHT_BLUE_
				printf("Eleito o processo %d com prioridade %d na lista %d para ocupar a CPU\n", eleito->pid,
						eleito->prioridade, i);
				fflush(stdout);
				break;     //Sai da busca...
			}
		}

		sem_post(&SEMAFORO_LISTA_DE_PROCESSOS);

		if (!eleito)
		{
			//Ninguem para ir para o processador :(
			continue;
		}

		//Agora que já possui o proceso a ser computado o coloca no processador.

		int motivoDeParada = processar(eleito);

		switch (motivoDeParada)
		{

		case PROCESS_INTERRUPT:

			//Final de quantum. Processo vai para o final da lista.
			sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);
			char listaNome[20];
			_CYAN_
			printf("Escalonador = Inserindo processo %d de prioridade %d na fila %d\n", eleito->pid, eleito->prioridade,
					eleito->prioridade);
			pff("Escalonador = ");
			lista_inserir(LISTAS_DE_PROCESSOS + eleito->prioridade, eleito, 2);
			sprintf(listaNome, "Lista de Prioridade %d", eleito->prioridade);
			lista_imprimir(LISTAS_DE_PROCESSOS + eleito->prioridade, listaNome);
			pff("\n");

			sem_post(&SEMAFORO_LISTA_DE_PROCESSOS);

			break;

		case PROCESS_CREATE:
			//Final de quantum. Processo que saiu do processador entra em segundo na fila da sua prioridade.

			sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);
			_CYAN_
			pff("Escalonador = ");
			_LIGHT_BLUE_
			printf("Processo %d inserido na lista de prioridade %d\n", eleito->pid, eleito->prioridade);
			lista_inserir(LISTAS_DE_PROCESSOS + eleito->prioridade, eleito, 1);

			_CYAN_
			pff("Escalonador = ");
			_LIGHT_BLUE_

			printf("Lista de Prioridade %d", eleito->prioridade);
			lista_imprimir(LISTAS_DE_PROCESSOS + eleito->prioridade, "");
			pff("\n");
			sem_post(&SEMAFORO_LISTA_DE_PROCESSOS);

			break;

		case SEMAPHORE_P:
			sem_wait(&SEMAFORO_SEMAFOROS);

			_CYAN_
			pff("Escalonador = ");
			_LIGHT_BLUE_
			printf("Processo %d inserido na lista de bloqueados\n", eleito->pid);
			lista_inserir(LISTA_DE_BLOQUEADOS, eleito, LISTA_INICIO);

			_CYAN_
			pff("Escalonador = ");

			_LIGHT_BLUE_

			pff("Lista de Bloqueados");
			lista_imprimir(LISTA_DE_BLOQUEADOS, "");
			pff("\n");

			//==================================================

			eleito->estado = ESTADO_BLOQUEADO;

			sem_post(&SEMAFORO_SEMAFOROS);

			break;

		case SEMAPHORE_V:

			// Sai do processador para o processo que acordou trabalhar...
			sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);
			lista_inserir(LISTAS_DE_PROCESSOS + eleito->prioridade, eleito, 2);
			sem_post(&SEMAFORO_LISTA_DE_PROCESSOS);

			break;

		case PROCESS_FINISH:

			matarProcesso(eleito);

			break;

		case IO_REQUEST:
			// Não implementado conforme solicitado.
			break;

		case IO_FINISH:
			// Não implementado conforme solicitado.
			break;

		}

	}

}

int processar(bcp *processo)     //Pula pula :D
{

	sem_wait(&SEMAFORO_PROCESSADOR);
	// Libera o processador para trabalhar...

	ABORTAR_PROCESSAMENTO = PROCESSADOR_SUCESSO;

	int abortar = ABORTAR_PROCESSAMENTO;

	sem_post(&SEMAFORO_PROCESSADOR);

	processo->estado = ESTADO_RODANDO;
	_CYAN_
	pff("Processador = ");
	_LIGHT_CYAN_
	printf("Processando %d:", processo->pid);
	fflush(stdout);
	_DEFAULT_

	while (!abortar)
	{

		processo->pc++;

		if (processo->pc % 1000000 == 0)
		{
			_CPU_TICK_
			pff(" ");
			_DEFAULT_
		}

		//Verifica se é necessário abortar ou não.
		sem_wait(&SEMAFORO_PROCESSADOR);

		abortar = ABORTAR_PROCESSAMENTO;

		sem_post(&SEMAFORO_PROCESSADOR);

	}
	_CYAN_
	pff("Processador = ");
	_LIGHT_BLUE_
	printf("Processo %d saiu da CPU. Motivo: ", processo->pid);
	fflush(stdout);
	_YELLOW_
	imprimirNomeDaInterrupcao(abortar);
	pff("\n");

	_DEFAULT_

	//Devolve o motivo de ter abortado o processamento.

	fflush(stdout);
	return abortar;

}

int sysCall()
{

	//Sorteia uma chama de sistema aleatória que faça sentido.
	int call;
	if (ABORTAR_PROCESSAMENTO)
	{

		do
		{
			call = (rand() % TIPOS_DE_INTERRUPCAO) + 1;
		}
		while (call != 2);

	}
	else
		call = (rand() % TIPOS_DE_INTERRUPCAO) + 1;

	return call;

}

void matarProcesso(bcp *processo)
{
	free(processo);
}

int interruptControl()
{
	int x = rand() % 100;
	if (x < PORCENTAGEM_DE_INTERRUPCOES)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

bcp *criarProcesso()
{

	bcp *processo = malloc(sizeof(bcp));
	processo->estado = ESTADO_PRONTO;
	processo->pc = 0;
	processo->prioridade = rand() % NIVEIS_DE_PRIORIDADE;
	processo->timestamp = time(NULL );
	processo->pid = PID++;
	PROCESSOS_ATIVOS++;

	return processo;
}

void processInterrupt()
{

	sem_wait(&SEMAFORO_PROCESSADOR);

	ABORTAR_PROCESSAMENTO = PROCESS_INTERRUPT;
	sem_post(&SEMAFORO_PROCESSADOR);

}

void processCreate()
{

	bcp *p = criarProcesso();
	_CYAN_
	pff("Process Create = ");
	_LIGHT_GREEN_
	printf("Processo %d criado com prioridade %d e inserido na fila de prioridade 0 momentâneamente.\n", p->pid,
			p->prioridade);
	fflush(stdout);
	_DEFAULT_

	sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);

	lista_inserir(LISTAS_DE_PROCESSOS, p, LISTA_INICIO);

	_CYAN_
	pff("Process Create = ");
	lista_imprimir(LISTAS_DE_PROCESSOS, "Lista de Prioridade 0");
	pff("\n");

	sem_wait(&SEMAFORO_PROCESSADOR);

	ABORTAR_PROCESSAMENTO = PROCESS_CREATE;

	sem_post(&SEMAFORO_PROCESSADOR);

	sem_post(&SEMAFORO_LISTA_DE_PROCESSOS);

}

void processFinish()
{

	sem_wait(&SEMAFORO_PROCESSADOR);
	ABORTAR_PROCESSAMENTO = PROCESS_FINISH;
	sem_post(&SEMAFORO_PROCESSADOR);
}

void semaphoreP()
{
	sem_wait(&SEMAFORO_PROCESSADOR);

	ABORTAR_PROCESSAMENTO = SEMAPHORE_P;

	sem_post(&SEMAFORO_PROCESSADOR);

}

void semaphoreV()
{

	sem_wait(&SEMAFORO_SEMAFOROS);
	int elemento;
	bcp* desbloqueado = NULL;
	if (LISTA_DE_BLOQUEADOS->tamanho <= 0)
	{
		sem_post(&SEMAFORO_SEMAFOROS);
		return;
	}
	elemento = rand() % LISTA_DE_BLOQUEADOS->tamanho;

	lista_obter_elemento(LISTA_DE_BLOQUEADOS, &desbloqueado, elemento);

	_CYAN_

	pff("Semaphore V = ");
	_LIGHT_BLUE_
	printf("Acordando processo %d com prioridade %d que estava bloqueado\n", desbloqueado->pid,
			desbloqueado->prioridade);
	fflush(stdout);

	_DEFAULT_

	sem_post(&SEMAFORO_SEMAFOROS);

	sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);

// Coloca o processo na lista de processo de maior prioridade, para que seja
// processado

	_CYAN_

	pff("Semaphore V = ");
	_LIGHT_BLUE_
	printf("Inserindo processo %d com prioridade %d na fila 0\n", desbloqueado->pid,
			desbloqueado->prioridade);
	fflush(stdout);

	desbloqueado->estado = ESTADO_PRONTO;
	lista_inserir(LISTAS_DE_PROCESSOS, desbloqueado, 1);
	_CYAN_
	pff("Semaphore V = ");
	lista_imprimir(LISTAS_DE_PROCESSOS, "Lista de Prioridade 0");

	_DEFAULT_

	sem_post(&SEMAFORO_LISTA_DE_PROCESSOS);
}

void imprimirNomeDaInterrupcao(int i)
{
	switch (i)
	{
	case PROCESS_INTERRUPT:
		pff("PROCESS_INTERRUPT");
		break;
	case PROCESS_CREATE:
		pff("PROCESS_CREATE");
		break;
	case PROCESS_FINISH:
		pff("PROCESS_FINISH");
		break;
	case IO_REQUEST:
		pff("IO_REQUEST");
		break;
	case IO_FINISH:
		pff("IO_FINISH");
		break;
	case MEM_LOAD_REQ:
		pff("MEM_LOAD_REQ");
		break;
	case MEM_LOAD_FINISH:
		pff("MEM_LOAD_FINISH");
		break;
	case FS_REQUEST:
		pff("FS_REQUEST");
		break;
	case FS_FINISH:
		pff("FS_FINISH");
		break;
	case SEMAPHORE_P:
		pff("SEMAPHORE_P");
		break;
	case SEMAPHORE_V:
		pff("SEMAPHORE_V");
		break;
	}

}

// Funções não implementadas ==============================
void ioRequest()
{
// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
}
void ioFinish()
{
// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
}
void memLoadReq()
{
// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
}
void memLoadFinish()
{
// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
}
void fsRequest()
{
// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
}
void fsFinish()
{
// !!! NÃO IMPLEMENTADAS CONFORME SOLICITADO !!!
}
// ========================================================
