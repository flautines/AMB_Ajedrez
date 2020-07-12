#include <common.h>
///////////////////////////////////////////////////////////////////////////////
// MAIN
//
int main()
{
   AJD_Tablero tablero;

   inicializa (&tablero);

   menu();
   nuevoJuego (&tablero);
   
   ejecutaPartida (&tablero);
   
   
   liberaRecursos();

   return 0;
}
