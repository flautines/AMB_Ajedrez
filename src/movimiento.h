#pragma once

void efectuaMovimiento (AJD_MovimientoPtr movimiento);

AJD_Bool esMovimientoValido (AJD_MovimientoPtr movimiento, AJD_EstadoPtr estadoJuego);

AJD_Bool cumpleReglasMovimiento (AJD_MovimientoPtr movimiento);
