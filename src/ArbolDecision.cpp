#include "../include/ArbolDecision.h"
#include <iostream>
#include <sstream>   // ostringstream: arma texto como un StringBuilder de java

// indices de los servicios (mismo orden que Hospital::inicializarServicios)
const int URGENCIAS = 0;
const int MEDICINA_GENERAL = 1;
const int CARDIOLOGIA = 2;
const int NEUROLOGIA = 3;
const int TRAUMATOLOGIA = 4;
const int CIRUGIA = 5;
const int PEDIATRIA = 6;
const int HOSPITALIZACION = 7;

// constructor: el arbol queda listo apenas se crea
ArbolDecision::ArbolDecision() : raiz(nullptr) {
    construir();
}

// destructor: libera todos los nodos
ArbolDecision::~ArbolDecision() {
    liberar(raiz);
    raiz = nullptr;
}

// arma el arbol de arriba hacia abajo
//
// Edad >= 14?
// |-- no: Pediatria
// |-- si: Trauma == 1?
//         |-- si: RequiereCirugia == 1?
//         |       |-- si: Cirugia
//         |       |-- no: Traumatologia
//         |-- no: RequiereCirugia == 1?           (cirugia sin trauma, ej. apendicitis)
//                 |-- si: Cirugia
//                 |-- no: SintomasNeurologicos == 1?
//                         |-- si: Neurologia
//                         |-- no: TipoDolor == pecho?
//                                 |-- si: Cardiologia
//                                 |-- no: Temperatura >= 38.5?
//                                         |-- si: Urgencias
//                                         |-- no: Dolor >= 8?
//                                                 |-- si: Urgencias
//                                                 |-- no: PresionArterial >= 180?   (crisis hipertensiva)
//                                                         |-- si: Urgencias
//                                                         |-- no: DiasHospitalizacion >= 2?
//                                                                 |-- si: Hospitalizacion
//                                                                 |-- no: Medicina General
void ArbolDecision::construir() {
    // 1. raiz: los menores de 14 van siempre a pediatria
    raiz = new NodoDecision("Edad", 14);
    raiz->no = new NodoDecision(PEDIATRIA);

    // 2. trauma (se pregunta antes que los sintomas neurologicos)
    NodoDecision* trauma = new NodoDecision("Trauma", 1);
    raiz->si = trauma;

    // 2a. con trauma: cirugia o traumatologia
    NodoDecision* cirugiaConTrauma = new NodoDecision("RequiereCirugia", 1);
    trauma->si = cirugiaConTrauma;
    cirugiaConTrauma->si = new NodoDecision(CIRUGIA);
    cirugiaConTrauma->no = new NodoDecision(TRAUMATOLOGIA);

    // 2b. sin trauma pero requiere cirugia
    NodoDecision* cirugiaSinTrauma = new NodoDecision("RequiereCirugia", 1);
    trauma->no = cirugiaSinTrauma;
    cirugiaSinTrauma->si = new NodoDecision(CIRUGIA);

    // 3. sintomas neurologicos
    NodoDecision* neuro = new NodoDecision("SintomasNeurologicos", 1);
    cirugiaSinTrauma->no = neuro;
    neuro->si = new NodoDecision(NEUROLOGIA);

    // 4. dolor de pecho
    NodoDecision* pecho = new NodoDecision("TipoDolor", string("pecho"));
    neuro->no = pecho;
    pecho->si = new NodoDecision(CARDIOLOGIA);

    // 5. urgencia: fiebre alta, dolor intenso o presion muy alta (una pregunta por nodo)
    NodoDecision* fiebre = new NodoDecision("Temperatura", 38.5);
    pecho->no = fiebre;
    fiebre->si = new NodoDecision(URGENCIAS);

    NodoDecision* dolor = new NodoDecision("Dolor", 8);
    fiebre->no = dolor;
    dolor->si = new NodoDecision(URGENCIAS);

    NodoDecision* presion = new NodoDecision("PresionArterial", 180);
    dolor->no = presion;
    presion->si = new NodoDecision(URGENCIAS);

    // 6. estadia larga u hospitalizacion; si no, medicina general
    NodoDecision* dias = new NodoDecision("DiasHospitalizacion", 2);
    presion->no = dias;
    dias->si = new NodoDecision(HOSPITALIZACION);
    dias->no = new NodoDecision(MEDICINA_GENERAL);
}

// postorden: primero borra los hijos y al final el nodo
// (si se borrara el padre primero, se perderian los punteros a sus hijos)
void ArbolDecision::liberar(NodoDecision* nodo) {
    if (nodo == nullptr) return;
    liberar(nodo->si);
    liberar(nodo->no);
    delete nodo;
}

