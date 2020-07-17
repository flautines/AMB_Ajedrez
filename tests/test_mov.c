#include <common.h>

int main()
{
    AJD_EstadoPtr pestado = obtenEstadoPtr();

    inicializa();
    nuevoJuego();

    pestado->jugada.movBlancas.idOrigen = e2;
    pestado->jugada.movBlancas.idDestino = e4;
    pestado->jugada.movBlancas.jaque = TRUE;
    pestado->jugada.movNegras.idOrigen = e7;
    pestado->jugada.movNegras.idDestino = e5;
  
    ejecutaPartida();

    liberaRecursos();
            
    return 0;
}
