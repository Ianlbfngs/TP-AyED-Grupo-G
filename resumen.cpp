#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstdio>

using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

void mostrarResumenMozo(int idMozo, int productos, float comision);
bool crearNombreArchivoComanda(char nArchivo[]);
int resumen(const char nArchivo[]);

int main() {
    char nombreArchivo[26];
    if(!crearNombreArchivoComanda(nombreArchivo)) return 1;
    return resumen(nombreArchivo);
}

void mostrarResumenMozo(int idMozo, int productos, float comision) {
    cout << "Mozo " << idMozo
              << " | productos vendidos: " << productos
              << " | comision: $" << comision << "\n";
}

bool crearNombreArchivoComanda(char nArchivo[]) {
    //objetivo --> comandas_semana_sX-mm.dat (25 chars+'\0')
    //partes fijas
    char base[] = "comandas_semana_s"; 
    char extension[] = ".dat";
    //partes que varian
    int numeroSemana;
    int mes;

    cout << "Ingrese el numero de semana (de 1 a 4): ";
    cin>>numeroSemana;
    if (numeroSemana > 4 || numeroSemana <= 0) {
        cerr << "Numero de semana invalido.\n";
        return false;
    }
    cout << "Ingrese el numero de mes (1 a 12): ";
    cin >> mes;
    if (mes < 1 || mes > 12) {
        cerr << "Numero de mes invalido.\n";
        return false;
    }

    //copia del base[] a --> nArchivo[]
    for(int i = 0; base[i] != '\0'; ++i) {
        nArchivo[i] = base[i];
    }
    //carga de los caracs que varian
    nArchivo[17] = numeroSemana+'0';
    nArchivo[18] = '-';
    nArchivo[19] = (mes / 10) + '0';
    nArchivo[20] = (mes % 10) + '0';
    //copia de extension[] a --> nArchivo
    for(int i = 0; extension[i] != '\0'; ++i) {
        nArchivo[21 + i] = extension[i];
     }
	//ultimo char
	nArchivo[25] = '\0'; 
    return true;
}

int resumen(const char nArchivo[]){
    FILE* archivo = fopen(nArchivo, "rb");
    if (archivo == NULL) {
        cerr << "No se pudo abrir " << nArchivo << ".\n";
        return 1;
    }

    cout << '\n' << "Resumen de " << nArchivo << '\n';
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
        cerr << "Ocurrio un error al leer " << nArchivo << ".\n";
        fclose(archivo);
        return 1;
    }

    if (hayComandas) {
        mostrarResumenMozo(idMozoActual, productosMozo, comisionMozo);
    } else {
        cout << "La planilla semanal no contiene ventas.\n";
    }

    cout << "Total de productos vendidos por el buffet: "<< totalProductos << '\n';

    fclose(archivo);
    return 0;
}