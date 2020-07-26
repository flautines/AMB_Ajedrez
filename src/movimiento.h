#pragma once

/** id casillas involucrados en un enroque
  **************************************************************************************/
#define INICIO_REY_BLANCO        e1
#define EL_ORIGEN_TORRE_BLANCA   a1
#define EL_DESTINO_TORRE_BLANCA  d1
#define EL_DESTINO_REY_BLANCO    c1

#define EC_ORIGEN_TORRE_BLANCA   h1
#define EC_DESTINO_TORRE_BLANCA  f1
#define EC_DESTINO_REY_BLANCO    g1

#define INICIO_REY_NEGRO         e8
#define EL_ORIGEN_TORRE_NEGRA    a8
#define EL_DESTINO_TORRE_NEGRA   d8
#define EL_DESTINO_REY_NEGRO     c8

#define EC_ORIGEN_TORRE_NEGRA    h8
#define EC_DESTINO_TORRE_NEGRA   f8
#define EC_DESTINO_REY_NEGRO     g8

/**Estructura para saber el desplazamiento en X,Y de un movimiento de pieza
  **************************************************************************************/
typedef struct {
    int dx;
    int dy;
} AJD_MovInfo;

void efectuaMovimiento (AJD_MovimientoPtr movimiento);

AJD_Bool esMovimientoValido (AJD_MovimientoPtr movimiento, AJD_Color colorPieza,
                             AJD_FlagsPtr flags);
