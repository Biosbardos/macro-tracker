#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

/* ================= Prototipos de funciones ================= */

// Funciones para manejo de datos del usuario

/**
 * @brief Carga los datos del usuario desde el archivo.
 *
 * El archivo debe tener esta estructura (una entrada por línea):
 *   1. Nombre
 *   2. Altura (cm)
 *   3. Peso (kg)
 *   4. Edad (anhos)
 *   5. Fórmula elegida (int)
 *   6. Calorías de mantenimiento calculadas
 *   7. Calorías restantes (marcador diario)
 *   8. Gramos de proteína target
 *   9. Gramos de grasa target
 *   10. Gramos de carbohidratos target
 *   11. Gramos de proteína restantes
 *   12. Gramos de grasa restantes
 *   13. Gramos de carbohidratos restantes
 *
 * @param filename Archivo donde se encuentran los datos.
 * @param nombre Referencia donde se cargará el nombre.
 * @param altura Referencia para la altura.
 * @param peso Referencia para el peso.
 * @param edad Referencia para la edad.
 * @param formula Referencia para la opción de fórmula.
 * @param maintenanceCal Calorías de mantenimiento calculadas.
 * @param remainingCal Calorías restantes.
 * @param targetProtein Gramos de proteína target.
 * @param targetFat Gramos de grasa target.
 * @param targetCarb Gramos de carbohidratos target.
 * @param remainingProtein Gramos de proteína restantes.
 * @param remainingFat Gramos de grasa restantes.
 * @param remainingCarb Gramos de carbohidratos restantes.
 * @return true si se cargaron correctamente los datos, false en caso contrario.
 */
bool loadUserData(const string &filename,
                  string &nombre,
                  double &altura,
                  double &peso,
                  int &edad,
                  int &formula,
                  double &maintenanceCal,
                  double &remainingCal,
                  double &targetProtein,
                  double &targetFat,
                  double &targetCarb,
                  double &remainingProtein,
                  double &remainingFat,
                  double &remainingCarb);

/**
 * @brief Guarda los datos del usuario en el archivo.
 *
 * Se guardan los 13 elementos según la estructura definida en loadUserData.
 *
 * @param filename Archivo en el que se guardarán los datos.
 * @param nombre Nombre del usuario.
 * @param altura Altura del usuario.
 * @param peso Peso del usuario.
 * @param edad Edad del usuario.
 * @param formula Fórmula seleccionada.
 * @param maintenanceCal Calorías de mantenimiento calculadas.
 * @param remainingCal Calorías restantes.
 * @param targetProtein Gramos de proteína target.
 * @param targetFat Gramos de grasa target.
 * @param targetCarb Gramos de carbohidratos target.
 * @param remainingProtein Gramos de proteína restantes.
 * @param remainingFat Gramos de grasa restantes.
 * @param remainingCarb Gramos de carbohidratos restantes.
 */
void saveUserData(const string &filename,
                  const string &nombre,
                  double altura,
                  double peso,
                  int edad,
                  int formula,
                  double maintenanceCal,
                  double remainingCal,
                  double targetProtein,
                  double targetFat,
                  double targetCarb,
                  double remainingProtein,
                  double remainingFat,
                  double remainingCarb);

/**
 * @brief Permite editar los datos del usuario y recalcular las calorías y macros.
 *
 * Tras cada input se muestra un mensaje de confirmación y se recalculan:
 *   - Calorías de mantenimiento (usando calculateCalories)
 *   - Macros target: 25% de las calorías en proteína (4 cal/g), 20% en grasa (9 cal/g) y 55% en carbohidratos (4 cal/g)
 *   - Además, se reinician los marcadores diarios (restantes) con los valores target.
 *
 * @param nombre Referencia al nombre.
 * @param altura Referencia a la altura.
 * @param peso Referencia al peso.
 * @param edad Referencia a la edad.
 * @param formula Referencia a la opción de fórmula.
 * @param maintenanceCal Calorías de mantenimiento.
 * @param remainingCal Calorías restantes (se actualizarán).
 * @param targetProtein Gramos de proteína target (se calcularán).
 * @param targetFat Gramos de grasa target (se calcularán).
 * @param targetCarb Gramos de carbohidratos target (se calcularán).
 * @param remainingProtein Gramos de proteína restantes (se reinician).
 * @param remainingFat Gramos de grasa restantes (se reinician).
 * @param remainingCarb Gramos de carbohidratos restantes (se reinician).
 * @param filename Archivo donde se guardan los datos.
 */
