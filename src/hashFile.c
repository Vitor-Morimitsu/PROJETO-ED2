#include "hashFile.h"

#define registro_vazio 0
#define registro_valido 1
#define registro_removido 2
#define tamBucket 4
typedef struct{
    int id;
    char palavra[32];
    int valido;//0 - se estiver contendo algum dado 1 - se o dado for removido ou ter o registro vazio
}Registro;

typedef struct{
    int profundidadeLocal;
    Registro registros[tamBucket];
    int qtdRegistro;
}Bucket;
typedef struct{
    FILE* diretorio;
    FILE* dados;
    int profundidade;
}stHashFile;


int getBits(const char* palavra, int profundidade) {
    unsigned int hashVal = 0;
    for (int i = 0; palavra[i] != '\0'; i++) {
        hashVal += (unsigned char)palavra[i];
    }

    int mascara = (1 << profundidade) - 1;
    return hashVal & mascara;
}

HashFile criarHashFile(){
    //criar os arquivos
    FILE* diretorio = fopen("diretorio.bin", "wb");
    if(diretorio == NULL){
        printf("Erro ao abrir o arquivo de diretorio!\n");
        return NULL;
    } 

    FILE* dados = fopen("dados.bin", "wb");
    if(dados == NULL){
        fclose(diretorio);
        printf("Erro ao abrir arquivo de dados!\n");
        return NULL;
    }

    stHashFile *hash = malloc(sizeof(stHashFile));
    if(hash == NULL){
        fclose(diretorio);
        fclose(dados);
        return NULL;
    }

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

    stHashFile *hash = (stHashFile*)hashFile;

    //descrobir qual bucket vai receber a palavra
    int offsetBucket = getEnderecoDiretorioHashFile(hashFile,palavra);

    Bucket bucket;

    fseek(hash->dados,offsetBucket,SEEK_SET);

    fread(&bucket,sizeof(Bucket),1,hash->dados);
    if(bucket.qtdRegistro<tamBucket){
        int slot = bucket.qtdRegistro;

        bucket.registros[slot].id = offsetBucket + slot;

        strncpy(bucket.registros[slot].palavra, palavra,31);
        bucket.registros[slot].palavra[31] = '\0';

        bucket.registros[slot].valido = registro_valido;

        bucket.qtdRegistro++;

        fseek(hash->dados,offsetBucket,SEEK_SET);
        fwrite(&bucket,sizeof(Bucket),1,hash->dados);
    }else{
        hash->profundidade++;
        printf("Split foi necessário. Nova profundidade: %d\n", hash->profundidade);
    }
}

void removerDadosHashFile(HashFile hashFile, char* palavra) {
    if (hashFile == NULL) {
        printf("Erro em removerDadosHashFile\n");
        return;
    }

    stHashFile* hash = (stHashFile*)hashFile;

    int offsetBucket = getEnderecoDiretorioHashFile(hashFile, palavra);

    Bucket bucket;
    fseek(hash->dados, offsetBucket, SEEK_SET);
    fread(&bucket, sizeof(Bucket), 1, hash->dados);

    for (int i = 0; i < tamBucket; i++) {
        if (bucket.registros[i].valido != registro_valido) continue;

        if (strcmp(bucket.registros[i].palavra, palavra) == 0) {

            bucket.registros[i].valido = registro_removido;

            bucket.qtdRegistro--;

            fseek(hash->dados, offsetBucket, SEEK_SET);
            fwrite(&bucket, sizeof(Bucket), 1, hash->dados);

            return; 
        }

    }
    printf("Palavra '%s' não encontrada.\n", palavra);
}

void fecharHashFile(HashFile hashFile){
    if(hashFile == NULL){
        printf("Erro em fecharHashFile\n");
        return;
    }

    stHashFile *hash = (stHashFile*)hashFile;

    fclose(hash->diretorio);
    fclose(hash->dados);

    free(hash);
}

int getEnderecoDiretorioHashFile(HashFile hash, char* palavra){
    if(hash == NULL){
        printf("Erro em getEnderecoDiretorioHashFile\n");
        return -1;
    }

    stHashFile *hashFile = (stHashFile*)hash;
    int indice = getBits(palavra,hashFile->profundidade);

    fseek(hashFile->diretorio, sizeof(int) +  (indice * sizeof(int)) ,SEEK_SET);

    int offsetBucket = 0;

    fread(&offsetBucket, sizeof(int),1,hashFile->diretorio);
    return offsetBucket;
    
}

char* getDadoHashFile(HashFile hashFile, int enderecoDiretorio) {

    if (hashFile == NULL) return NULL;

    stHashFile* hash = (stHashFile*)hashFile;

    fseek(hash->diretorio,
          sizeof(int) + (enderecoDiretorio * sizeof(int)),
          SEEK_SET);

    int offsetBucket = 0;
    fread(&offsetBucket, sizeof(int), 1, hash->diretorio);

    Bucket bucket;
    fseek(hash->dados, offsetBucket, SEEK_SET);
    fread(&bucket, sizeof(Bucket), 1, hash->dados);

    for (int i = 0; i < tamBucket; i++) {

        if (bucket.registros[i].valido == registro_valido) {
            char* resultado = malloc(32);
            if (resultado == NULL) return NULL;
            strncpy(resultado, bucket.registros[i].palavra, 31);
            resultado[31] = '\0';

            return resultado; 
        }
    }

    return NULL;
}