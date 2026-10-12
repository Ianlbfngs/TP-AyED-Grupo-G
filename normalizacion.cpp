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
void encriptar (const char original[], char encriptada[])
{
    int i = 0;
    while (original[i] != '\0') {
        encriptada [i] = original [i] + K;
        i++;
    }
    encriptada [i]= '\0';

    }

    void numeroATexto(int n,char texto[]) {
        int cant = 0 , temp = n;
        while (temp > 0) {
            cant++;
            temp=temp/10;
        }
        texto [cant] = '\0';
        int i = cant - 1;
        while (n>0) {
            texto [i] = '0' + n % 10;
            n=n/10;
            i--;
