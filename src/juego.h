#pragma once

#include <tablero.h>
#include <stdint.h>

// Tipo para representar el estado
/*
| 7 | 0: turno jugador 1        |  | 3 | 0: torre negra no movido  |
|   | 1: turno jugador 2        |  |   | 1: torre negra movido     |
| 6 | 0: rey blanco no movido   |  | 2 | 0: rey blanco no en jaque |
|   | 1: rey blanco movido      |  |   | 1: rey blanco en jaque    |
| 5 | 0: rey negro no movido    |  | 1 | 0: rey negro no en jaque  | 
|   | 1: rey negro movido       |  |   | 1: rey negro en jaque     |
| 4 | 0: torre blanca no movido |  | 0 | 0: juegan negras          |
|   | 1: torre blanca movido    |  |   | 1: juegan blancas         |
*/
typedef struct _AJD_Estado
{
   uint8_t juegan_blancas      : 1; // bit 0
   uint8_t negro_jaque         : 1; // bit 1
   uint8_t blanco_jaque        : 1; // bit 2
   uint8_t torre_negra_movida  : 1; // bit 3
   uint8_t torre_blanca_movida : 1; // bit 4    
   uint8_t rey_negro_movido    : 1; // bit 5
   uint8_t rey_blanco_movido   : 1; // bit 6
   uint8_t turno_jugador       : 1; // bit 7
} AJD_Estado;

/////////////////////////////////////////////////////////////////////
// INTERFAZ PUBLICA
//
extern uint16_t   turno;          // Cuántos turnos se han jugado ya
extern AJD_Estado estado_juego;   // Estado del juego

void inicializa(AJD_TableroPtr tablero);
void nuevoJuego(AJD_TableroPtr tablero);
void muevePieza (AJD_TableroPtr tablero, int casilla_origen, int casilla_destino);
void liberaRecursos();
