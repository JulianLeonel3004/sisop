#include "definicion.h"
#include <stdio.h>      // printf, perror
#include <stdlib.h>     // malloc, free, exit, NULL
#include <unistd.h>     // fork, pipe, read, write, close, getpid
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // waitpid

int main(int argc, char** argv)
{
    // Leé parámetros (si no querés usarlos aún, igual valida)
    Params p;
    if (!parse_params(argc, argv, &p)) {
        return 1;
    }
    int cant_registros   = p.total_registros;
    int cant_generadores = p.generadores;

    // CREAR los pipes ANTES de fork
    int pipe_peticion[cant_generadores][2];
    int pipe_respuesta[cant_generadores][2];

    for (int g = 0; g < cant_generadores; g++) {
        if (pipe(pipe_peticion[g]) < 0) { perror("pipe_peticion falló"); exit(1); }
        if (pipe(pipe_respuesta[g]) < 0) { perror("pipe_respuesta falló"); exit(1); }
    }

    // crear array de pid para (coordinador + generadores)
    pid_t *pids = malloc((cant_generadores + 1) * sizeof(pid_t));
    if (pids == NULL) { perror("malloc falló"); return 1; }

    for (int i = 0; i <= cant_generadores; i++)
    {
        pid_t pid = fork();
        if (pid < 0) { perror("fork falló"); return 1; }

        if (pid == 0)
        {
            if(i == 0)
            {
                // ---- COORDINADOR ----
                funcion_prueba_coordinador();
                printf("Coordinador: PID=%d\n", getpid());

                for (int g = 0; g < cant_generadores; g++) {
                    close(pipe_peticion[g][1]);
                    close(pipe_respuesta[g][0]);
                }

                for (int g = 0; g < cant_generadores; g++) {
                    int pedido;
                    read(pipe_peticion[g][0], &pedido, sizeof(int));
                    printf("Coordinador recibió pedido %d del generador %d\n", pedido, g);

                    int respuesta = pedido * 2;
                    write(pipe_respuesta[g][1], &respuesta, sizeof(int));
                }

                exit(0);
            }
            else
            {
                // ---- GENERADOR ----
                funcion_prueba_generador();

                int idx = i - 1;
                close(pipe_peticion[idx][0]);
                close(pipe_respuesta[idx][1]);

                int pedido = (idx + 1) * 10;
                printf("Generador %d (PID=%d) pide %d\n", idx, getpid(), pedido);
                write(pipe_peticion[idx][1], &pedido, sizeof(int));

                int respuesta;
                read(pipe_respuesta[idx][0], &respuesta, sizeof(int));
                printf("Generador %d recibió respuesta %d\n", idx, respuesta);

                exit(0);
            }
        }
        else
        {
            pids[i] = pid;
        }
    }

    for (int i = 0; i <= cant_generadores; i++) {
        waitpid(pids[i], NULL, 0);
    }

    free(pids);
    (void)cant_registros; // aún no usado aquí
    return 0;
}
