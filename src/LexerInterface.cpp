#include "LexerInterface.hpp"
#include <fstream>
#include <sstream>

vector<Token> lerTokens(string nomeArquivo) {
    vector<Token> listaGeralTokens;
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cout << "ERRO: O Aluno 3 nao conseguiu abrir o arquivo de tokens: " << nomeArquivo << endl;
        return listaGeralTokens;
    }

    string linha;
    while (getline(arquivo, linha)) {
        // Ignora linhas vazias
        if (linha.empty()) continue;

        // Procura a primeira e a segunda vírgula
        size_t posVirgula1 = linha.find(',');
        size_t posVirgula2 = linha.find(',', posVirgula1 + 1);
        
        if (posVirgula1 == string::npos || posVirgula2 == string::npos) continue;

        string tipoLexer = linha.substr(0, posVirgula1);
        string valorLexer = linha.substr(posVirgula1 + 1, posVirgula2 - posVirgula1 - 1);
        string linhaLexer = linha.substr(posVirgula2 + 1);

        Token t;
        t.valor = valorLexer;
        t.linha = stoi(linhaLexer); // Converte a string da linha para int

        // ==========================================
        // ADAPTER: Traduzindo Fase 1 para Fase 2
        // ==========================================
        
        if (tipoLexer == "PARENTE") {
            if (valorLexer == "(") t.tipo = "PARENTE_ESQ";
            else if (valorLexer == ")") t.tipo = "PARENTE_DIR";
        } 
        else if (tipoLexer == "NUMERO") {
            t.tipo = "NUMERO";
        } 
        else if (tipoLexer == "VARIAVEL") {
            // Correção caso a Fase 1 tenha classificado palavras reservadas como variável
            if (valorLexer == "MEM") t.tipo = "KEY_MEM";
            else if (valorLexer == "START") t.tipo = "START";
            else if (valorLexer == "END") t.tipo = "END";
            else t.tipo = "VARIAVEL";
        } 
        else if (tipoLexer == "OPERADOR") {
            // Separa os relacionais dos aritméticos
            if (valorLexer == ">" || valorLexer == "<" || valorLexer == "=") {
                t.tipo = "OP_RELACIONAL";
            } else {
                t.tipo = "OP_ARITMETICO";
            }
        } 
        else if (tipoLexer == "KEYWORD") {
            if (valorLexer == "RES") t.tipo = "KEY_RES";
            else if (valorLexer == "MEM") t.tipo = "KEY_MEM";
            else if (valorLexer == "IF") t.tipo = "KEY_IF";
            else if (valorLexer == "WHILE") t.tipo = "KEY_WHILE";
            else if (valorLexer == "START") t.tipo = "START";
            else if (valorLexer == "END") t.tipo = "END";
        } 
        else {
            // Fallback caso venha algo não mapeado
            t.tipo = tipoLexer;
        }

        listaGeralTokens.push_back(t);
    }

    arquivo.close();
    cout << "[Aluno 3] Arquivo lido com sucesso! " << listaGeralTokens.size() << " tokens traduzidos para a Fase 2." << endl;

    return listaGeralTokens;
}