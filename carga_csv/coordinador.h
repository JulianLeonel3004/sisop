#ifndef COORDINADOR_H
#define COORDINADOR_H

#include "definiciones.h"
#include <semaphore.h>
#define TOTAL_IDS 10

typedef struct {
    int ids[TOTAL_IDS];
    int cantidad;
} ListaIDs;

void coordinador(int (*pipe_respuesta)[2], Alumno* mem_comp, int cant_registros, int id_cola);
void generar_y_enviar_ids(int (*pipe_respuesta)[2], int id_generador);
void guardarAlumnoCSV(Alumno alumno, const char* filename, int contador_registro);

extern int *bloque_actual_compartido;
extern sem_t *sem_bloque;
extern sem_t *Mutex;

extern sem_t *alumno_leido;
extern sem_t *nuevo_alumno;

#endif
