#include "../include/Paciente.h"

// constructor vacio que llama al padre // para evitar el caso de que se imprima comienza todo en 0 o ""
Paciente::Paciente()
    : Persona(), servicio(""), temperatura(0.0), presionArterial(0), dolor(0),
      tipoDolor(""), trauma(0), sintomasNeurologicos(0), requiereCirugia(0),
      diasHospitalizacion(0) {}

// constructor con datos llamando a persona
Paciente::Paciente(string id, string nombre, int edad, double temperatura, int presionArterial, int dolor,
     string tipoDolor, int trauma, int sintomasNeurologicos, int requiereCirugia, int diasHospitalizacion) 
    : Persona(id, nombre, edad) {
    this->temperatura = temperatura;
    this->presionArterial = presionArterial;
    this->dolor = dolor;
    this->tipoDolor = tipoDolor;
    this->trauma = trauma;
    this->sintomasNeurologicos = sintomasNeurologicos;
    this->requiereCirugia = requiereCirugia;
    this->diasHospitalizacion = diasHospitalizacion;
}
//int
// destructor
Paciente::~Paciente() {}

// getters
string Paciente::getServicio() const {
    return servicio;
}
void Paciente::setServicio(string service) {this->servicio = service;}
double Paciente::getTemperatura() const {return temperatura;}
int Paciente::getPresionArterial() const {return presionArterial;}
int Paciente::getDolor() const {return dolor;}
string Paciente::getTipoDolor() const {return tipoDolor;}
int Paciente::getTrauma() const {return trauma;}
int Paciente::getSintomasNeurologicos() const {return sintomasNeurologicos;}
int Paciente::getRequiereCirugia() const {return requiereCirugia;}
int Paciente::getDiasHospitalizacion() const {return diasHospitalizacion;}

// muestra los datos completos
void Paciente::mostrar() const {
    cout << "[" << id << "] " << nombre << " | " << edad << " anios | " << servicio << endl;
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
    cout << "Dolor: " << dolor << endl;
    cout << "TipoDolor: " << tipoDolor << endl;
    cout << "Trauma: " << trauma << endl;
    cout << "SintomasNeurologicos: " << sintomasNeurologicos << endl;
    cout << "RequiereCirugia: " << requiereCirugia << endl;
    cout << "DiasHospitalizacion: " << diasHospitalizacion << endl;
    cout << "\nRuta: work in progressss . . ." << endl;
}

// out de departamento
void Paciente::mostrarEnDepartamento() const {
    cout << nombre << " (" << edad << ")" << endl;
}

// out del historial
void Paciente::mostrarEnHistorial() const {
    cout << "Nombre: " << nombre << " | Edad: " << edad << " | Departamento: " << servicio << endl;
}
