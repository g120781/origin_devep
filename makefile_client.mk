
TARGETS := client

OBJECTS := client.o protocal_376.o

$(TARGETS):$(OBJECTS)
	gcc -g -O0 $^ -o $(TARGETS)

client.o : client.c
	gcc -c -g client.c -o client.o

protocal_376.o:protocal_376.c
	gcc -c -g protocal_376.c -o protocal_376.o

.PHONY:clean
	
clean:
	rm -rf *.o
