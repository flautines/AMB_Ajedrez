#include <stdint.h>
#include <ajd_types.h>
#include <tablero.h>
#include <interfaz.h>
#include <juego.h>

int main()
{
	inicializa();

	tableroDisposicionInicial();
	dibujaTablero(obtenTableroPtr());
}
