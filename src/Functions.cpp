#include "funciones.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

/* ================= Definiciones de funciones ================= */

// ---------------------- Funciones de datos del usuario ----------------------
bool loadUserData(const string &filename, Usuario &user) {
    ifstream infile(filename);
    if (!infile)
        return false;

    string line;
    vector<string> lines;
    while (getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();

    if (lines.size() < 13) {
        cerr << "Error: El archivo de usuario está corrupto o incompleto." << endl;
        return false;
    }

    try {
        user.nombre = lines[0];
        user.altura = stod(lines[1]);
        user.peso = stod(lines[2]);
        user.edad = stoi(lines[3]);
        user.formula = stoi(lines[4]);
        user.maintenanceCal = stod(lines[5]);
        user.remainingCal = stod(lines[6]);
        user.targetProtein = stod(lines[7]);
        user.targetFat = stod(lines[8]);
        user.targetCarb = stod(lines[9]);
        user.remainingProtein = stod(lines[10]);
        user.remainingFat = stod(lines[11]);
        user.remainingCarb = stod(lines[12]);
    } catch (...) {
        cerr << "Error: Formato incorrecto en el archivo de usuario." << endl;
        return false;
    }
    return true;
}

bool saveUserData(const string &filename, Usuario &user) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Error crítico: No se pudo abrir el archivo para guardar los datos del usuario (" << filename << ")." << endl;
        return false;
    }
    outfile << user.nombre << "\n"
            << user.altura << "\n"
            << user.peso << "\n"
            << user.edad << "\n"
            << user.formula << "\n"
            << user.maintenanceCal << "\n"
            << user.remainingCal << "\n"
            << user.targetProtein << "\n"
            << user.targetFat << "\n"
            << user.targetCarb << "\n"
            << user.remainingProtein << "\n"
            << user.remainingFat << "\n"
            << user.remainingCarb;
    outfile.close();
    return true;
}

void editUserData(Usuario &user, const string &filename) {

    cout << "Ingrese su nuevo nombre (sin tildes ni valores especiales): ";
    getline(cin, user.nombre);
    cout << "Bienvenido, " << user.nombre << endl;

    user.altura = pedirNumero("Ingrese su nueva altura en cm: ");
    cout << "Altura procesada y guardada con exito." << endl;

    user.peso = pedirNumero("Ingrese su nuevo peso en kg: ");
    cout << "Peso procesado y guardada con exito." << endl;

    user.edad = static_cast<int>(pedirNumero("Ingrese su nueva edad en anhos: "));
    cout << "Edad procesada y guardada con exito." << endl;

    user.formula = chooseCalorieFormula();
    cout << "Formula procesada y guardada con exito." << endl;;

    // Calcular lo consumido antes del cambio
    double proteinConsumed = user.targetProtein - user.remainingProtein;
    double fatConsumed = user.targetFat - user.remainingFat;
    double carbConsumed = user.targetCarb - user.remainingCarb;
    double calConsumed = user.maintenanceCal - user.remainingCal;

    // Recalcular las calorías de mantenimiento y las metas de macros
    user.maintenanceCal = calculateCalories(user.peso, user.altura, user.edad, user.formula);
    cout << "\nSu nuevo requerimiento de calorias de mantenimiento es: "
         << user.maintenanceCal << " kcal." << endl;

    calcularMacrosObjetivo(user.maintenanceCal, user.targetProtein, user.targetFat, user.targetCarb);

    // Ajustar los valores restantes a los nuevos targets
    user.remainingCal      = user.maintenanceCal - calConsumed;
    user.remainingProtein  = user.targetProtein - proteinConsumed;
    user.remainingFat      = user.targetFat - fatConsumed;
    user.remainingCarb     = user.targetCarb - carbConsumed;

    saveUserData(filename, user);
}

// ---------------------- Funciones para cálculos ----------------------
int chooseCalorieFormula() {
    int opcion;
    cout << "\nSeleccione la formula para calcular las calorias de mantenimiento:" << endl;
    cout << "1. Formula Hombres: Calorias = (10 * peso en kg) + (6.25 * altura en cm) - (5 * edad) + 5" << endl;
    cout << "2. Formula Mujeres: Calorias = (10 * peso en kg) + (6.25 * altura en cm) - (5 * edad) - 161" << endl;
    do {
        cout << "Ingrese 1 o 2: ";
        cin >> opcion;
        cin.ignore(10000, '\n');
    } while (opcion != 1 && opcion != 2);
    return opcion;
}

double calculateCalories(double peso, double altura, int edad, int formulaOption) {
    double result;
    switch(formulaOption) {
        case 1:
            result = (10 * peso) + (6.25 * altura) - (5 * edad) + 5;
            break;
        case 2:
            result = (10 * peso) + (6.25 * altura) - (5 * edad) - 161;
            break;
        default:
            result = 0; // En caso de opción inválida.
    }
    return result;
}

