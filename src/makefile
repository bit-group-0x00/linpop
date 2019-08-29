all:server client
server:server.o socket.o cJSON.o 
	gcc $^ -lm -o $@
client:client.o socket.o cJSON.o
	gcc $^ -lm -o $@
%.o:%.c
	gcc -c $^
%.o:util/%.c 
	gcc -w -c $^
clean:all
	rm -f *.o