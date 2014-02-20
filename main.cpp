/*
 * Proyecto 2: Simulacion
 * 
 * Integrantes:
 * Ada Parra       19842103
 * Eliecer Uribe   20181085
 * Gabriel Nuñez   19843489
 * Roynny Zambrano 22210443
 * Tulio M. Loreto 20261035
 * 
	Se desea simular el siguiente sistema:
	
	A la sección de Atención al Cliente del banco X llegan personas con un tiempo entre
	llegadas exponencial con media de a minutos. Con la finalidad de dar una buena
	atención al cliente, el banco cuenta con cuatro operadores entrenados en los
	procesos del banco, los cuales tardan en promedio b minutos por persona, tiempo
	que sigue una distribución exponencial.
	
	Se desea conocer el desempeño del sistema en cuanto a:
	
	a)Utilización promedio del sistema.
	b)Cantidad promedio de clientes en atención al cliente.
	c)Cantidad promedio de clientes en la cola de atención al cliente.
	d)Tiempo promedio que pasan los clientes en el banco.
	e)Tiempo promedio que esperan los clientes en la cola de atención al cliente. 
 */

/*
 * 
 * El modelo de colas a simular sera:
 * Modelo(M/M/s):(GD/~/~)
 * Multiples servidores prestan servicio en paralelo.
 * 
*/

#include "Simulacion.h"
#include<iostream> //Libreria standard.
#include<fstream>  //Libreria de manejo de archivos.
#include<cstdlib>
#include<list>	//Libreria de lista
#include<time.h> //Libreria para usar hora del sistema (semilla).
#include<math.h> //Para calcular ln(r)

//Para no incluir todo el espacio de nombres (using namespace std;) se incluye solo lo que se utilizara... 
using std::cin; //Para entradas.
using std::cout; //Para salidas.
using std::endl; //Para saltos de linea.
using std::list; //Para la lista.

// Se define estructura evento
struct evento {
	int tipo; // 0 = llegada, 1 = salida
	float tiempo;
	int id_cliente;
};

void generarModeloTeorico(int numeroModelo, Simulacion s, FILE *myfile); //Modelo teorico completo...
void inicializacion(float &reloj, list<evento> &lista_eventos, evento &e, int &cliente_actual);
void llegada(int &clientes_en_sistema, bool &taquilla_ocupada, int &cliente_actual, list<int> &cola, list<evento> &lista_eventos, float reloj, float num_u[]);
void salida(int &clientes_en_sistema, bool &taquilla_ocupada, list<int> &cola, list<evento> &lista_eventos, float reloj, float num_u[]);
void numerosEnteros(int num_e[], int n);
void numerosUniformes(int num_e[], float num_u[], int n);

//Funcion principal del proyecto.
int main(){
	
	/*FILE *myFile; //Variable para la apertura del archivo de entrada modelo.in.
	FILE *myOtherFile; //Variable para la creacion del archivo de salida performance.out.
	int n; //Cantidad de modelos a tratar.
	float a, //Parametro que representa la media de llegadas 'a' de personas.
		  b; //Parametro que representa el promedio 'b' de servicio por persona.
    int	  replicas, //Cantidad de replicas.
		  ttransicion, //Tiempo de transicion.
		  tsimulacion; //Tiempo de simulacion.
	myFile = fopen("modelo.in","r"); //Apertura del archivo de entrada modelo.in con permisos de lectura solamente.
	myOtherFile = fopen("performance.out","w");	//Apertura del archivo de salida performance.out con permisos de escritura solamente.
	Simulacion s; //Variable de clase Simulacion con la cual se va a interactuar en las simulaciones.
	
	
	fscanf(myFile, "%d", &n); //Lectura de la cantidad n de modelos a tratar.
	cout << "La cantidad de simulaciones a realizar son: " << n << endl; //Prueba.
	for(int i = 0; i < n; i++){ //Se lee cada uno de los modelos a tratar.
		fscanf(myFile, "%f %f %d %d %d", &a, &b, &replicas, &ttransicion, &tsimulacion); //Lectura de datos de entrada.
	*/	
		/**Carga de cada simulacion en la variable s*/
	/*	s.setA(a);
		s.setB(b);
		s.setReplicas(replicas);
		s.setTtransicion(ttransicion);
		s.setTsimulacion(tsimulacion);
		
		cout << "Parametro a: " << s.getA() << endl; //Prueba.
		cout << "Parametro b: " << s.getB() << endl; //Prueba.
		cout << "Numero de replicas: " << s.getReplicas() << endl; //Prueba.
		cout << "Tiempo de transicion: " << s.getTtransicion() << endl; //Prueba.
		cout << "Tiempo de simulacion: " << s.getTsimulacion() << endl; //Prueba.
		cout << "Presione enter para continuar..." << endl; //Prueba.
		
		generarModeloTeorico(i+1,s,myOtherFile); //Llamada para generar archivo de salida performance.out.
		
		getchar(); //Prueba.
	}
	
	
	fclose(myFile); //Cierre del archivo de entrada modelo.in.
	fclose(myOtherFile); //Cierre del archivo de salida performance.out.
	*/
	
	int num_e[30];
	float num_u[30];
	
	//Variables simulación
	float reloj = 0;
	evento e;
	evento prox_e;
	list<evento> lista_eventos;
	float tiempo;
	
	list<int> cola;
	//bool taquilla_ocupada[4];
	bool taquilla_ocupada = false;
	int clientes_en_sistema = 0;
	int cliente_actual = 0;
	
	numerosEnteros(num_e, 30);			//Genera números aleatorios enteros
	numerosUniformes(num_e, num_u, 30);	//GEnera números aleatorios uniformes partiendo de los enteros
	inicializacion(reloj, lista_eventos, e, cliente_actual);
	
	while(reloj < 480)
	{
		//Rutina tiempo
		tiempo = lista_eventos.front().tiempo;
		reloj += tiempo;
		
		//Rutina eventos
		prox_e = lista_eventos.front();
		lista_eventos.pop_front();
		if(prox_e.tipo == 0)			//Si el proximo evento es una LLEGADA
		{
			cout << reloj << "\tLlega cliente- tiempo: " << prox_e.tiempo << "\t-id: " << prox_e.id_cliente << endl;
			llegada(clientes_en_sistema, taquilla_ocupada, cliente_actual, cola, lista_eventos, reloj, num_u);
			
		}
		else 							//Si no, el evento es una SALIDA
		{
			cout << reloj << "\tSale cliente- tiempo:" << prox_e.tiempo << "\t-id: " << prox_e.id_cliente << endl;
			salida(clientes_en_sistema, taquilla_ocupada, cola, lista_eventos, reloj, num_u);
		
		}
		getchar(); 
	}
	return 0;							//Se le dice al sistema que todo salio bien.
}

