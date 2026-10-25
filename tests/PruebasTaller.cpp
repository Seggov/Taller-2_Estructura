#include <iostream>
#include <limits>
#include "../include/Hospital.h"

using namespace std;

int errores = 0;

// muestra solamente las comprobaciones que fallan
void comprobar(bool resultado, string mensaje) {
    if (!resultado) {
        cerr << "[FALLO] " << mensaje << endl;
        errores++;
    }
}

Paciente crearPaciente(string id) {
    return Paciente(id, "Paciente " + id, 25, 37.0, 120, 3,
                    "otro", false, false, false, 0);
}

// revisa servicios, limites y prioridades
void probarClasificacion() {
    ArbolDecision arbol;
    Paciente casos[8] = {
        Paciente("001", "Pediatria", 13, 37, 120, 3, "otro", false, false, false, 0),
        Paciente("002", "Cirugia", 25, 37, 120, 3, "abdomen", false, false, true, 0),
        Paciente("003", "Trauma", 25, 37, 120, 3, "extremidad", true, false, false, 0),
        Paciente("004", "Neuro", 25, 37, 120, 3, "cabeza", false, true, false, 0),
        Paciente("005", "Cardio", 25, 37, 120, 3, "pecho", false, false, false, 0),
        Paciente("006", "Urgencias", 25, 38.5, 120, 3, "otro", false, false, false, 0),
        Paciente("007", "Hospitalizacion", 25, 37, 120, 3, "otro", false, false, false, 2),
        Paciente("008", "Medicina", 14, 38.4, 120, 7, "otro", false, false, false, 1)
    };
    int esperados[8] = {6, 5, 4, 3, 2, 0, 7, 1};
    string ruta;
    for (int i = 0; i < 8; i++) {
        comprobar(arbol.clasificar(casos[i], ruta) == esperados[i], "clasificar " + casos[i].getNombre());
        comprobar(ruta.find("Edad < 14") != string::npos, "ruta desde la raiz");
    }

    Paciente dolor("009", "Dolor", 25, 37, 120, 8, "otro", false, false, false, 0);
    comprobar(arbol.clasificar(dolor, ruta) == 0, "limite de dolor 8");
    Paciente prioridad("010", "Prioridad", 25, 39, 120, 9, "pecho", true, true, true, 3);
    comprobar(arbol.clasificar(prioridad, ruta) == 5, "cirugia antes de trauma y otros sintomas");
    Paciente menor("011", "Menor", 13, 39, 120, 9, "pecho", true, true, true, 3);
    comprobar(arbol.clasificar(menor, ruta) == 6, "edad antes de otros criterios");
    Paciente invalido;
    comprobar(arbol.clasificar(invalido, ruta) == -1, "datos insuficientes");
    Paciente noFinito("012", "Invalido", 25, numeric_limits<double>::quiet_NaN(),
                     120, 3, "otro", false, false, false, 0);
    comprobar(arbol.clasificar(noFinito, ruta) == -1, "temperatura no finita");
    arbol.construir();
    comprobar(arbol.clasificar(casos[7], ruta) == 1, "reconstruccion del arbol");
    cout << "[OK] Comprobaciones del clasificador terminadas." << endl;
}

// revisa las cuatro rotaciones y la eliminacion
void probarAVL() {
    string secuencias[4][3] = {
        {"030", "020", "010"}, {"010", "020", "030"},
        {"030", "010", "020"}, {"010", "030", "020"}
    };
    for (int caso = 0; caso < 4; caso++) {
        ArbolAVL arbol;
        for (int i = 0; i < 3; i++) comprobar(arbol.insertar(crearPaciente(secuencias[caso][i])), "insertar en avl");
        comprobar(arbol.size() == 3 && arbol.obtenerAltura() == 2, "rotacion " + to_string(caso + 1));
        comprobar(!arbol.insertar(crearPaciente("020")), "rechazo de id repetido");
        for (int i = 0; i < 3; i++) {
            int balance = 99;
            comprobar(arbol.consultarBalance(secuencias[caso][i], balance) && balance == 0,
                      "balance despues de rotar");
        }
        comprobar(arbol.eliminar("020") && arbol.size() == 2, "eliminar nodo con dos hijos");
        comprobar(!arbol.eliminar("020"), "eliminar id inexistente");
        Paciente encontrado;
        comprobar(arbol.buscarPorId("010", encontrado) && encontrado.getNombre() == "Paciente 010", "datos restantes");
        comprobar(arbol.buscarPorNombre("Paciente 030", encontrado), "buscar por nombre");
        comprobar(arbol.eliminar("010") && arbol.eliminar("030"), "vaciar arbol");
        comprobar(arbol.isEmpty() && arbol.obtenerAltura() == 0, "altura del arbol vacio");
    }

    ArbolAVL ordenado;
    for (int i = 0; i < 100; i++) comprobar(ordenado.insertar(crearPaciente(to_string(1000 + i))), "insercion ordenada");
    for (int i = 0; i < 100; i += 2) comprobar(ordenado.eliminar(to_string(1000 + i)), "eliminacion intercalada");
    comprobar(ordenado.size() == 50, "cantidad despues de eliminar");
    for (int i = 1; i < 100; i += 2) {
        int balance = 99;
        comprobar(ordenado.consultarBalance(to_string(1000 + i), balance) && balance >= -1 && balance <= 1,
                  "balance conservado");
    }
    cout << "[OK] Comprobaciones de AVL terminadas." << endl;
}

