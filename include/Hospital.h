#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <iostream>
#include <string>
#include "Estructuras.h"

using namespace std;

// total de departamentos
// 0 Urgencias, 1 Medicina General, 2 Cardiologia, 3 Neurologia, 4 Traumatologia, 5 Cirugia, 6 Pediatria, 7 Hospitalizacion
const int TOTAL_SERVICIOS = 8;

// cantidad de campos por linea del archivo
const int NUM_CAMPOS = 11;

// tipos de dolor validos
const int TOTAL_TIPOS_DOLOR = 5;

// clase que controla todo el hospital
class Hospital {
private:
    // estructuras que manejan los datos
    ColaPacientes colaEspera;
    ListaServicios servicios;
    PilaHistorial historial;

    // arreglo para los servicios
    string nombresServicios[TOTAL_SERVICIOS];   

    // metodos con aritmetica de punteros
    void inicializarServicios(); // de aqui se saca la lista para encontrar el servicio con la enumeracion del 0-7 para el handshake entre nuestro trabajo independiente
    bool esServicioValido(string servLeido, string& servOficial) const;
    bool esTipoDolorValido(const string& leido, string& oficial) const;

    // conversion y control de datos invalidos
    bool convertirEntero(const string& texto, int minimo, int maximo, int& resultado) const;
    bool convertirDecimal(const string& texto, float minimo, float maximo, float& resultado) const;

    // arma un paciente desde una linea, si falla deja el motivo
    bool parsearLinea(string linea, Paciente& paciente, string& motivo) const;

public:
    Hospital();
    ~Hospital();

    // funciones principales
    bool cargarPacientes(string ruta);
    void atenderPacientes(int cantidad);
    void mostrarCola() const;
    void mostrarDepartamentos() const;
    void mostrarEstadoDepartamento(int indice) const;
    void mostrarHistorial() const;
    void buscarPaciente(string criterio) const;

    int getCantidadEnEspera() const;
};

#endif
