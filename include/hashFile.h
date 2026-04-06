#ifndef HASHFILE_H
#define HASHFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void* HashFile;

/// @brief Cria uma hashFile
/// @return Retorna a hash criada, ou NULL em caso de erro
HashFile criarHashFile();

/// @brief Retorna o índice do diretório para uma dada palavra
/// @param palavra Palavra a ser hasheada
/// @param profundidade Profundidade global atual do diretório
/// @return Índice entre 0 e 2^profundidade - 1
int getBits(const char* palavra, int profundidade);

/// @brief Insere uma palavra na hash file
/// @param hashFile Hash já existente
/// @param palavra Palavra a inserir
void inserirDadoHashFile(HashFile hashFile, char* palavra);

/// @brief Remove uma palavra da hash file
/// @param hashFile Hash já existente
/// @param palavra Palavra a remover
void removerDadosHashFile(HashFile hashFile, char* palavra);

/// @brief Fecha e libera uma hash file
/// @param hashFile Hash file a fechar
void fecharHashFile(HashFile hashFile);

/// @brief Retorna o offset do bucket no arquivo de dados
/// @param hash Hash já existente
/// @param palavra Palavra a ser localizada
/// @return Offset do bucket, ou -1 em erro
int getEnderecoDiretorioHashFile(HashFile hash, char* palavra);

/// @brief Retorna a primeira palavra válida no bucket indicado
/// @param hashFile Hash já existente
/// @param enderecoDiretorio Índice da entrada no diretório
/// @return String alocada com malloc (caller deve liberar), ou NULL
char* getDadoHashFile(HashFile hashFile, int enderecoDiretorio);

#endif
