#include <iostream>         ///Programa realizado por PABLO JIMENEZ CRUZ y SERGIO RAMOS MESA
#include <string>
#include <fstream>
using namespace std;

enum tElemento {Piedra, Papel, Tijera, Lagarto, Spock};
enum tResultado{GanaJ1, GanaJ2, Empate};

const string centinela = "XXX";  //Se trata de un centinela para determinar donde los ficheros deben dejar de leer datos


string bienvenida(); //Da la bienvenida al usuario y le pregunta por su nombre para comprobar si está en el archivo de usuarios
bool localizacionJugador(string apodo); //Busca el nombre del usuario en el archivo de texto. Si el usuario está deja acceder al programa, si no salé del programa
void menu(int &jugadas, int &j1, int &j2, int &empate); //Esta es una función que compone la estructura del menu principal (mostar menu, y ejecutar opcion)
int mostrarMenu();//Muestra el menú principal al usuario dando opciones para escoger una
int ejecutarOpc(int opc, int &jugadas, int &j1, int &j2, int &empate);//Ejecuta la opción deseada llamando a diferentes funciones según la elección del usuario
void menu2(int opc, int res, tElemento ap1, tElemento ap2, int &jugadas, int &j1, int &j2, int &empate);//Compone la estructura principal del submenu de partida
int mostrarMenu2();//Muestra el submenú dentro del juego y le da a elegir una opción al usuario
int ejecutarOpc2(int opc, int opc2, int res, tElemento ap1, tElemento ap2, int &jugadas, int &j1, int &j2, int &empate);//Ejecuta la opción del submenú llamando a diferentes funciones
int menuAp();//Muestra las apuestas disponibles al jugador y da a elegir una al usuario
bool mostrarReglas();//Abre un fichero de texto y lee las reglas del juego, una vez leidas la función cierra el archivo de texto para evirtar corrupciones
void anotaRes(int res, int &jugadas, int &j1, int &j2, int &empate); //Esta función anota el resultado y actualiza los contadores
void muestraRes(int jugadas, int j1, int j2, int empate); //Muestra los resultados (contadores) al slair al menú principal después de jugar una o varias partidas. No se muestran al salir del programa porque el propio programa da la opción de volver al menú para cambiar de modalidad de juego antes de salir
tElemento tiraJ1(); //Esta función ejecuta la apuesta de la maquina para "singleplayer" ramdomizando un numero de 1 a 5 y ligandolo a cada una de las 5 apuestas
tElemento tiraJ2(); //Esta función se parece a tiraJ1 solo que esta función se utiliza para los jugadores. Llama a la función menuAP y reliza una apuesta para cada jugador
void muestraGanador(tResultado res); //Muestra por pantalla el resultado de la partida (Quien gana o si hay un empate y devuelve el valor en un string
int ganador(tElemento ap1, tElemento ap2); //Esta función determina quien gana la partida, ponderando ocn las reglas del juego muestran en pantalla ambas apuestas y quien gana la partida
string muestraresultado(tElemento apX); //Est6a función transforma un tElemento en un string para poder mostrar por pantalla las opciones de los jugadores

int main()
{
	setlocale(LC_ALL, "spanish"); //Comando para configurar como idioma del programa por defecto el español
	int jugadas = 0, j1 = 0, j2 = 0, empate = 0;	
	string apodo = bienvenida(); ///PREGUNTAR si así o en dos lineas
	bool ok;
	ok = localizacionJugador(apodo);
	if (ok)
	{
		cout << endl; 
		menu(jugadas, j1, j2, empate);
	}
	system("pause");
	return 0;
}

///APARTADO DE LOGIN Y BIENVENIDA
string bienvenida()
{
	string apodo;
	cout << "	BIENVENIDO A						Programa realizado por:" << endl;
	cout << "PIEDRA PAPEL TIJERA LAGARTO SPOCK			- Sergio Ramos Mesa	- Pablo Jimenez Cruz "<< endl;
	cout << endl;
	cout << "Por favor, introduzca su nombre de usuario: ";
	cin >> apodo;
	return apodo;
}

