# Mi Repositorio 

[Repositorio](https://github.com/nelelizalde24/SO_Apuntes.git)

## Índice

- [Mi Repositorio](#mi-repositorio)
  - [Índice](#índice)
    - [Hola Mundo](#hola-mundo)
    - [Proceso 1](#proceso-1)
    - [Cola de Prioridad](#cola-de-prioridad)
    - [Funciones Recursiva](#funciones-recursiva)
    - [Hilos](#hilos)
    - [Peterson Consumidor](#peterson-consumidor)
    - [Punteros con Islas](#punteros-con-islas)
    - [Simulacion de Particiones](#simulacion-de-particiones)

### Hola Mundo 

```c 
#include <stdio.h>

int main (){
  
  int a = 4,b = 3;
  int *auxa, **auxaa, ***auxaaa;
  
  auxa = &a; ///Le da la direccion de memoria que tiene a y se la da a guardar al auxaa
  auxaa = &auxa;
  auxaaa = &auxaa;
  
  printf("Valor de auxaaa %p %d \n" , auxaaa , ***auxaaa);
  printf("Localidad de memoria A %p \n" ,&a);
  printf("Localidad de memoria B %p \n" ,&b);
  printf("Hola mundo \n");
  

return 0;

}
```

### Proceso 1

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    
    pid_t pid = fork();
    
    if(pid == 0){
       
       //Proceso hijo
       printf("Proceso hijo con PID : %d\n" , getpid());
       
    }else {
        //Proceso padre
        wait(NULL); //Esperar a que el proceso hijo termine
        printf("Proceso padre con PID : %d, el hijo termino \n", getpid());
    }
    
    return 0;
    
}
```

### Cola de Prioridad 

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _nodo {
    int valor;
    struct _nodo *siguiente;
} nodo;

// Genera un nuevo proceso con un identificador único
nodo* genera_proceso() {
    static int contador = 1;
    nodo* nuevo = (nodo*)malloc(sizeof(nodo));
    if (nuevo == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }
    nuevo->valor = contador++;
    nuevo->siguiente = NULL;
    return nuevo;
}

// Inserta un nuevo nodo al final de la lista
void insertar_final(nodo** cabeza) {
    nodo* nuevoNodo = genera_proceso();

    if (*cabeza == NULL) {
        *cabeza = nuevoNodo;
    } else {
        nodo* temp = *cabeza;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoNodo;
    }
}

// Imprime la lista de procesos en orden
void imprimir_lista(nodo* cabeza) {
    nodo* temp = cabeza;
    while (temp != NULL) {
        printf("Proceso %d -> ", temp->valor);
        temp = temp->siguiente;
    }
    printf("NULL\n");
}

// Atiende el primer proceso en la cola y lo elimina de la lista
void atender_proceso(nodo** cabeza) {
    if (*cabeza == NULL) {
        printf("No hay procesos para atender.\n");
        return;
    }

    nodo* temp = *cabeza;
    printf("Atendiendo proceso %d\n", temp->valor);
    *cabeza = temp->siguiente;
    free(temp);
}

int main() {
    nodo* cabeza = NULL;
    int op = 0;

    do {
        printf("1. Genera proceso\n");
        printf("2. Atiende proceso\n");
        printf("3. Mostrar Lista de Procesos\n");
        printf("4. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                insertar_final(&cabeza);
                break;
            case 2:
                atender_proceso(&cabeza);
                break;
            case 3:
                imprimir_lista(cabeza);
                break;
            case 4:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida\n");
        }
    } while (op != 4);

    return 0;
}
```

### Funciones Recursiva

``` c 
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// Función para la multiplicación usando solo incrementos y decrementos en un proceso hijo
int multiplicacion(int a, int b) {
    int resultado = 0;

    // Añadimos 'a' a 'resultado' en 'b' iteraciones
    for (int i = 0; i < b; i++) {
        int temp = a;
        while (temp > 0) { // Incrementamos resultado mientras temp decrementa a 0
            resultado++;
            temp--;
        }
    }
    return resultado;
}

// Función para la división usando solo incrementos y decrementos en un proceso hijo
int division(int a, int b) {
    if (b == 0) {
        printf("Error: División por cero.\n");
        return -1;
    }

    int cociente = 0;
    while (a >= b) { // Restamos 'b' de 'a' hasta que 'a' sea menor que 'b'
        int temp = b;
        while (temp > 0) {
            a--;
            temp--;
        }
        cociente++;
    }
    return cociente;
}

int main() {
    int opcion, a, b, resultado;
    pid_t pid;

    printf("Seleccione una operación:\n");
    printf("1. Multiplicación\n");
    printf("2. División\n");
    printf("Opción: ");
    scanf("%d", &opcion);

    printf("Ingrese el primer número: ");
    scanf("%d", &a);
    printf("Ingrese el segundo número: ");
    scanf("%d", &b);

    if (opcion == 1 || opcion == 2) {
        pid = fork(); // Crea un proceso hijo

        if (pid < 0) {
            printf("Error al crear el proceso.\n");
            return 1;
        }

        if (pid == 0) {  // Proceso hijo
            if (opcion == 1) {
                resultado = multiplicacion(a, b);
                printf("Resultado de la multiplicación: %d\n", resultado);
            } else if (opcion == 2) {
                resultado = division(a, b);
                printf("Resultado de la división: %d\n", resultado);
            }
            _exit(0); // Salir del proceso hijo
        } else { // Proceso padre
            wait(NULL); // Espera a que el proceso hijo termine
        }
    } else {
        printf("Opción inválida.\n");
    }

    return 0;
}

```

### Hilos 

```c
#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_PROCESOS 10

typedef struct _nodo {
    int valor;
    int prioridad;
    struct _nodo *siguiente;
} nodo;

nodo* cabeza = NULL;

int turno[2];
int eligiendo[2] = {0, 0};

int numero_aleatorio() {
    return (rand() % 4) + 1;
}

nodo* genera_proceso(int id) {
    nodo* nuevo = (nodo*)malloc(sizeof(nodo));
    nuevo->valor = id;
    nuevo->prioridad = numero_aleatorio();
    nuevo->siguiente = NULL;
    return nuevo;
}

int max_turno() {
    return turno[0] > turno[1] ? turno[0] : turno[1];
}

void* productor(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; i++) {
        eligiendo[0] = 1;
        turno[0] = max_turno() + 1;
        eligiendo[0] = 0;

        while (eligiendo[1]);
        while (turno[1] != 0 && (turno[1] < turno[0] || (turno[1] == turno[0] && 1 < 0)));

        nodo* nuevo = genera_proceso(id * 10 + i);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        printf("Productor %d generó proceso %d con prioridad %d\n", id, nuevo->valor, nuevo->prioridad);

        turno[0] = 0;
        sleep(1);
    }
    return NULL;
}

void* consumidor(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; i++) {
        eligiendo[1] = 1;
        turno[1] = max_turno() + 1;
        eligiendo[1] = 0;

        while (eligiendo[0]);
        while (turno[0] != 0 && (turno[0] < turno[1] || (turno[0] == turno[1] && 0 < 1)));

        if (cabeza != NULL) {
            nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            printf("Consumidor %d atendió proceso %d con prioridad %d\n", id, temp->valor, temp->prioridad);
            free(temp);
        } else {
            printf("No hay procesos para consumir\n");
        }

        turno[1] = 0;
        sleep(1);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t hilo_productor, hilo_consumidor;
    int id_productor = 1, id_consumidor = 2;

    // Crear los hilos con los argumentos correctos
    pthread_create(&hilo_productor, NULL, productor, &id_productor);
    pthread_create(&hilo_consumidor, NULL, consumidor, &id_consumidor);

    pthread_join(hilo_productor, NULL);
    pthread_join(hilo_consumidor, NULL);

    return 0;
}


```

### Peterson Consumidor

```c 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_PROCESOS 10

typedef struct _nodo {
    int valor;
    int prioridad;
    struct _nodo *siguiente;
} nodo;

nodo* cabeza = NULL;

int turno = 0;
int interesado[2] = {0, 0};
int numero_aleatorio() {

    return (rand() % 4) + 1;
}

nodo* genera_proceso(int id) {
    nodo* nuevo = (nodo*)malloc(sizeof(nodo));
    nuevo->valor = id;
    nuevo->prioridad = numero_aleatorio();
    nuevo->siguiente = NULL;
    return nuevo;
}

void* productor(void* arg) {
    int id = *(int*)arg;
    
    for (int i = 0; i < 5; i++) {
        interesado[0] = 1;
        turno = 1;
        
        while (interesado[1] && turno == 1);
        nodo* nuevo = genera_proceso(id * 10 + i);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        printf("Productor %d generó proceso %d con prioridad %d\n", id, nuevo->valor, nuevo->prioridad);
        interesado[0] = 0;
        sleep(1);
    }
    return NULL;
}

void* consumidor(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; i++) {
        interesado[1] = 1;
        turno = 0;
        while (interesado[0] && turno == 0);

        if (cabeza != NULL) {
            nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            printf("Consumidor %d atendió proceso %d con prioridad %d\n", id, temp->valor, temp->prioridad);
            free(temp);

        } else {
            printf("No hay procesos para consumir\n");
        }

        interesado[1] = 0;
        sleep(1);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t hilo_productor, hilo_consumidor;
    int id_productor = 1, id_consumidor = 2;

    pthread_create(&hilo_productor, NULL, productor, &id_productor);
    pthread_create(&hilo_consumidor, NULL, consumidor, &id_consumidor);

    pthread_join(hilo_productor, NULL);
    pthread_join(hilo_consumidor, NULL);

    return 0;
}


```

### Punteros con Islas 

```c
#include <stdio.h>
#include <stdbool.h>

#define ROWS 6
#define COLS 8

// Movimientos para las 8 direcciones (horizontal, vertical, diagonal)
int rowDirs[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int colDirs[] = {-1, 0, 1, -1, 1, -1, 0, 1};

// Comprueba si la posición (x, y) es válida
bool isValid(int x, int y, int visited[ROWS][COLS], int grid[ROWS][COLS]) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS && grid[x][y] == 1 && !visited[x][y]);
}

// DFS para marcar todas las celdas conectadas como visitadas
void dfs(int x, int y, int visited[ROWS][COLS], int grid[ROWS][COLS]) {
    visited[x][y] = 1;
    for (int i = 0; i < 8; i++) {
        int newX = x + rowDirs[i];
        int newY = y + colDirs[i];
        if (isValid(newX, newY, visited, grid)) {
            dfs(newX, newY, visited, grid);
        }
    }
}

// Función para contar islas en la matriz
int countIslands(int grid[ROWS][COLS]) {
    int visited[ROWS][COLS] = {0};
    int count = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j] == 1 && !visited[i][j]) {
                dfs(i, j, visited, grid);
                count++;
            }
        }
    }
    return count;
}

int main() {
    int grid[ROWS][COLS] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 1},
        {0, 1, 1, 0, 0, 1, 0, 1},
        {0, 1, 1, 0, 0, 1, 0, 1},
        {0, 0, 0, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    printf("Número de islas: %d\n", countIslands(grid));

    return 0;
}

```

### Simulacion de Particiones 

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PARTICIONES 100
#define MAX_PROCESOS 100
                        ////////QUE FUNCIONES DINAMICAMENTE///////////
typedef struct {
    int tamano;
    int id_de_proceso; // -1 si el proceso no es asignado
} Particion;

void imprime_memoria(Particion Particions[], int numero_de_particiones) {
    printf("\nEstado de la memoria:\n");
    for (int i = 0; i < numero_de_particiones; i++) {
        if (Particions[i].id_de_proceso == -1) {
            printf("Partición %d: %d KB (Libre)\n", i + 1, Particions[i].tamano);
        } else {
            printf("Partición %d: %d KB (Proceso %d)\n", i + 1, Particions[i].tamano, Particions[i].id_de_proceso);
        }
    }
    printf("\n");
}

int main() {
    int memoria_total, numero_de_particiones;
    Particion Particions[MAX_PARTICIONES];

    // Solicitar tamaño total de la memoria
    printf("Ingrese el tamaño total de la memoria (KB): ");
    scanf("%d", &memoria_total);

    // Solicitar número de particiones
    printf("Ingrese el número de particiones: ");
    scanf("%d", &numero_de_particiones);

    if (numero_de_particiones > MAX_PARTICIONES) {
        printf("Número máximo de particiones excedido (%d).\n", MAX_PARTICIONES);
        return 1;
    }

    // Solicitar tamaños de las particiones
    int tamano_total_particion = 0;
    for (int i = 0; i < numero_de_particiones; i++) {
        printf("Ingrese el tamaño de la partición %d (KB): ", i + 1);
        scanf("%d", &Particions[i].tamano);
        Particions[i].id_de_proceso = -1; // Inicialmente, todas están libres
        tamano_total_particion += Particions[i].tamano;
    }

    if (tamano_total_particion > memoria_total) {
        printf("Error: El tamaño total de las particiones excede el tamaño de la memoria.\n");
        return 1;
    }

    int op;
    do {
        printf("\n--- Menú ---\n");
        printf("1. Asignar proceso\n");
        printf("2. Liberar proceso\n");
        printf("3. Mostrar estado de la memoria\n");
        printf("4. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &op);

        switch (op) {
            case 1: {
                int id_de_proceso, tam_proceso;
                printf("Ingrese el ID del proceso: ");
                scanf("%d", &id_de_proceso);
                printf("Ingrese el tamaño del proceso (KB): ");
                scanf("%d", &tam_proceso);

                int asignado = 0;
                for (int i = 0; i < numero_de_particiones; i++) {
                    if (Particions[i].id_de_proceso == -1 && Particions[i].tamano >= tam_proceso) {
                        Particions[i].id_de_proceso = id_de_proceso;
                        asignado = 1;
                        printf("Proceso %d asignado a la partición %d.\n", id_de_proceso, i + 1);
                        break;
                    }
                }
                if (!asignado) {
                    printf("No se encontró una partición disponible para el proceso %d.\n", id_de_proceso);
                }
                break;
            }
            case 2: {
                int id_de_proceso;
                printf("Ingrese el ID del proceso a liberar: ");
                scanf("%d", &id_de_proceso);

                int libre = 0;
                for (int i = 0; i < numero_de_particiones; i++) {
                    if (Particions[i].id_de_proceso == id_de_proceso) {
                        Particions[i].id_de_proceso = -1;
                        libre = 1;
                        printf("Proceso %d liberado de la partición %d.\n", id_de_proceso, i + 1);
                        break;
                    }
                }
                if (!libre) {
                    printf("No se encontró el proceso %d en ninguna partición.\n", id_de_proceso);
                }
                break;
            }
            case 3:
                imprime_memoria(Particions, numero_de_particiones);
                break;
            case 4:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (op != 4);

    return 0;
}

```