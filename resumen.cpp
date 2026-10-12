#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iomanip>
#include <iostream>
using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

void mostrarResumenMozo(int idMozo, int productos, float comision) {
    cout << "Mozo " << idMozo
              << " | productos vendidos: " << productos
              << " | comision: $" << comision << '\n';
}


int main() {
    int numeroSemana;
    int mes;

    cout << "Numero de semana (por ejemplo, 1): ";
    if (!(cin >> numeroSemana) || numeroSemana <= 0) {
        cerr << "Numero de semana invalido.\n";
        return 1;
    }

    cout << "Numero de mes (1 a 12): ";
    if (!(cin >> mes) || mes < 1 || mes > 12) {
        cerr << "Numero de mes invalido.\n";
        return 1;
    }

    char nombreArchivo[64];
    snprintf(nombreArchivo, sizeof(nombreArchivo),
                  "comandas_semana_s%d-%02d.dat", numeroSemana, mes);

    FILE* archivo = fopen(nombreArchivo, "rb");
    if (archivo == nullptr) {
        cerr << "No se pudo abrir " << nombreArchivo << ".\n";
        return 1;
    }

    cout << '\n' << "Resumen de " << nombreArchivo << '\n';
    cout << fixed << setprecision(2);

   

   
    fclose(archivo);
    return 0;
}