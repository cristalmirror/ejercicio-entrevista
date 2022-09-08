/*este codigo es la modificacion del codigo test01.cpp, est codigo se diferencia
 en que su progrmacion es conocurrente de forma que a misma cantidad de tiempo ejecuta
 permite guardar 4 veses mas ordenes, reduciendo drasticamente el embudo que se produce
IMPORTANTE: ESTE CODIGO FUE PROGRAMADO DESDE GNU/LINUX CON G++*/
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<ctime>
#include<string>
#include<iterator>
#include<list>
#include<unistd.h>
#include<thread>
#include<algorithm>

using namespace std;


typedef struct node{//contiene el nodo de la lista __orders
	int num;
	string id;
}node_t;

typedef struct men{//tipo de dato de los mensajes
	int tam;
	string id;
	string text;
}message_t;

class OrderManager{
public:

        int __orders_prossed = 0;
	time_t __last_printed_log=time(0);
	list<node_t> lis;	
	node_t uuid_gen(node_t nd);//genera los uuid
	void __log(message_t  message);// imprime mensajes	
	void __generate_fake_order(int quantity);// genera las ordenes
	void __fake_save_on_db(node_t order);//simula el gurdadode de la orden
	void process_orders();//realiza el procesamiento las ordenes 
	
};    

node_t OrderManager::uuid_gen(node_t nd){//generador de uudi
	char hex_char[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	int i=0;
	       
	for (i = 0; i < 8; ++i) {
		nd.id=nd.id + hex_char[rand()%16];
	}
	i=0;
	nd.id=nd.id + '-';
		
	for (i = 0; i < 4; ++i) {
		nd.id=nd.id + hex_char[rand()%16];
	}
	i=0;
	nd.id=nd.id + '-';

	for (i = 0; i < 4; ++i) {
		nd.id=nd.id + hex_char[rand()%16];
	}
	i=0;
	nd.id=nd.id + '-';
		
	for (i = 0; i < 4; ++i) {
		nd.id=nd.id + hex_char[rand()%16];
	}
	i=0;
	nd.id=nd.id + '-';

	for (i = 0; i < 12; ++i) {
		nd.id=nd.id + hex_char[rand()%16];
	}

	return nd;

}

void OrderManager::__log(message_t  message){//imprime los mensajes
	time_t vtime;
	
	time(&vtime);		
	cout << ctime(&vtime)<<" > "<<message.tam<<message.text<<"\n";

}
	
void OrderManager::__generate_fake_order(int quantity){//genera las ordenes
	int i;
	message_t message;
	for (i = 0; i < quantity; ++i) {

		node_t __orders;
		__orders=uuid_gen(__orders);
		__orders.num=i;	
		lis.push_back(__orders);
		        
	}

	message.tam=i;
	message.text=" generated...";
	__log(message);
}

void OrderManager::__fake_save_on_db(node_t order){//simula el guradado de las ordenes
	time_t v2time;

	time(&v2time);
	cout << ctime(&v2time)  <<" > Order ["<< order.id <<"] "<< order.num <<" was successfully prosecuted."<< "\n";
	
	sleep(rand()%2);

}

void OrderManager::process_orders(){//realiza el procesos de guardado

	time_t now;
	
	for (auto i = lis.begin(); i != lis.end(); ++i) {
		__fake_save_on_db(*i);
		__orders_prossed++;

		now=time(0);
		if (now > __last_printed_log) {
			__last_printed_log =__last_printed_log+5;

			cout << ctime(&now) <<" Total orders executed: "<< __orders_prossed <<"/"<< lis.size() << "\n";

		}
	        
	}
	
}

int main(int argc, char *argv[]){
	OrderManager ObjOrdrs;
	time_t start_time,finish_time,delay;
	
	ObjOrdrs.__generate_fake_order(1000);//simula __init__ 
						       
	time(&start_time);

	/*atraves de estos hilos podemos potenciar rendimiento
	 del tiempo de guardado*/
	thread h1(&OrderManager::process_orders,&ObjOrdrs);
	thread h2(&OrderManager::process_orders,&ObjOrdrs);
	ObjOrdrs.process_orders();
	ObjOrdrs.process_orders();
       
		
	time(&finish_time);

	delay=finish_time - start_time;

	cout << ctime(&finish_time) <<" > Tiempo de ejecucion: "<< delay <<" segundos..."<< "\n";

	
	return 0;
}

