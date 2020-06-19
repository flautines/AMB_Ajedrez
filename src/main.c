#include <tablero.h>
#include <interfaz.h>
#include <juego.h>
#include <menu.h>

///////////////////////////////////////////////////////////////////////////////
// MAIN
//
int main()
{
   AJD_Tablero tablero;
   int celda_origen, celda_destino;

   inicializa (&tablero);

   nuevoJuego (&tablero);
   menuJuego (&estado_juego);
   
   for (;;)
   {
   		dibujaTablero (&tablero);
   		dibujaMarcadores (turno, &estado_juego);
   		obtenJugada(&celda_origen, &celda_destino);	
   		muevePieza (&tablero, celda_origen, celda_destino);
   }
   
   
   liberaRecursos();

   return 0;
}