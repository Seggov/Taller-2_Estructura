#ifndef PACIENTE_H
#define PACIENTE_H

#include "Persona.h"

// clase paciente que hereda de persona
class Paciente : public Persona {
private:
    // datos para la clasificacion
    double temperatura;
    int presionArterial;
    int dolor;
    string tipoDolor;
    bool trauma;
    bool sintomasNeurologicos;
    bool requiereCirugia;
    int diasHospitalizacion;

    // departamento asignado
    string servicio;

public:
    // constructores
    Paciente();
    Paciente(string id, string nombre, int edad, string servicio);
    Paciente(string id, string nombre, int edad, double temperatura,
             int presionArterial, int dolor, string tipoDolor, bool trauma,
             bool sintomasNeurologicos, bool requiereCirugia, int diasHospitalizacion);

    // destructor
    virtual ~Paciente() override;

    // getters de los datos clinicos
    double getTemperatura() const;
    int getPresionArterial() const;
    int getDolor() const;
    string getTipoDolor() const;
    bool getTrauma() const;
    bool getSintomasNeurologicos() const;
    bool getRequiereCirugia() const;
    int getDiasHospitalizacion() const;

    // get servicio
    string getServicio() const;

    // asigna el servicio despues de clasificar
    void setServicio(string servicio);

    // sobreescritura de mostrar
    void mostrar() const override;

    // metodos de out para cada caso del taller
    void mostrarEnFila(int correlativo) const;
    void mostrarEnAtencion() const;
    void mostrarEnDepartamento() const;
    void mostrarEnHistorial() const;
};

#endif
