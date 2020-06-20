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

   		procesaTeclado (&tablero);
   		//obtenJugada(&celda_origen, &celda_destino);	
   		//muevePieza (&tablero, celda_origen, celda_destino);
   		//estado_juego.turno_jugador ^= 1;
   		//estado_juego.juegan_blancas ^= 1;   		
   		//turno += estado_juego.juegan_blancas;
   }
   
   
   liberaRecursos();

   return 0;
}
