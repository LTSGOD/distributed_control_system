TARGET = executable
CC = gcc

INCLUDE = -lwiringPi -Ilib -Ican -Istub -Ihw

SRCS = rpi_1_main.c $(wildcard can/*.c stub/*.c lib/*.c hw/*.c)

all: 
	$(CC) -o $(TARGET) $(SRCS) $(INCLUDE)