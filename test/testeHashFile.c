#include "unity.h"
#include "hashFile.h"

void setUp(void) {
    remove("diretorio.bin");
    remove("dados.bin");
}

void tearDown(void) {
    remove("diretorio.bin");
    remove("dados.bin");
}

void test_criarHashFile_deveRetornarNaoNulo(void) {
    HashFile hf = criarHashFile();

    // Se malloc ou fopen falharem, retorna NULL — isso não deve ocorrer
    TEST_ASSERT_NOT_NULL(hf);

    fecharHashFile(hf);
}

void test_criarHashFile_devePoderSerFechadaSemCrash(void) {
    HashFile hf = criarHashFile();
    TEST_ASSERT_NOT_NULL(hf);

    // fecharHashFile não deve causar crash (fclose + free corretos)
    fecharHashFile(hf);
    // Se chegou aqui sem segfault, o teste passa
}

void test_fecharHashFile_comNullNaoDeveCrash(void) {
    // Caminho infeliz: fecharHashFile(NULL) deve apenas printar erro e retornar
    // Não deve causar segmentation fault
    fecharHashFile(NULL);
}

void test_inserir_comHashNulo_naoDeveCrash(void) {
    // Caminho infeliz: passar NULL como hashFile
    // A função deve checar e retornar sem crash
    inserirDadoHashFile(NULL, "teste");
}

void test_inserir_umaPalavra_devePersistirNoDisco(void) {
    HashFile hf = criarHashFile();
    TEST_ASSERT_NOT_NULL(hf);

    inserirDadoHashFile(hf, "gato");

    // Recupera via endereço 0 do diretório
    char* resultado = getDadoHashFile(hf, 0);

    // Deve ter encontrado algum dado válido no bucket
    TEST_ASSERT_NOT_NULL(resultado);

    free(resultado);
    fecharHashFile(hf);
}

void test_inserir_palavraLonga_naoDeveEstourarBuffer(void) {
    HashFile hf = criarHashFile();
    TEST_ASSERT_NOT_NULL(hf);

    // Palavra com 40 chars — campo suporta apenas 31 + '\0'
    // strncpy deve truncar sem estouro
    inserirDadoHashFile(hf, "abcdefghijklmnopqrstuvwxyz1234567890XYZ");

    fecharHashFile(hf);
    // Se chegou aqui sem crash ou sanitizer alert, passou
}

void test_inserir_ateLimiteDosBucket_semSplit(void) {
    HashFile hf = criarHashFile();
    TEST_ASSERT_NOT_NULL(hf);

    // tamBucket = 4: inserir 4 palavras que caiam no mesmo bucket
    // (palavras com mesmo hash módulo 2^profundidade)
    // Com profundidade 0, todos vão para o mesmo bucket
    inserirDadoHashFile(hf, "a");
    inserirDadoHashFile(hf, "b");
    inserirDadoHashFile(hf, "c");
    inserirDadoHashFile(hf, "d");

    // Nenhum crash esperado — bucket comporta 4 registros
    fecharHashFile(hf);
}

void test_remover_comHashNulo_naoDeveCrash(void) {
    removerDadosHashFile(NULL, "qualquer");
}

void test_remover_palavraInexistente_naoDeveCrash(void) {
    HashFile hf = criarHashFile();
    TEST_ASSERT_NOT_NULL(hf);

    // Tenta remover algo que nunca foi inserido
    // Deve imprimir "não encontrada" e retornar sem crash
    removerDadosHashFile(hf, "fantasma");

    fecharHashFile(hf);
}

void test_remover_palavraExistente_deveReduzirQtdRegistro(void) {
    HashFile hf = criarHashFile();
    TEST_ASSERT_NOT_NULL(hf);

    inserirDadoHashFile(hf, "gato");

    // Após inserção, getDadoHashFile deve achar o dado
    char* antes = getDadoHashFile(hf, 0);
    TEST_ASSERT_NOT_NULL(antes);
    free(antes);

    removerDadosHashFile(hf, "gato");

    // Após remoção lógica, o slot fica marcado como removido
    // getDadoHashFile não deve mais retornar "gato"
    // (retorna NULL se não houver mais registros válidos)
    char* depois = getDadoHashFile(hf, 0);
    TEST_ASSERT_NULL(depois);

    fecharHashFile(hf);
}

void test_getEndereco_comHashNulo_deveRetornarMenosUm(void) {
    // Contrato: retorna -1 em caso de erro
    int resultado = getEnderecoDiretorioHashFile(NULL, "palavra");
    TEST_ASSERT_EQUAL_INT(-1, resultado);
}

void test_getEndereco_deveRetornarOffsetNaoNegativo(void) {
    HashFile hf = criarHashFile();
    TEST_ASSERT_NOT_NULL(hf);

    // Um offset válido de bucket nunca é negativo
    int offset = getEnderecoDiretorioHashFile(hf, "gato");
    TEST_ASSERT_GREATER_OR_EQUAL_INT(0, offset);

    fecharHashFile(hf);
}

void test_getDado_comHashNulo_deveRetornarNull(void) {
    char* resultado = getDadoHashFile(NULL, 0);
    TEST_ASSERT_NULL(resultado);
}

