// Integrantes do grupo (ordem alfabética):
// Vinícius Matheus Sary de Lima - ViniMSLima
// Nome do grupo no Canvas: RA1 20

#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include "LexerInterface.hpp"

using namespace std;

class NoAST {
public:
    string tipo;             
    string valor;            
    vector<NoAST*> filhos;   

    NoAST(string t, string v) : tipo(t), valor(v) {}

    void adicionarFilho(NoAST* filho) {
        filhos.push_back(filho);
    }

    void imprimirArvore(ostream& out, int nivel = 0) {
        for (int i = 0; i < nivel; ++i) out << "  |";
        out << "-- [" << tipo << "] " << valor << "\n";
        
        for (auto filho : filhos) {
            filho->imprimirArvore(out, nivel + 1);
        }
    }
};

// Declaração da função do Aluno 2
NoAST* gerarArvore(vector<Token> tokens);

#endif