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
    int buscarMozo (Mozo mozos [], int len, const char nombre[]){
        int i = 0;
        while (i<len && strcmp (mozos[i].nombre , nombre) != 0) {
            i++;
        }
        if (i==len){
            return -1;
        } else {
            return i;
        }
    }

    int agregarMozo(Mozo mozos [],int& len, const char nombre[]){
        if(len == MAX_MOZOS) {
            return -1;
        
    }

    Mozo nuevo;
    nuevo.idMozo = len+1;
    strcpy(nuevo.nombre, nombre);
    char claveInicial[20];
    numeroATexto(nuevo.idMozo, claveInicial);
    encriptar(claveInicial, nuevo.password);
    nuevo.totalComision = 0;
    mozos[len]=nuevo;
    len++;
    return len - 1;
    }

    void grabarMozos (Mozo mozos[],int len){
        FILE* f = fopen("mozos.dat", "wb");
        if ( f==NULL) {
            cout << "Error: no se pudo crear mozos.dat" <<endl;
            return;
        }
        for (int i = 0; i < len; i++) {
            fwrite (&mozos[i], sizeof (Mozo), 1 , f);
        }
        fclose(f);
    }

