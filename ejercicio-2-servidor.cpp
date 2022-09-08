
//librerias usadas para trabajar la tabla de hash
#include<iostream>
#include<list>
#include<string>
#include<ctime>
#include<stdlib.h>

//librereias usadas para el socket de red

#include<unistd.h>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

//deteccion de errores para la conexion
#include<errno.h>
#include <stdio.h> 
#include <string.h>

#define SERV_PORT       8080               //puerto de comunicacion 
#define SERV_HOST_ADDR "192.168.87.37"     //ipV4
#define BUF_SIZE        100                //tamanio de los buffeers
#define BACKLOG         100  

using namespace std;


typedef struct element {
	int key;
	string __orders_id;
	int num_trn;
}orders_t;

class TablaHash {
// tamanio de la tabla(numero de elementos)
	int tamanio;
//  listas de los slots
	list<orders_t> *tabla;
public:

	TablaHash(int _tam);
	int funcion_hash(orders_t elem);
	void aniadir_elemento(orders_t elem);
	orders_t recuperar_elemento(orders_t elem);
	void eliminar_elemento(orders_t elem);
	
};

//defino el tamaño del hash que se usa
TablaHash::TablaHash(int _tam){
	tamanio = _tam;
	tabla=new list<orders_t>[tamanio]; 
}

//determina la posicion dentro de la tabla
int TablaHash::funcion_hash(orders_t elem){

	return (elem.key % tamanio);

}
//añade los elementos a la tabla
void TablaHash::aniadir_elemento(orders_t elem){

	int index_slot=funcion_hash(elem);
	tabla[index_slot].push_back(elem);

	
	
}

//elimina un elemento de la tabla de hash
void TablaHash::eliminar_elemento(orders_t elem){

	int index_slot=funcion_hash(elem);

	list<orders_t>:: iterator i;

	for(i = tabla[index_slot].begin(); i!=tabla[index_slot].end(); ++i){
		if((i->key==elem.key)&&(i->__orders_id==elem.__orders_id)) break;
	}

	if (i!=tabla[index_slot].end()) {
		tabla[index_slot].erase(i);
	}else{
		cout <<"la operacion no se pudo realizar o no existe la orden"<< "\n";

	}
	
}

//recupera un elemento para ser enviado al cliente

orders_t TablaHash::recuperar_elemento(orders_t elem){
	
	int index_slot = funcion_hash(elem);
	orders_t garbage;
	
	list<orders_t>:: iterator i;
	
	for (i = tabla[index_slot].begin(); i != tabla[index_slot].end(); ++i) {
		
	  if((i->key==elem.key)&&(i->__orders_id==elem.__orders_id)){
	            garbage = *i;
		    goto salto;   
	      }
	}

       if (i==tabla[index_slot].end()) {
	 
		cout <<"la operacion no se pudo realizar o no existe la orden"<< "\n";
		garbage.key=0;
		garbage.__orders_id="null";
	}

       salto:
       return garbage;
}


//generador de ordenes falsas

