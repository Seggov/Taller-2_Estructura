#include "../include/ArbolAVL.h"

// constructor del arbol
ArbolAVL::ArbolAVL() : raiz(nullptr), tamano(0) {}

// destructor que borra todos los nodos
ArbolAVL::~ArbolAVL() {
    liberar(raiz);
}

// libera en postorden
void ArbolAVL::liberar(NodoAVL* nodo) {
    if (nodo == nullptr) return;

    // primero libera los hijos
    liberar(nodo->izquierdo);
    liberar(nodo->derecho);

    // despues borra el nodo
    delete nodo;
}

// revisa si esta vacio
bool ArbolAVL::isEmpty() const {
    return raiz == nullptr;
}

// cantidad de pacientes
int ArbolAVL::size() const {
    return tamano;
}

// devuelve la altura del arbol
int ArbolAVL::obtenerAltura() const {
    if (raiz == nullptr) return 0;
    return raiz->altura;
}

// inserta un paciente y actualiza la cantidad
bool ArbolAVL::insertar(const Paciente& paciente) {
    bool insertado = false;
    raiz = insertarNodo(raiz, paciente, insertado);

    if (insertado) tamano++;
    return insertado;
}

// inserta siguiendo el orden de un abb
NodoAVL* ArbolAVL::insertarNodo(NodoAVL* nodo, const Paciente& paciente, bool& insertado) {
    if (nodo == nullptr) {
        nodo = new NodoAVL(paciente);
        insertado = true;
        return nodo;
    }

    // compara los ids como texto
    if (paciente.getId() < nodo->dato.getId()) {
        nodo->izquierdo = insertarNodo(nodo->izquierdo, paciente, insertado);
    } else if (paciente.getId() > nodo->dato.getId()) {
        nodo->derecho = insertarNodo(nodo->derecho, paciente, insertado);
    } else {
        // el id ya existe, no modifica el paciente
        return nodo;
    }

    // actualiza la altura al volver de la insercion
    int alturaIzquierda = 0;
    int alturaDerecha = 0;
    if (nodo->izquierdo != nullptr) alturaIzquierda = nodo->izquierdo->altura;
    if (nodo->derecho != nullptr) alturaDerecha = nodo->derecho->altura;

    if (alturaIzquierda > alturaDerecha) nodo->altura = alturaIzquierda + 1;
    else nodo->altura = alturaDerecha + 1;

    // el balanceo se agrega en el siguiente avance
    return nodo;
}

// busca desde la raiz comparando el id
bool ArbolAVL::buscarPorId(string id, Paciente& encontrado) const {
    const NodoAVL* actual = raiz;

    while (actual != nullptr) {
        if (id == actual->dato.getId()) {
            encontrado = actual->dato;
            return true;
        }

        if (id < actual->dato.getId()) actual = actual->izquierdo;
        else actual = actual->derecho;
    }

    return false;
}
