#pragma once

#include <tablero.h>
#include <stdint.h>

typedef enum {NO_SELECCION, ORIGEN_SELECCIONADO, DESTINO_SELECCIONADO} AJD_Seleccion;

// Tipo para representar el estado
typedef struct _AJD_Estado
{
   uint16_t turno;                     // Cuántos turnos se han jugado ya
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
   AJD_Seleccion casilla_seleccionada: 2; 
   AJD_CasillaPtr casilla_origen;  // casilla origen de la pieza a mover
   AJD_CasillaPtr casilla_destino; // casilla destino de la pieza a mover   
} AJD_Estado;

typedef struct _AJD_Estado* AJD_EstadoPtr;
/////////////////////////////////////////////////////////////////////
// INTERFAZ PUBLICA
//

void inicializa(AJD_TableroPtr tablero);
void nuevoJuego(AJD_TableroPtr tablero);
void ejecutaPartida (AJD_TableroPtr tablero);
void actualizaJuego (AJD_TableroPtr tablero);
void menu();
void liberaRecursos();