void generarModeloTeorico(int numeroModelo, Simulacion s, FILE *myfile){	
			
	fprintf(myfile,"Modelo %d: \n", numeroModelo);
	fprintf(myfile,"                 Utilizacion          L          Lq          W          Wq\n");
	fprintf(myfile,"Simulacion       xx.xx                xx.xx      xx.xx      xx.xx       xx.xx\n");
	fprintf(myfile,"Teorico          %.5f             %.5f    %.5f    %.5f     %.5f\n\n",s.PorcentajeUtilizacion(4,4,s.ro(),s.getA()),s.L(4,s.ro()),s.Lq(4,s.ro()),s.W(4,s.ro(),s.getA(),s.getB()),s.Wq(4,s.ro(),s.getA()));
	
}

void salida(int &clientes_en_sistema, bool &taquilla_ocupada, list<int> &cola, list<evento> &lista_eventos, float reloj, float num_u[])
{
	int proximo_cliente;
	evento evento_salida;
	
	clientes_en_sistema -= 1;
	if(cola.size() > 0)
	{
		//Sacamos el proximo cliente de la cola
		proximo_cliente = cola.front();
		cola.pop_front();
		//Programando la salida
		evento_salida.tipo = 1;
		evento_salida.tiempo =  -13*log(num_u[rand() % 31]);
		evento_salida.id_cliente = proximo_cliente;
		lista_eventos.push_back(evento_salida);
	}
	else
		taquilla_ocupada = false;
}

void llegada(int &clientes_en_sistema, bool &taquilla_ocupada, int &cliente_actual, list<int> &cola, list<evento> &lista_eventos, float reloj, float num_u[])
{
	evento evento_salida, proxima_llegada; 
	clientes_en_sistema += 1;
	
	if (taquilla_ocupada)
	{
		cola.push_back(cliente_actual);
	}
	else
	{
		taquilla_ocupada = true;
		evento_salida.tipo = 1;
		evento_salida.tiempo =  -13*log(num_u[rand() % 31]);	// tiempo aleatorio servicio
		evento_salida.id_cliente = cliente_actual;
		lista_eventos.push_back(evento_salida);
	}
	
	//Creando nueva llegada
	proxima_llegada.tipo = 0; 									// tipo llegada
	proxima_llegada.tiempo = (-15*log(num_u[rand() % 31]));		// tiempo aleatorio llegada
	cliente_actual += 1;
	proxima_llegada.id_cliente = cliente_actual;
	lista_eventos.push_back(proxima_llegada);
	
}

void inicializacion(float &reloj, list<evento> &lista_eventos, evento &e, int &cliente_actual)
{
	reloj = 0;
	e.tipo = 0;
	e.tiempo = 0;
	cliente_actual += 1;
	e.id_cliente = cliente_actual;		//Termino de crear evento e
	lista_eventos.push_back(e);			//Agrega primer evento
	
}

void numerosEnteros(int num_e[], int n)
{
	int i, hora = time(NULL);			//Inicializa variable hora con la del sistema
	
	srand(hora);						//Usa la hora del sistema como semilla
	
	for(i=0;i<n;i++)
	{
		num_e[i] = rand() % 100;
		//cout << num_e[i] << endl;
	}
}
void numerosUniformes(int num_e[], float num_u[], int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		num_u[i] = (num_e[i] + 0.5) / 100;
		//cout << num_u[i] << endl;
	}
}
