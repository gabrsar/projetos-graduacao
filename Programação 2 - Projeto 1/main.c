/*
Department: DCCE - Departamento de Ci�ncia da Computa��o e Estat�stica
Author: Gabriel Henrique Martinez Saraiva
ail: extremez3r0@gmail.com
Url: http://www.dcce.ibilce.unesp.br
===========================================================================
File: Projeto.c
Liscence: GPL
Version: 0.1
Created: 28/10/2010
Last Update: 28/10/2010
===========================================================================
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define _TAMANHO_MAXIMO_STRING 100
#define _IN  "bancoDadosDatas.txt"
#define _OUT "formatadas.txt"

//struct que representa Data
typedef struct{
	unsigned short int dia;
	unsigned short int mes;
	unsigned short int ano;
}Data;


//Prototipos ==================================================================

//Principais
unsigned short int getNumeroDoMes(char*);
unsigned short int isAnoBissexto(unsigned short int);
unsigned short int getNumeroDeDiasNoMes(unsigned short int,unsigned short int);
unsigned short int getDiasAteFinalDoAno(Data);
unsigned short int getDiaDoAno(Data);
unsigned short int isDataValida(Data);
unsigned short int adequarAno(unsigned short int);
unsigned int diasEntreDatas(Data,Data);
void data2String(Data,char*);
Data string2Data(char*);

//Auxiliares
void exibirData(Data);
void gtoupper(char*);
void gtrim(char*);
size_t gsubstr(char*,char*,unsigned int,unsigned int);


//=============================================================================

// Fun��es auxiliares =========================================================

//Converte uma String para maiusculo
void gtoupper(char *str){
	while(*str != '\0'){
		if(*str == ((char)231)){
			*str = '�';
		}
		*str = toupper(*str);
		str++;
	}
}

//Fun��o que remove os caracteres ' ' excedentes. (Deixa um entre cada palavra)
void gtrim(char * str){

	//ucc = Ultimo Caractere Copiado
	//pc = Proximo Caractere
	char *strC;

	strC = str;

	

	//n = Numero de espa�os em branco.
	unsigned int i,tamanho,n,k;

	i = k = 0;

	tamanho = strlen(str);

	for(;i<tamanho;i++){

		//Quando encontra um espa�o adiciona na contagem,
		//quando n�o, zera a contagem.
		n = ( *(str+i) == ' ') ? n+1 : 0 ;


		if(n>1){
			k++; //Avan�a para o proximo caractere
		}
		else{
			*strC = *(str+i);
			strC++;
		}
	}
	*strC = '\0';
}

//Pega uma parte da string de start � end (incluso) e joga em uma nova string.
size_t gsubstr(char* newStr,char* oldStr,unsigned int start,unsigned int end){

	size_t s = end - start + 1;
	unsigned int i=0;

	for(;i<s;i++) *(newStr+i) = *(oldStr+start+i);

	*(newStr+i) = '\0';


	return i;
}

void exibirData(Data d){
	printf("%2d/%2d/%4d\n",d.dia,d.mes,d.ano);
}
// ============================================================================

unsigned short int getNumeroDoMes(char *mes){

	unsigned short int tamanho = strlen(mes);
	char m[tamanho];
	
	strcpy(m,mes);//Cria uma c�pia para manter a original intacta.
	
	gtoupper(m);
	
	if( !strcmp(m,"JANEIRO"  ) ) return 1;
	if( !strcmp(m,"FEVEREIRO") ) return 2;
	if( !strcmp(m,"MARCO"    ) ) return 3;
	if( !strcmp(m,"ABRIL"    ) ) return 4;
	if( !strcmp(m,"MAIO"     ) ) return 5;
	if( !strcmp(m,"JUNHO"    ) ) return 6;
	if( !strcmp(m,"JULHO"    ) ) return 7;
	if( !strcmp(m,"AGOSTO"   ) ) return 8;
	if( !strcmp(m,"SETEMBRO" ) ) return 9;
	if( !strcmp(m,"OUTUBRO"  ) ) return 10;
	if( !strcmp(m,"NOVEMBRO" ) ) return 11;
	if( !strcmp(m,"DEZEMBRO" ) ) return 12;

	return 0;
}

//Verifica se um ano � bissexto
unsigned short int isAnoBissexto(unsigned short int ano){

	//Realiza os c�lculos de maneira que o ano bissexto tenha
	//aproximadamente 365.25 dias

	if(ano % 400 == 0){
		return 1;
	}
	if((ano % 100) != 0 && (ano % 4 == 0) ){
		return 1;
	}
	return 0;

}

unsigned short int getNumeroDeDiasNoMes(unsigned short int mes,unsigned short int ano){

	switch (mes){

		case 1:
		case 3:
		case 5:
		case 7:		//Meses com 31 dias
		case 8:
		case 10:
		case 12:
			return 31;
		case 4:
		case 6:		//Meses com 30 dias
		case 9:
		case 11:
			return 30;
		case 2:		//Fevereiro
			return (isAnoBissexto(ano)?29:28);
		default : return -1;
	}
}

unsigned short int getDiasAteFinalDoAno(Data d){

	unsigned short int dias=0;

	//N�mero de dias para o final do mes.
	dias = getNumeroDeDiasNoMes(d.mes,d.ano) - d.dia;

	//Corre para o pr�ximo mes (que � um mes completo) e inicia a acomula��o
	//dos dias at� o final do ano.(31/12/ano)
	d.mes++;

	for(;d.mes<=12;d.mes++){
		dias+= getNumeroDeDiasNoMes(d.mes,d.ano);
	}

	return dias;
}

unsigned short int getDiaDoAno(Data d){

	unsigned short int dia = d.dia;

	//Volta para o mes anterior (que � um mes completo) e inicia a
	//acomula��o dos dias at� o inicio do ano (1/1/ano)
	d.mes--;

	for(; d.mes >= 1; d.mes--){
		dia+= getNumeroDeDiasNoMes(d.mes,d.ano);
	}

	return dia;
}

unsigned short int isDataValida(Data d){

	if (d.dia < 1) return 0;

	if (d.mes < 1 || d.mes > 12) return 0;

	if (d.dia > getNumeroDeDiasNoMes(d.mes,d.ano)) return 0;

	return 1;//Data v�lida.

}

//Corrige o ano para um intev�lo j� definido 1951 - 2050
unsigned short int adequarAno(unsigned short int a){

	if(a<=   50) 			  return(2000 + a);
	if(a >=   51 && a<=   99) return(1900 + a);
	if(a >= 1951 && a<= 2050) return(a);
	return -1;
}

int isGreaterThan(Data d1,Data d2){
	if(d1.ano > d2.ano) return 1;
	else if(d1.ano == d2.ano){

		if(d1.mes > d2.mes) return 1;
		else if(d1.mes == d2.mes){

			if(d1.dia > d2.dia) return 1;
			else return 0;
		}
		else return 0;
	}
	return 0;
}

//Calcula a diferen�a de dias entre duas datas.
unsigned  int diasEntreDatas(Data d1,Data d2){
	//Obriga maior data ficar no d2.
	if(isGreaterThan(d1,d2)){
		Data aux = d1;
		d1=d2;
		d2=aux;
	}


	//ab = AnoBissexto aN = AnoNormal dA = Diferen�aDeAnos
	unsigned int aB,aN,dA;

	//N�mero de anos completos entre as duas datas
	dA = (d2.ano - d1.ano)-1;

	unsigned short int i;

	//Coleta quantos anos bissextos exisitiram entre duas datas.
	for(i=0;d1.ano+i<d2.ano;i++){
		if(isAnoBissexto(d1.ano+i))aB++;
	}
	
	aN = dA-aB;
	unsigned int dias;

	dias  = getDiasAteFinalDoAno(d1);

	dias += 365*aN;
	dias += 366*aB;

	dias += getDiaDoAno(d2); //N�o Conta o dia que t�!
	return dias;
}

//Converte uma Data pra string.
void data2String(Data d,char *str){
	sprintf(str,"%02d/%02d/%04d",d.dia,d.mes,d.ano);
}

//Converte uma string "dd/mm/aaaa" ou "dd de mmmmmmmmmm de aaaa" para uma data
Data string2Data(char *str){

	//Cria um backup da string para n�o destruir a original.
	char str2[strlen(str)];
	strcpy(str2,str);

	gtrim(str2);

	char tipo = '\0'; // Guarda o tipo da string > S = Simples C = complex.

	char *fimDia,*fimMes;

	char dia[3],mes[15],ano[4];

	Data data;

	if(strstr(str2,"/") != NULL){
		//Data no formato dd/mm/aaaa

		tipo = 'S';

		fimDia= strstr(str2,"/");
		*fimDia = '_';
		fimMes = strstr(str2,"/");
		*fimMes = '_';
	}else{
		//Data no formato dd de mmmmmmmmm de aaaa

		tipo = 'C';

		fimDia= strstr(str2,"de ");
		*fimDia = ' ';          //Destroi o "de" para achar o pr�ximo "de"
		*(fimDia + 1) = ' ';

		fimMes = strstr(str2,"de ");
		*fimMes = ' ';          //Destroi esse "de" para poder apertar a string,
		*(fimMes + 1 )= ' ';      //e fazer ela funcionar com o separador abaixo.

		gtrim(str2);

		fimDia--;//Aponta para o espa�o antes do primeiro "de"
		*fimDia = '/';//Destroi o espa�o colocando uma barra
		fimMes = strstr(str2," ");

	}
	gsubstr(dia,str2,0              ,fimDia-1-str2); // Pega dia.
	gsubstr(mes,str2,fimDia+1-str2  ,fimMes-1-str2); // Pega mes.
	gsubstr(ano,str2,fimMes+1-str2  ,strlen(str2));  // Pega ano.

	data.dia = atoi(dia);

	if(tipo == 'C'){
		data.mes = getNumeroDoMes(mes);
		}else{
	       	data.mes = atoi(mes);
	}
	
	data.ano = atoi(ano);
	data.ano = adequarAno(data.ano);
	return data;
}

int main(){

	FILE *arqIN = fopen(_IN,"r");
	FILE *arqOUT = fopen(_OUT,"w");

	printf("Abrindo arquivos...");
	if(arqIN == NULL){
		printf("Erro! O arquivo de entrada n�o pode ser aberto.\n");
		fclose(arqOUT);
		return -1;
	}

	if(arqOUT == NULL){
		printf("Erro! O arquivo de saida n�o pode ser aberto.\n");
		fclose(arqIN);
		return -2;
	}
	printf("OK!\n");
	
	char str1[_TAMANHO_MAXIMO_STRING];
	char str2[_TAMANHO_MAXIMO_STRING];
	
	char strD1[10];
	char strD2[10];

	Data d1,d2;

	unsigned int dias;

	printf("Realizando c�lculos e gravando resultados...");

	char data[300];

	while(!feof(arqIN)){
		
		fscanf(arqIN," %[^\n]\n",str1);

		fscanf(arqIN," %[^\n]\n",str2);

		d1 = string2Data(str1);
		d2 = string2Data(str2);

		if(!isDataValida(d1) || !isDataValida(d2)){
			printf("Erro! Existem dados inconsistentes no arquivo de entrada!\n");
			fclose(arqIN);
			fclose(arqOUT);
			return -3;
		}
		dias = diasEntreDatas(d1,d2);
		data2String(d1,strD1);
		data2String(d2,strD2);

		sprintf(data,"Data Inicial: %s Data Final: %s => Total de Dias Decorridos: %d %s\n",
				strD1,
				strD2,
				dias,
				(dias>1)?"dias":"dia"
		);

		fprintf(arqOUT,data);
	}
	printf("OK!.\n");

	printf("Fechando arquivos...");
	fclose(arqIN);
	fclose(arqOUT);
	printf("OK!\n");

	printf("Have a nice day :)\n\n");

	return 0;
}
