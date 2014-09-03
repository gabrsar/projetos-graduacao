//Bibliotecas necessárias 
#include <stdio.h>
#include <string.h>

/* Tamanho maximo que um Complexo pode representar (a+bi) em caracteres contado 
 * o sinal (+ ou -) e o "i"
 */
#define TAMANHO_MAXIMO_PARA_STRING 256


/* Representação de número complexo, sem o sinal de potência.
 * Para o problema da potência espera-se que o usuário já calcule o número
 * complexo e forneça-o com expoente 1.
 */
struct complexo {
	// Representação da parte real de um número complexo
	float real;
	// Representação da parte imaginária de um número complexo
	float imaginario; 
};


// Lista de protótipos das funções da biblioteca ===============================

// Entrada e Saida de informações
struct complexo receberComplexo();
void complexoToString( char *, struct complexo);

// Operações para cálculo com os números complexos.
struct complexo soma(struct complexo,struct complexo);
struct complexo subtracao(struct complexo,struct complexo);
struct complexo conjugado(struct complexo);
struct complexo multiplicacao(struct complexo,struct complexo);
struct complexo divisao(struct complexo,struct complexo);
// =============================================================================