void resetDailyMarker(const string &userFile, Usuario &user) {
    user.remainingCal     = user.maintenanceCal;
    user.remainingProtein = user.targetProtein;
    user.remainingFat     = user.targetFat;
    user.remainingCarb    = user.targetCarb;
    cout << "\nMarcador diario reiniciado:" << endl;
    mostrarRestante(user.remainingCal, user.remainingProtein, user.remainingFat, user.remainingCarb);

    saveUserData(userFile, user);
}

// ---------------------- Funciones para manejo de objetos nutricionales ----------------------
vector<Objeto> loadObjects(const string &filename) {
    vector<Objeto> objetos;
    ifstream infile(filename);
    if (!infile)
        return objetos; // Retorna vector vacío si no existe el archivo

    string linea;
    while (getline(infile, linea)) {
        if (linea.empty())
            continue;
        stringstream ss(linea);
        Objeto obj;
        string token;

        // Se espera el formato: nombre,proteina,carbohidratos,grasa,fibra,calorias
        getline(ss, obj.nombre, ',');

        getline(ss, token, ',');
        obj.proteina = stod(token);

        getline(ss, token, ',');
        obj.carbohidratos = stod(token);

        getline(ss, token, ',');
        obj.grasa = stod(token);

        getline(ss, token, ',');
        obj.fibra = stod(token);

        getline(ss, token, ',');
        obj.calorias = stod(token);

        objetos.push_back(obj);
    }
    infile.close();
    return objetos;
}

void saveObjects(const string &filename, const vector<Objeto> &objetos) {
    ofstream outfile(filename);
    if (!outfile) {
        cout << "Error al abrir el archivo para guardar objetos." << endl;
        return;
    }

    for (const auto &obj : objetos) {
        outfile << obj.nombre << ","
                << obj.proteina << ","
                << obj.carbohidratos << ","
                << obj.grasa << ","
                << obj.fibra << ","
                << obj.calorias << "\n";
    }
    outfile.close();
}

void addObject(const string &filename) {
    Objeto nuevo;
    cout << "Ingrese el nombre del objeto nutricional (sin tildes ni caracteres especiales): ";
    getline(cin, nuevo.nombre);
    cout << "Nombre procesado y guardado con exito." << endl;

    cout << "Los gramos a continuacion expreselos por cada 100 gramos de producto. Ademas, los valores decimales separelos con un punto no una coma" << endl;

    cout << "Ingrese gramos de proteina: ";
    cin >> nuevo.proteina;
    cout << "Proteina procesada y guardada con exito." << endl;

    cout << "Ingrese gramos de carbohidratos: ";
    cin >> nuevo.carbohidratos;
    cout << "Carbohidratos procesados y guardados con exito." << endl;

    cout << "Ingrese gramos de grasa: ";
    cin >> nuevo.grasa;
    cout << "Grasa procesada y guardada con exito." << endl;

    cout << "Ingrese gramos de fibra: ";
    cin >> nuevo.fibra;
    cout << "Fibra procesada y guardada con exito." << endl;

    // Calcular las calorías del objeto: proteína*4 + carbohidratos*4 + grasa*9 + fibra*2
    nuevo.calorias = (nuevo.proteina * 4) + (nuevo.carbohidratos * 4) + (nuevo.grasa * 9) + (nuevo.fibra * 2);

    // Cargar los objetos existentes, agregar el nuevo y guardar
    vector<Objeto> objetos = loadObjects(filename);
    objetos.push_back(nuevo);
    saveObjects(filename, objetos);
    cout << "\nObjeto creado y guardado con exito." << endl;
}

void deleteObject(const string &filename) {
    vector<Objeto> objetos = loadObjects(filename);
    if (objetos.empty()) {
        cout << "No hay objetos guardados para eliminar." << endl;
        return;
    }

    // Mostrar los objetos con su índice
    cout << "Objetos actuales:" << endl;
    imprimirObjetos(objetos);

    int indice;
    cout << "Ingrese el numero del objeto que desea eliminar: ";
    cin >> indice;
    if (indice < 0 || indice >= (int)objetos.size()) {
        cout << "Indice invalido. No se realizo ninguna eliminacion." << endl;
        return;
    }

    // Eliminar el objeto seleccionado y guardar los cambios
    objetos.erase(objetos.begin() + indice);
    saveObjects(filename, objetos);
    cout << "Objeto eliminado, procesado y guardado con exito." << endl;
}

