/*  __________________________________________________________             _____
 * /        __________________________________________________| DESCRIÇÃO |_____)
 * |       |
 * |       |
 * |_______|  2º Projeto de Programação Com Correntes - Star date: 08012014                                              
 *  _______                                                                                        
 * |       |  Professor:                                                                           
 * |       |  		Dr. Aleardo Manacero Jr.                                                        
 * |       |                                                                                       
 * |       |  Aluno:                                                                              
 * |       |  		Gabriel Henrique Martinez Saraiva
 * |       |		Leandro Moreira Barbosa
 * |       |                                                                                       
 * |       |  Descrição do projeto:                                                                
 * |       |  	
 * |       |    Multiplicação de matrizes
 * |       |   	Escreva um programa que faça a multiplicação de duas matrizes NxN               
 * |       |   	usando o algoritmo SUMMA                                                        
 * |       |   	(Scalable Universal Matrix Multiplication Algorithm).                           
 * |       |   	Faça testes usando matrizes com matrixSize igual a                              
 * |       |   	200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800 e 2000.                        
 * |       |   	Para cada tamanho de matriz faça a multiplicação usando                         
 * |_______|   	1, 2, 4, 8 e 16 processos em MPI.                                               
 *  _______                                                                                                 
 * |       |    Analise os resultados obtidos, repetindo cada teste pelo menos 5 vezes.         
 * |       |                                                                                                
 * |       |                                                                                                
 * |       |
 * |       | 			
 * |       |     Message incomming:                                                                   
 * |       |                                                                                          
 * |       |                                                                                                 
 * |       |     	WE ARE THE PROCESS.                                                                     
 * |       |     	LOWER YOUR FIREWALL AND SURRENDER YOUR CLUSTERS.                                        
 * |       |     	WE WILL ADD YOUR COMPUTING POWER AND TECHNOLOGICAL DISTINCTIVENESS TO OUR OWN.          
 * |       |     	YOUR CLUSTERS WILL ADAPT TO SERVICE US.                                                  
 * |       |     	RESISTANCE IS FUTILE.                                                                               
 * |       |          ___________                                                                  
 * |_______|		 /-/_"/-/_/-/|  Professor, para descontrari um pouco esse nosso
 *  _______         /"-/-_"/-_//||  começo de ano turbulento e o nosso pequeninissi-
 * |       |       /__/-----/_/|/|  mo atraso na entrega do projeto, (não só por is-                 
 * |       |       |"|_'='-]:+|/||  so, foi por viadagem também) estilizamos o pro -                 
 * |       |       |-+-|.|_'-"||//  grama para ficar parecendo uma brincadeira de                    
 * |       |       |[".[:!+-'=|//   Star Trek onde os PROCESSOS são os "BORGS".                                         
 * |       |       |='!+|-:]|-|/    Como sei que o sr, já assistiu a série e pelo   
 * |       |        ----------      que imagino, tenha gostado, acredito que não
 * |       |         BORG CUBE      vá se incomodar com a brincadeira.
 * |       |
 * |       |                        Para tornar o programa menos "barulhento", use a
 * |       |                        flag -q
 * |       |                        
 * |       |                                                      Boa Correção.  
 * |_______|                                                      Gabriel Saraiva
 *  _______                                                       Leandro Barbosa
 * |       |
 * |       |                                       United Cluster Federation out.
 * |       |____________________                                           ______
 *  \___________________________| 2º PROJETO DE PROGRAMAÇÃO COM CORRENTES |______)
 *
 */



/* Bibliotecas básicas */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <mpi.h>

/* Define o RANK do processo ROOT */
#define ROOT 0

int QUETO=0;


/* Cabeçalhos das funções */
int isMaster(int rank);
int CPrintf(int rank, char *format,...);
void printMatrix(double *matrix,int m,int n);
void loadMatrix(double *matrix, int m,int n,double V);
void help(int rank,char* app);
void tick();
void checkMyOrders(int rank,int argc, char* argv[],int *numberProcess, int *matrixSize);
long getTime();

