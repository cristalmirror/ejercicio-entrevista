#include <iostream>
#include <string>
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define SERVER_ADDRESS  "192.168.0.3"     //servidor ip
#define PORT            8080 

//buffers
char *buff_tx;       
char *buff_rx;                    


using namespace std;


typedef struct element {
	int key;
	string __orders_id;
        int num_opc,num_trn;
  
}orders_t;

//funcion main
int main() { 
  int sockfd,i; 
    struct sockaddr_in servaddr;
    orders_t aux;

    cout<<"instrodusca la operacion buscar transaccion (1) borrar (2): "<<endl;
    cin>>aux.num_opc;

    cout<<"instrodusca el id: "<<endl;
    cin>>aux.__orders_id;
    
    cout<<"introdusca el key: "<<endl;
    cin>>aux.key;

    FILE *archivo2=fopen("temp2","a+b");
    
    fwrite(&aux,sizeof(orders_t),1,archivo2);
    
    fread(&buff_tx,sizeof(buff_tx),1,archivo2);
    
    fclose(archivo2);
			  
    //crea el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
      cout<<"CLIENTE:la creiacion del socket fallo...\n"<<endl; 
        return -1;  
    } else {
      cout<<"CLIENTE: creacion del socket lista...\n"<<endl; 
    }
    
    
    memset(&servaddr, 0, sizeof(servaddr));

    // confiuguracion de ip y puerto
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr( SERVER_ADDRESS ); 
    servaddr.sin_port = htons(PORT); 
  
    //realiza lla concecion del socket cliente con el socket servidor
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) { 
        cout <<"[CLIENTE]: conexion fallida...\n"<<endl;  
        return -1;
    } 
    
    cout <<"[CLIENTE]: conexion exitosa..\n"<<endl; 
  
    //envia las ordenes al servidor
    write(sockfd, buff_tx, sizeof(buff_tx));     
    read(sockfd, buff_rx, sizeof(buff_rx));

    //desserializa el buffer tx
    FILE *archivo=fopen("temp","a+b");
    
    fwrite(&buff_rx,sizeof(buff_rx),1,archivo);

    fread(&aux,sizeof(orders_t),1,archivo);

    fclose(archivo);
		 
    //imprecion de los datos
    cout<<"[SERVER to CLIENT]: key >> "<<aux.key<<endl;
    cout<<"[SERVER to CLIENT]: orders >> "<<aux.__orders_id<<endl;
    cout<<"[SERVER to CLIENT]: transation number >> "<<aux.num_trn<<endl;
     
    /* cierra el socket */
    close(sockfd); 
} 
