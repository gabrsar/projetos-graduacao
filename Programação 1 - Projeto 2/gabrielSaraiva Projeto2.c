/*
 * Created on: Apr 22, 2010 ( 04:54:07 am )
 * Author: Gabriel Henrique Martinez Saraiva
 *
 * Projeto 2 ==================================================================
 *
 * Escreva um programa que apresente uma tabela de conversão
 * entre duas medidas de consumo de combustível bastante usadas,
 * que são quilometros por litro e milhas por galão.
 *
 * Sua tabela deve ser construída em duas etapas.
 * A primeira tendo como base quilometros por litro, entre 2km/l e 20km/l.
 * A segunda tendo como base milhas por galão, entre 10mpg e 40mpg.
 *
 *
 * Nas duas bases deve-se usar uma unidade como discretização da tabela.
 * Use também o galão como valendo 3,7 litros.
 *
 * ENTREGA EM: 30/04/2008, por e-mail para aleardo@sjrp.unesp.br
 *
 * ============================================================================
 *
 * Modelo para descobrir a conversao:
 *
 * distancia / combstivel = rendimento
 *
 * rendimento = distancia/combustivel
 *
 * distancia = rendimento/combustivel
 *
 * combustivel = rendimento*distancia
 *
 * i = distancia.
 * MPG = ((i*KM)/MI) * GL
 * KML = ((i*MI)/KM) / GL
 *
 * Algoritmo:
 *
 * CONSTANTE decimal MI	=	1.609
 * CONSTANTE decimal KM	=	1
 * CONSTANTE decimal GL	=	3.7
 * CONSTANTE decimal L 	=	1
 *
 * DECLARAR VARIAVEL inteiro KMLfaixaInicio = 02
 * DECLARAR VARIAVEL inteiro KMLfaixaFinal	= 20
 *
 * DECLARAR VARIAVEL inteiro MPGfaixaInicio = 10
 * DECLARAR VARIAVEL inteiro MPGfaixaFinal	= 40
 *
 * DECLARAR VARIAVEL inteiro I
 * DECLARAR VARIAVEL decimal KML
 * DECLARAR VARIAVEL decimal MPG
 *
 * PARA i= KMLfaixaInicio ATE KMLfaixaFinal FACA
 *
 * 		MPG = ((i*KM)/MI) * GL
 * 		EXIBA(i + "km/l ---> " + MPG + "mpg")
 *
 * FIM PARA
 *
 * PARA i= MPGfaixaInicio ATE MPGfaixaFinal FACA
 *
 * 		KML = ((i*MI)/KM) / GL
 * 		EXIBA(i + "mpg ---> " + KML + "km/l")
 *
 * FIM PARA
 *
 */

//Bibliotecas
#include <stdio.h>


//Constantes
#define KM  1.000 //metros
#define MI  1.609 //metros
#define L   1.000 //litros
#define GL  3.700 //litros

main(){
	int KMLfaixaInicio, KMLfaixaFinal;
	int MPGfaixaInicio, MPGfaixaFinal;
	float mpg=0,kml=0;
	int i=0; // Contador utilizado para as conversoes, faz o papel da distancia.

	KMLfaixaInicio=2;  KMLfaixaFinal=20; // Variacao ( 2 a 20)
	MPGfaixaInicio=10; MPGfaixaFinal=40; // Variacao (10 a 40)

	printf("Conversao de km/l para mpg. (de 02 a 20 km/l)\n");
	printf(" _________________________________\n");

	for (i = KMLfaixaInicio ; i <= KMLfaixaFinal ; i++ ){

		mpg = ((i * KM)/MI) * GL; // Calculo da conversao

		printf("|%3dkm/l\t|\t%5.1fmpg |\n",i,mpg);
	}
	printf(" ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");
	printf("\n\n");



	printf("Conversao de mpg para km/l. (de 10 a 40 mpg)\n");
	printf(" _________________________________\n");
	for (i=MPGfaixaInicio;i<=MPGfaixaFinal;i++){


		kml = ((i*MI)/KM) /GL; // Calculo da conversao

		printf("|%3dmpg\t\t|\t%5.1fkm/l |\n",i,kml);

	}
	printf(" ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");

}
