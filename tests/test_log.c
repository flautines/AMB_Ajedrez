#include <common.h>
#include <log.h>
#include <string.h>
#include <stdlib.h>

char buff[1024] = "";

int main()
{
    AJD_EstadoPtr pestado;
    AJD_idCasilla idCasilla = a2;

    inicializa();
    nuevoJuego();

    pestado = obtenEstadoPtr();
    pestado->jugada.movBlancas.idDestino = e4;
    pestado->jugada.movBlancas.jaque = TRUE;
    pestado->jugada.movNegras.idOrigen = e7;
    pestado->jugada.movNegras.idDestino = e5;

    for (idCasilla = a2; idCasilla !=h1+1; idCasilla++)
    {
        pestado->jugada.movBlancas.idOrigen = idCasilla;
        appendMovimiento (&pestado->jugada.movBlancas);
        appendMovimiento (&pestado->jugada.movNegras);
    }
    sprintPartida(buff);
    mvprintw(0, 0, "%s\n", buff);
    for (idCasilla = a8; idCasilla !=h7+1; idCasilla++)
    {
        pestado->jugada.movNegras.idOrigen = idCasilla;
        appendMovimiento (&pestado->jugada.movBlancas);
        appendMovimiento (&pestado->jugada.movNegras);
    }
    sprintPartida(buff);
    mvprintw(2, 0, "%s\n", buff);

    while (getch());
    liberaRecursos();
    
    return 0;
}
