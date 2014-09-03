/*
 * exibibicao.h
 *
 *  Created on: Oct 1, 2012
 *      Author: gabriel
 */

#ifndef EXIBICAO_H_
#define EXIBICAO_H_

#include <stdio.h>
#include <stdlib.h>

//Para não utilizar a função de limpar a tela apenas troque os comentários das linhas
//#define _LIMPAR_TELA_
#define _LIMPAR_TELA_ 	system("clear");

//Comente a linha abaixo para habilitar ou desabilitar o uso de cores
//O uso de cores é altamente recomendado pois facilita muito a visualização
//do que está ocorrendo no sistema.
#define __UTILIZAR_COR__

#ifdef __UTILIZAR_COR__
#define _DEFAULT_         	pff("\033[0m");
#define _BLACK_         	pff("\033[0;30m");
#define _BLUE_              pff("\033[0;34m");
#define _GREEN_             pff("\033[0;32m");
#define _CYAN_              pff("\033[0;36m");
#define _RED_               pff("\033[0;31m");
#define _PURPLE_            pff("\033[0;35m");
#define _BROWN_             pff("\033[0;33m");
#define _LIGHT_GRAY_        pff("\033[1;37m");
#define _DARK_GRAY_         pff("\033[1;30m");
#define _LIGHT_BLUE_        pff("\033[1;34m");
#define _LIGHT_GREEN_       pff("\033[1;32m");
#define _LIGHT_CYAN_        pff("\033[1;36m");
#define _LIGHT_RED_         pff("\033[1;31m");
#define _LIGHT_PURPLE_      pff("\033[1;35m");
#define _YELLOW_            pff("\033[1;33m");
#define _WHITE_             pff("\033[1;37m");
#define _CPU_TICK_			pff("\033[1;47;30m");

#else

#define _DEFAULT_
#define _BLACK_
#define _BLUE_
#define _GREEN_
#define _CYAN_
#define _RED_
#define _PURPLE_
#define _BROWN_
#define _LIGHT_GRAY_
#define _DARK_GRAY_
#define _LIGHT_BLUE_
#define _LIGHT_GREEN_
#define _LIGHT_CYAN_
#define _LIGHT_RED_
#define _LIGHT_PURPLE_
#define _YELLOW_
#define _WHITE_
#define _CPU_TICK_

#endif //__UTILIZAR_COR__

// Função que é um printf simples junto com um fflush(stdout). Apenas para simplificar.
void pff(char * texto);



#endif /* EXIBIBICAO_H_ */
