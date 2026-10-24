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

    // inserta en el subarbol y devuelve su raiz
    NodoAVL* insertarNodo(NodoAVL* nodo, const Paciente& paciente, bool& insertado);

    // elimina del subarbol y busca el sucesor
    NodoAVL* eliminarNodo(NodoAVL* nodo, const string& id, bool& eliminado);
    NodoAVL* menorNodo(NodoAVL* nodo) const;

    // busca por id o recorre por nombre
    const NodoAVL* buscarNodo(string id) const;
    bool buscarNombreNodo(NodoAVL* nodo, const string& nombre, Paciente& encontrado) const;

    // altura y balance de un nodo
    int alturaNodo(const NodoAVL* nodo) const;
    void actualizarAltura(NodoAVL* nodo);
    int obtenerBalance(const NodoAVL* nodo) const;
    NodoAVL* balancear(NodoAVL* nodo);

    // rotaciones simples
    NodoAVL* rotacionDerecha(NodoAVL* nodo);
    NodoAVL* rotacionIzquierda(NodoAVL* nodo);

    // rotaciones dobles
    NodoAVL* rotacionIzquierdaDerecha(NodoAVL* nodo);
    NodoAVL* rotacionDerechaIzquierda(NodoAVL* nodo);

    // recorridos del arbol
    void inorden(NodoAVL* nodo) const;
    void preorden(NodoAVL* nodo) const;
    void postorden(NodoAVL* nodo) const;
    void mostrarBalanceNodo(NodoAVL* nodo) const;

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

    // inserta por id y rechaza duplicados
    bool insertar(const Paciente& paciente);

    // elimina por id y devuelve si lo encontro
    bool eliminar(string id);

    // busca por id y copia el paciente encontrado
    bool buscarPorId(string id, Paciente& encontrado) const;

    // busca un nombre exacto, primero el menor id
    bool buscarPorNombre(string nombre, Paciente& encontrado) const;

    // muestra los pacientes en cada recorrido
    void mostrarInorden() const;
    void mostrarPreorden() const;
    void mostrarPostorden() const;

    // consulta por id o muestra el balance de todos
    bool consultarBalance(string id, int& balance) const;
    void mostrarBalance() const;
};

#endif
