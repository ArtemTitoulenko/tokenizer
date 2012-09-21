OUT=tokenizer
SEP="abc\xaf "
STR="the crazy fox jumped over the lazy dog"

# should return 1 2 3c

$(OUT): *.c
	gcc *.c -o $(OUT)
	chmod +x ./$(OUT)

test:
	./$(OUT) $(SEP) $(STR)

valgrind:
	valgrind ./$(OUT) $(SEP) $(STR)

clean:
	rm ./$(OUT)
	rm ./*.o