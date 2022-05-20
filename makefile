obj = main.o array.o ticketLock.o car.o

minicron: $(obj)
	gcc $(obj) -o  ./bin/bridge -pthread

$(obj): array.h ticketLock.h car.h

.PHONY : clean
clean:
	rm ./bin/bridge *.o