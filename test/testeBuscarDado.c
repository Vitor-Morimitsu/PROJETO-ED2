#include "hashFile.h"

// Arquivo responsável por buscar um dado em uma hash file extensível.

int main()
{
    HashFile novaHash = criarHashFile();
    if(novaHash == NULL) return 1;

    setbuf(stdin, NULL);
    char palavra[32];
    printf("Digite uma palavra que será armazenada no arquivo de dados da Hash File: ");
    fgets(palavra, sizeof(palavra), stdin);

    inserirDadoHashFile(novaHash, palavra);

    int endereco = getEnderecoDiretorioHashFile(novaHash,palavra);
    printf("Endereço do diretório : %d\n", endereco);

    char* dado = getDadoHashfile(novaHash, endereco);
    printf("Dado da hash: %s\n", dado);
}