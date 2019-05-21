#include "../snekbirb/board.h"
#include "../watcher/watcher.h"
#include "../random/pcg_basic.h"
#include <stdlib.h>
#include "../snekbirb/snek.h"
#include <math.h>


int main(int argc, char *argv[])
{
  // Revisar el input
  if (argc != 3)
  {
    //printf("Modo de uso: ./snakebirb test.txt output.txt\n");
    return 0;
  }

  /*
  El main actualmente usa  las funciones de crear y liberar tablero y se aplican
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



  // Abro la ventana
  watcher_open(argv[1]);

  // Abro el archivo
  FILE* file = fopen(argv[1], "r");

  // Creo el tablero
  Board* board = board_init(file);

  // Cierro el archivo
  fclose(file);

  // Inicio las colas visitar y Visitados
  Lista* visitar =  malloc(sizeof(Lista));
  visitar -> nodos = malloc(sizeof(Nodo)*1000000);
  visitar -> cola = 0;
  visitar -> cabeza = 0;





  // Dibujo el estado inicial
  watcher_draw_bird(board -> snek);

  // Arreglo de movimientos (asumo que no son mas de 100 para la demo pero pueden ser mas)
  Direction movs[100];
  int mov_count = 0;

  // Agrego a la lista los primeros puntos por visitar dado el snek Inicializa

  Snek* copy;
  for (int dir =0; dir < 4;dir++)
  {
    // Ejecuto el movimiento
    //printf("el movimiento que hago eso: %d\n",dir );
    copy = board_move(board, dir);
    if(!copy)
    {
      //printf("no valido perro\n");
    }
    else
    {
      for (int i = 0; i <1000000; i++)
      {
        if (visitar->nodos[i].valor == 0)
        {
          visitar->nodos[i].snek = copy;
          visitar->nodos[i].dir = dir;
          visitar->nodos[i].valor = 1;
          visitar->cola += 1;
          break;
        }
      }
      // printf("la cabeza esta en [%d.%d]\n",copy->head->row, copy->head->col);
    }
  }



  // printf("la direccion de la cabeza actual es:[%d.%d]\n",board->snek->head->row, board->snek->head->col );

  // Itero pidiendo movimientos al usuario 0 = UP, 1 = LEFT, 2 = RIGHT, 3 = DOWN
  // printf("---------------------\n");

  // sleep(5);
  while (visitar->cabeza < visitar->cola)

  {

    //asigno nuevo snek como el primero de cola por visitar
    copy = visitar->nodos[visitar->cabeza].snek;

    // muevo la cabeza en 1 para siguente por visitar
    visitar ->cabeza += 1;

    // Dibujo el nuevo estado
    watcher_draw_bird(copy);

    // Libero la serpiente inicial
    snek_destroy(board -> snek);

    // Pongo la nueva serpiente en el tablero
    board -> snek = copy;

    //variables para guardo de hash en matriz de estados visitados
    uint64_t siguiente;
    uint64_t algo;
    uint64_t nexthash;
    uint64_t nuevo;
    uint64_t res;
    uint64_t sum;
    res = board -> snek-> tail -> direction;
    nuevo = board -> snek -> hash;
    sum = board -> snek -> head->next -> direction;
    nuevo -= res;
    nuevo /= 4;
    nuevo += sum*(uint64_t)pow(4, board -> snek ->size -2);

    // El nuevo hash de la srpiente
    board -> snek -> hash = nuevo;

    siguiente = board -> cells[board->snek -> head -> row][board->snek -> head -> col].random;
    nexthash = nuevo ^ siguiente;
    algo = nexthash % 1000000;



    for (int num = 0; num < 100; num++)
    {
      if (board -> matriz[algo][num].valor == 0)
      {
        board -> matriz[algo][num].valor = nexthash;
        break;
      }
      else
      {
        if (board -> matriz[algo][num].valor == nexthash)
        {
          //significa que ya pase por este estado y debo hacer algo
          //printf("por aqui no deberia pasar MAIN\n");

          // return NULL;
        }
      }
    }


    board -> actual -> valor = nexthash;

    // Si gane, termino
    if (board_is_win(board))
    {
      //printf("Hurra\n");
      visitar->cabeza -= 1;
      //printf("la cabeza esta en [%d.%d]\n", visitar->nodos[visitar->cabeza].snek->head->row,
      //visitar->nodos[visitar->cabeza].snek->head->col);
      //printf("el valor de cabeza actual es: %d\n", visitar->cabeza);
      // printf("el valor del nodo de donde vienes es: %d\n", visitar->nodos[visitar->cabeza].num);
      //printf("el primero es  valor direccion : %d\n", visitar->nodos[visitar->cabeza].dir);

      int num;
      num = visitar->nodos[visitar->cabeza].num;
      movs[mov_count] = visitar->nodos[visitar->cabeza].dir;
      mov_count++;
      //printf("------------------- %d\n",visitar->nodos[num].dir );
      movs[mov_count] = visitar->nodos[num].dir;
      mov_count++;
      while (num > 0)
      {
        num = visitar->nodos[num].num;
        //printf("el valor del nodo de donde vienes es: %d\n", visitar->nodos[num].num);
        //printf("el valor direccion : %d\n", visitar->nodos[num].dir);
        movs[mov_count] = visitar->nodos[num].dir;
        mov_count++;

      }

      break;
    }

    // veo los nuevos moviementos que se pueden hacer desde el nuevo snek
    for (int dir =0; dir < 4;dir++)
    {
      // Ejecuto el movimiento
      copy = board_move(board, dir);
      if(!copy)
      {
        //printf("no valido perro\n");
      }
      else
      {
        //printf("si lo puedo hacer \n" );
        visitar->nodos[visitar->cola].num = visitar->cabeza-1;
        visitar->nodos[visitar->cola].dir = dir;
        visitar->nodos[visitar->cola].snek = copy;
        visitar->cola += 1;
      }
    }


    // Almaceno el movimiento en el arreglo
    //movs[mov_count] = dir;
    //mov_count++;

    // Escribo el movimiento en el archivo

  }




  // Abro el archivo de output
  FILE* out = fopen(argv[2], "w");

  // Escribo el numero de movimientos
  fprintf(out, "%d\n", mov_count);

  // Escribo los movimientos
  for (int i = 0; i < mov_count; i++)
  {
    fprintf(out, "%u\n", movs[mov_count - i-1]);
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
