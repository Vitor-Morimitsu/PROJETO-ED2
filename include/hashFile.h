#ifndef HASHFILE_H
#define HASHFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Arquivo .h destinado à implementação e uso de um hash file extensível que contém baldes associados aos endereços binários.
*/

typedef void* HashFile;

/// @brief Cria uma hashFile
/// @return Retorna a hash criada
HashFile criarHashFile(); 

/// @brief Insere um dado com base no endereço de memória dele
/// @param hashFile Hash já existente
void inserirDadoHashFile(HashFile hashFile, char* palavra);

/// @brief Remove algum dado de uma hash file
/// @param hashFile HashFile pré existente
void removerDadosHashFile(HashFile hashFile, char* palavra);

/// @brief Fecha uma hash file existente
/// @param hashFile Hash file pré existente
void fecharHashFile(HashFile hashFile);

/// @brief Organiza os dados da hashfile
/// @param hashFile Hash já existente
/// @return Hash organizada
HashFile organizarDadosHashFile(HashFile hashFile);

/// @brief Retorna o endereço que determinado dado está no diretório
/// @param hash Hash já existente
/// @param palavra Palavra que será procurada
/// @return O endereço do dado
long getEnderecoDiretorioHashFile(HashFile hash, char* palavra);

/// @brief Retorna um dado da Hash File
/// @param hash Hash já existente
/// @param enderecoDiretorio O endereco em binário do diretório
/// @return O dado
char* getDadoHashfile(HashFile hash, int enderecoDiretorio);

#endif