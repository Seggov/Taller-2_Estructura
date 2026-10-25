#include <iostream>
#include <string>
#include <limits>
#include "../include/Hospital.h"

using namespace std;

// prototipos para el menu
void menu();
bool leerNumero(int& valor);
bool selector(int& opcion);
void consultarDepartamento(Hospital& hospital);
void consultarArbol(Hospital& hospital);

int main() {
    Hospital hospital;

    // carga inicial de pacientes
    cout << "Iniciando sistema Hospital Marmaja..." << endl;
    hospital.cargarPacientes("data/data.txt");

    bool esTerminado = false;

    // ciclo del menu
    while (!esTerminado) {
        menu();
        int user_selector = 0;
        if (!selector(user_selector)) break;
        switch (user_selector) {
            // opcion atender
            case 1: {
                hospital.mostrarCola();
                if (hospital.getCantidadEnEspera() > 0) {
                    cout << "\nIndique la cantidad de pacientes a atender: ";
                    int cant = 0;
                    if (!leerNumero(cant)) {
                        esTerminado = true;
                        break;
                    }
                    hospital.atenderPacientes(cant);
                }
                break;
            }

            // opcion ver departamentos
            case 2: {
                consultarDepartamento(hospital);
                break;
            }

            // opcion buscar
            case 3: {
                cout << "\nIngrese el ID o Nombre a buscar: ";
                string criterio;
                if (!getline(cin, criterio)) {
                    esTerminado = true;
                    break;
                }
                hospital.buscarPaciente(criterio);
                break;
            }

            // opcion eliminar
            case 4: {
                cout << "\nIngrese el ID del paciente a eliminar: ";
                string id;
                if (!getline(cin, id)) {
                    esTerminado = true;
                    break;
                }
                if (hospital.eliminarPaciente(id)) cout << "Paciente eliminado del AVL." << endl;
                else cout << "No se encontro el ID en los servicios." << endl;
                break;
            }

            // opcion ver arbol de decision
            case 5: {
                consultarArbol(hospital);
                break;
            }

            // opcion salir
            case 6: {
                cout << "\nHasta luego :D." << endl;
                esTerminado = true;
                break;
            }

            // caso default
            default: {
                cout << "\nOpcion no valida. Intente nuevamente." << endl;
                break;
            }
        }
    }

    return 0;
}


// inputs para el menu por consola
void menu() {
    cout << "\n=== HOSPITAL MARMAJA ===" << endl;
    cout << "1. Atender pacientes" << endl;
    cout << "2. Ver departamento" << endl;
    cout << "3. Buscar paciente" << endl;
    cout << "4. Eliminar paciente" << endl;
    cout << "5. Ver arbol de decision" << endl;
    cout << "6. Salir" << endl;
}


// lee numero evitando errores
bool leerNumero(int& valor) {
    while (!(cin >> valor)) {
        if (cin.eof()) return false;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida. Ingrese un numero: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

// selector de opcion
bool selector(int& opcion) {
    cout << "\nSeleccionar opcion: ";
    return leerNumero(opcion);
}

// consulta el avl del departamento elegido
void consultarDepartamento(Hospital& hospital) {
    hospital.mostrarDepartamentos();
    cout << "\nSeleccione departamento (0 para volver): ";
    int indice = 0;
    if (!leerNumero(indice) || indice == 0) return;
    if (indice < 1 || indice > TOTAL_SERVICIOS) {
        cout << "[ERROR] Departamento no valido." << endl;
        return;
    }

    cout << "1. Inorden\n2. Preorden\n3. Postorden\n4. Altura\n5. Balance\n0. Volver" << endl;
    cout << "Seleccionar consulta: ";
    int opcion = 0;
    if (!leerNumero(opcion) || opcion == 0) return;
    hospital.mostrarEstadoDepartamento(indice, opcion);
}

// consulta los recorridos del clasificador
void consultarArbol(Hospital& hospital) {
    cout << "\n1. Inorden\n2. Preorden\n3. Postorden\n0. Volver" << endl;
    cout << "Seleccionar recorrido: ";
    int opcion = 0;
    if (!leerNumero(opcion) || opcion == 0) return;
    hospital.mostrarArbolDecision(opcion);
}