void editUserData(string &nombre,
                  double &altura,
                  double &peso,
                  int &edad,
                  int &formula,
                  double &maintenanceCal,
                  double &remainingCal,
                  double &targetProtein,
                  double &targetFat,
                  double &targetCarb,
                  double &remainingProtein,
                  double &remainingFat,
                  double &remainingCarb,
                  const string &filename);

/**
 * @brief Permite seleccionar la fórmula para el cálculo de calorías.
 *
 * Se muestran las siguientes opciones:
 *   1. Fórmula Hombres: Calorías = (10 × peso en kg) + (6.25 × altura en cm) − (5 × edad) + 5
 *   2. Fórmula Mujeres: Calorías = (10 × peso en kg) + (6.25 × altura en cm) − (5 × edad) − 161
 *
 * @return La opción elegida (1 o 2).
 */
int chooseCalorieFormula();

/**
 * @brief Calcula las calorías de mantenimiento en función del peso, altura, edad y fórmula.
 *
 * @param peso Peso del usuario.
 * @param altura Altura del usuario.
 * @param edad Edad del usuario.
 * @param formulaOption Opción de fórmula elegida.
 * @return Calorías calculadas.
 */
double calculateCalories(double peso, double altura, int edad, int formulaOption);

/**
 * @brief Reinicia el marcador diario: reestablece las calorías y macros restantes a sus valores target.
 *
 * @param userFile Archivo de datos del usuario.
 * @param nombre Nombre del usuario.
 * @param altura Altura del usuario.
 * @param peso Peso del usuario.
 * @param edad Edad del usuario.
 * @param formula Fórmula elegida.
 * @param maintenanceCal Calorías de mantenimiento.
 * @param targetProtein Gramos de proteína target.
 * @param targetFat Gramos de grasa target.
 * @param targetCarb Gramos de carbohidratos target.
 * @param remainingCal Referencia a las calorías restantes (se actualizarán).
 * @param remainingProtein Referencia a los gramos de proteína restantes.
 * @param remainingFat Referencia a los gramos de grasa restantes.
 * @param remainingCarb Referencia a los gramos de carbohidratos restantes.
 */
void resetDailyMarker(const string &userFile,
                      const string &nombre,
                      double altura,
                      double peso,
                      int edad,
                      int formula,
                      double maintenanceCal,
                      double targetProtein,
                      double targetFat,
                      double targetCarb,
                      double &remainingCal,
                      double &remainingProtein,
                      double &remainingFat,
                      double &remainingCarb);

// Funciones para manejo de objetos nutricionales

/**
 * @brief Estructura que define un objeto nutricional.
 */
struct Objeto {
    string nombre;
    double proteina;
    double carbohidratos;
    double grasa;
    double fibra;
    double calorias; // Se calcula como: proteina*4 + carbohidratos*4 + grasa*9 + fibra*2
};

/**
 * @brief Carga la lista de objetos desde el archivo.
 *
 * Cada línea en el archivo debe tener el formato:
 *   nombre,proteina,carbohidratos,grasa,calorias
 *
 * @param filename Archivo donde están almacenados los objetos.
 * @return Vector de objetos leídos.
 */
vector<Objeto> loadObjects(const string &filename);

/**
 * @brief Guarda la lista de objetos en el archivo.
 *
 * @param filename Archivo destino.
 * @param objetos Vector de objetos nutricionales.
 */
void saveObjects(const string &filename, const vector<Objeto> &objetos);

/**
 * @brief Permite crear un nuevo objeto nutricional, solicitando sus datos, calculando sus calorías
 *        y guardándolo en el archivo.
 *
 * @param filename Archivo donde se almacenan los objetos.
 */
void addObject(const string &filename);

/**
 * @brief Elimina un objeto nutricional existente. Se muestran los objetos actuales y se pide seleccionar
 *        el objeto a eliminar.
 *
 * @param filename Archivo donde se almacenan los objetos.
 */
