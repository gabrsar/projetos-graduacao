/* =============================================================================
 * Biblioteca com funções diversas.
 * Nome:libutils.h
 * Versão: 0.0.1
 * Autor:Gabriel Henrique Martinez Saraiva
 * Data:07/06/2010
 * =============================================================================
 */

#include "libutils.h"

//Função que realiza a limpeza da tela por saida ascii.	
void limparTela(){
	printf("\033[2J");
}

void pausa(){

	printf("\n\033[0mEntre com uma \033[1mletra\033[0m para continuar...\n");

	char a;
	//Pode-se utilizar scanf("%*s") mas é melhor uma letra pois assim evita o pressionamento de qualquer tecla.
	scanf(" %c",&a);

	
}

int receberInt(){
	int i;
	scanf(" %d",&i);
	return i;
}
char receberChar(){
	char c;
	scanf(" %c",&c);
	return c;
}
float receberFloat(){
	float f;
	scanf(" %f",&f);
	return f;
}
