GCC = gcc
MPICC = mpicc
MPILDFLAGS = -lm -std=c99 -O3 -pedantic -Wall
CFLAGS = -O3 -std=c99 -O3 -pedantic -Wall -I/usr/include/mpi -lm
LDFLAGS = -lm

OBJS = summa-mpi.o 

all: mpi

mpi: summa-mpi

summa-mpi: $(OBJS)
	$(MPICC) $(MPILDFLAGS) -o $@ $+

clean:
	rm -f *.o summa-mpi