// procesa de a uno para comprobar el orden fifo
void probarIntegracion() {
    Hospital hospital;
    comprobar(hospital.cargarPacientes("tests/data/pacientes_integracion.txt"), "cargar casos de integracion");
    comprobar(hospital.getCantidadEnEspera() == 8, "ocho pacientes en cola");
    hospital.atenderPacientes(0);
    comprobar(hospital.getCantidadEnEspera() == 8, "cantidad invalida no retira pacientes");
    string ids[8] = {"001", "002", "003", "004", "005", "006", "007", "008"};
    string servicios[8] = {"Urgencias", "Medicina General", "Cardiologia", "Neurologia",
                          "Traumatologia", "Cirugia", "Pediatria", "Hospitalizacion"};
    for (int i = 0; i < 8; i++) {
        hospital.atenderPacientes(1);
        comprobar(hospital.getCantidadEnEspera() == 7 - i, "orden de salida de la cola");
        Paciente encontrado;
        string servicio;
        comprobar(hospital.buscarPaciente(ids[i], encontrado, servicio), "buscar paciente atendido");
        comprobar(servicio == servicios[i] && encontrado.getServicio() == servicios[i], "avl del servicio correcto");
        for (int codigo = 0; codigo < 8; codigo++) {
            comprobar(hospital.getCantidadEnServicio(codigo) == (codigo <= i ? 1 : 0), "cantidad por servicio");
        }
        if (i < 7) {
            comprobar(hospital.buscarPaciente(ids[i + 1], encontrado, servicio) && servicio == "En espera",
                      "el siguiente paciente permanece en cola");
        }
    }

    comprobar(!hospital.cargarPacientes("tests/data/pacientes_integracion.txt"), "rechazo de duplicados en servicios");
    comprobar(hospital.getCantidadEnEspera() == 0, "duplicados no vuelven a la cola");
    comprobar(hospital.eliminarPaciente("002") && hospital.getCantidadEnServicio(1) == 0, "eliminar desde hospital");
    comprobar(hospital.cargarPacientes("tests/data/pacientes_integracion.txt"), "recargar id eliminado");
    comprobar(hospital.getCantidadEnEspera() == 1, "solo se recarga el id eliminado");
    hospital.atenderPacientes(1);
    for (int i = 0; i < 8; i++) comprobar(hospital.eliminarPaciente(ids[i]), "eliminar de cada servicio");
    for (int codigo = 0; codigo < 8; codigo++) comprobar(hospital.getCantidadEnServicio(codigo) == 0, "servicio vacio");
    comprobar(!hospital.eliminarPaciente("999"), "id no encontrado");

    Hospital datosInvalidos;
    comprobar(!datosInvalidos.cargarPacientes("tests/data/no_existe.txt"), "archivo inexistente");
    comprobar(datosInvalidos.cargarPacientes("tests/data/pacientes_errores.txt"), "carga con errores");
    comprobar(datosInvalidos.getCantidadEnEspera() == 2, "solo dos registros validos");
    datosInvalidos.atenderPacientes(10);
    comprobar(datosInvalidos.getCantidadEnServicio(1) == 1 && datosInvalidos.getCantidadEnServicio(6) == 1,
              "procesar los registros validos");
    cout << "[OK] Comprobaciones de integracion terminadas." << endl;
}

int main() {
    probarClasificacion();
    probarAVL();
    probarIntegracion();
    if (errores != 0) {
        cerr << "Pruebas terminadas con " << errores << " errores." << endl;
        return 1;
    }
    cout << "Todas las pruebas pasaron." << endl;
    return 0;
}
