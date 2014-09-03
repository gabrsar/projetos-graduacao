/* Universidade Estadual Paulista - "JÚLIO DE MESQUITA FILHO" - S.J.R.P.
 * Professor: Aleardo Manacero Junior. Monitor: Thiago Brait.
 * =============================================================================
 * Biblioteca com funções para manuseio de números complexos na forma algébrica.
 * Desenvolvido por Gabriel Henrique Martinez Saraiva, como 3º Projeto de
 * Programação I. 
 * =============================================================================
 */

//Bibliotecas necessárias 
#include "complexos.h"

// Função resposável por receber um número na forma complexa.
struct complexo receberComplexo(){

	float r,i;
	printf("Parte real:");	
	scanf("%f",&r);
	printf("Parte imaginária:");	
	scanf("%f",&i);
	struct complexo c;
	c.real = r;
	c.imaginario = i;

	return c;
}

/* Função resposável pela apresentação ao usuário do número complexo na forma
 * a+bi. Ex: 3+5i ou 3-5i
 *
 * Poderia ser retornado uma string (simplificaria o uso da função), mas foi 
 * por convenção evita-se o uso desse recurso em C. Então a função devolve o
 * texto final por uma váriavel passada por parametro como ponteiro.
 */
void complexoToString(char *destino, struct complexo numeroComplexo){

	char texto[TAMANHO_MAXIMO_PARA_STRING]; //Texto final
	char textoReal[TAMANHO_MAXIMO_PARA_STRING/2];
	char textoImaginario[(TAMANHO_MAXIMO_PARA_STRING-1)/2];
	

	sprintf(textoReal,	"%g",numeroComplexo.real);
	sprintf(textoImaginario,"%g",numeroComplexo.imaginario);

	
	strcpy(texto,textoReal);
	
	//Adiciona o sinal de + em "a+bi" caso a parte imáginaria seja positiva.
	if(numeroComplexo.imaginario >=0){
		strcat(texto,"+");
	}
	
	//Monta a string.
	strcat(texto,textoImaginario);
	strcat(texto,"i");
	strcpy(destino,texto);
	
}

// Realiza a soma de 02 números complexos. Retorna um número complexo como resultado.
struct complexo soma(struct complexo a,struct complexo b){
	struct complexo c;
	
	c.real = a.real + b.real;
	c.imaginario = a.imaginario + b.imaginario;
	
	return c;
}

// Realiza a subtração de 02 números complexos. Retorna um número complexo como resultado.
struct complexo subtracao(struct complexo a,struct complexo b){
	struct complexo c;
	
	c.real = a.real - b.real;
	c.imaginario = a.imaginario - b.imaginario;
	
	return c;
}

// Retorna o conjugado do número complexo. Retorna um número complexo como resultado.
struct complexo conjugado(struct complexo a){
	struct complexo c;
	
	c.real = a.real;
	c.imaginario = -a.imaginario;
	
	return c;
}

// Multiplica 02 números complexos. Retorna um número complexo como resultado.
struct complexo multiplicacao(struct complexo a,struct complexo b){
	struct complexo c;

	c.real = (a.real * b.real) + (-1*(a.imaginario * b.imaginario));
	c.imaginario = (a.real*b.imaginario) + (b.real*a.imaginario);

	return c;

}
// Realiza a divisão do complexo 'a' pelo complexo 'b'. Retorna um número complexo como resultado.
struct complexo divisao(struct complexo a,struct complexo b){
	struct complexo c;

	c.real = (a.real*b.real + a.imaginario*b.imaginario)/(b.real*b.real + b.imaginario*b.imaginario);
	c.imaginario = (a.imaginario*b.real - a.real*b.imaginario)/(b.real*b.real + b.imaginario*b.imaginario);

	return c;
}
