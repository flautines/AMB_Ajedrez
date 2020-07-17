#include <common.h>
#include <movimiento.h>

/****************************************************************************************
 * efectuaMovimiento
 ***************************************************************************************/
void efectuaMovimiento (AJD_TableroPtr tablero, AJD_MovimientoPtr movimiento)
{
    AJD_idCasilla origen  = movimiento->idOrigen;
    AJD_idCasilla destino = movimiento->idDestino;

    muevePieza (tablero, origen, destino);
}
/****************************************************************************************
 * muevePieza
 ***************************************************************************************/
void muevePieza (AJD_TableroPtr tablero, AJD_idCasilla origen, AJD_idCasilla destino)
{
    tablero->casillas[destino].pieza = tablero->casillas[origen].pieza;
    tablero->casillas[origen].pieza  = NO_PIEZA;
}
