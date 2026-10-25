# Taller 2 — Hospital Marmaja

Extensión del Taller 1 en C++: clasificar pacientes mediante un árbol de decisión y almacenarlos en un AVL por servicio. Las estructuras se implementan manualmente.

## Integrantes

| Nombre | RUT | GitHub | Carrera | Paralelo |
| --- | --- | --- | --- | --- |
| Pedro Segovia | 21672694-4 | Seggov | Ingeniería en Tecnologías de la Información | C2 |
| Luis Molina Aguilera | 21564225-9 | mixolydiann | Ingeniería en Tecnologías de la Información | C2 |

## Flujo del sistema

```mermaid
flowchart LR
    A["Archivo de pacientes"] --> B["Cola manual · FIFO"]
    B --> C["Árbol de decisión"]
    C --> D["Servicio asignado · 0 a 7"]
    D --> E["AVL del servicio<br/>Clave: ID"]
```

`Hospital` coordina el procesamiento en orden de llegada. Cada servicio mantiene su propio AVL, ordenado por ID.

El menú permite atender, consultar departamentos, buscar por ID o nombre, eliminar por ID y salir. En cada departamento se puede consultar inorden, preorden, postorden, altura y balance del AVL.

Los IDs repetidos o servicios inválidos se informan y descartan sin registrar una atención.

**Pruebas locales:** rotaciones, búsquedas y eliminación verificadas. El seguimiento temporal de `new/delete` confirmó la liberación de los nodos de AVL, cola, servicios e historial.

Las comprobaciones del clasificador, FIFO y AVL se ejecutan siguiendo [tests/README.md](tests/README.md).

## Compilación y ejecución

Desde la raíz del taller, con g++ y C++17:

En Linux o Codespaces:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic src/*.cpp -o hospital
./hospital
```

En Windows con g++:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic src/Main.cpp src/Persona.cpp src/Paciente.cpp src/Hospital.cpp src/Estructura.cpp src/ArbolDecision.cpp src/ArbolAVL.cpp -o hospital.exe
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

## Servicios

El clasificador devuelve un código para que `Hospital` seleccione el servicio y su AVL:

| Código | Servicio |
| --- | --- |
| 0 | Urgencias |
| 1 | Medicina General |
| 2 | Cardiología |
| 3 | Neurología |
| 4 | Traumatología |
| 5 | Cirugía |
| 6 | Pediatría |
| 7 | Hospitalización |

Los códigos internos son `0–7`; la selección de departamentos en el menú usa posiciones `1–8`.

## Árbol de decisión

Implementado en `ArbolDecision` con nodos enlazados por punteros (`NodoDecision`). Cada nodo es:

- una **pregunta** sobre un atributo del paciente, con un hijo `si` y un hijo `no`, o
- una **hoja**, que guarda el código del servicio final.

`clasificar()` parte en la raíz, responde la pregunta de cada nodo y baja por `si` o `no` hasta llegar a una hoja. Retorna el código del servicio y la ruta recorrida, por ejemplo:

```text
Ruta: Edad >= 14 -> Trauma == 0 -> RequiereCirugia == 0 -> SintomasNeurologicos == 0 -> TipoDolor != pecho -> Temperatura < 38.5 -> Dolor < 8 -> PresionArterial < 180 -> DiasHospitalizacion < 2
Servicio asignado: Medicina General
```

### Criterio

```text
¿Edad >= 14?
|-- No: Pediatria
|
|-- Si:
    ¿Trauma == 1?
    |-- Si:
    |   ¿RequiereCirugia == 1?
    |   |-- Si: Cirugia
    |   |-- No: Traumatologia
    |
    |-- No:
        ¿RequiereCirugia == 1?
        |-- Si: Cirugia
        |
        |-- No:
            ¿SintomasNeurologicos == 1?
            |-- Si: Neurologia
            |
            |-- No:
                ¿TipoDolor == pecho?
                |-- Si: Cardiologia
                |
                |-- No:
                    ¿Temperatura >= 38.5?
                    |-- Si: Urgencias
                    |
                    |-- No:
                        ¿Dolor >= 8?
                        |-- Si: Urgencias
                        |
                        |-- No:
                            ¿PresionArterial >= 180?
                            |-- Si: Urgencias
                            |
                            |-- No:
                                ¿DiasHospitalizacion >= 2?
                                |-- Si: Hospitalizacion
                                |-- No: Medicina General
```

### Justificación de cada pregunta

| Orden | Pregunta | Justificación |
| --- | --- | --- |
| 1 | `Edad >= 14` | Los menores de 14 se atienden siempre en Pediatría. |
| 2 | `Trauma == 1` | Una lesión física define la especialidad antes que los síntomas asociados. |
| 3 | `RequiereCirugia == 1` (con trauma) | Separa el trauma quirúrgico (Cirugía) del que no lo es (Traumatología). |
| 4 | `RequiereCirugia == 1` (sin trauma) | Un paciente que requiere cirugía sin trauma (por ejemplo, una apendicitis) también va a Cirugía. Se agregó al ejemplo del enunciado para que este caso no termine en Medicina General. |
| 5 | `SintomasNeurologicos == 1` | Síntomas neurológicos sin trauma ni cirugía corresponden a Neurología. |
| 6 | `TipoDolor == pecho` | El dolor de pecho se trata como posible problema cardíaco. |
| 7–9 | `Temperatura >= 38.5`, `Dolor >= 8`, `PresionArterial >= 180` | Fiebre alta, dolor intenso o crisis hipertensiva requieren atención inmediata en Urgencias. La presión arterial se agregó al ejemplo del enunciado. |
| 10 | `DiasHospitalizacion >= 2` | Una estadía estimada de dos días o más corresponde a Hospitalización; si no, Medicina General. |

### Casos de empate

Cuando un paciente cumple condiciones de más de un servicio, gana la primera pregunta que responde `si` en el orden de la tabla:

| Caso | Servicio | Motivo |
| --- | --- | --- |
| Menor de 14 con cualquier síntoma | Pediatría | La edad se pregunta primero. |
| Trauma y síntomas neurológicos | Traumatología (o Cirugía si requiere cirugía) | El trauma es la causa más probable del síntoma neurológico. |
| Síntomas neurológicos y requiere cirugía | Cirugía | La necesidad quirúrgica se pregunta antes. |
| Dolor de pecho con fiebre o dolor intenso | Cardiología | Se prioriza descartar un problema cardíaco. |

Los umbrales usan `>=`: una temperatura de exactamente `38.5`, un dolor de `8`, una presión de `180` o `2` días de hospitalización cuentan como `si`.

### Datos inválidos o insuficientes

Las líneas con datos inválidos, campos faltantes o IDs repetidos se rechazan al cargar el archivo, asi que el árbol solo recibe pacientes con los once campos válidos. Si un paciente llegara a una rama sin hoja, `clasificar()` retorna `-1` y `Hospital` informa que no se pudo clasificar sin insertarlo en ningún AVL.

### Recorridos y memoria

El árbol implementa recorridos preorden (nodo, si, no), inorden (si, nodo, no) y postorden (si, no, nodo) con `mostrarPreorden`, `mostrarInorden` y `mostrarPostorden`. El destructor libera los nodos con un recorrido postorden: primero los hijos y después el padre. El árbol no se puede copiar, para evitar que dos objetos liberen los mismos nodos.
