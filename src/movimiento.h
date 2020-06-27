#include <tablero.h>
#include <juego.h>
typedef struct _AJD_MovInfo {
    uint8_t casilla_origen:6;
    uint8_t casilla_destino:6;
    uint8_t srcY:3;
    uint8_t srcX:3;
    uint8_t dstY:3;
    uint8_t dstX:3;
    int dy:4;
    int dx:4;
} AJD_MovInfo;
////////////////////////////////////////////////////////////////////////////
// esMovimientoValido
//
// Comprueba si el movimiento de la pieza en casilla_origen hacia
// casilla_destino es válido.
//
int esMovimientoValido (AJD_TableroPtr tablero, AJD_Estado* estdo_juego);
////////////////////////////////////////////////////////////////////////////
// compruebaPeon
// 
// Comprueba si el movimiento de un peon en casilla_origen hacia
// casilla_destino es válido.
//
int compruebaPeon (AJD_TableroPtr tablero, AJD_Estado* estado_juego);
////////////////////////////////////////////////////////////////////////////
// compruebaAlfil
// 
// Comprueba si el movimiento de un alfil en casilla_origen hacia
// casilla_destino es válido.
//
int compruebaAlfil (AJD_TableroPtr tablero, AJD_Estado* estado_juego);
////////////////////////////////////////////////////////////////////////////
// hayPiezaValida
//
// Comprueba si la casilla seleccionada por el cursor contiene una pieza
// válida para mover
//
// Devuelve 1 si lo es, 0 en caso contrario.
//
int hayPiezaValida (AJD_TableroPtr tablero, uint8_t idCasilla, AJD_Estado* estado_juego);
////////////////////////////////////////////////////////////////////////////
// hayPieza
//
// Comprueba si hay alguna pieza en la casilla indicada 
// (1 ocupada / 0 libre) y el color de la pieza.
//
int hayPieza (AJD_TableroPtr tablero, uint8_t idCasilla, AJD_Color* color_pieza);
////////////////////////////////////////////////////////////////////////////
// hayPiezaEnTrayectoria
//
// Comprueba si alguna de las casillas desde una casilla origen hasta una 
// casilla destino está ocupada (1) o libre (0). 
// Las casillas origen/destino se infican en una estructura AJD_MovInfo.
// 
int hayPiezaEnTrayectoria (AJD_TableroPtr tablero, AJD_MovInfo* mov_info);