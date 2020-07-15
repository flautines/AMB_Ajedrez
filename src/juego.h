#pragma once

typedef struct {
    AJD_Jugada          jugada;
    AJD_Bool            jueganBlancas : 1;
    uint8_t             jugadorActual : 1;
    /*AJD_ListaJugadas    partida */
    uint16_t            segundosBlancas;
    uint16_t            segundosNegras;
    /*AJD_ERPerm          enroquePermitido;
    AJD_Seleccion       casillaSeleccionada;*/
    AJD_Bool            finJuego;
} AJD_Estado, *AJD_EstadoPtr;

void inicializa();
void nuevoJuego();
