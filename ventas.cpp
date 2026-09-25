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
//mozos
//comandas_dd-mm-aaaa
//comandas_semana_sX_mm

int main() {

	return 0;
}