#pragma once

/**  Tipo booleano
***************************************************************************************/
#ifdef TRUE
   #undef TRUE
   #undef FALSE
#endif
typedef enum { FALSE, TRUE } AJD_Bool;

/**  Tipo Color piezas/casillas 
 ***************************************************************************************/
typedef enum { NEGRO, BLANCO } AJD_Color;

/**  Tipo de enroques
 ***************************************************************************************/
typedef enum { ER_NONE, ER_LARGO, ER_CORTO } AJD_Enroque;

/**  Tipo de pieza+color
 ***************************************************************************************/
typedef enum { 
      NO_PIEZA,
      PEON_B, TORRE_B, CABALLO_B, ALFIL_B, DAMA_B, REY_B,
      PEON_N, TORRE_N, CABALLO_N, ALFIL_N, DAMA_N, REY_N

} AJD_Pieza;

/** Opciones posibles de selección de casilla con el cursor
 ***************************************************************************************/
typedef enum { 
   NO_SELECCION, 
   ORIGEN_SELECCIONADO, 
   DESTINO_SELECCIONADO
} AJD_Seleccion;

/** Constantes en notacion algebraica de cada indice de casilla
 ***************************************************************************************/
typedef enum { 
   a8, b8, c8, d8, e8, f8, g8, h8,
   a7, b7, c7, d7, e7, f7, g7, h7,
   a6, b6, c6, d6, e6, f6, g6, h6,
   a5, b5, c5, d5, e5, f5, g5, h5,
   a4, b4, c4, d4, e4, f4, g4, h4,
   a3, b3, c3, d3, e3, f3, g3, h3,
   a2, b2, c2, d2, e2, f2, g2, h2,
   a1, b1, c1, d1, e1, f1, g1, h1
} AJD_idCasilla;

/** Informacion de cada casilla [color casilla, pieza en esa casilla]
 ***************************************************************************************/
typedef struct {
   AJD_Color   color:1;
   AJD_Pieza     pieza:4;
} AJD_Casilla, *AJD_CasillaPtr;

/** Información mínima imprescindible para almacenar un movimiento
 ***************************************************************************************/
typedef struct {
      AJD_idCasilla   idOrigen   :6;
      AJD_idCasilla   idDestino  :6;
      AJD_Bool        captura    :1;
      AJD_Bool        jaque      :1;
      AJD_Enroque     enroque    :2;
      AJD_Bool        mate       :1;
} AJD_Movimiento, *AJD_MovimientoPtr;

/** Información de cada jugada
 ***************************************************************************************/
typedef struct {
   int            nturno;
   AJD_Movimiento movBlancas;
   AJD_Movimiento movNegras;
} AJD_Jugada, *AJD_JugadaPtr;

/** Bytes/chars totales en el sprite de cursor
 ***************************************************************************************/
#define NCHARS_IN_SPRITE 9

/** Representación gráfica del cursor (por ahora son chars de texto)
 ***************************************************************************************/
typedef struct 
{
   int ch[NCHARS_IN_SPRITE];
} AJD_Sprite;

/** Datos relativos a cursores de casillas
 ***************************************************************************************/
typedef struct 
{
   AJD_CasillaPtr casilla;
   uint8_t        visible:1;
   uint8_t        flash:1;
   AJD_Sprite*    sprite;
} AJD_Cursor, *AJD_CursorPtr;

/** Tipo para representar un tablero de ajedrez
 ***************************************************************************************/
typedef struct {
   AJD_Casilla casillas[8*8];
   AJD_Cursor  curFijo;
   AJD_Cursor  curMovil;
} AJD_Tablero, *AJD_TableroPtr;

/****************************************************************************************
 * Estado de juego
 *
 * jugada .............  Registro de jugada actual (# turno, mov. blancas, mov. negras)
 * jueganBlancas ......  TRUE si es el turno de las blancas
 * jugadorActual ......  Mueve jugador 1 (0) / jugador 2 (1)
 * partida ............  Registro de TODAS las jugadas
 * segundosBlancas ....  # segundos desde inicio de juego usados por BLANCAS
 * segundosNegras .....  # segundos desde inicio de juego usados por NEGRAS
 * enroquePermitido ...  Enroques (largos/cortos) permitidos para blancas y negras
 * casillaSeleccionada   Informacion de la selección de casilla efectuada con el cursor
 * finJuego ...........  TRUE si se ha seleccionado salir del juego
 *  
 ***************************************************************************************/
typedef struct {
    AJD_Jugada          jugada;
    AJD_Bool            jueganBlancas : 1;
    uint8_t             jugadorActual : 1;
    /*AJD_ListaJugadas    partida */
    uint16_t            segundosBlancas;
    uint16_t            segundosNegras;
    /*AJD_ERPerm          enroquePermitido;*/
    AJD_Seleccion       casillaSeleccionada;
    AJD_Bool            finJuego;
} AJD_Estado, *AJD_EstadoPtr;
