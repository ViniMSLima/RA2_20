#include <iostream>
#include "Gramatica.hpp"
#include "LexerInterface.hpp"

using namespace std;

int main() {
    cout << "===========================================" << endl;
    cout << " INICIANDO O COMPILADOR (FASE 2)" << endl;
    cout << "===========================================" << endl;

    // 1. O TRABALHO DO ALUNO 1
    cout << "\n[1] Construindo as regras do Aluno 1..." << endl;
    construirGramatica();
    calcularFirst();
    calcularFollow();
    construirTabelaLL1();
    cout << " -> Tabela LL(1) gerada com sucesso na memoria!" << endl;

    // 2. O TRABALHO DO ALUNO 3
    cout << "\n[2] Acionando o Aluno 3 (Leitor de Tokens)..." << endl;
    vector<Token> meusTokens = lerTokens("tokens_controle.txt");
    
    cout << "\n[!] Tokens traduzidos e prontos para o Aluno 2:" << endl;
    for (const auto& t : meusTokens) {
        cout << "  - [" << t.tipo << "] : " << t.valor << endl;
    }

    cout << "\n===========================================" << endl;
    cout << " TUDO PRONTO PARA O ALUNO 2 (PARSER)" << endl;
    cout << "===========================================" << endl;

    return 0;
}