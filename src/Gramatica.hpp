// Integrantes do grupo (ordem alfabética):
// Vinícius Matheus Sary de Lima - ViniMSLima
// Nome do grupo no Canvas: RA1 20

#ifndef GRAMATICA_HPP
#define GRAMATICA_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

using namespace std;

// Definição do tipo da Gramática
typedef map<string, vector<vector<string>>> Gramatica;

// A palavra 'extern' avisa o C++: "Essas variáveis existem em algum .cpp por aí, confia!"
extern Gramatica gramatica;
extern map<string, set<string>> conjuntosFirst;
extern map<string, set<string>> conjuntosFollow;
extern map<string, map<string, vector<string>>> tabelaLL1;


// Declaração das assinaturas das funções
void construirGramatica();
bool validarTerminal(string simbolo);
set<string> buscarFirst(string simbolo);
void calcularFirst();
void calcularFollow();
void construirTabelaLL1();
#endif
