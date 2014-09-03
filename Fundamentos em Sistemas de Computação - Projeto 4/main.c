/* main.c
 *
 * Created on: 30/11/2012
 *
 * 4º Projeto de Fundamentos em Sistemas de Computação.
 *
 * Professor: Dr. Aleardo Manacero Jr.
 *
 * Author: Gabriel Henrique Martinez Saraiva
 *
 * Descrição do projeto: http://www.dcce.ibilce.unesp.br/~aleardo/cursos/fsc/proj4.php
 *
 * Endereçamento IPv4
 *
 * Objetivo:
 * Implementar um conversor de endereços (bidirecional), que transforme
 * endereços IP em três valores, um caracter indicando a classe da rede
 * e dois inteiros identificando os endereços da rede e da máquina na
 * rede, e vice-versa (3 valores para IP).
 *
 * As entradas para o programa são um arquivo com uma sequência de endereços,
 * em que em cada linha é formada por um inteiro (que  * dá o tipo de conversão,
 * sendo 1=IP->trio e -1=trio->IP e 0=fim do programa), seguido ou de uma letra
 * e dois inteiros ou de umendereço composto por quatro inteiros separados por
 * pontos (numa única string).
 *
 * Seu programa deve também verificar se o endereço formado é válido ou não,
 * segundo os limites do protocolo IPv4.
 *
 * A saída é outro arquivo com a lista dos endereços correspondentes.
 *
 * Formatação do arquivo de entrada:
 *
 * 1 192.3.45.192
 * -1 B 2345 23
 * -1 C 65438 127
 * 1 3.4.5.6
 * 0
 *
 * Data de entrega:
 * 03/12/2012, até as 18 horas (horário de recebimento da mensagem).
 */

/* Textos utilizado para refencia sobre classes de ips
 * http://www.cisco.com/en/US/tech/tk365/technologies_tech_note09186a00800a67f5.shtml#reqs
 * http://cisco.iphelp.ru/faq/5/ch01lev1sec1.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Operações
#define CONVERTER_IP_PARA_TRIO 1
#define CONVERTER_TRIO_PARA_IP -1
#define SAIR 0

// Definições de tamanhos
#define BYTE_SIZE 8

#define IP_BITS 32
#define OCTETO_SIZE BYTE_SIZE

#define A_BYTES_REDE 8
#define A_BYTES_HOST 24

#define B_BYTES_REDE 16
#define B_BYTES_HOST 16

#define C_BYTES_REDE 24
#define C_BYTES_HOST 8

#define D_BYTES_REDE 0 // Não especificado
#define D_BYTES_HOST 0 // Não especificado
#define E_BYTES_REDE 0 // Não especificado
#define E_BYTES_HOST 0 // Não especificado
#define TAMANHO_NOME_ARQUIVO 256

// Tipos de dados
typedef unsigned int uint;

// Representação do TRIO.
typedef struct {
	char classe;
	uint rede;
	uint maquina;
} trio_t;

// Representação do IPv4.
typedef struct {
	uint octeto[4];
} ipv4_t;

// Cabeçalhos
int ler_operacao(FILE *arquivo);

int ler_ip(FILE *arquivo, ipv4_t *out);
int ler_trio(FILE *arquivo, trio_t *out);

int imprimir_ip(ipv4_t ip);
int imprimir_trio(trio_t trio);

void gravar_ip(FILE *arquivo, ipv4_t ip);
void gravar_trio(FILE *arquivo, trio_t trio);

int validar_ip(ipv4_t ip);
int validar_trio(trio_t trio);

trio_t converter_ip_para_trio(ipv4_t ip);
ipv4_t converter_trio_para_ip(trio_t trio);

uint converter_binario_para_decimal(char *in);
void converter_decimal_para_binario(int in, char *bin, int numero_de_bits);

int g_pow(int base, int potencia);

void creditos();
void help(char *nome);

// Final dos cabeçalhos.

int main(int argc, char **argv) {

	if (argc != 3) {
		help(argv[0]);
		exit(-1);
	}

	char entrada[TAMANHO_NOME_ARQUIVO];
	char saida[TAMANHO_NOME_ARQUIVO];

	strcpy(entrada, argv[1]);
	strcpy(saida, argv[2]);

	FILE *arquivo_entrada = fopen(entrada, "r");

	if (!arquivo_entrada) {
		printf("\nErro ao abrir o arquivo '%s' para leitura.\n", entrada);
		printf("Verifique se o arquivo existe e se as permissões estão corretas.\n\n");

		exit(-2);
	}

	FILE *arquivo_saida = fopen(saida, "w");

	if (!arquivo_saida) {
		printf("\nErro ao abrir o arquivo '%s' para escrita dos resultados.\n", saida);
		printf("Verifique se o arquivo já existe (se existir ele será sobrescrito!),");
		printf("e se as permissões estão corretas.\n\n");

		exit(-3);
	}

	// Tudo certo para começar...
	int continuar = 1;
	do {
		//Lê o que deve ser feito (primeira coluna no arquivo)
		int operacao = ler_operacao(arquivo_entrada);

		ipv4_t ip;
		trio_t trio;

		switch (operacao) {
		case CONVERTER_IP_PARA_TRIO:

			if (!ler_ip(arquivo_entrada, &ip)) {

				continue;
			}

			trio = converter_ip_para_trio(ip);

			gravar_trio(arquivo_saida, trio);

			imprimir_ip(ip);
			printf(" -> ");
			imprimir_trio(trio);
			printf("\n");

			break;
		case CONVERTER_TRIO_PARA_IP:

			if (!ler_trio(arquivo_entrada, &trio)) {

				continue;
			}

			ip = converter_trio_para_ip(trio);

			gravar_ip(arquivo_saida, ip);

			imprimir_trio(trio);
			printf(" -> ");
			imprimir_ip(ip);
			printf("\n");

			break;
		case SAIR:
			continuar = 0;
			break;
		default:
			printf("\nCódigo de operação não identificado. Abortando execução.\n\n");
			fclose(arquivo_entrada);
			fclose(arquivo_saida);
			exit(-4);

		}
	} while (continuar);

	fclose(arquivo_entrada);
	fclose(arquivo_saida);

	// That's is all Folks :)
	creditos();
	return EXIT_SUCCESS;
}

int ler_operacao(FILE *arquivo) {

	/*
	 * Lê a operação que deve ser executada (trio->ip ou ip->trio)
	 * e a retorna.
	 */

	int operacao;

	fscanf(arquivo, "%d", &operacao);

	// Remove o espaço no arquivo depois da operação.
	// Deixa o aqruivo pronto para executar a proxima operação.
	fgetc(arquivo);

	return operacao;
}

