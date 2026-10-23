#include "../include/Hospital.h"
#include <fstream>
#include <cctype>
#include <stdexcept>   // invalid_argument y out_of_range (excepciones de stoi/stof)

// limpia texto para comparar / reemplazo del metodo anterior 
static string aMinuscula(string s) {
    for (char& c : s) c = tolower((unsigned char)c);
    return s;
}

// constructor
Hospital::Hospital() {
    inicializarServicios();
}

// destructor
Hospital::~Hospital() {}

void Hospital::inicializarServicios() {
    string catalogo[TOTAL_SERVICIOS] = {
        "Urgencias", "Medicina General", "Cardiologia", "Neurologia",
        "Traumatologia", "Cirugia", "Pediatria", "Hospitalizacion"
    };

    const string* pOrigen = catalogo;
    string* pDestino = nombresServicios;

    for (int i = 0; i < TOTAL_SERVICIOS; i++) {
        // copia con aritmetica de punteros
        *(pDestino + i) = *(pOrigen + i);
        servicios.agregarServicio(*(pDestino + i));
    }
}

// validacion de departamento con punteros
bool Hospital::esServicioValido(string servLeido, string& servOficial) const {
    const string* inicio = nombresServicios;
    const string* fin = nombresServicios + TOTAL_SERVICIOS;

    for (const string* ptr = inicio; ptr < fin; ptr++) {
        if (aMinuscula(*ptr) == aMinuscula(servLeido)) {
            servOficial = *ptr;
            return true;
        }
    }
    return false;
}

// catalogo de tipos de dolor aceptados
static const string TIPOS_DOLOR[TOTAL_TIPOS_DOLOR] = {
    "pecho", "cabeza", "abdomen", "extremidad", "otro"
};

// valida el tipo de dolor con aritmetica de punteros (igual que esServicioValido)
bool Hospital::esTipoDolorValido(const string& leido, string& oficial) const {
    const string* inicio = TIPOS_DOLOR;
    const string* fin = TIPOS_DOLOR + TOTAL_TIPOS_DOLOR;

    for (const string* ptr = inicio; ptr < fin; ptr++) {
        // limpiar() pasa a minuscula y quita tildes: "PECHO" == "pecho"
        if (aMinuscula(*ptr) == aMinuscula(leido)) {
            oficial = *ptr;
            return true;
        }
    }
    return false;
}

// texto -> int, solo si TODO el texto es numero y esta en [minimo, maximo]
bool Hospital::convertirEntero(const string& texto, int minimo, int maximo, int& resultado) const {
    if (texto.empty()) return false;
    try {
        size_t usados = 0;                          // stoi anota aqui cuantos chars leyo
        int valor = stoi(texto, &usados);
        if (usados != texto.size()) return false;   // "12abc" -> usados = 2 de 5
        if (valor < minimo || valor > maximo) return false;
        resultado = valor;
        return true;
    } catch (const invalid_argument&) {             // "abc": no empieza con numero
        return false;
    } catch (const out_of_range&) {                 // "99999999999": no cabe en int
        return false;
    }
}

// texto -> float, misma idea que convertirEntero
bool Hospital::convertirDecimal(const string& texto, float minimo, float maximo, float& resultado) const {
    if (texto.empty()) return false;
    try {
        size_t usados = 0;
        float valor = stof(texto, &usados);
        if (usados != texto.size()) return false;   // "38,7" -> usados = 2 de 4
        if (valor < minimo || valor > maximo) return false;
        resultado = valor;
        return true;
    } catch (const invalid_argument&) {
        return false;
    } catch (const out_of_range&) {
        return false;
    }
}

