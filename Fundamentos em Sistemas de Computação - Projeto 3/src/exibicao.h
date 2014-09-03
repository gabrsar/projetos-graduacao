/*
 * exibicao.h
 *
 *  Created on: 09/11/2012
 *      Author: gabriel
 */

#ifndef EXIBICAO_H_
#define EXIBICAO_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdarg.h>
#
#define __UTILIZAR_COR__

#ifdef __UTILIZAR_COR__
#define _BLACK_            		"\033[0;30m"
#define _BLUE_             	 	"\033[0;34m"
#define _GREEN_            		"\033[0;32m"
#define _CYAN_             	 	"\033[0;36m"
#define _RED_               	"\033[0;31m"
#define _PURPLE_           	 	"\033[0;35m"
#define _BROWN_            	 	"\033[0;33m"
#define _LIGHT_GRAY_       	 	"\033[1;37m"
#define _DARK_GRAY_         	"\033[1;30m"
#define _LIGHT_BLUE_        	"\033[1;34m"
#define _LIGHT_GREEN_       	"\033[1;32m"
#define _LIGHT_CYAN_        	"\033[1;36m"
#define _LIGHT_RED_         	"\033[1;31m"
#define _LIGHT_PURPLE_      	"\033[1;35m"
#define _YELLOW_            	"\033[1;33m"
#define _WHITE_             	"\033[1;37m"
#define _DEFAULT_COLOR_			"\033[0m"
#define _BLACK_OVER_LIGHT_BLUE_ "\033[46;30m"

#define _COR_TEXTO_PADRAO_  	_DEFAULT_COLOR_
#define _COR_T1_				_LIGHT_CYAN_
#define _COR_T2_				_LIGHT_PURPLE_
#define _COR_T1_TEXTO_			_CYAN_
#define _COR_T2_TEXTO_			_PURPLE_
#define _COR_PACOTE_TIPO_		_PURPLE_
#define _COR_PACOTE_INFORMACAO_ _BROWN_
#define _COR_T1_SUCESSO_ 		_BLACK_OVER_LIGHT_BLUE_
#endif

int _TERMINAL_COLUNAS_;
int _TERMINAL_LINHAS_;
int _USAR_ALINHAMENTO_DE_TEXTO;

#define _EXIBICAO_ESQUERDA_ 0
#define _EXIBICAO_DIREITA_ 1
#define _EXIBICAO_CENTRO_ 2

void exibicao_iniciar();
void pff(char *texto);
void TPFF(int t, char *formato, int argc, ...);
void TPFFH(int t);
int exibicao_contarEspeciais(char *s);

#endif /* EXIBICAO_H_ */
