#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;
/* ================= Prototipos de funciones ================= */
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
 * @brief Estructura que define un usuario con sus datos nutricionales.
 *
 * Incluye nombre, altura, peso, edad, fórmula elegida, calorías de mantenimiento,
 * calorías restantes, metas de proteína, grasa y carbohidratos, y los valores restantes.
 */
struct Usuario {
    string nombre;
    double altura;
    double peso;
    int edad;
    int formula;
    double maintenanceCal;
    double remainingCal;
    double targetProtein;
    double targetFat;
    double targetCarb;
    double remainingProtein;
    double remainingFat;
    double remainingCarb;
};

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
 * @param user Estructura Usuario que contiene los datos del usuario.
 * @return true si se cargaron correctamente los datos, false en caso contrario.
 */
bool loadUserData(const string &filename, Usuario &user);

/**
 * @brief Guarda los datos del usuario en el archivo.
 *
 * Se guardan los 13 elementos según la estructura definida en loadUserData.
 *
 * @param filename Archivo en el que se guardarán los datos.
 * @param user Estructura Usuario que contiene los datos del usuario.
 * @return true si se guardaron correctamente los datos, false en caso contrario.
 */
bool saveUserData(const string &filename, Usuario &user);

/**
 * @brief Permite editar los datos del usuario y recalcular las calorías y macros.
 *
 * Tras cada input se muestra un mensaje de confirmación y se recalculan:
 *   - Calorías de mantenimiento (usando calculateCalories)
 *   - Macros target: 25% de las calorías en proteína (4 cal/g), 20% en grasa (9 cal/g) y 55% en carbohidratos (4 cal/g)
 *   - Además, se reinician los marcadores diarios (restantes) con los valores target.
 *
 * @param user Estructura Usuario que contiene los datos del usuario.
 * @param filename Archivo donde se guardan los datos.
 */
void editUserData(Usuario &user, const string &filename);

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
 * @param user Estructura Usuario que contiene los datos del usuario.
 */
void resetDailyMarker(const string &userFile, Usuario &user);

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
 * @param user Estructura Usuario que contiene los datos del usuario.
 */
void consumeObjects(const string &objFile, const string &userFile, Usuario &user);

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
void mostrarRestante(double &remainingCal,
                    double &remainingProtein,
                    double &remainingFat,
                    double &remainingCarb);

/**
 * @brief Calcula los valores objetivo de proteína, grasa y carbohidratos a partir de las calorías de mantenimiento.
 *
 * @param maintenanceCal Calorías de mantenimiento.
 * @param targetProtein Referencia donde se guarda la proteína objetivo.
 * @param targetFat Referencia donde se guarda la grasa objetivo.
 * @param targetCarb Referencia donde se guarda el carbohidrato objetivo.
 */
void calcularMacrosObjetivo(double maintenanceCal, double &targetProtein, double &targetFat, double &targetCarb);

/**
 * @brief Solicita un número al usuario y valida la entrada.
 *
 * @param mensaje Mensaje a mostrar.
 * @return double Valor introducido.
 */
double pedirNumero(const string& mensaje);

/**
 * @brief Solicita un índice válido al usuario, asegurándose de que esté dentro del rango [0, max).
 *
 * @param max Valor máximo (exclusivo) para el índice.
 * @param mensaje Mensaje a mostrar al usuario.
 * @return int Índice válido introducido por el usuario.
 */
int pedirIndiceValido(int max, const string& mensaje);

#endif // FUNCIONES_H_INCLUDED
