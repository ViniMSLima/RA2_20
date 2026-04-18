#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Classe que representa um pedaço do nosso código
class NoAST {
public:
    string tipo;             // Ex: "PROGRAMA", "COMANDO", "OPERADOR", "NUMERO"
    string valor;            // Ex: "+", "10", "X", "IF"
    vector<NoAST*> filhos;   // Os galhos da árvore (operandos ou sub-comandos)

    // Construtor
    NoAST(string t, string v) : tipo(t), valor(v) {}

    // Função para adicionar um filho
    void adicionarFilho(NoAST* filho) {
        filhos.push_back(filho);
    }

    // Função maravilhosa para desenhar a árvore no terminal e o professor dar nota 10
    void imprimirArvore(int nivel = 0) {
        for (int i = 0; i < nivel; ++i) cout << "  |";
        cout << "-- [" << tipo << "] " << valor << endl;
        
        for (auto filho : filhos) {
            filho->imprimirArvore(nivel + 1);
        }
    }
};

#endif