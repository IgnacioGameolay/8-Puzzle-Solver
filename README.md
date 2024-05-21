En este *repl* puedes encontrar varios ejemplos que te pueden ayudar con las tareas de estructura.

## Código de Ejemplo (tarea1)
Para ejecutar el ejemplo tarea1 primero debemos compilar (en la carpeta raíz)
````
gcc tdas/*.c tarea1.c -Wno-unused-result -o tarea1
````

Y luego ejecutar:
````
./tarea1
````

## TDAs
En la carpeta `tdas` se encuentran implementados distintos TDAs que puedes utilizar (lista, pila, cola, cola con prioridad y mapas). 

Las implementaciones no son las más eficientes (todas usan como estructura de datos una **lista enlazada**), por lo que puedes reemplazarlas por las que has realizado en los labs.

## Otros códigos (en carpeta examples)
Para ejecutar los distintos ejemplos que hay en la carpeta `examples`, primero debes compilarlos. Si estamos en la carpeta raíz:
````
gcc tdas/*.c examples/example2_menu.c -Wno-unused-result -o example
````
Y luego ejecutarlos:
````
./example
````

Se incluyen los siguientes ejemplos:
* `example1_list`: Uso del TDA Lista, inserción y eliminación de elementos.
* `example2_menu`: Ejemplo de menú con submenús.
* `example3_readcsv`: Ejemplo de lectura desde un archivo csv y almacenamiento en datos estructurados.
* `example4_map`: Ejemplo de uso del TDA mapa.