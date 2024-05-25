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

// Función para evaluar si un estado dado es el final
int is_final_state(State* s){
	
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
	if (new_state == NULL){
		printf("Error al asignar memoria para el nuevo estado\n");
		exit(EXIT_FAILURE);
	}
	
	memcpy(new_state->square, s->square, sizeof(int) * 9);
	
	new_state->x = s->x;
	new_state->y = s->y;
	new_state->actions = list_create();
	
	int action = list_first(s->actions);
	
	while(action != NULL){
		list_pushBack(new_state->actions, action);
		action = list_next(s->actions);
	}
	return new_state;
	
}

//Funcion para transicionar de un estado a otro
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
		list_pushBack(new_state->actions, action);
		return new_state;
	}
	return NULL;
}

//Funcion para obtener el listado de estados adyacentes
List* get_adj_states(State* s){
	List* adj = list_create();
	

	for (int action = 1; action <= 4; action++){
		State* new_state = transition(s, action);
		if (new_state != NULL){
			list_pushBack(adj, new_state);
		}
	}

	return adj;
}

//FUncion para calcular la Distancia de Manhattan
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

//Funcion para realizar la Busqueda en Profundidad
State* DFS(State* initial_state, int* count){
	//Crear limitantes
	int max_high = 12;
	int current_high = 0;

	//Crear stacks
	Stack* stack = stack_create(stack);
	stack_push(stack, initial_state);

	printf("\n--Estado N°%d--\n", *count);
	imprimirEstado(initial_state);
	
	while(list_size(stack) != 0){
		State* current_state = (State*) stack_top(stack);
		stack_pop(stack);
		
		current_high= list_size(current_state->actions);
		if (current_high >= max_high) continue;

		List* adj = get_adj_states(current_state);
		State* aux_state = list_first(adj);
		
		while(aux_state != NULL){
			if (is_final_state(aux_state)){
				list_clean(adj);
				stack_clean(stack);
				(*count)++;
				return aux_state;
			}

			stack_push(stack, aux_state);
			
			(*count)++;
			printf("\n--Estado N°%d--\n", *count);
			imprimirEstado(aux_state);
			
			aux_state = list_next(adj);
		}
		
		free(aux_state);
		list_clean(adj);
		
	}
	printf("No se encontro solucion\n");
	return NULL;
}

//Funcion para realizar la Busqueda en Amplitud
State* BFS(State* initial_state, int* count){
	//Crear limitantes
	int max_high = 12;
	int current_high = 0;

	//Crear stacks
	Queue* queue = queue_create(queue);
	queue_insert(queue, initial_state);

	//Recorrer cola
	while(list_size(queue) != 0){
		State* current_state = (State*) queue_front(queue);
		queue_remove(queue);


		current_high= list_size(current_state->actions);
		if (current_high >= max_high) continue;
		
		List* adj = get_adj_states(current_state);
		State* aux_state = list_first(adj);

		while(aux_state != NULL){
			if (is_final_state(aux_state)){
				list_clean(adj);
				queue_clean(queue);
				(*count)++;
				return aux_state;
			}
	
			queue_insert(queue, aux_state);
			(*count)++;
			printf("--Estado N°%d--\n", *count);
			imprimirEstado(aux_state);
			
			aux_state = list_next(adj);
		}

		free(aux_state);
		list_clean(adj);
	}
	printf("No se encontro solucion\n");
	return NULL;
}

//Funcion para realizar la Busqueda del Mejor Candidato
State* BEST_FIRST(State* initial_state, int* count){
	int max_high = 12;
	int current_high = 0;
	//Crear stacks
	Heap* heap = heap_create();
	int dist = distancia_L1(initial_state);
	heap_push(heap, initial_state, dist);
	
	State* current_state = NULL;
	while(heap_top(heap)){
		current_state = (State*) heap_top(heap);
		heap_pop(heap);
		
		//imprimirEstado(current_state);
		current_high = list_size(current_state->actions);
		if (current_high >= max_high) continue;
		
		List* adj = get_adj_states(current_state);
		State* aux_state = list_first(adj);

		while(aux_state != NULL){
			if (is_final_state(aux_state)){
				list_clean(adj);
				free(heap);
				(*count)++;
				return aux_state;
			}

			dist = distancia_L1(aux_state);
			heap_push(heap, aux_state, dist);

			(*count)++;
			printf("--Estado N°%d--\n", *count);
			imprimirEstado(aux_state);
			
			
			aux_state = list_next(adj);
		}
		
		free(aux_state);
		list_clean(adj);
		
	}
	
	printf("No se encontro solucion\n");
	return current_state;
}

int main() { 
	// Estado inicial del puzzle
	//028134657
	State estado_inicial = {
		{{0, 2, 8}, // Primera fila (0 representa espacio vacío)
		 {1, 3, 4}, // Segunda fila
		 {6, 5, 7}, // Tercera fila
		 },  
		0, 0   // Posición del espacio vacío (fila 0, columna 0)
	}; 

	estado_inicial.actions = list_create();
	
	// Abrir archivo de log
	FILE* log_file = fopen("log.txt", "w");
	if (log_file == NULL) {
			printf("Error al abrir el archivo de log\n");
			return 1;
	}

	// Redirigir stdout al archivo de log
	int stdout_copy = dup(fileno(stdout));
	if (dup2(fileno(log_file), fileno(stdout)) == -1) {
			printf("Error al redirigir stdout\n");
			fclose(log_file);
			return 1;
	}
	
	// Imprime el estado inicial
	puts("========================================");
	puts("     Estado inicial del puzzle:\n");
	imprimirEstado(&estado_inicial);
	puts("========================================");
	int count;
	State* final_state = (State*) malloc(sizeof(State));
	int opcion;
	do {
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
			printf("--FINAL DFS-- con %d iteraciones\n",count);
			imprimirEstado(final_state);
			return 0;
			break;
		case '2':
			count = 0;
			final_state = BFS(&estado_inicial, &count);
			printf("--FINAL BFS-- con %d iteraciones\n",count);
			imprimirEstado(final_state);
			return 0;
			break;
		case '3':
			count = 0;
			final_state = BEST_FIRST(&estado_inicial, &count);
			printf("--FINAL BEST_FIRST-- con %d iteraciones\n",count);
			imprimirEstado(final_state);
			return 0;
			break;
		}
		presioneTeclaParaContinuar();
		limpiarPantalla();

	} while (opcion != '4');

	return 0;
}
