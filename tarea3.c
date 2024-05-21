#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>

// Definición de la estructura para el estado del puzzle
typedef struct {
    int square[3][3]; // Matriz 3x3 que representa el tablero
    int x;    // Posición x del espacio vacío
    int y;    // Posición y del espacio vacío
    List* actions; //Secuencia de movimientos para llegar al estado
} State;

int distancia_L1(State* state) {
    int ev=0;
    int k=1;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++){
            int val=state->square[i][j];
            if (val==0) continue;
            int ii=(val)/3;
            int jj=(val)%3;
            ev+= abs(ii-i) + abs(jj-j);
        }
    return ev;
}


// Función para imprimir el estado del puzzle
void imprimirEstado(const State *estado) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (estado->square[i][j] == 0)
                printf("x "); // Imprime un espacio en blanco para el espacio vacío
            else
                printf("%d ", estado->square[i][j]);
        }
        printf("\n");
    }
}


int main() {
    // Estado inicial del puzzle
    State estado_inicial = {
        {{0, 2, 8}, // Primera fila (0 representa espacio vacío)
         {1, 3, 4}, // Segunda fila
         {6, 5, 7}, // Tercera fila
         },  
        0, 1   // Posición del espacio vacío (fila 0, columna 1)
    };
    estado_inicial.actions = list_create();

    // Imprime el estado inicial
    printf("Estado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);

    printf("Distancia L1:%d\n", distancia_L1(&estado_inicial));

    //Ejemplo de heap (cola con prioridad)
    printf("\n***** EJEMPLO USO DE HEAP ******\nCreamos un Heap e insertamos 3 elementos con distinta prioridad\n");
    Heap* heap = heap_create();
    char* data = strdup("Cinco");
    printf("Insertamos el elemento %s con prioridad -5\n", data);
    heap_push(heap, data, -5 /*prioridad*/);
    data = strdup("Seis");
    printf("Insertamos el elemento %s con prioridad -6\n", data);
    heap_push(heap, data, -6 /*prioridad*/);
    data = strdup("Siete");
    printf("Insertamos el elemento %s con prioridad -7\n", data);
    heap_push(heap, data, -7 /*prioridad*/);

    printf("\nLos elementos salen del Heap ordenados de mayor a menor prioridad\n");
    while (heap_top(heap) != NULL){
        printf("Top: %s\n", (char*) heap_top(heap));      
        heap_pop(heap);
    }
    printf("No hay más elementos en el Heap\n");

    int opcion;
    do {
        printf("\n***** EJEMPLO MENU ******\n");
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");
        
        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");
    
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);
    
        switch (opcion) {
        case '1':
          //dfs(estado_inicial);
          break;
        case '2':
          //bfs(estado_inicial);
          break;
        case '3':
          //best_first(estado_inicial);
          break;
        }
        presioneTeclaParaContinuar();
        limpiarPantalla();

  } while (opcion != '4');

  return 0;
}
