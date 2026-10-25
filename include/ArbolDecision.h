#ifndef ARBOL_DECISION_H
#define ARBOL_DECISION_H

#include "Paciente.h"

// nodo para una pregunta o un servicio
struct NodoDecision {
    string texto;
    int criterio;
    int servicio;
    NodoDecision* izquierdo;
    NodoDecision* derecho;

    NodoDecision(string texto, int criterio, int servicio)
        : texto(texto), criterio(criterio), servicio(servicio),
          izquierdo(nullptr), derecho(nullptr) {}
};

// izquierda significa si, derecha significa no
class ArbolDecision {
private:
    NodoDecision* raiz;

    void liberar(NodoDecision* nodo);
    bool datosValidos(const Paciente& paciente) const;
    bool cumpleCriterio(int criterio, const Paciente& paciente) const;
    void inorden(NodoDecision* nodo) const;
    void preorden(NodoDecision* nodo) const;
    void postorden(NodoDecision* nodo) const;

public:
    ArbolDecision();
    ~ArbolDecision();

    // evita compartir los nodos al copiar
    ArbolDecision(const ArbolDecision&) = delete;
    ArbolDecision& operator=(const ArbolDecision&) = delete;

    void construir();

    // devuelve 0 a 7, o -1 si los datos no sirven
    int clasificar(const Paciente& paciente, string& ruta) const;
    void mostrarInorden() const;
    void mostrarPreorden() const;
    void mostrarPostorden() const;
};

#endif
