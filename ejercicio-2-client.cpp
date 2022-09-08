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

#define SERVER_ADDRESS  "192.168.87.37"     //servidor ip
#define PORT            8080 

//buffers
char buff_tx[100];       
char buff_rx[100];                    


using namespace std;


//funcion main
int main() { 
  int sockfd,i; 
    struct sockaddr_in servaddr;
    string id,opc,key,res;

    cout<<"instrodusca la operacion buscar transaccion (1) borrar (2): "<<endl;
    cin>>opc;

    cout<<"instrodusca el id: "<<endl;
    cin>>id;
    
    cout<<"introdusca el key: "<<endl;
    cin>>key;

    res=opc+"+"+id+"+"+key;
    
    for(i=0;i<res.length();++i){
      buff_tx[i]=res[i];
    }
    printf("%s \n",buff_tx);
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
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) 
    { 
      cout <<"[CLIENTE]: conexion fallida...\n"<<endl;  
        return -1;
    } 
    
   cout <<"[CLIENTE]: conexion exitosa..\n"<<endl; 
  
    //envia las ordenes al servidor
    write(sockfd, buff_tx, sizeof(buff_tx));     
    read(sockfd, buff_rx, sizeof(buff_rx));
    printf("[CLIETE]: Transacion: %s \n", buff_rx);
   
       
    /* cierra socket */
    close(sockfd); 
} 
