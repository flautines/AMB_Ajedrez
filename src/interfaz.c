#include <ncurses.h>
#include <tablero.h>
#include <interfaz.h>

#define TABLERO_ROW_START	10
#define TABLERO_COL_START	10
#define MARCADOR_ROW_START	TABLERO_ROW_START
#define MARCADOR_COL_START	TABLERO_COL_START + 8 * 3 + 2
#define MARCADOR_LAST_ROW	TABLERO_ROW_START + 8 * 3 - 1
////////////////////////////////////////////////////////////////////////////
// _dibujaCasilla
//
// Dibuja una casilla del tablero con su pieza si la tuviera.
//
// ...###
// .p.###
// ...###
//
void _dibujaCasilla(AJD_TableroPtr tablero, int ncasilla, int posx, int posy)
{
	// columna y fila correspondientes a esta casilla en el tablero
	int nrow = ncasilla / 8;
	int ncol = ncasilla & 7;

	// Puntero a la casilla a dibujar
	AJD_CasillaPtr casilla = &(tablero->casilla[nrow * 8 + ncol]);

	// Color principal/fondo a usar dependiendo de la casilla
	int color = casilla->color == BLANCO ? 1 : 2;	
	attron(COLOR_PAIR (color));

	for (nrow = 0; nrow < ALTO_CASILLA; nrow++)
	{		
		move (posy+nrow, posx);
		printw("   ");	// 3 espacios

		// Dibuja pieza si es que hay alguna en la casilla actual
		if (casilla->pieza)
		{			
			dibujaPieza (posy+1, posx+1, casilla->pieza, casilla->color_pieza);
		}

		attron(COLOR_PAIR (color));
	}
}
////////////////////////////////////////////////////////////////////////////
// inicializaPantalla
//
// Inicializa libreria ncurses y la interfaz de usuario en general.
//
void inicializaPantalla()
{
	// Inicializa ncurses
	initscr();

	// Habilita teclas especiales (F1, cursores, ESC, etc.)
	keypad (stdscr, 1);

	// Envío de teclas inmediato (sin pulsar ENTER)
	cbreak();

	// No mostrar caracteres pulsados
	noecho();

	// No espera en lectura de caracter
	nodelay(stdscr, 1);

	// Ocultar cursor
	curs_set(0);

	// Inicializa colores 
	if ( has_colors() )
	{
		start_color();

		// Negro sobre fondo blanco para dibujar casilla blanca
		init_pair(1, COLOR_BLACK, COLOR_WHITE);

		// Blanco sobre negro para dibujar casilla negra y textos
		init_pair(2, COLOR_WHITE, COLOR_BLACK);

		// Amarillo sobre negro para los menus
		init_pair(3, COLOR_YELLOW, COLOR_BLACK);

		attron(COLOR_PAIR(1));
	}
	//printw("Ncurses Inicializado");
}
////////////////////////////////////////////////////////////////////////////
// liberaPantalla
//
// Libera los recursos de ncurses.
//
void liberaPantalla()
{
	endwin();
}
////////////////////////////////////////////////////////////////////////////
// dibujaTablero
//
// Dibuja el tablero en pantalla según su estado actual.
//
void dibujaTablero(AJD_TableroPtr tablero)
{
	int row, col, y, x;
	clear();

	int idCasilla = 0;
	for (row=0; row < 8; row++)
	for (col=0; col < 8; col++)
	{
		_dibujaCasilla (tablero, idCasilla, 
			TABLERO_COL_START + col * ANCHO_CASILLA,
			TABLERO_ROW_START + row * ALTO_CASILLA);

		// Siguiente casilla
		idCasilla++;
	}

	// Dibuja los numeros de las filas
	attron ( COLOR_PAIR (2) );
	row = 8;
	y = TABLERO_ROW_START + 1;
	x = TABLERO_COL_START-2;
	while (row)
	{
		move (y, x);
		printw("%d", row);
		y += 3;
		row--;
	}

	// Dibuja letras de columnas
	col = 8;
	x = TABLERO_COL_START + 8 * 3 - 2;
	y = TABLERO_ROW_START - 2;
	while (col)
	{
		move (y, x);
		printw ("%c", col+'a'-1);
		x -= 3;
		col--;
	}
}
////////////////////////////////////////////////////////////////////////////
// dibujaPieza Dibuja una pieza del ajedrez en la posición indicada
//
void dibujaPieza (int posy, int posx, AJD_Pieza pieza, AJD_Color color)
{
	// Array de caracteres representado la letra a dibujar de cada pieza
	char *sprPiezas=" PTCADR";
	char sprite = sprPiezas[pieza];

	// Piezas negras las mostraremos en minusculas
	if (color == NEGRO) sprite += 32;
	if (color == BLANCO) color = 1;

	// Dibuja la letra de la pieza en color blanco sobre fondo negro
	attron (COLOR_PAIR (1));
	move (posy, posx);
	printw("%c", sprite);
}
////////////////////////////////////////////////////////////////////////////
// dibujaMarcadores Dibuja los marcadores de tiempo, turno, etc.
//
void dibujaMarcadores(uint16_t turno, AJD_Estado* estado)
{
	int x,y;

	x = MARCADOR_COL_START;
	y = MARCADOR_ROW_START + 1;
	attron (COLOR_PAIR (2));
	move (y,x);
	printw ("AJEDREZ 1.0");

	y += 1;
	move (y,x);
	printw ("(c) Jun 2020 Andres Mata");

	y += 4;
	move (y,x);
	printw ("Blancas 00:00");

	y += 2;
	move (y,x);
	printw ("Negras 00:00");

	y = MARCADOR_LAST_ROW - 1;
	move (y,x);
	printw ("Turno %02d (Jugador %c)", turno, estado->turno_jugador+'1');

	y += 1;
	move (y,x);
	printw ("Juegan %s", estado->juegan_blancas ? "blancas" : "negras");
}
////////////////////////////////////////////////////////////////////////////
// dibujaMenu Dibuja un menu con su titulo y sus elementos.
//
void dibujaMenu(int y, int x, menu_t* menu)
{
	int nitems = menu->nitems;
	int item_id = 1;

	// Menu Title
	attron ( COLOR_PAIR (3) );	
	move (y, x);
	printw (menu->items[0].menuString);

	while (nitems--)
	{
		y += 2;
		menuItem_t* menu_item = &(menu->items[item_id]);

		move (y,x);
		attron (COLOR_PAIR (2) );

		// Resaltar la opcion actual del menu
		if (item_id == menu->selected)
			attron (COLOR_PAIR (1) );

		printw (menu_item->menuString);

		item_id++;
	}

	y += 3;
	attron (COLOR_PAIR (2));
	move (y,x);
	printw ("Cursores selecciona opcion, ENTER confirmar");
}

