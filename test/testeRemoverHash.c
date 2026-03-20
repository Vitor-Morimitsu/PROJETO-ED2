#include "hashFile.h"

// Arquivo responsável por testar a remoção de dados de um hash file extensível

int main()
{
    HashFile novaHash = criarHashFile();
    if(novaHash == NULL) printf("Erro ao criar Hash file no arquivo teste de removerHash!\n");

    setbuf(stdin, NULL);
    char palavra[32];
    printf("Digite uma palavra que será armazenada no arquivo de dados da Hash File: ");
    fgets(palavra, sizeof(palavra), stdin);

    inserirDadoHashFile(novaHash, palavra);

    removerDadosHashFile(novaHash, palavra);
}