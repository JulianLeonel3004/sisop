
/*
    validación de parametros:

    //parametros obligatorios
    //r cantida de regitros
    //g cantidad de generadores

    ./main -r 3 -g 2

    //En caso de necesitar ayuda
    //-h o --help debe dar una ayuda
    ./main -h
    ./main --help

    Hay que validar que los parametros sean adecuados y en caso contrario enviar un mensaje de error.
    Podemos basarnos en como git valida sus parametros o preguntarle a chatGPT


    Probablemente esta funcion deba devolver una estructura para cargar en el main los variables:

    int cant_registros;
    int cant_generadores;

    Que por ahora están hardcodeadas
 */
void funcion_prueba_parametros()
{
    printf("Soy parametros\n");
}