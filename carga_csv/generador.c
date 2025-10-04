#include "definiciones.h"

void funcion_prueba_generador()
{
    printf("Soy generador\n");
}

void generador(int (*pipe_peticion)[2], int (*pipe_respuesta)[2], int idx_pipe, Alumno* mem_comp)
{
    int lectura, escritura = 0;
    //cierro los extremos que no voy a usar de cada pipe
    close(pipe_peticion[idx_pipe][lectura]);
    close(pipe_respuesta[idx_pipe][escritura]);

    int pedido_ultimos_10_ids = 1; //uso un valor cualquiera como señal
    write(pipe_peticion[idx_pipe][escritura], &pedido_ultimos_10_ids, sizeof(int));
    
    //Espero la respuesta con las 10 ids y lo guardo en la variable ids
    int ids[10];
    read(pipe_respuesta[idx_pipe][lectura], &ids, sizeof(int));

    for (int i = 0; i < 10; i++)
    {
        mem_comp->id = i;
        //datos de prueba
        snprintf(mem_comp->nombre, MAX_STR+1, "Nombre%d", i);
        snprintf(mem_comp->apellido, MAX_STR+1, "Apellido%d", i);
        mem_comp->anio = i;
        snprintf(mem_comp->materia, MAX_STR+1, "Materia%d", i);
    }
    

}
