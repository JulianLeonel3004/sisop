#ifndef DEFINICION_H_INCLUDED
#define DEFINICION_H_INCLUDED

#include <stdbool.h>

// ---- Estructura de parámetros ----
typedef struct {
    int generadores;        // -g N (>=1)
    int total_registros;    // -r M (>=1)
    const char* salida_csv; // -o (default "datos.csv")
    bool mostrar_ayuda;     // -h / --help
} Params;

// ---- Prototipos ----
bool parse_params(int argc, char** argv, Params* out);
bool genera_readme(const Params* p);

// Funciones de prueba
void funcion_prueba_parametros(void);
void funcion_prueba_generador(void);
void funcion_prueba_coordinador(void);

#endif // DEFINICION_H_INCLUDED
