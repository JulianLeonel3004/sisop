// Ejemplo archivo
/*
ID	Nombre	Apellido	Anio	Materia
1	Homero	Simpson	3	Sistemas operativos
*/

/*
Este main solo genera procesos y llama a las funciones que deben ser desarrolladas para su funcionamiento
 */
#include "definiciones.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

sem_t *Mutex;
sem_t *capacidad_memoria;
sem_t *nuevo_alumno;
int shmid_memoria_compartida;
Alumno *mem_comp;
static volatile int terminar_proceso = 0;
pid_t *pids;
int id_cola = 0;
int cant_generadores = 0;

void manejar_terminacion_main(int sig) {
        if (sig == SIGTERM || sig == SIGINT) {
            printf("Terminando main...\n");
            liberar_todo(id_cola, pids);
            terminar_proceso = 1;
            enviar_kill(pids, cant_generadores);
        }
}

int main(int argc, char** argv)
{
    // Leé parámetros (si no querés usarlos aún, igual valida)
    Params p;
    if (!parse_params(argc, argv, &p)) {
        return 1;
    }
    int cant_registros   = p.total_registros;
    cant_generadores = p.generadores;
    
    int pipe_respuesta[cant_generadores][2];

    
    signal(SIGTERM, manejar_terminacion_main);
    signal(SIGINT, manejar_terminacion_main);

    // CREAR los pipes ANTES de fork
    for (int g = 0; g < cant_generadores; g++) {
        if (pipe(pipe_respuesta[g]) < 0) {
            perror("pipe_respuesta falló");
            exit(1);
        }
    }

    crear_memoria_compartida();

    id_cola = crear_cola();

    // Limpiar semáforos previos si existen
    limpiar_semaforos();
    
    // Crear semáforos con nombre
    crear_semaforos();

    // crear array de pid para generadores
    pids = malloc((cant_generadores + 1) * sizeof(pid_t));
    if (pids == NULL)
    {
        perror("malloc falló");
        return 1;
    }
    // ejemplo: mostrar el array
    for (int i = 0; i <= cant_generadores; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork falló");
            return 1;
        }
        if (pid == 0)
        {

            if(i == 0)
            {
                coordinador(pipe_respuesta, mem_comp, cant_registros, id_cola, cant_generadores);
            }
            else
            {
                // AGREGAR GENERADOR
                int idx = i - 1; // índices 0..cant_generadores-1
                generador(pipe_respuesta, idx, mem_comp, id_cola);
            }

        }
        else
        {
            // El padre guarda el PID del hijo
            pids[i] = pid;
        }
    }

    //imprimir los pids
    for (int i = 0; i <= cant_generadores; i++) {
        if(i == 0){
            printf("Coordinador, PID=%d\n", pids[i]);
        }else{
            printf("Generador, PID=%d\n", pids[i]);
        }
    }

    // Esperar Enter para terminar todos los procesos o recibir señal
    printf("Presiona Enter para terminar todos los procesos o Ctrl+C para terminar...\n");
    while (!terminar_proceso) {
        if (getchar() == '\n') {
            break;
        }
    }
    
    // Solo ejecutar limpieza si no se recibió una señal
    if (!terminar_proceso) {
        // Enviar señal SIGTERM a todos los procesos hijos (incluyendo coordinador)
        enviar_kill(pids, cant_generadores);
        
        // Esperar a que terminen todos los procesos hijos
        enviar_kill(pids, cant_generadores);

        // Libera semaforos, memoria compartida, cola de mensajes y array de pids
        liberar_todo(id_cola, pids);
    }

    printf("Finalizo\n");

    return 0;
}