int ler_ip(FILE *arquivo, ipv4_t *out) {

	/*
	 * Lê um ip no arquivo. Verifica se ele é válido. Só grava o ip em out se for válido.
	 * Retorna 1 se o ip for válido.
	 * Retorna 0 se o ip não for válido.
	 */

	if (!arquivo) {
		return 0;
	}

	if (!out) {
		return 0;
	}

	ipv4_t ip;
	char octeto[4];

	char c;

	for (int i = 0; i < 4; i++) {

		for (int j = 0;; j++) {

			c = fgetc(arquivo);

			if (feof(arquivo)) {
				break;
			}

			if (c == '.' || c < '0' || c > '9') {
				octeto[j] = '\0';
				break;
			}
			octeto[j] = c;
		}

		ip.octeto[i] = atoi(octeto);
	}

	if (validar_ip(ip)) {
		*out = ip;
		return 1;
	}

	return 0;
}

int ler_trio(FILE *arquivo, trio_t *out) {

	/*
	 * Lê um trio no arquivo. Verifica se ele é válido. Só grava o trio em out se for válido.
	 * Retorna 1 se o trio for válido.
	 * Retorna 0 se o trio não for válido.
	 */

	trio_t trio;

	if (!arquivo) {
		return 0;
	}

	if (!out) {
		return 0;
	}

	trio.classe = fgetc(arquivo);

	fscanf(arquivo, "%d", &trio.rede);

	fgetc(arquivo);

	fscanf(arquivo, "%d", &trio.maquina);

	if (validar_trio(trio)) {
		*out = trio;
		return 1;
	}
	return 0;

}

int imprimir_ip(ipv4_t ip) {

	/*
	 * Grava um IP e depois uma quebra de linha no arquivo
	 */

	return printf("%u.%u.%u.%u", ip.octeto[0], ip.octeto[1], ip.octeto[2], ip.octeto[3]);
}

int imprimir_trio(trio_t trio) {

	/*
	 * Grava um TRIO e depois uma quebra de linha no arquivo
	 */

	return printf("%c %u %u", trio.classe, trio.rede, trio.maquina);
}

void gravar_ip(FILE *arquivo, ipv4_t ip) {

	/*
	 * Grava um IP e depois uma quebra de linha no arquivo
	 */

	fprintf(arquivo, "%u.%u.%u.%u\n", ip.octeto[0], ip.octeto[1], ip.octeto[2], ip.octeto[3]);
}

