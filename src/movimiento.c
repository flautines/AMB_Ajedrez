#include <movimiento.h>
#include <interfaz.h>
#include <utils.h>

#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////
// Variable privada con informacion adicional del movimiento
AJD_MovInfo movInfo;
////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS (INTERFAZ PRIVADA)
////////////////////////////////////////////////////////////////////////////
void obtenMovInfo (AJD_EstadoPtr estado_juego);
int cumpleReglasComer             (AJD_TableroPtr tablero);
int cumpleReglasMovimiento        (AJD_TableroPtr tablero);
int cumpleReglasComerPeon         ();
int cumpleReglasMovimientoPeon    ();
int cumpleReglasMovimientoCaballo ();
int seMueveEnVertHorz             (AJD_TableroPtr tablero);
int seMueveEnDiagonal             (AJD_TableroPtr tablero);
int caminoLibre                   (AJD_TableroPtr tablero);
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int esMovimientoValido (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego)
{
    obtenMovInfo (estado_juego);
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
            return cumpleReglasComer (tablero);
    }
    else
        return cumpleReglasMovimiento (tablero);
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
////////////////////////////////////////////////////////////////////////////
// INTERFAZ PRIVADA
////////////////////////////////////////////////////////////////////////////
// obtenMovInfo
//
//  Obtiene detalles necesarios para realizar comprobaciones posteriores
// respecto las casillas origen y destino de un movimiento.
//
void obtenMovInfo (AJD_EstadoPtr estado_juego)
{
    AJD_CasillaPtr origen, destino;
    int dir;
    
    origen = estado_juego->casilla_origen;
    destino = estado_juego->casilla_destino;
    // Si el color de la pieza es BLANCO se mueven 1 hacia arriba (-1), si no,
    // se mueven 1 hacia abajo (+1)
    dir = origen->color_pieza ? -1 : 1;
    mvprintw (3,0, "dir: %2d", dir);
    
    movInfo.origen  = origen;
    movInfo.destino = destino;    
    movInfo.srcY    = origen->id /8;
    movInfo.srcX    = origen->id &7;
    movInfo.dstY    = destino->id /8;
    movInfo.dstX    = destino->id &7;    
    movInfo.dy      = movInfo.dstY - movInfo.srcY;
    movInfo.dx      = movInfo.dstX - movInfo.srcX;
    // Multiplicando dy * dir nos da la "distancia" en vertical.
    // Realmente no se trata de distancia en el sentido matemático, ya que
    // tiene signo, dependiendo de si la pieza se ha movido hacia adelante (positivo)
    // o hacia atrás (negativo)
    movInfo.distY = movInfo.dy * dir;
    // Distancia en horizontal (en el sentido matematico, 
    // (siempre positivo, nunca negativo) del movimiento
    movInfo.distX = abs (movInfo.dx);
}
////////////////////////////////////////////////////////////////////////////
int cumpleReglasMovimiento (AJD_TableroPtr tablero)
{
    AJD_Pieza pieza = movInfo.origen->pieza;

    switch (pieza)
    {
        case PEON:
            return cumpleReglasMovimientoPeon ();

        case CABALLO:
            return cumpleReglasMovimientoCaballo ();

        case TORRE:
            return seMueveEnVertHorz (tablero);
        default:
            return 0;
    }
}
////////////////////////////////////////////////////////////////////////////
int cumpleReglasComer (AJD_TableroPtr tablero)
{
    AJD_Pieza pieza = movInfo.origen->pieza;

    switch (pieza)
    {
        case PEON:
            return cumpleReglasComerPeon ();
            break;

        case CABALLO:
            return cumpleReglasMovimientoCaballo ();            

        case TORRE:
            return seMueveEnVertHorz (tablero);

        default:
            return 0;
    }
}
////////////////////////////////////////////////////////////////////////////
int cumpleReglasComerPeon ()
{
    return movInfo.distX == 1 && movInfo.distY == 1;
}
////////////////////////////////////////////////////////////////////////////
int cumpleReglasMovimientoPeon ()
{
    AJD_Color colorPiezaOrigen = movInfo.origen->color_pieza;
    int distY = movInfo.distY;
    int distX = movInfo.distX;
    int filaInicioPeones = colorPiezaOrigen ? 6 : 1;    

    if (distX == 0)
    {
        // Movimiento vertical de dos casillas, sólo es válido si 
        // es 1er movimiento o lo que es lo mismo, si se parte de 
        // las filas de inicio de PEON.
        return (distY == 2 && movInfo.srcY == filaInicioPeones)
        // Movimiento vertical de una casilla
            || distY == 1;
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////
int seMueveEnVertHorz (AJD_TableroPtr tablero)
{         
    return (movInfo.dy == 0 || movInfo.dx == 0) 
        && caminoLibre (tablero);
}
////////////////////////////////////////////////////////////////////////////
int cumpleReglasMovimientoCaballo ()
{
    // La "distancia" movInfo.distY es un tanto especial, ya que tiene
    // signo para poder saber si la pieza se desplaza hacia adelante o atrás.
    // Necesitamos el valor absoluto para obtener la distancia en el sentido
    // matemático.
    int distY = abs(movInfo.distY);
    int distX = movInfo.distX;

    // Movmiento en "L"
    return (distY == 2 && distX == 1) || (distX == 2 && distY == 1);
}
////////////////////////////////////////////////////////////////////////////
int caminoLibre (AJD_TableroPtr tablero)
{
    int dx, dy;    
    AJD_CasillaPtr origen   = movInfo.origen;
    AJD_CasillaPtr destino  = movInfo.destino;
    AJD_idCasilla idOrigen  = origen->id;
    AJD_idCasilla idDestino = destino->id;    
    dx = dy = 0;

    dx = sign (movInfo.dx);
    dy = sign (movInfo.dy) * 8;

    for (AJD_idCasilla idCasilla = idOrigen+dy+dx; idCasilla != idDestino; idCasilla += dy+dx)
    {
        AJD_CasillaPtr casilla = &tablero->casilla[idCasilla];
        if (casillaOcupada (casilla)) 
            return 0;
    }
    return 1;
}