void test_getDado_bucketVazio_deveRetornarNull(void) {
    HashFile hf = criarHashFile();
    TEST_ASSERT_NOT_NULL(hf);

    // Nenhuma inserção — bucket está vazio
    char* resultado = getDadoHashFile(hf, 0);
    TEST_ASSERT_NULL(resultado);

    fecharHashFile(hf);
}

void test_getDado_aposInsercao_deveRetornarStringValida(void) {
    HashFile hf = criarHashFile();
    TEST_ASSERT_NOT_NULL(hf);

    inserirDadoHashFile(hf, "cachorro");

    char* resultado = getDadoHashFile(hf, 0);
    TEST_ASSERT_NOT_NULL(resultado);

    // O retorno deve ser uma string válida (não vazia)
    TEST_ASSERT_TRUE(strlen(resultado) > 0);

    free(resultado); // caller é responsável por liberar
    fecharHashFile(hf);
}

void test_getDado_retornoDeveSerCopiaMallocada(void) {
    HashFile hf = criarHashFile();
    TEST_ASSERT_NOT_NULL(hf);

    inserirDadoHashFile(hf, "leao");

    char* resultado = getDadoHashFile(hf, 0);
    TEST_ASSERT_NOT_NULL(resultado);

    // free() não deve crashar — confirma que veio de malloc
    free(resultado);

    fecharHashFile(hf);
}

void test_getBits_profundidadeZero_deveRetornarSempre0(void) {
    // Com profundidade 0: máscara = (1<<0)-1 = 0
    // qualquer palavra & 0 = 0
    TEST_ASSERT_EQUAL_INT(0, getBits("gato",     0));
    TEST_ASSERT_EQUAL_INT(0, getBits("cachorro", 0));
    TEST_ASSERT_EQUAL_INT(0, getBits("leao",     0));
}

void test_getBits_deveRetornarDentroDoIntervalo(void) {
    int profundidade = 3;
    int limite = (1 << profundidade); // 8: índices válidos são 0..7

    // Vários inputs — todos devem cair em [0, 7]
    TEST_ASSERT_TRUE(getBits("gato",     profundidade) >= 0);
    TEST_ASSERT_TRUE(getBits("gato",     profundidade) <  limite);
    TEST_ASSERT_TRUE(getBits("cachorro", profundidade) >= 0);
    TEST_ASSERT_TRUE(getBits("cachorro", profundidade) <  limite);
    TEST_ASSERT_TRUE(getBits("",         profundidade) >= 0);
}

void test_getBits_mesmaPalavra_deveRetornarMesmoIndice(void) {
    // Hash deve ser determinístico
    int r1 = getBits("gato", 2);
    int r2 = getBits("gato", 2);
    TEST_ASSERT_EQUAL_INT(r1, r2);
}

void test_getBits_profundidadeMaior_podeDistinguirMaisPalavras(void) {
    // Com profundidade maior, o espaço de índices cresce
    // Não garante colisão zero, mas verifica que o intervalo está correto
    int p = 4;
    int limite = (1 << p); // 16
    for (int i = 0; i < limite; i++) {
        char palavra[8];
        snprintf(palavra, sizeof(palavra), "p%d", i);
        int idx = getBits(palavra, p);
        TEST_ASSERT_TRUE(idx >= 0 && idx < limite);
    }
}

int main(void) {
    UNITY_BEGIN();

    // criarHashFile / fecharHashFile
    RUN_TEST(test_criarHashFile_deveRetornarNaoNulo);
    RUN_TEST(test_criarHashFile_devePoderSerFechadaSemCrash);
    RUN_TEST(test_fecharHashFile_comNullNaoDeveCrash);

    // inserirDadoHashFile
    RUN_TEST(test_inserir_comHashNulo_naoDeveCrash);
    RUN_TEST(test_inserir_umaPalavra_devePersistirNoDisco);
    RUN_TEST(test_inserir_palavraLonga_naoDeveEstourarBuffer);
    RUN_TEST(test_inserir_ateLimiteDosBucket_semSplit);

    // removerDadosHashFile
    RUN_TEST(test_remover_comHashNulo_naoDeveCrash);
    RUN_TEST(test_remover_palavraInexistente_naoDeveCrash);
    RUN_TEST(test_remover_palavraExistente_deveReduzirQtdRegistro);

    // getEnderecoDiretorioHashFile
    RUN_TEST(test_getEndereco_comHashNulo_deveRetornarMenosUm);
    RUN_TEST(test_getEndereco_deveRetornarOffsetNaoNegativo);

    // getDadoHashFile
    RUN_TEST(test_getDado_comHashNulo_deveRetornarNull);
    RUN_TEST(test_getDado_bucketVazio_deveRetornarNull);
    RUN_TEST(test_getDado_aposInsercao_deveRetornarStringValida);
    RUN_TEST(test_getDado_retornoDeveSerCopiaMallocada);

    // getBits
    RUN_TEST(test_getBits_profundidadeZero_deveRetornarSempre0);
    RUN_TEST(test_getBits_deveRetornarDentroDoIntervalo);
    RUN_TEST(test_getBits_mesmaPalavra_deveRetornarMesmoIndice);
    RUN_TEST(test_getBits_profundidadeMaior_podeDistinguirMaisPalavras);

    return UNITY_END();
}