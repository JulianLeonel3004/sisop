#ifndef COLA_H
#define COLA_H

#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLAVE_COLA 1234  // clave única para la cola

// Estructura de mensaje
struct mensaje {
    int generador_id;
};

#endif

