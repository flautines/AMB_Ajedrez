#include <stdio.h>
#include <string.h>
#include <juego.h>
#include <tablero.h>
#include <interfaz.h>
#include <movimiento.h>
//#include <ncurses.h>

////////////////////////////////////////////////////////////////////////////
// FUNCIONES PRIVADAS forward declarations
void _colocaPiezas (AJD_TableroPtr tablero);

////////////////////////////////////////////////////////////////////////////
// VARIABLES PRIVADAS AL MÓDULO
uint16_t   turno;        // Cuántos turnos se han jugado ya
AJD_Estado estado_juego;   // Estado del juego

////////////////////////////////////////////////////////////////////////////
// INTERFAZ PÚBLICA
////////////////////////////////////////////////////////////////////////////
// INICIALIZA
//
// Pone todas las casillas del tablero a su estado inicial:
//    - Todas las casillas están vacías
//    - El color de cada casilla alterna entre blanco y negro y la
//      casilla inferior derecha es de color blanco
//
void inicializa(AJD_TableroPtr tablero)
{
   // El cursor de seleccion es visible y sin flash
   tablero->cursor.visible = 1;
   tablero->cursor.flash = 0;

   AJD_Color color = NEGRO;
   AJD_idCasilla indice = 0;

   for (int i=0; i<8; i++) 
   {      
      color ^= 1; // Alterna entre blanco/negro       
      for (int j=0; j<8; j++)    
      {
         // puntero a casilla actual, mejora legibilidad codigo
         AJD_Casilla* casilla = &(tablero->casilla[i*8+j]);

         casilla->color = color;
         color ^= 1; // Alterna entre blanco/negro       
         
         // La esquina superior izquierda es 'a8', hacia la derecha se 
         // incrementa la letra hasta la 'h' y de arriba a abajo 
         // decrementa el número.
         //casilla->notacion[0] = 'a' + j;
         //casilla->notacion[1] = '8'-i;
         //casilla->notacion[2] = '\0';  // NULL terminated string

         // Inicialmente el tablero está vacío
         // El color de la pieza cuando la casilla está vacía es irrelevante
         casilla->pieza = NONE;
         casilla->color_pieza = BLANCO;
         casilla->indice = indice++;
      }
   }   
   //printf("sizeof(AJD_Tablero) = %ld\n", sizeof (AJD_Tablero));

   // Inicializa la UI
   inicializaPantalla();
}
////////////////////////////////////////////////////////////////////////////
// liberaRecursos
//
// Libera los recursos usados por el programa, incluída la UI
void liberaRecursos()
{
   liberaPantalla();
}
////////////////////////////////////////////////////////////////////////////
// nuevoJuego
//
// Prepara el estado del juego y el tablero para una partida nueva:
//
//    - Estado del juego: ninguna pieza movida, ningún rey en jaque,
//      turno del jugador 1, juegan blancas
//
//    - Coloca las piezas en el tablero para una partida nueva
//
//    - Turnos jugados = 0
//
void nuevoJuego(AJD_TableroPtr tablero)
{
   // Estado del juego
   memset(&estado_juego, 0, sizeof (AJD_Estado));
   estado_juego.juegan_blancas = 1;

   // Turno
   turno = 1;

   // Coloca las piezas
   _colocaPiezas (tablero);

   // Posicion inicial del cursor de selección de pieza
   tablero->cursor.idCasilla = 8*6+3; //(d2)
}
////////////////////////////////////////////////////////////////////////////
// actualizaJuego
//
// Actualiza el estado del juego
void actualizaJuego (AJD_TableroPtr tablero)
{
   procesaTeclado (tablero, &estado_juego);

   switch (estado_juego.casilla_seleccionada)
   {   
   case 1:
      if (!hayPiezaValida(tablero, estado_juego.casilla_origen, &estado_juego))
      {
         estado_juego.casilla_origen = 0;
         estado_juego.casilla_seleccionada = 0;
      }
      else
      {
         // Casilla origen seleccionada, flash cursor
         tablero->cursor.flash = 1;        
      }
      break;
   case 2:
      if (esMovimientoValido (tablero, &estado_juego))
      {
         muevePieza (tablero, 
                     estado_juego.casilla_origen, 
                     estado_juego.casilla_destino);

         estado_juego.turno_jugador ^= 1;
         estado_juego.juegan_blancas ^= 1;
         estado_juego.casilla_seleccionada = 0;
         turno += estado_juego.juegan_blancas;
         estado_juego.casilla_origen = estado_juego.casilla_destino = 0;

         // movimiento efectuado, restaura el cursor a "no-flash"
         tablero->cursor.flash = 0;
      }
      else
      {
         estado_juego.casilla_destino = 0;
         estado_juego.casilla_seleccionada = 1;
      }
      break;
   }
}
////////////////////////////////////////////////////////////////////////////
// muevePieza
//
// Mueve una pieza desde una casilla origen a una casilla destino
void muevePieza (AJD_TableroPtr tablero, uint8_t casilla_origen, uint8_t casilla_destino)
{
   AJD_Pieza pieza_a_mover = tablero->casilla[casilla_origen].pieza;   
   AJD_Color color_pieza   = tablero->casilla[casilla_origen].color_pieza;   

   tablero->casilla[casilla_origen].pieza        = NONE;
   tablero->casilla[casilla_destino].pieza       = pieza_a_mover;
   tablero->casilla[casilla_destino].color_pieza = color_pieza;
}
////////////////////////////////////////////////////////////////////////////
// INTERFAZ PRIVADA
////////////////////////////////////////////////////////////////////////////
// _colocaPiezas
//
// Dispone las piezas en el tablero para una partida nueva
//
void _colocaPiezas(AJD_TableroPtr tablero)
{   
   AJD_Pieza piezasMayores[8] = { TORRE, CABALLO, ALFIL, DAMA, REY, ALFIL, CABALLO, TORRE };
   for (int col=0; col < 8; col++)
   {

      tablero->casilla[col].pieza               = piezasMayores[col]; // fila 1: piezas mayores negras
      tablero->casilla[col].color_pieza         = NEGRO;

      tablero->casilla[7*8 + col].pieza         = piezasMayores[col]; // fila 8: piezas mayores blancas
      //tablero->casilla[7*8 + col].color_pieza   = BLANCO;

      tablero->casilla[8 + col].pieza           = PEON;               // fila 2: peones negros
      tablero->casilla[8 + col].color_pieza     = NEGRO;
      
      tablero->casilla[6*8 + col].pieza         = PEON;               // fils 7: peones blancos
      //tablero->casilla[6*8 + col].color_pieza   = BLANCO;          
   }
}
