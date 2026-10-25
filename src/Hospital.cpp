#include "../include/Hospital.h"
#include <fstream>
#include <cctype>
#include <cmath>

// limpia texto para comparar
static string limpiar(string s) {
    string res = "";
    for (size_t i = 0; i < s.length(); i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == 0xC3 && i + 1 < s.length()) {
            unsigned char next = (unsigned char)s[i + 1];
            if (next == 0xAD || next == 0x8D) { res += 'i'; i++; continue; }
            if (next == 0xA1 || next == 0x81) { res += 'a'; i++; continue; }
            if (next == 0xA9 || next == 0x89) { res += 'e'; i++; continue; }
            if (next == 0xB3 || next == 0x93) { res += 'o'; i++; continue; }
            if (next == 0xBA || next == 0x9A) { res += 'u'; i++; continue; }
        }
        if (c >= 'A' && c <= 'Z') res += (char)(c + 32);
        else if (c != ' ' && c != '\t' && c != '\r' && c != '\n') res += (char)c;
    }
    return res;
}

// convierte el campo completo a entero
static bool convertirEntero(string texto, int& valor) {
    try {
        size_t posicion = 0;
        valor = stoi(texto, &posicion);
        return posicion == texto.length();
    } catch (...) {
        return false;
    }
}

// convierte el campo completo a decimal
static bool convertirDecimal(string texto, double& valor) {
    try {
        size_t posicion = 0;
        valor = stod(texto, &posicion);
        return posicion == texto.length() && std::isfinite(valor);
    } catch (...) {
        return false;
    }
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

// valida departamento con punteros
bool Hospital::esServicioValido(string servLeido, string& servOficial) const {
    const string* inicio = nombresServicios;
    const string* fin = nombresServicios + TOTAL_SERVICIOS;

    for (const string* ptr = inicio; ptr < fin; ptr++) {
        if (limpiar(*ptr) == limpiar(servLeido)) {
            servOficial = *ptr;
            return true;
        }
    }
    return false;
}

// metodo para separar la linea con punteros
bool Hospital::parsearLinea(string linea, Paciente& paciente, string& motivo) const {
    motivo = "";
    if (linea.empty()) {
        motivo = "linea vacia";
        return false;
    }

    const char* ptr = linea.c_str();
    const int TOTAL_CAMPOS = 11;
    string tokens[TOTAL_CAMPOS];
    const string nombresCampos[TOTAL_CAMPOS] = {
        "ID", "nombre", "edad", "temperatura", "presion arterial", "dolor",
        "tipo de dolor", "trauma", "sintomas neurologicos", "requiere cirugia",
        "dias de hospitalizacion"
    };

    for (int i = 0; i < TOTAL_CAMPOS; i++) {
        const char* inicioToken = ptr;

        // avanza hasta el separador
        while (*ptr != '\0' && *ptr != ';') {
            ptr++;
        }

        // quita los espacios de los extremos
        const char* finToken = ptr;
        while (inicioToken < finToken &&
               (*inicioToken == ' ' || *inicioToken == '\t' || *inicioToken == '\r')) {
            inicioToken++;
        }
        while (finToken > inicioToken &&
               (*(finToken - 1) == ' ' || *(finToken - 1) == '\t' || *(finToken - 1) == '\r')) {
            finToken--;
        }

        if (inicioToken == finToken) {
            motivo = "campo vacio: " + nombresCampos[i];
            return false;
        }
        tokens[i] = string(inicioToken, finToken - inicioToken);

        // revisa que existan exactamente once campos
        if (i < TOTAL_CAMPOS - 1) {
            if (*ptr != ';') {
                motivo = "tiene " + to_string(i + 1) + " campos, se esperaban " + to_string(TOTAL_CAMPOS);
                return false;
            }
            ptr++;
        } else if (*ptr != '\0') {
            motivo = "tiene mas de " + to_string(TOTAL_CAMPOS) + " campos";
            return false;
        }
    }

    int edad = 0, presion = 0, dolor = 0, dias = 0;
    double temperatura = 0;

    // convierte y valida cada dato numerico
    if (!convertirEntero(tokens[2], edad) || edad < 0 || edad > 130) {
        motivo = "edad invalida (" + tokens[2] + "), rango 0-130";
        return false;
    }
    if (!convertirDecimal(tokens[3], temperatura) || temperatura <= 0) {
        motivo = "temperatura invalida (" + tokens[3] + "), debe ser un numero positivo con punto decimal";
        return false;
    }
    if (!convertirEntero(tokens[4], presion) || presion <= 0) {
        motivo = "presion arterial invalida (" + tokens[4] + "), debe ser un entero positivo";
        return false;
    }
    if (!convertirEntero(tokens[5], dolor) || dolor < 0 || dolor > 10) {
        motivo = "dolor invalido (" + tokens[5] + "), rango 0-10";
        return false;
    }

    if (!convertirEntero(tokens[10], dias) || dias < 0) {
        motivo = "dias de hospitalizacion invalidos (" + tokens[10] + "), deben ser un entero no negativo";
        return false;
    }

    // valida los campos que solo reciben cero o uno
    for (int i = 7; i <= 9; i++) {
        if (tokens[i] != "0" && tokens[i] != "1") {
            motivo = nombresCampos[i] + " debe ser 0 o 1 (" + tokens[i] + ")";
            return false;
        }
    }

    // valida el tipo de dolor
    string tipoDolor = limpiar(tokens[6]);
    if (tipoDolor != "pecho" && tipoDolor != "cabeza" && tipoDolor != "abdomen" &&
        tipoDolor != "extremidad" && tipoDolor != "otro") {
        motivo = "tipo de dolor desconocido (" + tokens[6] + ")";
        return false;
    }

    // crea el paciente sin asignar un servicio
    paciente = Paciente(tokens[0], tokens[1], edad, temperatura, presion, dolor,
                        tipoDolor, tokens[7] == "1", tokens[8] == "1", tokens[9] == "1", dias);
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
    int cargados = 0;
    int rechazados = 0;
    int numeroLinea = 0;

    while (getline(archivo, linea)) {
        numeroLinea++;

        // quita la marca utf8 si viene al inicio
        if (numeroLinea == 1 && linea.compare(0, 3, "\xEF\xBB\xBF") == 0) {
            linea.erase(0, 3);
        }
        if (limpiar(linea).empty()) continue;

        // salta la cabecera si viene en el archivo
        if (limpiar(linea) == "id;nombre;edad;temperatura;presionarterial;dolor;"
                              "tipodolor;trauma;sintomasneurologicos;requierecirugia;diashospitalizacion") {
            continue;
        }

        Paciente p;
        string motivo;
        if (!parsearLinea(linea, p, motivo)) {
            cout << "[ERROR] Linea " << numeroLinea << ": " << motivo << endl;
            rechazados++;
            continue;
        }

        // revisa si ya existe el id
        if (colaEspera.existe(p.getId()) || servicios.existePaciente(p.getId())) {
            cout << "[ERROR] Linea " << numeroLinea << ": ID repetido " << p.getId() << "." << endl;
            rechazados++;
            continue;
        }

        colaEspera.enqueue(p);
        cargados++;
    }

    archivo.close();
    cout << "-> Pacientes cargados en cola: " << cargados << endl;
    if (rechazados > 0) cout << "-> Registros rechazados: " << rechazados << endl;
    return cargados > 0;
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
        // 1. se retira de la queue
        Paciente p;
        if (!colaEspera.dequeue(p)) break;

        // 2 y 3. se evalua con el arbol de decision y se obtiene el servicio
        string ruta;
        int depto = arbol.clasificar(p, ruta);
        if (depto < 0 || depto >= TOTAL_SERVICIOS) {
            cout << "[ERROR] Paciente " << p.getId() << " no se pudo clasificar." << endl;
            continue;
        }
        p.setServicio(nombresServicios[depto]);

        // 5. se informa el resultado
        p.mostrarEnAtencion();
        cout << "\nRuta: " << ruta << endl;
        cout << "\nServicio asignado: " << nombresServicios[depto] << endl;

        // 4. se inserta en el AVL del servicio
        if (!servicios.agregarPacienteAServicio(depto, p)) {
            cout << "[ERROR] Paciente " << p.getId()
                 << " rechazado: ID repetido o servicio no valido." << endl;
            continue;
        }
        cout << "Paciente insertado en el AVL de " << nombresServicios[depto] << "." << endl;

        // guarda en historial
        historial.push(p);

        cout << endl;
        atendidos++;
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
        nodo->pacientes.mostrarInorden();
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
        cout << "[EN COLA DE ESPERA]" << endl;
        p.mostrar();
        return;
    }

    string serv;
    if (servicios.buscarPaciente(criterio, p, serv)) {
        cout << "[ATENDIDO EN: " << serv << "]" << endl;
        p.mostrar();
        return;
    }

    cout << "No se encontro ningun paciente con ese criterio." << endl;
}

int Hospital::getCantidadEnEspera() const {
    return colaEspera.size();
}
