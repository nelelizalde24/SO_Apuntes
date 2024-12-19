# Mi Repositorio 

[Repositorio en GitHub](https://github.com/nelelizalde24/SO_Apuntes.git)

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
    - [Problema de los Filósofos](#problema-de-los-filósofos)
    - [Actividad](#actividad)
      - [**OBJETIVO**](#objetivo)
      - [**¿Qué se espera de ti?**](#qué-se-espera-de-ti)
      - [**Administración de Memoria**](#administración-de-memoria)
      - [**Administración de Entrada/Salida**](#administración-de-entradasalida)
    - [Actividades: Dispositivos de entrada y salida en Linux](#actividades-dispositivos-de-entrada-y-salida-en-linux)
    - [Comandos de Entrada y Salida, Discos y Archivos](#comandos-de-entrada-y-salida-discos-y-archivos)
    - [**Actividad Final**](#actividad-final)
      - [**Sistemas de Archivos**](#sistemas-de-archivos)
      - [**Protección y Seguridad**](#protección-y-seguridad)

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
[Ejecuta el programa aquí](https://ideone.com)
![codigo ejecutado](img/holamundo.png)


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
[Ejecuta el programa aquí](https://ideone.com)
![codigo ejecutado](img/proceso1.png)


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
[Ejecuta el programa aquí](https://ideone.com)
![codigo ejecutado](img/colaprioridad.png)



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
[Ejecuta el programa aquí](https://ideone.com)
![codigo ejecutado](img/funcionesrecursivas.png)

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
[Ejecuta el programa aquí](https://ideone.com)
![codigo ejecutado](img/hilos.png)

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
[Ejecuta el programa aquí](https://ideone.com)
![codigo ejecutado](img/petersonConsumidor.png)

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
[Ejecuta el programa aquí](https://ideone.com)
![codigo ejecutado](img/islas.png)

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
[Ejecuta el programa aquí](https://ideone.com)
![codigo ejecutado](img/pasrticionesSimulacion.png)

---

### Problema de los Filósofos



Cinco filósofos se sientan alrededor de una mesa y pasan su vida cenando y pensando. Cada filósofo tiene un plato de fideos y un tenedor a la izquierda de su plato. Para comer los fideos son necesarios dos tenedores y cada filósofo sólo puede tomar los que están a su izquierda y derecha. Si cualquier filósofo toma un tenedor y el otro está ocupado, se quedará esperando, con el tenedor en la mano, hasta que pueda tomar el otro tenedor, para luego empezar a comer. Si dos filósofos adyacentes intentan tomar el mismo tenedor a una vez, se produce una condición de carrera: ambos compiten por tomar el mismo tenedor, y uno de ellos se queda sin comer. Si todos los filósofos toman el tenedor que está a su derecha al mismo tiempo, entonces todos se quedarán esperando eternamente, porque alguien debe liberar el tenedor que les falta. Nadie lo hará porque todos se encuentran en la misma situación (esperando que alguno deje sus tenedores). Entonces los filósofos se morirán de hambre. El problema consiste en encontrar un algoritmo que permita que los filósofos nunca se mueran de hambre.



---
### Actividad 

#### **OBJETIVO**

El objetivo de estas actividades es fortalecer tu comprensión de dos áreas
fundamentales en sistemas operativos: la administración de memoria y la
administración de entrada/salida (E/S). Estos conceptos son esenciales para
entender cómo un sistema operativo gestiona los recursos de hardware y soft-
ware para garantizar el funcionamiento eficiente de un equipo de cómputo.


#### **¿Qué se espera de ti?**

1. **Lectura y análisis crítico:**

   1.1 Antes de comenzar las actividades, revisa los temas relacionados
   en tu material de estudio o en recursos confiables. Asegúrate
   de comprender los conceptos clave, como memoria virtual, pagi-
   nación, dispositivos de bloque y planificación de discos.

   1.2 Reflexiona sobre cómo estos conceptos son aplicados en sistemas
   operativos modernos como Linux o Windows.

2. **Desarrollo práctico:**

    2.1 Implementarás programas en lenguajes como C o Python para
    simular y profundizar en los mecanismos estudiados. Esto in-
    cluye la creación de algoritmos de administración de memoria y
    simuladores de entrada/salida.

    2.2 Al realizar estas tareas, te familiarizarás con técnicas de pro-
    gramación orientadas a sistemas, como el manejo de memoria
    dinámica, la gestión de interrupciones y la planificación de recur-
    sos.

3. **Resolución de problemas:**

    3.1 Las actividades incluyen preguntas teóricas y prácticas diseñadas
    para retarte a analizar problemas reales y proponer soluciones
    innovadoras.

    3.2 Se espera que simules entornos de sistemas operativos y explores
    cómo interactúan sus componentes clave.


#### **Administración de Memoria**

1. **Política y filosofía**
   
    1.1 ¿Cuál es la diferencia entre fragmentación interna y    externa? Explica
    cómo cada una afecta el rendimiento de la memoria.

    Respuesta : En ambos casos, el desperdicio de memoria reduce la eficiencia del sistema, pero la fragmentación externa tiende a ser más problemática en sistemas que requieren bloques grandes y contiguos.

    | **Aspecto**              | **Fragmentación Interna**                      | **Fragmentación Externa**                      |
    |--------------------------|-----------------------------------------------|-----------------------------------------------|
    | **Causa principal**       | Bloques asignados más grandes de lo necesario. | Espacio libre dividido en pequeños fragmentos. |
    | **Memoria desaprovechada**| Dentro de los bloques asignados.               | Entre los bloques asignados.                  |
    | **Soluciones**            | Ajuste del tamaño de los bloques.             | Compactación, paginación o segmentación.      |


    1.2 Investiga y explica las políticas de reemplazo de páginas en sistemas
    operativos. ¿Cuál consideras más eficiente y por qué?

    Respuesta : 
    En general, la política más eficiente depende del contexto. Para entornos con recursos suficientes, LRU es preferible, mientras que Clock es más adecuado para sistemas con limitaciones de hardware.

2. **Memoria real**
   
    2.1 Escribe un programa en C o Python que simule la administración de
    memoria mediante particiones fijas.

    Respuesta : 

    ```c 
    #include <stdio.h>
    #include <stdbool.h>

    #define MAX_PARTITIONS 10
    #define MAX_PROCESSES 10

    typedef struct {
        int size;
        bool is_free;
        int process_id;
    } Partition;

    typedef struct {
        int id;
        int size;
    } Process;

    // Inicialización de particiones de memoria
    void initialize_partitions(Partition partitions[], int sizes[], int count) {
        for (int i = 0; i < count; i++) {
            partitions[i].size = sizes[i];
            partitions[i].is_free = true;
            partitions[i].process_id = -1; // -1 indica que no hay proceso asignado
        }
    }

    // Asignar un proceso a una partición
    bool allocate_process(Partition partitions[], int partition_count, Process process) {
        for (int i = 0; i < partition_count; i++) {
            if (partitions[i].is_free && partitions[i].size >= process.size) {
                partitions[i].is_free = false;
                partitions[i].process_id = process.id;
                printf("Proceso %d asignado a partición de tamaño %d.\n", process.id, partitions[i].size);
                return true;
            }
        }
        printf("Proceso %d no pudo ser asignado.\n", process.id);
        return false;
    }

    // Liberar una partición
    bool deallocate_process(Partition partitions[], int partition_count, int process_id) {
        for (int i = 0; i < partition_count; i++) {
            if (!partitions[i].is_free && partitions[i].process_id == process_id) {
                partitions[i].is_free = true;
                partitions[i].process_id = -1;
                printf("Proceso %d liberado de partición de tamaño %d.\n", process_id, partitions[i].size);
                return true;
            }
        }
        printf("Proceso %d no encontrado en memoria.\n", process_id);
        return false;
    }

    // Mostrar el estado de la memoria
    void display_memory(Partition partitions[], int partition_count) {
        printf("\nEstado de la memoria:\n");
        for (int i = 0; i < partition_count; i++) {
            if (partitions[i].is_free) {
                printf("Partición %d: Tamaño %d, Libre\n", i + 1, partitions[i].size);
            } else {
                printf("Partición %d: Tamaño %d, Ocupada por Proceso %d\n", i + 1, partitions[i].size, partitions[i].process_id);
            }
        }
        printf("\n");
    }

    int main() {
        // Definición de particiones y procesos
        int partition_sizes[] = {100, 200, 300, 400};
        int partition_count = 4;
        Partition partitions[MAX_PARTITIONS];
        initialize_partitions(partitions, partition_sizes, partition_count);

        Process processes[] = {
            {1, 120}, // Proceso 1: Tamaño 120
            {2, 80},  // Proceso 2: Tamaño 80
            {3, 200}, // Proceso 3: Tamaño 200
            {4, 50}   // Proceso 4: Tamaño 50
        };
        int process_count = 4;

        // Asignar procesos a particiones
        for (int i = 0; i < process_count; i++) {
            allocate_process(partitions, partition_count, processes[i]);
        }

        display_memory(partitions, partition_count);

        // Liberar un proceso
        deallocate_process(partitions, partition_count, 2);

        display_memory(partitions, partition_count);

        // Intentar asignar un proceso que no cabe
        Process large_process = {5, 500};
        allocate_process(partitions, partition_count, large_process);

        display_memory(partitions, partition_count);

        return 0;
    }
    ```


    2.2 Diseña un algoritmo para calcular qué procesos pueden ser asignados
    a un sistema con memoria real limitada utilizando el algoritmo de
    "primera cabida".

    Respuesta : 

    ```c 
    #include <stdio.h>
    #include <stdbool.h>

    #define MAX_PARTITIONS 10
    #define MAX_PROCESSES 10

    typedef struct {
        int size;
        bool is_free;
        int process_id;
    } Partition;

    typedef struct {
        int id;
        int size;
        bool is_assigned;
    } Process;

    // Inicialización de particiones de memoria
    void initialize_partitions(Partition partitions[], int sizes[], int count) {
        for (int i = 0; i < count; i++) {
            partitions[i].size = sizes[i];
            partitions[i].is_free = true;
            partitions[i].process_id = -1; // -1 indica que no hay proceso asignado
        }
    }

    // Asignar procesos utilizando el algoritmo de primera cabida
    void first_fit(Partition partitions[], int partition_count, Process processes[], int process_count) {
        for (int i = 0; i < process_count; i++) {
            processes[i].is_assigned = false; // Inicialmente, el proceso no está asignado
            for (int j = 0; j < partition_count; j++) {
                if (partitions[j].is_free && partitions[j].size >= processes[i].size) {
                    partitions[j].is_free = false;
                    partitions[j].process_id = processes[i].id;
                    processes[i].is_assigned = true;
                    printf("Proceso %d asignado a partición de tamaño %d.\n", processes[i].id, partitions[j].size);
                    break; // Se detiene al encontrar la primera partición adecuada
                }
            }
            if (!processes[i].is_assigned) {
                printf("Proceso %d no pudo ser asignado.\n", processes[i].id);
            }
        }
    }

    // Mostrar el estado de la memoria
    void display_memory(Partition partitions[], int partition_count) {
        printf("\nEstado de la memoria:\n");
        for (int i = 0; i < partition_count; i++) {
            if (partitions[i].is_free) {
                printf("Partición %d: Tamaño %d, Libre\n", i + 1, partitions[i].size);
            } else {
                printf("Partición %d: Tamaño %d, Ocupada por Proceso %d\n", i + 1, partitions[i].size, partitions[i].process_id);
            }
        }
        printf("\n");
    }

    // Mostrar el estado de los procesos
    void display_processes(Process processes[], int process_count) {
        printf("\nEstado de los procesos:\n");
        for (int i = 0; i < process_count; i++) {
            if (processes[i].is_assigned) {
                printf("Proceso %d: Tamaño %d, Asignado\n", processes[i].id, processes[i].size);
            } else {
                printf("Proceso %d: Tamaño %d, No asignado\n", processes[i].id, processes[i].size);
            }
        }
        printf("\n");
    }

    int main() {
        // Definición de particiones y procesos
        int partition_sizes[] = {100, 200, 300, 400};
        int partition_count = 4;
        Partition partitions[MAX_PARTITIONS];
        initialize_partitions(partitions, partition_sizes, partition_count);

        Process processes[] = {
            {1, 120}, // Proceso 1: Tamaño 120
            {2, 80},  // Proceso 2: Tamaño 80
            {3, 200}, // Proceso 3: Tamaño 200
            {4, 50},  // Proceso 4: Tamaño 50
            {5, 500}  // Proceso 5: Tamaño 500 (No cabe)
        };
        int process_count = 5;

        // Asignar procesos usando el algoritmo de primera cabida
        first_fit(partitions, partition_count, processes, process_count);

        // Mostrar el estado de la memoria y los procesos
        display_memory(partitions, partition_count);
        display_processes(processes, process_count);

        return 0;
    }

    ```

3. **Organización de memoria virtual**
   
    3.1 Investiga y explica el concepto de "paginación" y "segmentación".
    ¿Cuáles son las ventajas y desventajas de cada técnica?

    Respuesta :

    **Paginacion :** La memoria se divide en bloques de tamaño fijo llamados páginas (en el espacio lógico del proceso) y marcos (en el espacio físico de la memoria). Cada página de un proceso puede ser mapeada a cualquier marco disponible en la memoria física, lo que permite una asignación no contigua. 

    Ventajas:

    Eficiencia en el uso de memoria: No hay fragmentación externa porque los marcos se llenan completamente.
    
    Asignación no contigua: Las páginas pueden asignarse a marcos no adyacentes, facilitando la gestión de la memoria.
    
    Facilidad para la multitarea: Es más fácil compartir memoria entre procesos porque las páginas pueden mapearse fácilmente.

    Desventajas:

    Fragmentación interna: Si el tamaño de una página no se utiliza completamente, el espacio restante queda desaprovechado.
    
    Sobrecarga de la tabla de páginas: La necesidad de mantener y buscar en tablas de páginas grandes puede consumir memoria adicional y tiempo de procesamiento.
    
    Costo del hardware: Requiere hardware adicional para la traducción de direcciones (como una unidad MMU).

    
    **Segmentacion :** La memoria se divide en bloques lógicos de tamaño variable llamados segmentos, que representan unidades funcionales del programa, como código, datos y pila. Cada segmento tiene un tamaño y propósito definidos.

    Ventajas:

    Representación lógica: Se alinea mejor con la estructura lógica del programa, ya que cada segmento puede representar una parte funcional (p. ej., pila, datos).
    
    Facilidad de crecimiento: Los segmentos pueden crecer o reducirse según las necesidades del programa, si hay espacio disponible.
    
    Seguridad y protección: Cada segmento puede tener permisos específicos, lo que facilita el control de acceso.

    Desventajas:

    Fragmentación externa: Los segmentos de tamaño variable pueden dejar huecos en la memoria cuando se liberan.
    
    Sobrecarga de gestión: Requiere mantener una tabla de segmentos, lo que añade complejidad.
    
    Asignación complicada: Encontrar espacio para un segmento grande puede ser difícil en un sistema con memoria fragmentada.




    3.2 Escribe un programa que simule una tabla de páginas para procesos
    con acceso aleatorio a memoria virtual.

    Respuesta : 

    ```c 
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>

    #define MAX_PAGES 10
    #define MAX_FRAMES 10

    // Estructura para representar una entrada en la tabla de páginas
    typedef struct {
        int page_number;  // Número de la página
        int frame_number; // Número del marco en la memoria física
    } PageTableEntry;

    // Genera un marco aleatorio para una página
    int assign_random_frame(int used_frames[], int total_frames) {
        int frame;
        do {
            frame = rand() % total_frames; // Generar un número de marco aleatorio
        } while (used_frames[frame]); // Verificar si el marco ya está asignado
        used_frames[frame] = 1; // Marcar el marco como usado
        return frame;
    }

    // Imprime la tabla de páginas
    void print_page_table(PageTableEntry page_table[], int page_count) {
        printf("\nTabla de Páginas:\n");
        printf("Página | Marco\n");
        printf("-------|-------\n");
        for (int i = 0; i < page_count; i++) {
            printf("   %d   |   %d\n", page_table[i].page_number, page_table[i].frame_number);
        }
        printf("\n");
    }

    // Traduce una dirección virtual a física usando la tabla de páginas
    void translate_virtual_to_physical(PageTableEntry page_table[], int page_count, int virtual_address, int page_size) {
        int page_number = virtual_address / page_size;
        int offset = virtual_address % page_size;

        if (page_number >= page_count) {
            printf("Error: La dirección virtual %d es inválida (fuera de rango).\n", virtual_address);
            return;
        }

        int frame_number = page_table[page_number].frame_number;
        int physical_address = frame_number * page_size + offset;

        printf("Dirección Virtual: %d -> Dirección Física: %d (Página %d, Marco %d, Offset %d)\n",
            virtual_address, physical_address, page_number, frame_number, offset);
    }

    int main() {
        srand(time(NULL));

        int page_count, frame_count, page_size;
        int used_frames[MAX_FRAMES] = {0}; // Arreglo para rastrear marcos usados

        // Entrada de datos
        printf("Ingrese el número de páginas: ");
        scanf("%d", &page_count);
        if (page_count > MAX_PAGES) {
            printf("Error: El número máximo de páginas es %d.\n", MAX_PAGES);
            return 1;
        }

        printf("Ingrese el número de marcos: ");
        scanf("%d", &frame_count);
        if (frame_count > MAX_FRAMES) {
            printf("Error: El número máximo de marcos es %d.\n", MAX_FRAMES);
            return 1;
        }

        printf("Ingrese el tamaño de cada página (en bytes): ");
        scanf("%d", &page_size);

        PageTableEntry page_table[MAX_PAGES];

        // Asignar marcos aleatorios a cada página
        for (int i = 0; i < page_count; i++) {
            page_table[i].page_number = i;
            page_table[i].frame_number = assign_random_frame(used_frames, frame_count);
        }

        // Imprimir la tabla de páginas
        print_page_table(page_table, page_count);

        // Traducir direcciones virtuales
        int virtual_address;
        while (1) {
            printf("Ingrese una dirección virtual (o -1 para salir): ");
            scanf("%d", &virtual_address);

            if (virtual_address == -1) {
                break;
            }

            translate_virtual_to_physical(page_table, page_count, virtual_address, page_size);
        }

        printf("Simulación terminada.\n");
        return 0;
    }

    ```


4. **Administración de memoria virtual**    

    
    4.1 IEscribe un código que implemente el algoritmo de reemplazo de página
    "Least Recently Used" (LRU).

        Respuesta : 


    4.2 Diseña un diagrama que represente el proceso de traducción de direc-
    ciones virtuales a físicas en un sistema con memoria virtual.

        Respuesta :

**Integración**

 1. Analiza un sistema operativo moderno (por ejemplo, Linux o Windows) e identifica cómo administra la memoria virtual.

        Respuesta :




 2. Realiza una simulación en cualquier lenguaje de programación que emule el swapping de procesos en memoria virtual.
        
        Respuesta :


#### **Administración de Entrada/Salida**

1. **Dispositivos y manejadores de dispositivos**
   
     1.1 Explica la diferencia entre dispositivos de bloque y dispositivos de
     carácter. Da un ejemplo de cada uno.

         Respuesta :

     1.2 Diseña un programa que implemente un manejador de dispositivos sen-
     cillo para un dispositivo virtual de entrada.

         Respuesta :


2. **Mecanismos y funciones de los manejadores de dispositivos**
   
     2.1 Investiga qué es la interrupción por E/S y cómo la administra el sis-
     tema operativo. Escribe un ejemplo en pseudocódigo para simular este
     proceso.  

          Respuesta :

    2.2 Escribe un programa que utilice el manejo de interrupciones en un
     sistema básico de simulación.

          Respuesta :

3.  **Estructuras de datos para manejo de dispositivos**
   
     3.1 Investiga y explica qué es una cola de E/S. Diseña una simulación de
     una cola con prioridad.

           Respuesta :

     3.2 Escribe un programa que simule las operaciones de un manejador de
     dispositivos utilizando una tabla de estructuras.

           Respuesta :

4. **Operaciones de Entrada/Salida**

     4.1 Diseña un flujo que describa el proceso de lectura de un archivo desde
     un disco magnético. Acompáñalo con un programa básico que simule
     el proceso.

           Respuesta :

     4.2 Implementa un programa en Python, C o java que realice operaciones
     de entrada/salida asíncronas usando archivos.

           Respuesta :

**Integración**

1. Escribe un programa que implemente el algoritmo de planificación de
discos "Elevator (SCAN)".

         Respuesta :

2. Diseña un sistema que maneje múltiples dispositivos simulados (disco
duro, impresora, teclado) y muestra cómo se realiza la comunicación
entre ellos.

         Respuesta :

**Avanzados**

1. Explica cómo los sistemas operativos modernos optimizan las opera-
ciones de entrada/salida con el uso de memoria caché.

         Respuesta :

---


### Actividades: Dispositivos de entrada y salida en Linux

- **Introducción**

En este ejercicio, aprenderá a listar, verificar y analizar los dispositivos de entrada y salida en Linux. Usarán comandos básicos y herramientas comunes disponibles en cualquier distribución.

- **Actividad 1: Listar dispositivos conectados**

1. Objetivo

Conocer los dispositivos de entrada y salida conectados al sistema.

2. Instrucciones

    1. Abra una terminal en su entorno Linux.
    2. Ejecute los siguientes comandos y anote sus observaciones:
        `lsblk`: Enumera los dispositivos de bloque.
        
        `lsusb`: Lista los dispositivos conectados a los puertos USB.
        
        `lspci`: Muestra los dispositivos conectados al bus PCI.
        
        `dmesg | grep usb`: Muestra los mensajes del kernel relacionados con dispositivos USB.
    3. Conteste:
        - ¿Qué tipos de dispositivos se muestran en la salida de `lsblk`?
            
              En resumen, los dispositivos listados son bucles (loop), discos físicos (disk), particiones (part), y un dispositivo de solo lectura (rom).

        - ¿Cuál es la diferencia entre `lsusb` y `lspci`?

              lsusb: Muestra periféricos externos.

              lspci: Muestra tarjetas de expansión y controladoras.

        - ¿Qué información adicional proporciona `dmesg | grep usb`?
  
               De echo el comando seria asi para que te deje dar permiso: sudo dmesg | grep usb

               El comando te proporciona un registro detallado de todos los eventos de interacción con dispositivos USB en el sistema, desde la detección hasta la manipulación del dispositivo (como el almacenamiento masivo).
               

- **Actividad 2: Verificar dispositivos de almacenamiento**

1. Objetivo

Aprender cómo identificar discos duros, particiones y su configuración.

2. Instrucciones

    1. Use el comando `fdisk -l` para listar todos los discos y particiones.
    2. Utilice `blkid` para ver los identificadores UUID y los tipos de sistema de archivos.
    3. Use `df -h` para listar los dispositivos montados y su espacio disponible.
    4. Conteste:
        - ¿Qué dispositivos de almacenamiento están conectados a su sistema?
        
              Dispositivo: /dev/sda (maquina virtual)

              Dispositivo: /dev/sdb (usb)

        - ¿Qué particiones están montadas actualmente?
        
               S.ficheros     Tamaño Usados  Disp Uso% Montado en
                tmpfs            391M   1,7M  390M   1% /run
                /dev/sda2         25G    15G  8,3G  65% /
                tmpfs            2,0G    40M  1,9G   2% /dev/shm
                tmpfs            5,0M   8,0K  5,0M   1% /run/lock
                tmpfs            391M   136K  391M   1% /run/user/1000
                /dev/sdb1         15G   6,6G  8,0G  45% /mnt/nueva_particion

        
        - ¿Qué tipo de sistemas de archivos se usan en las particiones?
  
              ext4 en las particiones de discos físicos (/dev/sda2 y /dev/sdb1).

              tmpfs en las particiones de espacio temporal en memoria.

- **Actividad 3: Explorar dispositivos de entrada**

1. Objetivo

Identificar dispositivos como teclados, ratones y cámaras.

2. Instrucciones

    1. Ejecute `cat /proc/bus/input/devices` para listar los dispositivos de entrada.
    2. Use `evtest` para monitorear eventos de dispositivos de entrada (requiere permisos de superusuario).
    3. Investigue los siguientes dispositivos:
        - Teclado
        - Mouse
        - Controladores USB adicionales
    4. Conteste:
        - ¿Qué eventos genera cada dispositivo al interactuar con ellos?
        
              Teclado: Generalmente genera eventos EV_KEY para las teclas presionadas y liberadas, y EV_SYN para sincronizar los eventos.
              Mouse: Genera eventos EV_REL (movimiento del puntero) y EV_KEY (clics de botones).
              Controladores USB adicionales: Pueden generar eventos EV_ABS (movimiento de ejes) y EV_KEY (presión de botones), dependiendo del tipo de controlador (por ejemplo, un gamepad).
        
        - ¿Cómo se identifican los dispositivos en `/proc/bus/input/devices`?

              Los dispositivos de entrada se identifican por el campo Name="...", que describe el dispositivo (por ejemplo, "USB Keyboard", "USB Mouse"). También puedes ver el campo Handlers=, que indica el tipo de controlador utilizado por el dispositivo (por ejemplo, kbd para un teclado o mouse para un ratón). Además, el campo Bus=... muestra el tipo de bus (por ejemplo, USB, PS/2).


**Actividad 4: Examinar dispositivos de salida**

1. Objetivo

Entender cómo identificar dispositivos de salida como monitores y tarjetas de sonido.

2. Instrucciones

    1. Use `xrandr` para listar las pantallas conectadas y sus resoluciones.
    2. Ejecute `aplay -l` para listar las tarjetas de sonido disponibles.
    3. Use `lsof /dev/snd/*` para ver qué procesos están utilizando la tarjeta de sonido.
    4. Conteste:
        - ¿Qué salidas de video están disponibles en su sistema?
        
              La salida de video disponible en tu sistema es Virtual-1, con una resolución actual de 1920x966. También tiene varias resoluciones adicionales disponibles, como 1280x960, 1152x864, 1024x768, etc.
        
        - ¿Qué dispositivos de sonido se detectaron?
        
              Se detectó la tarjeta de sonido Intel 82801AA-ICH, con el dispositivo Intel ICH.
        
        - ¿Qué procesos están usando la tarjeta de sonido?

               Los procesos que están utilizando la tarjeta de sonido son:

                pipewire (PID 2738), que está usando el dispositivo /dev/snd/seq.

                wireplumb (PID 2742), que está utilizando el dispositivo /dev/snd/controlC0.

**Actividad 5: Crear un script de resumen**

1. Objetivo

Automatizar la recopilación de información de dispositivos de entrada y salida.

2. Instrucciones

    1. Cree un archivo llamado `dispositivos.sh` y agregue el siguiente contenido: ```bash #!/bin/bash echo "Dispositivos de bloque:" lsblk echo "Dispositivos USB:" lsusb echo "Dispositivos PCI:" lspci echo "Dispositivos de entrada:" cat /proc/bus/input/devices echo "Salidas de video:" xrandr echo "Tarjetas de sonido:" aplay -l ```
    2. Ejecute el script usando `bash dispositivos.sh`.
    3. Modifique el script para guardar la salida en un archivo llamado `resumendispositivos.txt`.
    
         [resumendispositivos.txt](resumendispositivos.txt)
    
    4. Conteste:
        - ¿Qué ventajas tiene usar un script para recopilar esta información?
        
                Usar un script para recopilar esta información tiene varias ventajas:

                Automatización: Puedes ejecutar el script una y otra vez sin tener que escribir comandos manualmente cada vez, lo que ahorra tiempo.
                
                Repetibilidad: Si necesitas recopilar la misma información más tarde o en múltiples máquinas, puedes ejecutar el mismo script y obtener resultados consistentes.
                
                Almacenamiento de resultados: Puedes redirigir la salida del script a un archivo de texto, como hiciste con resumendispositivos.txt, lo que facilita el análisis posterior.
                
                Consistencia: No hay margen para olvidarse de ejecutar algún comando o cometer errores al escribir comandos manualmente.
        
        - ¿Qué cambios realizaría para personalizar el script?

                Para personalizar aún más el script, podrías realizar los siguientes cambios:

                Agregar más información: Si quieres más detalles sobre los dispositivos o servicios, puedes añadir más comandos al script, como lsusb -v para más detalles sobre los dispositivos USB o lscpu para información detallada sobre la CPU.
                
                Formatear la salida: Podrías agregar algunas instrucciones de formato, como encabezados más claros, para que los resultados sean más fáciles de leer, por ejemplo, usando echo "-----".
                
                Filtrar resultados: Si no te interesa la salida completa de algún comando, puedes usar grep para filtrar la información relevante. Por ejemplo, si solo te interesa ver las tarjetas de sonido activas, podrías usar aplay -l | grep -i "card" para filtrar las líneas que contienen "card".
                
                Agregar fecha al archivo: Si ejecutas el script con frecuencia, podrías agregar la fecha al nombre del archivo para no sobrescribir los resultados anteriores. Por ejemplo:
                echo "Información recopilada el $(date)" >> "resumendispositivos_$(date +'%Y%m%d_%H%M').txt"




**Actividad 6: Reflexión y discusión**

1. Objetivo

Analizar la importancia del manejo de dispositivos en sistemas Linux.

2. Instrucciones

    1. Reflexione sobre lo aprendido y discuta en equipo:
        - ¿Qué comando encontró más útil y por qué?
        - ¿Qué tan importante es conocer los dispositivos conectados al sistema?
        - ¿Cómo podrían estos conocimientos aplicarse en la administración de sistemas?

---


### Comandos de Entrada y Salida, Discos y Archivos

- ####  **Ejercicio 1: Montar y Desmontar Discos**

   - Objetivo: Aprender a montar y desmontar un dispositivo externo.
    Inserta una memoria USB en el sistema.

   - Encuentra el dispositivo usando el comando:

         lsblk

       o

         fdisk -l

    - Monta la memoria USB en un directorio, por ejemplo, `/mnt/usb`:

          sudo mount /dev/sdX1 /mnt/usb

    - Verifica que esté montado correctamente:

          df -h

    - Copia un archivo desde tu directorio personal al dispositivo USB:

          cp archivo.txt /mnt/usb/

    - Desmonta la memoria USB:

          sudo umount /mnt/usb

    - ***RESULTADO***

        `nelosn@nelosn-lenovo:~$ lsblk` "Veo mi dispositivo usb"

            NAME   MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS
            loop0    7:0    0     4K  1 loop /snap/bare/5
            loop1    7:1    0  73,9M  1 loop /snap/core22/1722
            loop2    7:2    0 270,7M  1 loop /snap/firefox/4259
            loop3    7:3    0  73,9M  1 loop /snap/core22/1663
            loop4    7:4    0   273M  1 loop /snap/firefox/5273
            loop5    7:5    0  11,1M  1 loop /snap/firmware-updater/147
            loop6    7:6    0  10,7M  1 loop /snap/firmware-updater/127
            loop7    7:7    0   497M  1 loop /snap/gnome-42-2204/141
            loop8    7:8    0 505,1M  1 loop /snap/gnome-42-2204/176
            loop9    7:9    0  91,7M  1 loop /snap/gtk-common-themes/1535
            loop10   7:10   0  10,7M  1 loop /snap/snap-store/1218
            loop11   7:11   0  12,2M  1 loop /snap/snap-store/1216
            loop12   7:12   0  38,8M  1 loop /snap/snapd/21759
            loop13   7:13   0  44,3M  1 loop /snap/snapd/23258
            loop14   7:14   0   476K  1 loop /snap/snapd-desktop-integration/157
            loop15   7:15   0   568K  1 loop /snap/snapd-desktop-integration/253
            sda      8:0    0    25G  0 disk 
            ├─sda1   8:1    0     1M  0 part 
            └─sda2   8:2    0    25G  0 part /var/snap/firefox/common/host-hunspell
                                            /
            sdb      8:16   1  14,5G  0 disk 
            └─sdb1   8:17   1  14,5G  0 part /media/nelosn/ESD-ISO
            sr0     11:0    1  1024M  0 rom  

        `nelosn@nelosn-lenovo:~$ sudo mount /dev/sdb1 /mnt/usb` "Monto la usb a un directorio"
            
        `nelosn@nelosn-lenovo:~$ df -h` "Veo mi usb se haya montado bien"

            S.ficheros     Tamaño Usados  Disp Uso% Montado en
            tmpfs            391M   1,6M  390M   1% /run
            /dev/sda2         25G    15G  8,7G  63% /
            tmpfs            2,0G    35M  1,9G   2% /dev/shm
            tmpfs            5,0M   8,0K  5,0M   1% /run/lock
            tmpfs            391M   136K  391M   1% /run/user/1000
            /dev/sdb1         15G   7,3G  7,4G  50% /mnt/usb

        `nelosn@nelosn-lenovo:~$ touch ~/archivo.txt` "Creo un .txt en mi directorio presonal"

        `nelosn@nelosn-lenovo:~$ ls` "Veo que se haya creado el archivo"

            archivo.txt  Documentos  Imágenes  Plantillas  snap
            Descargas    Escritorio  Música    Público     Vídeos

        `nelosn@nelosn-lenovo:~$ cp archivo.txt /mnt/usb/` "Copio el archivo y lo paso a la usb"

        `nelosn@nelosn-lenovo:~$ ls /mnt/usb` "Veo el archivo en mi usb"

            acta.pdf
            archivo.txt
            CartaCompromisoLaboral.pdf
            'comprobante enero-junio.pdf'
            CURP_EIJN020224HMNLMLA5.pdf
            curso
            'El grinch animado.mp4'
            'ELECTRONICA DIGITAL.pptx'
            'ERS simplificado.doc'
            estc3a1tica-de-russel-hibbeler-12va-edicic3b3n.pdf
            '_Fundamentos de física Vol. 1, 9na Edición - Raymond A. Serway.pdf'
            'Fundamentos de física Vol. 1, 9na Edición - Raymond A. Serway.pdf'
            Git-2.44.0-64-bit.exe
            'halo ce by the king of MP4'
            imagenes.docx
            'ITMORELIA-IT-AC-001-02 SOLICITUD DE INSCRIPCION (3).doc'
            'ITMORELIA-IT-AC-001-05 CONTRATO CON EL ESTUDIANTE (1) (1).doc'
            jdk-8u301-windows-x64.exe
            'JERRGA DEL CUBO DE RUBIK.docx'
            maceta.pdf
            'Mapa Simulacion 2.0.pdf'
            'Mario Bross la pelicula@ByCasique.mp4'
            'Memoria en Java POO.rar'
            musica
            mysql-installer-community-8.0.30.0.msi
            netbeans-8.2-windows.exe
            'Nueva carpeta'
            'Nueva carpeta (2)'
            'Nueva carpeta (3)'
            paginas.pdf
            'pcb vex.docx'
            postgresql-16.2-1-windows-x64-binaries.zip
            postgresql-16.2-1-windows-x64.exe
            Práctica_2.pdf
            'Principios electricos.pdf'
            protadaPortafolio.pdf
            'Protocolo de Investigacion V4.pdf'
            'Protocolo de Investigacion V5-Final.pdf'
            'Protocolo del Proyecto de Vinculación-V2.docx'
            'Protocolo del Proyecto de Vinculación-V3.pdf'
            'REGLAMENTO DE USO DE LOCKERS ISC.pdf'
            Simio-12.219.22821.zip
            'Solicitud de Proyecto del banco de proyectos.docx'
            StudentExpertFitSetup.exe
            'System Volume Information'
            'talLer IV'
            xampp-windows-x64-8.0.30-0-VS16-installer.exe

        `nelosn@nelosn-lenovo:~$ sudo umount /mnt/usb` "Desmonto mi usb"

        `nelosn@nelosn-lenovo:~$ df -h` "Veo que se haya desmontado la usb"

            S.ficheros     Tamaño Usados  Disp Uso% Montado en
            tmpfs            391M   1,6M  390M   1% /run
            /dev/sda2         25G    15G  8,7G  63% /
            tmpfs            2,0G    35M  1,9G   2% /dev/shm
            tmpfs            5,0M   8,0K  5,0M   1% /run/lock
            tmpfs            391M   136K  391M   1% /run/user/1000
            /dev/sdb1         15G   7,3G  7,4G  50% /media/nelosn/ESD-ISO

        `nelosn@nelosn-lenovo:~$`



- #### **Ejercicio 2: Redirección de Entrada y Salida**

    - Objetivo: Usar redirección para guardar la salida de comandos en archivos.

    - Lista los archivos de tu directorio actual y guarda el resultado en un archivo `listado.txt`:

            ls -l > listado.txt

    - Muestra el contenido del archivo en la terminal:

            cat listado.txt

    - Añade la fecha actual al final del archivo:

            date >> listado.txt

    - Muestra todo el contenido del archivo nuevamente:

            cat listado.txt


    - ***RESULTADO***

        `nelosn@nelosn-lenovo:~$ ls -l > listado.txt` "Crea y escribe en un archivo mi directorio personal"

        `nelosn@nelosn-lenovo:~$ cat listado.txt` "Veo lo que se escribio"

            total 36
            -rw-rw-r-- 1 nelosn nelosn    0 dic 10 23:57 archivo.txt
            drwxr-xr-x 2 nelosn nelosn 4096 dic  1 22:42 Descargas
            drwxr-xr-x 4 nelosn nelosn 4096 dic  3 17:18 Documentos
            drwxr-xr-x 2 nelosn nelosn 4096 mar 19  2024 Escritorio
            drwxr-xr-x 3 nelosn nelosn 4096 dic  3 17:17 Imágenes
            -rw-rw-r-- 1 nelosn nelosn    0 dic 11 00:22 listado.txt
            drwxr-xr-x 2 nelosn nelosn 4096 mar 19  2024 Música
            drwxr-xr-x 2 nelosn nelosn 4096 mar 19  2024 Plantillas
            drwxr-xr-x 2 nelosn nelosn 4096 mar 19  2024 Público
            drwx------ 6 nelosn nelosn 4096 dic  1 22:08 snap
            drwxr-xr-x 2 nelosn nelosn 4096 mar 19  2024 Vídeos

        `nelosn@nelosn-lenovo:~$ date >> listado.txt` "Lediogo que imprima la fecha y hora en el archivo"

        `nelosn@nelosn-lenovo:~$ cat listado.txt` "Veo lo que escribio"

            total 36
            -rw-rw-r-- 1 nelosn nelosn    0 dic 10 23:57 archivo.txt
            drwxr-xr-x 2 nelosn nelosn 4096 dic  1 22:42 Descargas
            drwxr-xr-x 4 nelosn nelosn 4096 dic  3 17:18 Documentos
            drwxr-xr-x 2 nelosn nelosn 4096 mar 19  2024 Escritorio
            drwxr-xr-x 3 nelosn nelosn 4096 dic  3 17:17 Imágenes
            -rw-rw-r-- 1 nelosn nelosn    0 dic 11 00:22 listado.txt
            drwxr-xr-x 2 nelosn nelosn 4096 mar 19  2024 Música
            drwxr-xr-x 2 nelosn nelosn 4096 mar 19  2024 Plantillas
            drwxr-xr-x 2 nelosn nelosn 4096 mar 19  2024 Público
            drwx------ 6 nelosn nelosn 4096 dic  1 22:08 snap
            drwxr-xr-x 2 nelosn nelosn 4096 mar 19  2024 Vídeos
            mié 11 dic 2024 00:23:21 CST

        `nelosn@nelosn-lenovo:~$ `


- #### **Ejercicio 3: Copiar y Mover Archivos**

    - Objetivo: Practicar copiar y mover archivos y directorios.

    - Crea un archivo de texto llamado `archivo1.txt`:

            echo "Este es un archivo de prueba" > archivo1.txt

    - Copia este archivo a otro directorio, por ejemplo, `/tmp`:

            cp archivo1.txt /tmp/

    - Renombra el archivo copiado a `archivo2.txt` en `/tmp`:

            mv /tmp/archivo1.txt /tmp/archivo2.txt

    - Mueve el archivo `archivo2.txt` de vuelta a tu directorio actual:

            mv /tmp/archivo2.txt .

    - ***RESULTADO***

        `nelosn@nelosn-lenovo:~$ echo "Este es un archivo de prueba" > archivo1.txt` "Ecribe y crea el archivo"

        `nelosn@nelosn-lenovo:~$ ls` "Veo que este en el directorio"

            archivo1.txt  Documentos  Imágenes     Música      Público  Vídeos
            Descargas     Escritorio  listado.txt  Plantillas  snap

        `nelosn@nelosn-lenovo:~$ cp archivo1.txt /tmp/` "Copia el archivo y lo pasa al directorio tmp"

        `nelosn@nelosn-lenovo:~$ ls /tmp` "Compruebo que este"

            archivo1.txt
            snap-private-tmp
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-colord.service-gN5zuy
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-fwupd.service-JW7Vlu
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-ModemManager.service-HGXyxs
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-polkit.service-xXtLq4
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-power-profiles-daemon.service-TzSFXa
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-switcheroo-control.service-8BwyAy
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-systemd-logind.service-Lwf6ZW
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-systemd-oomd.service-LeVvmv
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-systemd-resolved.service-53VrRA
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-upower.service-pxGj51

        `nelosn@nelosn-lenovo:~$  mv /tmp/archivo1.txt /tmp/archivo2.txt` "Reinscribo el nombre del archivo "

        `nelosn@nelosn-lenovo:~$ ls /tmp` "Compruebo "

            archivo2.txt
            snap-private-tmp
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-colord.service-gN5zuy
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-fwupd.service-JW7Vlu
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-ModemManager.service-HGXyxs
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-polkit.service-xXtLq4
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-power-profiles-daemon.service-TzSFXa
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-switcheroo-control.service-8BwyAy
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-systemd-logind.service-Lwf6ZW
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-systemd-oomd.service-LeVvmv
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-systemd-resolved.service-53VrRA
            systemd-private-a18893d0522b43adb7a28d9c6a43c082-upower.service-pxGj51

        `nelosn@nelosn-lenovo:~$ mv /tmp/archivo2.txt .` "Ahora ese archivo lo paso a directorio personal"

        `nelosn@nelosn-lenovo:~$ ls` " Compruebo "
            archivo1.txt  Descargas   Escritorio  listado.txt  Plantillas  snap
            archivo2.txt  Documentos  Imágenes    Música       Público     Vídeos

        `nelosn@nelosn-lenovo:~$ `



- #### **Ejercicio 4: Comprimir y Descomprimir Archivos**

    - Objetivo: Aprender a trabajar con compresión de archivos. 

    - Crea un directorio llamado `backup` y copia algunos archivos en él. 

    - Comprime el directorio `backup` en un archivo `.tar.gz`:

            tar -czvf backup.tar.gz backup/

    - Borra el directorio original y extrae el contenido del archivo comprimido:

            tar -xzvf backup.tar.gz

    - ***RESULTADO***

        `nelosn@nelosn-lenovo:~$ mkdir backup` "Creo el directorio"

        `nelosn@nelosn-lenovo:~$ ls` "Veo que se creo"

            archivo1.txt  archivo.txt  Descargas   Escritorio  listado.txt  Plantillas  snap
            archivo2.txt  backup       Documentos  Imágenes    Música       Público     Vídeos

        `nelosn@nelosn-lenovo:~$ cp -r /home/nelosn/Documentos/SO backup/` "Copio unos harchivos"

        `nelosn@nelosn-lenovo:~$ ls backup/` "Veo que ya tengo los harchivos en el directorio"
            SO

        `nelosn@nelosn-lenovo:~$ tar -czvf backup.tar.gz backup/` "Comprimo el directorio"

            backup/
            backup/SO/
            backup/SO/tarea2/
            backup/SO/tarea2/planificadores2
            backup/SO/tarea2/planificadores1.c
            backup/SO/tarea2/planificadores7
            backup/SO/tarea2/planificadores5
            backup/SO/tarea2/planificadores3
            backup/SO/tarea2/planificadores6.c
            backup/SO/tarea2/planificadores1
            backup/SO/tarea2/planificadores7.c
            backup/SO/tarea2/planificadores2.c
            backup/SO/tarea2/planificadores4
            backup/SO/tarea2/planificadores3.c
            backup/SO/tarea2/planificadores4.c
            backup/SO/tarea2/planificadores6
            backup/SO/tarea2/planificadores5.c
            backup/SO/tarea2/planificadores
            backup/SO/matrix.c
            backup/SO/Proceso4
            backup/SO/Proceso4.c
            backup/SO/matrix
            backup/SO/tarea1/
            backup/SO/tarea1/ejercicio3.c
            backup/SO/tarea1/ejercicio4
            backup/SO/tarea1/ejercicio8
            backup/SO/tarea1/ejercicio2.c
            backup/SO/tarea1/ejercicio4.c
            backup/SO/tarea1/ejercicio8.c
            backup/SO/tarea1/ejercicio1.c
            backup/SO/tarea1/ejercicio6
            backup/SO/tarea1/ejercicio7
            backup/SO/tarea1/ejercicio7.c
            backup/SO/tarea1/ejercicio5
            backup/SO/tarea1/ejercicio1
            backup/SO/tarea1/ejercicio3
            backup/SO/tarea1/ejercicio2
            backup/SO/tarea1/ejercicio6.c
            backup/SO/tarea1/programa
            backup/SO/tarea1/ejercicio5.c

        `nelosn@nelosn-lenovo:~$ ls`"Veo que ya me aparece el directorio comprimido"

            archivo1.txt  backup         Documentos  listado.txt  Público
            archivo2.txt  backup.tar.gz  Escritorio  Música       snap
            archivo.txt   Descargas      Imágenes    Plantillas   Vídeos

        `nelosn@nelosn-lenovo:~$ rm -r backup/`"Borro el directorio original"

        `nelosn@nelosn-lenovo:~$ ls` "Veo que ya no esta"

            archivo1.txt  archivo.txt    Descargas   Escritorio  listado.txt  Plantillas  snap
            archivo2.txt  backup.tar.gz  Documentos  Imágenes    Música       Público     Vídeos

        `nelosn@nelosn-lenovo:~$ tar -xzvf backup.tar.gz` "Descomprimo el directorio que esta comprimido"

            backup/
            backup/SO/
            backup/SO/tarea2/
            backup/SO/tarea2/planificadores2
            backup/SO/tarea2/planificadores1.c
            backup/SO/tarea2/planificadores7
            backup/SO/tarea2/planificadores5
            backup/SO/tarea2/planificadores3
            backup/SO/tarea2/planificadores6.c
            backup/SO/tarea2/planificadores1
            backup/SO/tarea2/planificadores7.c
            backup/SO/tarea2/planificadores2.c
            backup/SO/tarea2/planificadores4
            backup/SO/tarea2/planificadores3.c
            backup/SO/tarea2/planificadores4.c
            backup/SO/tarea2/planificadores6
            backup/SO/tarea2/planificadores5.c
            backup/SO/tarea2/planificadores
            backup/SO/matrix.c
            backup/SO/Proceso4
            backup/SO/Proceso4.c
            backup/SO/matrix
            backup/SO/tarea1/
            backup/SO/tarea1/ejercicio3.c
            backup/SO/tarea1/ejercicio4
            backup/SO/tarea1/ejercicio8
            backup/SO/tarea1/ejercicio2.c
            backup/SO/tarea1/ejercicio4.c
            backup/SO/tarea1/ejercicio8.c
            backup/SO/tarea1/ejercicio1.c
            backup/SO/tarea1/ejercicio6
            backup/SO/tarea1/ejercicio7
            backup/SO/tarea1/ejercicio7.c
            backup/SO/tarea1/ejercicio5
            backup/SO/tarea1/ejercicio1
            backup/SO/tarea1/ejercicio3
            backup/SO/tarea1/ejercicio2
            backup/SO/tarea1/ejercicio6.c
            backup/SO/tarea1/programa
            backup/SO/tarea1/ejercicio5.c

        `nelosn@nelosn-lenovo:~$ ls` "Veo que ya aparece el directorio descomrpimido"

            archivo1.txt  backup         Documentos  listado.txt  Público
            archivo2.txt  backup.tar.gz  Escritorio  Música       snap
            archivo.txt   Descargas      Imágenes    Plantillas   Vídeos

        `nelosn@nelosn-lenovo:~$ `


- #### **Ejercicio 5: Permisos y Propiedades de Archivos**

    - Objetivo: Aprender a modificar permisos y propietarios de archivos.

    - Crea un archivo llamado `privado.txt`:

            touch privado.txt

    - Cambia los permisos del archivo para que solo el propietario pueda leer y escribir:

            chmod 600 privado.txt

    - Cambia el propietario del archivo a otro usuario (si tienes privilegios):

            sudo chown usuario privado.txt

    - ***RESULTADO***

        `nelosn@nelosn-lenovo:~$ touch privado.txt` "Creo el archivo"

        `nelosn@nelosn-lenovo:~$ chmod 600 privado.txt` " Cambio los permisos leer y escribir propietario"

        `nelosn@nelosn-lenovo:~$ echo "Texto adicional" >> privado.txt` "Escribo un texto"

        `nelosn@nelosn-lenovo:~$ cat privado.txt` "Veo lo que escribio"

             Texto adicional

        `nelosn@nelosn-lenovo:~$ sudo chown jose privado.txt` "Cambio el propetario del archivo "

        `nelosn@nelosn-lenovo:~$ echo "Texto nuevo" >> privado.txt` "Vuelco a escribir algo"

             bash: privado.txt: Permiso denegado

        `nelosn@nelosn-lenovo:~$ su jose` "cambio de usuario"
        
        `jose@nelosn-lenovo:/home/nelosn$ echo "Texto nuevo" >> privado.txt` "Escribo algo en el archivo"

             bash: privado.txt: Permiso denegado

        `jose@nelosn-lenovo:/home/nelosn$ exit` " No deja escribir ni leer el archi por que solo el propetario origianl "

             exit

        `nelosn@nelosn-lenovo:~$ sudo chown nelosn privado.txt` "Cambio de propietario "

        `nelosn@nelosn-lenovo:~$ echo "Texto nuevo" >> privado.txt` "Vuevo a escribir algo"

        `nelosn@nelosn-lenovo:~$ cat privado.txt`

        Texto adicional
        Texto nuevo

        `nelosn@nelosn-lenovo:~$ `



- #### **Ejercicio 6: Exploración de Dispositivos**

    - Objetivo: Identificar discos y particiones en el sistema.

    - Usa `lsblk` para listar los discos y particiones:

            lsblk

    - Usa `du -sh` para ver el tamaño del contenido en un directorio de tu elección:

            du -sh /ruta/directorio

    - Verifica el uso de disco con `df -h`:

            df -h

    - ***RESULTADO***

        `nelosn@nelosn-lenovo:~$ lsblk`
                NAME   MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS
                loop0    7:0    0     4K  1 loop /snap/bare/5
                loop1    7:1    0  73,9M  1 loop /snap/core22/1663
                loop2    7:2    0  73,9M  1 loop /snap/core22/1722
                loop3    7:3    0 270,7M  1 loop /snap/firefox/4259
                loop4    7:4    0  10,7M  1 loop /snap/firmware-updater/127
                loop5    7:5    0   273M  1 loop /snap/firefox/5273
                loop6    7:6    0  11,1M  1 loop /snap/firmware-updater/147
                loop7    7:7    0   497M  1 loop /snap/gnome-42-2204/141
                loop8    7:8    0 505,1M  1 loop /snap/gnome-42-2204/176
                loop9    7:9    0  91,7M  1 loop /snap/gtk-common-themes/1535
                loop10   7:10   0  12,2M  1 loop /snap/snap-store/1216
                loop11   7:11   0  10,7M  1 loop /snap/snap-store/1218
                loop12   7:12   0  38,8M  1 loop /snap/snapd/21759
                loop13   7:13   0  44,3M  1 loop /snap/snapd/23258
                loop14   7:14   0   476K  1 loop /snap/snapd-desktop-integration/157
                loop15   7:15   0   568K  1 loop /snap/snapd-desktop-integration/253
                sda      8:0    0    25G  0 disk 
                ├─sda1   8:1    0     1M  0 part 
                └─sda2   8:2    0    25G  0 part /var/snap/firefox/common/host-hunspell
                                                /
                sdb      8:16   1  14,5G  0 disk 
                └─sdb1   8:17   1  14,5G  0 part /media/nelosn/ESD-ISO
                sr0     11:0    1  1024M  0 rom  

        `nelosn@nelosn-lenovo:~$ du -sh /media/nelosn/ESD-ISO`

                7,3G	/media/nelosn/ESD-ISO

        `nelosn@nelosn-lenovo:~$ df -h /media/nelosn/ESD-ISO`

                S.ficheros     Tamaño Usados  Disp Uso% Montado en
                /dev/sdb1         15G   7,3G  7,4G  50% /media/nelosn/ESD-ISO

        `nelosn@nelosn-lenovo:~$ `


- #### **Ejercicio 7: Crear y Formatear Particiones**

    - Objetivo: Crear y formatear una nueva partición (Usar disco de práctica o máquina virtual).

    - Identifica un disco no particionado:

            sudo fdisk -l

    - Usa `fdisk` para crear una nueva partición:

            sudo fdisk /dev/sdX

    - Formatea la partición como `ext4`:

            sudo mkfs.ext4 /dev/sdX1

    - Monta la partición en un directorio y prueba escribiendo archivos en ella:

            sudo mount /dev/sdX1 /mnt/nueva_particion
            echo "Prueba de escritura" > /mnt/nueva_particion/test.txt

    - ***RESUTADO***
  
       ![ejercicio](img/particionusb.png)



### **Actividad Final** 

#### **Sistemas de Archivos**

- #### ***Ejercicio 1: Concepto y noción de archivo real y virtual***

    Descripción:

    Define los conceptos de archivo real y archivo virtual y explica sus diferencias.
    Identifica ejemplos prácticos de cada tipo en sistemas operativos actuales.

    Tareas:

    - Define el concepto de archivo real y archivo virtual.
  
      Archivo Real:
      Un archivo real es un conjunto de datos almacenados físicamente en un dispositivo de almacenamiento, como un disco duro, una unidad SSD o una memoria USB.

      Archivo Virtual:
      Un archivo virtual no tiene una existencia física directa en el sistema de almacenamiento. En cambio, es una representación lógica o temporal generada por el sistema operativo o por aplicaciones.


    - Proporciona ejemplos de cómo los sistemas operativos manejan archivos
      reales y virtuales.

      Archivos Reales:

      Documentos de texto (.txt, .docx) almacenados en un disco duro.

      Imágenes (.jpg, .png) y videos (.mp4, .avi) descargados y guardados en la computadora.

      Archivos ejecutables (.exe) que representan programas instalados.

      Archivos Virtuales:

      Dispositivos en sistemas operativos Unix/Linux representados como archivos en el directorio /dev, como /dev/null o /dev/sda.
      
      Archivos temporales en memoria, como los generados por navegadores para almacenar información en caché mientras se navega.
      
      Flujos de datos de red o pipes utilizados para comunicar procesos sin almacenamiento físico, como en ls | grep "archivo" en la terminal.


    - Explica un caso práctico donde un archivo virtual sea más útil que un
      archivo real.

        El archivo /dev/null actúa como un "sumidero" de datos. Cualquier información escrita en este archivo virtual se descarta automáticamente, lo que es útil para pruebas o para redirigir salidas no deseadas en scripts.
        
        Por ejemplo:

        `find / -name "archivo" > /dev/null`



- #### ***Ejercicio 2: Componentes de un sistema de archivos***

    Descripción:

    Investiga los componentes principales de un sistema de archivos y compáralos
    entre dos sistemas operativos, como Linux y Windows.

    Tareas:

    - Identifica los componentes clave de un sistema de archivos (por ejem-
    plo, metadatos, tablas de asignación, etc.).

        Metadatos: Información sobre los archivos y directorios, como nombre, tamaño, permisos, propietario, y marcas de tiempo.
    
        Tablas de Asignación: Estructuras que rastrean qué bloques de almacenamiento están asignados a un archivo.
    
        Directorios: Organización jerárquica de los archivos dentro del sistema.
    
        Bloques de Datos: Áreas del dispositivo de almacenamiento donde se guardan los datos reales de los archivos.
    
        Journaling (si aplica): Sistema de registro para mantener integridad en caso de fallos.

    - Crea un cuadro comparativo de cómo estos componentes funcionan en
    sistemas como EXT4 y NTFS.

        | **Componente**            | **EXT4 (Linux)**                                                                 | **NTFS (Windows)**                                                                 |
        |---------------------------|----------------------------------------------------------------------------------|-----------------------------------------------------------------------------------|
        | **Metadatos**             | Almacena metadatos en inodos (información sobre tamaño, tiempo, permisos, etc.). | Utiliza MFT (Master File Table) para registrar información detallada de archivos. |
        | **Tablas de Asignación**  | Estructura basada en grupos de bloques. Utiliza mapas de bits para rastrear bloques libres. | Tabla de clústeres para rastrear el estado de asignación.                          |
        | **Directorios**           | Árbol jerárquico optimizado con índices hash para búsquedas rápidas.             | Árbol B+ en la MFT para gestionar estructuras de directorios.                      |
        | **Bloques de Datos**      | Tamaño configurable (1024 a 4096 bytes). Optimizado para archivos grandes.       | Tamaño dinámico de clúster (dependiendo del tamaño del volumen).                  |
        | **Journaling**            | Utiliza un sistema de journaling basado en transacciones (modo completo o ordenado). | Registro avanzado para transacciones de archivos, incluyendo cambios parciales.   |
        | **Compatibilidad**        | Específico para sistemas Linux (aunque soportado en Windows con software adicional). | Compatible con todas las versiones modernas de Windows.                           |


    - Describe las ventajas y desventajas de cada sistema basado en sus
    componentes.

        EXT4 (Linux):

        Ventajas:

        Soporte robusto para journaling, minimizando riesgos de corrupción en fallos.
        Alta velocidad y eficiencia en la gestión de archivos grandes.
        Optimización para SSD y sistemas modernos con funciones como "extents".

        Desventajas:

        No tiene compresión ni cifrado nativo.
        Menor soporte en Windows, requiere software de terceros.

        NTFS (Windows):

        Ventajas:

        Soporte nativo para compresión y cifrado.
        Amplia compatibilidad con sistemas Windows y otros sistemas operativos.
        Gestión avanzada de permisos de seguridad.

        Desventajas:

        Puede ser más lento en ciertas operaciones debido a su complejidad.
        Desempeño subóptimo en sistemas con recursos limitados o volúmenes muy pequeños.



- ####  ***Ejercicio 3: Organización lógica y física de archivos***

    Descripción:

    Crea un esquema que muestre la organización lógica y física de un sistema
    de archivos. Explica cómo se relacionan las estructuras lógicas con las físicas
    en el disco.

    Tareas:

    - Diseña un árbol jerárquico que represente la organización lógica de
    directorios y subdirectorios.

            /
            ├── home
            │   ├── user
            │   │   ├── documents
            │   │   │   ├── file1.txt
            │   │   │   └── file2.txt
            │   │   ├── downloads
            │   │   │   └── movie.mp4
            │   │   └── pictures
            │   │       └── photo.jpg
            ├── var
            │   ├── log
            │   │   └── syslog
            │   └── www
            │       └── index.html
            └── etc
                ├── hosts
                └── fstab



    - Explica cómo se traduce la dirección lógica a la dirección física en el
    disco.

        Dirección Lógica: Representa la ubicación del archivo en el árbol de directorios (por ejemplo, /home/user/documents/file1.txt).
        
        Traducción a Dirección Física:

        El sistema de archivos asigna bloques físicos en el disco para almacenar los datos del archivo.
        
        Una estructura como una tabla de asignación (en EXT4, inodos; en NTFS, MFT) relaciona las rutas lógicas con los bloques físicos.
        
        Cada archivo o directorio tiene un identificador que apunta a los bloques donde están los datos.

    - Proporciona un ejemplo práctico de cómo un archivo se almacena físi-
    camente.


- #### ***Ejercicio 4: Mecanismos de acceso a los archivos***

    Descripción:

    Simula diferentes mecanismos de acceso a archivos (secuencial, directo e
    indexado) en un entorno práctico.

    Tareas:

    1. Define los diferentes mecanismos de acceso.
       
        Acceso Secuencial:

        Los datos del archivo se leen o escriben de manera consecutiva, desde el inicio hasta el final.
        
        Ideal para archivos de texto o logs donde el procesamiento es lineal.

        Acceso Directo:

        Permite acceder a una posición específica en el archivo sin necesidad de leer los datos previos.
        
        Útil para bases de datos o archivos grandes donde se necesita leer solo partes específicas.

        Acceso Indexado:

        Se utiliza una estructura de índice para mapear las posiciones de los datos en el archivo.
        
        Común en bases de datos y sistemas de almacenamiento organizados.
   
    2. Escribe un pseudocódigo que muestre cómo acceder a:
    
       -  Un archivo secuencialmente.

            Abrir archivo "datos.txt" en modo lectura

            Mientras no se alcance el final del archivo:

             Leer línea o registro actual

             Procesar datos

            Cerrar archivo

      
       - Un archivo directamente mediante su posición.
         
            Abrir archivo "datos.txt" en modo lectura

            Ir a la posición específica (por ejemplo, byte 1024)

            Leer datos desde esa posición

            Procesar datos

            Cerrar archivo


       - Un archivo utilizando un índice.

            Abrir archivo "datos.txt" en modo lectura
            
            Cargar índice del archivo en memoria (ejemplo: índice indica que el registro 5 está en el byte 2048)
            
            Ir a la posición indicada por el índice (byte 2048)
            
            Leer y procesar datos desde esa posición
            
            Cerrar archivo


    3. Compara las ventajas de cada mecanismo dependiendo del caso de uso.

        | **Mecanismo**     | **Ventajas**                                                                                         | **Casos de Uso**                                                                                 |
        |-------------------|----------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------|
        | **Secuencial**    | - Simplicidad en la implementación.                                                                 | Archivos de log, procesadores de texto, análisis completo de datos.                             |
        |                   | - Eficiente para recorrer todo el archivo.                                                          |                                                                                                  |
        | **Directo**       | - Acceso rápido a una posición específica.                                                          | Bases de datos, archivos binarios grandes, multimedia con puntos de salto.                      |
        |                   | - Evita lectura innecesaria de datos intermedios.                                                   |                                                                                                  |
        | **Indexado**      | - Eficiente para buscar registros específicos.                                                      | Sistemas de bases de datos, búsqueda en grandes colecciones de archivos, sistemas de catálogos. |
        |                   | - Combina la velocidad del acceso directo con organización.                                         |                                                                                                  |



- #### ***Ejercicio 5: Modelo jerárquico y mecanismos de recuperaciónen caso de falla***

    Descripción:

    Diseña una estructura jerárquica para un sistema de archivos y simula un
    escenario de falla en el sistema. Describe cómo recuperar los datos utilizando
    mecanismos de recuperación.

    Tareas:

    - Diseña un modelo jerárquico para un sistema de archivos con al menos
    tres niveles de directorios.

            /
            ├── home
            │   ├── user
            │   │   ├── documents
            │   │   │   ├── report.docx
            │   │   │   ├── notes.txt
            │   │   └── pictures
            │   │       ├── photo1.jpg
            │   │       └── photo2.png
            ├── var
            │   ├── log
            │   │   ├── syslog
            │   │   └── error.log
            ├── etc
            │   ├── config.conf
            │   ├── hosts
            │   └── network


    - Simula una falla en un directorio específico y describe los pasos nece-
    sarios para recuperarlo.

        Escenario:

        El directorio /home/user/documents se corrompe debido a una falla del sistema (por ejemplo, un apagón repentino o errores en el disco).
        Los archivos contenidos (report.docx y notes.txt) no son accesibles.

        Pasos para Recuperar el Directorio:

        Identificar la Falla:
        Usar herramientas del sistema operativo para diagnosticar el problema, como fsck en Linux o el comprobador de discos en Windows.

        Montar el Sistema de Archivos en Modo Recuperación:
        Iniciar el sistema en modo recuperación o desde un entorno de arranque.
        Montar el sistema de archivos como solo lectura para evitar más daños.

        Ejecutar Herramientas de Recuperación:
        En Linux:

        f sck /dev/sdX

        Esto reparará inodos dañados y tablas de asignación.
        En Windows:
        Usar chkdsk con opciones como /f o /r.

        Restaurar desde Respaldo:

        Si los archivos no se recuperan, restaurar el directorio desde un respaldo previamente creado.

    - Explica qué herramientas o técnicas de respaldo (backup) utilizarías
    para evitar pérdida de datos.

        Copias de Seguridad Automáticas:

        Usar herramientas como rsync (Linux) o Backup and Restore (Windows) para programar copias regulares.

        Sistemas de Versionado:

        Implementar herramientas como Git o soluciones empresariales (ejemplo: Snapshots en sistemas de archivos ZFS o Btrfs).

        Almacenamiento en la Nube:

        Utilizar servicios como Google Drive, OneDrive o soluciones empresariales como AWS S3.

        Redundancia Física:

        Configurar RAID para protegerse contra fallas en discos duros.

        Pruebas Periódicas:

        Validar regularmente que las copias de seguridad sean funcionales.


#### **Protección y Seguridad**

- #### ***Ejercicio 1: Concepto y objetivos de protección y seguridad***

    Descripción:

    Investiga los conceptos de protección y seguridad en sistemas operativos.
    Analiza los objetivos principales que deben cumplir estos mecanismos.

    Tareas:

    - Define los conceptos de protección y seguridad en el contexto de sis-
    temas operativos.

    - Identifica los objetivos principales de un sistema de protección y se-
    guridad, como confidencialidad, integridad y disponibilidad.

    - Da un ejemplo práctico de cómo se aplican estos objetivos en un sistema
    operativo.


- #### ***Ejercicio 2: Clasificación aplicada a la seguridad***

    Descripción:

    Clasifica los mecanismos de seguridad en un sistema operativo y explica cómo
    cada tipo contribuye a la protección del sistema.

    Tareas:
    
    - Investiga las clasificaciones comunes de la seguridad, como física, lógica
    y de red.

    - Explica el papel de cada clasificación en la protección de un sistema
    operativo.

    - Proporciona ejemplos prácticos de herramientas o técnicas utilizadas
    en cada clasificación.


- #### ***Ejercicio 3: Funciones del sistema de protección***
  
    Descripción:

    Analiza las funciones que cumple un sistema de protección en un entorno
    multiusuario.

    Tareas:

    - Describe cómo un sistema de protección controla el acceso a los recur-
    sos.

    - Explica las funciones principales como autenticación, autorización y
    auditoría.

    - Diseña un caso práctico donde se muestren las funciones de un sistema
    de protección en acción.


- #### ***Ejercicio 4: Implantación de matrices de acceso***
  
    Descripción:

    Crea e implementa una matriz de acceso para un sistema que contiene usuar-
    ios y recursos con diferentes niveles de permisos.

    Tareas:

    - Diseña una matriz de acceso para un sistema con al menos 3 usuarios
    y 4 recursos.

    - Explica cómo esta matriz se utiliza para controlar el acceso en un
    sistema operativo.

    - Simula un escenario donde un usuario intenta acceder a un recurso no
    permitido y cómo la matriz lo bloquea.


- #### ***Ejercicio 5: Protección basada en el lenguaje***
  
    Descripción:

    Investiga cómo los lenguajes de programación pueden implementar mecan-
    ismos de protección.

    Tareas:

    - Explica el concepto de protección basada en el lenguaje.
  
    - Proporciona un ejemplo de cómo un lenguaje como Java o Rust asegura
    la memoria y evita accesos no autorizados.

    - Compara este enfoque con otros mecanismos de protección en sistemas
    operativos.


- #### ***Ejercicio 6: Validación y amenazas al sistema***
  
    Descripción:

    Analiza las principales amenazas a un sistema operativo y los mecanismos
    de validación utilizados para prevenirlas.

    Tareas:

    - Investiga y describe al menos tres tipos de amenazas comunes (por
    ejemplo, malware, ataques de fuerza bruta, inyección de código).

    - Explica los mecanismos de validación como autenticación multifactor
    y control de integridad.

    - Diseña un esquema de validación para un sistema operativo con múlti-
    ples usuarios.


- #### ***Ejercicio 7: Cifrado***
  
    Descripción:

    Explora cómo los mecanismos de cifrado protegen la información en un sis-
    tema operativo.

    Tareas:

    - Define los conceptos de cifrado simétrico y asimétrico.
  
    - Proporciona un ejemplo práctico de cada tipo de cifrado aplicado en
    sistemas operativos.

    - Simula el proceso de cifrado y descifrado de un archivo con una clave
    dada.



