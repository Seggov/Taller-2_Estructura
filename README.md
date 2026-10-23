# Taller 2 — Hospital Marmaja

Extensión del Taller 1 en C++: clasificar pacientes mediante un árbol de decisión y almacenarlos en un AVL por servicio. Las estructuras se implementan manualmente.

## Integrantes

| Nombre | RUT | GitHub | Carrera | Paralelo |
| --- | --- | --- | --- | --- |
| Pedro Segovia | 21672694-4 | Seggov | Ingeniería en Tecnologías de la Información | C2 |
| Luis Molina Aguilera | 21.564.225-9 | mixolydiann | Ingeniería en Tecnologías de la Información | C2 |

## Flujo previsto

```mermaid
flowchart LR
    A["Archivo de pacientes"] --> B["Cola manual · FIFO"]
    B --> C["Árbol de decisión"]
    C --> D["Servicio asignado · 0 a 7"]
    D --> E["AVL del servicio<br/>Clave: ID"]
```

`Hospital` coordina el procesamiento en orden de llegada. Cada servicio tendrá su propio AVL, ordenado por ID.

**Implementado:** lectura y validación de pacientes, cola FIFO, búsqueda por ID o nombre y menú. El AVL tiene nodo, constructor, destructor y consultas básicas.

**Pendiente:** árbol de decisión y justificación de sus reglas; operaciones e integración de los AVL; pruebas en Codespaces y completar paralelos. Por ahora, atender conserva los pacientes sin clasificar en la cola y muestra un aviso.

## Compilación y ejecución

Desde la raíz del taller, con g++ y C++17:

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

El clasificador devolverá un código para que `Hospital` seleccione el servicio y su AVL:

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
