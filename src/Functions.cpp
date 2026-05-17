#include "funciones.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <thread>
#include <chrono>

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

    if (lines.size() < 15) {
        cerr << "Error: El archivo de usuario esta corrupto o incompleto. \n";
        return false;
    }

    try {
        user.nombre = lines[0];
        user.altura = stod(lines[1]);
        user.peso = stod(lines[2]);
        user.edad = stoi(lines[3]);
        user.formula = stoi(lines[4]);
        user.proteinaPorcentaje = stoi (lines[5]); // Me duele tener que guardar esto pero sino no se puede mostrar en printProjectInfo()
        user.carbsPorcentaje = stoi (lines[6]);
        user.grasaPorcentaje = stoi (lines[7]);
        user.maintenanceCal = stod(lines[8]);
        user.remainingCal = stod(lines[9]);
        user.targetProtein = stod(lines[10]);
        user.targetFat = stod(lines[11]);
        user.targetCarb = stod(lines[12]);
        user.remainingProtein = stod(lines[13]);
        user.remainingFat = stod(lines[14]);
        user.remainingCarb = stod(lines[15]);
    } catch (...) {
        cerr << "Error: Formato incorrecto en el archivo de usuario. \n\n";
        return false;
    }
    return true;
}

bool saveUserData(const string &filename, Usuario &user) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Error critico: No se pudo abrir el archivo para guardar los datos del usuario (" << filename << "). \n";
        return false;
    }
    outfile << user.nombre << "\n"
            << user.altura << "\n"
            << user.peso << "\n"
            << user.edad << "\n"
            << user.formula << "\n"
            << user.proteinaPorcentaje << "\n"
            << user.carbsPorcentaje << "\n"
            << user.grasaPorcentaje << "\n"
            << user.maintenanceCal << "\n"
            << user.remainingCal << "\n"
            << user.targetProtein << "\n"
            << user.targetFat << "\n"
            << user.targetCarb << "\n"
            << user.remainingProtein << "\n"  //Modular esta función de guardar lo que queda que se usa todo el rato
            << user.remainingFat << "\n"
            << user.remainingCarb;
    outfile.close();
    return true;
}

void editUserData(Usuario &user, const string &filename) {

    cout << "Ingrese su nuevo nombre (sin tildes ni valores especiales): ";
    getline(cin, user.nombre);
    cout << "\nBienvenido, " << user.nombre << "\n\n";

    user.altura = getNumber("Ingrese su nueva altura en cm: ");
    cout << "Altura procesada y guardada con exito. \n\n";

    user.peso = getNumber("Ingrese su nuevo peso en kg: ");
    cout << "Peso procesado y guardada con exito. \n\n";

    user.edad = static_cast<int>(getNumber("Ingrese su nueva edad en anhos: "));
    cout << "Edad procesada y guardada con exito. \n";

    user.formula = chooseCalorieFormula();
    cout << "\nFormula procesada y guardada con exito.\n";

    // Calcular lo consumido antes del cambio
    double proteinConsumed = user.targetProtein - user.remainingProtein;
    double fatConsumed = user.targetFat - user.remainingFat;
    double carbConsumed = user.targetCarb - user.remainingCarb;
    double calConsumed = user.maintenanceCal - user.remainingCal;

    // Recalcular las calorías de mantenimiento y las metas de macros
    user.maintenanceCal = calculateCalories(user.peso, user.altura, user.edad, user.formula);

    cout << "Ingrese a continuacion el porcentaje de macros que desea para su dieta (sin poner el % y que sume 100 por favor): \n\n";
    user.proteinaPorcentaje = static_cast<int>(getNumber("Porcentaje de proteinas:  "));
    user.carbsPorcentaje = static_cast<int>(getNumber("Porcentaje de carbohidratos: "));
    user.grasaPorcentaje = static_cast<int>(getNumber("Porcentaje de grasas: "));

    // Comprobar que el usuario no es deficiente mental
    if (user.proteinaPorcentaje + user.carbsPorcentaje + user.grasaPorcentaje == 100){
        cout << "\nPorcentajes de macros procesados y guardados con exito. \n\n";
        this_thread::sleep_for(chrono::milliseconds(2000)); // Dos segundos
    } else{
        cout << "\nEspabila eso no suma 100 jefe, te quedas con los predeterminados \n\n";
        user.proteinaPorcentaje = 25;
        user.carbsPorcentaje= 55;
        user.grasaPorcentaje= 20;
        this_thread::sleep_for(chrono::milliseconds(5000)); // Cinco segundos
    }

    clearTerminal();
    printHeader();

    cout << "Su nuevo requerimiento de calorias de mantenimiento es: "
         << user.maintenanceCal << " kcal. \n\n";

    calculateGoalMacros(user.proteinaPorcentaje, user.carbsPorcentaje, user.grasaPorcentaje, user.maintenanceCal, user.targetProtein, user.targetFat, user.targetCarb);

    // Ajustar los valores restantes a los nuevos targets
    user.remainingCal      = user.maintenanceCal - calConsumed;
    user.remainingProtein  = user.targetProtein - proteinConsumed;
    user.remainingFat      = user.targetFat - fatConsumed;
    user.remainingCarb     = user.targetCarb - carbConsumed;

    saveUserData(filename, user);
}

