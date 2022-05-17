obj = main.o array.o ticketLock.o

minicron: $(obj)
	gcc $(obj) -o  ./bin/bridge -pthread

$(obj): array.h ticketLock.h

.PHONY : clean
clean:
	rm ./bin/bridge *.o