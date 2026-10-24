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
    return alturaNodo(raiz);
}

// altura de un nodo o cero si no existe
int ArbolAVL::alturaNodo(NodoAVL* nodo) const {
    if (nodo == nullptr) return 0;
    return nodo->altura;
}

// actualiza la altura usando el hijo mas alto
void ArbolAVL::actualizarAltura(NodoAVL* nodo) {
    if (nodo == nullptr) return;

    int alturaIzquierda = alturaNodo(nodo->izquierdo);
    int alturaDerecha = alturaNodo(nodo->derecho);

    if (alturaIzquierda > alturaDerecha) nodo->altura = alturaIzquierda + 1;
    else nodo->altura = alturaDerecha + 1;
}

// diferencia entre las alturas de los hijos
int ArbolAVL::obtenerBalance(NodoAVL* nodo) const {
    if (nodo == nullptr) return 0;
    return alturaNodo(nodo->izquierdo) - alturaNodo(nodo->derecho);
}

// sube el hijo izquierdo
NodoAVL* ArbolAVL::rotacionDerecha(NodoAVL* nodo) {
    NodoAVL* nuevaRaiz = nodo->izquierdo;
    NodoAVL* subarbol = nuevaRaiz->derecho;

    nuevaRaiz->derecho = nodo;
    nodo->izquierdo = subarbol;

    // primero actualiza el nodo que baja
    actualizarAltura(nodo);
    actualizarAltura(nuevaRaiz);
    return nuevaRaiz;
}

// sube el hijo derecho
NodoAVL* ArbolAVL::rotacionIzquierda(NodoAVL* nodo) {
    NodoAVL* nuevaRaiz = nodo->derecho;
    NodoAVL* subarbol = nuevaRaiz->izquierdo;

    nuevaRaiz->izquierdo = nodo;
    nodo->derecho = subarbol;

    // primero actualiza el nodo que baja
    actualizarAltura(nodo);
    actualizarAltura(nuevaRaiz);
    return nuevaRaiz;
}

// izquierda en el hijo y derecha en el nodo
NodoAVL* ArbolAVL::rotacionIzquierdaDerecha(NodoAVL* nodo) {
    nodo->izquierdo = rotacionIzquierda(nodo->izquierdo);
    return rotacionDerecha(nodo);
}

// derecha en el hijo e izquierda en el nodo
NodoAVL* ArbolAVL::rotacionDerechaIzquierda(NodoAVL* nodo) {
    nodo->derecho = rotacionDerecha(nodo->derecho);
    return rotacionIzquierda(nodo);
}

// inserta un paciente y actualiza la cantidad
bool ArbolAVL::insertar(const Paciente& paciente) {
    bool insertado = false;
    raiz = insertarNodo(raiz, paciente, insertado);

    if (insertado) tamano++;
    return insertado;
}

// inserta por id y mantiene el balance
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
    actualizarAltura(nodo);
    int balance = obtenerBalance(nodo);

    // revisa si pesa mas hacia la izquierda
    if (balance > 1) {
        if (obtenerBalance(nodo->izquierdo) < 0) return rotacionIzquierdaDerecha(nodo);
        return rotacionDerecha(nodo);
    }

    // revisa si pesa mas hacia la derecha
    if (balance < -1) {
        if (obtenerBalance(nodo->derecho) > 0) return rotacionDerechaIzquierda(nodo);
        return rotacionIzquierda(nodo);
    }

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