void deleteObject(const string &filename);

/**
 * @brief Permite "consumir" objetos nutricionales. Se muestran los objetos existentes y el usuario puede
 *        seleccionar (repetidamente) cuáles consumir hasta que ingrese "f". Se actualizan las calorías
 *        y macros restantes y se informa al usuario.
 *
 * @param objFile Archivo de objetos.
 * @param userFile Archivo de datos del usuario.
 * @param nombre Nombre del usuario.
 * @param altura Altura del usuario.
 * @param peso Peso del usuario.
 * @param edad Edad del usuario.
 * @param formula Fórmula elegida.
 * @param maintenanceCal Calorías de mantenimiento.
 * @param targetProtein Gramos de proteína target.
 * @param targetFat Gramos de grasa target.
 * @param targetCarb Gramos de carbohidratos target.
 * @param remainingCal Referencia a las calorías restantes (se actualizarán).
 * @param remainingProtein Referencia a la proteína restante.
 * @param remainingFat Referencia a la grasa restante.
 * @param remainingCarb Referencia a los carbohidratos restantes.
 */
void consumeObjects(const string &objFile,
                    const string &userFile,
                    const string &nombre,
                    double altura,
                    double peso,
                    int edad,
                    int formula,
                    double maintenanceCal,
                    double targetProtein,
                    double targetFat,
                    double targetCarb,
                    double &remainingCal,
                    double &remainingProtein,
                    double &remainingFat,
                    double &remainingCarb);

/**
 * @brief Muestra por pantalla las calorías y macros restantes del día.
 *
 * Imprime en consola el estado actual de las calorías, proteína, grasa y carbohidratos restantes,
 * es decir, los valores que aún puede consumir el usuario en el día según su objetivo y lo que ya ha consumido.
 *
 * @param remainingCal Calorías restantes.
 * @param remainingProtein Gramos de proteína restantes.
 * @param remainingFat Gramos de grasa restantes.
 * @param remainingCarb Gramos de carbohidratos restantes.
 */
void mostrarRestante(double remainingCal,
                    double remainingProtein,
                    double remainingFat,
                    double remainingCarb);

