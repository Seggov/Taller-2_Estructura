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
int ArbolAVL::alturaNodo(const NodoAVL* nodo) const {
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
int ArbolAVL::obtenerBalance(const NodoAVL* nodo) const {
    if (nodo == nullptr) return 0;
    return alturaNodo(nodo->izquierdo) - alturaNodo(nodo->derecho);
}

// actualiza la altura y corrige el balance
NodoAVL* ArbolAVL::balancear(NodoAVL* nodo) {
    if (nodo == nullptr) return nullptr;

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

    return balancear(nodo);
}

// elimina un paciente y actualiza la cantidad
bool ArbolAVL::eliminar(string id) {
    bool eliminado = false;
    raiz = eliminarNodo(raiz, id, eliminado);

    if (eliminado) tamano--;
    return eliminado;
}

// elimina por id y mantiene el balance
NodoAVL* ArbolAVL::eliminarNodo(NodoAVL* nodo, const string& id, bool& eliminado) {
    if (nodo == nullptr) return nullptr;

    if (id < nodo->dato.getId()) {
        nodo->izquierdo = eliminarNodo(nodo->izquierdo, id, eliminado);
    } else if (id > nodo->dato.getId()) {
        nodo->derecho = eliminarNodo(nodo->derecho, id, eliminado);
    } else {
        eliminado = true;

        // si tiene cero o un hijo, lo reemplaza por ese hijo
        if (nodo->izquierdo == nullptr || nodo->derecho == nullptr) {
            NodoAVL* hijo = nodo->izquierdo;
            if (hijo == nullptr) hijo = nodo->derecho;
            delete nodo;
            return hijo;
        }

        // con dos hijos, copia al menor del lado derecho
        NodoAVL* sucesor = menorNodo(nodo->derecho);
        nodo->dato = sucesor->dato;
        string idSucesor = sucesor->dato.getId();
        nodo->derecho = eliminarNodo(nodo->derecho, idSucesor, eliminado);
    }

    return balancear(nodo);
}

// busca el menor del subarbol
NodoAVL* ArbolAVL::menorNodo(NodoAVL* nodo) const {
    while (nodo != nullptr && nodo->izquierdo != nullptr) nodo = nodo->izquierdo;
    return nodo;
}

// busca desde la raiz comparando el id
const NodoAVL* ArbolAVL::buscarNodo(string id) const {
    const NodoAVL* actual = raiz;

    while (actual != nullptr) {
        if (id == actual->dato.getId()) return actual;

        if (id < actual->dato.getId()) actual = actual->izquierdo;
        else actual = actual->derecho;
    }

    return nullptr;
}

// copia el paciente si encuentra su id
bool ArbolAVL::buscarPorId(string id, Paciente& encontrado) const {
    const NodoAVL* nodo = buscarNodo(id);
    if (nodo == nullptr) return false;

    encontrado = nodo->dato;
    return true;
}

// busca el nombre recorriendo desde el menor id
bool ArbolAVL::buscarPorNombre(string nombre, Paciente& encontrado) const {
    return buscarNombreNodo(raiz, nombre, encontrado);
}

// recorre porque el arbol esta ordenado por id
bool ArbolAVL::buscarNombreNodo(NodoAVL* nodo, const string& nombre, Paciente& encontrado) const {
    if (nodo == nullptr) return false;

    if (buscarNombreNodo(nodo->izquierdo, nombre, encontrado)) return true;

    if (nodo->dato.getNombre() == nombre) {
        encontrado = nodo->dato;
        return true;
    }

    return buscarNombreNodo(nodo->derecho, nombre, encontrado);
}

// muestra izquierda, nodo y derecha
void ArbolAVL::mostrarInorden() const {
    if (isEmpty()) {
        cout << "(No hay pacientes en el arbol)" << endl;
        return;
    }
    inorden(raiz);
}

void ArbolAVL::inorden(NodoAVL* nodo) const {
    if (nodo == nullptr) return;

    inorden(nodo->izquierdo);
    cout << "[" << nodo->dato.getId() << "] ";
    nodo->dato.mostrarEnDepartamento();
    inorden(nodo->derecho);
}

// muestra nodo, izquierda y derecha
void ArbolAVL::mostrarPreorden() const {
    if (isEmpty()) {
        cout << "(No hay pacientes en el arbol)" << endl;
        return;
    }
    preorden(raiz);
}

void ArbolAVL::preorden(NodoAVL* nodo) const {
    if (nodo == nullptr) return;

    cout << "[" << nodo->dato.getId() << "] ";
    nodo->dato.mostrarEnDepartamento();
    preorden(nodo->izquierdo);
    preorden(nodo->derecho);
}

// muestra izquierda, derecha y nodo
void ArbolAVL::mostrarPostorden() const {
    if (isEmpty()) {
        cout << "(No hay pacientes en el arbol)" << endl;
        return;
    }
    postorden(raiz);
}

void ArbolAVL::postorden(NodoAVL* nodo) const {
    if (nodo == nullptr) return;

    postorden(nodo->izquierdo);
    postorden(nodo->derecho);
    cout << "[" << nodo->dato.getId() << "] ";
    nodo->dato.mostrarEnDepartamento();
}

// consulta el balance de un paciente
bool ArbolAVL::consultarBalance(string id, int& balance) const {
    const NodoAVL* nodo = buscarNodo(id);
    if (nodo == nullptr) return false;

    balance = obtenerBalance(nodo);
    return true;
}

// muestra altura y balance de cada paciente
void ArbolAVL::mostrarBalance() const {
    if (isEmpty()) {
        cout << "(No hay pacientes en el arbol)" << endl;
        return;
    }
    mostrarBalanceNodo(raiz);
}

void ArbolAVL::mostrarBalanceNodo(NodoAVL* nodo) const {
    if (nodo == nullptr) return;

    mostrarBalanceNodo(nodo->izquierdo);
    cout << "ID: " << nodo->dato.getId()
         << " | Altura: " << nodo->altura
         << " | Balance: " << obtenerBalance(nodo) << endl;
    mostrarBalanceNodo(nodo->derecho);
}
