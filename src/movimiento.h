#pragma once 

#include <tablero.h>
#include <juego.h>

// Máximo número de casillas que pueden moverse la TORRE
#define MAX_CASILLAS_TORRE 7  
// Máximo número de casillas que pueden moverse el ALFIL
#define MAX_CASILLAS_ALFIL 7  
// Máximo número de casillas que pueden moverse la DAMA
#define MAX_CASILLAS_DAMA 7  
// Máximo número de casillas que puede moverse el PEON
#define MAX_CASILLAS_PEON   1
// Máximo número de casillas que puede moverse el REY
#define MAX_CASILLAS_REY    1

typedef struct _AJD_MovInfo {
    uint8_t casilla_origen:6;
    uint8_t casilla_destino:6;
    AJD_Color color_pieza_origen;
    AJD_Color color_pieza_destino;
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
int compruebaPeon (AJD_TableroPtr tablero, AJD_MovInfo* mov_info);
////////////////////////////////////////////////////////////////////////////
// compruebaAlfil
// 
// Comprueba si el movimiento de un alfil en casilla_origen hacia
// casilla_destino es válido.
//
int compruebaAlfil (AJD_TableroPtr tablero, AJD_MovInfo* mov_info);
////////////////////////////////////////////////////////////////////////////
// compruebaTorre
// 
// Comprueba si el movimiento de una torre en casilla_origen hacia
// casilla_destino es válido.
//
int compruebaTorre (AJD_TableroPtr tablero, AJD_MovInfo* mov_info);
////////////////////////////////////////////////////////////////////////////
// hayPiezaValida
//
// Comprueba si la casilla seleccionada por el cursor contiene una pieza
// válida para mover
//
// Devuelve 1 si lo es, 0 en caso contrario.
//
int hayPiezaValida (AJD_TableroPtr tablero, AJD_idCasilla idCasilla, AJD_Estado* estado_juego);
////////////////////////////////////////////////////////////////////////////
// hayPieza
//
// Comprueba si en la casilla indicada hay alguna pieza (1) o esta libre (0)
//
int hayPieza (AJD_TableroPtr tablero, AJD_idCasilla idCasilla);
////////////////////////////////////////////////////////////////////////////
// hayPiezaEnTrayectoria
//
// Comprueba si alguna de las casillas desde una casilla origen hasta una 
// casilla destino está ocupada (1) o libre (0). 
// Las casillas origen/destino se infican en una estructura AJD_MovInfo.
// 
int hayPiezaEnTrayectoria (AJD_TableroPtr tablero, AJD_MovInfo* mov_info);
