#pragma once
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

///////////////////////////////////////////////////////////////////////////////
// Casilla
//
// El tipo casilla guarda información de una casilla en el tablero así como
// qué pieza hay en esa casilla.
//
typedef struct _AJD_Casilla
{
   AJD_Color   color:1;      // Color de la casilla (BLANCO o NEGRO)
   //char        notacion[3];// Notación para la casilla ej: 'h2' ("a..h1..8\0")
   AJD_Pieza   pieza:3;      // Tipo de pieza en la casilla
   AJD_Color   color_pieza:1;// Color de la pieza en esta casilla
} AJD_Casilla;

////////////////////////////////////////////////////////////////////////////
// Cursor de selección de pieza
typedef struct 
{
   uint8_t idCasilla:6;
   uint8_t visible:1;
   uint8_t flash:1;
} AJD_Cursor;

//AJD_Cursor _cursorSeleccion;
typedef struct _AJD_Tablero
{
   AJD_Casilla casilla[8*8];
   AJD_Cursor cursor;
} AJD_Tablero;

typedef struct _AJD_Tablero* AJD_TableroPtr;
typedef struct _AJD_Casilla* AJD_CasillaPtr;
