#include <common.h>
/*
#include <time.h>
#include <stdio.h>
*/
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////
// FUNCIONES PRIVADAS forward declarations
void tableroDisposicionInicial (AJD_TableroPtr tablero);
void actualizaCrono ();

////////////////////////////////////////////////////////////////////////////
// VARIABLES PRIVADAS AL MÓDULO
AJD_Casilla casilla_null;
AJD_Estado estado_juego;   // Estado del juego
static time_t crono;       // Temporizador para contar tiempo

/////////////////////////////////////////////////////////////////////////////////////////
// INTERFAZ PÚBLICA
/////////////////////////////////////////////////////////////////////////////////////////
// INICIALIZA
//
// Pone todas las casillas del tablero a su estado inicial:
//    - Todas las casillas están vacías
//    - El color de cada casilla alterna entre blanco y negro y la
//      casilla inferior derecha es de color blanco
//
void inicializa(AJD_TableroPtr tablero)
{
   AJD_Color color = NEGRO;
   AJD_idCasilla id = 0;

   for (int i=0; i<8; i++) 
   {
      // Alterna entre blanco/negro
      color ^= 1; 
      for (int j=0; j<8; j++)    
      {
         // puntero a casilla actual, mejora legibilidad codigo
         AJD_CasillaPtr casilla = &(tablero->casilla[i*8+j]);

         casilla->color = color;
         color ^= 1; // Alterna entre blanco/negro       
         
         // Inicialmente el tablero está vacío
         // El color de la pieza cuando la casilla está vacía es irrelevante
         casilla->pieza = NONE;
         casilla->color_pieza = BLANCO;
         casilla->id = id++;
      }
   }

   // Inicializa la UI
   inicializaPantalla();
   inicializaSprites(tablero);
}
/////////////////////////////////////////////////////////////////////////////////////////
// liberaRecursos
//
// Libera los recursos usados por el programa, incluída la UI
void liberaRecursos()
{   
   liberaPantalla();
   puts ("ncurses finalizado");
}
/////////////////////////////////////////////////////////////////////////////////////////
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
   
   // Restablecer tiempos de ambos jugadores 
   estado_juego.segundos_blancas = 0;
   estado_juego.segundos_negras  = 0;

   // Coloca las piezas
   tableroDisposicionInicial (tablero);

   // El cursor movil y de pieza seleccionada se posicionan a d2
   // El cursor movil es visible y sin flash
   // El cursor de pieza seleccionada no es visible y con flash
   tablero->cursorMovil.casilla = tablero->cursorPiezaSeleccionada.casilla = &tablero->casilla[d2];   
   tablero->cursorMovil.visible = 1;
   tablero->cursorPiezaSeleccionada.visible = 0;
   tablero->cursorMovil.flash = 0;
   tablero->cursorPiezaSeleccionada.flash = 1;

   // Turno actual   
   estado_juego.nturno = 1;
   estado_juego.turno_actual.juegan_blancas = TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////
// actualizaJuego
//
// Actualiza el estado del juego
void actualizaJuego (AJD_TableroPtr tablero)
{
   AJD_TurnoPtr turno_actual = &(estado_juego.turno_actual);

   actualizaCrono ();
   procesaTeclado (tablero, &estado_juego);
   
   switch (estado_juego.casilla_seleccionada)
   {   
   case ORIGEN_SELECCIONADO:
   {
      AJD_CasillaPtr casilla_origen = &(estado_juego.turno_actual.origen);
      if (!hayPiezaValida(tablero, casilla_origen, turno_actual))
      {
         turno_actual->origen = casilla_null;
         estado_juego.casilla_seleccionada = NO_SELECCION;
      }
      else
      {
         // Casilla origen seleccionada, muestra el cursor fijo
         tablero->cursorPiezaSeleccionada.visible = 1;
      }
      break;
   }
   case DESTINO_SELECCIONADO:
   {
      

      // Si se ha seleccionado un movimiento valido efectuar las correspondientes
      // acciones y actualizar información para el siguiente turno.
      if (esMovimientoValido (tablero, &estado_juego))
      {
         if (estado_juego.turno_actual.enroque)
            efectuaEnroque (tablero, &estado_juego);
         else
            muevePieza (tablero, &estado_juego);

         if (peonUltimaFila (tablero, turno_actual))
            promocionaPeon (tablero, turno_actual);

         // Siguiente turno
         estado_juego.casilla_seleccionada = NO_SELECCION;
         turno_actual->njugador ^= 1;
         turno_actual->juegan_blancas ^= 1;         
         estado_juego.nturno += turno_actual->juegan_blancas;
         
         // Resetea los datos de las casillas origen y destino para el siguiente turno
         turno_actual->origen    = casilla_null;
         turno_actual->destino   = casilla_null;

         turno_actual->enroque = NO_ENROQUE;

         // movimiento efectuado, oculta el cursor fijo
         tablero->cursorPiezaSeleccionada.visible = 0;

         // Restablece contadores de tiempo
         time (&crono);
      }
      // Si no es un movimiento valido
      // Resetea los datos de casilla destino de este turno y marcar que no se ha 
      // seleccionado un destino con el cursor.
      else
      {
         turno_actual->destino = casilla_null;
         estado_juego.casilla_seleccionada = ORIGEN_SELECCIONADO;
      }
      break;
   }
   default:
      break;
   }

}
/////////////////////////////////////////////////////////////////////////////////////////
// menu
// 
// Ejecuta los diferentes menus previos al comienzo
void menu()
{
   menuJuego (&estado_juego);
}
/////////////////////////////////////////////////////////////////////////////////////////
//
// Bucle principal, se ejecuta hasta que termina la partida.
//
void ejecutaPartida (AJD_TableroPtr tablero)
{
   while (!estado_juego.fin_juego)
   {
      dibujaJuego (tablero, &estado_juego);

      actualizaJuego (tablero);
   }
}
/////////////////////////////////////////////////////////////////////////////////////////
// INTERFAZ PRIVADA
void actualizaCrono()
{
    // Actualización del cronometro
   if (difftime (time(NULL), crono) >= 1.0)
   {
     if (estado_juego.turno_actual.juegan_blancas)
         estado_juego.segundos_blancas++;
     else
         estado_juego.segundos_negras++;
     time(&crono);
   }
}
