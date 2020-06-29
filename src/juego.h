#pragma once

#include <tablero.h>
#include <stdint.h>

// Tipo para representar el estado
typedef struct _AJD_Estado
{
   uint8_t juegan_blancas      : 1; 
   uint8_t negro_jaque         : 1; 
   uint8_t blanco_jaque        : 1; 
   uint8_t enroque_largo_blanco_invalidado  : 1;
   uint8_t enroque_corto_blanco_invalidado  : 1;
   uint8_t enroque_largo_negro_invalidado  : 1;
   uint8_t enroque_corto_negro_invalidado  : 1;
   //uint8_t rey_negro_movido    : 1; 
   //uint8_t rey_blanco_movido   : 1; 
   uint8_t turno_jugador       : 1; 
   uint8_t casilla_seleccionada: 2; // 00: Ninguna casilla seleccionada
                                    // 01: Celda origen seleccionada
                                    // 02: Celda destino seleccionada
   uint8_t casilla_origen;          // casilla origen de la pieza a mover
   uint8_t casilla_destino;         // casilla destino de la pieza a mover

} AJD_Estado;

/////////////////////////////////////////////////////////////////////
// INTERFAZ PUBLICA
//
extern uint16_t   turno;          // Cuántos turnos se han jugado ya
extern AJD_Estado estado_juego;   // Estado del juego

void inicializa(AJD_TableroPtr tablero);
void nuevoJuego(AJD_TableroPtr tablero);
void actualizaJuego (AJD_TableroPtr tablero);
void liberaRecursos();
