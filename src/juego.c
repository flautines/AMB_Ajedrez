#include <common.h>
#include <movimiento.h>
#include <log.h>
#include <stdio.h>
/****************************************************************************************
 * Variables PRIVADAS
 ***************************************************************************************/
AJD_Estado estadoJuego;
AJD_Estado estadoJuegoInicio = {
    {1,                             /* Jugada 1             */
        { e2, e2,                   /* Origen/Destino movimiento Blancas */
            {0, 0, 0, 0}            /* Flags del movimiento anterior */
        },               
        { e7, e7,                   /* Origen/Destino movimiento Blancas */
            {0, 0, 0, 0 }           /* Flags del movimiento anterior */
        }
    },
    TRUE,                           /* Juegan Blancas       */
    0,                              /* Tiempo Blancas       */
    0,                              /* Tiempo Negras        */
    NO_SELECCION                    /* Casilla seleccionada */
};
/* TODO: este buffer debe eliminarse, es para probar log de partida */
char buff[1024];
/****************************************************************************************
 ****************************************************************************************
 * Funciones PUBLICAS (Implementacion)
 ****************************************************************************************
 ***************************************************************************************/
/****************************************************************************************
 * inicializa
 *
 * Inicializa todos los elementos del juego: tablero, librerías de pantalla, etc.
 *
 ***************************************************************************************/
void inicializa()
{
    inicializaTablero();
    inicializaPantalla();

    /* Inicializa sprites de los cursores */
    inicializaSprites(obtenTableroPtr());

    /* Inicializa el log que guarda los movimientos */
    initLogPartida();
}
/****************************************************************************************
 * liberaRecursos
 *
 * Libera los recursos usados por el programa, incluída la UI
 ***************************************************************************************/
void liberaRecursos()
{       
    liberaPantalla();
    puts ("ncurses finalizado");
    liberaLogPartida();
}
/****************************************************************************************
 * nuevoJuego
 *
 * Prepara el estado del juego y el tablero para una partida nueva:
 *
 *    - Estado del juego: ninguna pieza movida, ningún rey en jaque,
 *      turno del jugador 1, juegan blancas, contadores a cero
 *
 *    - Coloca las piezas en el tablero para una partida nueva
 *
 *    - Comienza el turno 1
 *
 ***************************************************************************************/
void nuevoJuego()
{
    AJD_TableroPtr tablero;
    tableroDisposicionInicial();

    estadoJuego = estadoJuegoInicio;
   
    tablero = obtenTableroPtr();
    /**********************************************
     * El cursor movil y fijo se posicionan a d2
     * El cursor movil es visible y sin flash
     * El cursor fijo no es visible y con flash
     **********************************************/    
    tablero->curMovil.casilla = tablero->curFijo.casilla = idToPtr(d2);
    tablero->curMovil.visible = TRUE;
    tablero->curFijo.visible  = FALSE;
    tablero->curMovil.flash   = FALSE;
    tablero->curFijo.flash    = TRUE;

    /* No hay ninguna casilla seleccionada */
    estadoJuego.casillaSeleccionada = NO_SELECCION;     
}
/****************************************************************************************
 *  ejecutaPartida
 *
 * Bucle principal, se ejecuta hasta que termina la partida.
 * Devuelve TRUE si el juego ha terminado.
 ***************************************************************************************/
AJD_Bool ejecutaPartida()
{    
    AJD_Accion accion = NO_ACCION;    
    AJD_TableroPtr ptablero = obtenTableroPtr();

    while (accion != SALIR)
    {     
        accion = procesaTeclado (ptablero, &estadoJuego);

        dibujaJuego(ptablero, &estadoJuego);

        if (accion == NO_ACCION) continue;
        
        actualizaCursor (accion);

        /* Si se ha seleccionado origen y destino intenta efectuar el movimiento */
        estadoJuego.casillaSeleccionada = obtenSeleccion (&estadoJuego, accion);
        if (estadoJuego.casillaSeleccionada == DESTINO_SELECCIONADO)
        {
            if (efectuaJugada (ptablero))
            {
                dibujaFlags (&estadoJuego);

                siguienteTurno(ptablero);
            }
            else
            /* El movimiento no se pudo efectuar por ser invalido, se debe seleccionar
               otra casilla destino.                                                  */
            {
                estadoJuego.casillaSeleccionada = ORIGEN_SELECCIONADO;
            }
            
        }        
    }
    
    return (accion == SALIR);
}
/****************************************************************************************
 *  siguienteTurno
 *
 *  Actualiza estado de juego para el próximo turno
 ***************************************************************************************/
void siguienteTurno(AJD_TableroPtr tablero)
{
    estadoJuego.jueganBlancas ^= 1;
    estadoJuego.jugadorActual ^= 1;        

    if (estadoJuego.jueganBlancas) 
    {
        /* Blancas y negras han terminado sus movimientos */
        estadoJuego.jugada.nturno += 1;
        /* Resetea el cursor de selección al peon de dama blanco */
        tablero->curMovil.casilla = idToPtr (d2);
    }
    else
        /* Resetea el cursor de selección al peon de dama negro */
        tablero->curMovil.casilla = idToPtr (d7);

    /* Oculta el cursor fijo */
    tablero->curFijo.visible = FALSE;

    /* Nuevo turno, no hay casilla origen/destino seleccionada */
    estadoJuego.casillaSeleccionada = NO_SELECCION;        
}
/****************************************************************************************
 *  efectuaJugada
 *
 *  Efectua movimiento de piezas blancas / negras si es válido.
 *  Devuelve TRUE si se efectuó el movimiento, FALSE en caso contrario.
 ***************************************************************************************/
AJD_Bool efectuaJugada (AJD_TableroPtr tablero)
{
    AJD_MovimientoPtr movimiento;
    AJD_Flags         flags;

    if (estadoJuego.jueganBlancas)
    {
        movimiento = &estadoJuego.jugada.movBlancas;
    }
    else
    {
        movimiento = &estadoJuego.jugada.movNegras;
    }

    if (esMovimientoValido (movimiento, &estadoJuego, &flags))
    {
        /* El movimiento es válido, por tanto, los flags alterados también.  */
        movimiento->flags = flags;
        /* Primero hay que añadir el movimiento al log. Si lo hacemos despues de
         * mover la ficha, las piezas en el tablero han cambiado y la información
         * es incorrecta.
         */
        appendMovimiento (movimiento);
        efectuaMovimiento (movimiento);    

        /* TODO: mostrar movimientos --> interfaz.c */
        sprintPartida(buff);
        mvprintw (40,0, "%s", buff);
        return TRUE;
    }
    else
        return FALSE;
}
/****************************************************************************************
 * obtenEstadoPtr
 *
 * Obten el puntero al estado de juego
 ***************************************************************************************/
AJD_EstadoPtr obtenEstadoPtr ()
{
    return &estadoJuego;
}
