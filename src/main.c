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

   inicializa (&tablero);

   menuJuego();
   nuevoJuego (&tablero);
   
   dibujaTablero (&tablero);
   dibujaMarcadores (turno, &estado_juego);
   
   liberaRecursos();

   return 0;
}