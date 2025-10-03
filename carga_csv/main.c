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

    funcion_prueba_parametros();

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
                printf("Coordinador: PID %d\n", i, getpid());
                exit(0); // importante: salir para que los hijos no sigan forkeando
            }
            else
            {
                // AGREGAR GENERADOR
                funcion_prueba_generador();
                printf("generador: PID %d\n", i, getpid());
                exit(0); // importante: salir para que los hijos no sigan forkeando
            }

        }
        else
        {
            // El padre guarda el PID del hijo
            pids[i] = pid;
        }
    }

    free(pids);
    return 0;
}