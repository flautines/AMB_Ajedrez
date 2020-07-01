#include <movimiento.h>
#include <interfaz.h>

#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS (INTERFAZ PRIVADA)
////////////////////////////////////////////////////////////////////////////
int cumpleReglasComer             (AJD_MovInfo* movInfo);
int cumpleReglasMovimiento        (AJD_MovInfo* movInfo);
int cumpleReglasComerPeon         (AJD_MovInfo* movInfo);
int cumpleReglasMovimientoPeon    (AJD_MovInfo* movInfo);
int cumpleReglasMovimientoCaballo (AJD_MovInfo* movInfo);
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int esMovimientoValido (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego)
{
    AJD_MovInfo movInfo = obtenMovInfo (estado_juego);
    muestraMovInfo (&movInfo);
    
    // Si la casilla destino está ocupada, descartar el movimiento
    // si hay una pieza del mismo color. Si la pieza es de distino
    // color, aun hay la posibilidad de que sea un movimiento valido
    // de comer.
    // Finalmente, si la casilla está libre, comprobar si la pieza
    // se mueve según sus reglas.
    if (casillaOcupada (movInfo.destino))
    {
        if (movInfo.destino->color_pieza == movInfo.origen->color_pieza)
            return 0;
        else        
            return cumpleReglasComer (&movInfo);
    }
    else
        return cumpleReglasMovimiento (&movInfo);        
}
////////////////////////////////////////////////////////////////////////////
void muevePieza (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego)
{
    AJD_CasillaPtr origen, destino;    
    origen = estado_juego->casilla_origen;    
    destino = estado_juego->casilla_destino;

    destino->pieza = origen->pieza;
    destino->color_pieza = origen->color_pieza;
    origen->pieza = NONE;
}
////////////////////////////////////////////////////////////////////////////
int casillaOcupada (AJD_CasillaPtr casilla)
{
    return casilla->pieza != NONE;
}
////////////////////////////////////////////////////////////////////////////
int hayPiezaValida (AJD_TableroPtr tablero, AJD_CasillaPtr casilla, AJD_EstadoPtr estado_juego)
{
    return casillaOcupada (casilla)
        && casilla->color_pieza == estado_juego->juegan_blancas;
}
////////////////////////////////////////////////////////////////////////////
AJD_MovInfo obtenMovInfo (AJD_EstadoPtr estado_juego)
{
    AJD_MovInfo info;
    AJD_CasillaPtr origen, destino;
    int dir;
    
    origen = estado_juego->casilla_origen;
    destino = estado_juego->casilla_destino;
    // Si el color de la pieza es BLANCO se mueven 1 hacia arriba (-1), si no,
    // se mueven 1 hacia abajo (+1)
    dir = origen->color_pieza ? -1 : 1;
    mvprintw (3,0, "dir: %2d", dir);
    
    info.origen  = origen;
    info.destino = destino;    
    info.srcY    = origen->id /8;
    info.srcX    = origen->id &7;
    info.dstY    = destino->id /8;
    info.dstX    = destino->id &7;    
    info.dy      = info.dstY - info.srcY;
    info.dx      = info.dstX - info.srcX;
    // Multiplicando dy * dir nos da la distancia en vertical
    info.distY = info.dy * dir;
    // Distancia en horizontal del movimiento    
    info.distX = abs (info.dx);

    return info;
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// INTERFAZ PRIVADA
////////////////////////////////////////////////////////////////////////////
int cumpleReglasMovimiento (AJD_MovInfo* movInfo)
{
    AJD_Pieza pieza = movInfo->origen->pieza;

    switch (pieza)
    {
        case PEON:
            return cumpleReglasMovimientoPeon (movInfo);

        case CABALLO:
            return cumpleReglasMovimientoCaballo (movInfo);

        default:
            return 0;
    }
}
////////////////////////////////////////////////////////////////////////////
int cumpleReglasComer (AJD_MovInfo* movInfo)
{
    AJD_Pieza pieza = movInfo->origen->pieza;

    switch (pieza)
    {
        case PEON:
            return cumpleReglasComerPeon (movInfo);
            break;

        case CABALLO:
            return 1;

        default:
            return 0;
    }
}
////////////////////////////////////////////////////////////////////////////
int cumpleReglasComerPeon (AJD_MovInfo* movInfo)
{
    return movInfo->distX == 1 && movInfo->distY == 1;
}
////////////////////////////////////////////////////////////////////////////
int cumpleReglasMovimientoPeon (AJD_MovInfo* movInfo)
{
    AJD_Color colorPiezaOrigen = movInfo->origen->color_pieza;
    int distY = movInfo->distY;
    int distX = movInfo->distX;
    int filaInicioPeones = colorPiezaOrigen ? 6 : 1;    

    if (distX == 0)
    {
        // Movimiento vertical de dos casillas, sólo es válido si 
        // es 1er movimiento o lo que es lo mismo, si se parte de 
        // las filas de inicio de PEON.
        return (distY == 2 && movInfo->srcY == filaInicioPeones)
        // Movimiento vertical de una casilla
            || distY == 1;
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////
int cumpleReglasMovimientoCaballo (AJD_MovInfo* movInfo)
{
    int distY = abs(movInfo->distY);
    int distX = movInfo->distX;

    // Movmiento en "L"
    return (distY == 2 && distX == 1) || (distX == 2 && distY == 1);
}
