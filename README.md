# Taller 2 — Hospital Marmaja

Extensión del Taller 1 en C++: clasificar pacientes mediante un árbol de decisión y almacenarlos en un AVL por servicio. Las estructuras se implementan manualmente.

## Integrantes

| Nombre | RUT | GitHub | Carrera | Paralelo |
| --- | --- | --- | --- | --- |
| Pedro Segovia | 21672694-4 | Seggov | Ingeniería en Tecnologías de la Información | C2 |
| Luis Molina Aguilera | 21.564.225-9 | mixolydiann | Ingeniería en Tecnologías de la Información | C2 |

## Funcionamiento

```mermaid
flowchart LR
    A["Archivo de pacientes"] --> B["Cola manual · FIFO"]
    B --> C["Árbol de decisión"]
    C --> D["Servicio asignado · 0 a 7"]
    D --> E["AVL del servicio<br/>Clave: ID"]
```

`Hospital` coordina el procesamiento en orden de llegada. Cada servicio mantiene su propio AVL, ordenado por ID.

El menú permite atender, ver departamentos, buscar, eliminar, recorrer el árbol de decisión y salir. En cada departamento se puede consultar inorden, preorden, postorden, altura y balance.

Los IDs repetidos o servicios inválidos se informan y descartan sin registrar una atención.

Las [pruebas del taller](tests/README.md) comprueban clasificación, FIFO, duplicados, datos inválidos y operaciones AVL.

## Compilación y ejecución

Desde la raíz del taller, con g++ y C++17:

En Linux o Codespaces:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic src/*.cpp -o hospital
./hospital
```

En Windows con g++:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic src/Main.cpp src/Persona.cpp src/Paciente.cpp src/Hospital.cpp src/Estructura.cpp src/ArbolAVL.cpp src/ArbolDecision.cpp -o hospital.exe
.\hospital.exe
```

## Archivo de pacientes

Se carga `data/data.txt`: once campos separados por `;`, cabecera opcional y decimales con punto. El servicio no viene en el archivo.

```text
ID;Nombre;Edad;Temperatura;PresionArterial;Dolor;TipoDolor;Trauma;SintomasNeurologicos;RequiereCirugia;DiasHospitalizacion
001;Juan Perez;25;37.0;174;3;otro;0;0;0;0
```

- **Identificación:** ID y nombre obligatorios; el ID conserva sus ceros iniciales.
- **Valores numéricos:** edad entera de 0 a 130, temperatura positiva y finita, presión sistólica entera positiva, dolor entero de 0 a 10 y días de hospitalización enteros no negativos.
- **Tipo de dolor:** `pecho`, `cabeza`, `abdomen`, `extremidad` u `otro`. Trauma, síntomas neurológicos y cirugía aceptan exactamente `0` o `1`.
- **Errores:** se informa si el archivo no existe. Los registros inválidos o duplicados se omiten indicando línea y motivo; se conserva el primer registro válido de cada ID.

## Criterio del árbol de decisión

La primera condición verdadera determina el servicio. La rama izquierda representa «sí» y la derecha «no». Se utilizan ocho atributos del paciente.

| Prioridad | Condición | Servicio y código | Justificación |
| --- | --- | --- | --- |
| 1 | Edad < 14 | Pediatría · 6 | Separa a los menores antes de evaluar otros criterios. |
| 2 | RequiereCirugia = 1 | Cirugía · 5 | Incluye cirugía con o sin trauma. |
| 3 | Trauma = 1 | Traumatología · 4 | Recibe trauma que no requiere cirugía. |
| 4 | SintomasNeurologicos = 1 | Neurología · 3 | Distingue síntomas neurológicos del dolor aislado. |
| 5 | TipoDolor = pecho | Cardiología · 2 | Separa los casos de dolor de pecho. |
| 6 | Temperatura >= 38.5 o Dolor >= 8 | Urgencias · 0 | Agrupa fiebre o dolor intenso. |
| 7 | DiasHospitalizacion >= 2 | Hospitalización · 7 | Considera estancias de dos días o más. |
| 8 | Ninguna anterior | Medicina General · 1 | Recibe los casos generales restantes. |

Ante varias condiciones se respeta este orden: un menor que requiere cirugía queda en Pediatría; un adulto con cirugía y trauma queda en Cirugía. Estos criterios definen la clasificación del taller.

Los datos inválidos o insuficientes producen el código `-1` y se informa el rechazo. Los códigos válidos son `0–7`; la selección de departamentos en el menú usa posiciones `1–8` en el orden Urgencias, Medicina General, Cardiología, Neurología, Traumatología, Cirugía, Pediatría y Hospitalización.
