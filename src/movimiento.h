#pragma once

/**Estructura para saber el desplazamiento en X,Y de un movimiento de pieza
  **************************************************************************************/
typedef struct {
    int dx;
    int dy;
} AJD_MovInfo;

void efectuaMovimiento (AJD_MovimientoPtr movimiento);

AJD_Bool esMovimientoValido (AJD_MovimientoPtr movimiento, AJD_EstadoPtr estadoJuego);

AJD_Bool cumpleReglasMovimiento (AJD_MovimientoPtr movimiento);
