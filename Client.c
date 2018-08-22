#include <unistd.h>
#include <arpa/inet.h>
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
    struct sockaddr_in address;
    int sock = 0;
    struct sockaddr_in serv_addr;

    //Problema
    int op;
    Funcionario f;

    //Cria o socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    //Seta as informações do servidor (Protocolo e porta)
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    /* Converte o endereço IP do servidor de string para binario e armazena
       nas configurações de socket do server*/
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    //Solicita conexão com o servidor (neste momento o servidor precisa estar escutando)
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("\nConnection Failed \n");
        return -1;
    }

    do {
        //Seta as informações do funcionario
        printf ("Digite o nome do funcionario: ");
        setbuf(stdin,NULL);
        scanf("%[^\n]s", f.nome);
        printf ("Digite o cargo do funcionario: ");
        setbuf(stdin,NULL);
        scanf ("%[^\n]s", f.cargo);
        printf ("Digite o salario do funcionario: ");
        setbuf(stdin,NULL);
        scanf ("%lf",&f.salario);

        //Envia as informações do funcinario para o servidor
        send(sock , &f , sizeof(Funcionario) , 0 );

        //Lê o novo salário enviado pelo servidor
        read( sock , &(f.salario), sizeof(f.salario));

        //Printa as informações
        printf ("================================\n");
        printf ("Nome: %s\n", f.nome);
        printf ("Salario reajustado: %lf\n",f.salario);
        printf ("================================\n");

        printf ("Digite 1 para inserir um novo funcionario ou 0 para sair\nSelecione a opcao: ");
        scanf("%d",&op);

    } while (op);

    return (0);
}
