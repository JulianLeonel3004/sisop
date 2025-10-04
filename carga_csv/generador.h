#ifndef GENERADOR_H
#define GENERADOR_H

#include "definiciones.h"

void funcion_prueba_generador();
void generador(int (*pipe_peticion)[2], int (*pipe_respuesta)[2], int idx_pipe, Alumno* mem_comp);

#endif // GENERADOR_H
