#ifndef ARBOL_AVL_H
#define ARBOL_AVL_H

#include "Paciente.h"

// struct nodo para el arbol
struct NodoAVL {
    Paciente dato;
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    // crea el nodo sin hijos
    NodoAVL(const Paciente& paciente)
        : dato(paciente),
          izquierdo(nullptr),
          derecho(nullptr),
          altura(1) {}
};

// arbol avl de pacientes
class ArbolAVL {
private:
    // raiz y cantidad de pacientes
    NodoAVL* raiz;
    int tamano;

    // libera los nodos del arbol
    void liberar(NodoAVL* nodo);

public:
    // constructor y destructor
    ArbolAVL();
    ~ArbolAVL();

    // evita copias del arbol
    ArbolAVL(const ArbolAVL&) = delete;
    ArbolAVL& operator=(const ArbolAVL&) = delete;

    // funciones basicas
    bool isEmpty() const;
    int size() const;
    int obtenerAltura() const;
};

#endif
