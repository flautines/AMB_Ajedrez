#include <movimiento.h>
#include <utils.h>
#include <stdlib.h>

// el uso de ncurses es temporal, mientras se depuran los movimientos
#include <ncurses.h>
#ifndef NULL
#define NULL 0
#endif

// FUNCIONES PRIVADAS (forward declarations)
int  _compruebaDiagonales (AJD_TableroPtr tablero, AJD_MovInfo* mov_info);
int  _compruebaHorzVert   (AJD_TableroPtr tablero, AJD_MovInfo* mov_info, int distancia);
int  _compruebaEnroque    (AJD_TableroPtr tablero, AJD_Estado* estado_juego, 
                            AJD_MovInfo* mov_info);
int  _compruebaMovEnL     (AJD_TableroPtr tablero, AJD_MovInfo* mov_info);
void _obtenMovInfo        (AJD_TableroPtr tablero, AJD_Estado* estado_juego, 
                            AJD_MovInfo* mov_info);
////////////////////////////////////////////////////////////////////////////
// FUNCIONES PUBLICAS 
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// muevePieza
//
// Mueve una pieza desde una casilla origen a una casilla destino
//
// void muevePieza (AJD_TableroPtr tablero, uint8_t casilla_origen, uint8_t casilla_destino)
void muevePieza (AJD_TableroPtr tablero, AJD_Estado* estado_juego) 
{
   //AJD_Pieza pieza_a_mover = tablero->casilla[casilla_origen].pieza;   
   //AJD_Color color_pieza   = tablero->casilla[casilla_origen].color_pieza;
    AJD_idCasilla casilla_origen = estado_juego->casilla_origen;
    AJD_idCasilla casilla_destino = estado_juego->casilla_destino;
    AJD_Pieza pieza_a_mover = tablero->casilla[casilla_origen].pieza;   
    AJD_Color color_pieza   = tablero->casilla[casilla_origen].color_pieza;

   // marcar torre y/o rey movido si es necesario
   switch (pieza_a_mover)
   {
      case TORRE:
      {
        AJD_MovInfo mov_info;
        _obtenMovInfo (tablero, estado_juego, &mov_info);
        int enroque = _compruebaEnroque (tablero, estado_juego, &mov_info);
        
        if (color_pieza == BLANCO)
        {
            if (!estado_juego->torre_1a_movida) 
            {
               estado_juego->torre_1a_movida = 
                  (casilla_origen == ENROQUE_LARGO_ORIGEN_T_BLANCA);
            }
            else if (!estado_juego->torre_1h_movida) 
            {
               estado_juego->torre_1h_movida = 
                  (casilla_origen == ENROQUE_CORTO_ORIGEN_T_BLANCA);
            }            
        }
        else 
        {
            if (!estado_juego->torre_8a_movida) 
            {
               estado_juego->torre_8a_movida = 
                  (casilla_origen == ENROQUE_LARGO_ORIGEN_T_NEGRA);
            }         
            else if (!estado_juego->torre_8a_movida)
            {
               estado_juego->torre_8h_movida = 
               (casilla_origen == ENROQUE_CORTO_ORIGEN_T_NEGRA);  
           }
        }

        mvprintw (50, 0, "%s", enroque ? "O-O" : "   "); 
        break;
    }
    case REY:
        if (color_pieza == BLANCO) {
            if (!estado_juego->rey_blanco_movido)
               estado_juego->rey_blanco_movido = 1;        
        }
        else if (!estado_juego->rey_negro_movido)
            estado_juego->rey_negro_movido = 1;
        break;

    default:
        break;
    }

    tablero->casilla[casilla_origen].pieza        = NONE;
    tablero->casilla[casilla_destino].pieza       = pieza_a_mover;
    tablero->casilla[casilla_destino].color_pieza = color_pieza;
}
// esMovimientoValido
//
// Comprueba si el movimiento de la pieza en casilla_origen hacia
// casilla_destino es válido.
//
int  esMovimientoValido (AJD_TableroPtr tablero, AJD_Estado* estado_juego)
{
    AJD_MovInfo mov_info;
    AJD_Pieza   pieza; 

    _obtenMovInfo (tablero, estado_juego, &mov_info);
    pieza = tablero->casilla[mov_info.casilla_origen].pieza;

    //mvprintw (3,0, "pieza: %c",  " PTCADR"[pieza]);
    switch (pieza)
    {        
        // En principio no recibiremos nunca NONE, pero por si acaso...
        case NONE:
            return false;

        case PEON:
            return compruebaPeon (tablero, &mov_info);

        case TORRE:            
            return compruebaTorre (tablero, &mov_info);

        case CABALLO:
            return compruebaCaballo (tablero, &mov_info);

        case ALFIL:
            return compruebaAlfil (tablero, &mov_info);

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
int  compruebaPeon (AJD_TableroPtr tablero, AJD_MovInfo* mov_info)
{
    // Movimiento básico del peon
    //
    //   dir (direccion de movimiento) -1 BLANCAS, 1 NEGRAS
    //   filaPeones (fila de inicio) 6 BLANCAS, 1 NEGRAS
    //    
    int dir         = mov_info->color_pieza_origen ? -1 : 1;
    int filaPeones  = mov_info->color_pieza_origen ? 6 : 1;
    //mvprintw (4,0, "dir: %d, filaPeones: %d", dir, filaPeones);

    // El peon sólo puede mover "hacia adelante", excepto
    // para comer (digaonal adyacente) o si es el primer
    // movimiento del peon.
    if (mov_info->dy == dir)
    {
        return (
            // Come ficha
            (mov_info->dx == -1 || mov_info->dx == 1)
            && hayPieza(tablero, mov_info->casilla_destino)
            && (mov_info->color_pieza_origen != mov_info->color_pieza_destino))

            // Movimiento 1 casilla adelante libre
        ||  (mov_info->dx == 0 && !hayPieza (tablero, mov_info->casilla_destino)
        );
    }
    // Movimiento incial del peon dos casillas adelante libre
    if (mov_info->dy == 2*dir && mov_info->dx == 0)
        return (mov_info->srcY == filaPeones 
            && !hayPieza (tablero, mov_info->casilla_destino));

    // TODO: PROMOCION ULTIMA FILA

    // Si llega a este punto consideramos el movimiento ilegal.
    return false;
}
////////////////////////////////////////////////////////////////////////////
// compruebaAlfil
// 
// Comprueba si el movimiento de un alfil en casilla_origen hacia
// casilla_destino es válido.
//
int compruebaAlfil (AJD_TableroPtr tablero, AJD_MovInfo* mov_info)
{
    // El movimiento será válido si se mueve por las diagonales del
    // mismo color sin 'saltar' por encima de otras piezas
    AJD_Casilla casilla_destino = tablero->casilla[mov_info->casilla_destino];
    int seMueveEnDiagonal       = _compruebaDiagonales(tablero, mov_info);
    int casillaOcupada          = hayPieza(tablero, casilla_destino.indice);

    return  (seMueveEnDiagonal && !casillaOcupada)
         || (seMueveEnDiagonal && casillaOcupada && 
            (mov_info->color_pieza_origen != mov_info->color_pieza_destino));
}
////////////////////////////////////////////////////////////////////////////
// compruebaTorre
// 
// Comprueba si el movimiento de una torre en casilla_origen hacia
// casilla_destino es válido.
//
int compruebaTorre (AJD_TableroPtr tablero, AJD_MovInfo* mov_info)
{
    // El movimiento será válido si se mueve horizontal o vertical
    // sin 'saltar' por encima de otras piezas
    AJD_Casilla casilla_destino = tablero->casilla[mov_info->casilla_destino];
    int seMueveHorzVert         = _compruebaHorzVert (tablero, mov_info, MAX_CASILLAS_TORRE);
    int casillaOcupada          = hayPieza (tablero, casilla_destino.indice);

    return  (seMueveHorzVert && !casillaOcupada)
         || (seMueveHorzVert && casillaOcupada && 
            (mov_info->color_pieza_origen != mov_info->color_pieza_destino));
}
////////////////////////////////////////////////////////////////////////////
// compruebaCaballo
// 
// Comprueba si el movimiento de un caballo en casilla_origen hacia
// casilla_destino es válido.
//
int compruebaCaballo (AJD_TableroPtr tablero, AJD_MovInfo* mov_info)
{
    AJD_Casilla casilla_destino = tablero->casilla[mov_info->casilla_destino];
    int seMueveEnL              = _compruebaMovEnL (tablero, mov_info);
    int casillaOcupada          = hayPieza(tablero, casilla_destino.indice);

    // El movimiento será válido si se mueve en L a una casilla libre
    // o se mueve en L y come una pieza enemiga.
    return (seMueveEnL && !casillaOcupada) 
        || (seMueveEnL && casillaOcupada &&
            mov_info->color_pieza_origen != mov_info->color_pieza_destino);
}
////////////////////////////////////////////////////////////////////////////
// hayPieza
//
// Comprueba si hay alguna pieza en la casilla indicada 
// (1 ocupada / 0 libre) y el color de la pieza.
//
int hayPieza (AJD_TableroPtr tablero, AJD_idCasilla idCasilla)
{
    AJD_Casilla casilla = tablero->casilla[idCasilla]; 
    AJD_Pieza pieza = casilla.pieza;
   
    return pieza != NONE;
}
////////////////////////////////////////////////////////////////////////////
// hayPiezaEnTrayectoria
//
// Comprueba si alguna de las casillas desde una casilla origen hasta una 
// casilla destino está ocupada (1) o libre (0). 
// Las casillas origen/destino se infican en una estructura AJD_MovInfo.
//
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
    AJD_idCasilla idCasilla, casilla_destino;

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

        //mvprintw (5+t,0, "Comprobando idCasilla: %2d", idCasilla);
        
        if (hayPieza (tablero, idCasilla) && idCasilla != casilla_destino)
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
int hayPiezaValida (AJD_TableroPtr tablero, AJD_idCasilla idCasilla, AJD_Estado* estado_juego)
{
   
   AJD_Color color_pieza = tablero->casilla[idCasilla].color_pieza;

   return (hayPieza (tablero, idCasilla)
      &&   color_pieza == estado_juego->juegan_blancas);
}
////////////////////////////////////////////////////////////////////////////
// FUNCIONES PRIVADAS
////////////////////////////////////////////////////////////////////////////
// _compruebaDiagonales
// 
// Comprueba si el movimiento de una pieza se está haciendo en diagonal
// y no hay piezas obstaculizando el movimiento.
//
int _compruebaDiagonales (AJD_TableroPtr tablero, AJD_MovInfo* mov_info)
{

    //mvprintw(2,0, "dy:%d, dx:%d", mov_info->dy, mov_info->dx);

    // Movimiento es diagonal por casilla del mismo color
    if ((mov_info->dy == mov_info->dx) ||  
        (mov_info->dy == -mov_info->dx)) { 
            return !hayPiezaEnTrayectoria (tablero, mov_info);
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////
// _compruebaHorzVert
// 
// Comprueba si el movimiento de una pieza se está haciendo en horizontal
// o vertical y no hay piezas obstaculizando el movimiento.
//
int _compruebaHorzVert (AJD_TableroPtr tablero, AJD_MovInfo* mov_info, int distancia)
{
    int dx, dy;

    dx = mov_info->dx;
    dy = mov_info->dy;

    //mvprintw(4,0, "dy:%2d, dx:%2d", dy, dx);

    // Movimiento invalido si se mueve mas lejos de lo permitido
    if (dx > distancia || dy > distancia) return 0;

    // Movimiento es horizontal/vertical y sin piezas que obstaculicen?
    if (mov_info->dx == 0 || mov_info->dy== 0) {
            return !hayPiezaEnTrayectoria (tablero, mov_info);
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////
// _compruebaEnroque
// 
// Comprueba si el movimiento de una pieza es un enroque válido
//
int _compruebaEnroque (AJD_TableroPtr tablero, AJD_Estado* estado_juego, 
                       AJD_MovInfo* mov_info)
{
    AJD_idCasilla casilla_origen = estado_juego->casilla_origen;
    AJD_idCasilla casilla_destino = estado_juego->casilla_destino;
    int caminoOcupado, casillaOcupada;
    
    int enroquePermitidoBlancas = 
        !(estado_juego->rey_blanco_movido || 
          estado_juego->torre_1a_movida   || estado_juego->torre_1h_movida);

    int enroquePermitidoNegras = 
        !(estado_juego->rey_negro_movido  || 
          estado_juego->torre_8a_movida   || estado_juego->torre_8h_movida);

    // El camino y la casilla destino deben estar libres
    caminoOcupado = hayPiezaEnTrayectoria (tablero, mov_info);
    casillaOcupada = hayPieza (tablero, casilla_destino);
    if (caminoOcupado || casillaOcupada) 
        return 0;

    // Juegan BLANCAS y pueden enrocar, comprobemos si el movimiento es valido
    if (enroquePermitidoBlancas) // && juegan_blancas)
    {
        switch (casilla_origen) {
            case ENROQUE_LARGO_ORIGEN_T_BLANCA:                
                return casilla_destino == ENROQUE_LARGO_DESTINO_T_BLANCA;

            case ENROQUE_CORTO_ORIGEN_T_BLANCA:
                return casilla_destino == ENROQUE_CORTO_DESTINO_T_BLANCA;

            case ENROQUE_ORIGEN_R_BLANCO:
                return (casilla_destino == ENROQUE_LARGO_DESTINO_R_BLANCO 
                    ||  casilla_destino == ENROQUE_CORTO_DESTINO_R_BLANCO);
        }
    }
    // Juegan NEGRAS y pueden enrocar, comprobemos si el movimiento es valido
    if (enroquePermitidoNegras) // && !juegan_blancas)
    {
        switch (casilla_origen) {
            case ENROQUE_LARGO_ORIGEN_T_NEGRA:
                return casilla_destino == ENROQUE_LARGO_DESTINO_T_NEGRA;

            case ENROQUE_CORTO_ORIGEN_T_NEGRA:
                return casilla_destino == ENROQUE_CORTO_DESTINO_T_NEGRA;

            case ENROQUE_ORIGEN_R_NEGRO:
                return (casilla_destino == ENROQUE_LARGO_DESTINO_R_NEGRO 
                    ||  casilla_destino == ENROQUE_CORTO_DESTINO_R_NEGRO);
        }
    }

    // Si llegamos a este punto el enroque no es valido
    return 0;
}
////////////////////////////////////////////////////////////////////////////
// _compruebaMovEnL
// 
// Comprueba si el movimiento de una pieza se está haciendo forma de L
//
int _compruebaMovEnL (AJD_TableroPtr tablero, AJD_MovInfo* mov_info)
{
    int dx, dy;

    dx = mov_info->dx;
    dy = mov_info->dy;

    //mvprintw(4,0, "dy:%2d, dx:%2d", dy, dx);

    return (abs (dx) == 2 && abs (dy) == 1)
        || (abs (dy) == 2 && abs (dx) == 1);
}
////////////////////////////////////////////////////////////////////////////
// _obtenMovInfo
// 
// Obtiene coordenadas X, Y de las casillas origen y destino de un 
// movimiento de pieza. También calcula el desplazamiento dx,dy que supone
// ese movimiento respecto a la casilla origen.
//
void _obtenMovInfo (AJD_TableroPtr tablero, AJD_Estado* estado_juego, 
                    AJD_MovInfo* mov_info)
{
    AJD_idCasilla casilla_origen; 
    AJD_idCasilla casilla_destino;

    casilla_origen  = estado_juego->casilla_origen;
    casilla_destino = estado_juego->casilla_destino;

    mov_info->color_pieza_origen = tablero->casilla[casilla_origen].color_pieza;
    mov_info->color_pieza_destino = tablero->casilla[casilla_destino].color_pieza;
    mov_info->casilla_origen  = casilla_origen;
    mov_info->casilla_destino = casilla_destino;
    mov_info->srcY = casilla_origen / 8;
    mov_info->srcX = casilla_origen & 7;
    mov_info->dstY = casilla_destino / 8;
    mov_info->dstX = casilla_destino & 7;
    mov_info->dy = mov_info->dstY - mov_info->srcY;
    mov_info->dx = mov_info->dstX - mov_info->srcX;
}
