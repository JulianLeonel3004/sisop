#include "definiciones.h"
#include <semaphore.h>
#include <signal.h>


void crear_memoria_compartida(){
    int shmid; // Identificador de la memoria compartida
   // Generar una clave única para la memoria compartida
   // "shmfile" debe ser un archivo existente
   // 65 es un ID arbitrario para diferenciar claves
   key_t key = ftok("shmfile", 65);

   // Crear/acceder a la memoria compartida
   // sizeof(Alumno) reserva espacio para un registro Alumno
   // 0666 → permisos lectura/escritura para todos
   // IPC_CREAT → crea la memoria si no existe
   shmid = shmget(key, sizeof(Alumno), 0666 | IPC_CREAT);
   if (shmid < 0) {
       perror("shmget"); // Imprime error si falla la creación
       exit(1);      // Termina el programa
   }

   // Guardar el ID globalmente para poder liberarlo después
   shmid_memoria_compartida = shmid;

   // Asociar la memoria compartida al espacio de direcciones del proceso
   // shm_ptr apunta a la memoria compartida
   Alumno *shm_ptr = (Alumno*) shmat(shmid, NULL, 0);
   if (shm_ptr == (void*) -1) {
       perror("shmat"); // Imprime error si falla la asociación
       exit(1);
   }

   mem_comp = shm_ptr;
}


int crear_cola() {
    int id_cola = 0;
     // Acceder a la cola existente
    id_cola = msgget(CLAVE_COLA, 0666 | IPC_CREAT); //crea una cola de mensajes (o la abre si ya existe)
    if (id_cola == -1) {
        perror("No puede acceder a la cola");
        exit(1);
    }
    return id_cola;
}

void limpiar_semaforos(){
    sem_unlink("/sem_mutex");
    sem_unlink("/sem_capacidad_memoria");
    sem_unlink("/sem_nuevo_alumno");
}

void crear_semaforos(){
    Mutex = sem_open("/sem_mutex", O_CREAT | O_EXCL, 0600, 1);
    capacidad_memoria = sem_open("/sem_capacidad_memoria", O_CREAT | O_EXCL, 0600, 1);
    nuevo_alumno = sem_open("/sem_nuevo_alumno", O_CREAT | O_EXCL, 0600, 0);
}

void liberar_semaforos(){
        // Liberar semáforos
        sem_close(Mutex);
        sem_close(capacidad_memoria);
        sem_close(nuevo_alumno);
    
       limpiar_semaforos();
}

void liberar_memoria_compartida(){
    // Desasociar memoria compartida
    shmdt(mem_comp);

    // Eliminar segmento de memoria compartida
    shmctl(shmid_memoria_compartida, IPC_RMID, NULL);
}

void liberar_todo(int id_cola, int pids[]){
    liberar_semaforos();
    liberar_memoria_compartida();
    msgctl(id_cola, IPC_RMID, NULL);
    free(pids);
}

void enviar_kill(int pids[], int cant_generadores){
    for (int i = 0; i <= cant_generadores; i++) {
        if (pids[i] > 0) {
            kill(pids[i], SIGTERM);
        }
    }
}