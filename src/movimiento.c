#include <common.h>
#include <movimiento.h>
#include <utils.h>
#include <stdlib.h>
/****************************************************************************************
 * Variables privadas
 ***************************************************************************************/

/****************************************************************************************
 * Funciones PRIVADAS (forward declaration)
 ***************************************************************************************/
AJD_Bool cumpleReglasPeon (AJD_MovimientoPtr movimiento, AJD_Pieza pieza);
AJD_MovInfo obtenMovInfo (AJD_MovimientoPtr movimiento);
AJD_Bool esPrimerMovimiento (AJD_MovimientoPtr movimiento, AJD_Pieza pieza);

/****************************************************************************************
 ****************************************************************************************
 * Funciones PUBLICAS (Implementacion)
 ****************************************************************************************
 ***************************************************************************************/
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
    AJD_Pieza pieza = obtenPieza (idToPtr (movimiento->idOrigen));

    switch (pieza)
    {
        case PEON_B:
        case PEON_N:
            return cumpleReglasPeon (movimiento, pieza);

        default:
            return TRUE;
    }
    
}


/****************************************************************************************
 ****************************************************************************************
 * Funciones PRIVADAS (Implementacion)
 ****************************************************************************************
 ***************************************************************************************/
AJD_Bool cumpleReglasPeon (AJD_MovimientoPtr movimiento, AJD_Pieza pieza)
{
    /* Puntero a la casilla destino */
    AJD_CasillaPtr pdestino = idToPtr (movimiento->idDestino);

    /* Desplazamiento X,Y del movimiento */    
    AJD_MovInfo movInfo = obtenMovInfo (movimiento); 

    /* Sentido vertical del movimiento: -1 BLANCAS, 1 NEGRAS */
    int dirY = (pieza == PEON_B) ? -1 : 1;
    
    /* Distancia vertical, será >0 si se mueve adelante, <0 hacia atrás */
    int distY = dirY * movInfo.dy;
  
    /* El peon sólo puede mover hacia adelante una casilla, o dos casillas si es su
     * primer movimiento. */
    if (movInfo.dx == 0 && casillaVacia (pdestino))
        return (distY == 2 && esPrimerMovimiento (movimiento, pieza))
        /* TODO: comprobar camino libre cuando distY == 2 */
        || distY == 1;

    /* O posible captura si se mueve en diagonal una casilla */
    else
        return abs(movInfo.dx) == 1 && 
               distY == 1 && 
               hayPiezaOponente (pieza==PEON_B, pdestino);
}
/****************************************************************************************
 ***************************************************************************************/
AJD_MovInfo obtenMovInfo (AJD_MovimientoPtr movimiento)
{
    AJD_MovInfo movInfo = { 0, 0 };
    int dxOri, dyOri, dxDes, dyDes;
    
    dxOri = movimiento->idOrigen  &7;
    dxDes = movimiento->idDestino &7;
    dyOri = movimiento->idOrigen  /8;
    dyDes = movimiento->idDestino /8;

    movInfo.dx = dxDes - dxOri;
    movInfo.dy = dyDes - dyOri;

    mvprintw (1,0, "des, ori: (%2d, %2d)\tdx, dy: (%2d, %2d)", 
        movimiento->idDestino, movimiento->idOrigen, movInfo.dx, movInfo.dy);

    return movInfo;
}
/****************************************************************************************
 * esPrimerMovimiento
 *
 * Devuelve TRUE si se está moviendo el peon indicado por primera vez.
 ***************************************************************************************/
AJD_Bool esPrimerMovimiento (AJD_MovimientoPtr movimiento, AJD_Pieza pieza)
{
    /* Será primer movimiento si su origen es la fila inicial de peones de su color,
     * es decir, a7..h7 para las NEGRAS o a2..h2 para las BLANCAS.
     */
    AJD_idCasilla filaInicioPeones = (pieza == PEON_B) ? a2 : a7;
    return movimiento->idOrigen >= filaInicioPeones && 
           movimiento->idOrigen <= filaInicioPeones + 7;
}

