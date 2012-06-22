PROJECT=	librbtree
CC=		gcc
LD=		ld
INCL=		includes
CFLAGS=		-I $(INCL) -g
LDFLAGS=	-lc --dynamic-linker=/lib/ld-linux.so.2 -e main
EXEC=		librbtree.out
SRC=		$(wildcard src/*.c)
OBJ=		$(SRC:.c=.o)

all: $(EXEC)

librbtree.out: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -rf src/*.o	src/*~ includes/*~ *~

distclean: clean
	@rm -f $(EXEC)

dist: distclean
	@tar czf ../_$(PROJECT).tgz ../$(PROJECT)
	@mv ../_$(PROJECT).tgz ../$(PROJECT)_`date "+%d%b%Y-%kh%M"`.tgz
