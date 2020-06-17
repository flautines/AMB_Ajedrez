APP		:= ajedrez
SRCDIR  := src
OBJDIR  := obj
SOURCES := ${OBJDIR}/interfaz.o \
		   ${OBJDIR}/menu.o     \
		   ${OBJDIR}/juego.o    \
		   ${OBJDIR}/main.o
INCDIRS := ${SRCDIR}
CC 		:= clang
CFLAGS 	:= -Wall -pedantic -std=c99 -I ${INCDIRS}
LDFLAGS := -lncurses

${APP}: ${SOURCES}
	${CC} ${LDFLAGS} $^ -o $@

${OBJDIR}/%.o : ${SRCDIR}/%.c
	${CC} -c ${CFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${OBJDIR}/*.o ${APP}