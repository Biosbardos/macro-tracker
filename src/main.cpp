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
    string nombre;
    double altura, peso;
    int edad, formula;

    double maintenanceCal;
    double remainingCal;
    double targetProtein, targetFat, targetCarb;
    double remainingProtein, remainingFat, remainingCarb;

    // Si no existe el archivo de datos, es la primera ejecución.
    Usuario user;
    if (!loadUserData(userDataFile, user))
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
        calcularMacrosObjetivo(maintenanceCal, targetProtein, targetFat, targetCarb);

        // Inicialmente, los marcadores diarios son iguales a los targets
        remainingCal      = maintenanceCal;
        remainingProtein  = targetProtein;
        remainingFat      = targetFat;
        remainingCarb     = targetCarb;

        // Guardar la información del usuario
        saveUserData(userDataFile, user);

        // Terminar el programa tras la primera ejecución
        return 0;
    }
    else {
        // Ejecuciones subsiguientes: se cargan los datos y se le da la bienvenida al usuario.
        cout << "Bienvenido de nuevo, " << nombre << "!" << endl;

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
