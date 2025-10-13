# Compilar
compilar:
		gcc -o cargador_file main.c coordinador.c generador.c parametros.c  sincronizacion.c

# Ejecutar con parámetros -r38 -g4 // predeterminado
run:
	./cargador_file -r38 -g4

# Mostrar ayuda
help:
	./cargador_file -h

# Monitorear procesos
monitor-proc:
	ps aux | grep cargador_file

# Monitorear IPC
monitor-ipc:
	ipcs -a

# Monitorear semáforos
monitor-sem:
	ls /dev/shm/sem.*

# Limpiar
clean:
	rm -f cargador_file *.o
