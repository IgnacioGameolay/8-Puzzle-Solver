#include <stdio.h>
#include <stdlib.h>

// Definición de la estructura para el estado del puzzle
typedef struct {
    int square[3][3]; // Matriz 3x3 que representa el tablero
    int x;    // Posición x del espacio vacío
    int y;    // Posición y del espacio vacío
} State;

int distancia(int* square) {
    int ev=0;
    int k=1;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++){
            if(i==2 && j==2) break;
            int val=square[i][j];
            int ii=(val-1)/3;
            int jj=(val-1)%3;
            ev+= abs(ii-i) + abs(jj-j);
        }
    return ev;
}




// Función para imprimir el estado del puzzle
void imprimirEstado(const State *estado) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (estado->square[i][j] == 0)
                printf("  "); // Imprime un espacio en blanco para el espacio vacío
            else
                printf("%d ", estado->square[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Estado inicial del puzzle
    State estado_inicial = {
        {{8, 0, 6} // Primera fila (0 representa espacio vacío)
         {5, 4, 7}, // Segunda fila
         {2, 3, 1}, // Tercera fila
         },  
        0, 1   // Posición del espacio vacío (fila 0, columna 1)
    };

    // Imprime el estado inicial
    printf("Estado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);

    return 0;
}
