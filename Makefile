CC = gcc
CFLAGS = -Wall -std=gnu99 -pedantic -g
MAIN = tree
OBJS = tree.o array_list.o

all : $(MAIN)

$(MAIN) : $(OBJS) tree.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

#array_list_tests.o : task1_tests.c task1.h
#	$(CC) $(CFLAGS) -c task1_tests.c

tree.o : tree.c tree.h
	$(CC) $(CFLAGS) -c tree.c

array_list.o : array_list.c array_list.h
	$(CC) $(CFLAGS) -c array_list.c

test: $(MAIN)
	(chmod +x ./test_run.sh) && (./test_run.sh | grep -i "failed") || echo "Tests passed"

clean :
	rm *.o $(MAIN) core
