obj = main.o array.o

minicron: $(obj)
	gcc $(obj) -o ./bin/bridge

$(obj): array.h

.PHONY : clean
clean:
	rm ./bin/bridge *.o