SRC=4gew.c i2c_keys.c
OBJ=$(SRC:.c=.o)
EXEC=4gew
CC = /home/es/uClinux-dist/tools/ucfront-gcc /usr/local/arm-linux/bin/gcc
CFLAGS  = -I./inc/ -I. -march=armv4t -mtune=arm7tdmi -O0 -ggdb3 -fomit-frame-pointer -pipe -msoft-float -fno-common -fno-builtin -Wall -DEMBED -Dlinux -D__linux__ -Dunix -D__uClinux__
LDFLAGS = -Wl,-elf2flt="-rk"

all: prog

prog: $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~ *.gdb $(EXEC) $(OBJ)