bool localizacionJugador(string apodo)
{
	bool encontrado = true;
	string fichLine;
	ifstream fich;
	fich.open("registro.txt"); //Abre el fichero registro.txt

	if (!(fich.is_open())) //El if define si el fichero se ha abierto o ha habido un error
	{
		cout << "Error inesperado, el fichero no pudo abrirse" << endl;
		encontrado = false;
	}
	else
	{

		while (fichLine != apodo && fichLine != centinela) //Mientras que el fichero no lea el nombre introducido ni el centinela debe seguir leyendo
		{
			getline(fich, fichLine);
		}
		if (fichLine != apodo) 
		{ 
			encontrado = false;  
			cout << "Lo sentimos, el nombre no se encuentra en nuestra base de datos." << endl;
		}
		else if (fichLine == apodo) { cout << "Bienvenido " << apodo << endl; }
	}
	fich.close();
	return encontrado;
}

///APARTADO MENU
//Menu principal
void menu(int &jugadas, int &j1, int &j2, int &empate)
{
	int opc = 1;
	while (opc != 0 && opc != 2)
	{
		opc = mostrarMenu();
		ejecutarOpc(opc, jugadas, j1, j2, empate);
	}
}

int mostrarMenu()
{
	int opc;
	cout << "1-> Mostrar Reglas" << endl;
	cout << "2-> Un Jugador" << endl;
	cout << "3-> Dos jugadores" << endl;
	cout << "0-> Salir" << endl;
	cout << "Introduzca una opción para continuar: ";
	cin >> opc;
	while (opc < 0 || opc > 3 || cin.fail())
	{
		cout << "Se ha producido un error inesperado" << endl;
		cin.clear();
		cin.ignore();
		cout << "Introduzca de nuevo el valor de opción: ";
		cin >> opc;
	}
	return opc;
}

int ejecutarOpc(int opc, int &jugadas, int &j1, int &j2, int &empate)
{
	int res;
	tElemento ap1;
	tElemento ap2;
	if (opc == 1)
	{
		mostrarReglas();
	}
	else if (opc == 2)
	{
		cout << endl;
		cout << "--------------" << endl;
		cout << "SINGLEPLAYER" << endl;
		cout << "--------------" << endl;
		cout << endl;
		cout << "Turno del Jugador 1 (IA)" << endl;
		ap1 = tiraJ1();
		cout << "Turno del Jugador 2" << endl;
		ap2 = tiraJ2();
		res = ganador(ap1, ap2);
		anotaRes(res, jugadas, j1, j2, empate);
		menu2(opc, res, ap1, ap2, jugadas, j1, j2, empate);
	}
	else if (opc == 3)
	{
		//system("cls");
		cout << endl;
		cout << "------------------" << endl;
		cout << "LOCAL MULTIPLAYER " << endl;
		cout << "------------------" << endl;
		cout << "Turno del Jugador 1" << endl;
		ap1 = tiraJ2();
		cout << "Turno del Jugador 2" << endl;
		ap2 = tiraJ2();
		res = ganador(ap1, ap2);
		anotaRes(res, jugadas, j1, j2, empate);
		menu2(opc, res, ap1, ap2, jugadas, j1, j2, empate);
	}
	else if (opc == 0)
	{
		cout << "Ha elegido salir." << endl;
		cout << "Mostrando tabla de resultados: " << endl;
		return 0;
	}
	return opc;
}

//Submenu
void menu2(int opc, int res, tElemento ap1, tElemento ap2, int &jugadas, int &j1, int &j2, int &empate)
{
	int opc2 = 1;
	while (opc2 != 3)
	{
		opc2 = mostrarMenu2();
		ejecutarOpc2(opc, opc2, res, ap1, ap2, jugadas, j1, j2, empate);
	}
}

int mostrarMenu2()
{
	int opc;
	cout << endl;
	cout << "1-> Mostrar las reglas nuevamente" << endl;
	cout << "2-> Seguir jugando" << endl;
	cout << "3-> Salir al menu principal" << endl;
	cout << "Introduzca una opcion para continuar: ";
	cin >> opc;
	while (opc < 1 || opc > 3 || cin.fail())
	{
		cout << "Se ha producido un error inesperado" << endl;
		cin.clear();
		cin.ignore();
		cout << "Introduzca de nuevo el valor de opcion: ";
		cin >> opc;
	}
	return opc;
}

