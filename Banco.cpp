#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
#include <conio.h> // Para usar _getch()
#include <windows.h> // Para manipular el color de la consola
#include <fstream> // Para manejar archivos
#include <ctime> // Para manejar la fecha y hora
#include <iomanip> // Para manejar la precisión de los números

// Declaraciones de funciones
void gotoxy(int x, int y);
void dibujarMarco();
void setColor(int color);

struct Usuario {
    std::string cedula;
    std::string telefono;
    int edad;
    std::string cuentaBancaria;
};
struct Transferencia {
    std::string cuentaOrigen;
    std::string cuentaDestino;
    double monto;
};

struct Prestamo {
    std::string cuenta;
    double monto;
};

class Banco {
private:
    std::string nombreBanco = "Banco Hamza";
    std::unordered_map<std::string, Usuario> usuarios;
    std::vector<Transferencia> transferencias;
    std::vector<Prestamo> prestamos;
    std::vector<std::string> beneficios;

    std::string generarCuentaBancaria(const std::string& cedula, const std::string& nombre) {
        std::string cuenta = cedula.substr(0, 4) + nombre.substr(0, 2);
        return cuenta;
    }

    bool esNumero(const std::string& str) {
        return std::regex_match(str, std::regex("\\d+"));
    }

    bool esTexto(const std::string& str) {
        return std::regex_match(str, std::regex("[a-zA-Z]+"));
    }

    bool validarCedulaEcuatoriana(const std::string& cedula) {
        if (cedula.length() != 10 || !esNumero(cedula)) {
            return false;
        }
        int provincia = std::stoi(cedula.substr(0, 2));
        if (provincia < 1 || provincia > 24) {
            return false;
        }
        int tercerDigito = std::stoi(cedula.substr(2, 1));
        if (tercerDigito >= 6) {
            return false;
        }
        int suma = 0;
        for (int i = 0; i < 9; i++) {
            int digito = std::stoi(cedula.substr(i, 1));
            if (i % 2 == 0) {
                digito *= 2;
                if (digito > 9) {
                    digito -= 9;
                }
            }
            suma += digito;
        }
        int digitoVerificador = std::stoi(cedula.substr(9, 1));
        int decenaSuperior = (suma + 9) / 10 * 10;
        return (decenaSuperior - suma) == digitoVerificador;
    }

    void setColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void mostrarCargando() {
        setColor(11); // Establecer color celeste claro
        std::cout << "Cargando sistemas bancarios..." << std::endl;
        setColor(7); // Restaurar color predeterminado
        Sleep(3000); // Esperar 3 segundos
    }

    void guardarUsuarioEnArchivo(const Usuario& usuario) {
        std::ofstream archivo("usuarios.txt", std::ios::app);
        if (archivo.is_open()) {
            archivo << "Cédula: " << usuario.cedula << "\n";
            archivo << "Teléfono: " << usuario.telefono << "\n";
            archivo << "Edad: " << usuario.edad << "\n";
            archivo << "Cuenta Bancaria: " << usuario.cuentaBancaria << "\n";
            archivo << "--------------------------\n";
            archivo.close();
        } else {
            std::cerr << "No se pudo abrir el archivo para guardar el usuario." << std::endl;
        }
    }

    void mostrarDatosUsuario(const Usuario& usuario) {
        setColor(11); // Establecer color celeste claro
        std::cout << "Datos del usuario en " << nombreBanco << ":" << std::endl;
        std::cout << "Cédula: " << usuario.cedula << std::endl;
        std::cout << "Teléfono: " << usuario.telefono << std::endl;
        std::cout << "Edad: " << usuario.edad << std::endl;
        std::cout << "Cuenta Bancaria: " << usuario.cuentaBancaria << std::endl;
        setColor(7); // Restaurar color predeterminado
    }

    void mostrarFechaHora() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        setColor(11); // Establecer color celeste claro
        std::cout << "Fecha: " << 1900 + ltm->tm_year << "-"
                  << 1 + ltm->tm_mon << "-"
                  << ltm->tm_mday << " "
                  << "Hora: " << ltm->tm_hour << ":"
                  << ltm->tm_min << ":"
                  << ltm->tm_sec << std::endl;
        setColor(7); // Restaurar color predeterminado
    }

    bool esNumeroValido(const std::string& str) {
        return std::regex_match(str, std::regex("^[0-9]+$"));
    }

    bool esTextoValido(const std::string& str) {
        return std::regex_match(str, std::regex("^[a-zA-Z]+$"));
    }

