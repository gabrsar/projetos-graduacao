/*
 * Primeiro projeto de Fundamentos em Sistemas de Computação.
 * Professor: Aleardo Manacero Jr.
 * Alunos:
 * 		Gabriel Henrique Martinez Saraiva - gabrielsaraiva7@gmail.com
 * 		Igor Stefani Buttarello -
 *
 *
 * Descrição do trabalho conforme encontrado na página atual:
 * http://www.dcce.ibilce.unesp.br/~aleardo/cursos/fsc/proj1.php
 *
 * Projeto 1: Gerenciamento de processos :::
 * Objetivo:
 * Implementar um núcleo gerenciador de processos, usando threads. O núcleo deve ter uma interface com uma função chamada interruptControl, que retorna um inteiro identificando o tipo de interrupção que demandou a sua ação (retorna 0 se não houve interrupção), e uma função chamada sysCall (chamada quando o retorno da interruptControl é 0), que retorna um inteiro correspondente ao tipo de funcionalidade invocada por outro thread qualquer do sistema.
 * A partir do inteiro obtido de uma dessas funções o núcleo ativa o thread correspondente, bem como o escalonamento do próximo processo usando o algoritmo de prioridade definida por um campo do BCP. As origens possíveis para os eventos a serem tratados pelo núcleo (com cada inteiro que as identificam) são:
 *
 * processInterrupt (1) -- interrupção gerada pelo final do quantum-time de um processo
 * semaphoreP (10) -- tratamento de bloqueio de processo
 * semaphoreV (11) -- tratamento de desbloqueio de processo
 * ioRequest (4) -- chamada de operação de entrada/saída
 * ioFinish (5) -- sinalização de final de E/S
 * memLoadReq (6) -- chamada de operação de carregamento na memória
 * memLoadFinish (7) -- sinalização de final de carregamento
 * fsRequest (8) -- chamada para operação no sistema de arquivos
 * fsFinish (9) -- sinalização de final de operação no SA
 * processCreate (2) -- chamada para iniciar a criação de um processo no BCP
 * processFinish (3) -- chamada para terminar a existência de um processo no BCP
 *
 *
 * Nesse momento é preciso escrever os códigos correspondentes às threads de semáforos, criação, tármino e interrupção de processos e escalonamento. As demais funcionalidades devem ser implementadas como funções vazias.
 *
 * ALGORITMO DE ESCALONAMENTO:
 *
 * Por prioridade, com preempção pela chegada de novo processo ou pelo início ou conclusão de E/S ou bloqueio.
 *
 * Para melhor entendimento de como programar usando threads veja esse documento.
 *
 * Para a implementação desse núcleo faça uso das bibliotecas "semaphore.h" e "pthreads.h" presentes no UNIX para o uso de semáforos e de threads.
 *
 * PRAZO PARA ENTREGA: 01/10/2012, até 24h00 (data e horário do recebimento do e-mail para aleardo at ibilce dot unesp dot br)
 *
 */

/* Comentários sobre o desenvolvimento do projeto.
 *
 * Professor, nosso projeto está divido em arquivos para facilitar o desenvolvimento,
 * a coompreenção e também a compilação e já ir acostumando com o desenvolvimento de
 * grandes projetos.
 *
 * Para compilá-lo basta executar o comando make na pasta source do projeto.
 *
 * Esperamos que o projeto funcione bem pois desenvolvemos com muito carinho, amor
 * e testamos bastante para que ficasse estavel e fácil de entender o que está
 * acontecendo.
 *
 * Gabriel/Igor.
 */

#include "nucleo.h"

int main()
{
	iniciarSistema();
}
