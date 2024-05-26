# 8-Puzzles Resolver

## Descripción del programa

Este programa cumple la funcionalidad de ser un solucionador para rompecabezas de 8 piezas, mejor conocido como 8-Puzzle. Lo anterior implica que así a los usuarios cargar películas, buscar por id, buscar por director, buscar por género, buscar por década, buscar por rango de calificaciones, buscar por década y género (doble filtro). 
Este programa está enfocado para facilitar la búsqueda y filtraje de películas dada una base de datos. Además, el sistema resulta, ser intuitivo y fácil de utilizar.



## Cómo compilar y ejecutar

Como uno de los objetivos del programa es ser fácil de utilizar y ser sumamente accesible, es recomendable utilizar webs como `Repl.it` para probar y ejecutar el sistema rápidamente. 
Para ello seguir los siguientes pasos:

1. Visitar [Repl.it](https://repl.it/).
2. Crear una nueva cuenta o iniciar sesión si ya tienes una.
3. Estando en el `dashboard`, seleccionar la opción "`New Repl`" y eligir "`Import from GitHub`".
4. Clonar el repositorio de GitHub: `https://github.com/IgnacioGameolay/Tarea3.git`
5. [Repl.it](http://repl.it/) clonará el repositorio y preparará el entorno de ejecución automáticamente.
6. Finalmente, presionar el botón "Run" para compilar y ejecutar la aplicación de manera automática. O bien, compilar el código manualmente utilizando: `gcc tdas/*.c tarea3.c -Wno-unused-result -o tarea3` , y posteriormente ejecutar con: `./tarea3`


## Funcionalidades

### Funcionando correctamente:

- Resolver el puzzle utilizando Búsqueda en Profundidad (DFS)
- Resolver el puzzle utilizando Búsqueda en Profundidad (BFS)
- Resolver el puzzle utilizando Búsqueda de Mejor Primero (Best-First Search)
- Salir del sistema.

### Problemas conocidos:

- Se visitan nodos o estados que ya han sido visitados anteriormente, lo que aumenta el número de iteraciones. Esto pordría ocurrir debido a la naturaleza del almacenamiento de los nodos adyacentes en la estructura.
- Si se modifica la posición inicial o final desde el código, el programa debería de ser capaz de solucionarlo, pero es probable que si el estado ingresado está muy alejado de la respuesta, el programa podría no encontrar la solución por limitaciones de memoria y/o altura.
- Al estar las búsquedas limitadas por altura, es probable que para ciertos estados iniciales no alcance a hallar soluciones.

### A mejorar:

- Permitir la inserción manual de estados iniciales y altura máxima personalizados.
- Incorporar la funcionalidad de marcar los nodos o estados que ya han sido visitados, ahorrando así iteraciones y permitiendo búsquedas más eficaces.
- Implementar y desarrollar una interfaz que facilite al usuario trabajar con el sistema mucho más cómodamente que por consola.

## Ejemplo de uso
A continuación, veremos un ejemplo de cómo utilizar el sistema para buscar la solución a un 8-puzzle dado un estado inicial.
Para ello, al iniciar el sistema tendremos el siguiente menú con el cual interactuar:

``` 
========================================
     Estado inicial del puzzle:

x 2 8 
1 3 4 
6 5 7 
========================================
========================================
     Escoge método de búsqueda
========================================
1) Búsqueda en Profundidad
2) Buscar en Anchura
3) Buscar Mejor Primero
4) Salir
Ingrese su opción: 
```
En este menú, podremos seleccionar una de las 4 opciones cuando se nos solicite `Ingrese su opción: `.

Los nombres de las opciones son bastante descriptivas (hacen lo que mencionan), pero de igual manera a continuación se mostrarán los pasos a seguir en el caso de uso de cada uno de las búsquedas disponibles. Adicionalmente, cabe mencionar que cada opción de búsqueda imprimirá por consola todos los estados visitados hasta llegar al estado final (El '0' es representado por una 'x', que vendría siendo la casilla vacía o nula):

``` 
      x 1 2 
      3 4 5 
      6 7 8 
```


**Opción 1 - Búsqueda en Profundidad (DFS)**

Luego de seleccionar la opción 1, veremos cómo se van recorriendo distintos estados del puzzle hasta llegar al estado final. En esta búsqueda se emplea el uso de una Pila o "Stack" para guardar o almacenar los estados actuales del puzzle, de los cuales luego se calcularán sus estados adyacentes o vecinos, esto se realiza moviendo la casilla vacía en una dirección válida (arriba, abajo, izquierda, derecha). Este procedimiento se repite hasta dar con el estado final. Y como se podrá ver en un ejemplo a continuación, la casilla nula o vacía se moviliza entre los estados adyacentes dado un estado:

```
Ingrese su opción: 1
--Estado N°0--
x 2 8 
1 3 4 
6 5 7 

--Estado N°1--
1 2 8 
x 3 4 
6 5 7 

--Estado N°2--
2 x 8 
1 3 4 
6 5 7 
....

--FINAL DFS-- Con 119558 Iteraciones
x 1 2 
3 4 5 
6 7 8 

```

**Opción 2 - Búsqueda en Amplitud (BFS)**

Similar a la primera opción, en esta búsqueda se emplea una Cola o Queue para almacenar los nuevos estados que se van generando. La principal diferencia con la Búsqueda en Profundidad, es que se evalúan todos los estados vecinos antes de pasar a evaluar los estados de la Cola. A continuación un ejemplo:

```
Ingrese su opción: 2
--Estado N°0--
x 2 8 
1 3 4 
6 5 7 

--Estado N°1--
1 2 8 
x 3 4 
6 5 7 

--Estado N°2--
2 x 8 
1 3 4 
6 5 7 
....

--FINAL BFS-- con 221143 iteraciones
x 1 2 
3 4 5 
6 7 8 

```

**Opción 3 - Búsqueda del Mejor Primero (Best-First Search)**

Al seleccionar la 3ra opción, se realizará una búsqueda por el mejor primero, aplicando una "función heurística", o bien, función de evaluación, conocida como distancia de "Manhattan", siendo en este caso la la cantidad de acciones más corta que le tomaría a un numero en una determinada posición llegar a su casilla/posición final. Para ello en cada iteración se selecciona al nodo que tiene el valor más bajo (la distancia más corta) y se les da mayor prioridad en el HEAP (estructura utilizada) por estar más cerca del resultado. El resto es similar a las opciones anteriores, a continuación el ejemplo:

```
Ingrese su opción: 3
--Estado N°0--
x 2 8 
1 3 4 
6 5 7 

--Estado N°1--
1 2 8 
x 3 4 
6 5 7 

--Estado N°2--
2 x 8 
1 3 4 
6 5 7 
....

--FINAL BEST_FIRST-- con 299006 iteraciones
x 1 2 
3 4 5 
6 7 8 
```

**Opción 4: Salir**

Finalmente, para terminar la ejecución del programa, seleccionamos la opción 4 y finalizará automáticamente.

***Cualquier consulta o duda que pueda surgir, contactarse al siguiente mail: 
ireyestoledo8@gmail.com***
-