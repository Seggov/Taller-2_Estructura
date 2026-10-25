# Pruebas del taller

Ejecutar desde la raiz del taller. Se comprueban los ocho servicios, limites y prioridades del clasificador, sus tres recorridos, FIFO, duplicados, archivos invalidos y operaciones AVL.

Linux o Codespaces:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic tests/PruebasTaller.cpp src/Persona.cpp src/Paciente.cpp src/ArbolDecision.cpp src/ArbolAVL.cpp src/Estructura.cpp src/Hospital.cpp -o pruebas_taller
./pruebas_taller
```

En Windows, usar `-o pruebas_taller.exe` y ejecutar `.\pruebas_taller.exe`.

En el programa principal, la opcion 5 permite elegir el recorrido del clasificador y la opcion 6 permite salir. Los submenus aceptan 0 para volver y el programa termina si se cierra la entrada.

Los mensajes de rechazo corresponden a los casos invalidos del archivo de prueba. El resultado final debe ser `Todas las pruebas pasaron.` y el codigo de salida `0`.

En `pacientes_integracion.txt`, los ID `001` a `008` corresponden a Urgencias, Medicina General, Cardiologia, Neurologia, Traumatologia, Cirugia, Pediatria y Hospitalizacion, respectivamente.

En `pacientes_busqueda.txt`, algunos nombres coinciden con otros ID. Se comprueba que el ID tenga prioridad en la cola y en los servicios sin cambiar el orden de llegada.
