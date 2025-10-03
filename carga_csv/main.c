// Ejemplo archivo
/*
ID	Nombre	Apellido	Anio	Materia
1	Homero	Simpson	3	Sistemas operativos
*/

/*
Este main solo genera procesos y llama a las funciones que deben ser desarrolladas para su funcionamiento
 */
#include "definiciones.h"

int main(void)
{
    int cant_registros = 3;
    int cant_generadores = 2;
    int pipe_peticion[cant_generadores][2];
    int pipe_respuesta[cant_generadores][2];

    funcion_prueba_parametros();


    // CREAR los pipes ANTES de fork
    for (int g = 0; g < cant_generadores; g++) {
        if (pipe(pipe_peticion[g]) < 0) {
            perror("pipe_peticion falló");
            exit(1);
        }
        if (pipe(pipe_respuesta[g]) < 0) {
            perror("pipe_respuesta falló");
            exit(1);
        }
    }

    // crear array de pid para generadores
    pid_t *pids = malloc((cant_generadores + 1) * sizeof(pid_t));
    if (pids == NULL)
    {
        perror("malloc falló");
        return 1;
    }
    // ejemplo: mostrar el array
    for (int i = 0; i < 4; i++)
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
                // AGREGAR COORDINADOR
                funcion_prueba_coordinador();

                /*
                Prueba de comunicación entre procesos
                */


               // Proceso coordinador
                printf("Coordinador: PID=%d\n", getpid());

                for (int g = 0; g < cant_generadores; g++) {
                    close(pipe_peticion[g][1]);   // el coordinador solo lee peticiones
                    close(pipe_respuesta[g][0]); // el coordinador solo escribe respuestas
                }

                for (int g = 0; g < cant_generadores; g++) {
                    int pedido;
                    read(pipe_peticion[g][0], &pedido, sizeof(int));
                    printf("Coordinador recibió pedido %d del generador %d\n", pedido, g);

                    int respuesta = pedido * 2; // Ejemplo: el coordinador "procesa" el número
                    write(pipe_respuesta[g][1], &respuesta, sizeof(int));
                }

                exit(0);
            }
            else
            {
                // AGREGAR GENERADOR
                funcion_prueba_generador();

                /*
                Prueba de comunicación entre procesos
                */

                //
               // Proceso generador
                int idx = i - 1;
                close(pipe_peticion[idx][0]);   // generador solo escribe petición
                close(pipe_respuesta[idx][1]);  // generador solo lee respuesta

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
            // El padre guarda el PID del hijo
            pids[i] = pid;
        }
    }

   // free(pids);
     for (int i = 0; i <= cant_generadores; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return 0;
}