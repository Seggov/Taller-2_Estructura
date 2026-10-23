#include "../include/Paciente.h"

// constructor vacio que llama al padre
Paciente::Paciente()
    : Persona(), temperatura(0), presionArterial(0), dolor(0), tipoDolor("otro"),
      trauma(false), sintomasNeurologicos(false), requiereCirugia(false),
      diasHospitalizacion(0), servicio("") {}

// constructor con datos llamando a persona
Paciente::Paciente(string id, string nombre, int edad, string servicio)
    : Persona(id, nombre, edad), temperatura(0), presionArterial(0), dolor(0),
      tipoDolor("otro"), trauma(false), sintomasNeurologicos(false),
      requiereCirugia(false), diasHospitalizacion(0), servicio(servicio) {}

// constructor con los datos del archivo nuevo
Paciente::Paciente(string id, string nombre, int edad, double temperatura,
                   int presionArterial, int dolor, string tipoDolor, bool trauma,
                   bool sintomasNeurologicos, bool requiereCirugia, int diasHospitalizacion)
    : Persona(id, nombre, edad), temperatura(temperatura), presionArterial(presionArterial),
      dolor(dolor), tipoDolor(tipoDolor), trauma(trauma),
      sintomasNeurologicos(sintomasNeurologicos), requiereCirugia(requiereCirugia),
      diasHospitalizacion(diasHospitalizacion), servicio("") {}

// destructor
Paciente::~Paciente() {}

// devuelve la temperatura
double Paciente::getTemperatura() const {
    return temperatura;
}

// devuelve la presion arterial
int Paciente::getPresionArterial() const {
    return presionArterial;
}

// devuelve el nivel de dolor
int Paciente::getDolor() const {
    return dolor;
}

// devuelve el tipo de dolor
string Paciente::getTipoDolor() const {
    return tipoDolor;
}

// revisa si tiene trauma
bool Paciente::getTrauma() const {
    return trauma;
}

// revisa si tiene sintomas neurologicos
bool Paciente::getSintomasNeurologicos() const {
    return sintomasNeurologicos;
}

// revisa si requiere cirugia
bool Paciente::getRequiereCirugia() const {
    return requiereCirugia;
}

// devuelve los dias de hospitalizacion
int Paciente::getDiasHospitalizacion() const {
    return diasHospitalizacion;
}

// devuelve el servicio
string Paciente::getServicio() const {
    return servicio;
}

// asigna el servicio despues de clasificar
void Paciente::setServicio(string servicio) {
    this->servicio = servicio;
}

// muestra los datos completos
void Paciente::mostrar() const {
    cout << "[" << id << "] " << nombre << " | " << edad << " anios | ";
    if (servicio.empty()) cout << "Pendiente de clasificacion";
    else cout << servicio;
    cout << endl;

    cout << "T: " << temperatura << " | PA: " << presionArterial
         << " | D: " << dolor << " | TD: " << tipoDolor
         << " | Trauma: " << trauma << " | SN: " << sintomasNeurologicos
         << " | RC: " << requiereCirugia << " | DH: " << diasHospitalizacion << endl;
}

// out para la cola de espera
void Paciente::mostrarEnFila(int correlativo) const {
    cout << correlativo << ". " << id << " - " << nombre << endl;
}

// out para cuando se atiende
void Paciente::mostrarEnAtencion() const {
    cout << "ID: " << id << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Edad: " << edad << endl;
    cout << "Temperatura: " << temperatura << endl;
    cout << "Presion arterial: " << presionArterial << endl;
    cout << "Dolor: " << dolor << endl;
    cout << "Tipo de dolor: " << tipoDolor << endl;
    cout << "Trauma: " << trauma << endl;
    cout << "Sintomas neurologicos: " << sintomasNeurologicos << endl;
    cout << "Requiere cirugia: " << requiereCirugia << endl;
    cout << "Dias de hospitalizacion: " << diasHospitalizacion << endl;
    if (servicio.empty()) {
        cout << "Servicio: Pendiente de clasificacion" << endl;
        return;
    }
    cout << "Servicio: " << servicio << endl;
    cout << "Paciente enviado a " << servicio << "." << endl;
}

// out de departamento
void Paciente::mostrarEnDepartamento() const {
    cout << nombre << " (" << edad << ")" << endl;
}

// out del historial
void Paciente::mostrarEnHistorial() const {
    cout << "Nombre: " << nombre << " | Edad: " << edad << " | Departamento: " << servicio << endl;
}
