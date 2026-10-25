#ifndef ARBOL_DECISION_H
#define ARBOL_DECISION_H

#include <string>
#include "Paciente.h"

using namespace std;

// nodo del arbol de decision
// hay dos tipos de nodo:
//   - pregunta: mira un atributo del paciente y sigue por "si" o por "no"
//   - hoja: no pregunta nada, solo guarda el servicio final (0..7)
struct NodoDecision {
    // ---- datos de la pregunta (solo nodos pregunta) ----
    string atributo;   // dato del paciente que se revisa: "Edad", "Trauma", "TipoDolor", ...
    double umbral;     // la pregunta es: valor del atributo >= umbral
    string texto;      // solo para TipoDolor: valor esperado (ej. "pecho")

    // ---- dato de la hoja ----
    int servicio;      // indice del servicio (0..7) si es hoja, -1 si es pregunta

    // ---- hijos ----
    NodoDecision* si;  // camino cuando la respuesta es si
    NodoDecision* no;  // camino cuando la respuesta es no

    // pregunta numerica: atributo >= umbral
    NodoDecision(string atributo, double umbral)
        : atributo(atributo), umbral(umbral), texto(""), servicio(-1), si(nullptr), no(nullptr) {}

    // pregunta de texto: atributo == texto
    NodoDecision(string atributo, string texto)
        : atributo(atributo), umbral(0), texto(texto), servicio(-1), si(nullptr), no(nullptr) {}

    // hoja con el servicio final
    NodoDecision(int servicio)
        : atributo(""), umbral(0), texto(""), servicio(servicio), si(nullptr), no(nullptr) {}

    // true si el nodo es una hoja
    bool esHoja() const { return servicio != -1; }
};

// arbol de decision que clasifica pacientes en un servicio
class ArbolDecision {
private:
    NodoDecision* raiz;

    // arma todas las preguntas y hojas del arbol
    void construir();

    // libera los nodos en postorden (primero los hijos, despues el padre)
    void liberar(NodoDecision* nodo);

    // responde la pregunta de un nodo para un paciente
    bool responder(const NodoDecision* nodo, const Paciente& paciente) const;
    double valorNumerico(const Paciente& paciente, const string& atributo) const;

    // texto de una pregunta respondida (ej. "Edad >= 14" o "Trauma == 0")
    string describir(const NodoDecision* nodo, bool respuesta) const;

    // texto de un nodo para los recorridos (ej. "Edad >= 14?" o "[Pediatria]")
    string textoNodo(const NodoDecision* nodo, const string* nombres) const;

    // recorridos recursivos (nivel = profundidad, para la sangria)
    void preorden(const NodoDecision* nodo, const string* nombres, int nivel) const;
    void inorden(const NodoDecision* nodo, const string* nombres, int nivel) const;
    void postorden(const NodoDecision* nodo, const string* nombres, int nivel) const;

public:
    // constructor: construye el arbol completo
    ArbolDecision();

    // destructor: libera todos los nodos
    ~ArbolDecision();

    // evita copias: dos arboles apuntando a los mismos nodos los borrarian dos veces
    ArbolDecision(const ArbolDecision&) = delete;
    ArbolDecision& operator=(const ArbolDecision&) = delete;

    // evalua al paciente desde la raiz hasta una hoja
    // retorna el indice del servicio (0..7) y deja en ruta el camino recorrido
    int clasificar(const Paciente& paciente, string& ruta) const;

    // recorridos; nombres = arreglo con el nombre de cada servicio (indice 0..7)
    void mostrarPreorden(const string* nombres) const;
    void mostrarInorden(const string* nombres) const;
    void mostrarPostorden(const string* nombres) const;
};

#endif
