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
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

/* ================= Función principal ================= */
int main() {
    const string userDataFile = "datos.txt";
    const string objDataFile  = "objetos.txt";
     Usuario user;

    // Si no existe el archivo de datos, es la primera ejecución.
    if (!loadUserData(userDataFile, user))
    {
        // Ingreso de datos (primera vez)
        cout << "Ingrese su nombre (sin tildes ni valores especiales): ";
        getline(cin, user.nombre);
        cout << "Bienvenido, " << user.nombre << endl;

        user.altura = pedirNumero("Ingrese su altura en cm: ");
        cout << "Altura procesada y guardada con exito." << endl;

        user.peso = pedirNumero("Ingrese su peso en kg: ");
        cout << "Peso procesado y guardado con exito." << endl;

        user.edad = static_cast<int>(pedirNumero("Ingrese su edad en anhos: "));
        cout << "Edad procesada y guardada con exito." <<endl;

        user.formula = chooseCalorieFormula();
        cout << "Formula procesada y guardada con exito." << endl;

        // Calcular las calorías de mantenimiento
        user.maintenanceCal = calculateCalories(user.peso, user.altura, user.edad, user.formula);
        cout << "\nSu requerimiento de calorias de mantenimiento es: "
             << user.maintenanceCal << " kcal." << endl;

        // Calcular los targets de macros según porcentajes predeterminados:
        // 25% de proteína (4 cal/g), 20% de grasa (9 cal/g) y 55% de carbohidratos (4 cal/g)
        calcularMacrosObjetivo(user.maintenanceCal, user.targetProtein, user.targetFat, user.targetCarb);

        // Inicialmente, los marcadores diarios son iguales a los targets
        user.remainingCal      = user.maintenanceCal;
        user.remainingProtein  = user.targetProtein;
        user.remainingFat      = user.targetFat;
        user.remainingCarb     = user.targetCarb;

        // Guardar la información del usuario
        saveUserData(userDataFile, user);

        // Terminar el programa tras la primera ejecución
        return 0;
    }
    else {
        // Ejecuciones subsiguientes: se cargan los datos y se le da la bienvenida al usuario.
        cout << "Bienvenido de nuevo, " << user.nombre << "!" << endl;

        int opcion;
        do {
            cout << "\nMenu de opciones:" << endl;
            cout << "0. Editar datos de usuario (nombre, altura, peso, edad, formula)" << endl;
            cout << "1. Crear un objeto nutricional" << endl;
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
                    editUserData(user, userDataFile);
                    break;
                case 1:
                    addObject(objDataFile);
                    break;
                case 2:
                    deleteObject(objDataFile);
                    break;
                case 3:
                    consumeObjects(objDataFile, userDataFile, user);
                    break;
                case 4:
                    resetDailyMarker(userDataFile, user);
                    break;
                case 5:
                    mostrarRestante(user.remainingCal, user.remainingProtein, user.remainingFat, user.remainingCarb);
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
