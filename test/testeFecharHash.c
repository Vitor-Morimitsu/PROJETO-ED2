#include "hashFile.h"

// Arquivo responsável por testar o fechamento de uma hash file extensível

int main()
{
    HashFile novaHash = criarHashFile();
    if(novaHash == NULL) return 1;

    fecharHashFile(novaHash);
    if(novaHash == NULL) printf("Sucesso ao fechar a hashFile em testeFecharHash!\n");
}