// valor numerico de un atributo (los bool se convierten a 1 o 0)
double ArbolDecision::valorNumerico(const Paciente& paciente, const string& atributo) const {
    if (atributo == "Edad") return paciente.getEdad();
    if (atributo == "Temperatura") return paciente.getTemperatura();
    if (atributo == "PresionArterial") return paciente.getPresionArterial();
    if (atributo == "Dolor") return paciente.getDolor();
    if (atributo == "Trauma") return paciente.getTrauma() ? 1 : 0;
    if (atributo == "SintomasNeurologicos") return paciente.getSintomasNeurologicos() ? 1 : 0;
    if (atributo == "RequiereCirugia") return paciente.getRequiereCirugia() ? 1 : 0;
    if (atributo == "DiasHospitalizacion") return paciente.getDiasHospitalizacion();
    return 0;
}

// responde la pregunta del nodo: true = si, false = no
bool ArbolDecision::responder(const NodoDecision* nodo, const Paciente& paciente) const {
    if (nodo->atributo == "TipoDolor") {
        return paciente.getTipoDolor() == nodo->texto;
    }
    return valorNumerico(paciente, nodo->atributo) >= nodo->umbral;
}

// true si el atributo solo vale 0 o 1
static bool esBinario(const string& atributo) {
    return atributo == "Trauma" || atributo == "SintomasNeurologicos" || atributo == "RequiereCirugia";
}

// texto de la pregunta ya respondida, para la ruta
string ArbolDecision::describir(const NodoDecision* nodo, bool respuesta) const {
    ostringstream salida;
    if (nodo->atributo == "TipoDolor") {
        salida << "TipoDolor " << (respuesta ? "== " : "!= ") << nodo->texto;
    } else if (esBinario(nodo->atributo)) {
        salida << nodo->atributo << " == " << (respuesta ? 1 : 0);
    } else {
        salida << nodo->atributo << (respuesta ? " >= " : " < ") << nodo->umbral;
    }
    return salida.str();
}

// baja desde la raiz respondiendo preguntas hasta llegar a una hoja
int ArbolDecision::clasificar(const Paciente& paciente, string& ruta) const {
    ruta = "";
    const NodoDecision* actual = raiz;

    while (actual != nullptr && !actual->esHoja()) {
        bool respuesta = responder(actual, paciente);

        // agrega este paso a la ruta
        if (!ruta.empty()) ruta += " -> ";
        ruta += describir(actual, respuesta);

        // avanza al hijo que corresponde
        actual = respuesta ? actual->si : actual->no;
    }

    // si el arbol estuviera mal armado (rama sin hoja) no hay servicio
    if (actual == nullptr) return -1;
    return actual->servicio;
}

// texto de un nodo para mostrarlo en los recorridos
string ArbolDecision::textoNodo(const NodoDecision* nodo, const string* nombres) const {
    if (nodo->esHoja()) {
        return "[" + *(nombres + nodo->servicio) + "]";
    }
    return describir(nodo, true) + "?";
}

// imprime la sangria segun la profundidad del nodo
static void sangria(int nivel) {
    for (int i = 0; i < nivel; i++) cout << "    ";
}

// preorden: nodo, despues "si", despues "no"
void ArbolDecision::preorden(const NodoDecision* nodo, const string* nombres, int nivel) const {
    if (nodo == nullptr) return;
    sangria(nivel);
    cout << textoNodo(nodo, nombres) << endl;
    preorden(nodo->si, nombres, nivel + 1);
    preorden(nodo->no, nombres, nivel + 1);
}

// inorden: "si", despues nodo, despues "no"
void ArbolDecision::inorden(const NodoDecision* nodo, const string* nombres, int nivel) const {
    if (nodo == nullptr) return;
    inorden(nodo->si, nombres, nivel + 1);
    sangria(nivel);
    cout << textoNodo(nodo, nombres) << endl;
    inorden(nodo->no, nombres, nivel + 1);
}

// postorden: "si", despues "no", despues nodo
void ArbolDecision::postorden(const NodoDecision* nodo, const string* nombres, int nivel) const {
    if (nodo == nullptr) return;
    postorden(nodo->si, nombres, nivel + 1);
    postorden(nodo->no, nombres, nivel + 1);
    sangria(nivel);
    cout << textoNodo(nodo, nombres) << endl;
}

void ArbolDecision::mostrarPreorden(const string* nombres) const {
    cout << "\n--- Arbol de decision (preorden: nodo, si, no) ---" << endl;
    preorden(raiz, nombres, 0);
}

void ArbolDecision::mostrarInorden(const string* nombres) const {
    cout << "\n--- Arbol de decision (inorden: si, nodo, no) ---" << endl;
    inorden(raiz, nombres, 0);
}

void ArbolDecision::mostrarPostorden(const string* nombres) const {
    cout << "\n--- Arbol de decision (postorden: si, no, nodo) ---" << endl;
    postorden(raiz, nombres, 0);
}