void consumeObjects(const string &objFile, const string &userFile, Usuario &user) {
    vector<Objeto> objetos = loadObjects(objFile);
    if (objetos.empty()) {
        cout << "No hay objetos nutricionales disponibles." << endl;
        return;
    }

    double totalConsumedCal = 0;
    double totalConsumedProtein = 0;
    double totalConsumedFat = 0;
    double totalConsumedCarb = 0;
    double totalConsumedFibra = 0;

    cout << "\nListado de objetos nutricionales:" << endl;
    imprimirObjetos(objetos);

    cout << "Ingrese el numero del objeto que desea consumir o 'f' para finalizar: ";
    string input;
    // Se usa getline para permitir capturar la 'f'
    while (getline(cin, input)) {
        if (input == "f" || input == "F")
            break;
        if(input.empty())
            continue;
        int index = -1;
        try {
            index = stoi(input);
        } catch (...) {
            cout << "Entrada invalida. Intente de nuevo: ";
            continue;
        }
        if(index < 0 || index >= (int)objetos.size()){
            cout << "Indice fuera de rango. Intente de nuevo: ";
            continue;
        }

        // Preguntar cuántos gramos se consumieron
        double gramos = 0;
        cout << "Cuantos gramos de " << objetos[index].nombre << " consumiste? ";
        cin >> gramos;
        cin.ignore();

        // Calcular proporción
        double factor = gramos / 100.0;

        totalConsumedCal     += objetos[index].calorias * factor;
        totalConsumedProtein += objetos[index].proteina * factor;
        totalConsumedFat     += objetos[index].grasa * factor;
        totalConsumedCarb    += objetos[index].carbohidratos * factor;
        totalConsumedFibra   += objetos[index].fibra * factor;

        cout << "Has consumido: " << gramos << "g de " << objetos[index].nombre << endl;
        cout << "Ingrese otro numero o 'f' para finalizar: ";
    }

    // Actualizar los marcadores diarios
    user.remainingCal      -= totalConsumedCal;
    user.remainingProtein  -= totalConsumedProtein;
    user.remainingFat      -= totalConsumedFat;
    user.remainingCarb     -= totalConsumedCarb;

    cout << "\nTotal consumido:" << endl;
    cout << "Calorias: " << totalConsumedCal << " Kcal" << endl;
    cout << "Proteina: " << totalConsumedProtein << " g" << endl;
    cout << "Grasa: " << totalConsumedFat << " g" << endl;
    cout << "Carbohidratos: " << totalConsumedCarb << " g" << endl;
    cout << "Fibra: " << totalConsumedFibra << " g" << endl;

    cout << "\nMarcador diario actualizado:" << endl;
    mostrarRestante(user.remainingCal, user.remainingProtein, user.remainingFat, user.remainingCarb);

    // Guardar los nuevos valores en el archivo de usuario
    saveUserData(userFile, user);
}

// Función auxiliar para imprimir el valor con color
void imprimirValor(const string &etiqueta, double &valor, const string &unidad) {
    // Si el valor es negativo, se imprime en rojo; si es mayor o igual a cero, en verde.
    if (valor < 0)
        cout << etiqueta << ": " << "\033[31m" << valor << " " << unidad << "\033[0m" << endl;
    else
        cout << etiqueta << ": " << "\033[32m" << valor << " " << unidad << "\033[0m" << endl;
}

void mostrarRestante(double &remainingCal, double &remainingProtein, double &remainingFat, double &remainingCarb) {
    if (remainingCal < 0 || remainingProtein < 0 || remainingFat < 0 || remainingCarb < 0) {
        cout << "\n\033[31mAtencion: Has excedido tus limites diarios!\033[0m" << endl;
    } else {
        cout << "\n\033[32mMarcador diario:\033[0m" << endl;
    }

    // Se imprime cada información con el color correspondiente
    imprimirValor("Calorias restantes", remainingCal, "Kcal");
    imprimirValor("Proteina restante", remainingProtein, "g");
    imprimirValor("Grasa restante", remainingFat, "g");
    imprimirValor("Carbohidratos restantes", remainingCarb, "g");
}

void calcularMacrosObjetivo(double maintenanceCal, double &targetProtein, double &targetFat, double &targetCarb) {
    targetProtein = (maintenanceCal * 0.25) / 4;
    targetFat     = (maintenanceCal * 0.20) / 9;
    targetCarb    = (maintenanceCal * 0.55) / 4;
}

double pedirNumero(const string& mensaje) {
    double valor;
    while (true) {
        cout << mensaje;
        cin >> valor;
        if (cin.fail() || valor < 0) {
            cout << "Entrada inválida. Intente de nuevo." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        } else {
            cin.ignore(10000, '\n');
            return valor;
        }
    }
}

int pedirIndiceValido(int max, const string& mensaje) {
    int indice;
    while (true) {
        cout << mensaje;
        cin >> indice;
        if (cin.fail() || indice < 0 || indice >= max) {
            cout << "Índice inválido. Intente de nuevo." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        } else {
            cin.ignore(10000, '\n');
            return indice;
        }
    }
}

void imprimirObjetos(const vector<Objeto> &objetos) {
    for (size_t i = 0; i < objetos.size(); i++) {
        cout << i << ". " << objetos[i].nombre
             << " (Proteina: " << objetos[i].proteina
             << " g, Carbohidratos: " << objetos[i].carbohidratos
             << " g, Grasa: " << objetos[i].grasa
             << " g, Fibra: " << objetos[i].fibra
             << " g, Calorias: " << objetos[i].calorias << " Kcal)" << endl;
    }
}