int ejecutarOpc2(int opc, int opc2, int res, tElemento ap1, tElemento ap2, int &jugadas, int &j1, int &j2, int &empate)
{
	if (opc2 == 1)
	{
		mostrarReglas();
	}
	else if (opc2 == 2)
	{
		cout << endl;
		cout << "--------------" << endl;
		cout << "NUEVA PARTIDA" << endl;
		cout << "--------------" << endl;
		if (opc == 2)
		{
			cout << "Turno del Jugador 1" << endl;
			ap1 = tiraJ1();
			cout << "Turno del Jugador 2" << endl;
			ap2 = tiraJ2();
			res = ganador(ap1, ap2);
			anotaRes(res, jugadas, j1, j2, empate);
		}
		else if (opc == 3)
		{
			cout << "Turno del Jugador 1" << endl;
			ap1 = tiraJ2();
			cout << "Turno del Jugador 2" << endl;
			ap2 = tiraJ2();
			res = ganador(ap1, ap2);
			anotaRes(res, jugadas, j1, j2, empate);
		}
	}
	else if (opc2 == 3)
	{
		cout << endl;
		muestraRes(jugadas, j1, j2, empate);
		cout << endl;
		cout << "VOLVIENDO AL MENU PRINCIPAL" << endl;
		menu(jugadas, j1, j2, empate);
	}
	return opc2;
}

	
bool mostrarReglas()
{
	bool ok;
	string fichLine;
	ifstream fich;
	fich.open("reglas.txt");

	if (!(fich.is_open()))
	{
		cout << "Error inesperado, el fichero no pudo abrirse" << endl;
		ok = false;
	}
	else
	{
		ok = true;
		while ((fichLine != centinela))
		{
			getline(fich, fichLine);
			if (fichLine != centinela)
			{
				cout << fichLine << endl;
			}
		}
	}
	fich.close();
	return ok;
}

///APARTADO DE APUESTAS
//En este apartado se encuentra todo lo relacionado con las apuestas de ambos jugadores (V1)
int menuAp()
{
	int ap;
	cout << "APUESTAS POSIBLES: " << endl;
	cout << "1 -> Piedra" << endl;
	cout << "2 -> Papel" << endl;
	cout << "3 -> Tijera" << endl;
	cout << "4 -> Lagarto" << endl;
	cout << "5 -> Spock" << endl;
	cout << "Introduzca una opción para continuar: ";
	cin >> ap;
	while (ap < 1 || ap > 5 || cin.fail())
	{
		cout << "Error inesperado en el programa" << endl;
		cin.clear();
		cin.ignore();
		cout << "Por favor, introduzca de nuevo el dato: " << endl;
		cin >> ap;
	}
	return ap;
}

string muestraresultado(tElemento apX) //Esta función convierte un tElemento en un string para poder mostrar en pantalla la opción de cada jugador
{
	string ap;
	if (apX == 0)
	{
		ap = "Piedra";
	}
	else if (apX == 1)
	{
		ap = "Papel";
	}
	else if (apX == 2)
	{
		ap = "Tijera";
	}
	else if (apX == 3)
	{
		ap = "Lagarto";
	}
	else if (apX == 4)
	{
		ap = "Spock";
	}
	return ap;
}

tElemento tiraJ1()
{
	int x;
	tElemento ap1;
	string ap;
	x = rand() % 5;
	if (x == 0) { ap1 = Piedra; }
	else if (x == 1) { ap1 = Papel; }
	else if (x == 2) { ap1 = Tijera; }
	else if (x == 3) { ap1 = Lagarto; }
	else if (x == 4) { ap1 = Spock; }
	else { cout << "Error inesperado en el programa, reinicie la consola" << endl; }
	ap = muestraresultado(ap1);
	cout << "Apuesta realizada, buena suerte" << endl;
	cout << endl;
	return ap1;
}


tElemento tiraJ2()
{
	tElemento ap2;
	string apuesta2;
	int x;
	x = menuAp();
	if (x == 1) { ap2 = Piedra; }
	else if (x == 2) { ap2 = Papel; }
	else if (x == 3) { ap2 = Tijera; }
	else if (x == 4) { ap2 = Lagarto; }
	else if (x == 5) { ap2 = Spock; }
	else { cout << "Error inesperado en el programa, reinicie la consola" << endl; }
	apuesta2 = muestraresultado(ap2);
	cout << "Apuesta realizada, buena suerte" << endl;
	cout << endl;
	return ap2;
}

