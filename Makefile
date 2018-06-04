OBJS := myecho mycat mycp
CC := gcc

all: clean $(OBJS)

myecho: myecho.c 
	$(CC) -o myecho myecho.c
	
mycat: mycat.c
	$(CC) -o mycat mycat.c
	
mycp: mycp.c
	$(CC) -o mycat mycp.c
	
.PHONY: all clean

clean:
	rm -f $(OBJS)