// metodo para separar la linea con punteros
// ahora arma directamente el paciente (paciente se recibe por referencia)
bool Hospital::parsearLinea(string linea, Paciente& paciente, string& motivo) const {

    const char* ptr = linea.c_str();
    string tokens[NUM_CAMPOS];
    int tokenIdx = 0;

    // recorre TODA la linea (antes paraba en el campo 11 e ignoraba lo que sobraba)
    while (*ptr != '\0') {
        // salta espacios
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\r') {
            ptr++;
        }
        if (*ptr == '\0') break;

        // aparecio un campo numero 12
        if (tokenIdx == NUM_CAMPOS) {
            motivo = "tiene mas de " + to_string(NUM_CAMPOS) + " campos";
            return false;
        }

        const char* inicioToken = ptr;

        // avanza hasta el separador
        while (*ptr != '\0' && *ptr != ';') {
            ptr++;
        }

        // retrocede espacios
        const char* finToken = ptr - 1;
        while (finToken >= inicioToken && (*finToken == ' ' || *finToken == '\t' || *finToken == '\r')) {
            finToken--;
        }

        if (finToken >= inicioToken) {
            tokens[tokenIdx] = string(inicioToken, finToken - inicioToken + 1);
        }
        tokenIdx++;

        if (*ptr == ';') {
            ptr++;
        }
    }

    if (tokenIdx < NUM_CAMPOS) {
        motivo = "tiene " + to_string(tokenIdx) + " campos, se esperaban " + to_string(NUM_CAMPOS);
        return false;
    }
    if (tokens[0].empty()) { motivo = "ID vacio"; return false; }
    if (tokens[1].empty()) { motivo = "nombre vacio"; return false; }

    int edad, presion, dolor, trauma, neuro, cirugia, dias;
    float temperatura;
    string tipoDolor;

    // cada campo con su rango; si falla se dice cual y por que
    if (!convertirEntero(tokens[2], 0, 120, edad)) {
        motivo = "edad invalida (" + tokens[2] + "), rango 0-120"; return false;
    }
    if (!convertirDecimal(tokens[3], 34.0f, 43.0f, temperatura)) {
        motivo = "temperatura invalida (" + tokens[3] + "), rango 34.0-43.0 con punto"; return false;
    }
    if (!convertirEntero(tokens[4], 60, 250, presion)) {
        motivo = "presion invalida (" + tokens[4] + "), rango 60-250"; return false;
    }
    if (!convertirEntero(tokens[5], 0, 10, dolor)) {
        motivo = "dolor invalido (" + tokens[5] + "), rango 0-10"; return false;
    }
    if (!esTipoDolorValido(tokens[6], tipoDolor)) {
        motivo = "tipo de dolor desconocido (" + tokens[6] + ")"; return false;
    }
    if (!convertirEntero(tokens[7], 0, 1, trauma)) {
        motivo = "trauma debe ser 0 o 1 (" + tokens[7] + ")"; return false;
    }
    if (!convertirEntero(tokens[8], 0, 1, neuro)) {
        motivo = "sintomas neurologicos debe ser 0 o 1 (" + tokens[8] + ")"; return false;
    }
    if (!convertirEntero(tokens[9], 0, 1, cirugia)) {
        motivo = "requiere cirugia debe ser 0 o 1 (" + tokens[9] + ")"; return false;
    }
    if (!convertirEntero(tokens[10], 0, 365, dias)) {
        motivo = "dias de hospitalizacion invalidos (" + tokens[10] + "), rango 0-365"; return false;
    }

    paciente = Paciente(tokens[0], tokens[1], edad, temperatura, presion,
                        dolor, tipoDolor, trauma, neuro, cirugia, dias);
    return true;
}

