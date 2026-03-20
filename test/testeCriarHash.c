#include "hashFile.h"
//Arquivo responsável por testar a criação de uma Hash File extensível

int main()
{
    HashFile novaHash = criarHashFile();
    if(novaHash != NULL){
        printf("Hash File criada com sucesso no arquivo de testes!\n");
    }else{
        printf("Erro ao criar a HashFile no arquivo de testes!\n");
    }
}