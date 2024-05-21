#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char title[100];
  List *genres;
  char director[300];
  float rating;
  int year;
} Film;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por id");
  puts("3) Buscar por director");
  puts("4) Buscar por género");
  puts("5) Buscar por década");
  puts("6) Buscar por rango de calificaciones");
  puts("7) Buscar por década y género");
  puts("8) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

/**
 * Carga películas desde un archivo CSV y las almacena en un mapa por ID.
 */
void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenres) {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));
    strcpy(peli->id, campos[1]);        // Asigna ID
    strcpy(peli->title, campos[5]);     // Asigna título
    strcpy(peli->director, campos[14]); // Asigna director
    peli->genres = split_string(campos[11], ",");       // Inicializa la lista de géneros
    peli->year =
        atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero

    
    // Inserta la película en el mapa usando el ID como clave
    map_insert(pelis_byid, peli->id, peli);

    // Código generado con ayuda de chatgpt3.5
    // conversación: https://chat.openai.com/share/5f0643ad-e8f5-4fb7-a0fa-2d2f92408429
    
    // Obtiene el primer género de la lista de géneros de la película
    char *genre = list_first(peli->genres);
    // Itera sobre cada género de la película
    while (genre != NULL) {
        // Busca el género en el mapa pelis_bygenres
        MapPair *genre_pair = map_search(pelis_bygenres, genre);

        // Si el género no existe en el mapa, crea una nueva lista y agrégala al mapa
        if (genre_pair == NULL) {
            List *new_list = list_create();
            list_pushBack(new_list, peli);
            map_insert(pelis_bygenres, genre, new_list);
        } else {
            // Si el género ya existe en el mapa, obtén la lista y agrega la película
            List *genre_list = (List *)genre_pair->value;
            list_pushBack(genre_list, peli);
        }

        // Avanza al siguiente género en la lista
        genre = list_next(peli->genres);
    }
    
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas


  // Itera sobre el mapa para mostrar las películas cargadas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    printf("ID: %s, Título: %s, Director: %s, Año: %d\n", peli->id, peli->title,
           peli->director, peli->year);

    printf("Géneros: ");
    for(char *genre = list_first(peli->genres); genre != NULL; genre = list_next(peli->genres))
      printf("%s, ", genre);
    printf("\n");
    
    pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
  }
}

/**
 * Busca y muestra la información de una película por su ID en un mapa.
 */
void buscar_por_id(Map *pelis_byid) {
  char id[10]; // Buffer para almacenar el ID de la película

  // Solicita al usuario el ID de la película
  printf("Ingrese el id de la película: ");
  scanf("%s", id); // Lee el ID del teclado

  // Busca el par clave-valor en el mapa usando el ID proporcionado
  MapPair *pair = map_search(pelis_byid, id);

  // Si se encontró el par clave-valor, se extrae y muestra la información de la
  // película
  if (pair != NULL) {
    Film *peli =
        pair->value; // Obtiene el puntero a la estructura de la película
    // Muestra el título y el año de la película
    printf("Título: %s, Año: %d\n", peli->title, peli->year);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La película con id %s no existe\n", id);
  }
}

void buscar_por_genero(Map *pelis_bygenres) {
  char genero[100];

  // Solicita al usuario el ID de la película
  printf("Ingrese el género de la película: ");
  scanf("%s", genero); // Lee el ID del teclado

  MapPair *pair = map_search(pelis_bygenres, genero);
  
  if (pair != NULL) {
      List* pelis = pair->value;
      Film *peli = list_first(pelis);
      
      while (peli != NULL) {
        printf("ID: %s, Título: %s, Director: %s, Año: %d\n", peli->id, peli->title,
           peli->director, peli->year);
        peli = list_next(pelis);
      }
  }
}

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_byid = map_create(is_equal_str);
  Map *pelis_bygenres = map_create(is_equal_str);

  // Recuerda usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_byid, pelis_bygenres);
      break;
    case '2':
      buscar_por_id(pelis_byid);
      break;
    case '3':
      break;
    case '4':
      buscar_por_genero(pelis_bygenres);
      break;
    case '5':
      break;
    case '6':
      break;
    case '7':
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}
