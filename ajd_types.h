#pragma once

#ifdef TRUE
	#undef TRUE
	#undef FALSE
#endif

typedef enum { FALSE, TRUE } AJD_Bool;
typedef enum { NEGRO, BLANCO } AJD_Color;
typedef enum { ER_NONE, ER_LARGO, ER_CORTO } AJD_ERTipo;
typedef enum { 
    NO_PIEZA,
    PEON_B, TORRE_B, CABALLO_B, ALFIL_B, DAMA_B, REY_B,
    PEON_N, TORRE_N, CABALLO_N, ALFIL_N, DAMA_N, REY_N

} AJD_Pieza;
typedef uint8_t AJD_idCasilla;

typedef struct {
	AJD_Color     color;
	AJD_Pieza	  pieza;
} AJD_Casilla, *AJD_CasillaPtr;

typedef struct {
    AJD_Casilla casillas[8*8];    
} AJD_Tablero, *AJD_TableroPtr;

typedef struct {
    AJD_idCasilla   idOrigen;
    AJD_idCasilla   idDestino;
    AJD_Bool        come_pieza : 1;
    AJD_Bool        jaque : 1;
    AJD_ERTipo      enroque : 1;
    AJD_Bool        mate : 1;
} AJD_Turno, *AJD_TurnoPtr;
