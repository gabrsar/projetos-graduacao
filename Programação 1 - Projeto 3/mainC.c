//Biblioteclas e códigos externos ==============================================
#include <stdio.h>	
#include "complexos.h"
#include "libutils.h"

//Prototipos ===================================================================
void menu();
void menuOperacoesBasicas();
void menuOperacoesEmVetor();
void menuImprimirNumeroComplexo();
// =============================================================================

int main(){
	menu();
	return 0;
}


//Função que controla o fluxo principal do programa.
void menu(){
	
	char opc=' ';
	
	do{
		limparTela();
		printf("+========= Calculadora de Números Complexos =========+\n");
		printf("|            Autor: Gabriel H. M. Saraiva            |\n");
		printf("+====================================================+\n");
		printf("| 1 - Operações com elementos em um vetor            |\n");
		printf("| 2 - Imprimir um número complexo                    |\n");
		printf("| 3 - Operações básicas (+,-,*,/) números complexos  |\n");
		printf("| 0 - Sair.											 |\n");
		printf("+====================================================+\n");
		printf("> Opção:");

		opc = receberChar();

		switch(opc){
			case '1':
				menuOperacoesEmVetor();
				break;
			case '2':
				menuImprimirNumeroComplexo();
				break;
			case '3': 
				menuOperacoesBasicas();
				break;
			case '0':
				break;

			default:
				printf("\033[1;31m\n\nOpção inválida!\nTente novamente.\n\n\033[0m");
				pausa();
		}

	}while(opc!='0');

	printf("\n\n Tenha um bom dia! =D \n\n");
	
	//Sai do programa.
}

//Controla fluxo nas operações com vetores
void menuOperacoesEmVetor(){

	int tamanhoVetor;
	
	limparTela();
	printf("+=  Programa > Operações com elementos de um vetor  =+\n");
	printf("+====================================================+\n");
	do{
		printf("> Entre com a quantidade de elementos desejada:");
		tamanhoVetor = receberInt();
		if(tamanhoVetor <1){
			printf("\033[1;31m \n\nOpção inválida!\nO tamanho de um vetor tem que ser maior que 0.\033[0m\n\n");
		}
		

	}while(tamanhoVetor < 1);

	struct complexo vetor[tamanhoVetor];
		
	printf("\n");

	int i;
	for(i=0;i<tamanhoVetor;i++){
		printf("\nElemento %d de %d:\n",i+1 ,tamanhoVetor);
		vetor[i] = receberComplexo();
	}

	// Seleção da operação
	printf("\n");
	char opc=' ';
	do{
		printf("> Selecione a operação desejada ( + - / * ):");
		//scanf(" %c",&opc);
		opc = receberChar();
		printf("\n");
		if( opc == '+' || opc == '-' || opc == '*' || opc == '/' ){
			printf("\nPosição - Número Complexo\n");
			for(i=0;i<tamanhoVetor;i++){
				char complexoTmp[TAMANHO_MAXIMO_PARA_STRING];
				complexoToString(complexoTmp,vetor[i]);

				printf("%7d - %s\n",i+1 ,complexoTmp);
				
			}
	
			// Seleção dos indices para realizar a operação
			int a,b;
			// Indice 1			
			do{
				
				printf("Entre com a posição do 1º número complexo:");
				a = receberInt();
				a--;
				
				if(a<0 || a>=tamanhoVetor){
					printf("\033[1;31mPosição inválida!\nTente novamente.\033[0m\n");
				}
				
			}while(a < 0 || a >=tamanhoVetor);

			// Indice 2
			do{			
				printf("Entre com a posição do 2º número complexo:");
				b = receberInt();
				b--;
				
				if(b<0 || b>=tamanhoVetor){
					printf("\033[1;31mPosição inválida!\nTente novamente.\033[0m\n");
				}
				
			}while(b<0 || b>=tamanhoVetor);
			
				
			char complexoA[TAMANHO_MAXIMO_PARA_STRING];
			char complexoB[TAMANHO_MAXIMO_PARA_STRING];
			char complexoC[TAMANHO_MAXIMO_PARA_STRING];

			complexoToString(complexoA,vetor[a]);
			complexoToString(complexoB,vetor[b]);

			switch(opc){
				case '+':
					complexoToString(complexoC,soma(vetor[a],vetor[b]));
					break;
				case '-':
					complexoToString(complexoC,subtracao(vetor[a],vetor[b]));
					break;
				case '*':
					complexoToString(complexoC,multiplicacao(vetor[a],vetor[b]));
					break;
				case '/':
					complexoToString(complexoC,divisao(vetor[a],vetor[b]));
					break;
			}

			printf("\nO resultado da operação %s %c %s = %s\n",complexoA,opc,complexoB,complexoC);
			
			printf("\n\n\n> Entre com 0 para voltar ao menu ou\n");
		}
	}while(opc != '0');
}

void menuOperacoesBasicas(){
	char opc=' ';
		
		limparTela();
		printf("+====  Programa > Operações com Números Avulsos  ====+\n");
		printf("+====================================================+\n");
		do{

			printf("> Selecione a operação desejada ( + - / * ):");

			opc = receberChar();

			if(opc == '+' || opc == '-' || opc == '*' || opc == '/' ){
				struct complexo a,b;


				printf("\nEntre com o 1º número complexo:\n");
				a = receberComplexo();
				printf("\nEntre com o 2º número complexo:\n");
				b = receberComplexo();

				char complexoA[TAMANHO_MAXIMO_PARA_STRING];
				char complexoB[TAMANHO_MAXIMO_PARA_STRING];
				char complexoC[TAMANHO_MAXIMO_PARA_STRING];

				complexoToString(complexoA,a);
				complexoToString(complexoB,b);

				switch(opc){
					case '+':
						complexoToString(complexoC,soma(a,b));
						break;
					case '-':
						complexoToString(complexoC,subtracao(a,b));
						break;
					case '*':
						complexoToString(complexoC,multiplicacao(a,b));
						break;
					case '/':
						complexoToString(complexoC,divisao(a,b));
						break;
				}
	
				printf("O resultado da operação %s %c %s é %s\n",complexoA,opc,complexoB,complexoC);
	
				printf("\n\n> Entre com 0 para voltar.\n");
			}
		}while(opc != '0');
	}
	

void menuImprimirNumeroComplexo(){
	
	printf("+======  Programa > Imprimir número complexo  =======+\n");
	printf("+====================================================+\n");
	
	struct complexo n;
     
     	n=receberComplexo();
     
     	char complexoN[TAMANHO_MAXIMO_PARA_STRING];
     
     	complexoToString(complexoN,n);
     
     	printf("O seu numero complexo é:%s\n\n",complexoN);
	pausa();
    
}