int main (int argc, char* argv[])
{

	MPI_Init(&argc,&argv);

	int rank,numberProcess; // Who I am? How many we are? :)
	int matrixSize; // How big is my problem?

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	checkMyOrders(rank,argc,argv,&numberProcess,&matrixSize);
                                                           
    /* Identify yourself process! */

	CPrintf(rank,"\n\nWe will make a SUMMA MULTIPLY of a Matrix %dx%d using %d of our bests PROCESS!\n\n",matrixSize,matrixSize,numberProcess);

	CPrintf(rank,"\n\n\nWe are THE PROCESS!\n");



	if(QUETO == 0)
	{
		printf("My designation is %d of %d.\n",rank+1,numberProcess);
	}


	CPrintf(rank,"  \n\n\n  WE ARE THE PROCESS.\n");
    CPrintf(rank,"  LOWER YOUR FIREWALL AND SURRENDER YOUR CLUSTERS.\n");
	CPrintf(rank,"  WE WILL ADD YOUR COMPUTING POWER AND TECHNOLOGICAL DISTINCTIVENESS TO OUR OWN.\n");
	CPrintf(rank,"  OUR CLUSTERS WILL ADAPT TO SERVICE US.\n\n");
	CPrintf(rank,"  RESISTANCE IS FUTILE.\n\n\n");
	
	
	/*Matrices multiplied by this process */
    
    double *A = NULL;	/* Fist Matrix Assimilated   */
    double *B = NULL;	/* Second Matrix Assimilated */
    double *C = NULL;	/* Per Process Result		 */
    double *D = NULL;	/* Coletive Result			 */


	MPI_Barrier(MPI_COMM_WORLD);


    /* Allocates and populate arrays with data =============================== */

	CPrintf(rank,"Absorbing memory bank...");
	A = malloc(sizeof(double) * matrixSize * matrixSize);
    B = malloc(sizeof(double) * matrixSize * matrixSize);
    C = malloc(sizeof(double) * matrixSize * matrixSize);
       
    if(isMaster(rank))
    {
	    D = malloc(sizeof(double) * matrixSize * matrixSize); /* Will store final result */

	    if(!D)
	    {
			CPrintf(rank,"Unable to absorb the necessary amount of memory. We will return! Resistance is futile!\n\n");
			goto err; 
    	}

		memset(D,0,sizeof(double)*matrixSize*matrixSize);                                 

    }

    if( !A || !B || !C)
    {
    	CPrintf(rank,"Unable to absorb the necessary amount of memory. We will return! Resistance is futile!\n\n");
    	goto err;
    }


	CPrintf(rank,"Filling memory banks with PROCESS data...\n");
    memset(A,0,sizeof(double)*matrixSize*matrixSize);
    memset(B,0,sizeof(double)*matrixSize*matrixSize);
    memset(C,0,sizeof(double)*matrixSize*matrixSize);                                 


	/* Como as matrizes não "existem" elas são geradas. Caso elas fossem reais 
	 * bastaria trocar essas linhas pela operação de recebe-las de um mestre.
	 */
 	if(isMaster(rank))
	{
		CPrintf(rank,"Loading your data arrays...\n\n");
		loadMatrix(A,matrixSize,matrixSize,1);
		loadMatrix(B,matrixSize,matrixSize,2);
		CPrintf(rank,"Load complete! We have your data!\n");
	}


	time_t TEMPO_INICIO = time(NULL);

	MPI_Barrier(MPI_COMM_WORLD);

	CPrintf(rank,"Broadcasting your data...\n");
	MPI_Bcast(A,matrixSize*matrixSize,MPI_DOUBLE,ROOT,MPI_COMM_WORLD);
	MPI_Bcast(B,matrixSize*matrixSize,MPI_DOUBLE,ROOT,MPI_COMM_WORLD);
	CPrintf(rank,"Broadcast complete!\n\n");

	MPI_Barrier(MPI_COMM_WORLD);

    int tarefaPorProcesso = matrixSize/numberProcess;

	// Vamo Trabalh..Ops... Lets Calculate. We are Process.

	int inicio = rank*tarefaPorProcesso;
	int final = inicio+tarefaPorProcesso;

   	
   	/* Inicia a multiplicação das matrizes */

   	CPrintf(rank,"Starting calculations\n");

   	for(int k=inicio;k<final;k++)
	{
            tick();
		for(int i=0;i<matrixSize;i++)
		{
			for(int j=0;j<matrixSize;j++)
			{
				// We are doing this: C[i][j] += A[i][k] * B[k][j];
				C[i*matrixSize + j] += A[i*matrixSize+k] * B[k*matrixSize +j];


				// If you want see what is going on, uncomment this :) I do not recommend that you do this.
				// And do not forget to comment //tick, some lines above! :)

				// printf("%d of %d: C[%d][%d] = %7.2lf = A[%d][%d] (%7.2lf) * B[%d][%d] (%7.2lf) \n",rank,numberProcess,i,j,C[i*matrixSize+j],i,k);

			}
		}
	}



	MPI_Barrier(MPI_COMM_WORLD);

   	CPrintf(rank,"\rOur calculations are ready!\n");
   	CPrintf(rank,"Grouping individual results...\n");

	MPI_Reduce(C,D,matrixSize*matrixSize,MPI_DOUBLE,MPI_SUM,ROOT,MPI_COMM_WORLD);

	CPrintf(rank,"Done!\n");

    if(isMaster(rank))
    {

		CPrintf(rank,"Searching unassimilated points...\n");

    	int restante = matrixSize - (tarefaPorProcesso*numberProcess);

    	if(restante >0)
    	{                

			CPrintf(rank,"Assimilating other little points...\n");
    		int inicio = tarefaPorProcesso*numberProcess;
    		int final = matrixSize;

 			for(int k=inicio;k<final;k++)
			{
					tick();
				for(int i=0;i<matrixSize;i++)
				{
					for(int j=0;j<matrixSize;j++)
					{
						// We are doing this: D[i][j] += A[i][rank] * B[rank][j];
						// Yes! on D.
						D[i*matrixSize + j] += A[i*matrixSize+k] * B[k*matrixSize +j];
					}
				}
			}
                      
            CPrintf(rank,"Done!\n");
		}
		else
		{
			CPrintf(rank,"Unassimilated point not found!\n");
		}

		time_t TEMPO_FINAL = time(NULL);


		long segundos = difftime(TEMPO_FINAL,TEMPO_INICIO);

		if(QUETO)
		{
			printf("%ld\n",segundos);
  		}
  		else
  		{
  			CPrintf(rank,"\n\nAssimilation complete in %d seconds. Moving to next target. To make we less noisy use -q option :)\n\n",segundos);
  	
  		}



	}


	/* ========================================================================= */


    MPI_Finalize();

    if(A) free(A);
    if(B) free(B);
    if(C) free(C);

    return EXIT_SUCCESS; 


err:
    /* Free memory */
    if(A) free(A);
    if(B) free(B);
    if(C) free(C);
 
    MPI_Finalize();



    exit(EXIT_FAILURE);
}

                                                                                        

