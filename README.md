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
    - [Actividad](#actividad)
      - [**OBJETIVO**](#objetivo)
      - [**¿Qué se espera de ti?**](#qué-se-espera-de-ti)
      - [**Administración de Memoria**](#administración-de-memoria)
      - [**Administración de Entrada/Salida**](#administración-de-entradasalida)
    - [Actividades: Dispositivos de entrada y salida en Linux](#actividades-dispositivos-de-entrada-y-salida-en-linux)

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

       Respuesta : 

    1.2 Investiga y explica las políticas de reemplazo de páginas en sistemas
    operativos. ¿Cuál consideras más eficiente y por qué?

       Respuesta : 

2. **Memoria real**
   
    2.1 Escribe un programa en C o Python que simule la administración de
    memoria mediante particiones fijas.

       Respuesta : 


    2.2 Diseña un algoritmo para calcular qué procesos pueden ser asignados
    a un sistema con memoria real limitada utilizando el algoritmo de
    "primera cabida".

       Respuesta : 

3. **Organización de memoria virtual**
   
    3.1 Investiga y explica el concepto de "paginación" y "segmentación".
    ¿Cuáles son las ventajas y desventajas de cada técnica?

       Respuesta : 


    3.2 Escribe un programa que simule una tabla de páginas para procesos
    con acceso aleatorio a memoria virtual.

       Respuesta : 

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

**Introducción**

En este ejercicio, aprenderá a listar, verificar y analizar los dispositivos de entrada y salida en Linux. Usarán comandos básicos y herramientas comunes disponibles en cualquier distribución.

**Actividad 1: Listar dispositivos conectados**

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
        ¿Qué tipos de dispositivos se muestran en la salida de `lsblk`?
        ¿Cuál es la diferencia entre `lsusb` y `lspci`?
        ¿Qué información adicional proporciona `dmesg | grep usb`?

**Actividad 2: Verificar dispositivos de almacenamiento**

1. Objetivo

Aprender cómo identificar discos duros, particiones y su configuración.

2. Instrucciones

    1. Use el comando `fdisk -l` para listar todos los discos y particiones.
    2. Utilice `blkid` para ver los identificadores UUID y los tipos de sistema de archivos.
    3. Use `df -h` para listar los dispositivos montados y su espacio disponible.
    4. Conteste:
        - ¿Qué dispositivos de almacenamiento están conectados a su sistema?
        - ¿Qué particiones están montadas actualmente?
        - ¿Qué tipo de sistemas de archivos se usan en las particiones?

**Actividad 3: Explorar dispositivos de entrada**

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
        - ¿Cómo se identifican los dispositivos en `/proc/bus/input/devices`?

**Actividad 4: Examinar dispositivos de salida**

1. Objetivo

Entender cómo identificar dispositivos de salida como monitores y tarjetas de sonido.

2. Instrucciones

    1. Use `xrandr` para listar las pantallas conectadas y sus resoluciones.
    2. Ejecute `aplay -l` para listar las tarjetas de sonido disponibles.
    3. Use `lsof /dev/snd/*` para ver qué procesos están utilizando la tarjeta de sonido.
    4. Conteste:
        - ¿Qué salidas de video están disponibles en su sistema?
        - ¿Qué dispositivos de sonido se detectaron?
        - ¿Qué procesos están usando la tarjeta de sonido?

**Actividad 5: Crear un script de resumen**

1. Objetivo

Automatizar la recopilación de información de dispositivos de entrada y salida.

2. Instrucciones

    1. Cree un archivo llamado `dispositivos.sh` y agregue el siguiente contenido: ```bash #!/bin/bash echo "Dispositivos de bloque:" lsblk echo "Dispositivos USB:" lsusb echo "Dispositivos PCI:" lspci echo "Dispositivos de entrada:" cat /proc/bus/input/devices echo "Salidas de video:" xrandr echo "Tarjetas de sonido:" aplay -l ```
    2. Ejecute el script usando `bash dispositivos.sh`.
    3. Modifique el script para guardar la salida en un archivo llamado `resumendispositivos.txt`.
    4. Conteste:
        - ¿Qué ventajas tiene usar un script para recopilar esta información?
        - ¿Qué cambios realizaría para personalizar el script?

**Actividad 6: Reflexión y discusión**

1. Objetivo

Analizar la importancia del manejo de dispositivos en sistemas Linux.

2. Instrucciones

    1. Reflexione sobre lo aprendido y discuta en equipo:
        - ¿Qué comando encontró más útil y por qué?
        - ¿Qué tan importante es conocer los dispositivos conectados al sistema?
        - ¿Cómo podrían estos conocimientos aplicarse en la administración de sistemas?


