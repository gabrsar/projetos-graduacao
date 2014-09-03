#include "nucleo.h"

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
	// Caso de Testes para o segundo projeto ==============
	/*pararInterrupcao(PROCESS_CREATE);
	 dispararInterrupcao(PROCESS_CREATE);
	 dispararInterrupcao(PROCESS_CREATE);
	 dispararInterrupcao(IO_REQUEST);
	 dispararInterrupcao(IO_REQUEST);
	 dispararInterrupcao(IO_REQUEST);
	 dispararInterrupcao(IO_FINISH);
	 dispararInterrupcao(IO_FINISH);
	 dispararInterrupcao(IO_FINISH);


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
			_SLEEP_
			dispararInterrupcao(sysCall());
		}
	}

	//MARCADOR 2 ==========================================
	while (1)
		;
}

void iniciarSistema()
{

	pff("+----- NÚCLEO GERENCIADOR DE PROCESSOS -----+\n");
	pff("|      > Controle de  Acesso a Disco <      |\n");
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

	iniciarDisk();
	definirConfiguracoesIniciais();
	definirThreads();
	definirSemaforos();
	definirListasDePrioridades();

	iniciarDisk();

	pff("\nIniciando Escalonador: ");

	//Inicia o escalonador de tarefas.

	pthread_create(&THREAD_ESCALONADOR, &THREAD_ESCALONADOR_ATTR, (void*) &escalonador, NULL );

	pff("OK!");

	//Fica escutando as interrupções

	gerenciarInterrupcoes();

}

void definirConfiguracoesIniciais()
{

	pff("\nDefinindo configurações do sistema: ");

	PID = PID_INICIAL;

	ABORTAR_PROCESSAMENTO = 0;

	pff("OK!");
}

void definirListasDePrioridades()
{

	pff("\nDefinindo Lista de Prioridades: ");
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

	pff("OK!");

	// Aproveita e inicia a lista de processos bloqueados.
	LISTA_DE_BLOQUEADOS_POR_SEMAFORO = (lista*) malloc(sizeof(lista));
	lista_iniciar(LISTA_DE_BLOQUEADOS_POR_SEMAFORO);

	// Segundo projeto
	LISTA_DE_BLOQUEADOS_POR_ENTRADA_E_SAIDA = (lista*) malloc(sizeof(lista));
	lista_iniciar(LISTA_DE_BLOQUEADOS_POR_ENTRADA_E_SAIDA);

}

void definirSemaforos()
{
	pff("\nDefinindo Semaforos: ");
	sem_init(&SEMAFORO_LISTA_DE_PROCESSOS, 1, 1);
	sem_init(&SEMAFORO_PROCESSADOR, 1, 1);
	sem_init(&SEMAFORO_SEMAFOROS, 1, 1);

	pff("OK!");
}

void definirThreads()
{

	pff("\nDefinindo threads: ");
	// Deixa as threads prontas pra briga :D
	pthread_attr_init(&THREAD_PROCESS_INTERRUPT_ATTR);
	pthread_attr_init(&THREAD_PROCESS_CREATE_ATTR);
	pthread_attr_init(&THREAD_PROCESS_FINISH_ATTR);
	pthread_attr_init(&THREAD_SEMAPHORE_P_ATTR);
	pthread_attr_init(&THREAD_SEMAPHORE_V_ATTR);
	pthread_attr_init(&THREAD_ESCALONADOR_ATTR);

	//Segundo projeto
	pthread_attr_init(&THREAD_IO_REQUEST_ATTR);
	pthread_attr_init(&THREAD_IO_FINISH_ATTR);

	pthread_attr_setscope(&THREAD_PROCESS_INTERRUPT_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_PROCESS_CREATE_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_PROCESS_FINISH_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_SEMAPHORE_P_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_SEMAPHORE_V_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_ESCALONADOR_ATTR, PTHREAD_SCOPE_SYSTEM);

	//Segundo projeto
	pthread_attr_setscope(&THREAD_IO_REQUEST_ATTR, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setscope(&THREAD_IO_FINISH_ATTR, PTHREAD_SCOPE_SYSTEM);

	pff("OK!");
}

void dispararInterrupcao(int call)
{

	_SLEEP_
	_DEFAULT_

	//system("read X");

	pff("\n\nRequisição: ");
	if (call > 5 && call < 10)
	{
		// Interrupções não implementadas
		_RED_
	}
	else
	{
		// Interrupções implementadas
		_LIGHT_RED_
	}

	imprimirNomeDaInterrupcao(call);
	pff("\n");

	_DEFAULT_

	switch (call)
	{

	case PROCESS_INTERRUPT:

		pthread_create(&THREAD_PROCESS_INTERRUPT, &THREAD_PROCESS_INTERRUPT_ATTR, (void*) &processInterrupt, NULL );

		break;

	case PROCESS_CREATE:

		pthread_create(&THREAD_PROCESS_CREATE, &THREAD_PROCESS_CREATE_ATTR, (void*) &processCreate,
				NULL );

		break;

	case PROCESS_FINISH:

		pthread_create(&THREAD_PROCESS_FINISH, &THREAD_PROCESS_FINISH_ATTR, (void*) &processFinish, NULL );

		break;

	case SEMAPHORE_P:

		pthread_create(&THREAD_SEMAPHORE_P, &THREAD_SEMAPHORE_P_ATTR, (void*) &semaphoreP, NULL );

		break;
	case SEMAPHORE_V:

		pthread_create(&THREAD_SEMAPHORE_V, &THREAD_SEMAPHORE_V_ATTR, (void*) &semaphoreV, NULL );

		break;

	case IO_REQUEST:

		pthread_create(&THREAD_IO_REQUEST, &THREAD_IO_REQUEST_ATTR, (void*) &ioRequest, NULL );

		break;
	case IO_FINISH:
		pthread_create(&THREAD_IO_FINISH, &THREAD_IO_FINISH_ATTR, (void*) &ioFinish, NULL );

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
		{

			//Varre toda a lista de processos e pega o primeiro processo de maior prioridade

			for (int i = 0; i < NIVEIS_DE_PRIORIDADE; i++)
			{

				if ((LISTAS_DE_PROCESSOS + i)->tamanho > 0)
				{

					//Existem processos nessa lista.
					//Pega o primeiro.

					lista_obter_elemento(LISTAS_DE_PROCESSOS + i, &eleito, LISTA_INICIO);

					_CYAN_
					pff("\nEscalonador\t = ");
					_LIGHT_BLUE_
					printf("Eleito o processo %d com prioridade %d na lista %d para ocupar a CPU.", eleito->pid,
							eleito->prioridade, i);
					fflush(stdout);
					break;     //Sai da busca...
				}
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

		//Gerencia o que fazer com o processo quando ele sair da CPU.
		switch (motivoDeParada)
		{

		case PROCESS_INTERRUPT:

			//Final de quantum. Processo vai para o final da lista.
			sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);
			{
				char listaNome[20];
				_CYAN_
				pff("\nEscalonador\t = ");
				_LIGHT_BLUE_
				printf("Inserindo processo %d de prioridade %d na fila %d.", eleito->pid,
						eleito->prioridade,
						eleito->prioridade);
				_DEFAULT_
				_CYAN_
				pff("\nEscalonador\t = ");
				lista_inserir(LISTAS_DE_PROCESSOS + eleito->prioridade, eleito, 2);
				sprintf(listaNome, "Lista de Prioridade %d", eleito->prioridade);
				lista_imprimir(LISTAS_DE_PROCESSOS + eleito->prioridade, listaNome);
				pff(".");
			}
			sem_post(&SEMAFORO_LISTA_DE_PROCESSOS);

			break;

		case PROCESS_CREATE:
			//Final de quantum. Processo que saiu do processador entra em segundo na fila da sua prioridade.

			sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);
			{
				_CYAN_
				pff("\nEscalonador\t = ");
				_LIGHT_BLUE_
				printf("Processo %d inserido na lista de prioridade %d.", eleito->pid, eleito->prioridade);
				lista_inserir(LISTAS_DE_PROCESSOS + eleito->prioridade, eleito, 1);

				_CYAN_
				pff("\nEscalonador\t = ");
				_LIGHT_PURPLE_

				printf("Lista de Prioridade %d.", eleito->prioridade);
				lista_imprimir(LISTAS_DE_PROCESSOS + eleito->prioridade, "");

			}
			sem_post(&SEMAFORO_LISTA_DE_PROCESSOS);

			break;

		case SEMAPHORE_P:
			sem_wait(&SEMAFORO_SEMAFOROS);
			{
				_CYAN_
				pff("\nEscalonador\t = ");
				_LIGHT_BLUE_
				printf("Processo %d inserido na lista de bloqueados.", eleito->pid);
				lista_inserir(LISTA_DE_BLOQUEADOS_POR_SEMAFORO, eleito, LISTA_INICIO);

				_CYAN_
				pff("\nEscalonador\t = ");

				_LIGHT_BLUE_

				lista_imprimir(LISTA_DE_BLOQUEADOS_POR_SEMAFORO, "Lista de Bloqueados");
				pff(".");

				eleito->estado = ESTADO_BLOQUEADO;

			}
			sem_post(&SEMAFORO_SEMAFOROS);

			break;

		case SEMAPHORE_V:

			// Sai do processador para o processo que acordou trabalhar...

			sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);
			{
				lista_inserir(LISTAS_DE_PROCESSOS + eleito->prioridade, eleito, 2);
			}
			sem_post(&SEMAFORO_LISTA_DE_PROCESSOS);

			break;

		case PROCESS_FINISH:

			matarProcesso(eleito);

			break;

		case IO_REQUEST:

			sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);
			{
				_CYAN_
				pff("\nEscalonador\t = ");
				_LIGHT_BLUE_
				printf("Processo %d inserido na lista de bloqueados por I/O.", eleito->pid);
				fflush(stdout);
				lista_inserir(LISTA_DE_BLOQUEADOS_POR_ENTRADA_E_SAIDA, eleito, LISTA_FIM);
				_CYAN_
				pff("\nEscalonador\t = ");

				lista_imprimir(LISTA_DE_BLOQUEADOS_POR_ENTRADA_E_SAIDA, "Lista de Bloqueados por I/O");

			}
			sem_post(&SEMAFORO_LISTA_DE_PROCESSOS);

			break;

		case IO_FINISH:

			//Quando um processo é parado por que outro acabou sua operação de entrada e saida,
			//ele deve ser colocado para ser executado assim que possivel (inicio da fila 0)
			sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);
			{
				lista_inserir(LISTAS_DE_PROCESSOS, eleito, LISTA_INICIO);
			}
			sem_post(&SEMAFORO_LISTA_DE_PROCESSOS);

			break;

		}

	}

}

int processar(bcp *processo)     //Pula pula :D
{

	int abortar;

	sem_wait(&SEMAFORO_PROCESSADOR);
	{

		PROCESSO_NA_CPU = processo;

		ABORTAR_PROCESSAMENTO = NAO_ABORTAR;		// Libera o processador para trabalhar...

		abortar = ABORTAR_PROCESSAMENTO;
	}
	sem_post(&SEMAFORO_PROCESSADOR);

	processo->estado = ESTADO_RODANDO;
	_CYAN_
	pff("\nProcessador\t = ");
	_LIGHT_CYAN_
	printf("Processando %d:", processo->pid);
	fflush(stdout);
	_DEFAULT_

	while (abortar == NAO_ABORTAR)
	{

		processo->pc++;

		if (processo->pc % 1000000 == 0)
		{

			_CPU_TICK_
			pff(":");
			_DEFAULT_
		}

		//Verifica se é necessário abortar ou não.
		sem_wait(&SEMAFORO_PROCESSADOR);

		abortar = ABORTAR_PROCESSAMENTO;

		if (abortar)
		{
			PROCESSO_NA_CPU = NULL;
		}

		sem_post(&SEMAFORO_PROCESSADOR);

	}
	_CYAN_
	pff("\nProcessador\t = ");
	_LIGHT_BLUE_
	printf("Processo %d saiu da CPU. Motivo: ", processo->pid);
	fflush(stdout);
	_YELLOW_
	imprimirNomeDaInterrupcao(abortar);

	_DEFAULT_
	pff(".");
	//Devolve o motivo de ter abortado o processamento.

	return abortar;

}

int sysCall()
{

	//Sorteia uma chama de sistema aleatória que faça sentido.
	int call = (rand() % TIPOS_DE_INTERRUPCAO) + 1;

	// Se não houver ninguem processando cria alguém.
	if (ABORTAR_PROCESSAMENTO)
	{

		do
		{
			call = (rand() % TIPOS_DE_INTERRUPCAO) + 1;
		}
		while (call != 2);

	}
	else
	{

		// Só dispara IO_FINISH se houver alguém aguardando por IO
		if (call == IO_FINISH)
		{

			sem_wait(&SEMAFORO_DISK);
			{
				if (!REQUISICAO_EM_EXECUCAO)
				{
					//Sustitui uma IO_FINISH inválida por uma PROCESS_INTERRUPT
					call = IO_REQUEST;
				}
			}
			sem_post(&SEMAFORO_DISK);
		}

		// Estão ficando muitos processos travados no semaforo.
		// Então apenas 20% das SEM_WAIT serão disparadas,
		// as outras 80% serão transformadas em SEM_POST.

		if (call == SEMAPHORE_P)
		{
			int i = rand() % 100;
			if (i > 20)
			{
				call = SEMAPHORE_V;
			}
		}

	}

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
	pff("\nProcess Create\t = ");
	_LIGHT_GREEN_
	printf(
			"Processo %d criado com prioridade %d e inserido na fila de prioridade 0 momentâneamente.",
			p->pid,
			p->prioridade);
	fflush(stdout);
	_DEFAULT_

	sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);
	{

		lista_inserir(LISTAS_DE_PROCESSOS, p, LISTA_INICIO);

		_CYAN_
		pff("\nProcess Create\t = ");
		lista_imprimir(LISTAS_DE_PROCESSOS, "Lista de Prioridade 0");

		sem_wait(&SEMAFORO_PROCESSADOR);
		{
			ABORTAR_PROCESSAMENTO = PROCESS_CREATE;
		}
		sem_post(&SEMAFORO_PROCESSADOR);

	}
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
	if (LISTA_DE_BLOQUEADOS_POR_SEMAFORO->tamanho <= 0)
	{
		sem_post(&SEMAFORO_SEMAFOROS);
		return;
	}
	elemento = rand() % LISTA_DE_BLOQUEADOS_POR_SEMAFORO->tamanho;

	lista_obter_elemento(LISTA_DE_BLOQUEADOS_POR_SEMAFORO, &desbloqueado, elemento);

	_CYAN_

	pff("\nSemaphore V = ");
	_LIGHT_BLUE_
	printf("Acordando processo %d com prioridade %d que estava bloqueado.", desbloqueado->pid,
			desbloqueado->prioridade);
	fflush(stdout);

	_DEFAULT_

	sem_post(&SEMAFORO_SEMAFOROS);

	sem_wait(&SEMAFORO_LISTA_DE_PROCESSOS);

// Coloca o processo na lista de processo de maior prioridade, para que seja
// processado

	_CYAN_

	pff("\nSemaphore V = ");
	_LIGHT_BLUE_
	printf("Inserindo processo %d com prioridade %d na fila 0.", desbloqueado->pid,
			desbloqueado->prioridade);
	fflush(stdout);

	desbloqueado->estado = ESTADO_PRONTO;
	lista_inserir(LISTAS_DE_PROCESSOS, desbloqueado, 1);
	_CYAN_
	pff("\nSemaphore V = ");
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
		pff("MEM_LOAD_REQ [NÃO IMPLEMENTADA]");
		break;
	case MEM_LOAD_FINISH:
		pff("MEM_LOAD_FINISH [NÃO IMPLEMENTADA]");
		break;
	case FS_REQUEST:
		pff("FS_REQUEST [NÃO IMPLEMENTADA]");
		break;
	case FS_FINISH:
		pff("FS_FINISH [NÃO IMPLEMENTADA]");
		break;
	case SEMAPHORE_P:
		pff("SEMAPHORE_P");
		break;
	case SEMAPHORE_V:
		pff("SEMAPHORE_V");
		break;
	}

}

void ioRequest()
{

	//Gera uma requisição de I/O e envia para a DiskControl

	bcp *solicitante;

	sem_wait(&SEMAFORO_PROCESSADOR);
	{
		solicitante = PROCESSO_NA_CPU;
		ABORTAR_PROCESSAMENTO = IO_REQUEST;
	}
	sem_post(&SEMAFORO_PROCESSADOR);

	lista_query *requisicao = gerarRequisicaoDeDisco(solicitante);

	sem_wait(&SEMAFORO_LISTAS_DE_REQUISICOES_DE_DISCO);
	{
		lista_REQUEST_inserir(LISTA_DE_REQUISICOES_DE_DISCO_PENDENTES, requisicao);
	}

	sem_post(&SEMAFORO_LISTAS_DE_REQUISICOES_DE_DISCO);

	// Monta argumento para DiskControl
	argDC *paramentroDC = (argDC*) malloc(sizeof(argDC));

	paramentroDC->block = requisicao->addr;
	paramentroDC->rw = rand() % 2;
	paramentroDC->from = 1;     //IO_REQUEST

	pthread_create(&THREAD_DISKCONTROL, &THREAD_DISKCONTROL_ATTR, (void*) &runDiskControl, paramentroDC);

}

void ioFinish()
{
	sem_wait(&SEMAFORO_PROCESSADOR);
	{
		ABORTAR_PROCESSAMENTO = IO_FINISH;

	}
	sem_post(&SEMAFORO_PROCESSADOR);

	// Monta argumento para DiskControl

	argDC *paramentroDC = (argDC*) malloc(sizeof(argDC));
	paramentroDC->block = 0;
	paramentroDC->rw = 0;
	paramentroDC->from = 2;     //IO_FINISH

	pthread_create(&THREAD_DISKCONTROL, &THREAD_DISKCONTROL_ATTR, (void*) &runDiskControl, paramentroDC);

}

// Funções não implementadas ==============================
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

