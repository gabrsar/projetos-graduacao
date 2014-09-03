/*
 * main.c
 *
 *  Created on: 08/11/2012
 *      Author: gabriel
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "listaGenerica/listaGenerica.h"
#include "exibicao.h"
#include "thread1.h"
#include "thread2.h"

#define TAMANHO_MAXIMO_MENSAGEM (1024*1024) //1MB
void iniciar();
void carregarDadosDeMensagens();
void apresentacao();

int main()
{

	exibicao_iniciar();

	// Para deixar as duas trheads alinhadas a esquerda alterar o valor de _USAR_ALINHAMENTO_DE_TEXTO como 0
	_USAR_ALINHAMENTO_DE_TEXTO = 1;

	system("clear");
	apresentacao();
	carregarDadosDeMensagens();
	iniciar();

	return 0; // Não vai chegar aqui nunca :( Tadinho...

}

void iniciar()
{

	srand(time(NULL ));

	THREAD1_TEMPO_SLEEP = UM_SEGUNDO;
	THREAD2_TEMPO_SLEEP = UM_SEGUNDO;

	// Coloca um pacote vazio inválido no canal.
	PONTO_DE_TRANSMISSAO.numero = -1;
	PONTO_DE_TRANSMISSAO.tempoDeVida = -1;
	PONTO_DE_TRANSMISSAO.tipo = PACOTE_TIPO_VAZIO;

	pthread_t THREAD1;
	pthread_t THREAD2;

	pthread_attr_t THREAD1_ATTR;
	pthread_attr_t THREAD2_ATTR;

	pthread_attr_init(&THREAD1_ATTR);
	pthread_attr_init(&THREAD2_ATTR);

	pthread_create(&THREAD1, &THREAD1_ATTR, (void*) &thread1_run, NULL );
	pthread_create(&THREAD2, &THREAD2_ATTR, (void*) &thread2_run, NULL );

	while (1)
	{
		sleep(10000); //Evita espera ocupada pela thread principal
	}
}

void carregarDadosDeMensagens()
{

	printf("Carregando arquivo de mensagens: 'MENSAGEM.txt'\n");

	FILE *arquivoDeDados = fopen("MENSAGEM.txt", "r");
	size_t tamanho;
	if (arquivoDeDados)
	{
		fseek(arquivoDeDados, 0L, SEEK_END);
		size_t tamanho = ftell(arquivoDeDados);
		rewind(arquivoDeDados);

		tamanho = tamanho % TAMANHO_MAXIMO_MENSAGEM;

		MENSAGEM = (char*) malloc(sizeof(char) * tamanho + 1);

		fread(MENSAGEM, sizeof(char), tamanho, arquivoDeDados);

		MENSAGEM[tamanho] = '\0';


		fclose(arquivoDeDados);

		printf("Mensagem carregada. Tamanho total: %d bytes.\n", (int) tamanho);
	}
	else
	{
		char semMensagem[] = "ABCDEFGHIJKLMNOPQRSTUVXYWZ";
		tamanho = strlen(semMensagem);
		MENSAGEM = (char*) malloc(sizeof(char) * tamanho + 1);
		strcpy(MENSAGEM, semMensagem);
		printf("ERRO! Não foi possivel abrir o path. Utilizando mensagem de aviso.\n");
	}

}

void apresentacao()
{
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	printf(" _______________________________________________________________________________\n");
	printf("|                                                                               |\n");
	printf("|             3º Projeto de Fundamentos em Sistemas de Computação               |\n");
	printf("| ----------------------------------------------------------------------------- |\n");
	printf("|                                                                               |\n");
	printf("|       Universidade Estadual Paulista \"Júlio de Mesquita Filho\" (UNESP)        |\n");
	printf("|              Instituto de Biociências, Letras e Ciências Exatas               |\n");
	printf("|                                                                               |\n");
	printf("| Professor: Dr. Aleardo Manacero Jr.                                           |\n");
	printf("|                                                                               |\n");
	printf("|    Alunos: Gabriel Henrique Martinez Saraiva                                  |\n");
	printf("|_______________________________________________________________________________|\n");

	printf("\n\n\n");

	printf("Pressione ENTER para iniciar.");
	fflush(stdout);
	system("read _FSC3_VAR_");

	printf("\r\033[1A");
	printf("                             \n");

}

