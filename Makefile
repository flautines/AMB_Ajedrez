APP		:= ajedrez
SRCDIR  := src
OBJDIR  := obj
SOURCES := ${OBJDIR}/interfaz.o \
		   ${OBJDIR}/menu.o     \
		   ${OBJDIR}/juego.o    \
		   ${OBJDIR}/main.o 	\
		   ${OBJDIR}/movimiento.o \
		   ${OBJDIR}/utils.o
INCDIRS := ${SRCDIR}
CC 		:= clang
CFLAGS 	:= -Wall -Werror -pedantic -std=c99 -I ${INCDIRS} -finput-charset=UTF-8
LDFLAGS := -lncurses

${APP}: ${SOURCES}
	${CC} ${LDFLAGS} -v $^ -o $@

${OBJDIR}/%.o : ${SRCDIR}/%.c
	${CC} -c ${CFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm -f ${OBJDIR}/*.o ${APP}
