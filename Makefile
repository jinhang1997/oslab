
myecho: myecho.c
	gcc -o myecho myecho.c
	
.PHONY: clean

clean:
	rm -f myecho
