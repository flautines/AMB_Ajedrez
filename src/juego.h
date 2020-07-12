#pragma once
/*#include <tablero.h>
#include <stdint.h>
#include <time.h>
*/
/////////////////////////////////////////////////////////////////////////////////////////
// Tipo booleano
#ifdef TRUE
   #undef TRUE
   #undef FALSE
#endif
typedef enum {
   FALSE,
   TRUE,
} AJD_Bool;
/////////////////////////////////////////////////////////////////////////////////////////
// Opciones selección de casilla con el cursor
typedef enum { 
   NO_SELECCION, 
   ORIGEN_SELECCIONADO, 
   DESTINO_SELECCIONADO
} AJD_Seleccion;
/////////////////////////////////////////////////////////////////////////////////////////
// Tipos de enroque
typedef enum { 
   NO_ENROQUE, 
   ENROQUE_LARGO, 
   ENROQUE_CORTO 
} AJD_Enroque;
/////////////////////////////////////////////////////////////////////////////////////////
// Opciones posibles respecto a poder realizar o no enroques (largo o corto) por parte
// de blancas y/o negras.
typedef enum { 
   ERLARGO_PROHIBIDO_B  = 0x01,  // 0b0001
   ERCORTO_PROHIBIDO_B  = 0x02,  // 0b0010
   ERLARGO_PROHIBIDO_N  = 0x04,  // 0b0100
   ERCORTO_PROHIBIDO_N  = 0x08   // 0b1000
} AJD_ERPerm;
/////////////////////////////////////////////////////////////////////////////////////////
// Opciones para determinar qué jugador está en jaque
typedef enum { 
   NO_JAQUE, 
   NEGRO_EN_JAQUE, 
   BLANCO_EN_JAQUE 
} AJD_Jaque;
/////////////////////////////////////////////////////////////////////////////////////////
// Tipo para almacenar información sobre cada turno
typedef struct {
   uint8_t     njugador       : 1;
   AJD_Bool    juega_humano   : 1;
   AJD_Bool    juegan_blancas : 1;
   AJD_Casilla origen;
   AJD_Casilla destino;        
   AJD_Enroque enroque        : 2;
   AJD_Bool    come_pieza     : 1;
   AJD_Bool    jaque          : 1;
   AJD_Bool    mate           : 1;
} AJD_Turno, *AJD_TurnoPtr;
/////////////////////////////////////////////////////////////////////////////////////////
// Tipo para representar un nodo en la lista de turnos a guardar
//typedef struct *AJD_TurnoNodo AJD_TurnoPtr;
typedef struct _AJD_TurnoNodo* AJD_TurnoNodoPtr;
struct _AJD_TurnoNodo {  
   AJD_Turno        turno;
   AJD_TurnoNodoPtr next;
} AJD_TurnoNodo; 
/////////////////////////////////////////////////////////////////////////////////////////
// Tipo para representar la lista de turnos a guardar
typedef struct {
   AJD_TurnoNodoPtr begin;
   AJD_TurnoNodoPtr end;
   uint16_t         len;
} AJD_ListaTurnos;
/////////////////////////////////////////////////////////////////////////////////////////
// Tipo para representar el estado del juego
typedef struct {
   uint16_t        nturno;
   AJD_Turno       turno_actual;
   AJD_ListaTurnos partida;
   uint16_t        segundos_blancas;
   uint16_t        segundos_negras;
   AJD_Jaque       en_jaque             : 2;
   AJD_ERPerm      enroque_permitido    : 4;
   AJD_Seleccion   casilla_seleccionada : 2; 
   AJD_Bool        fin_juego            : 1;
} AJD_Estado, *AJD_EstadoPtr;
/////////////////////////////////////////////////////////////////////////////////////////
// INTERFAZ PUBLICA
//
void inicializa(AJD_TableroPtr tablero);
void nuevoJuego(AJD_TableroPtr tablero);
void ejecutaPartida (AJD_TableroPtr tablero);
void actualizaJuego (AJD_TableroPtr tablero);
void menu();
void liberaRecursos();
