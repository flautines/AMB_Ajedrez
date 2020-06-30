#include <movimiento.h>

int esMovimientoValido (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego)
{
    AJD_CasillaPtr casilla_origen = estado_juego->casilla_origen;

    switch (casilla_origen->pieza)
    {
        case PEON:
            return 1;
            break;

        case CABALLO:
            return 1;
            break;

        case ALFIL:
            return 1;
            break;

        case REY:
            return 1;
            break;

        case DAMA:
            return 1;
            break;

        default:
            return 0;
    }
}

void muevePieza (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego)
{
    AJD_CasillaPtr origen, destino;    
    origen = estado_juego->casilla_origen;    
    destino = estado_juego->casilla_destino;

    destino->pieza = origen->pieza;
    destino->color_pieza = origen->color_pieza;
    origen->pieza = NONE;
}

int hayPiezaValida (AJD_TableroPtr tablero, AJD_CasillaPtr casilla, AJD_EstadoPtr estado_juego)
{
    return casilla->pieza != NONE 
        && casilla->color_pieza == estado_juego->juegan_blancas;
}
