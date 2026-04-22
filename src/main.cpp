// Integrantes do grupo (ordem alfabética):
// Vinícius Matheus Sary de Lima - ViniMSLima
// Nome do grupo no Canvas: RA1 20

#include <iostream>
#include <string>
#include <fstream>
#include "Fase1_Lexico.hpp"
#include "LexerInterface.hpp"   // Aluno 3
#include "Gramatica.hpp"        // Aluno 1
#include "AST.hpp"              // Aluno 2
#include "GeradorAssembly.hpp"  // Aluno 4

using namespace std;

int main(int argc, char* argv[]) {
    // 1. VALIDAÇÃO DE SEGURANÇA
    if (argc < 2) {
        cout << "\n[ERRO FATAL] Arquivo de codigo-fonte nao informado!" << endl;
        cout << "Modo de uso correto: ./AnalisadorSintatico <nome_do_arquivo.txt>" << endl;
        cout << "Exemplo: ./AnalisadorSintatico teste1.txt\n" << endl;
        return 1; 
    }

    string arquivoCodigo = argv[1]; 
    string arquivoTokens = "tokens_temp_fase1.txt"; 

    cout << "===========================================" << endl;
    cout << " COMPILADOR RPN - PIPELINE COMPLETO" << endl;
    cout << " Lendo arquivo: " << arquivoCodigo << endl;
    cout << "===========================================" << endl;

    // ==========================================
    // ESTEIRA 1: ANÁLISE LÉXICA (Trabalho 1)
    // ==========================================
    cout << "\n>>> INICIANDO FASE 1: ANALISE LEXICA" << endl;
    executarFase1(arquivoCodigo, arquivoTokens); 

    // ==========================================
    // ESTEIRA 2: ANÁLISE SINTÁTICA (Trabalho 2)
    // ==========================================
    cout << "\n>>> INICIANDO FASE 2: ANALISE SINTATICA" << endl;

    cout << "[1] Aluno 1: Construindo Gramatica e Tabela LL(1)..." << endl;
    construirGramatica();
    calcularFirst();
    calcularFollow();
    construirTabelaLL1();

    cout << "[2] Aluno 3: Carregando e adaptando tokens..." << endl;
    vector<Token> tokens = lerTokens(arquivoTokens); 

    if (tokens.empty()) {
        cout << "\n[FALHA] Nenhum token gerado. Verifique o arquivo de entrada." << endl;
        return 1;
    }

    cout << "[3] Aluno 2: Construindo Arvore Sintatica (AST)..." << endl;
    // Alterado de parsear para gerarArvore conforme requisito 7.4 do edital
    NoAST* arvore = gerarArvore(tokens);

    if (arvore != nullptr) {
        cout << "\n[SUCESSO] Codigo compilado sem erros sintaticos!" << endl;
        cout << "AST Gerada no console:\n" << endl;
        
        arvore->imprimirArvore(cout); 

        ofstream arquivoArvore("arvore_sintatica.txt");
        if (arquivoArvore.is_open()) {
            arvore->imprimirArvore(arquivoArvore);
            arquivoArvore.close();
            cout << "\n[+] A Arvore Sintatica foi salva no arquivo 'arvore_sintatica.txt'." << endl;
        }

        // ==========================================
        // ESTEIRA 4: GERAÇÃO DE CÓDIGO (Assembly)
        // ==========================================
        cout << "\n[4] Aluno 4: Gerando Codigo Assembly ARMv7 (Double Precision)..." << endl;
        GeradorAssembly gerador;
        gerador.gerar(arvore, "saida.s");

    } else {
        cout << "\n[FALHA] Compilacao abortada na Fase 2." << endl;
    }

    return 0;
}