void gravar_trio(FILE *arquivo, trio_t trio) {

	/*
	 * Grava um TRIO e depois uma quebra de linha no arquivo
	 */

	fprintf(arquivo, "%c %u %u\n", trio.classe, trio.rede, trio.maquina);
}

int validar_ip(ipv4_t ip) {

	/*
	 * Verifica se um ip é válido ou não.
	 * Válido: Retorna 1
	 * Inválido: Retorna 0
	 * Referencia utilizada para ips válidos: http://www.cisco.com/en/US/tech/tk365/technologies_tech_note09186a00800a67f5.shtml#reqs
	 */

	int valido = 1;

	if (ip.octeto[0] < 1 || ip.octeto[0] > 254) {
		valido = 0;
	}

	for (int i = 1; i < 4; i++) {
		if (ip.octeto[i] < 0 || ip.octeto[i] > 255) {
			valido = 0;
		}
	}

	if (!valido) {
		printf("\nIP inválido encontrado: ");
		imprimir_ip(ip);
		printf("\n");
		return 0;
	}

	return 1;
}

int validar_trio(trio_t trio) {

	/*
	 * Verifica se um trio é válido ou não.
	 * Válido: Retorna 1
	 * Inválido: Retorna 0
	 */

	int valido = 1;
	if (trio.classe < 'A' || trio.classe > 'E') {
		valido = 0;
	}

	switch (trio.classe) {
	case 'A':
		if (trio.rede < 0 || trio.rede > 127) {
			valido = 0;
		}

		if (trio.maquina < 0 || trio.maquina > 16777215) {
			valido = 0;
		}
		break;
	case 'B':
		if (trio.rede < 0 || trio.rede > 16384) {
			valido = 0;
		}

		if (trio.maquina < 0 || trio.maquina > 65535) {
			valido = 0;
		}
		break;
	case 'C':
		if (trio.rede < 0 || trio.rede > 2031616) {
			valido = 0;
		}

		if (trio.maquina < 0 || trio.maquina > 255) {
			valido = 0;
		}
		break;

	case 'D':
		// Não especificado o número de redes e hosts.
		valido = 0;
		break;
	case 'E':
		// Não especificado o número de redes e hosts.
		valido = 0;
		break;
	default:
		valido = 0;
		break;

	}

	if (!valido) {
		printf("\nTRIO inválido encontrado: ");
		imprimir_trio(trio);
		printf("\n");
		return 0;
	}

	return 1;

}

trio_t converter_ip_para_trio(ipv4_t ip) {

	/*
	 * Converte um IP para um TRIO (operação 1) e retorna o trio.
	 */

	trio_t t;

	if (ip.octeto[0] >= 1 && ip.octeto[0] <= 127) {
		t.classe = 'A';

		t.rede = ip.octeto[0];
		t.maquina = ip.octeto[1] * 65536 + ip.octeto[2] * 256 + ip.octeto[3];

	}
	else if (ip.octeto[0] >= 128 && ip.octeto[0] <= 191) {
		t.classe = 'B';

		t.rede = (ip.octeto[0] - 128) * 256 + ip.octeto[1] + 1;
		t.maquina = ip.octeto[2] * 256 + ip.octeto[3];
	}
	else if (ip.octeto[0] >= 192 && ip.octeto[0] <= 223) {
		t.classe = 'C';

		t.rede = (ip.octeto[0] - 192) * (256 * 256) + ip.octeto[1] * 256 + ip.octeto[2];
		t.maquina = ip.octeto[3];
	}
	else if (ip.octeto[0] >= 224 && ip.octeto[0] <= 239) {
		t.classe = 'D';
		t.maquina = 0;
		t.rede = 0;
	}
	else if (ip.octeto[0] >= 240 && ip.octeto[0] <= 254) {
		t.classe = 'E';
		t.maquina = 0;
		t.rede = 0;
	}

	return t;
}

ipv4_t converter_trio_para_ip(trio_t trio) {

	/*
	 * Converte um TRIO para um IP (operação -1) e retorna o IP.
	 */

	/* IP: x.y.w.z */
	ipv4_t ip;

	uint z = 0;
	uint w = 0;
	uint y = 0;
	uint x = 0;

	if (trio.classe == 'A') {

		uint id = trio.maquina / 256;
		z = trio.maquina % 256;
		w = id % 256;
		y = id / 256;
		x = trio.rede;
	}
	else if (trio.classe == 'B') {

		z = trio.maquina % 256;
		w = trio.maquina / 256;
		y = (trio.rede - 1) % 256;
		x = 128 + (trio.rede - 1) / 256;
	}
	else if (trio.classe == 'C') {

		x = 192 + trio.rede / (65535);
		y = (trio.rede / 256) % 256;
		w = trio.rede % 256;
		z = trio.maquina;
	}
	else if (trio.classe == 'D' || trio.classe == 'E') {
		printf("\nCLASSE D ou E: Não existem especificações para converter redes e máquinas nessas classes.\n");
	}

	ip.octeto[3] = z;
	ip.octeto[2] = w;
	ip.octeto[1] = y;
	ip.octeto[0] = x;

	return ip;

}

