#include <iostream>
#include <string>
#include "Fase1_Lexico.hpp"
#include "LexerInterface.hpp"   // Aluno 3
#include "Gramatica.hpp"        // Aluno 1
#include "AST.hpp"              // Aluno 2
#include "GeradorAssembly.hpp"  // Aluno 4

using namespace std;

// O (int argc, char* argv[]) é o que permite ler coisas direto do terminal!
int main(int argc, char* argv[]) {
    // 1. VALIDAÇÃO DE SEGURANÇA
    // Se o usuário não digitou o nome do arquivo, a gente avisa e fecha o programa.
    if (argc < 2) {
        cout << "\n[ERRO FATAL] Arquivo de codigo-fonte nao informado!" << endl;
        cout << "Modo de uso correto: ./AnalisadorSintatico <nome_do_arquivo.txt>" << endl;
        cout << "Exemplo: ./AnalisadorSintatico teste1.txt\n" << endl;
        return 1; // Retorna erro pro sistema operacional
    }

    // 2. PEGANDO O ARQUIVO DO TERMINAL
    string arquivoCodigo = argv[1]; // Aqui ele pega o "teste1.txt" que o professor digitou
    string arquivoTokens = "tokens_temp_fase1.txt"; // Arquivo temporário que a Fase 1 vai gerar

    cout << "===========================================" << endl;
    cout << " COMPILADOR RPN - PIPELINE COMPLETO" << endl;
    cout << " Lendo arquivo: " << arquivoCodigo << endl;
    cout << "===========================================" << endl;

    // ==========================================
    // ESTEIRA 1: ANÁLISE LÉXICA (Trabalho 1)
    // ==========================================
    cout << "\n>>> INICIANDO FASE 1: ANALISE LEXICA" << endl;
    // Pega o arquivo do professor e cospe o nosso .txt de tokens interno
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

    // Se a Fase 1 falhou e não gerou tokens, a gente para aqui
    if (tokens.empty()) {
        cout << "\n[FALHA] Nenhum token gerado. Verifique o arquivo de entrada." << endl;
        return 1;
    }

    cout << "[3] Aluno 2: Construindo Arvore Sintatica (AST)..." << endl;
    NoAST* arvore = parsear(tokens);

    if (arvore != nullptr) {
        cout << "\n[SUCESSO] Codigo compilado sem erros sintaticos!" << endl;
        cout << "AST Gerada no console:\n" << endl;
        
        // 1. Imprime na tela (console)
        arvore->imprimirArvore(cout); 

        // 2. Salva no arquivo para o Aluno 4 (e para entregar)
        ofstream arquivoArvore("arvore_sintatica.txt");
        if (arquivoArvore.is_open()) {
            arvore->imprimirArvore(arquivoArvore);
            arquivoArvore.close();
            cout << "\n[+] A Arvore Sintatica foi salva no arquivo 'arvore_sintatica.txt'." << endl;
        }

        GeradorAssembly gerador;
        gerador.gerar(arvore, "saida.s");

    } else {
        cout << "\n[FALHA] Compilacao abortada na Fase 2." << endl;
    }

    return 0;
}