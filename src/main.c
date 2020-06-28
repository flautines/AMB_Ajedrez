#include <interfaz.h>
#include <juego.h>
#include <menu.h>
#include <tablero.h>
///////////////////////////////////////////////////////////////////////////////
// MAIN
//
int main()
{
   AJD_Tablero tablero;

   inicializa (&tablero);

   nuevoJuego (&tablero);
   menuJuego (&estado_juego);
   
   for (;;)
   {
      dibujaJuego (&tablero);

      actualizaJuego (&tablero);
   }
   
   
   liberaRecursos();

   return 0;
}
