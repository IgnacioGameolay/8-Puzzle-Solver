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


/**
 *
 * Función para imprimir el estado del puzzle, es decir, todos sus elementos.
 *
 * @param *estado Puntero al estado del puzzle.
 */
// 
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


/**
 *
 * Función comprobar si un estado dado como parametro corresponde al estado objetivo.
 *
 * @param *estado Puntero al estado del puzzle.
 * @return Retorna 1 si los estados son iguales, 0 en caso contrario.
 */
// 
int is_final_state(State* s){
	//Definir estado final o estado objetivo
	int final_state[3][3] = {{0,1,2},{3,4,5},{6,7,8}};
	//Comprobar el estado dado con el final
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if (s->square[i][j] != final_state[i][j]) return 0;
		}
	}
	//Si son iguales retornar 1
	return 1;
}

/**
 *
 * Función para "clonar" o copiar un estado dado como parametro.
 *
 * @param *estado Puntero a un estado del puzzle.
 * @return Retorna el un nuevo estado, siendo una copia exacta del estado dado como parametro.
 */
// 
State* copy(State* s){
	State* new_state = (State*) malloc(sizeof(State));
	if (new_state == NULL){
		printf("Error al asignar memoria para el nuevo estado\n");
		exit(EXIT_FAILURE);
	}
	//Copiar la matriz 'square' del estado dado 's' al nuevo estado 'new_state'
	memcpy(new_state->square, s->square, sizeof(int) * 9);

	//Copiar las posiciones del estado vacio
	new_state->x = s->x;
	new_state->y = s->y;
	new_state->actions = list_create();

	//Crear y copiar la lista de acciones del estado dado al nuevo estado
	int action = list_first(s->actions);
	
	while(action != NULL){
		list_pushBack(new_state->actions, action);
		action = list_next(s->actions);
	}
	//Retornar el estado copiado
	return new_state;
}

/**
 *
 * Funcion para transicionar de un estado a otro
 *
 * @param *estado Puntero a un estado del puzzle.
 * @param action Acci[on] a realizar con la casilla nula (donde se mueve).
 * @return Retorna un nuevo estado, siendo la modificaciones del estado dado una vez aplicada la accion.
 */
