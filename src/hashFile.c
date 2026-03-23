#include "hashFile.h"

typedef struct{
    FILE* diretorio;
    FILE* dados;
    int profundidade;
}stHashFile;

HashFile criarHashFile(){
    //criar os arquivos
    FILE* diretorio = fopen("diretorio.bin", "wb");
    if(diretorio == NULL){
        printf("Erro ao abrir o arquivo de diretorio!\n");
    } 

    FILE* dados = fopen("dados.txt", "w");
    if(dados == NULL){
        printf("Erro ao abrir arquivo de dados!\n");
    }

    stHashFile *hash = malloc(sizeof(stHashFile));
    hash->diretorio = diretorio;
    hash->dados = dados;
    hash->profundidade = 0;

    return ((HashFile)hash);
}

void inserirDadoHashFile(HashFile hashFile, char* palavra){
    if(hashFile == NULL){
        printf("Erro ao abrir hashFile em inserirDadosHashFile\n");
        return;
    }

    stHashFile *hash = (HashFile)hashFile;

    /*BUCKET
        d' dado
           dado
           dado
           dado
    */

    char linha[32];
    int i = 0;
    while(fgets(linha[i],sizeof(linha), hash->diretorio) != NULL){
        //pegar o endereco em binario
        int enderecoBinario = fread(&enderecoBinario, sizeof(int), 1 , hash->diretorio);

        //acessar o bucket do endereco no arquivo de dados
        int enderecoBucket = fread(&enderecoBucket, sizeof(int), 1 , hash->dados);

        //ver se está cheio, se não estiver add o item

        //se estiver cheio, extender para um novo bucket

        i++;
    }
}
