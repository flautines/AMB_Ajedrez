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
/****************************************************************************************
 * 
 ***************************************************************************************/
AJD_Bool esMovimientoValido (AJD_MovimientoPtr movimiento, AJD_EstadoPtr estadoJuego)
{
    AJD_Bool jueganBlancas = estadoJuego->jueganBlancas;
    /*
     * Primero que cumpla las reglas de movimiento especifico de la pieza.
     * Si lo cumple, mira si la casilla destino está ocupada.
     * Si la pieza ocupada es del mismo "bando", el movimiento no es valido 
     * Si la casilla es del mismo color, come la pieza. 
     * Si la casilla está libre, es un movimiento valido.
     * El PEON es la única pieza que come de forma diferente, así que se 
     * comprueba en un caso aparte si se detecta movimiento de pieza no válido.
    */
    if (cumpleReglasMovimiento (movimiento))
    {
        return !hayPiezaJugador (jueganBlancas, idToPtr(movimiento->idDestino));
    }
    else
        return FALSE;
}
/****************************************************************************************
 * 
 ***************************************************************************************/
AJD_Bool cumpleReglasMovimiento (AJD_MovimientoPtr movimiento)
{
    return TRUE;
}
