# Taller 2 - Estructura de Datos

Hospital Marmaja. Proyecto en desarrollo para extender el Taller 1 con un arbol de decision y un AVL por servicio.

## Integrantes

| Nombre | RUT | GitHub | Carrera | Paralelo |
| --- | --- | --- | --- | --- |
| Pedro Segovia | 21672694-4 | Seggov | Ingenieria en Tecnologias de la Informacion | Pendiente |
| Luis Molina Aguilera | 21.564.225-9 | mixolydiann | Ingenieria en Tecnologias de la Informacion | Pendiente |

Falta completar el paralelo de cada integrante antes de la entrega.

## Compilacion y ejecucion

Ejecutar desde la carpeta raiz del taller. Se requiere un compilador de C++17.

En Linux o Codespaces:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic src/*.cpp -o hospital
./hospital
```

En Windows con g++:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic src/Main.cpp src/Persona.cpp src/Paciente.cpp src/Hospital.cpp src/Estructura.cpp src/ArbolAVL.cpp -o hospital.exe
.\hospital.exe
```

La ejecucion en Codespaces debe comprobarse antes de entregar.

## Archivo de pacientes

El programa carga `data/data.txt`. Cada registro tiene exactamente once campos separados por punto y coma:

```text
ID;Nombre;Edad;Temperatura;PresionArterial;Dolor;TipoDolor;Trauma;SintomasNeurologicos;RequiereCirugia;DiasHospitalizacion
001;Juan Perez;25;37.0;174;3;otro;0;0;0;0
```

La cabecera es opcional. El servicio se asignara despues de evaluar al paciente con el arbol de decision.

Reglas de carga:

- ID y nombre obligatorios. El ID se conserva como texto, incluidos sus ceros iniciales.
- Edad entera entre 0 y 130.
- Temperatura positiva y finita. Los decimales se escriben con punto.
- Presion arterial: presion sistolica representada por un entero positivo.
- Dolor entero entre 0 y 10.
- Tipo de dolor: pecho, cabeza, abdomen, extremidad u otro.
- Trauma, sintomas neurologicos y requiere cirugia: exactamente `0` o `1`.
- Dias de hospitalizacion: entero no negativo.
- Los IDs duplicados en la cola o los servicios se rechazan. Se conserva el primer registro valido.
- Las lineas invalidas se omiten indicando numero de linea y motivo; la lectura continua.

## Codigos de los servicios

El clasificador debera devolver uno de estos codigos. `Hospital` lo utilizara para seleccionar el servicio y su AVL:

| Codigo | Servicio |
| --- | --- |
| 0 | Urgencias |
| 1 | Medicina General |
| 2 | Cardiologia |
| 3 | Neurologia |
| 4 | Traumatologia |
| 5 | Cirugia |
| 6 | Pediatria |
| 7 | Hospitalizacion |

Este orden corresponde al catalogo de `Hospital`. La seleccion actual de departamentos en el menu utiliza posiciones de 1 a 8.

## Estado actual

- Lectura de los once campos, validacion y carga en una cola manual FIFO.
- Consulta de pacientes por ID o nombre.
- Metodo `setServicio()` disponible para asignar el resultado de la clasificacion.
- Nodo y estructura base del AVL, con destructor y consultas de vacio, cantidad y altura.
- Menu de cuatro opciones: atender, ver departamentos, buscar y salir.

La clasificacion y las operaciones del AVL estan pendientes. Hasta conectar el clasificador, los pacientes permanecen en espera al intentar atenderlos.

Falta implementar y documentar el arbol de decision, sus prioridades y casos de prueba; completar el AVL y su integracion; y verificar el funcionamiento y la memoria en Codespaces.
