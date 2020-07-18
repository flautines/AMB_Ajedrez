SRCDIR  := ../src
OBJDIR  := ../obj
TESTDIR := ./
SOURCES := ${OBJDIR}/tablero.o  \
		   ${OBJDIR}/interfaz.o \
		   ${OBJDIR}/juego.o 	\
		   ${OBJDIR}/movimiento.o \
		   ${OBJDIR}/utils.o
INCDIRS := ${SRCDIR}
CC 		:= clang
CFLAGS 	:= -Wall -Werror -pedantic -std=c89 -I ${INCDIRS} -finput-charset=UTF-8
LDFLAGS := -lncurses

test_mov: ${SOURCES} test_mov.o
	${CC} ${LDFLAGS} $^ -o $@

${OBJDIR}/%.o : ${SRCDIR}/%.c
	${CC} -c ${CFLAGS} $^ -o $@

test_idStr: ${SOURCES} test_idStr.o
	${CC} ${LDFLAGS} $^ -o $@
	
${OBJDIR}/test_mov.o : ${TESTDIR}/test_mov.c
	${CC} -c ${CFLAGS} $^ -o $@	

${OBJDIR}/test_idStr.o : ${TESTDIR}/test_idStr.c
	${CC} -c ${CFLAGS} $^ -o $@		

.PHONY: clean

clean:
	rm -f ${OBJDIR}/*.o ${APP}
