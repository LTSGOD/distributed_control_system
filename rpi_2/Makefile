TARGET = executable
CC = gcc

INCLUDE = -lwiringPi -Ilib -Ican -Istub -IRPC

SRCS = rpi_2_main.c $(wildcard can/*.c stub/*.c RPC/*.c)

all: 
	$(CC) -o $(TARGET) $(SRCS) $(INCLUDE)