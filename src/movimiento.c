#include <common.h>
#include <movimiento.h>

/****************************************************************************************
 * efectuaMovimiento
 ***************************************************************************************/
void efectuaMovimiento (AJD_MovimientoPtr movimiento)
{
    AJD_idCasilla origen  = movimiento->idOrigen;
    AJD_idCasilla destino = movimiento->idDestino;

    muevePieza (origen, destino);
}
