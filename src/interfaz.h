#pragma once

#include "tablero.h"
#include "juego.h"
#include "menu.h"

#define ANCHO_CASILLA 3
#define ALTO_CASILLA  3

void inicializaPantalla();
void liberaPantalla();
void dibujaTablero(AJD_TableroPtr tablero);
void dibujaPieza (int posy, int posx, AJD_Pieza pieza, AJD_Color color);
void dibujaMarcadores(uint16_t turno, AJD_Estado* estado);
void dibujaMenu (int y, int x, menu_t* menu);
int muestraMenu (int x, int y, menu_t* menu);
int obtenJugada (int* celda_origen, int* celda_destino);