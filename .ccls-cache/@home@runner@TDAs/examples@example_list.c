#include "../tdas/list.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  // Crear una lista
  List *myList = list_create();
  if (myList == NULL) {
    printf("No se pudo crear la lista.\n");
    return 1;
  }

  // Agregar elementos
  int data[] = {10, 20, 30, 40, 50};
  for (int i = 0; i < 5; ++i) {
    list_pushBack(myList, &data[i]);
  }

  // Mostrar elementos (recorrido desde el principio)
  printf("Elementos de la lista: ");
  for (int *num = list_first(myList); num != NULL; num = list_next(myList)) {
    printf("%d ", *num);
  }
  printf("\n");

  // Eliminar el primer y último elemento
  list_popFront(myList);
  list_popBack(myList);

  // Mostrar elementos después de las eliminaciones
  printf("Elementos después de popFront y popBack: ");
  for (int *num = list_first(myList); num != NULL; num = list_next(myList)) {
    printf("%d ", *num);
  }
  printf("\n");

  // Limpiar la lista
  list_clean(myList);
  free(myList);
}