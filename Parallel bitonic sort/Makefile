CC=g++
CFLAGS=-fopenmp
DEPS = BitnoicSort.h

%.o: %.c $(DEPS)
  $(CC) -c -o $@ $< $(CFLAGS)

openmp_bitnoic_sort: BitnoicSort.o main.o 
  $(CC) -o openmp_bitnoic_sort BitnoicSort.o main.o

clean:
  rm -f *.o *.d *~ $(PROGS)
  