uint converter_binario_para_decimal(char *in) {

	/*
	 * Converte um texto terminado em \0 de uns e zeros (em binário) em um valor inteiro e o retorna.
	 */

	uint tamanho = strlen(in);

	uint valor = 0;

	for (int i = 0; i < tamanho; i++) {

		int n = in[i] == '0' ? 0 : 1;

		if (n) {
			valor += n * g_pow(2, tamanho - i - 1);
		}
	}

	return valor;
}

void converter_decimal_para_binario(int in, char *bin, int numero_de_bits) {

	/*
	 * Converte um número inteiro de para binário e grava em 'bin' com NUMERO_DE_BITS numero de bits.
	 */

//Monta o texto em binário de traz pra frente. Assim não precisa inverter.
	bin += numero_de_bits;

	*bin = '\0';
	bin--;

	int d = 0;

	while (in) {
		*bin = in % 2 ? '1' : '0';

		bin--;
		in = in / 2;
		d++;
	}

	while (d < numero_de_bits) {
		*bin = '0';

		bin--;
		d++;
	}

}

int g_pow(int base, int potencia) {

	/*
	 * Função otimizada para cálculo de potencias para conversão de bases inteiras.
	 */

	register int p = potencia;
	register int b = base;

	register int resultado = base;

	if (p == 0) {
		return 1;
	}

	for (register int i = 1; i < p; i++) {
		resultado *= b;
	}

	return resultado;
}

void creditos() {

// Adaptado, ajustado e modificado de : http://www.chris.com/ascii/index.php?art=holiday%2Fchristmas%2Ftrees
	printf("+----------------------------------------------------------+\n");
	printf("|       *             ,                               *    |\n");
	printf("|                   _/^\\_          4º Projeto de FSC       |\n");
	printf("|                  <     >       -----------------------   |\n");
	printf("| *                 /.-.\\                                  |\n");
	printf("|          *        `/&\\`       Prof. Dr. Aleadro M. Jr.   |\n");
	printf("|                  ,@.*;@,       Aluno: Gabriel Saraiva    |\n");
	printf("|                 /_o.I $_\\    *                           |\n");
	printf("|    *           (`'--:o(_@;                         *     |\n");
	printf("|               /`;--.,__ `')             *                |\n");
	printf("|              ;@`o $ O,*`'`&\\                             |\n");
	printf("|        *    (`'--)_@ ;o $'()\\      *                     |\n");
	printf("|             /`;--._`''--._O'@;                           |\n");
	printf("|            /&*,()~o`;-.,_ `\"\"`)                 *        |\n");
	printf("| *          /`,@ ;+& () o*`;-';\\                          |\n");
	printf("|           (`\"\"--.,_0 +$ @' &()\\                          |\n");
	printf("|           /-.,_    ``''--....-'`)  *                     |\n");
	printf("|      *    /@$;o`:;'--,.__   __.'\\                        |\n");
	printf("|          ;*,&(); @ $ &^;~`\"`o;@();         *             |\n");
	printf("|          /(); o^~; & ().o@*&`;&$O\\                       |\n");
	printf("|          `\"=\"==\"\"==,,,.,=\"==\"===\"`                       |\n");
	printf("|       __.---.--.-''#####---...___...-----._          *   |\n");
	printf("|                    #####                                 |\n");
	printf("|               (\\   #####                                 |\n");
	printf("|     '`         \\)_`\"\"\"\"\"`          Desejo ao sr e        |\n");
	printf("|     *       .--' ')            toda sua familia e amigos |\n");
	printf("|           o(  )_-\\                 um Feliz Natal.       |\n");
	printf("|             `\"\"\"` `      *                       *       |\n");
	printf("+----------------------------------------------------------+\n\n\n");

}

void help(char *nome) {

	printf("\n\n");

	printf("Oooops. Você esqueceu dos parametros ou os passou errado :P\n");
	printf("Execute o programa da seguinte forma:\n\n");
	printf("%s [arquivo de entrada] [arquivo de saida]\n\n\n", nome);

}
