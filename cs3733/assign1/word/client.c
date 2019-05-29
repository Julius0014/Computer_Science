//Julius Jenkins ubh226
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT     8080 
#define MAXLINE 1024
// create socket and return socket file desciptor
int createSocket(void){
    int socketnum;
    if ( (socketnum = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    return socketnum;
}
//Fill server information
struct sockaddr_in fillServerInfo(struct sockaddr_in server,char ipaddress[100],int port){
    server.sin_family = AF_INET;
    if(port == ' '){
        server.sin_port = htons(PORT); 
    }else{
        server.sin_port = htons(port); 
    }
    server.sin_addr.s_addr = inet_addr(ipaddress);
    return server;
}
//send message to server 
int sendMsg(char msg[100],struct sockaddr_in servaddr,int sockfd){
    if (sendto(sockfd,msg, strlen(msg),0,(const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror("ERROR SENDING MESSAGE:");
        exit(-1);
    }else{
        printf("MESSAGE SENT\n");
    }
    return 0;
}
//get info from stardard in
char getinfo(char msg[100],char *buffer){
    printf("%s\n",msg);
    if(fgets(buffer,MAXLINE,stdin)){
        return;
    }else{
        perror("READ ERROR\n");
        exit(-1);
    }
    return;
}
int main(int argc,char *argv[]) {
    int sockfd; 
    struct sockaddr_in   servaddr; 
    sockfd = createSocket();
    char ipaddr[100],port[100],msg[100];
    //Enter Info for server and message to send 
    getinfo("Enter Server Address",ipaddr);
    getinfo("Enter Server Port",port);
    getinfo("Enter Message to Server",msg);
    //strcat(msg,"\000");
    servaddr = fillServerInfo(servaddr,ipaddr,atoi(port));
    sendMsg(msg,servaddr,sockfd);
    close(sockfd); 
    return 0; 
} 

