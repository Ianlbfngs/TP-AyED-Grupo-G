#include <iostream>
using namespace std;

struct Producto {
	int codigo;
	char descripcion[50];
	float precio;
	int stockActual;
};

struct Mozo {
	int idMozo;
	char nombre[50];
	char password[20];
	float totalComision;
};
struct Comanda {
	int idMozo;
	int CodigoProducto;
	int cantidad;
	float comision;
};

const float TASA_COMISION = 0.10f; //10% de lo vendido

//nombres de archivos
//mozos.dat
//comandas_dd-mm-aaaa.dat
//comandas_semana_sX_mm.dat

void cargarFecha(char fecha[]);
void crearNombreArchivoComanda(char nArchivo[]);
FILE* abrirArchivoComanda();



int main() {
	return 0;
}


FILE* abrirArchivoComanda() {
	char nombreArchivoComanda[24];
	crearNombreArchivoComanda(nombreArchivoComanda);
	FILE* fC = fopen(nombreArchivoComanda, "ab");
	if (fC == NULL) {
		cout << "Error al crear/abrir el archivo " << nombreArchivoComanda << endl;
	}
	return fC;
}

void cargarFecha(char fecha[]) {
	cout << "Ingrese la fecha (formato dd-MM-aaaa)" << endl;
	cin >> fecha;
	return;
}

void crearNombreArchivoComanda(char nArchivo[]) {
	char comandas[10] = "comandas_";
	char fecha[11];
	char extension[5] = ".dat";

	cargarFecha(fecha);
	int j = 0;
	for (int i = 0;i < 9;i++) {
		nArchivo[i] = comandas[j++];
	}
	j = 0;
	for (int i = 9;i < 19;i++) {
		nArchivo[i] = fecha[j++];
	}
	j = 0;
	for (int i = 19;i < 23;i++) {
		nArchivo[i] = extension[j++];
	}
	return;
}