// metodo para la lectura de pacientes
bool Hospital::cargarPacientes(string ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        archivo.open("../" + ruta);
        if (!archivo.is_open()) {
            cout << "[ERROR] No se pudo abrir: " << ruta << endl;
            return false;
        }
    }

    string linea;
    int numLinea = 0;
    int cargados = 0;
    int rechazados = 0;

    while (getline(archivo, linea)) {
        numLinea++;

        // linea vacia o solo espacios: se ignora sin reclamar
        if (aMinuscula(linea).empty()) continue;

        // el servicio ya no viene en el archivo: lo decide el arbol al atender
        Paciente p;
        string motivo;
        if (!parsearLinea(linea, p, motivo)) {
            cout << "[RECHAZADA] linea " << numLinea << ": " << motivo << endl;
            rechazados++;
            continue;
        }

        // si ya existe
        if (colaEspera.existe(p.getId()) || servicios.existePaciente(p.getId())) {
            cout << "[RECHAZADA] linea " << numLinea << ": ID duplicado (" << p.getId() << ")" << endl;
            rechazados++;
            continue;
        }

        colaEspera.enqueue(p);
        cargados++;
    }

    archivo.close();
    cout << "-> Pacientes cargados en cola: " << cargados;
    if (rechazados > 0) cout << " | lineas rechazadas: " << rechazados;
    cout << endl;
    return cargados > 0;
    // si no null
}

// out para la cola
void Hospital::mostrarCola() const {
    cout << "\n=== PACIENTES EN ESPERA ===" << endl;
    colaEspera.mostrar();
}

// atencion de pacientes y derivacion
void Hospital::atenderPacientes(int cantidad) {
    if (colaEspera.isEmpty()) {
        cout << "\n(No hay pacientes en cola de espera)" << endl;
        return;
    }

    cout << "\n=== ATENDIENDO PACIENTES ===" << endl;
    int atendidos = 0;

    while (atendidos < cantidad && !colaEspera.isEmpty()) {
        Paciente p;
        if (colaEspera.dequeue(p)) {
            // deriva al servicio
            servicios.agregarPacienteAServicio(p.getServicio(), p);

            // guarda en historial
            historial.push(p);

            // imprime la ficha
            p.mostrarEnAtencion();
            cout << endl;
            atendidos++;
        }
    }
}

// out de departamentos
void Hospital::mostrarDepartamentos() const {
    cout << "\n=== DEPARTAMENTOS/SERVICIOS ===" << endl;
    servicios.mostrar();
}

// out de pacientes en el servicio seleccionado
void Hospital::mostrarEstadoDepartamento(int indice) const {
    NodoServicio* nodo = servicios.obtenerPorIndice(indice);
    if (nodo != nullptr) {
        string nomMayus = nodo->nombre;
        for (char& c : nomMayus) c = toupper((unsigned char)c);

        cout << "\n=== ESTADO " << nomMayus << " ===" << endl;
        cout << "Pacientes en el departamento de " << nodo->nombre << ": " << nodo->pacientes.size() << endl;
        nodo->pacientes.mostrar();
    } else {
        cout << "\n[ERROR] Departamento no valido." << endl;
    }
}

// out para el historial
void Hospital::mostrarHistorial() const {
    cout << "\n=== HISTORIAL DE ULTIMAS ATENCIONES DEL HOSPITAL ===" << endl;
    historial.mostrar();
}

// consulta de paciente
void Hospital::buscarPaciente(string criterio) const {
    if (criterio.empty()) return;

    cout << "\n--- Resultado de Busqueda: '" << criterio << "' ---" << endl;

    Paciente p;
    if (colaEspera.buscar(criterio, p)) {
        cout << "[EN COLA DE ESPERA] ID: " << p.getId() << " | Nombre: " << p.getNombre()
             << " | Edad: " << p.getEdad() << " | Servicio: " << p.getServicio() << endl;
        return;
    }

    string serv;
    if (servicios.buscarPaciente(criterio, p, serv)) {
        cout << "[ATENDIDO EN: " << serv << "] ID: " << p.getId()
             << " | Nombre: " << p.getNombre() << " | Edad: " << p.getEdad() << endl;
        return;
    }

    cout << "No se encontro ningun paciente con ese criterio." << endl;
}

int Hospital::getCantidadEnEspera() const {
    return colaEspera.size();
}