public:
    Banco() {
        // Inicializar beneficios
        beneficios.push_back("Tasas de interés bajas en préstamos.");
        beneficios.push_back("Descuentos en comercios afiliados.");
        beneficios.push_back("Programas de recompensas.");
        beneficios.push_back("Asesoría financiera gratuita.");
    }

    void registrarUsuario(const std::string& cedula, const std::string& telefono, int edad, const std::string& nombre) {
        if (edad < 18) {
            setColor(11); // Establecer color celeste claro
            std::cout << "El usuario debe ser mayor o igual a 18 años." << std::endl;
            setColor(7); // Restaurar color predeterminado
            return;
        }
        if (!validarCedulaEcuatoriana(cedula)) {
            setColor(11); // Establecer color celeste claro
            std::cout << "La cédula ingresada pertenece a otra nacionalidad. Por favor, ingrese una cédula de nacionalidad ecuatoriana." << std::endl;
            setColor(7); // Restaurar color predeterminado
            return;
        }
        if (telefono.length() != 10 || !std::regex_match(telefono, std::regex("^[09]\\d{9}$"))) {
            setColor(11); // Establecer color celeste claro
            std::cout << "El teléfono debe tener exactamente 10 dígitos y comenzar con 0 o 9." << std::endl;
            setColor(7); // Restaurar color predeterminado
            return;
        }
        if (!esTextoValido(nombre)) {
            setColor(11); // Establecer color celeste claro
            std::cout << "El nombre debe contener solo letras." << std::endl;
            setColor(7); // Restaurar color predeterminado
            return;
        }
        if (usuarios.find(cedula) != usuarios.end()) {
            setColor(11); // Establecer color celeste claro
            std::cout << "El usuario ya está registrado. Sus datos son:" << std::endl;
            mostrarDatosUsuario(usuarios[cedula]);
            setColor(7); // Restaurar color predeterminado
            return;
        }
        std::string cuentaBancaria = generarCuentaBancaria(cedula, nombre);
        Usuario nuevoUsuario = {cedula, telefono, edad, cuentaBancaria};
        usuarios[cedula] = nuevoUsuario;
        guardarUsuarioEnArchivo(nuevoUsuario);
        setColor(11); // Establecer color celeste claro
        std::cout << "Usuario registrado exitosamente. Su cuenta bancaria es: " << cuentaBancaria << std::endl;
        std::cout << "Bienvenido al Banco. Gracias por elegirnos. Ofrecemos muchos beneficios y descuentos." << std::endl;
        setColor(7); // Restaurar color predeterminado
    }

    bool estaRegistrado(const std::string& cedula) {
        return usuarios.find(cedula) != usuarios.end();
    }

    bool esAptoParaCredito(const std::string& cedula) {
        if (!estaRegistrado(cedula)) {
            setColor(11); // Establecer color celeste claro
            std::cout << "El usuario no está registrado." << std::endl;
            setColor(7); // Restaurar color predeterminado
            return false;
        }
        return usuarios[cedula].edad >= 18;
    }

    void realizarTransferencia(const std::string& cedula) {
        if (!estaRegistrado(cedula)) {
            setColor(11); // Establecer color celeste claro
            std::cout << "El usuario no está registrado." << std::endl;
            setColor(7); // Restaurar color predeterminado
            return;
        }
        std::string cuentaDestino;
        double monto;
        setColor(11); // Establecer color celeste claro
        std::cout << "Ingrese la cuenta bancaria del destinatario: ";
        setColor(7); // Restaurar color predeterminado
        std::cin >> cuentaDestino;
        bool cuentaValida = false;
        for (const auto& usuario : usuarios) {
            if (usuario.second.cuentaBancaria == cuentaDestino) {
                cuentaValida = true;
                break;
            }
        }
        if (!cuentaValida) {
            setColor(11); // Establecer color celeste claro
            std::cout << "La cuenta bancaria del destinatario no es válida." << std::endl;
            setColor(7); // Restaurar color predeterminado
            return;
        }
        setColor(11); // Establecer color celeste claro
        std::cout << "Ingrese el monto a transferir: ";
        setColor(7); // Restaurar color predeterminado
        std::cin >> monto;
        Transferencia nuevaTransferencia = {usuarios[cedula].cuentaBancaria, cuentaDestino, monto};
        transferencias.push_back(nuevaTransferencia);
        setColor(11); // Establecer color celeste claro
        std::cout << "Transferencia de " << monto << " realizada exitosamente a la cuenta " << cuentaDestino << "." << std::endl;
        setColor(7); // Restaurar color predeterminado
    }

    void solicitarPrestamo(const std::string& cedula) {
        if (!estaRegistrado(cedula)) {
            setColor(11); // Establecer color celeste claro
            std::cout << "El usuario no está registrado." << std::endl;
            setColor(7); // Restaurar color predeterminado
            return;
        }
        std::string montoStr;
        double monto;
        setColor(11); // Establecer color celeste claro
        std::cout << "Ingrese el monto del préstamo: ";
        setColor(7); // Restaurar color predeterminado
        std::cin >> montoStr;
        if (!esNumeroValido(montoStr)) {
            setColor(11); // Establecer color celeste claro
            std::cout << "El monto debe ser un número válido." << std::endl;
            setColor(7); // Restaurar color predeterminado
            return;
        }
        monto = std::stod(montoStr);
        Prestamo nuevoPrestamo = {usuarios[cedula].cuentaBancaria, monto};
        prestamos.push_back(nuevoPrestamo);
        setColor(11); // Establecer color celeste claro
        std::cout << "Préstamo de " << monto << " solicitado exitosamente." << std::endl;
        setColor(7); // Restaurar color predeterminado
    }

    void mostrarBeneficios() {
        setColor(11); // Establecer color celeste claro
        std::cout << "Beneficios del " << nombreBanco << ":" << std::endl;
        for (const auto& beneficio : beneficios) {
            std::cout << beneficio << std::endl;
        }
        setColor(7); // Restaurar color predeterminado
    }

    void mostrarMenu(const std::string& cedula) {
        if (!estaRegistrado(cedula)) {
            setColor(11); // Establecer color celeste claro
            std::cout << "El usuario no está registrado." << std::endl;
            setColor(7); // Restaurar color predeterminado
            return;
        }
        int opcion = 1;
        char tecla;
        do {
            dibujarMarco();
            mostrarFechaHora();
            gotoxy(30, 5); std::cout << "+==============+";
            gotoxy(30, 6); std::cout << "| MENU DE USUARIO |";
            gotoxy(30, 7); std::cout << "+==============+";
            
            gotoxy(25, 9);  std::cout << (opcion == 1 ? "-> " : "   ") << "1. Préstamos";
            gotoxy(25, 10); std::cout << (opcion == 2 ? "-> " : "   ") << "2. Transferencias";
            gotoxy(25, 11); std::cout << (opcion == 3 ? "-> " : "   ") << "3. Consultar saldo";
            gotoxy(25, 12); std::cout << (opcion == 4 ? "-> " : "   ") << "4. Ver beneficios";
            gotoxy(25, 13); std::cout << (opcion == 5 ? "-> " : "   ") << "5. Salir";
            
            tecla = _getch();
            if (tecla == 72 && opcion > 1) opcion--; // Flecha arriba
            if (tecla == 80 && opcion < 5) opcion++; // Flecha abajo

            if (tecla == 13) { // Enter
                mostrarCargando();
                switch (opcion) {
                    case 1:
                        solicitarPrestamo(cedula);
                        break;
                    case 2:
                        realizarTransferencia(cedula);
                        break;
                    case 3:
                        setColor(11); // Establecer color celeste claro
                        std::cout << "Saldo actual: 1000.00" << std::endl; // Ejemplo de saldo
                        setColor(7); // Restaurar color predeterminado
                        break;
                    case 4:
                        mostrarBeneficios();
                        break;
                    case 5:
                        setColor(11); // Establecer color celeste claro
                        std::cout << "Saliendo del menú de usuario..." << std::endl;
                        setColor(7); // Restaurar color predeterminado
                        break;
                }
                if (opcion != 5) {
                    setColor(11); // Establecer color celeste claro
                    std::cout << "Presione cualquier tecla para continuar..." << std::endl;
                    setColor(7); // Restaurar color predeterminado
                    _getch();
                }
            }
        } while (tecla != 13 || opcion != 5);
    }

    void mostrarMenuInicio() {
        int opcion = 1;
        char tecla;
        std::string cedula, telefono, nombre;
        std::string edadStr;
        int edad;

        do {
            dibujarMarco();
            gotoxy(30, 5); std::cout << "+==============+";
            gotoxy(30, 6); std::cout << "| MENU PRINCIPAL |";
            gotoxy(30, 7); std::cout << "+==============+";
            
            gotoxy(25, 9);  std::cout << (opcion == 1 ? "-> " : "   ") << "1. Registrar usuario";
            gotoxy(25, 10); std::cout << (opcion == 2 ? "-> " : "   ") << "2. Acceder al menú de usuario";
            gotoxy(25, 11); std::cout << (opcion == 3 ? "-> " : "   ") << "3. Salir del programa";
            
            tecla = _getch();
            if (tecla == 72 && opcion > 1) opcion--; // Flecha arriba
            if (tecla == 80 && opcion < 3) opcion++; // Flecha abajo

            if (tecla == 13) { // Enter
                mostrarCargando();
                switch (opcion) {
                    case 1:
                        setColor(11); // Establecer color celeste claro
                        std::cout << "Ingrese su número de cédula completa: ";
                        setColor(7); // Restaurar color predeterminado
                        std::cin >> cedula;
                        setColor(11); // Establecer color celeste claro
                        std::cout << "Ingrese su número de teléfono: ";
                        setColor(7); // Restaurar color predeterminado
                        std::cin >> telefono;
                        setColor(11); // Establecer color celeste claro
                        std::cout << "Ingrese su edad: ";
                        setColor(7); // Restaurar color predeterminado
                        std::cin >> edadStr;
                        if (!esNumeroValido(edadStr)) {
                            setColor(11); // Establecer color celeste claro
                            std::cout << "La edad debe ser un número válido." << std::endl;
                            setColor(7); // Restaurar color predeterminado
                            break;
                        }
                        edad = std::stoi(edadStr);
                        setColor(11); // Establecer color celeste claro
                        std::cout << "Ingrese su nombre: ";
                        setColor(7); // Restaurar color predeterminado
                        std::cin >> nombre;
                        registrarUsuario(cedula, telefono, edad, nombre);
                        break;
                    case 2:
                        setColor(11); // Establecer color celeste claro
                        std::cout << "Ingrese su cédula para acceder al menú: ";
                        setColor(7); // Restaurar color predeterminado
                        std::cin >> cedula;
                        mostrarMenu(cedula);
                        if (esAptoParaCredito(cedula)) {
                            setColor(11); // Establecer color celeste claro
                            std::cout << "El usuario es apto para crédito." << std::endl;
                            setColor(7); // Restaurar color predeterminado
                        } else {
                            setColor(11); // Establecer color celeste claro
                            std::cout << "El usuario no es apto para crédito." << std::endl;
                            setColor(7); // Restaurar color predeterminado
                        }
                        break;
                    case 3:
                        setColor(11); // Establecer color celeste claro
                        std::cout << "Saliendo del programa..." << std::endl;
                        setColor(7); // Restaurar color predeterminado
                        break;
                }
                if (opcion != 3) {
                    setColor(11); // Establecer color celeste claro
                    std::cout << "Presione cualquier tecla para continuar..." << std::endl;
                    setColor(7); // Restaurar color predeterminado
                    _getch();
                }
            }
        } while (tecla != 13 || opcion != 3);
    }
};