// 
State* transition(State* current_state, int action){
	int new_x = current_state->x;
	int new_y = current_state->y;

	//Modificar la pos. de la casilla nula seg[un] la accion.
	switch(action){
		case 1:
			new_y--; //Arriba
			break;
		case 2:
			new_y++; //Abajo
			break;
		case 3:
			new_x--; ///Izquierda
			break;
		case 4:
			new_x++; //Derecha
			break;
		default:
			return NULL; // Si la accion no es valida return NULL
	}

	//Comprobar si es valido el mov.
	if (new_x >= 0 && new_x < 3 && new_y >= 0 && new_y < 3){
		//Crear estado
		State* new_state = copy(current_state);

		//Cambiar espacio vacio con el cuadro adyacente
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

/**
 *
 * Funcion para obtener el listado de estados v[alidos adyacentes a un estado dado
 * como parametro. Es decir, los estados o nodos que se pueden llegar a partir de ese estado.
 *
 * @param *estado Puntero a un estado que queremos obtener los adyacentes.
 * @return Retorna una lista de los estados a los que se puede llegar desde el estado dado.
 */
// 
List* get_adj_states(State* s){
	List* adj = list_create();//Crear lista de estados adyacentes
	
	for (int action = 1; action <= 4; action++){
		State* new_state = transition(s, action); //Obtener un estado adyacente
		if (new_state != NULL){
			//Si el estado calculado es válido, se agrega a la lista de estados adyacentes
			list_pushBack(adj, new_state);
		}
	}
	
	return adj;
}

/**
 *
 * Funcion para calcular la Distancia de Manhattan entre el estado dado y el estado final.
 * Esta calculo se realiza sumando las distancias horizontales y verticales entre cada elemento
 *
 * @param *estado Puntero a un estado que queremos obtener los adyacentes.
 * @return Retorna una lista de los estados a los que se puede llegar desde el estado dado.
 */
// 
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


/**
 *
 * Funcion para realizar la Busuqeda en Profundidad (DFS) desde un estado inicial dado.
 * Esta funcion utiliza una pila para realizar la busqueda y verificacion de etados.
 * Se utiliza una limitante de altura para detener la busqeuda y evitar iterar infinitamente.
 *
 * @param *initial_state Puntero al estado inicial desde el cual iniciar la búsqueda.
 * @return *count Puntero al contador de iteraciones que se tomaron al realizar la búsqueda.
 */
// 
State* DFS(State* initial_state, int* count){
	//Crear limitantes de altura
	int max_high = 12;
	int current_high = 0;

	//Crear Stack
	Stack* stack = stack_create(stack);
	stack_push(stack, initial_state);

	printf("\n--Estado N°%d--\n", *count);
	imprimirEstado(initial_state);

	//Recorremos los estados almacenados en el Stack
	while(list_size(stack) != 0){
		State* current_state = (State*) stack_top(stack);
		stack_pop(stack);

		//Verificar altura
		current_high= list_size(current_state->actions);
		if (current_high >= max_high) continue;

		//Obtener estados adyacentes del estado actual (current_state)
		List* adj = get_adj_states(current_state);
		State* aux_state = list_first(adj);

		//Recorremos la lista de estados adyacentes
		while(aux_state != NULL){
			//Verificacion de estado final
			if (is_final_state(aux_state)){
				list_clean(adj);
				stack_clean(stack);
				(*count)++;
				return aux_state;
			}

			//Si no es estado final, agregar al stack
			stack_push(stack, aux_state); 

			//Aumentar contador e imprimir estado
			(*count)++;
			printf("\n--Estado N°%d--\n", *count);
			imprimirEstado(aux_state);
			
			aux_state = list_next(adj); //Pasar al siguiente estado adyacente 
		}
		
		free(aux_state);
		list_clean(adj);
		
	}
	printf("No se encontro solucion\n");
	return NULL;
}

/**
 *
 * Funcion para realizar la Búsqueda en Amplitud (BFS) desde un estado inicial dado.
 * Esta funciíon utiliza una cola para realizar la busqueda y verificacion de etados.
 * Se utiliza una limitante de altura para detener la busqeuda y evitar iterar infinitamente.
 *
 * @param *initial_state Puntero al estado inicial desde el cual iniciar la búsqueda.
 * @return *count Puntero al contador de iteraciones que se tomaron al realizar la búsqueda.
 */
// 
State* BFS(State* initial_state, int* count){
	//Crear limitantes de altura
	int max_high = 12;
	int current_high = 0;

	//Crear Cola
	Queue* queue = queue_create(queue);
	queue_insert(queue, initial_state);

	//Recorremos los estados almacenados en la cola
	while(list_size(queue) != 0){
		State* current_state = (State*) queue_front(queue);
		queue_remove(queue);

		//Verificar altura
		current_high= list_size(current_state->actions);
		if (current_high >= max_high) continue;

		//Obtener estados adyacentes del estado actual (current_state)
		List* adj = get_adj_states(current_state);
		State* aux_state = list_first(adj);

		//Recorremos la lista de estados adyacentes
		while(aux_state != NULL){
			//Verificacion de estado final
			if (is_final_state(aux_state)){
				list_clean(adj);
				queue_clean(queue);
				(*count)++;
				return aux_state;
			}

			//Si no es estado final, agregar al queue
			queue_insert(queue, aux_state);

			//Aumentar contador e imprimir estado
			(*count)++;
			printf("--Estado N°%d--\n", *count);
			imprimirEstado(aux_state);
			
			aux_state = list_next(adj); //Pasar al siguiente estado adyacente
		}

		free(aux_state);
		list_clean(adj);
	}
	printf("No se encontro solucion\n");
	return NULL;
}

/**
 *
 * Función para realizar la Búsqueda del Mejor Primero (Best-First) desde un estado inicial.
 *
 * Esta funciíon utiliza un montículo(Heap) para mantener los estados y 
 * seleccionar el mejor candidato en cada iteración.
 *
 * Adicionalmente, se utiliza una limitante de altura para detener la busqeuda y evitar iterar 
 * infinitamente.
 *
 * @param *initial_state Puntero al estado inicial desde el cual iniciar la búsqueda.
 * @return *count Puntero al contador de iteraciones que se tomaron al realizar la búsqueda.
 */
// 
State* BEST_FIRST(State* initial_state, int* count){
	//Crear limitantes de altura
	int max_high = 12;
	int current_high = 0;
	
	//Crear Heap
	Heap* heap = heap_create();
	
	//Calcular distancia e insertar con prioridad en el Heap
	int dist = distancia_L1(initial_state);
	heap_push(heap, initial_state, dist);

	//Recorremos los estados almacenados en el Heap
	State* current_state = NULL;
	while(heap_top(heap)){
		current_state = (State*) heap_top(heap);
		heap_pop(heap);
		
		//Verificar altura
		current_high = list_size(current_state->actions);
		if (current_high >= max_high) continue;
		
		//Obtener estados adyacentes del estado actual (current_state)
		List* adj = get_adj_states(current_state);
		State* aux_state = list_first(adj);

		//Recorremos la lista de estados adyacentes
		while(aux_state != NULL){
			//Verificar estado final
			if (is_final_state(aux_state)){
				list_clean(adj);
				free(heap);
				(*count)++;
				return aux_state;
			}

			//Si no es estado final, agregar al heap con prioridad
			dist = distancia_L1(aux_state); //Calcular mejor distancia
			heap_push(heap, aux_state, dist);

			//Aumentar contador e imprimir estado
			(*count)++;
			printf("--Estado N°%d--\n", *count);
			imprimirEstado(aux_state);
			
			aux_state = list_next(adj); //Pasar al siguiente estado adyacente
		}
		
		free(aux_state);
		list_clean(adj);
	}
	
	printf("No se encontro solucion\n");
	return current_state;
}


/**
 *
 * Función main
 *
*/ 
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
	// Crear la lista de acciones
	estado_inicial.actions = list_create();
	
	// Imprime el estado inicial
	puts("========================================");
	puts("     Estado inicial del puzzle:\n");
	imprimirEstado(&estado_inicial);
	puts("========================================");
	
	// Declarar variables a utilizar
	int count; //Contador de iteraciones
	State* final_state = (State*) malloc(sizeof(State)); //Puntero al estado final
	char opcion; //Option del menu
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
			//Busqeuda en Profundidad
			count = 0;
			final_state = DFS(&estado_inicial, &count);
			printf("--FINAL DFS-- con %d iteraciones\n",count);
			imprimirEstado(final_state);
			break;
		case '2':
			//Busqueda en Amplitud
			count = 0;
			final_state = BFS(&estado_inicial, &count);
			printf("--FINAL BFS-- con %d iteraciones\n",count);
			imprimirEstado(final_state);
			break;
		case '3':
			//Busqeuda del Mejor Primero
			count = 0;
			final_state = BEST_FIRST(&estado_inicial, &count);
			printf("--FINAL BEST_FIRST-- con %d iteraciones\n",count);
			imprimirEstado(final_state);
			break;
		}
		presioneTeclaParaContinuar();
		limpiarPantalla();

	} while (opcion != '4');

	return 0;
}
