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

    Comanda comanda;
    bool hayComandas = false;
    int idMozoActual = 0;
    int productosMozo = 0;
    float comisionMozo = 0.0f;
    int totalProductos = 0;

    while (fread(&comanda, sizeof(Comanda), 1, archivo) == 1) {
        if (!hayComandas) {
            idMozoActual = comanda.idMozo;
            hayComandas = true;
        } else if (comanda.idMozo != idMozoActual) {
            mostrarResumenMozo(idMozoActual, productosMozo, comisionMozo);
            idMozoActual = comanda.idMozo;
            productosMozo = 0;
            comisionMozo = 0.0f;
        }

        productosMozo += comanda.cantidad;
        comisionMozo += comanda.comision;
        totalProductos += comanda.cantidad;
    }

    if (ferror(archivo)) {
        cerr << "Ocurrio un error al leer " << nombreArchivo << ".\n";
        fclose(archivo);
        return 1;
    }

    if (hayComandas) {
        mostrarResumenMozo(idMozoActual, productosMozo, comisionMozo);
    } else {
        cout << "La planilla semanal no contiene ventas.\n";
    }

    cout << "Total de productos vendidos por el buffet: "
              << totalProductos << '\n';

    fclose(archivo);
    return 0;
}