#include "definiciones.h"

void funcion_prueba_generador()
{
    printf("Soy generador\n");
}

void generador(int (*pipe_respuesta)[2], int idx_pipe, Alumno* mem_comp, int id_cola)
{
    printf("Generador %d iniciando\n", idx_pipe);
    int lectura = 0;
    int escritura = 1;
    

    //cierro los extremos que no voy a usar de cada pipe
    close(pipe_respuesta[idx_pipe][escritura]);

    Mensaje msg;
    msg.mtype = 1; // un único tipo de mensaje
    msg.generador_id = idx_pipe;

    printf("Generador %d enviando mensaje\n", idx_pipe);
    if (msgsnd(id_cola, &msg, sizeof(Mensaje) - sizeof(long), 0) == -1) {
        perror("fallo pedidos de ids");
    } else {
        printf("Generador %d mensaje enviado correctamente\n", idx_pipe);
    }
    
    //Espero la respuesta con los ids y lo guardo en la variable ids
    ListaIDs lista;
   // int cantidad_ids =10;
    
    // Primero leer la cantidad de IDs
 //   read(pipe_respuesta[idx_pipe][lectura], &cantidad_ids, sizeof(int));
    
    // Luego leer los IDs
    //read(pipe_respuesta[idx_pipe][lectura], ids, sizeof(int) * cantidad_ids);

    read(pipe_respuesta[idx_pipe][lectura], &lista, sizeof(lista));

    // logeo
    for(int j = 0; j < lista.cantidad; j++)
    {
        printf("id %d del generador %d\n", lista.ids[j], idx_pipe);
    }

    //Escribo el registro de alumno en la memoria compartida
    for (int i = 0; i < lista.cantidad; i++)
    {

        sem_wait(capacidad_memoria);
        sem_wait(Mutex);
        mem_comp->id = lista.ids[i];
        //datos de prueba
        snprintf(mem_comp->nombre, MAX_STR+1, "Nombre %d", idx_pipe);
        snprintf(mem_comp->apellido, MAX_STR+1, "Apellido%d", i);
        mem_comp->anio = i;
        snprintf(mem_comp->materia, MAX_STR+1, "Materia%d", i);
        sem_post(Mutex);
        sem_post(nuevo_alumno);
    }
    printf("generador %d salio\n", idx_pipe);

}
