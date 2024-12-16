#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <random>
#include <functional>

using namespace std;

void hora() {
    time_t time1;
    time(&time1);
    cout << ctime(&time1);
}

string clases[8] = {
    "Historia", "Quimica", "Fisica", "Ingles", "Matematicas", "Artes", "Atletismo", "Etica"
};

string genSalt(int length = 16) {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

    string salt;
    for (int i = 0; i < length; ++i) {
        salt += alphanum[dis(gen)];
    }
    return salt;
}

string SaltHashPassword(const string& password, const string& salt) {
    string saltedPassword = password + salt;
    return to_string(hash<string>{}(saltedPassword));
}

bool RegistrarUsuario(const string& username, const string& password, const string& filePath) {
    string salt = genSalt();
    string hashedPassword = SaltHashPassword(password, salt);

    ofstream outFile(filePath, ios::app);
    if (!outFile) {
        cout << "No se pudo abrir el archivo. Verifique la ruta y los permisos." << endl;
        return false;
    }

    outFile << username << " " << hashedPassword << " " << salt << endl;
    outFile.close();

    if (outFile.fail()) {
        cout << "Error al escribir en el archivo." << endl;
        return false;
    }

    return true;
}

bool loginUsuario(const string& username, const string& password, const string& filePath) {
    ifstream inFile(filePath);
    string storedUsername, storedHashedPassword, storedSalt;

    while (inFile >> storedUsername >> storedHashedPassword >> storedSalt) {
        if (storedUsername == username) {
            string hashedPassword = SaltHashPassword(password, storedSalt);
            if (hashedPassword == storedHashedPassword) {
                return true;
            }
        }
    }
    return false;
}

void CambiarNombreDeClase() {
    int index;
    string newName;

    cout << "Ingrese el numero de la clase que desea cambiar (1-8): ";
    cin >> index;
    if (index < 1 || index > 8) {
        cout << "Numero de clase invalido!" << endl;
        return;
    }

    cin.ignore();
    cout << "Ingrese el nuevo nombre para la clase " << clases[index - 1] << ": ";
    getline(cin, newName);

    clases[index - 1] = newName;
    cout << "Nombre de clase actualizado correctamente." << endl;

    ofstream outFile("classes.txt");
    if (!outFile) {
        cout << "Error al abrir el archivo para guardar los cambios." << endl;
        return;
    }
    for (const auto& clase : clases) {
        outFile << clase << endl;
    }
    outFile.close();
}

void Asistencia() {
    int ClasesTotal = 8;
    int ClasesAusente;

    cout << "He faltado a : \n";
    cin >> ClasesAusente;
    if (ClasesAusente > ClasesTotal) {
        cout << "Cantidad de clases con ausencia mayor a clases en total \n";
    }

    int Total = ClasesTotal - ClasesAusente;
    cout << "Ha asistido a un total de: " << Total << " clases de 8 clases\n";

    double porciento = (static_cast<double>(Total) / ClasesTotal) * 100;
    cout << "En promedio, asiste a un total de: " << porciento << "%";
}

void MostrarClases() {
    for (int i = 8 - 1; i >= 0; i--) {
        cout << clases[i] << "\n";
    }
}

void Calificaciones() {
    double NotaMatematicas, NotaQuimica, NotaFisica, NotaIngles, NotaHistoria, NotaArtes, NotaEtica, NotaAtletismo;
    cout << "Ingrese sus calificaciones (ultimo periodo)" << endl;
    cout << "Matematicas: "; cin >> NotaMatematicas;
    cout << "Quimica: "; cin >> NotaQuimica;
    cout << "Fisica: "; cin >> NotaFisica;
    cout << "Ingles: "; cin >> NotaIngles;
    cout << "Historia: "; cin >> NotaHistoria;
    cout << "Artes: "; cin >> NotaArtes;
    cout << "Etica: "; cin >> NotaEtica;
    cout << "Atletismo: "; cin >> NotaAtletismo;

    double NotaPromedio = (NotaMatematicas + NotaArtes + NotaAtletismo + NotaQuimica + NotaHistoria + NotaIngles + NotaEtica + NotaFisica) / 8;

    cout << "Nota promedio: " << NotaPromedio;
    cout << "\n";

    if (NotaPromedio > 100 || NotaPromedio < 0) {
        cout << "Promedio imposible. Por favor ingresar calificaciones nuevamente\n";
    }
}

void HoraHastaSalida() {
    time_t now;
    struct tm Hora_Salida = {};

    time(&now);
    struct tm *Hora_Actual = localtime(&now);

    Hora_Salida = *Hora_Actual;
    Hora_Salida.tm_hour = 15;
    Hora_Salida.tm_min = 0;
    Hora_Salida.tm_sec = 0;

    time_t Hora_Salida1 = mktime(&Hora_Salida);

    if (difftime(Hora_Salida1, now) < 0) {
        Hora_Salida.tm_mday += 1;
        Hora_Salida1 = mktime(&Hora_Salida);
    }

    double seconds_left = difftime(Hora_Salida1, now);
    int hours_left = seconds_left / 3600;
    int minutes_left = (seconds_left - hours_left * 3600) / 60;
    int seconds_left_int = seconds_left - (hours_left * 3600 + minutes_left * 60);

    cout << "Tiempo restante hasta las 3PM: "
         << hours_left << " horas, "
         << minutes_left << " minutos, "
         << seconds_left_int << " segundos.\n";
}

bool SwitchReset() {
    char choice;
    cout << "\nDesea volver al menu principal? (Y/N): ";
    cin >> choice;
    return (choice == 'Y' || choice == 'y');
}

int main() {
    int opcion;
    bool opc = false;
    string username, password;
    string filePath;
    bool loggedIn = false;

    cout << "Fecha y hora actual: ";
    hora();

    cout << "Por favor ingrese la ruta del archivo para guardar sus credenciales: ";
    cin >> filePath;

    while (!loggedIn) {
        cout << "Seleccione una opcion:\n";
        cout << "1) Registrarse\n";
        cout << "2) Iniciar Sesion\n";
        cin >> opcion;

        if (opcion == 1) {
            cout << "Ingrese un nombre de usuario: ";
            cin >> username;
            cout << "Ingrese una contrasena: ";
            cin >> password;

            if (RegistrarUsuario(username, password, filePath)) {
                cout << "Usuario registrado exitosamente.\n";
            } else {
                cout << "Error al registrar el usuario.\n";
            }
        } else if (opcion == 2) {
            cout << "Ingrese su nombre de usuario: ";
            cin >> username;
            cout << "Ingrese su contrasena: ";
            cin >> password;

            if (loginUsuario(username, password, filePath)) {
                cout << "Inicio de sesion exitoso.\n";
                loggedIn = true;
            } else {
                cout << "Credenciales incorrectas, intente nuevamente.\n";
            }
        } else {
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    }

    do {
        cout << "Elija una opcion:" << endl;
        cout << "1) Medir asistencia" << endl;
        cout << "2) Mostrar clases" << endl;
        cout << "3) Promedio" << endl;
        cout << "4) Hora hasta salida" << endl;
        cout << "5) Cambiar nombre de clase" << endl;
        cin >> opcion;

        switch (opcion) {
            case 1: Asistencia(); break;
            case 2: MostrarClases(); break;
            case 3: Calificaciones(); break;
            case 4: HoraHastaSalida(); break;
            case 5: CambiarNombreDeClase(); break;
            default: cout << "Opcion no valida" << endl; break;
        }

        opc = SwitchReset();

    } while (opc);

    return 0;
}
