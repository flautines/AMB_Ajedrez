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

/**  Tipo indice de casilla (0..63)
 ***************************************************************************************/
/*typedef uint8_t AJD_idCasilla;*/

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

/** Tipo para representar un tablero de ajedrez
 ***************************************************************************************/
typedef struct {
      AJD_Casilla casillas[8*8];    
} AJD_Tablero, *AJD_TableroPtr;

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
   AJD_Movimiento movNnegras;
} AJD_Jugada, *AJD_JugadaPtr;
