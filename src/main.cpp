/**
 * @file main.cpp
 * @brief Programa que permite al usuario llevar un registro de las calorias que consume y sus macros diarios.
 *
 * Este programa permite al usuario:
 *  -Interactuar con el programa a través de un menú de opciones, los datos se guardan en archivos para persistencia entre ejecuciones.
 *  -Ingresar sus datos personales (nombre, altura, peso, edad y fórmula de cálculo de calorías).
 *  -Calcular sus requerimientos calóricos de mantenimiento y sus macros objetivos.
 *  -Gestionar objetos nutricionales (creación, eliminación y consumo).
 *  -Reiniciar los marcadores diarios
 *  -Mostrar las calorías y macros restantes.
 *
 * El flujo del programa es el siguiente:
 *  1. Si es la primera ejecución, solicita los datos del usuario y los guarda en un archivo.
 *  2. En ejecuciones subsiguientes, carga los datos del usuario desde el archivo y muestra un menú de opciones.
 *  3. El usuario puede editar sus datos, crear o eliminar objetos nutricionales, consumir objetos y reiniciar los marcadores diarios.
 *  4. Los datos del usuario y los objetos nutricionales se guardan en archivos para su uso posterior.
 *
 * @author Biosbardos
 * @date 27/05/2025
 */

#include "funciones.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

/* ================= Función principal ================= */
int main() {
    const string userDataFile = "datos.txt";
    const string objDataFile  = "objetos.txt";
    Usuario user;

    printHeader(VERSION);

    // Si no existe el archivo de datos, es la primera ejecución.
    if (!loadUserData(userDataFile, user)){

        // Ingreso de datos (primera vez)
        cout << "Ingrese su nombre (sin tildes ni caracteres especiales): ";
        getline(cin, user.nombre);
        cout << "\nBienvenido, " << user.nombre << "\n\n";

        user.altura = getNumber("Ingrese su altura en cm: ");
        cout << "Altura procesada y guardada con exito. \n\n";

        user.peso = getNumber("Ingrese su peso en kg: ");
        cout << "Peso procesado y guardado con exito. \n\n";

        user.edad = static_cast<int>(getNumber("Ingrese su edad en anhos: "));
        cout << "Edad procesada y guardada con exito. \n";

        user.formula = chooseCalorieFormula();
        cout << "\nFormula procesada y guardada con exito. \n\n";

        // Calcular las calorías de mantenimiento
        user.maintenanceCal = calculateCalories(user.peso, user.altura, user.edad, user.formula);
        cout << "Su requerimiento de calorias de mantenimiento es: "
             << user.maintenanceCal << " kcal. \n";

        // Calcular los targets de macros según porcentajes predeterminados:
        // 25% de proteína (4 cal/g), 20% de grasa (9 cal/g) y 55% de carbohidratos (4 cal/g)
        calculateGoalMacros(user.maintenanceCal, user.targetProtein, user.targetFat, user.targetCarb);

        // Inicialmente, los marcadores diarios son iguales a los targets
        user.remainingCal      = user.maintenanceCal;
        user.remainingProtein  = user.targetProtein;
        user.remainingFat      = user.targetFat;
        user.remainingCarb     = user.targetCarb;

        // Guardar la información del usuario
        saveUserData(userDataFile, user);

        this_thread::sleep_for(chrono::milliseconds(5000)); // Cinco segundos para que dé tiempo a ver los últimos couts
        return 0;// Terminar el programa tras la primera ejecución

    } else { // Ejecuciones subsiguientes: se cargan los datos y se le da la bienvenida al usuario.

        cout << "Bienvenido de nuevo, " << user.nombre << "! \n\n";

        string opcion; // No rompas la cabeza, tiene que ser string por narices todo lo demás da problemas (que haya pensado yo).
        int index = -1;
        do {
            cout << "Menu de opciones: \n";
            cout << "0. Mostrar informacion del proyecto \n";
            cout << "1. Editar datos de usuario (nombre, altura, peso, edad, formula) \n";
            cout << "2. Administrar objetos nutricionales \n"; // Crear o eliminar
            cout << "3. Consumir objetos nutricionales \n";
            cout << "4. Mostrar calorias y macros restantes \n";
            cout << "5. Reiniciar marcador diario (nuevo dia) \n";
            cout << "6. Salir \n";
            cout << "Ingrese una opcion: ";
            getline(cin, opcion);

            if (opcion.empty()){
                clearTerminal();
                printHeader(VERSION);
                continue;
            }

            try {
                index = stoi (opcion);
            } catch (...) {
                index = MAX_INTEGER; //Son las 4 de la mañana, no sé si esto es una solución elegante o una chapuzada. Queda así.
            }

            clearTerminal();
            switch (index) {
                case 0:
                    printHeader(VERSION);
                    printProjectInfo(VERSION);
                    break;
                case 1:
                    printHeader(VERSION);
                    editUserData(user, userDataFile);
                    break;
                case 2:
                    printHeader(VERSION);
                    manageObject(objDataFile);
                    break;
                case 3:
                    printHeader(VERSION);
                    consumeObjects(objDataFile, userDataFile, user);
                    break;
                case 4:
                    printHeader(VERSION);
                    printRemaining(user.remainingCal, user.remainingProtein, user.remainingFat, user.remainingCarb);
                    break;
                case 5:
                    printHeader(VERSION);

                    char eleccion;
                    cout << "Esta seguro de que quiere reiniciar el marcador diario? (Y/N) \n";
                    cin >> eleccion;
                    cin.ignore(MAX_INTEGER, '\n');

                    clearTerminal();
                    printHeader(VERSION);

                    if(eleccion == 'Y' || eleccion == 'y'){
                        resetDailyMarker(userDataFile, user);
                        break;
                    }
                    break;
                case 6:
                    printHeader(VERSION);
                    cout << "Saliendo del programa. Hasta luego!";
                    this_thread::sleep_for(chrono::milliseconds(1500)); // Un segundo y medio
                    break;
                default:
                    printHeader(VERSION);
                    cout << "Opcion invalida, intente de nuevo. \n\n";
            }
        } while(index != 6);
    }
    return 0;
}