/* ================= Función principal ================= */
int main() {
    const string userDataFile = "datos.txt";
    const string objDataFile  = "objetos.txt";
    string nombre;
    double altura, peso;
    int edad, formula;

    double maintenanceCal;
    double remainingCal;
    double targetProtein, targetFat, targetCarb;
    double remainingProtein, remainingFat, remainingCarb;

    // Si no existe el archivo de datos, es la primera ejecución.
    if (!loadUserData(userDataFile, nombre, altura, peso, edad, formula,
                      maintenanceCal, remainingCal,
                      targetProtein, targetFat, targetCarb,
                      remainingProtein, remainingFat, remainingCarb))
    {
        // Ingreso de datos (primera vez)
        cout << "Ingrese su nombre (sin tildes ni valores especiales): ";
        getline(cin, nombre);
        cout << "Bienvenido, " << nombre << endl;

        cout << "Ingrese su altura en cm: ";
        cin >> altura;
        cout << "Altura procesada y guardada con exito." << endl;

        cout << "Ingrese su peso en kg: ";
        cin >> peso;
        cout << "Peso procesado y guardado con exito." << endl;

        cout << "Ingrese su edad en anhos: ";
        cin >> edad;
        cout << "Edad procesada y guardada con exito." <<endl;

        cout << "\nSeleccione la formula para calcular las calorias de mantenimiento:" << endl;
        cout << "1. Formula Hombres: Calorias = (10 * peso en kg) + (6.25 * altura en cm) - (5 * edad) + 5" << endl;
        cout << "2. Formula Mujeres: Calorias = (10 * peso en kg) + (6.25 * altura en cm) - (5 * edad) - 161" << endl;
        cout << "Ingrese 1 o 2: ";
        cin >> formula;
        cout << "Formula procesada y guardada con exito." << endl;

        // Calcular las calorías de mantenimiento
        maintenanceCal = calculateCalories(peso, altura, edad, formula);
        cout << "\nSu requerimiento de calorias de mantenimiento es: "
             << maintenanceCal << " kcal." << endl;

        // Calcular los targets de macros según porcentajes predeterminados:
        // 25% de proteína (4 cal/g), 20% de grasa (9 cal/g) y 55% de carbohidratos (4 cal/g)
        targetProtein = (maintenanceCal * 0.25) / 4;
        targetFat     = (maintenanceCal * 0.20) / 9;
        targetCarb    = (maintenanceCal * 0.55) / 4;

        // Inicialmente, los marcadores diarios son iguales a los targets
        remainingCal      = maintenanceCal;
        remainingProtein  = targetProtein;
        remainingFat      = targetFat;
        remainingCarb     = targetCarb;

        // Guardar la información del usuario
        saveUserData(userDataFile, nombre, altura, peso, edad, formula,
                     maintenanceCal, remainingCal,
                     targetProtein, targetFat, targetCarb,
                     remainingProtein, remainingFat, remainingCarb);

        // Terminar el programa tras la primera ejecución
        return 0;
    }
    else {
        // Ejecuciones subsiguientes: se cargan los datos y se le da la bienvenida al usuario.
        cout << "Bienvenido de nuevo, " << nombre << "!" << endl;

        int opcion;
        do {
            cout << "\nMenu de opciones:" << endl;
            cout << "0. Editar datos de usuario (nombre, altura, peso, edad, formula) (un enter pls)" << endl;
            cout << "1. Crear un objeto nutricional (dale un enter antes de poner nada)" << endl;
            cout << "2. Eliminar un objeto nutricional" << endl;
            cout << "3. Consumir objetos nutricionales" << endl;
            cout << "4. Reiniciar marcador diario (nuevo dia)" << endl;
            cout << "5. Mostrar calorias y macros restantes" << endl;
            cout << "6. Salir" << endl;
            cout << "Ingrese una opcion: ";
            cin >> opcion;
            cin.ignore(); // Limpiar el buffer de nueva línea
            switch (opcion) {
                case 0:
                    editUserData(nombre, altura, peso, edad, formula,
                                 maintenanceCal, remainingCal,
                                 targetProtein, targetFat, targetCarb,
                                 remainingProtein, remainingFat, remainingCarb,
                                 userDataFile);
                    break;
                case 1:
                    addObject(objDataFile);
                    break;
                case 2:
                    deleteObject(objDataFile);
                    break;
                case 3:
                    consumeObjects(objDataFile, userDataFile, nombre, altura, peso, edad, formula,
                                   maintenanceCal, targetProtein, targetFat, targetCarb,
                                   remainingCal, remainingProtein, remainingFat, remainingCarb);
                    break;
                case 4:
                    resetDailyMarker(userDataFile, nombre, altura, peso, edad, formula,
                                     maintenanceCal, targetProtein, targetFat, targetCarb,
                                     remainingCal, remainingProtein, remainingFat, remainingCarb);
                    break;
                case 5:
                    mostrarRestante(remainingCal, remainingProtein, remainingFat, remainingCarb);
                    break;
                case 6:
                    cout << "Saliendo del programa. Hasta luego!" << endl;
                    break;
                default:
                    cout << "Opcion invalida, intente de nuevo." << endl;
            }
        } while(opcion != 6);
    }
    return 0;
}

/* ================= Definiciones de funciones ================= */

// ---------------------- Funciones de datos del usuario ----------------------
bool loadUserData(const string &filename,
                  string &nombre,
                  double &altura,
                  double &peso,
                  int &edad,
                  int &formula,
                  double &maintenanceCal,
                  double &remainingCal,
                  double &targetProtein,
                  double &targetFat,
                  double &targetCarb,
                  double &remainingProtein,
                  double &remainingFat,
                  double &remainingCarb) {
    ifstream infile(filename);
    if (!infile)
        return false;

    getline(infile, nombre);
    infile >> altura >> peso >> edad >> formula >> maintenanceCal >> remainingCal
           >> targetProtein >> targetFat >> targetCarb
           >> remainingProtein >> remainingFat >> remainingCarb;
    infile.close();
    return true;
}

