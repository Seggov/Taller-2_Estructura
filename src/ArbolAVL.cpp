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
