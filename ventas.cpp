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
//constante de corrimiento de contraseñas
const int K = 10; //cambiar?
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

bool buscarMozo(FILE* fM, Mozo &m) {	//busqueda PUP, con el mozo como parametro x ref.
	int idMozo;
	bool cancelarBusqueda;
	bool encontrado = false;

	while (!encontrado) {

		cout << "Ingrese el id del mozo buscado o -1 para cancelar la busqueda" << endl;
		cin >> idMozo;

		if (idMozo == -1) {

			cout << "Cancelar la busqueda de mozo finalizara la carga de las ventas del dia" << endl;
			cout << "Proceder? (1 = si | 0 = no)" << endl;
			cin >> cancelarBusqueda;

			if (cancelarBusqueda) return false; //busqueda de mozo cancelada
			else continue;

		}

		long pos = idMozo;
		fseek(fM, pos * sizeof(Mozo), SEEK_SET);
		encontrado = fread(&m, sizeof(Mozo), 1, fM);	//1 encontrado | 0 no encontrado
		if (!encontrado) cout << "El mozo con el id: " << idMozo << ", no fue encontrado" << endl;
	}
	return true; //mozo encontrado
}

bool loginMozo(Mozo m) {
	Mozo mozo;
	char passIngresada[20];

	while (true) {
		cout << "Ingrese la contraseña del mozo con id " << mozo.idMozo <<" o -1 para cancelar el login"<< endl;
		cin >> passIngresada;
		if (passIngresada[0] == -1) return false; //login cancelado
		for (int i = 0;i < 20;i++) {
			if (mozo.password[i] - K != passIngresada[i]) {
				cout << "Contrasena incorrecta" << endl;
				continue; //discrepancia -> vuelve a pedir la contra
			}
			else break;	//contras iguales -> sale del loop
		}
	}
	return true;	//login exitoso
}

void cargarVenta(Mozo m,Producto p, Comanda &cN) {
	cN.idMozo = m.idMozo;
	cN.codigoProducto = p.codigo;
	cout << "Ingrese la cantidad de " << p.descripcion << endl;
	cin >> cN.cantidad;
	cN.comision = TASA_COMISION * (cN.cantidad * p.precio);
	
}

long buscarProducto(FILE* fI, Producto p) { //busqueda binaria, pq estan ordenados pero faltan algunos ids
	int codigoBuscado;

	cout << "Ingrese el codigo del producto" << endl;
	cin >> codigoBuscado;

	fseek(fI, 0, SEEK_END);
	long n = ftell(fI) / sizeof(Producto);
	long pri = 0;
	long ult = n - 1;
	long pos = -1;
	while (pri <= ult && pos == -1) {
		long med = (pri + ult) / 2;
		fseek(fI, med * sizeof(Producto), SEEK_SET);
		fread(&p, sizeof(Producto), 1, fI);
		if (p.codigo == codigoBuscado) pos = med;
		else if (codigoBuscado > p.codigo) pri = med + 1;
		else ult = med - 1;
	}
	return pos;
}

void cargarDia(FILE* fM, FILE* fI) {	//fM -> fMozos | fI -> fInventario
	FILE* fC = abrirArchivoComanda();	//fC -> fComandas
	if (!verificarAperturaDeArchivo(fC,"de comandas del dia solicitado")) return; //vuelve al main y se le pregunta al usuario si quiere cargar otro dia

	Comanda comandaNueva;
	Mozo mozoAux;
	Producto productoAux;
	bool continuar = true;

	while (continuar) {
		if (!buscarMozo(fM, mozoAux)) {
			cout << "Busqueda de mozo cancelada" << endl;
			break;	//sale del while 
		}
		if (!loginMozo(mozoAux)) {
			cout << "Login cancelado" << endl; 
			continue;	//itera y vuelve a pedir mozo
		}
		while (buscarProducto(fI, productoAux) == -1) cout << "Producto no encontrado" << endl;
		cargarVenta(mozoAux, productoAux,comandaNueva);

		if (guardarVenta(fC, comandaNueva)) {
			actualizarStock(fI, comandaNueva);
			actualizarComisionMozo(fM, comandaNueva);
		}
		cout << "Cargar otra venta? (1 = si | 0 = no)" << endl;
		cin >> continuar; //true -> sigue el while | false -> sale
	}
	ordenarComandas(fC);
	fclose(fC);
}

FILE* abrirArchivoComanda() {	//abrir o crear (ab)
	char nombreArchivoComanda[24];
	crearNombreArchivoComanda(nombreArchivoComanda); //el usuario carga el dia y se completa el array de chars para el nombre del .dat
	//verifica si es NULL en "cargarDia()"
	return fopen(nombreArchivoComanda, "ab");	//abre el arch con el nombre armado
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
	verificarAperturaDeArchivo(fM, nombreArch);
	return fM;
}