void saveUserData(const string &filename,
                  const string &nombre,
                  double altura,
                  double peso,
                  int edad,
                  int formula,
                  double maintenanceCal,
                  double remainingCal,
                  double targetProtein,
                  double targetFat,
                  double targetCarb,
                  double remainingProtein,
                  double remainingFat,
                  double remainingCarb) {
    ofstream outfile(filename);
    if (!outfile) {
        cout << "Error al abrir el archivo para guardar los datos." << endl;
        exit(1);
    }
    outfile << nombre << "\n"
            << altura << "\n"
            << peso << "\n"
            << edad << "\n"
            << formula << "\n"
            << maintenanceCal << "\n"
            << remainingCal << "\n"
            << targetProtein << "\n"
            << targetFat << "\n"
            << targetCarb << "\n"
            << remainingProtein << "\n"
            << remainingFat << "\n"
            << remainingCarb;
    outfile.close();
}

void editUserData(string &nombre,
                  double &altura,
                  double &peso,
                  int &edad,
                  int &formula,
                  double &maintenanceCal,
                  double &remainingCal,
                  double &targetProtein,
                  double &targetFat,
                  double &targetCarb,
                  double &remainingProtein,
                  double &remainingFat,
                  double &remainingCarb,
                  const string &filename) {
    cin.ignore(); // Limpiar el buffer
    cout << "Ingrese su nuevo nombre (sin tildes ni valores especiales): ";
    getline(cin, nombre);
    cout << "Bienvenido, " << nombre << endl;

    cout << "Ingrese su nueva altura en cm: ";
    cin >> altura;
    cout << "Altura procesada y guardada con exito." << endl;

    cout << "Ingrese su nuevo peso en kg: ";
    cin >> peso;
    cout << "Peso procesado y guardado con exito." << endl;


    cout << "Ingrese su nueva edad en anhos: ";
    cin >> edad;
    cout << "Edad procesada y guardada con exito." <<endl;

    cout << "\nSeleccione la formula para calcular las calorias de mantenimiento:" << endl;
    cout << "1. Formula Hombres: Calorias = (10 * peso en kg) + (6.25 * altura en cm) - (5 * edad) + 5" << endl;
    cout << "2. Formula Mujeres: Calorias = (10 * peso en kg) + (6.25 * altura en cm) - (5 * edad) - 161" << endl;
    cout << "Ingrese 1 o 2: ";
    cin >> formula;
    cout << "Formula procesada y guardada con exito." << endl;

    double proteinConsumed = targetProtein - remainingProtein;
    double fatConsumed = targetFat - remainingFat;
    double carbConsumed = targetCarb - remainingCarb;
    double calConsumed = maintenanceCal - remainingCal;

    // Recalcular las calorías de mantenimiento y las metas de macros
    maintenanceCal = calculateCalories(peso, altura, edad, formula);
    cout << "\nSu nuevo requerimiento de calorias de mantenimiento es: "
         << maintenanceCal << " kcal." << endl;

    targetProtein = (maintenanceCal * 0.25) / 4;
    targetFat     = (maintenanceCal * 0.20) / 9;
    targetCarb    = (maintenanceCal * 0.55) / 4;

    // Reiniciar los marcadores diarios
    remainingCal      = maintenanceCal - calConsumed;
    remainingProtein  = targetProtein - proteinConsumed;
    remainingFat      = targetFat - fatConsumed;
    remainingCarb     = targetCarb - carbConsumed;

    saveUserData(filename, nombre, altura, peso, edad, formula,
                 maintenanceCal, remainingCal,
                 targetProtein, targetFat, targetCarb,
                 remainingProtein, remainingFat, remainingCarb);
}