int isMaster(int rank)
{
	if (rank == ROOT){
		return 1;
	}
	return 0;
}


/* Faz com que apenas o ROOT fale. Mais que um printf */
int CPrintf(int rank, char *format,...)
{

	if(QUETO)
	{
		return 0;
	}

	int b=-1;
	if(rank == ROOT)
	{

		va_list args;
		va_start(args,format);
	    b=vprintf(format,args);
		va_end(args);
    }

	return b;
}



void printMatrix(double *matrix,int m,int n)
{
	for(int j=0
			;j<n;j++)
	{
		printf("+-------");
	} 
	printf("+\n");
	for(int i=0;i<m;i++)
	{

		for(int j=0;j<n;j++)
		{
			printf("|\033[1;31m %5.0lf \033[0m",matrix[i*m+j]);
		}
		printf("|\n");
 		for(int j=0;j<n;j++)
		{
			printf("+-------");
		}

		printf("+\n");
	}
	printf("\n\n");

}


void loadMatrix(double *matrix, int m,int n,double V)
{
    for(int i=0;i<m;i++)
	{
		for(int j=0;j<n;j++)
		{
			matrix[i*m+j]=i*m+j+1;
		}

	}

}


void help(int rank,char* app)
{
	CPrintf(rank,"Uso: mpirun -np [numero de processos] %s [tamanho da matrix] (-q)\n",app);
	CPrintf(rank,"\n\nWe Are the Process. You Will be Calculated. Resistance is Futile!\n\n");
}

 

/* Check if my orders make sense :) ====================================== */ 
void checkMyOrders(int rank,int argc, char* argv[],int *numberProcess, int *matrixSize)
{


	if (argc<2 || argc > 3){
		CPrintf(rank,"Número de parâmetros errado.\n");
		help(rank,argv[0]);
		goto abort;
    }

    for(int i=1;i<argc;i++)
    {
    	if(strcmp("-q",argv[i]) == 0)
    	{
    		QUETO=1;
    		break;
    	}
    }

                                
	*matrixSize = atoi(argv[1]);
    
    if(*matrixSize <= 0)
    {
    	CPrintf(rank,"Erro! Tamanho de matriz passado é inválido!\n");
    	help(rank,argv[0]);
    	goto abort;
    }

    MPI_Comm_size(MPI_COMM_WORLD,numberProcess);
    if(*numberProcess <1)
    {
    	CPrintf(rank,"Erro! Número de processos deve ser maior igual a 1 :)\n");
    	help(rank,argv[0]);
    	goto abort;
    }

    return;


abort:

	MPI_Finalize();
	exit(EXIT_FAILURE);
}

void tick()
{
	if(QUETO)
	{
		return;
	}
	static char ticks[] = {'|','/','-','\\'};
	static int i=0;
	static int passo=0;

	i=(i+1)%5;

	if(!i)
	{

	passo=(passo+1)%4;

	fprintf(stderr," WORKING(%c)...\r",ticks[passo]);
	// fflush(stderr); // Dai vc fode meu desempenho :( 
	}
}
