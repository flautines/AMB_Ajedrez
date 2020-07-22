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
AJD_Bool cumpleReglasPeon (AJD_MovimientoPtr movimiento, AJD_Color colorPieza);
AJD_Bool cumpleReglasCaballo (AJD_MovimientoPtr movimiento, AJD_Color colorPieza);
AJD_MovInfo obtenMovInfo (AJD_MovimientoPtr movimiento);
AJD_Bool esPrimerMovimiento (AJD_MovimientoPtr movimiento, AJD_Color colorPieza);
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
 * esMovimientoValido
 *
 * Comprueba si un movimiento desde una casilla origen a destino es válido.
 ***************************************************************************************/
AJD_Bool esMovimientoValido (AJD_MovimientoPtr movimiento, AJD_EstadoPtr estadoJuego)
{
    AJD_Pieza pieza = obtenPieza (idToPtr (movimiento->idOrigen));
    AJD_Color colorPieza = estadoJuego->jueganBlancas ? BLANCO : NEGRO;

    switch (pieza)
    {
        case PEON_B:
        case PEON_N:
            return cumpleReglasPeon (movimiento, colorPieza);

        case CABALLO_B:
        case CABALLO_N:
            return cumpleReglasCaballo (movimiento, colorPieza);

        default:
            return TRUE;
    }
}

/****************************************************************************************
 ****************************************************************************************
 * Funciones PRIVADAS (Implementacion)
 ****************************************************************************************
 ***************************************************************************************/
AJD_Bool cumpleReglasPeon (AJD_MovimientoPtr movimiento, AJD_Color colorPieza)
{
    /* Punteros a la casilla origen y destino */
    AJD_CasillaPtr porigen  = idToPtr (movimiento->idOrigen);
    AJD_CasillaPtr pdestino = idToPtr (movimiento->idDestino);

    /* Desplazamiento X,Y del movimiento */    
    AJD_MovInfo movInfo = obtenMovInfo (movimiento); 

    /* Sentido vertical del movimiento: -1 BLANCAS, 1 NEGRAS */
    int dirY = colorPieza ? -1 : 1;
    
    /* Distancia vertical, será >0 si se mueve adelante, <0 hacia atrás */
    int distY = dirY * movInfo.dy;
  
    /* El peon sólo puede mover hacia adelante una casilla, o dos casillas si es su
     * primer movimiento. */
    if (movInfo.dx == 0 && casillaVacia (pdestino))
        return (distY == 2 && 
                esPrimerMovimiento (movimiento, colorPieza) &&
                caminoLibre (porigen, pdestino, 0, movInfo.dy))
        /* TODO: comprobar camino libre cuando distY == 2 */
        || distY == 1;

    /* O posible captura si se mueve en diagonal una casilla */
    else
        return abs(movInfo.dx) == 1 && 
               distY == 1 && 
               hayPiezaOponente (colorPieza, pdestino);
}
/****************************************************************************************
 * cumpleReglasMovimientoCaballo
 ****************************************************************************************/
AJD_Bool cumpleReglasCaballo (AJD_MovimientoPtr movimiento, AJD_Color colorPieza)
{
    AJD_CasillaPtr pdestino = idToPtr (movimiento->idDestino);

    /* Desplazamiento X,Y del movimiento */    
    AJD_MovInfo movInfo = obtenMovInfo (movimiento); 

    /* En realidad queremos la magnitud del desplazamiento dx,dy */    
    int distY = abs(movInfo.dx);
    int distX = abs(movInfo.dy);

    /* No puede haber una pieza del jugador en destino */
    if (hayPiezaJugador (colorPieza, pdestino))
        return FALSE;

    /* Movmiento en "L" */
    return (distY == 2 && distX == 1) || (distX == 2 && distY == 1);
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
AJD_Bool esPrimerMovimiento (AJD_MovimientoPtr movimiento, AJD_Color colorPieza)
{
    /* Será primer movimiento si su origen es la fila inicial de peones de su color,
     * es decir, a7..h7 para las NEGRAS o a2..h2 para las BLANCAS.
     */
    AJD_idCasilla filaInicioPeones = colorPieza ? a2 : a7;
    return movimiento->idOrigen >= filaInicioPeones && 
           movimiento->idOrigen <= filaInicioPeones + 7;
}

