#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;


struct Comanda {
	int idMozo;
	int codigoProducto;
	int cantidad;
	float comision;
};

const int MAX_DIAS = 7; // maximo de dias de la semana

//Declaraciones
bool verificarAperturaDeArchivo(FILE* f, string nombre);
void cargarFecha(char fecha[], int numeroDia);
void crearNombreArchivoDiario(char nombreArchivo[], char fecha[]);
bool crearNombreArchivoSemanal(int numeroSemana, int mes, char nombreArchivo[]);
int abrirArchivosDiarios(FILE* fd[], char fechar[][11], int cantDias, Comanda actual[], bool hayDato[]);
int mezclarComandas(FILE* fd[], Comanda actual[], bool hayDato[], int cantDias, FILE* fS);
void cerrarArchivos(FILE* fD[], int cantDias);


bool verificarAperturaDeArchivo(FILE* f, string nombre) {
	if (f == NULL) {                                                       //chequea que el archivo se haya abierto correctamente
		cout << "Error, no se pudo abrir el archivo " << nombre << endl;
		return false;
	}
	else return true;
}

void cargarFecha(char fecha[], int numeroDia) {
	cout << "Ingrese la fecha del dia " << numeroDia << " (DD-MM-AAAA): ";// Solicita al usuario que ingrese la fecha del día correspondiente
	cin.ignore();
	cin.getline(fecha,11);
}

void crearNombreArchivoDiario(char nombreArchivo[], char fecha[]) {  // Construye el nombre del archivo diario a partir de la fecha
	char prefijo[] = "Comandas_"; // 9 caracteres en total
	char extension[] = ".dat"; // 4 caracteres en total

	int pos = 0;
	for (int i = 0; i < 9; i++) {
		nombreArchivo[pos++] = prefijo[i];  // Copia el prefijo al nombre del archivo
	}
	for (int i = 0; i < 10; i++) {
		nombreArchivo[pos++] = fecha[i];  // Copia la fecha al nombre del archivo
	}
	for (int i = 0; i < 4; i++) {
		nombreArchivo[pos++] = extension[i];  // Copia la extensión al nombre del archivo
	}
	nombreArchivo[pos] = '\0';  // Agrega el carácter nulo al final del nombre del archivo
}

bool crearNombreArchivoSemanal(int numeroSemana, int mes, char nombreArchivo[]) {

	char base[] = "Comandas_semana_s"; // 19 caracteres en total
	char extension[] = ".dat"; // 4 caracteres en total

	if (numeroSemana > 4 || numeroSemana <= 0) { //semana valida solo entre 1 y 4
		cout << "Numero de semana invalido.\n";
		return false;
	}
	if (mes < 1 || mes > 12) { //mes valido solo entre 1 y 12
		cout << "Numero de mes invalido.\n";
		return false;
	}
	int pos = 0;
	for (int i = 0; base[i] != '\0'; i++) {
		nombreArchivo[pos++] = base[i]; //copia la base del nombre
	}
	nombreArchivo[pos++] = '0' + numeroSemana; //copia el numero de semana
	nombreArchivo[pos++] = '_';
	nombreArchivo[pos++] = (mes / 10) + '0'; //decena del mes
	nombreArchivo[pos++] = (mes % 10) + '0'; //unidad del mes
	for (int i = 0; extension[i] != '\0'; i++) {
		nombreArchivo[pos++] = extension[i]; //copia la extension
	}
	nombreArchivo[pos] = '\0';


	return true;
}

