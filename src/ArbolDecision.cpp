#include "../include/ArbolDecision.h"
#include <cmath>

// construye el arbol al iniciar
ArbolDecision::ArbolDecision() : raiz(nullptr) {
    construir();
}

ArbolDecision::~ArbolDecision() {
    liberar(raiz);
}

// libera primero los hijos
void ArbolDecision::liberar(NodoDecision* nodo) {
    if (nodo == nullptr) return;
    liberar(nodo->izquierdo);
    liberar(nodo->derecho);
    delete nodo;
}

// arma desde la ultima pregunta hasta la raiz
void ArbolDecision::construir() {
    liberar(raiz);
    raiz = nullptr;

    NodoDecision* dias = new NodoDecision("DiasHospitalizacion >= 2", 7, -1);
    dias->izquierdo = new NodoDecision("Hospitalizacion", 0, 7);
    dias->derecho = new NodoDecision("Medicina General", 0, 1);

    NodoDecision* urgencia = new NodoDecision("Temperatura >= 38.5 o Dolor >= 8", 6, -1);
    urgencia->izquierdo = new NodoDecision("Urgencias", 0, 0);
    urgencia->derecho = dias;

    NodoDecision* pecho = new NodoDecision("TipoDolor == pecho", 5, -1);
    pecho->izquierdo = new NodoDecision("Cardiologia", 0, 2);
    pecho->derecho = urgencia;

    NodoDecision* neuro = new NodoDecision("SintomasNeurologicos == 1", 4, -1);
    neuro->izquierdo = new NodoDecision("Neurologia", 0, 3);
    neuro->derecho = pecho;

    NodoDecision* trauma = new NodoDecision("Trauma == 1", 3, -1);
    trauma->izquierdo = new NodoDecision("Traumatologia", 0, 4);
    trauma->derecho = neuro;

    NodoDecision* cirugia = new NodoDecision("RequiereCirugia == 1", 2, -1);
    cirugia->izquierdo = new NodoDecision("Cirugia", 0, 5);
    cirugia->derecho = trauma;

    raiz = new NodoDecision("Edad < 14", 1, -1);
    raiz->izquierdo = new NodoDecision("Pediatria", 0, 6);
    raiz->derecho = cirugia;
}

// mantiene las mismas reglas de datos que la carga
bool ArbolDecision::datosValidos(const Paciente& paciente) const {
    if (paciente.getId().empty() || paciente.getNombre().empty()) return false;
    if (paciente.getEdad() < 0 || paciente.getEdad() > 130) return false;
    if (!std::isfinite(paciente.getTemperatura()) || paciente.getTemperatura() <= 0) return false;
    if (paciente.getPresionArterial() <= 0) return false;
    if (paciente.getDolor() < 0 || paciente.getDolor() > 10) return false;
    if (paciente.getDiasHospitalizacion() < 0) return false;

    string tipo = paciente.getTipoDolor();
    return tipo == "pecho" || tipo == "cabeza" || tipo == "abdomen" ||
           tipo == "extremidad" || tipo == "otro";
}

// evalua la pregunta del nodo actual
bool ArbolDecision::cumpleCriterio(int criterio, const Paciente& paciente) const {
    switch (criterio) {
        case 1: return paciente.getEdad() < 14;
        case 2: return paciente.getRequiereCirugia();
        case 3: return paciente.getTrauma();
        case 4: return paciente.getSintomasNeurologicos();
        case 5: return paciente.getTipoDolor() == "pecho";
        case 6: return paciente.getTemperatura() >= 38.5 || paciente.getDolor() >= 8;
        case 7: return paciente.getDiasHospitalizacion() >= 2;
    }
    return false;
}

// sigue las preguntas hasta una hoja
int ArbolDecision::clasificar(const Paciente& paciente, string& ruta) const {
    ruta = "";
    if (!datosValidos(paciente)) {
        ruta = "Datos invalidos o insuficientes";
        return -1;
    }

    const NodoDecision* actual = raiz;
    while (actual != nullptr && actual->servicio == -1) {
        bool respuesta = cumpleCriterio(actual->criterio, paciente);
        if (!ruta.empty()) ruta += " -> ";
        ruta += actual->texto;
        if (respuesta) ruta += " [Si]";
        else ruta += " [No]";
        actual = respuesta ? actual->izquierdo : actual->derecho;
    }

    if (actual == nullptr) {
        ruta += " -> Servicio no encontrado";
        return -1;
    }
    ruta += " -> " + actual->texto;
    return actual->servicio;
}

void ArbolDecision::mostrarInorden() const {
    inorden(raiz);
}

void ArbolDecision::inorden(NodoDecision* nodo) const {
    if (nodo == nullptr) return;
    inorden(nodo->izquierdo);
    cout << nodo->texto << endl;
    inorden(nodo->derecho);
}

void ArbolDecision::mostrarPreorden() const {
    preorden(raiz);
}

void ArbolDecision::preorden(NodoDecision* nodo) const {
    if (nodo == nullptr) return;
    cout << nodo->texto << endl;
    preorden(nodo->izquierdo);
    preorden(nodo->derecho);
}

void ArbolDecision::mostrarPostorden() const {
    postorden(raiz);
}

void ArbolDecision::postorden(NodoDecision* nodo) const {
    if (nodo == nullptr) return;
    postorden(nodo->izquierdo);
    postorden(nodo->derecho);
    cout << nodo->texto << endl;
}