orders_t gen_order_uuid(int n){

	orders_t elem;
	char hex_char[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	int i=0;
	string id;
	       
	for (i = 0; i < 8; ++i) {
		elem.__orders_id=elem.__orders_id + hex_char[rand()%16];
	}
	i=0;
	elem.__orders_id=elem.__orders_id + '-';
		
	for (i = 0; i < 4; ++i) {
		elem.__orders_id=elem.__orders_id + hex_char[rand()%16];
	}
	i=0;
	elem.__orders_id=elem.__orders_id + '-';

	for (i = 0; i < 4; ++i) {
		elem.__orders_id=elem.__orders_id + hex_char[rand()%16];
	}
	i=0;
	elem.__orders_id=elem.__orders_id + '-';
		
	for (i = 0; i < 4; ++i) {
		elem.__orders_id=elem.__orders_id + hex_char[rand()%16];
	}
	i=0;
	elem.__orders_id=elem.__orders_id + '-';

	for (i = 0; i < 12; ++i) {
		elem.__orders_id=elem.__orders_id + hex_char[rand()%16];
	}

	elem.key=rand() % 100;
	elem.num_trn=rand() % 27000000 + 20000000;
	

	if (n<10) {
		
		cout <<"["<< elem.key <<"] ["<< elem.__orders_id <<"]\n";
		
	}
	
	return elem;
	
}




// funcion pricipal
int main() {

        //definicion de elementos del socket

        int sockfd, connfd; //socket en escuchar y socket descriptor  
	unsigned int len; 
	struct sockaddr_in servaddr,client;

	int len_rx,lan_tx=0,cntr=0;
        char buff_tx[BUF_SIZE],buff_rx[BUF_SIZE],*aux2;
  
  //definicion de los elementos de la tabla y de la tabla
        orders_t el,aux,aux1;
	TablaHash ObjTabla(5000);

	for (int i = 0; i < 1000000; ++i) {
	       
		el=gen_order_uuid(i);
		ObjTabla.aniadir_elemento(el);
		       
	}
        //cracion del socket
         
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1)  { 
	        fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
		return -1;
	} else {
	        cout<<"[SERVER]: Socket successfully created.."<<endl; 
	}

//limpia la memmoria 
	memset(&servaddr, 0, sizeof(servaddr));
  
    //asigna los puertos y la ip
	servaddr.sin_family      = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); 
	servaddr.sin_port        = htons(SERV_PORT); 
    
    
    //unifica todos los elementos del socket
	 if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
	          fprintf(stderr, "[SERVER-error]: socket bind failed. %d: %s \n", errno, strerror( errno ));
		  return -1;
	  } else {
	          cout<<"[SERVER]: Socket successfully binded "<<endl;
	  }
  
    //escucha
	if ((listen(sockfd, BACKLOG)) != 0) { 
	        fprintf(stderr, "[SERVER-error]: socket listen failed. %d: %s \n", errno, strerror( errno ));
		return -1;
	} else {
	        cout<<"[SERVER]: Listening on SERV_PORT "<< ntohs(servaddr.sin_port) <<endl; 
	}
    
	len = sizeof(client); 
  
      //se crea un bucle para acepatar la comunicacion y estar escuchando la red
	while(1) {
	  connfd = accept(sockfd, (struct sockaddr *)&client, &len); 
	  if (connfd < 0) { 
                  fprintf(stderr, "[SERVER-error]: connection not accepted. %d: %s \n", errno, strerror( errno ));
		  return -1;
	  } else {              
            while(1) {  
                //esta a la espera de los mensajes del cliente
                  len_rx = read(connfd, buff_rx, sizeof(buff_rx));

		  string aux1s="",aux2s="";
		  char combert[7];
		  for (int i = 0; i < strlen(buff_rx); ++i) {//realiza la cobercion del mensaje para las operaciones

			aux1s=aux1s + buff_rx[i];

	      
		  }
		 
		  for(int i=39; i<strlen(buff_rx);++i) {

		    aux2s=aux2s + buff_rx[i];
		    
		  }  
		  
       		  cout<<aux2s<<endl;
		  char opc=buff_rx[0];
		  aux1s.erase(aux1s.begin()+1);
		  aux1s.erase(aux1s.begin()+1);
		  aux1s.erase(aux1s.end()-1);
		  aux1s.erase(aux1s.end()-1);
		  aux1s.erase(aux1s.end()-1);
		  string::size_type sz;
		  aux.__orders_id=aux1s;
		  aux.key= stoi (aux2s,&sz);
		  cout<<aux.key<<endl;
		  //se seleciona la operacion
		  if (opc=='1') {
		    
		          aux1=ObjTabla.recuperar_elemento(aux);

			  for (int i = 0; i < aux1.__orders_id.length(); ++i) {

				buff_tx[i]=aux1.__orders_id[i];
				
			  }
			  
		  } else {
		          ObjTabla.eliminar_elemento(aux);
			  
		  }
                
                  if(len_rx == -1) {
		        fprintf(stderr, "[SERVER-error]: connfd cannot be read. %d: %s \n", errno, strerror( errno ));
                  }
                  else if(len_rx == 0) {
		          cout<<"[SERVER]: client socket closed"<<endl; 
			  close(connfd);
			  break; 
		  } else {
			//envia mensajes al cliente como respuesta	    
		          write(connfd, buff_tx, strlen(buff_tx));
			  cout<<"[SERVER]: "<< buff_rx<<endl;
                }            
            }  
        }                      
    }    

	  
}