///APARTADO DE PROCESAMIENTO DE GANADOR
int ganador(tElemento ap1, tElemento ap2)
{
	int x;
	tResultado ganador;
	cout << endl;
	//CASOS EN LOS QUE GANA JUGADOR 1
	if (ap1 == Tijera && ap2 == Papel)
	{
		ganador = GanaJ1;
		muestraGanador(ganador);
		x = 1;
	}
	else if (ap1 == Papel && ap2 == Piedra)
	{
		ganador = GanaJ1;
		muestraGanador(ganador);
		x = 1;
	}
	else if (ap1 == Piedra && ap2 == Lagarto)
	{
		ganador = GanaJ1;
		muestraGanador(ganador);
		x = 1;
	}
	else if (ap1 == Lagarto && ap2 == Spock)
	{
		ganador = GanaJ1;
		muestraGanador(ganador);
		x = 1;
	}
	else if (ap1 == Spock && ap2 == Tijera)
	{
		ganador = GanaJ1;
		muestraGanador(ganador);
		x = 1;
	}
	else if (ap1 == Tijera && ap2 == Lagarto)
	{
		ganador = GanaJ1;
		muestraGanador(ganador);
		x = 1;
	}
	else if (ap1 == Lagarto && ap2 == Papel)
	{
		ganador = GanaJ1;
		muestraGanador(ganador);
		x = 1;
	}
	else if (ap1 == Papel && ap2 == Spock)
	{
		ganador = GanaJ1;
		muestraGanador(ganador);
		x = 1;
	}
	//CASOS QUEDAN EN TABLAS

	else if (ap1 == Piedra && ap2 == Piedra)
	{
		ganador = Empate;
		muestraGanador(Empate);
		x = 0;
	}
	else if (ap1 == Papel && ap2 == Papel)
	{
		ganador = Empate;
		muestraGanador(Empate);
		x = 0;
	}
	else if (ap1 == Tijera && ap2 == Tijera)
	{
		ganador = Empate;
		muestraGanador(Empate);
		x = 0;
	}
	else if (ap1 == Lagarto && ap2 == Lagarto)
	{
		ganador = Empate;
		muestraGanador(Empate);
		x = 0;
	}
	else if (ap1 == Spock && ap2 == Spock)
	{
		ganador = Empate;
		muestraGanador(Empate);
		x = 0;
	}
	//CASOS EN LOS QUE GANA EL JUGADOR DOS
	else
	{
		ganador = GanaJ2;
		muestraGanador(GanaJ2);
		x = 2;
	}
	cout << " RESULTADOS " << endl;
	cout << "J1: " << muestraresultado(ap1) << " <-- VS --> J2: " << muestraresultado(ap2) << endl; //El problema es que muestra un numero y debe mostrar un texto ej: Spock VS Piedra; no 5 VS 1
	//cout << ganador;
	return x;
}


void muestraGanador(tResultado res) {
	if (res == GanaJ1) {
		cout << "Ha ganado el Jugador 1" << endl;
	}
	else if (res == GanaJ2) {
		cout << "Ha ganado el Jugador 2" << endl;
	}
	else if (res == Empate) {
		cout << "Ups, parece que ha habido un empate." << endl;
	}
}

///APARTADO DE ANOTACION DE RESULTADOS
void anotaRes(int res, int &jugadas, int &j1, int &j2, int &empate)             //Si x=0, empate, si x=1, gana J1, si x=2, gana J2
{
	if (res == 0)
	{
		jugadas++;
		empate++;
	}
	else if (res == 1)
	{
		jugadas++;
		j1++;
	}
	else if (res == 2)
	{
		jugadas++;
		j2++;
	}
}

void muestraRes(int jugadas, int j1, int j2, int empate)
{
	cout << "--------------------" << endl;
	cout << " TABLA DE RESULTADOS" << endl;
	cout << "--------------------" << endl;
	cout << "Partidas jugadas: " << jugadas << endl;
	cout << "Partidas ganadas: " << j2 << endl;
	cout << "Partidas perdidas: " << j1 << endl;
	cout << "Partidas empatadas:" << empate << endl;
}