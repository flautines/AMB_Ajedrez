#pragma once
#include <ncurses.h>
#include <stdint.h>

// BOOL typedef
#ifndef bool
   typedef uint8_t bool;
   #define true 1
   #define false 0
#endif

// Color enum (piezas o casillas)
typedef enum { NEGRO, BLANCO } AJD_Color;

// Identificador de pieza
typedef enum { NONE, PEON, TORRE, CABALLO, ALFIL, DAMA, REY } AJD_Pieza;

// Constantes para mejorar legibilidad del codigo al acceder a indices de casillas
enum { 
   a8, b8, c8, d8, e8, f8, g8, h8,
   a7, b7, c7, d7, e7, f7, g7, h7,
   a6, b6, c6, d6, e6, f6, g6, h6,
   a5, b5, c5, d5, e5, f5, g5, h5,
   a4, b4, c4, d4, e4, f4, g4, h4,
   a3, b3, c3, d3, e3, f3, g3, h3,
   a2, b2, c2, d2, e2, f2, g2, h2,
   a1, b1, c1, d1, e1, f1, g1, h1,
};


///////////////////////////////////////////////////////////////////////////////
// Casilla
//
// El tipo casilla guarda información de una casilla en el tablero así como
// qué pieza hay en esa casilla.
//

typedef uint8_t AJD_idCasilla ;
typedef struct _AJD_Casilla
{
   AJD_Color     color:1;      // Color de la casilla (BLANCO o NEGRO)
   AJD_Pieza     pieza:3;      // Tipo de pieza en la casilla
   AJD_Color     color_pieza:1;// Color de la pieza en esta casilla
   AJD_idCasilla id;       // Indice en el array de casillas (0..63)
} AJD_Casilla;

typedef struct _AJD_Casilla* AJD_CasillaPtr;

////////////////////////////////////////////////////////////////////////////
// Cursor de selección de pieza
#define NCHARS_IN_SPRITE 9
typedef struct _AJD_Sprite {
   chtype ch[NCHARS_IN_SPRITE];
} AJD_Sprite;

typedef struct 
{
   AJD_CasillaPtr casilla;
   uint8_t     visible:1;
   uint8_t     flash:1;
   AJD_Sprite* sprite;
} AJD_Cursor;

//AJD_Cursor _cursorSeleccion;
typedef struct _AJD_Tablero
{
   AJD_Casilla casilla[8*8];
   AJD_Cursor cursorPiezaSeleccionada;
   AJD_Cursor cursorMovil;
} AJD_Tablero;

typedef struct _AJD_Tablero* AJD_TableroPtr;

