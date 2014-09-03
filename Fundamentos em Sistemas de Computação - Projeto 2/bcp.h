
#ifndef BCP_H_
#define BCP_H_

#define ESTADO_INATIVO 0
#define ESTADO_RODANDO 1
#define ESTADO_PRONTO 2
#define ESTADO_ESPERA 3
#define ESTADO_BLOQUEADO 4

typedef struct
{
	int pid;
	long pc;
	int prioridade;
	long timestamp;
	int estado;
	int tempoParaProcessar;

} bcp;

#endif /* BCP_H_ */
