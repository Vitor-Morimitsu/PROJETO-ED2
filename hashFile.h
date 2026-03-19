#ifndef HASHFILE_H
#define HASHFILE_H

#include <stdio.h>
#include <stdlib.h>

/*
    Arquivo .h destinado à implementação e uso de um hash file que contém baldes associados aos endereços binários.
*/

typedef void* HashFile;

/// @brief Cria uma hashFile
/// @return Retorna a hash criada
HashFile criarHashFile(); 

/// @brief Insere um dado com base no endereço de memória dele
/// @param hashFile Hash já existente
void inserirDadoHashFile(HashFile hashFile);

/// @brief Organiza os dados da hashfile
/// @param hashFile Hash já existente
/// @return Hash organizada
HashFile organizarDadosHashFile(HashFile hashFile);

/// @brief Retorna o endereço que determinado dado está no diretório
/// @param hash Hash já existente
/// @param palavra Palavra que será procurada
/// @return 
int getEnderecoDiretorioHashFile(HashFile hash, char* palavra);










#endif