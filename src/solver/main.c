#include "../snekbirb/board.h"
#include "../watcher/watcher.h"
#include "../random/pcg_basic.h"

int main(int argc, char *argv[])
{
  // Revisar el input
  if (argc != 3)
  {
    printf("Modo de uso: ./snakebirb test.txt output.txt\n");
    return 0;
  }

  /*
  El main actualmente usa las funciones de crear y liberar tablero y se aplican
  movimientos según lo que se escriba por consola. También se abre la interfaz
  gráfica para mostrar su uso. Este código no tiene leaks ni errores y solamente
  sirve para mostrara la forma en que se usan las funciones.
  La modelación del programa ya está dada para que no tengan que modelar los
  movimientos. Sin embargo son libres de agregar o eliminar lo que quieran en
  los archivos de la carpeta snekbirb. Por ejemplo está recomendado almacenar
  una matriz de números aleatorios junto con el tablero y un valor de hash junto
  con la serpiente.
  La modelación actual está hecha de manera de que cada operación tenga costo
  igual al largo de la serpiente. Si bien se puede mover la serpiente en O(1)
  siempre y cuando no caiga, para la búsqueda es necesario copiar la serpiente.
  Es por esto que cada operación toma O(snek). Para tu función de hash trata de
  que el tiempo en calcularla no tome más que O(snek).
  Tu debes implementar el algoritmo de BFS para encontrar la secuencia de pasos
  mínima que resuelve el problema. Este algoritmo va a necesitar de una tabla de
  hash para alamacenar los estados ya visitados para no repetirlos. Finalmente
  tu código debe escribir en el archivo dado la secuencia de pasos ejecutados
  en el formato especificado en el enunciado.
  La interfaz es solamente para que puedas debuguear tu programa y no tiene
  influencia en la correccion.
  */

  // Ejemplo de generar numero random
  uint64_t r = get_random();
  printf("%lu\n", r);

  // Abro la ventana
  watcher_open(argv[1]);

  // Abro el archivo
  FILE* file = fopen(argv[1], "r");

  // Creo el tablero
  Board* board = board_init(file);

  // Cierro el archivo
  fclose(file);

  // Dibujo el estado inicial
  watcher_draw_bird(board -> snek);

  // Arreglo de movimientos (asumo que no son mas de 100 para la demo pero pueden ser mas)
  Direction movs[100];
  int mov_count = 0;

  // Itero pidiendo movimientos al usuario 0 = UP, 1 = LEFT, 2 = RIGHT, 3 = DOWN
  Direction dir = 0;
  while (dir < 4)
  {
    /*
    Esto es una demo de los movimientos de la serpiente y del modo de uso de la
    funcion board_move. La idea es que si la serpiente se mueve en una direccion
    invalida o si la serpiente muere al hacer el movimiento, se retorna NULL y
    no se modifica el tablero actual.
    Si el movimiento es válido, se retorna una nueva serpiente con la nueva
    posición. La serpiente anterior sigue existiendo.
    Los movimientos hechos se almacenan en el arreglo de movimientos para luego
    escribirlos en el archivo de output.
    */

    // Leo un movimiento de la consola
    scanf("%u", &dir);

    // Almaceno el movimiento en el arreglo
    movs[mov_count] = dir;
    mov_count++;

    // Escribo el movimiento en el archivo

    // Ejecuto el movimiento
    Snek* copy = board_move(board, dir);

    // Si hago un movimiento invalido o la serpiente muere, termino
    if (!copy)
    {
      break;
    }

    // Dibujo el nuevo estado
    watcher_draw_bird(copy);

    // Libero la serpiente inicial
    snek_destroy(board -> snek);

    // Pongo la nueva serpiente en el tablero
    board -> snek = copy;

    // Si gane, termino
    if (board_is_win(board))
    {
      printf("Hurra\n");
      break;
    }
  }

  // Abro el archivo de output
  FILE* out = fopen(argv[2], "w");

  // Escribo el numero de movimientos
  fprintf(out, "%d\n", mov_count);

  // Escribo los movimientos
  for (int i = 0; i < mov_count; i++)
  {
    fprintf(out, "%u\n", movs[i]);
  }

  // Cierro el archivo de output
  fclose(out);

  // Cierro la interfaz
  watcher_close();

  // Libero el pajaro
  snek_destroy(board -> snek);

  // Libero la memoria del tablero
  board_destroy(board);

  // Retorno que todo esta bien
  return 0;
}
