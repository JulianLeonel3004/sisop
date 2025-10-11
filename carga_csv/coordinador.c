#include "definiciones.h"
#include <time.h>
#include <pthread.h>
#define LECTURA 0
#define ESCRITURA 1
#define ARCHIVO "alumnos.csv"

void coordinador(int (*pipe_respuesta)[2], Alumno* mem_comp, int cant_registros, int id_cola, int cant_generadores){
    int contador_registro = 0;
    int primer_id_valido = 1;
    int contador_cola = 0;
    int contador_ids = 0;
    Mensaje msg;
    Alumno alumno;

    printf("[Coordinador] Esperando mensajes...\n");
   
    // ABRE ARCHIVO
    FILE* fp = fopen(ARCHIVO, "a");
    if (fp == NULL) {
        perror("Error abriendo archivo CSV");
        return;
    }
    // Escribir encabezado
    fprintf(fp, "id,nombre,apellido,anio,materia\n");

    //int generadores_recibidos = 0;
    while (contador_registro < cant_registros) { // Esperar a recibir de ambos generadores
        // ESPERA MENSAJE DE GENERADOR
       if(contador_cola < cant_generadores){
        if (msgrcv(id_cola, &msg, sizeof(Mensaje) - sizeof(long), 0, 0) == -1) {
            perror("msgrcv");
            continue;
        }
        contador_cola++;

        if(contador_ids < cant_registros){ // solo si aún hay registros pendientes de generar
            generar_y_enviar_ids(pipe_respuesta, msg.generador_id, cant_registros, contador_ids, &primer_id_valido);
            contador_ids += primer_id_valido-1;
        }

       }
        

        printf("[Coordinador] Recibido de generador: %d\n", msg.generador_id);
       // generadores_recibidos++;
        //GENERAR IDS
     
        
        // Solo procesar si aún no hemos alcanzado cant_registros
        //if (contador_registro < cant_registros) {
            // Consumir registros hasta alcanzar cant_registros
           // int registros_a_consumir = (cant_registros - contador_registro < TOTAL_IDS) ? 
             //                          (cant_registros - contador_registro) : TOTAL_IDS;
            
           // for (int k = 0; k < registros_a_consumir && contador_registro < cant_registros; k++) {
                sem_wait(nuevo_alumno);
                sem_wait(Mutex);

                alumno.id = mem_comp->id;
                snprintf(alumno.nombre, MAX_STR+1, "%s", mem_comp->nombre);
                snprintf(alumno.apellido, MAX_STR+1, "%s", mem_comp->apellido);
                alumno.anio = mem_comp->anio;
                snprintf(alumno.materia, MAX_STR+1, "%s", mem_comp->materia);

                sem_post(Mutex);
                sem_post(capacidad_memoria);

                //GUARDAR EN CSV
                guardarAlumnoCSV(alumno, ARCHIVO, contador_registro, fp);
                contador_registro++;
                printf("contador_registro: %d\n", contador_registro);
           // }
       // }
    
    }

    printf("salio del while\n");

    // Cerrar archivo CSV
    fclose(fp);
    printf("archivo cerrado\n");
    
    // La eliminación de la cola la hace el proceso padre tras esperar a los hijos
    printf("coordinador terminando\n");
    exit(0);
}

void generar_y_enviar_ids(int (*pipe_respuesta)[2], int id_generador, int cant_registros, int contador_ids, int *primer_id_valido) {
   
    ListaIDs lista;
  
    close(pipe_respuesta[id_generador][LECTURA]);

    int registros_a_consumir = (cant_registros - contador_ids < TOTAL_IDS) ? 
                          (cant_registros - contador_ids) : TOTAL_IDS;
             
  //  int ids_necesarios = TOTAL_IDS;

   // int ids_necesarios = cant_registros - *ultimo_id_enviado;
   // int ids_necesarios = *ultimo_id_enviado + 1;
    lista.cantidad = registros_a_consumir;
    printf("contador: %d\n", contador_ids);
    printf("lista.cantidad: %d\n", lista.cantidad);

    // 2. Crear IDs consecutivos del 1 al ids_necesarios
    for (int i = 0; i < lista.cantidad; i++) {
        lista.ids[i] = *primer_id_valido + i;
    }

    *primer_id_valido = lista.ids[lista.cantidad - 1] + 1;

    // 3. Mezclar (Fisher–Yates)
   /*for (int i = lista.cantidad - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = lista.ids[i];
        lista.ids[i] = lista.ids[j];
        lista.ids[j] = temp;
    }*/
    
    //se envia primero la cantidad de IDs, luego los IDs
  //  write(pipe_respuesta[id_generador][ESCRITURA], &lista.cantidad, sizeof(int));
    write(pipe_respuesta[id_generador][ESCRITURA], &lista, sizeof(lista));
}

void guardarAlumnoCSV(Alumno alumno, const char* filename, int contador_registro, FILE* fp) {
    // Escribir el alumno
    fprintf(fp, "%d,%s,%s,%d,%s\n",
            alumno.id,
            alumno.nombre,
            alumno.apellido,
            alumno.anio,
            alumno.materia);

    fflush(fp); // Forzar escritura al disco
    printf("Alumno guardado en %s\n", filename);
}