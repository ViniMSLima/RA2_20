#ifndef LEXER_INTERFACE_HPP
#define LEXER_INTERFACE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Estrutura padrão do Token
struct Token {
    string tipo;   // Ex: NUMERO, PARENTE_ESQ, KEY_IF, OP_RELACIONAL
    string valor;  // Ex: 10, (, IF, >
};

// Declaração da função do Aluno 3
vector<Token> lerTokens(string nomeArquivo);

#endif