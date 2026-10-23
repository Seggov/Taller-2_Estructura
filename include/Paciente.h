#ifndef PACIENTE_H
#define PACIENTE_H

#include "Persona.h"

// clase paciente que hereda de persona
class Paciente : public Persona {
private:
    string servicio; // servicio ahora deberá ser asignado post arbolin
    double temperatura; 
    int presionArterial;
    int dolor;
    string tipoDolor;
    int trauma;
    int sintomasNeurologicos;
    int requiereCirugia;
    int diasHospitalizacion;


public:
    // constructores
    Paciente();
    Paciente(string id, string nombre, int edad, double temperatura, int presionArterial,
    int dolor,string tipoDolor, int trauma, int sintomasNeurologicos, int requiereCirugia, int diasHospitalizacion);

    // destructor
    virtual ~Paciente() override;

    // info
    void setServicio(string service);
    string getServicio() const;
    double getTemperatura() const;
    int getPresionArterial() const;
    int getDolor() const;
    string getTipoDolor() const;
    int getTrauma() const;
    int getSintomasNeurologicos() const;
    int getRequiereCirugia() const;
    int getDiasHospitalizacion() const;


    // sobreescritura de mostrar
    void mostrar() const override;

    // metodos de out para cada caso del taller
    void mostrarEnFila(int correlativo) const;
    void mostrarEnAtencion() const;
    void mostrarEnDepartamento() const;
    void mostrarEnHistorial() const;
};

#endif
