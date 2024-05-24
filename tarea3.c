#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/stack.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include "tdas/queue.h"
#include <string.h>

// Definición de la estructura para el estado del puzzle
typedef struct {
	int square[3][3]; // Matriz 3x3 que representa el tablero
	int x;    // Posición x del espacio vacío
	int y;    // Posición y del espacio vacío
	List* actions; //Secuencia de movimientos para llegar al estado
	//int is_visited; //Si es 1 ya se visito, si no ha sido visitado es un 0
} State;


// Función para crear un nuevo estado
State* createState(){
	State* s = (State*)malloc(sizeof(State));
	s->actions = list_create();
	return s;
}

int is_final_state(State* s){
	//int final_state[3][3] = {{1,2,3},{4,5,6},{7,8,0}};
	int final_state[3][3] = {{0,1,2},{3,4,5},{6,7,8}};
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if (s->square[i][j] != final_state[i][j]) return 0;
		}
	}
	return 1;
}

// Función para copiar un nuevo estado
State* copy(State* s){
	State* new_state = (State*) malloc(sizeof(State));
	
	memcpy(new_state->square, s->square, sizeof(int) * 9);
	
	new_state->x = s->x;
	new_state->y = s->y;
	new_state->actions = list_create();
	
	int action = list_first(s->actions);
	
	while(action != NULL){
		list_pushFront(new_state->actions, action);
		action = list_next(s->actions);
	}
	return new_state;
	
}

State* transition(State* current_state, int action){
	int new_x = current_state->x;
	int new_y = current_state->y;

	switch(action){
		case 1:
			new_y--;//arriba
			break;
		case 2:
			new_y++; //abajo
			break;
		case 3:
			new_x--; ///izq
			break;
		case 4:
			new_x++; //der
			break;
		default:
			return NULL;
	}

	//Comprobar si es valido el mov.
	if (new_x >= 0 && new_x < 3 && new_y >= 0 && new_y < 3){
		//Crear estado
		State* new_state = copy(current_state);

		//Cambiar espacio vacio con la cuadro adyacente
		new_state->square[current_state->x][current_state->y] = new_state->square[new_x][new_y];

		new_state->square[new_x][new_y] = 0;
		new_state->x = new_x;
		new_state->y = new_y;
		//Agregar action a la list de actions
		list_pushFront(new_state->actions, action);
		return new_state;
	}
	return NULL;
}

List* get_adj_states(State* s){
	List* adj = list_create();
	
	//while (!is_final_state(aux)){
		for (int action = 1; action <= 4; action++){
			State* new_state = transition(s, action);
			if (new_state != NULL){
				list_pushFront(adj, new_state);
			}
		}
		//aux = new_state;
	//}
	//printf("xd");	
	return adj;
}



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

/*Para evitar que la búsqueda en profundidad se quede iterando hasta el infinito, pueden limitar la altura del árbol de búsqueda. Por ejemplo. si el tamaño de la lista de acciones es mayor a 10, continuar con el siguiente estado (y no agregar estados adyacentes).*/
State* DFS(State* initial_state, int* count){
	//Crear limitantes
	//
	int max_high = 12;
	int current_high = 0;

	//Crear stacks
	Stack* stack = stack_create(stack);
	stack_push(stack, initial_state);
	
	
	while(list_size(stack) != 0){
		State* current_state = (State*) stack_top(stack);
		stack_pop(stack);
		
		
		/*if (is_final_state(current_state)){
			free(stack);
			return current_state;
		}*/
		//printf("altura actual: %d\n", current_high);
				current_high= list_size(current_state->actions);
				if (current_high >= max_high) continue;
		//if (current_state->is_visited == 1) continue;
		//else current_state->is_visited = 1;
		
		//if (list_size(current_state->actions) < 
			List* adj = get_adj_states(current_state);
			State* aux_state = list_first(adj);
		
			while(aux_state != NULL){
				if (is_final_state(aux_state)){
					list_clean(adj);
					stack_clean(stack);
					return aux_state;
				}
										
				stack_push(stack, aux_state);
				
				//printf("----%d----\n", *count);
				aux_state = list_next(adj);
			}
		//
		
				//list_clean(aux_state->actions);
			//free(aux_state);
			//free(current_state);
				//list_clean(current_state->actions);
				//free(aux_state->actions);
				free(aux_state);
				//free(current_state);
				list_clean(adj);
				(*count)++;
		//current_high++;
		//} 
		
	}
	printf("No se encontro solucion\n");
	return NULL;
}

State* BFS(State* initial_state, int* count){
	//Crear limitantes
	//
	int max_high = 12;
	int current_high = 0;

	//Crear stacks
	Stack* queue = queue_create(queue);
	queue_insert(queue, initial_state);


	while(list_size(queue) != 0){
		State* current_state = (State*) queue_front(queue);
		queue_remove(queue);

		/*
		if (is_final_state(current_state)){
			list_clean(adj);
			free(queue);
			return current_state;
		}*/
		//printf("altura actual: %d\n", current_high);
				current_high= list_size(current_state->actions);
				if (current_high >= max_high) continue;
		//if (current_state->is_visited == 1) continue;
		//else current_state->is_visited = 1;

		//if (list_size(current_state->actions) < 
			List* adj = get_adj_states(current_state);
			State* aux_state = list_first(adj);

			while(aux_state != NULL){
				if (is_final_state(aux_state)){
					list_clean(adj);
					queue_clean(queue);
					return aux_state;
				}

				queue_insert(queue, aux_state);

				//printf("----%d----\n", *count);
				aux_state = list_next(adj);
			}
		//

				//list_clean(aux_state->actions);
			//free(aux_state);
			//free(current_state);
				//list_clean(current_state->actions);
				//free(aux_state->actions);
				free(aux_state);
				//free(current_state);
				list_clean(adj);
				(*count)++;
		//current_high++;
		//} 

	}
	printf("No se encontro solucion\n");
	return NULL;
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
	printf("----\n");
}


int main() { 
	// Estado inicial del puzzle
	
	State estado_inicial = {
		{{0, 2, 8}, // Primera fila (0 representa espacio vacío)
		 {1, 3, 4}, // Segunda fila
		 {6, 5, 7}, // Tercera fila
		 },  
		0, 0   // Posición del espacio vacío (fila 0, columna 1)
	}; 
	/*State estado_inicial = {
		{{1, 2, 5}, // Primera fila (0 representa espacio vacío)
		 {3, 4, 8}, // Segunda fila
		 {6, 7, 0}, // Tercera fila
		 },  
		2, 2   // Posición del espacio vacío (fila 0, columna 1)
	};*/
	estado_inicial.actions = list_create();

	// Imprime el estado inicial
	printf("Estado inicial del puzzle:\n");
	imprimirEstado(&estado_inicial);

	printf("Distancia L1:%d\n", distancia_L1(&estado_inicial));


	//Generar estados adyacentes
	List* adj = get_adj_states(&estado_inicial);
	printf("Estados adyacentes:\n");
	
	State* state = (State*)list_first(adj);
	while (state != NULL){
		imprimirEstado(state);
		state = (State*)list_next(adj);
		printf("----\n");
	}

	
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

	int count;
	State* final_state = (State*) malloc(sizeof(State));
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
			count = 0;
			final_state = DFS(&estado_inicial, &count);
			printf("--FINAL DFS-- con %d\n",count);
			imprimirEstado(final_state);
			break;
		case '2':
			count = 0;
			final_state = BFS(&estado_inicial, &count);
			printf("--FINAL BFS-- con %d\n",count);
			imprimirEstado(final_state);
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
