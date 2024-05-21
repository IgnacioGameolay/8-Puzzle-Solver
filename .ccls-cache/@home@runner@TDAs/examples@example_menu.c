#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>

// Prototipos de funciones auxiliares
void registrarPaciente();
void importarPacientes();
void exportarPacientes();

// Función para mostrar el menú
void mostrarMenu() {
  system("clear");
  puts("========================================");
  puts("          Control de Pacientes");
  puts("========================================");

  puts("1) Registrar Paciente.");
  puts("2) Importar Pacientes desde cvs.");
  puts("3) Exportar Pacientes a cvs.");
  puts("4) Salir.");
}

int main() {
  int opcion;

  do {
    mostrarMenu();
    printf("Ingrese su opción: ");
    scanf("%d", &opcion);

    switch (opcion) {
    case 1:
      registrarPaciente();
      break;
    case 2:
      importarPacientes();
      break;
    case 3:
      exportarPacientes();
      break;
    case 4:
      puts("Saliendo del programa...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }

  } while (opcion != 4);

  return 0;
}

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

// Definición de funciones auxiliares
void registrarPaciente() {
  puts("registrarPaciente");
  presioneTeclaParaContinuar();
}

void importarPacientes() {
  puts("importarPacientes");
  presioneTeclaParaContinuar();
}

void exportarPacientes() {
  puts("exportarPacientes");
  presioneTeclaParaContinuar();
}
