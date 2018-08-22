#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

typedef struct a {
    char nome[30];
    char cargo[30];
    double salario;
} Funcionario;

int main(int argc, char const *argv[]){
    //Socket
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    //Problema
    Funcionario f;
    double reajuste;

    // Cria o socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Settando opções do socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bindando as informações de endereço do server ao socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //Tudo pronto, agora o server esta escutando o socket a fim de encontrar uma nova conexão
    if (listen(server_fd, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //Uma vez encontrada a solicitação, server aceita a conexao
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while(1){
        //Le os dados do socket
        read( new_socket , &f, sizeof(Funcionario));

        //Calcula o reajuste
        if (!strcmp(f.cargo,"OPERADOR")) reajuste = 1.2;
        else if (!strcmp(f.cargo,"PROGRAMADOR")) reajuste = 1.18;
        else reajuste = 1;

        f.salario *= reajuste;

        //Envia o salario reajustado
        send(new_socket , &(f.salario) , sizeof(f.salario) , 0 );
    }

    return (0);
}
