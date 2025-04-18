CC=gcc
CFLAGS=-I.

all : ../bin/seq_min_max ../bin/par_min_max ../bin/exec_seq_min_max

../bin/exec_seq_min_max: ../bin/seq_min_max
	$(CC) -o ../bin/exec_seq_min_max exec_sequential_min_max.c

../bin/seq_min_max : ../bin/utils.o ../bin/find_min_max.o utils.h find_min_max.h
	$(CC) -o ../bin/seq_min_max ../bin/find_min_max.o ../bin/utils.o sequential_min_max.c $(CFLAGS)

../bin/par_min_max : ../bin/utils.o ../bin/find_min_max.o utils.h find_min_max.h
	$(CC) -o ../bin/par_min_max ../bin/utils.o ../bin/find_min_max.o parallel_min_max.c $(CFLAGS)

../bin/utils.o : utils.h
	$(CC) -o ../bin/utils.o -c utils.c $(CFLAGS)

../bin/find_min_max.o : utils.h find_min_max.h
	$(CC) -o ../bin/find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm ../bin/utils.o ../bin/find_min_max.o ../bin/seq_min_max ../bin/par_min_max ../bin/exec_seq_min_max
