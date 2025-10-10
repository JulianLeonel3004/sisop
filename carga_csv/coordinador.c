#include <stdio.h>
#include "coordinador.h"
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include "cola.h"
#include "definiciones.h"
#define LECTURA 0
#define ESCRITURA 1

void coordinador(int (*pipe_respuesta)[2], Alumno* mem_comp, int cant_registros){
    int id_cola;
    struct mensaje msg;
    int contador_registro = 0;

    // Acceder a la cola existente
    id_cola = msgget(CLAVE_COLA, 0666 | IPC_CREAT); //crea una cola de mensajes (o la abre si ya existe)
    if (id_cola == -1) {
        perror("No puede acceder a la cola");
        exit(1);
    }

    printf("[Coordinador] Esperando mensajes...\n");
    while (1) {
        int id_generador = msg.generador_id;

        if (msgrcv(id_cola, &msg, sizeof(msg.generador_id), 0, 0) == -1) {
            perror("No puede recibir el mensaje");
            exit(1);
        }

        printf("[Coordinador] Recibido: %d\n", msg.generador_id);
        //GENERAR IDS
        generar_y_enviar_ids(pipe_respuesta, msg.generador_id);

        //CADA VEZ QUE LLEGA UN REGISTRO
        if(contador_registro >= cant_registros){
            break;
        }
        //GUARDAR EN CSV
        guardarAlumnoCSV(mem_comp, "alumnos.csv", contador_registro);
    }

    // Eliminar cola
    msgctl(id_cola, IPC_RMID, NULL);
}

void generar_y_enviar_ids(int (*pipe_respuesta)[2], int id_generador) {
    ListaIDs lista;
    lista.cantidad = TOTAL_IDS;
    
    close(pipe_respuesta[id_generador][LECTURA]);

    // 1. Sección crítica protegida por semáforo
    sem_wait(sem_bloque);
    int bloque = *bloque_actual_compartido;
    (*bloque_actual_compartido)++;
    sem_post(sem_bloque);

    // 2. Crear IDs consecutivos del bloque actual
    int inicio = bloque * TOTAL_IDS + 1;
    for (int i = 0; i < lista.cantidad; i++) {
        lista.ids[i] = inicio + i;
    }

    // 3. Mezclar (Fisher–Yates)
    for (int i = lista.cantidad - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = lista.ids[i];
        lista.ids[i] = lista.ids[j];
        lista.ids[j] = temp;
    }
    
    //Espero la respuesta con las 10 ids y lo guardo en la variable ids
    write(pipe_respuesta[id_generador][ESCRITURA], lista.ids, sizeof(int) * lista.cantidad);
}

void guardarAlumnoCSV(Alumno* mem_comp, const char* filename, int contador_registro) {
    FILE* fp = fopen(filename, "a");
    if (fp == NULL) {
        perror("Error abriendo archivo CSV");
        return;
    }

    // Escribir encabezado
    fprintf(fp, "id,nombre,apellido,anio,materia\n");

    // Escribir el alumno
    fprintf(fp, "%d,%s,%s,%d,%s\n",
            mem_comp->id,
            mem_comp->nombre,
            mem_comp->apellido,
            mem_comp->anio,
            mem_comp->materia);

    fclose(fp);
    printf("Alumno guardado en %s\n", filename);
    contador_registro++;
}