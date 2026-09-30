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
	int codigoProducto;
	int cantidad;
	float comision;
};

const float TASA_COMISION = 0.10f; //10% de lo vendido

//nombres de archivos
const char nombreArchMozos[10] = "mozos.dat";
const char nombreArchInventario[15] = "inventario.dat";
//comandas_dd-mm-aaaa.dat
//comandas_semana_sX_mm.dat

bool verificarAperturaDeArchivo(FILE* f, string nombre);
FILE* abrirArchivoEditable(const char* nombreArch);
void cargarFecha(char fecha[]);
void crearNombreArchivoComanda(char nArchivo[]);
FILE* abrirArchivoComanda();
void cargarDia(FILE* fM, FILE*  fI);
bool guardarVenta(FILE* fV, Comanda comanda);


int main() {
	FILE* fMozos = abrirArchivoEditable(nombreArchMozos);
	FILE* fInventario = abrirArchivoEditable(nombreArchInventario);
	if (!verificarAperturaDeArchivo(fMozos,nombreArchMozos) || !verificarAperturaDeArchivo(fInventario,nombreArchInventario)) return -1; //no se puede abrir mozos o inv --> no se puede seguir
	bool cargarOtroDia;
	while (true) {
		cargarDia(fMozos, fInventario);
		cout << "Cargar las ventas de otro dia? (1 = si | 0 = no)";
		cin >> cargarOtroDia;
		if (cargarOtroDia) continue;
		else break;
	}
	fclose(fMozos);
	fclose(fInventario);
	return 0;
}

bool guardarVenta(FILE* fV, Comanda comanda) {
	if (fwrite(&comanda, sizeof(Comanda), 1, fV) == 1) {
		cout << "Venta agregada correctamente" << endl;
		return true;
	}
	else {
		cout << "No fue posible agregar la venta" << endl;
		return false;
	}
}

Comanda cargarVenta(Mozo m,Producto p, FILE* fV) {
	Comanda comandaN;
	comandaN.idMozo = m.idMozo;
	comandaN.codigoProducto = p.codigo;
	cout << "Ingrese la cantidad de " << p.descripcion << endl;
	cin >> comandaN.cantidad;
	comandaN.comision = TASA_COMISION * (comandaN.cantidad * p.precio);
	return comandaN;
}

void cargarDia(FILE* fM, FILE* fI) {
	FILE* fVentas = abrirArchivoComanda();
	Comanda comandaNueva;
	Mozo mozoAux;
	Producto productoAux;
	bool continuar;
	while (true) {
		mozoAux = conseguirMozo(fM);
		productoAux = conseguirProducto(fI);
		comandaNueva = cargarVenta(mozoAux, productoAux, fVentas);
		if (guardarVenta(fVentas, comandaNueva)) {
			actualizarStock(fI, comandaNueva);
			actualizarComisionMozo(fM, comandaNueva);
		}
		cout << "Cargar otra venta? (1 = si | 0 = no)" << endl;
		cin >> continuar;
		if (continuar) continue;
		else break;
	}
	ordenarComandas(fVentas);
	fclose(fVentas);
}

FILE* abrirArchivoComanda() {	//abrir o crear (ab)
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

bool verificarAperturaDeArchivo(FILE* f, string nombre) {
	if (f == NULL) {
		cout << "Erorr al abrir el archivo " << nombre << endl;
		return false;
	}
	else return true;
}

FILE* abrirArchivoEditable(const char* nombreArch) {
	FILE* fM = fopen(nombreArch, "rb+");
	if (fM == NULL) {
		cout << "No fue posible abrir el archivo " << nombreArch << endl;
	}
	return fM;
}