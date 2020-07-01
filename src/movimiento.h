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

// id casillas involucrados en un enroque
#define ENROQUE_LARGO_ORIGEN_T_BLANCA   56  // T BLANCA desde 1a
#define ENROQUE_LARGO_DESTINO_T_BLANCA  59  // T BLANCA hacia 1d
#define ENROQUE_CORTO_ORIGEN_T_BLANCA   63  // T BLANCA desde 1h
#define ENROQUE_CORTO_DESTINO_T_BLANCA  61  // T BLANCA hacia 1f
#define ENROQUE_LARGO_ORIGEN_T_NEGRA     0  // T NEGA   desde 8a
#define ENROQUE_LARGO_DESTINO_T_NEGRA    3  // T NEGA   hacia 8d
#define ENROQUE_CORTO_ORIGEN_T_NEGRA     7  // T NEGA   desde 8h
#define ENROQUE_CORTO_DESTINO_T_NEGRA    5  // T NEGA   hacia 8f
#define ENROQUE_ORIGEN_R_BLANCO         60  // R BLANCO desde 1e
#define ENROQUE_LARGO_DESTINO_R_BLANCO  58  // R BLANCO hacia 1c
#define ENROQUE_CORTO_DESTINO_R_BLANCO  62  // R BLANCO hacia 1h
#define ENROQUE_ORIGEN_R_NEGRO           4  // R NEGRO  desde 8e
#define ENROQUE_LARGO_DESTINO_R_NEGRO    2  // R NEGRO  hacia 8c
#define ENROQUE_CORTO_DESTINO_R_NEGRO    6  // R NEGRO  hacia 8h


typedef struct _AJD_MovInfo {
    AJD_CasillaPtr origen;
    AJD_CasillaPtr destino;
    uint8_t srcY:3;
    uint8_t srcX:3;
    uint8_t dstY:3;
    uint8_t dstX:3;
    int dy:4;
    int dx:4;  
    int distY:4;
    int distX:4;
} AJD_MovInfo;
////////////////////////////////////////////////////////////////////////////
// esMovimientoValido
//
//  Comprueba si un movimiento desde una casilla origen a destino
// es válido.
//
int esMovimientoValido (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego);
////////////////////////////////////////////////////////////////////////////
// compruebaMovimientoPieza
//
//  Comprueba si la pieza a mover lo hace segun sus reglas
//
int compruebaMovimientoPieza (AJD_MovInfo* movInfo);
////////////////////////////////////////////////////////////////////////////
// muevePieza
//
//  Mueve una pieza desde una casilla origen a una casilla destino determinado
// en estado_juego
//
void muevePieza (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego);
////////////////////////////////////////////////////////////////////////////
// casillaLibre
//
//  Comprueba si en la casilla indicada hay una pieza sin importar color
//
int casillaOcupada (AJD_CasillaPtr casilla);
////////////////////////////////////////////////////////////////////////////
// hayPiezaValida
//
//  Comprueba si en la casilla indicada hay una pieza del jugador que 
// efectua la jugada actual.
//
int hayPiezaValida (AJD_TableroPtr tablero, AJD_CasillaPtr casilla, AJD_EstadoPtr estado_juego);
////////////////////////////////////////////////////////////////////////////
// obtenMovInfo
//
//  Obtiene detalles necesarios para realizar comprobaciones posteriores
// respecto las casillas origen y destino de un movimiento.
//
AJD_MovInfo obtenMovInfo (AJD_EstadoPtr estado_juego);
