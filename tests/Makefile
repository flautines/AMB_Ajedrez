APP     := test_pgnread
SRCDIR  := ../src
OBJDIR  := ../obj
OBJTEST := ./obj
TESTDIR := ./
SOURCES := ${OBJDIR}/tablero.o  \
		   ${OBJDIR}/interfaz.o \
		   ${OBJDIR}/juego.o 	\
		   ${OBJDIR}/movimiento.o \
		   ${OBJDIR}/utils.o \
		   ${OBJDIR}/log.o \
		   ${OBJDIR}/pgnreader.o
INCDIRS := ${SRCDIR}
CC 		:= clang
CFLAGS 	:= -Wall -Werror -pedantic -std=c89 -I ${INCDIRS} -finput-charset=UTF-8
LDFLAGS := -lncurses

.PHONY: clean cleanall

${APP}: ${SOURCES} ${OBJTEST}/${APP}.o
	${CC} ${LDFLAGS} $^ -o $@

${OBJTEST}/%.o : $(TESTDIR)/%.c
	${CC} -c ${CFLAGS} $^ -o $@

${OBJDIR}/%.o : ${SRCDIR}/%.c
	${CC} -c ${CFLAGS} $^ -o $@

clean:
	rm -f ${TESTDIR}/*.o

cleanall:
	rm -f ${TESTDIR}/*.o ${OBJDIR}/*.o
