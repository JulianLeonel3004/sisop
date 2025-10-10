#include "definiciones.h"

void funcion_prueba_generador()
{
    printf("Soy generador\n");
}

void generador(int (*pipe_respuesta)[2], int idx_pipe, Alumno* mem_comp, int id_cola)
{
    int lectura = 0;
    int escritura = 1;
    

    //cierro los extremos que no voy a usar de cada pipe
    close(pipe_respuesta[idx_pipe][escritura]);

    Mensaje msg;
    msg.mtype = 1; // un único tipo de mensaje
    msg.generador_id = idx_pipe;

    if (msgsnd(id_cola, &msg, sizeof(Mensaje) - sizeof(long), 0) == -1) {
        perror("fallo pedidos de ids");
    }
    
    //Espero la respuesta con las 10 ids y lo guardo en la variable ids
    int ids[10];
    read(pipe_respuesta[idx_pipe][lectura], ids, sizeof(int)*10);

    for (int i = 0; i < 10; i++)
    {
        sem_wait(alumno_leido);
        sem_wait(Mutex);
        mem_comp->id = ids[i];
        //datos de prueba
        snprintf(mem_comp->nombre, MAX_STR+1, "Nombre%d", i);
        snprintf(mem_comp->apellido, MAX_STR+1, "Apellido%d", i);
        mem_comp->anio = i;
        snprintf(mem_comp->materia, MAX_STR+1, "Materia%d", i);
        sem_post(Mutex);
        sem_post(nuevo_alumno);
    }
    printf("generador %d salio\n", idx_pipe);

}
