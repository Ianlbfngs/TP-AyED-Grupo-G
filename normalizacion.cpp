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

    long buscarProducto(FILE* f, int codigo, Producto& p) {
        fseek(f, 0, SEEK_END);
        long n = ftell(f) / sizeof (Producto);
        long pri = 0;
        long ult = n - 1;
        long pos = -1;
        while (pri <= ult && pos == -1){
            long med = (pri + ult) / 2;
            fseek(f , med * sizeof (Producto), SEEK_SET);
            if (fread(&p, sizeof(Producto), 1 , f)!=1){
                return -1;
            }
            if (p.codigo == codigo){
                pos = med;

            } else if (codigo > p.codigo) {
                pri=med+1;
            }else {
                ult=med - 1;
            }
        }
        return pos;
    }

   void descontarStock(FILE* f, int codigo, int cantidad) {
    Producto p;
    long pos = buscarProducto(f, codigo, p);
    if (pos == -1) {
        cout << "Aviso: el producto " << codigo << " no existe en el inventario" << endl;
        return;
    }
    if (p.stockActual < cantidad) {
        cout << "Aviso: sin stock suficiente de " << p.descripcion
             << " (habia " << p.stockActual << ", se vendieron " << cantidad << ")" << endl;
        p.stockActual = 0;
    } else {
        p.stockActual = p.stockActual - cantidad;
    }
    fseek(f, pos * sizeof(Producto), SEEK_SET);
    fwrite(&p, sizeof(Producto), 1, f);
}
bool vaAntes(ComandaDia a, ComandaDia b) {
    int cmp = strcmp(a.fecha, b.fecha);
    if (cmp != 0) {
        return cmp < 0;
    }
    return a.comanda.idMozo < b.comanda.idMozo;
}
 
void ordenarInsercion(ComandaDia v[], int len) {
    for (int i = 1; i < len; i++) {
        ComandaDia clave = v[i];
        int j = i - 1;
        while (j >= 0 && vaAntes(clave, v[j])) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = clave;
    }
}
void copiarAlFinal(char destino[], int& pos, const char origen[]) {
    int j = 0;
    while (origen[j] != '\0') {
        destino[pos] = origen[j];
        pos++;
        j++;
    }
}
 
void armarNombreArchivo(const char fecha[], char nombre[]) {
    int pos = 0;
    copiarAlFinal(nombre, pos, "comandas_");
    copiarAlFinal(nombre, pos, fecha);
    copiarAlFinal(nombre, pos, ".dat");
    nombre[pos] = '\0';
}
 
int grabarPlanillas(ComandaDia v[], int len) {
    int cantPlanillas = 0;
    int i = 0;
    while (i < len) {
        char fechaActual[11];
        strcpy(fechaActual, v[i].fecha);
        char nombreArchivo[40];
        armarNombreArchivo(fechaActual, nombreArchivo);
        FILE* f = fopen(nombreArchivo, "wb");
        if (f == NULL) {
            cout << "Error: no se pudo crear " << nombreArchivo << endl;
            return cantPlanillas;
        }
        while (i < len && strcmp(v[i].fecha, fechaActual) == 0) {
            fwrite(&v[i].comanda, sizeof(Comanda), 1, f);
            i++;
        }
        fclose(f);
        cantPlanillas++;
    }
    return cantPlanillas;
}
 
int main() {
    FILE* fHist = fopen("comandas_historicas.dat", "rb");
    if (fHist == NULL) {
        cout << "Error: no se pudo abrir comandas_historicas.dat" << endl;
        return 1;
    }
    FILE* fInv = fopen("inventario.dat", "rb+");
    if (fInv == NULL) {
        cout << "Error: no se pudo abrir inventario.dat" << endl;
        fclose(fHist);
        return 1;
    }
 
    Mozo mozos[MAX_MOZOS];
    int lenMozos = 0;
    ComandaDia comandas[MAX_COMANDAS];
    int lenComandas = 0;
    ComandaHistorica h;
    bool error = false;
 
    while (!error && fread(&h, sizeof(ComandaHistorica), 1, fHist) == 1) {
        int pos = buscarMozo(mozos, lenMozos, h.nombreMozo);
        if (pos == -1) {
            pos = agregarMozo(mozos, lenMozos, h.nombreMozo);
        }
        if (pos == -1 || lenComandas == MAX_COMANDAS) {
            cout << "Error: se supero la capacidad de los vectores" << endl;
            error = true;
        } else {
            mozos[pos].totalComision = mozos[pos].totalComision + h.comision;
 
            strcpy(comandas[lenComandas].fecha, h.fecha);
            comandas[lenComandas].comanda.idMozo = mozos[pos].idMozo;
            comandas[lenComandas].comanda.codigoProducto = h.codigoProducto;
            comandas[lenComandas].comanda.cantidad = h.cantidad;
            comandas[lenComandas].comanda.comision = h.comision;
            lenComandas++;
 
            descontarStock(fInv, h.codigoProducto, h.cantidad);
        }
    }
    fclose(fHist);
    fclose(fInv);
 
    grabarMozos(mozos, lenMozos);
    ordenarInsercion(comandas, lenComandas);
    int cantPlanillas = grabarPlanillas(comandas, lenComandas);
 
    cout << "Normalizacion terminada: " << lenComandas << " ventas, "
         << lenMozos << " mozos, " << cantPlanillas << " planillas diarias." << endl;
    return 0;
} 