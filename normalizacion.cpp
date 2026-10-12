#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct ComandaHistorica {
    char fecha[11] , nombreMozo [50];
    int codigoProducto , cantidad;
    float comision;
};

struct Producto {
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};

struct Mozo {
    int idMozo;
    char nombre[50], password[20];
    float totalComision;
};

struct Comanda {
    int idMozo, codigoProducto, cantidad;
    float comision;
};

struct ComandaDia {
    char fecha[11];
    Comanda comanda;
};
const int K=5;
const int MAX_MOZOS = 100;
const int MAX_COMANDAS = 5000;
