APP		:= ajedrez
SRCDIR  := src
OBJDIR  := obj
SOURCES := ${OBJDIR}/main.o 	\
		   ${OBJDIR}/tablero.o  \
		   ${OBJDIR}/interfaz.o \
		   ${OBJDIR}/juego.o
INCDIRS := ${SRCDIR}
CC 		:= clang
CFLAGS 	:= -Wall -Werror -pedantic -std=c89 -I ${INCDIRS} -finput-charset=UTF-8
LDFLAGS := -lncurses

${APP}: ${SOURCES}
	${CC} ${LDFLAGS} -v $^ -o $@

${OBJDIR}/%.o : ${SRCDIR}/%.c
	${CC} -c ${CFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm -f ${OBJDIR}/*.o ${APP}
