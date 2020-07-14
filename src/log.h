#pragma once

extern char* strCasillas[];

/////////////////////////////////////////////////////////////////////////////////////////
// turnoToAlgebra
//  Convierte la información de un turno a formato Algebraico
//
void turnoToAlgebra (AJD_TurnoPtr turno, char* buff);
/////////////////////////////////////////////////////////////////////////////////////////
// algebraToTurno
//  Convierte información algebraica en format AJD_Turno
//
void algebraToTurno (char* buff, AJD_TurnoPtr turno, AJD_Color color_jugador);