int muestraMenu (int x, int y, menu_t* menu)
{	
	int ch;

	clear();
	do
	{
		dibujaMenu (x, y, menu);
		ch = getch();
		if (ch == KEY_UP) 	menu->selected--;
		if (ch == KEY_DOWN)	menu->selected++;

		if (menu->selected < 1) menu->selected = menu->nitems;
		if (menu->selected > menu->nitems) menu->selected = 1;
	} while (ch != '\n');

	return true;
}

int obtenJugada (int* celda_origen, int* celda_destino)
{
	// columna/fila
	int columna, fila, celda;
	int ch;

	int y = MARCADOR_ROW_START + 11;
	move (y, MARCADOR_COL_START);
	printw ("Instroduzca casilla ORIGEN");
	move (y+=1, MARCADOR_COL_START);
	printw ("Columna:");
	move (y+=1, MARCADOR_COL_START);	
	printw ("Fila:");

	// Activar que se muestren caracteres tecleados
	curs_set (1);
	echo ();
	nodelay(stdscr, 0);

	move (MARCADOR_ROW_START + 12, MARCADOR_COL_START + 9);
	ch = getch();
	columna = ch - 'a';

	move (MARCADOR_ROW_START + 13, MARCADOR_COL_START + 6);
	ch = getch();
	fila = 8 - 	ch + '0';
	*celda_origen = 8 * fila + columna;

	move (0,0);
	printw ("Celda Origen %d", *celda_origen);

	move (y+=2, MARCADOR_COL_START);
	printw ("Instroduzca casilla DESTINO");
	move (y+=1, MARCADOR_COL_START);
	printw ("Columna:");
	move (y+=1, MARCADOR_COL_START);	
	printw ("Fila:");

	move (MARCADOR_ROW_START + 16, MARCADOR_COL_START + 10);
	
	ch = getch();
	columna = ch - 'a';

	move (MARCADOR_ROW_START + 17, MARCADOR_COL_START + 6);
	ch = getch();
	fila = 8 - ch + '0';
	*celda_destino = 8 * fila + columna;

	move (1,0);
	printw ("Celda Destino %d", *celda_destino);

	ch = getch();
}