void recalculateUserData (Usuario &user){
    double proteinConsumed = user.targetProtein - user.remainingProtein;
    double fatConsumed = user.targetFat - user.remainingFat;
    double carbConsumed = user.targetCarb - user.remainingCarb;
    double calConsumed = user.maintenanceCal - user.remainingCal;

    user.maintenanceCal = calculateCalories(user.peso, user.altura, user.edad, user.formula);
    calculateGoalMacros(user.proteinaPorcentaje, user.carbsPorcentaje, user.grasaPorcentaje, user.maintenanceCal, user.targetProtein, user.targetFat, user.targetCarb);


    user.remainingCal      = user.maintenanceCal - calConsumed;
    user.remainingProtein  = user.targetProtein - proteinConsumed;
    user.remainingFat      = user.targetFat - fatConsumed;
    user.remainingCarb     = user.targetCarb - carbConsumed;
}

void manageEditUserData(Usuario &user, const string &filename){
   char opcion;
        do {
            cout << "Menu de opciones: \n";
            cout << "0. Editar todas las opciones \n";
            cout << "1. Editar el nombre \n";
            cout << "2. Editar la altura \n";
            cout << "3. Editar el peso \n";
            cout << "4. Editar la edad \n";
            cout << "5. Editar la formula de calculo \n";
            cout << "6. Editar los porcentajes de macros \n";
            cout << "X. Volver al menu principal \n";
            cout << "Ingrese una opcion: ";
            cin >> opcion;

            cin.ignore(MAX_INTEGER, '\n');
            clearTerminal();
            printHeader();
            switch (opcion) { //Abstraer esta chapuzada
                case '0':
                    editUserData(user, filename);

                    break;
                case '1':
                    cout << "Ingrese su nuevo nombre (sin tildes ni valores especiales): ";
                    getline(cin, user.nombre);

                    saveUserData(filename, user);

                    clearTerminal();
                    printHeader();
                    cout << "Bienvenido, " << user.nombre << "\n\n";
                    break;
                case '2':
                    user.altura = getNumber("Ingrese su nueva altura en cm: ");

                    recalculateUserData(user);

                    saveUserData(filename, user);

                    clearTerminal();
                    printHeader();
                    cout << "Altura procesada y guardada con exito. \n\n";
                    break;
                case '3':
                    user.peso = getNumber("Ingrese su nuevo peso en kg: ");

                    recalculateUserData(user);

                    saveUserData(filename, user);

                    clearTerminal();
                    printHeader();
                    cout << "Peso procesado y guardada con exito. \n\n";
                    break;
                case '4':
                    user.edad = static_cast<int>(getNumber("Ingrese su nueva edad en anhos: "));

                    recalculateUserData(user);

                    saveUserData(filename, user);

                    clearTerminal();
                    printHeader();
                    cout << "Edad procesada y guardada con exito. \n";
                    break;
                case '5':
                    user.formula = chooseCalorieFormula();

                    recalculateUserData(user);

                    saveUserData(filename, user);

                    clearTerminal();
                    printHeader();
                    cout << "\nFormula procesada y guardada con exito.\n";
                    break;
                case '6':{
                    cout << "Ingrese a continuacion el porcentaje de macros que desea para su dieta (sin poner el % y que sume 100 por favor): \n\n";
                    user.proteinaPorcentaje = static_cast<int>(getNumber("Porcentaje de proteinas:  "));
                    user.carbsPorcentaje = static_cast<int>(getNumber("Porcentaje de carbohidratos: "));
                    user.grasaPorcentaje = static_cast<int>(getNumber("Porcentaje de grasas: "));

                    if (user.proteinaPorcentaje + user.carbsPorcentaje + user.grasaPorcentaje == 100){
                        clearTerminal();
                        printHeader();
                        cout << "\nPorcentajes de macros procesados y guardados con exito. \n\n";
                    } else{
                        user.proteinaPorcentaje = 25;
                        user.carbsPorcentaje= 55;
                        user.grasaPorcentaje= 20;

                        clearTerminal();
                        printHeader();
                        cout << "\nEspabila eso no suma 100 jefe, te quedas con los predeterminados (se pueden editar despues)\n\n";
                    }

                    double proteinConsumed = user.targetProtein - user.remainingProtein;
                    double fatConsumed = user.targetFat - user.remainingFat;
                    double carbConsumed = user.targetCarb - user.remainingCarb;

                    calculateGoalMacros(user.proteinaPorcentaje, user.carbsPorcentaje, user.grasaPorcentaje, user.maintenanceCal, user.targetProtein, user.targetFat, user.targetCarb);

                    user.remainingProtein  = user.targetProtein - proteinConsumed;
                    user.remainingFat      = user.targetFat - fatConsumed;
                    user.remainingCarb     = user.targetCarb - carbConsumed;

                    saveUserData(filename, user);
                    break;
                    }
                case 'X':
                case 'x':
                    break;
                default:
                    cout << "Opcion invalida, intente de nuevo. \n\n";
            }
        } while(opcion != 'X' && opcion != 'x');
}


