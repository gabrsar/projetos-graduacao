#!/bin/bash

cd src


# -D _BSD_SOURCE é utilizado para não gerar WARNING do usleep
gcc listaGenerica/listaGenerica.c exibicao.c padroes.c thread1.c thread2.c main.c -o FSC3 -std=c99 -lpthread -Wall -Wextra -D _BSD_SOURCE



mv FSC3 ../

cd ..

echo "O executável gerado é: FSC3"

