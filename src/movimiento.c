#include <common.h>
#include <utils.h>

#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// INTERFAZ PRIVADA
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// Variable privada con informacion adicional del movimiento
AJD_MovInfo movInfo;
/////////////////////////////////////////////////////////////////////////////////////////
void     obtenMovInfo (AJD_TurnoPtr);
AJD_Bool cumpleReglasMovimiento (AJD_TableroPtr, AJD_EstadoPtr);
AJD_Bool cumpleReglasComerPeon ();
AJD_Bool cumpleReglasMovimientoPeon (AJD_TableroPtr);
AJD_Bool cumpleReglasMovimientoCaballo ();
AJD_Bool cumpleReglasMovimientoRey (AJD_TableroPtr, AJD_EstadoPtr);
AJD_Bool seMueveEnVertHorz (AJD_TableroPtr);
AJD_Bool seMueveEnDiagonal (AJD_TableroPtr);
AJD_Bool caminoLibre (AJD_TableroPtr, AJD_CasillaPtr, AJD_CasillaPtr);
void     actualizaOpcionesDeEnroque (AJD_Casilla, AJD_EstadoPtr);
/////////////////////////////////////////////////////////////////////////////////////////
// obtenMovInfo
//
//  Obtiene detalles necesarios para realizar comprobaciones posteriores
// respecto las casillas origen y destino de un movimiento.
//
void obtenMovInfo (AJD_TurnoPtr turno)
{
    AJD_CasillaPtr origen, destino;
    int dir;
    
    origen  = &(turno->origen);
    destino = &(turno->destino);
    // Si el color de la pieza es BLANCO se mueven 1 hacia arriba (-1), si no,
    // se mueven 1 hacia abajo (+1)
    dir = origen->color_pieza ? -1 : 1;
    
    movInfo.origen  = origen;
    movInfo.destino = destino;    
    movInfo.srcY    = origen->id /8;
    movInfo.srcX    = origen->id &7;
    movInfo.dstY    = destino->id /8;
    movInfo.dstX    = destino->id &7;    
    movInfo.dy      = movInfo.dstY - movInfo.srcY;
    movInfo.dx      = movInfo.dstX - movInfo.srcX;
    // Multiplicando dy * dir nos da la "distancia" en vertical.
    // Realmente no se trata de distancia en el sentido matemático, ya que
    // tiene signo, dependiendo de si la pieza se ha movido hacia adelante (positivo)
    // o hacia atrás (negativo)
    movInfo.distY = movInfo.dy * dir;
    // Distancia en horizontal (en el sentido matematico, 
    // (siempre positivo, nunca negativo) del movimiento
    movInfo.distX = abs (movInfo.dx);
}
/////////////////////////////////////////////////////////////////////////////////////////
// cumpleReglasMovimiento
//
AJD_Bool cumpleReglasMovimiento (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego)
{
    AJD_Pieza pieza = movInfo.origen->pieza;

    switch (pieza)
    {
        case PEON:
            return cumpleReglasMovimientoPeon (tablero);

        case CABALLO:
            return cumpleReglasMovimientoCaballo ();

        case ALFIL:
            return seMueveEnDiagonal (tablero);

        case TORRE:        
            return seMueveEnVertHorz (tablero);

        case DAMA:
            return seMueveEnDiagonal (tablero)
                || seMueveEnVertHorz (tablero);

        case REY:
        {
            return cumpleReglasMovimientoRey (tablero, estado_juego);
        }
        default:
            return 0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
// cumpleReglasComerPeon
//
AJD_Bool cumpleReglasComerPeon ()
{
    return movInfo.distX == 1 && movInfo.distY == 1;
}
/////////////////////////////////////////////////////////////////////////////////////////
// cumpleReglasMovimientoPeon
//
AJD_Bool cumpleReglasMovimientoPeon (AJD_TableroPtr tablero)
{
    AJD_Color colorPiezaOrigen = movInfo.origen->color_pieza;
    int distY = movInfo.distY;
    int distX = movInfo.distX;
    int filaInicioPeones = colorPiezaOrigen ? 6 : 1;    

    if (distX == 0)
    {
        // Movimiento vertical de dos casillas, sólo es válido si 
        // es 1er movimiento o lo que es lo mismo, si se parte de 
        // las filas de inicio de PEON.
        return (distY == 2 && movInfo.srcY == filaInicioPeones 
                && caminoLibre (tablero, movInfo.origen, movInfo.destino))
        // Movimiento vertical de una casilla
            || distY == 1;
    }
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
// seMueveEnVertHorz
//
AJD_Bool seMueveEnVertHorz (AJD_TableroPtr tablero)
{         
    return (movInfo.dy == 0 || movInfo.dx == 0) 
        && caminoLibre (tablero, movInfo.origen, movInfo.destino);
}
/////////////////////////////////////////////////////////////////////////////////////////
// seMueveEnDiagonal
//
AJD_Bool seMueveEnDiagonal (AJD_TableroPtr tablero)
{
    return (abs(movInfo.dy) == abs(movInfo.dx))
        && caminoLibre (tablero, movInfo.origen, movInfo.destino);
}
/////////////////////////////////////////////////////////////////////////////////////////
// cumpleReglasMovimientoCaballo
//
AJD_Bool cumpleReglasMovimientoCaballo ()
{
    // La "distancia" movInfo.distY es un tanto especial, ya que tiene
    // signo para poder saber si la pieza se desplaza hacia adelante o atrás.
    // Necesitamos el valor absoluto para obtener la distancia en el sentido
    // matemático.
    int distY = abs(movInfo.distY);
    int distX = movInfo.distX;

    // Movmiento en "L"
    return (distY == 2 && distX == 1) || (distX == 2 && distY == 1);
}
/////////////////////////////////////////////////////////////////////////////////////////
// cumpleReglasMovimientoRey
//
AJD_Bool cumpleReglasMovimientoRey (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego)
{
    int dx = movInfo.dx;
    int dy = abs (movInfo.dy);

    // Intenta efectuar un enroque?
    if (abs(dx)==2 && dy == 0) 
    {
        if (dx < 0) 
        {            
            if (puedeEnrocar (tablero, estado_juego, ENROQUE_LARGO))
            {
                estado_juego->turno_actual.enroque = ENROQUE_LARGO;
                return 1;
            }

        }
        else if (dx > 0) 
        {
            if (puedeEnrocar (tablero, estado_juego, ENROQUE_CORTO))
            {
                estado_juego->turno_actual.enroque = ENROQUE_CORTO;
                return 1;
            }
        }
    }
    // Movimiento normal
    dx = abs(dx);
    return (dx==1 && dy==1) 
        ||  (dx==0 && dy==1)
        ||  (dx==1 && dy==0);
}
/////////////////////////////////////////////////////////////////////////////////////////
// caminoLibre
//
AJD_Bool caminoLibre (AJD_TableroPtr tablero, AJD_CasillaPtr origen, AJD_CasillaPtr destino)
{
    int dx, dy;    
    AJD_idCasilla idOrigen  = origen->id;
    AJD_idCasilla idDestino = destino->id;    
    dx = dy = 0;

    dx = sign (movInfo.dx);
    dy = sign (movInfo.dy) * 8;

    for (AJD_idCasilla idCasilla = idOrigen+dy+dx; idCasilla != idDestino; idCasilla += dy+dx)
    {
        AJD_CasillaPtr casilla = &tablero->casilla[idCasilla];
        if (casillaOcupada (casilla)) 
            return 0;
    }
    return 1;
}
////////////////////////////////////////////////////////////////////////////
// actualizaOpcionesDeEnroque
//
void actualizaOpcionesDeEnroque (AJD_Casilla origen, AJD_EstadoPtr estado_juego)
{    
    // Si se mueve el rey queda prohibido cualquier enroque
    if (origen.pieza == REY)
    {
        if (estado_juego->turno_actual.juegan_blancas)
        {
            estado_juego->enroque_permitido |= ERLARGO_PROHIBIDO_B | ERCORTO_PROHIBIDO_B;
        }
        else
        {
            estado_juego->enroque_permitido |= ERLARGO_PROHIBIDO_N | ERCORTO_PROHIBIDO_N;
        }
    }
    // Si se mueve una torre se prohibe el enroque corto o largo dependiendo 
    // de la torre movida
    else if (origen.pieza == TORRE)
    {
        switch (origen.id)
        {
            case a1: estado_juego->enroque_permitido |= ERLARGO_PROHIBIDO_B;break;
            case a8: estado_juego->enroque_permitido |= ERLARGO_PROHIBIDO_N;break;
            case h1: estado_juego->enroque_permitido |= ERCORTO_PROHIBIDO_B;break;
            case h8: estado_juego->enroque_permitido |= ERCORTO_PROHIBIDO_N;break;
            default: break;
        }
        
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// INTERFAZ PUBLICA
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// esMovimientoValido
//
AJD_Bool esMovimientoValido (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego)
{
    obtenMovInfo (&(estado_juego->turno_actual));
    muestraMovInfo (&movInfo);
    
    // Primero que cumpla las reglas de movimiento especifico de la pieza.
    // Si lo cumple, mira si la casilla destino está ocupada.
    // Si la pieza ocupada es del mismo "bando", el movimiento no es valido 
    // Si la casilla es del mismo color, come la pieza. 
    // Si la casilla está libre, es un movimiento valido.
    // El PEON es la única pieza que come de forma diferente, así que se 
    // comprueba en un caso aparte si se detecta movimiento de pieza no válido.
    if (cumpleReglasMovimiento (tablero, estado_juego))
    {
        if (casillaOcupada (movInfo.destino))
        {               
            if (movInfo.origen->pieza != PEON)
                return  (movInfo.destino->color_pieza != movInfo.origen->color_pieza);
        }           
        else            
            return 1;
    }    
    if (movInfo.origen->pieza == PEON)
        return cumpleReglasComerPeon() && casillaOcupada (movInfo.destino);
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
// puedeEnrocar
//
AJD_Bool puedeEnrocar (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego, AJD_Enroque enroque)
{
    AJD_CasillaPtr destino, casillaRey;
    AJD_Bool juegan_blancas;

    juegan_blancas = estado_juego->turno_actual.juegan_blancas;
    destino = casillaRey = NULL;
    // El enroque sólo es admisible si todos cumplen las siguientes condiciones:
    //
    // Ninguna de las piezas que intervienen en el enroque puede haber sido movido previamente.
    // No debe haber ninguna pieza entre el rey y la torre;
    // El rey no puede estar en jaque, ni tampoco podrá pasar a través de casillas que están 
    // bajo ataque por parte de las piezas enemigas. Al igual que con cualquier movimiento, 
    // el enroque es ilegal si pusiera al rey en jaque.    
    casillaRey = juegan_blancas ? 
        &(tablero->casilla[INICIO_REY_BLANCO]) :
        &(tablero->casilla[INICIO_REY_NEGRO]);

    switch (enroque)
    {
        case ENROQUE_LARGO:
            if (juegan_blancas)
                if (estado_juego->enroque_permitido&ERLARGO_PROHIBIDO_B) return 0;
                else destino = &(tablero->casilla[EL_ORIGEN_TORRE_BLANCA]);
            else
                if (estado_juego->enroque_permitido&ERLARGO_PROHIBIDO_N) return 0;
                else destino = &(tablero->casilla[EL_ORIGEN_TORRE_NEGRA]);
            break;
            
        case ENROQUE_CORTO:
            if (juegan_blancas)
                if (estado_juego->enroque_permitido&ERCORTO_PROHIBIDO_B) return 0;
                else destino = &(tablero->casilla[EC_ORIGEN_TORRE_BLANCA]);
            else
                if (estado_juego->enroque_permitido&ERCORTO_PROHIBIDO_N) return 0;
                else destino = &(tablero->casilla[EC_ORIGEN_TORRE_NEGRA]);
            break;

        default:
            break;
    }
    return caminoLibre (tablero, casillaRey, destino);
}
/////////////////////////////////////////////////////////////////////////////////////////
// muevePieza
//
void muevePieza (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego)
{
    AJD_TurnoPtr turno = &estado_juego->turno_actual;
    // Si el movimiento es de alguna de las dos TORRES o el REY, invalidar la opción
    // de enroque correspondiente.
    actualizaOpcionesDeEnroque (estado_juego->turno_actual.origen, estado_juego);

    // Actualiza el turno actual con el movimiento realizado
    turno->destino.pieza        = turno->origen.pieza;
    turno->destino.color_pieza  = turno->origen.color_pieza;
    
    // Actualiza el tablero
    tablero->casilla[turno->origen.id].pieza = NONE;
    tablero->casilla[turno->destino.id].pieza = turno->destino.pieza;
    tablero->casilla[turno->destino.id].color_pieza = turno->destino.color_pieza;
}
////////////////////////////////////////////////////////////////////////////
// promocionaPeon
//
//  Promociona el peon en la casilla indicada
//
void promocionaPeon (AJD_TableroPtr tablero, AJD_TurnoPtr turno)
{
    AJD_Pieza promocion_seleccionada = DAMA;
    mvprintw (3,63, "Promocionando PEON      ");
    // TODO: De momento siempre promociona a DAMA, queda pendiente
    //       poder seleccionar (con cursores p.ej.) el tipo de pieza

    // Registramos la promoción en 'turno' para luego añadirlo al log
    // de la partida.
    turno->destino.pieza = promocion_seleccionada;
    turno->promocion     = TRUE;

    // Actualizamos el tablero con la nueva promoción
    tablero->casilla[turno->destino.id].pieza = promocion_seleccionada;
}
////////////////////////////////////////////////////////////////////////////
// peonUltimafila
//
//  Comprueba si un peon ha efectuado un movimiento a la ultima fila
//
AJD_Bool peonUltimaFila (AJD_TableroPtr tablero, AJD_TurnoPtr turno)
{
    AJD_idCasilla limites[2][2] = { 
    // 1a y ultima casilla fila promocion NEGRAS
        {a1, h1},
    // 1a y ultima casilla fila promocion BLANCAS
        {a8, h8}
    };
    AJD_idCasilla idCasilla;
    AJD_Bool juegan_blancas;    

    // Si no es un peon no hagas más nada
    if (turno->destino.pieza != PEON) return 0;


    idCasilla = turno->destino.id;    
    juegan_blancas = turno->juegan_blancas;

    mvprintw (3,63, "[%d] in [%d]..[%d]?     ", 
              idCasilla, 
              limites[juegan_blancas][0],
              limites[juegan_blancas][1]);

    if (idCasilla >= limites[juegan_blancas][0] 
        &&  idCasilla <= limites[juegan_blancas][1])
    {
        mvprintw (3,80, "SI!!");
        return 1;
    }
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
// efectuaEnroque
//
void efectuaEnroque (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego)
{
    AJD_idCasilla idOrigen, idDestino;
    AJD_TurnoPtr turno = &estado_juego->turno_actual;

    mvprintw (3, 62, "ENROCANDO!!!");
    // Mueve primero el rey
    muevePieza (tablero, estado_juego);

    idOrigen = idDestino = 0;
    // Selecciona la torre origen/destino correspondiente al enroque
    if (turno->juegan_blancas)
    {
        if (turno->enroque == ENROQUE_LARGO)
        {
            idOrigen = EL_ORIGEN_TORRE_BLANCA;
            idDestino = EL_DESTINO_TORRE_BLANCA;
        }
        else if (turno->enroque == ENROQUE_CORTO)
        {
            idOrigen = EC_ORIGEN_TORRE_BLANCA;
            idDestino = EC_DESTINO_TORRE_BLANCA;
        }
    }
    else
    {
        if (turno->enroque == ENROQUE_LARGO)
        {
            idOrigen = EL_ORIGEN_TORRE_NEGRA;
            idDestino = EL_DESTINO_TORRE_NEGRA;
        }
        else if (turno->enroque == ENROQUE_CORTO)
        {
            idOrigen = EC_ORIGEN_TORRE_NEGRA;
            idDestino = EC_DESTINO_TORRE_NEGRA;
        }
    }
    // Actualiza la información de las casillas involucradas en el enroque...
    turno->origen  = tablero->casilla[idOrigen];
    turno->destino = tablero->casilla[idDestino];
    
    // ... y realiza su movimiento correspondiente    
    muevePieza (tablero, estado_juego);
}
/////////////////////////////////////////////////////////////////////////////////////////
// casillaOcupada
//
AJD_Bool casillaOcupada (AJD_CasillaPtr casilla)
{
    return casilla->pieza != NONE;
}
/////////////////////////////////////////////////////////////////////////////////////////
// hayPiezaValida
//
AJD_Bool hayPiezaValida (AJD_TableroPtr tablero, AJD_CasillaPtr casilla, AJD_TurnoPtr turno)
{
    AJD_Color color_jugador = turno->juegan_blancas ? BLANCO : NEGRO;
    return casillaOcupada (casilla)
        && casilla->color_pieza == color_jugador;
}
/////////////////////////////////////////////////////////////////////////////////////////