// ---------------------- Funciones para cálculos ----------------------
int chooseCalorieFormula() {
    int opcion;
    cout << "\nSeleccione la formula para calcular las calorias de mantenimiento:" << endl;
    cout << "1. Formula Hombres: Calorias = (10 * peso en kg) + (6.25 * altura en cm) - (5 * edad) + 5" << endl;
    cout << "2. Formula Mujeres: Calorias = (10 * peso en kg) + (6.25 * altura en cm) - (5 * edad) - 161" << endl;
    cout << "Ingrese 1 o 2: ";
    cin >> opcion;
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

void resetDailyMarker(const string &userFile,
                      const string &nombre,
                      double altura,
                      double peso,
                      int edad,
                      int formula,
                      double maintenanceCal,
                      double targetProtein,
                      double targetFat,
                      double targetCarb,
                      double &remainingCal,
                      double &remainingProtein,
                      double &remainingFat,
                      double &remainingCarb) {

    remainingCal     = maintenanceCal;
    remainingProtein = targetProtein;
    remainingFat     = targetFat;
    remainingCarb    = targetCarb;
    cout << "\nMarcador diario reiniciado:" << endl;
    cout << "Calorias restantes: " << remainingCal << " Kcal" << endl;
    cout << "Proteina restante: " << remainingProtein << " g" << endl;
    cout << "Grasa restante: " << remainingFat << " g" << endl;
    cout << "Carbohidratos restantes: " << remainingCarb << " g" << endl;
    saveUserData(userFile, nombre, altura, peso, edad, formula,
                 maintenanceCal, remainingCal,
                 targetProtein, targetFat, targetCarb,
                 remainingProtein, remainingFat, remainingCarb);
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
    cin.ignore(); // Limpiar el buffer
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
    for (size_t i = 0; i < objetos.size(); i++) {
        cout << i << ". " << objetos[i].nombre
             << " (Proteina: " << objetos[i].proteina
             << " g, Carbohidratos: " << objetos[i].carbohidratos
             << " g, Grasa: " << objetos[i].grasa
             << " g, Fibra: " << objetos[i].fibra
             << " g, Calorias: " << objetos[i].calorias << " Kcal)" << endl;
    }

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

void consumeObjects(const string &objFile,
                    const string &userFile,
                    const string &nombre,
                    double altura,
                    double peso,
                    int edad,
                    int formula,
                    double maintenanceCal,
                    double targetProtein,
                    double targetFat,
                    double targetCarb,
                    double &remainingCal,
                    double &remainingProtein,
                    double &remainingFat,
                    double &remainingCarb) {
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
    for (size_t i = 0; i < objetos.size(); i++) {
        cout << i << ". " << objetos[i].nombre
             << " (Proteina: " << objetos[i].proteina << " g, Carbohidratos: "
             << objetos[i].carbohidratos << " g, Grasa: " << objetos[i].grasa
             << " g, Fibra: " << objetos[i].fibra
             << " g, Calorias: " << objetos[i].calorias << " Kcal)" << endl;
    }

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
    remainingCal      -= totalConsumedCal;
    remainingProtein  -= totalConsumedProtein;
    remainingFat      -= totalConsumedFat;
    remainingCarb     -= totalConsumedCarb;

    cout << "\nTotal consumido:" << endl;
    cout << "Calorias: " << totalConsumedCal << " Kcal" << endl;
    cout << "Proteina: " << totalConsumedProtein << " g" << endl;
    cout << "Grasa: " << totalConsumedFat << " g" << endl;
    cout << "Carbohidratos: " << totalConsumedCarb << " g" << endl;
    cout << "Fibra: " << totalConsumedFibra << " g" << endl;

    cout << "\nMarcador diario actualizado:" << endl;
    cout << "Calorias restantes: " << remainingCal << " Kcal" << endl;
    cout << "Proteina restante: " << remainingProtein << " g" << endl;
    cout << "Grasa restante: " << remainingFat << " g" << endl;
    cout << "Carbohidratos restantes: " << remainingCarb << " g" << endl;

    // Guardar los nuevos valores en el archivo de usuario
    saveUserData(userFile, nombre, altura, peso, edad, formula,
                 maintenanceCal, remainingCal,
                 targetProtein, targetFat, targetCarb,
                 remainingProtein, remainingFat, remainingCarb);
}

void mostrarRestante(double remainingCal, double remainingProtein, double remainingFat, double remainingCarb) {
    cout << "\nMarcador diario actual:" << endl;
    cout << "Calorias restantes: " << remainingCal << " Kcal" << endl;
    cout << "Proteina restante: " << remainingProtein << " g" << endl;
    cout << "Grasa restante: " << remainingFat << " g" << endl;
    cout << "Carbohidratos restantes: " << remainingCarb << " g" << endl;
}