int abrirArchivosDiarios(FILE* fD[], char fechas[][11], int cantDias, Comanda actual[], bool hayDato[]) { //abre cada planilla diaria y carga su primer registro
	int archivosAbiertos = 0;

	for (int i = 0; i < cantDias; i++) { //recorre cada dia pedido
		char nombreArchivo[24];
		crearNombreArchivoDiario(nombreArchivo, fechas[i]);
		fD[i] = fopen(nombreArchivo, "rb");

		if (fD[i] == NULL) { //el dia no tuvo ventas, no es error fatal
			cerr << "Aviso: no se encontro " << nombreArchivo << ", se omite ese dia." << endl;
			hayDato[i] = false;
			continue;
		}

		archivosAbiertos++;
		hayDato[i] = (fread(&actual[i], sizeof(Comanda), 1, fD[i]) == 1); //lectura anticipada
	}

	return archivosAbiertos;
}

int mezclarComandas(FILE* fD[], Comanda actual[], bool hayDato[], int cantDias, FILE* fS) { //apareo de los archivos ordenados por idMozo
	int totalRegistros = 0;
	bool quedaAlgo = true;

	while (quedaAlgo) { //repite hasta que ningun archivo tenga mas datos
		int indiceMenor = -1;

		for (int i = 0; i < cantDias; i++) { //busca el idMozo mas chico entre los archivos activos
			if (!hayDato[i]) continue;
			if (indiceMenor == -1 || actual[i].idMozo < actual[indiceMenor].idMozo) {
				indiceMenor = i;
			}
		}

		if (indiceMenor == -1) { //no quedo ningun archivo con datos
			quedaAlgo = false;
			continue;
		}

		fwrite(&actual[indiceMenor], sizeof(Comanda), 1, fS); //graba el minimo encontrado
		totalRegistros++;

		hayDato[indiceMenor] = (fread(&actual[indiceMenor], sizeof(Comanda), 1, fD[indiceMenor]) == 1); //avanza solo ese archivo
	}

	return totalRegistros;
}

void cerrarArchivosDiarios(FILE* fD[], int cantDias) { //cierra todos los archivos diarios abiertos
	for (int i = 0; i < cantDias; i++) {
		if (fD[i] != NULL) fclose(fD[i]);
	}
}

//main
int main() {
	int cantDias;
	cout << "Cuantos dias tiene la semana a cerrar? ";
	cin >> cantDias;
	if (cantDias <= 0 || cantDias > MAX_DIAS) {
		cerr << "Cantidad de dias invalida." << endl;
		return -1;
	}

	char fechas[MAX_DIAS][11];
	for (int i = 0; i < cantDias; i++) { //pide la fecha de cada dia
		cargarFecha(fechas[i], i + 1);
	}

	int numeroSemana, mes;
	cout << "Ingrese el numero de semana (de 1 a 4): ";
	cin >> numeroSemana;
	cout << "Ingrese el numero de mes (1 a 12): ";
	cin >> mes;

	FILE* fD[MAX_DIAS]; //fD -> fDias (una planilla por dia)
	Comanda actual[MAX_DIAS];
	bool hayDato[MAX_DIAS];

	int archivosAbiertos = abrirArchivosDiarios(fD, fechas, cantDias, actual, hayDato);
	if (archivosAbiertos == 0) { //ningun dia tuvo planilla --> no se puede seguir
		cerr << "No se encontro ninguna planilla diaria para esa semana." << endl;
		return -1;
	}

	char nombreArchSemanal[26];
	if (!crearNombreArchivoSemanal(numeroSemana, mes, nombreArchSemanal)) {
		cerrarArchivosDiarios(fD, cantDias);
		return -1;
	}

	FILE* fS = fopen(nombreArchSemanal, "wb"); //fS -> fSemanal (si ya existia, se pisa)
	if (!verificarAperturaDeArchivo(fS, nombreArchSemanal)) {
		cerrarArchivosDiarios(fD, cantDias);
		return -1;
	}

	int totalRegistros = mezclarComandas(fD, actual, hayDato, cantDias, fS);

	cerrarArchivosDiarios(fD, cantDias);
	fclose(fS);

	cout << "\nListo. Se genero " << nombreArchSemanal << " con " << totalRegistros
		<< " comandas, ordenadas por mozo." << endl;

	return 0;
}