#pragma once
#include <stdint.h>
#include <stdio.h>
#include "snek.h"

/** Elementos posibles que pueden haber en el tablero */
typedef enum celltype
{
  EMPTY = 0,
  WALL = 1,
  SPIKE = 2
} CellType;

/** Celda de la matriz */
typedef struct cell
{
  /** Tipo de celda */
  CellType type;
  /** ALtura de la celda (distancia a la espina o suelo) */
  uint8_t height;
  /** Tipo de celda a la que cae */
  CellType fall_type;
  /** Numero aleatorio que se creara para la tabla de hash */
  uint64_t random;
} Cell;

typedef struct hash
{
  /** valor de hash del objeto */
  uint64_t valor;
} Hash;

typedef struct nodo
{
  // snek actual desde donde me voya a mover a nueva pos dada por seg
  Snek* snek;

  int num;
  // a donde me voy a mover(up, down, .....)
  Direction dir;
  int valor;
}Nodo;


typedef struct lista
{
  // lugares por visitar
  Nodo* nodos;
  int cabeza;
  int cola;
}Lista;



/** Estructura del tablero */
typedef struct board
{
  /** Snek del puzle */
  Snek* snek;
  /** Alto del tablero */
  uint8_t height;
  /** Ancho del tablero */
  uint8_t width;
  /** Matriz de celdas del tablero de height x width */
  Cell** cells;
  /** Fila del portal */
  uint8_t portal_row;
  /** Columna del portal */
  uint8_t portal_col;
  // valor de hash actual del tablero
  Hash* actual;
  // matriz de estados de hash ya visitados
  Hash** matriz;


} Board;

////////////////////////////////////////////////////////////////////////////////
//                                Funciones                                   //
////////////////////////////////////////////////////////////////////////////////

/** Inicializa el tablero a partir del archivo */
Board* board_init(FILE* file);

/** Destruye el tablero liberando todo */
void board_destroy(Board* board);

/** Copia el snek y hace el movimiento, luego retorna el nuevo snek. Si el
movimiento no es valido o mata a la serpiente, retorno NULL */
Snek* board_move(Board* board, Direction dir);

/** Retorna True si en el estado del tablero se resolvio el puzzle */
bool board_is_win(Board* board);