// ---------------------- Funciones para cálculos ----------------------
int chooseCalorieFormula() {
    int opcion;
    cout << "\nSeleccione la formula para calcular las calorias de mantenimiento: \n";
    cout << "1. Formula Hombres: Calorias = (10 * peso en kg) + (6.25 * altura en cm) - (5 * edad) + 5 \n";
    cout << "2. Formula Mujeres: Calorias = (10 * peso en kg) + (6.25 * altura en cm) - (5 * edad) - 161 \n";
    do {
        cout << "Ingrese 1 o 2: ";
        cin >> opcion;
        cin.clear();
        cin.ignore(MAX_INTEGER, '\n');
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
void calculateGoalMacros(double &proteinaPorcentaje, double &carbsProcentaje, double &grasaPorcentaje, double maintenanceCal, double &targetProtein, double &targetFat, double &targetCarb) {
    targetProtein = (maintenanceCal * (proteinaPorcentaje/100)) / 4;
    targetFat     = (maintenanceCal * (grasaPorcentaje/100)) / 9;
    targetCarb    = (maintenanceCal * (carbsProcentaje/100)) / 4;
}

void resetDailyMarker(const string &userFile, Usuario &user) {
    user.remainingCal     = user.maintenanceCal;
    user.remainingProtein = user.targetProtein;
    user.remainingFat     = user.targetFat;
    user.remainingCarb    = user.targetCarb;
    cout << "Marcador diario reiniciado: \n\n";
    printRemaining(user.remainingCal, user.remainingProtein, user.remainingFat, user.remainingCarb);

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
        cout << "Error al abrir el archivo para guardar objetos. \n";
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
    cout << "\nNombre procesado y guardado con exito. \n\n";

    cout << "Los gramos a continuacion expreselos por cada 100 gramos de producto. Ademas, los valores decimales separelos con un punto no una coma. \n\n";

    nuevo.proteina = getNumber ("Ingrese gramos de proteina: ");
    cout << "Proteina procesada y guardada con exito. \n\n";

    nuevo.carbohidratos = getNumber("Ingrese gramos de carbohidratos: ");
    cout << "Carbohidratos procesados y guardados con exito. \n\n";

    nuevo.grasa = getNumber("Ingrese gramos de grasa: ");
    cout << "Grasa procesada y guardada con exito. \n\n";

    nuevo.fibra = getNumber("Ingrese gramos de fibra: ");
    cout << "Fibra procesada y guardada con exito. \n\n";

    // Calcular las calorías del objeto: proteína*4 + carbohidratos*4 + grasa*9 + fibra*2
    nuevo.calorias = (nuevo.proteina * 4) + (nuevo.carbohidratos * 4) + (nuevo.grasa * 9) + (nuevo.fibra * 2);

    // Cargar los objetos existentes, agregar el nuevo y guardar
    vector<Objeto> objetos = loadObjects(filename);
    objetos.push_back(nuevo);
    saveObjects(filename, objetos); //modificar esto de urgencia

    clearTerminal();
    printHeader();
    cout << "Objeto creado y guardado con exito. \n\n";
}

void deleteObject(const string &filename) {
    vector<Objeto> objetos = loadObjects(filename);
    if (objetos.empty()) {
        cout << "No hay objetos guardados para eliminar. \n\n";
        return;
    }

    // Mostrar los objetos con su índice
    cout << "Objetos actuales: \n";
    printObjects(objetos);

    int indice;
    cout << "\nIngrese el numero del objeto que desea eliminar: ";
    cin >> indice;
    if (cin.fail()||indice < 0 || indice >= (int)objetos.size()) {
        cout << "\nIndice invalido. No se realizo ninguna eliminacion. \n\n";
        cin.clear();
        cin.ignore(MAX_INTEGER, '\n');
        return;
    }

    // Eliminar el objeto seleccionado y guardar los cambios
    objetos.erase(objetos.begin() + indice);
    saveObjects(filename, objetos);
    cout << "\nObjeto eliminado, procesado y guardado con exito.\n\n";
}

void manageObject(const string &filename){
   char opcion; //char porque soy un vago, el programa es gratis éche o que hai.
        do {
            cout << "Menu de opciones: \n";
            cout << "0. Crear un nuevo objeto nutricional \n";
            cout << "1. Eliminar un objeto nutricional existente \n";
            cout << "X. Volver al menu principal \n";
            cout << "Ingrese una opcion: ";
            cin >> opcion;

            cin.ignore(MAX_INTEGER, '\n');
            clearTerminal();
            printHeader();
            switch (opcion) {
                case '0':
                    addObject(filename);
                    break;
                case '1':
                    deleteObject(filename);
                    break;
                case 'X':
                case 'x':
                    break;
                default:
                    cout << "Opcion invalida, intente de nuevo. \n\n";
            }
        } while(opcion != 'X' && opcion != 'x');
}

void consumeObjects(const string &objFile, const string &userFile, Usuario &user) {
    vector<Objeto> objetos = loadObjects(objFile);
    if (objetos.empty()) {
        cout << "No hay objetos nutricionales disponibles. \n\n";
        return;
    }

    double totalConsumedCal = 0;
    double totalConsumedProtein = 0;
    double totalConsumedFat = 0;
    double totalConsumedCarb = 0;
    double totalConsumedFibra = 0;

    cout << "Listado de objetos nutricionales: \n";
    printObjects(objetos);

    cout << "\nIngrese el numero del objeto que desea consumir o 'X' para finalizar: ";
    string input;

    while (getline(cin, input)) {
        if (input == "x" || input == "X"){
            break;
        }
        if(input.empty())
            continue;
        int index = -1;
        try {
            index = stoi(input);
        } catch (...) {
            cout << "\nEntrada invalida. Intente de nuevo: ";
            continue;
        }
        if(index < 0 || index >= (int)objetos.size()){
            cout << "\nIndice fuera de rango. Intente de nuevo: ";
            continue;
        }

        double gramos = 0;
        while (true) { //Chapuzada de época pero no me valía la función normal y no tengo tiempo.
            // Preguntar cuántos gramos se consumieron
            cout << "\nCuantos gramos de " << objetos[index].nombre << " consumiste? ";
            cin >> gramos;
            if (cin.fail() || gramos < 0) {
                cout << "Entrada invalida. Intente de nuevo. \n\n";
                cin.clear();
                cin.ignore(MAX_INTEGER, '\n');
            } else {
                cin.ignore(MAX_INTEGER, '\n');
                break;
            }
        }

        // Calcular proporción
        double factor = gramos / 100.0;

        totalConsumedCal     += objetos[index].calorias * factor;
        totalConsumedProtein += objetos[index].proteina * factor;
        totalConsumedFat     += objetos[index].grasa * factor;
        totalConsumedCarb    += objetos[index].carbohidratos * factor;
        totalConsumedFibra   += objetos[index].fibra * factor;

        cout << "\nHas consumido: " << gramos << "g de " << objetos[index].nombre << endl;
        cout << "\nIngrese otro numero o 'X' para finalizar: ";
    }

    // Actualizar los marcadores diarios
    user.remainingCal      -= totalConsumedCal;
    user.remainingProtein  -= totalConsumedProtein;
    user.remainingFat      -= totalConsumedFat;
    user.remainingCarb     -= totalConsumedCarb;

    clearTerminal();
    printHeader();

    cout << "Total consumido:\n";
    cout << "Calorias: " << totalConsumedCal << " Kcal \n";
    cout << "Proteina: " << totalConsumedProtein << " g \n";
    cout << "Grasa: " << totalConsumedFat << " g \n";
    cout << "Carbohidratos: " << totalConsumedCarb << " g \n";
    cout << "Fibra: " << totalConsumedFibra << " g \n";

    cout << "\nMarcador diario actualizado: \n\n";
    printRemaining(user.remainingCal, user.remainingProtein, user.remainingFat, user.remainingCarb);

    // Guardar los nuevos valores en el archivo de usuario
    saveUserData(userFile, user);
}

// ---------------------- Funciones auxiliares ----------------------
// Función auxiliar para imprimir el valor con color
void printValue(const string &etiqueta, double &valor, const string &unidad) {
    // Si el valor es negativo, se imprime en rojo; si es mayor o igual a cero, en verde.
    if (valor < 0)
        cout << etiqueta << ": " << "\033[31m" << valor << " " << unidad << "\033[0m" << endl;
    else
        cout << etiqueta << ": " << "\033[32m" << valor << " " << unidad << "\033[0m" << endl;
}

void printRemaining(double &remainingCal, double &remainingProtein, double &remainingFat, double &remainingCarb) {
    if (remainingCal < 0 || remainingProtein < 0 || remainingFat < 0 || remainingCarb < 0) {
        cout << "\033[31mAtencion: Has excedido tus limites diarios!\033[0m" << endl;
    } else {
        cout << "\033[32mMarcador diario:\033[0m" << endl;
    }

    // Se imprime cada información con el color correspondiente
    printValue("Calorias restantes", remainingCal, "Kcal");
    printValue("Proteina restante", remainingProtein, "g");
    printValue("Grasa restante", remainingFat, "g");
    printValue("Carbohidratos restantes", remainingCarb, "g");
    cout << "\n";
}

double getNumber(const string& mensaje) {
    double valor;
    while (true) {
        cout << mensaje;
        cin >> valor;
        if (cin.fail() || valor < 0) {
            cout << "Entrada invalida. Intente de nuevo. \n\n";
            cin.clear();
            cin.ignore(MAX_INTEGER, '\n');
        } else {
            cin.ignore(MAX_INTEGER, '\n');
            return valor;
        }
    }
}

int getValidIndex(int max, const string& mensaje) { //Sí, sé que no se usa en nada. Está para aportar presencia.
    int indice;
    while (true) {
        cout << mensaje;
        cin >> indice;
        if (cin.fail() || indice < 0 || indice >= max) {
            cout << "Indice invalido. Intente de nuevo. \n";
            cin.clear();
            cin.ignore(MAX_INTEGER, '\n');
        } else {
            cin.ignore(MAX_INTEGER, '\n');
            return indice;
        }
    }
}

void printObjects(const vector<Objeto> &objetos) { //función alterativa que imprima objetos largos en versión reducida para que no ocupe tanto en pantalla
    for (size_t i = 0; i < objetos.size(); i++) {
        cout << i << ". " << objetos[i].nombre
             << " (Proteina: " << objetos[i].proteina
             << " g, Carbohidratos: " << objetos[i].carbohidratos
             << " g, Grasa: " << objetos[i].grasa
             << " g, Fibra: " << objetos[i].fibra
             << " g, Calorias: " << objetos[i].calorias << " Kcal)\n";
    }
}

void printHeader(){
    time_t t = time(nullptr);
    tm *horaLocal = localtime(&t);

    cout << put_time(horaLocal, "%d/%m/%Y %H:%M") << "                                                     Macro Tracker " << VERSION << " Copyright (C) 2025 Biosbardos" << "\n\n";
}

void clearTerminal() {
    cout << "\033[2J\033[1;1H";
}

void printProjectInfo(double &proteinaPorcentaje, double &carbsPorcentaje, double &grasaPorcentaje){
    cout << "Macro Tracker " << VERSION << "\n";
    cout << "Copyright (C) 2025 Biosbardos"  << "\n\n";
    //cout << "Ubicacion del proyecto: " << std::filesystem::current_path() << "\n"; //Da fallos con mi versión de compilador, tendré que ver
    cout << "Formula de calculo de calorias: Harris-Benedict\n";
    cout << "Distrubucion de macros: " << proteinaPorcentaje << "% proteinas, " << carbsPorcentaje << "% carbohidratos y " << grasaPorcentaje << "% grasas \n";
    cout << "Archivos de la aplicacion: datos.txt, objetos.txt\n"; //archivo para la música para poner en .mp3 MP3 NO WAV la música que quieres de fondo TO DO
    cout << "Lenguaje de programacion: Cpp\n\n";
}