// Constantes para el tamaño de la pantalla
const int ANCHO = 80;
const int ALTO = 25;

// Función para posicionar el cursor
void gotoxy(int x, int y) {
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Función para dibujar el marco
void dibujarMarco() {
    system("cls");
    
    // Líneas horizontales (usando '=' para mejorar la estética)
    for(int i = 0; i < ANCHO; i++) {
        gotoxy(i, 0); std::cout << "=";
        gotoxy(i, 3); std::cout << "=";
        gotoxy(i, ALTO-1); std::cout << "=";
    }
    
    // Líneas verticales (usando '|' para mejorar la estética)
    for(int i = 1; i < ALTO-1; i++) {
        gotoxy(0, i); std::cout << "|";
        gotoxy(ANCHO-1, i); 
        setColor(13); // Establecer color morado eléctrico
        std::cout << "|";
        setColor(7); // Restaurar color predeterminado
    }
    
    // Esquinas (usando '+' para mejorar la estética)
    gotoxy(0, 0); std::cout << "+";
    gotoxy(0, ALTO-1); std::cout << "+";
    gotoxy(ANCHO-1, 0); 
    setColor(13); // Establecer color morado eléctrico
    std::cout << "+";
    setColor(7); // Restaurar color predeterminado
    gotoxy(ANCHO-1, ALTO-1); 
    setColor(13); // Establecer color morado eléctrico
    std::cout << "+";
    setColor(7); // Restaurar color predeterminado
    
    // Header
    gotoxy(2, 1); std::cout << "+==== Banco Hamza ====+";
    gotoxy(2, 2); std::cout << "| Sistema v1.0        |";
    
    // Footer
    gotoxy(2, ALTO-2); std::cout << "Christian Bautista y Jair Chicaiza";
    gotoxy(ANCHO-30, ALTO-2); std::cout << "Creado para Banco Hamza";
    
    // Fecha y hora
    time_t now = time(0);
    tm* time = localtime(&now);
    gotoxy(ANCHO-22, 1);
    std::cout << std::setw(2) << std::setfill('0') << time->tm_mday << "/"
              << std::setw(2) << std::setfill('0') << (time->tm_mon + 1) << "/"
              << (1900 + time->tm_year);
    gotoxy(ANCHO-22, 2);
    std::cout << std::setw(2) << std::setfill('0') << time->tm_hour << ":"
              << std::setw(2) << std::setfill('0') << time->tm_min << ":"
              << std::setw(2) << std::setfill('0') << time->tm_sec;
}

int main() {
    Banco banco;
    while (true) {
        banco.mostrarMenuInicio();
        Sleep(1000); // Esperar 1 segundo
    }
    return 0;
}
