#include <movimiento.h>
#include <utils.h>

#include <ncurses.h>

#ifndef NULL
#define NULL 0
#endif

// FUNCIONES PRIVADAS (forward declarations)
int _longitud2 (int dx, int dy);
int _compruebaDiagonales (AJD_TableroPtr tablero, AJD_Estado* estado_juego);
void _obtenMovInfo (AJD_Estado* estado_juego, AJD_MovInfo* mov_info);
////////////////////////////////////////////////////////////////////////////
// FUNCIONES PUBLICAS 
////////////////////////////////////////////////////////////////////////////
// esMovimientoValido
//
// Comprueba si el movimiento de la pieza en casilla_origen hacia
// casilla_destino es válido.
//
int  esMovimientoValido (AJD_TableroPtr tablero, AJD_Estado* estado_juego)
{
    uint8_t casilla_origen = estado_juego->casilla_origen;
    AJD_Pieza pieza = tablero->casilla[casilla_origen].pieza;

    switch (pieza)
    {        
        // En principio no recibiremos nunca NONE, pero por si acaso...
        case NONE:
            return false;

        case PEON:
            return compruebaPeon (tablero, estado_juego);

        case TORRE:
        case CABALLO:
        case ALFIL:
            return compruebaAlfil (tablero, estado_juego);

        case DAMA:
        case REY:
            return true;
    }
    // No debería llegar nunca a este punto, pero si lo hace consideramos
    // que el movimiento es ilegal.
    return false;
}
////////////////////////////////////////////////////////////////////////////
// compruebaPeon
// 
// Comprueba si el movimiento de un peon en casilla_origen hacia
// casilla_destino es válido.
//
int  compruebaPeon (AJD_TableroPtr tablero, AJD_Estado* estado_juego)
{
    uint8_t juegan_blancas; 
    AJD_MovInfo mov_info;

    _obtenMovInfo (estado_juego, &mov_info);
    juegan_blancas = estado_juego->juegan_blancas;
    //
    // Movimiento básico del peon
    //
    int dir = juegan_blancas ? -1 : 1;
    int filaPeones = juegan_blancas ? 6 : 1;

    // El peon sólo puede mover "hacia adelante", excepto
    // para comer (digaonal adyacente) o si es el primer
    // movimiento del peon.
    if (mov_info.dy == dir)
    {
        AJD_Color color_pieza;
        return
            // Come ficha
            ((mov_info.dx == -1 || mov_info.dx == 1)
            && hayPieza(tablero, mov_info.casilla_destino, &color_pieza)
            && (color_pieza ^ juegan_blancas))

            // Movimiento 1 casilla adelante libre
        ||  (mov_info.dx == 0 && !hayPieza (tablero, mov_info.casilla_destino, NULL));
    }
            // Movimiento incial del peon dos casillas adelante libre
    if (mov_info.dy == 2*dir && mov_info.dx == 0)
        return (mov_info.srcY == filaPeones 
            && !hayPieza (tablero, mov_info.casilla_destino, NULL));

    // Si llega a este punto consideramos el movimiento ilegal.
    return false;
}
////////////////////////////////////////////////////////////////////////////
// compruebaAlfil
// 
// Comprueba si el movimiento de un alfil en casilla_origen hacia
// casilla_destino es válido.
//
int compruebaAlfil (AJD_TableroPtr tablero, AJD_Estado* estado_juego)
{
    // movimiento será válido si se mueve por las diagonales del
    // mismo color sin 'saltar' por encima de otras piezas
    AJD_Color color_pieza;
    int seMueveEnDiagonal = _compruebaDiagonales(tablero, estado_juego);
    int casillaOcupada = hayPieza(tablero, 
                            estado_juego->casilla_destino,
                            &color_pieza);

    return  (seMueveEnDiagonal && !casillaOcupada)
         || (seMueveEnDiagonal 
             && casillaOcupada 
             && color_pieza != estado_juego->juegan_blancas);
}
////////////////////////////////////////////////////////////////////////////
// hayPieza
//
// Comprueba si hay alguna pieza en la casilla indicada 
// (1 ocupada / 0 libre) y el color de la pieza.
//
int hayPieza (AJD_TableroPtr tablero, uint8_t idCasilla, AJD_Color* color_pieza)
{
    AJD_Casilla casilla = tablero->casilla[idCasilla]; 
    AJD_Pieza pieza = casilla.pieza;

    if (color_pieza)
        *color_pieza = casilla.color_pieza;
   
    return pieza != NONE;
}
int hayPiezaEnTrayectoria (AJD_TableroPtr tablero, AJD_MovInfo* mov_info)
{
    // La trayectoria del movimiento es una recta. Recordemos que un 
    // punto 'p' pertenece a la recta si:
    //
    // p(t) = origen + t * direccion
    //
    // Por tanto, comprobaremos si hay una pieza en cada punto p(t) 
    // que pertenezcan a la recta que forma la trayectoria.
    int8_t casillaX, casillaY, dy, dx;
    uint8_t idCasilla, casilla_destino;

    idCasilla = mov_info->casilla_origen;
    casilla_destino = mov_info->casilla_destino;
    casillaX  = mov_info->srcX;
    casillaY  = mov_info->srcY;
    dy = sign (mov_info->dy);
    dx = sign (mov_info->dx);

    for (int t=0; casilla_destino != idCasilla; ++t) {
        casillaX += dx;
        casillaY += dy;
        // Convierte de coordenadas x,y a id en el array unidimiensional
        // de casillas.
        idCasilla = (casillaY << 3) + casillaX;

        mvprintw (5+t,0, "Comprobando idCasilla: %2d", idCasilla);
        
        if (hayPieza (tablero, idCasilla, NULL) && idCasilla != casilla_destino)
            return 1;
    }
    // Si hemos llegado aquí significa que no había ninguna pieza a lo
    // largo de la trayectoria.
    return 0;
}
////////////////////////////////////////////////////////////////////////////
// hayPiezaValida
//
// Comprueba si la casilla seleccionada por el cursor contiene una pieza
// válida para mover
//
// Devuelve 1 si lo es, 0 en caso contrario.
//
int hayPiezaValida (AJD_TableroPtr tablero, uint8_t idCasilla, AJD_Estado* estado_juego)
{
   
   AJD_Color color_pieza;

   return (hayPieza (tablero, idCasilla, &color_pieza)
      &&   color_pieza == estado_juego->juegan_blancas);
}
////////////////////////////////////////////////////////////////////////////
// FUNCIONES PRIVADAS
////////////////////////////////////////////////////////////////////////////
// compruebaDiagonales
// 
// Comprueba si el movimiento de una pieza se está haciendo en diagonal
// y no hay piezas obstaculizando el movimiento.
//
int _compruebaDiagonales (AJD_TableroPtr tablero, AJD_Estado* estado_juego)
{
    AJD_MovInfo mov_info;

    _obtenMovInfo (estado_juego, &mov_info);

    mvprintw(2,0, "dy:%d, dx:%d", mov_info.dy, mov_info.dx);

    // Movimiento es diagonal por casilla del mismo color
    if ((mov_info.dy == mov_info.dx) ||  
        (mov_info.dy == -mov_info.dx)) { 
            return !hayPiezaEnTrayectoria (tablero, &mov_info);
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////
// _obtenMovInfo
// 
// Obtiene coordenadas X, Y de las casillas origen y destino de un 
// movimiento de pieza. También calcula el desplazamiento dx,dy que supone
// ese movimiento respecto a la casilla origen.
//
void _obtenMovInfo (AJD_Estado* estado_juego, AJD_MovInfo* mov_info)
{
    uint8_t casilla_origen; 
    uint8_t casilla_destino;

    casilla_origen  = estado_juego->casilla_origen;
    casilla_destino = estado_juego->casilla_destino;

    mov_info->casilla_origen  = casilla_origen;
    mov_info->casilla_destino = casilla_destino;
    mov_info->srcY = casilla_origen / 8;
    mov_info->srcX = casilla_origen & 7;
    mov_info->dstY = casilla_destino / 8;
    mov_info->dstX = casilla_destino & 7;
    mov_info->dy = mov_info->dstY - mov_info->srcY;
    mov_info->dx = mov_info->dstX - mov_info->srcX;
}
// _longitud2
// 
// Devuelve longitud del vector al cuadrado
//
int _longitud2 (int dx, int dy)
{
    return (dx*dx + dy*dy);
}
