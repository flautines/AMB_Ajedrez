#include <menu.h>
#include <interfaz.h>

/////////////////////////////////////////////////////////////////////
// strings y posicion del menu eleccion del tipo de jugador
//
menu_t menuJugadores = 
{	3, 	// 3 items in the menu (title does not count)
	1,	// First item selected
	{
		{ "  Tipo de jugadores"	 },
		{ "     CPU vs HUMANO   " },
		{ "  HUMANO vs HUMANO   " }, 
		{ "     CPU vs CPU      " },
	},
};


/////////////////////////////////////////////////////////////////////
// menuJuego Elección de tipo de jugadores, humano vs cpu y color
//
void menuJuego()
{
	dibujaMenu (5, 7, &menuJugadores);
}