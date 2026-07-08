CC = gcc

main:
	$(CC) main.c src/colors.c src/server_defaults.c net/DNP.c  -o rmain
