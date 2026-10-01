#include <iostream>
#include <cstdio>
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

void buscarMozo(FILE* fM, Mozo& m,int idMozo ) {	//busqueda PUP, con el id/pos ya conocido
	long pos = idMozo;
	fseek(fM, pos * sizeof(Mozo), SEEK_SET);
	fread(&m, sizeof(Mozo), 1, fM);
}

long buscarMozo(FILE* fM, Mozo &m) {	//busqueda PUP, con el mozo como parametro x ref.
	int idMozo;
	bool encontrado = false;

	cout << "Ingrese el id del mozo buscado" << endl;
	cin >> idMozo;

	fseek(fM, idMozo * sizeof(Mozo), SEEK_SET);
	encontrado = fread(&m, sizeof(Mozo), 1, fM);	//1 encontrado | 0 no encontrado
	if (!encontrado) {
		cout << "El mozo con el id: " << idMozo << ", no fue encontrado" << endl;
		return -1;
	}
	
	return idMozo; //mozo encontrado
}


bool loginMozo(Mozo m) {
	char passIngresada[20];

}

void cargarVenta(Mozo m,Producto p, Comanda &cN) {
	cN.idMozo = m.idMozo;
	cN.codigoProducto = p.codigo;
	cout << "Ingrese la cantidad de " << p.descripcion << endl;
	cin >> cN.cantidad;
	cN.comision = TASA_COMISION * (cN.cantidad * p.precio);
	
}

long buscarProducto(FILE* fI, Producto &p, int codigoBuscado) { //busqueda binaria de producto, con el codigo buscado como param.

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

long buscarProducto(FILE* fI, Producto &p) { //busqueda binaria de producto
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

void actualizarStock(FILE* fI, Comanda c) {
	Producto producto;
	long pos = buscarProducto(fI, producto, c.codigoProducto);
	if (pos == -1) {	//nunca va a llegar aca
		cout << "Producto no encontrado" << endl;
		return;
	}
	producto.stockActual -= c.cantidad;
	fseek(fI, pos * sizeof(Producto), SEEK_SET);
	fwrite(&producto, sizeof(Producto), 1, fI);
	
}

void actualizarComisionMozo(FILE* fM, Comanda c) {
	Mozo mozo;
	buscarMozo(fM, mozo, c.idMozo);
	mozo.totalComision += c.comision;
	fseek(fM, mozo.idMozo * sizeof(Mozo), SEEK_SET);
	fwrite(&mozo, sizeof(Mozo), 1, fM);
}

bool placeholder(FILE* fM, Mozo &m) { 
	bool opcion; 
	while (buscarMozo(fM, m) == -1) {
		cout << "Mozo no encontrado" << endl;
		cout << "Seleccione una opcion" << endl;
		cout << "0. Terminar carga del dia" << endl;
		cout << "1. Volver a buscar un mozo" << endl;
		cin >> opcion;
		if (!opcion) return true; //se termina la carga del dia
		//opcion en !true (false) -> sigue el while
	}	
	return false; //mozo encontrado
}

bool placeholder2(Mozo m) {
	bool opcion;
	while (!loginMozo(m)) {
		cout << "Login faillido" << endl;
		cout << "Seleccione una opcion" << endl;
		cout << "0. Cambiar mozo" << endl;
		cout << "1. Volver a intenar iniciar sesion" << endl; 
		cin >> opcion;
		if (!opcion) return true; //se cancela el login 
		//opcion en !true (false) -> sigue el while
	}

	return false; //login exitoso
}

bool placeholder3(FILE* fI, Producto &p) {
	bool opcion;
	while (buscarProducto(fI, p) == -1) {
		cout << "Producto no encontrado" << endl;
		cout << "Seleccione una opcion" << endl;
		cout << "0. Cancelar busqueda de producto" << endl;
		cout << "1. Buscar otro producto" << endl; 
		cin >> opcion;
		if (!opcion) return true; //se cancela la busqueda 
		//opcion en !true (false) -> sigue el while
	}
	return false; //producto encontrado
}

void cargarDia(FILE* fM, FILE* fI) {	//fM -> fMozos | fI -> fInventario
	FILE* fC = abrirArchivoComanda();	//fC -> fComandas
	if (!verificarAperturaDeArchivo(fC,"de comandas del dia solicitado")) return; //vuelve al main y se le pregunta al usuario si quiere cargar otro dia

	Comanda comandaNueva;
	Mozo mozoAux;
	Producto productoAux;
	bool continuar = true;
	while (continuar) {
		if (placeholder(fM, mozoAux)) break;	//se termina la carga del dia == sale del while
		if (placeholder2(mozoAux)) continue; //se cancela el login == itera el while (y vuelve a 'buscar mozo')
		if (placeholder3(fI,productoAux)) continue; //se cancela la busqueda de prod == itera el while (y vuelve a 'buscar mozo')

		cargarVenta(mozoAux, productoAux,comandaNueva);

		if (guardarVenta(fC, comandaNueva)) {
			actualizarStock(fI, comandaNueva);
			actualizarComisionMozo(fM, comandaNueva);
		}
		cout << "Cargar otra venta? (1 = si | 0 = no)" << endl;
		cin >> continuar; //true -> sigue el while | false -> sale
	}
	//ordenarComandas(fC);
	fclose(fC);
}

FILE* abrirArchivoComanda() {	//abrir o crear (ab)
	char nombreArchivoComanda[24]="comandas_";
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
	char fecha[11];
	char extension[5] = ".dat";

	cargarFecha(fecha);
	int j = 0;
	for (int i = 9;i < 19;i++) {
		nArchivo[i] = fecha[j++];
	}
	j = 0;
	for (int i = 19;i < 23;i++) {
		nArchivo[i] = extension[j++];
	}
	nArchivo[23] = '\0';
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