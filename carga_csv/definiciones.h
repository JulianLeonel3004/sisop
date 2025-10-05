#ifndef DEFINICIONES_H
#define DEFINICIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>       // Para shmget(), shmat(), shmdt(), shmctl()
#include <semaphore.h>
#include <fcntl.h>           // Para las flags O_CREAT, O_EXCL
#define MAX_STR 10
typedef struct {
    int id;
    char nombre[MAX_STR + 1];   // +1 para el '\0' (fin de cadena)
    char apellido[MAX_STR + 1];
    int anio;
    char materia[MAX_STR + 1];
} Alumno;

#endif // DEFINICIONES_H
