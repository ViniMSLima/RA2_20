// Integrantes do grupo (ordem alfabética):
// Vinícius Matheus Sary de Lima - ViniMSLima
// Nome do grupo no Canvas: RA1 20

#ifndef GERADOR_ASSEMBLY_HPP
#define GERADOR_ASSEMBLY_HPP

#include "AST.hpp"
#include <fstream>
#include <map>
#include <string>

using namespace std;

class GeradorAssembly {
private:
    ofstream saida;
    int labelCount;
    int proxEndereco;
    int linhaAtual; 
    map<string, double> literais; 
    map<string, int> memorias; 

    string criarLabel(string prefixo);
    void gerarCodigo(NoAST* no);

public:
    GeradorAssembly();
    void gerar(NoAST* raiz, string nomeArquivo);
};

#endif