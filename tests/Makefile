TEST		:= test_log
SRCDIR  := ../src
OBJDIR  := ../obj
TESTDIR := ${SRCDIR}/tests
SOURCES := 	${OBJDIR}/tablero.o  \
			${OBJDIR}/juego.o    \
			${OBJDIR}/menu.o     \
			${OBJDIR}/interfaz.o \
		   	${OBJDIR}/movimiento.o \
		   	${OBJDIR}/utils.o
INCDIRS := ${SRCDIR}
CC 		:= clang
CFLAGS 	:= -Wall -Werror -pedantic -std=c99 -I ${INCDIRS} -finput-charset=UTF-8
LDFLAGS := -lncurses

${TEST}: ${SOURCES} \
		${OBJDIR}/log.o \
		${TEST}.o
	${CC} ${LDFLAGS} -v $^ -o $@

${OBJDIR}/%.o : ${SRCDIR}/%.c
	${CC} -c ${CFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm -f *.o ${APP}
