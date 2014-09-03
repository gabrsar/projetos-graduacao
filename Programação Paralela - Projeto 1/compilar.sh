#!/bin/bash


# Pra que usar o Make? ... Macho que é macho faz no braço :P
gcc src/Common/common.c src/Client/client.c src/Client/mergesort.c -o clientExe  -std=gnu99 -lpthread -lrt -Wall -Wextra -O3

gcc src/Common/common.c src/Server/server.c -o serverExe -std=gnu99 -lpthread -lrt -Wall -Wextra -O3
