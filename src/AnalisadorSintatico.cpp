#include "Gramatica.hpp"

// Instanciando a variável global declarada no .hpp
Gramatica gramatica;

void construirGramatica() {
    gramatica["programa"] = { {"PARENTE_ESQ", "START", "PARENTE_DIR", "proximo_bloco"} };
    
    gramatica["proximo_bloco"] = {
        {"PARENTE_ESQ", "corpo_instrucao"},
        {"EPSILON"}
    };

    gramatica["corpo_instrucao"] = {
        {"END", "PARENTE_DIR"},
        {"expressao", "PARENTE_DIR", "proximo_bloco"}
    };

    gramatica["expressao"] = { {"lista_operandos", "operador_final"} };

    gramatica["lista_operandos"] = {
        {"operando", "lista_operandos"},
        {"EPSILON"}
    };

    gramatica["operando"] = {
        {"NUMERO"},
        {"VARIAVEL"},
        {"PARENTE_ESQ", "expressao", "PARENTE_DIR"}
    };

    gramatica["operador_final"] = {
        {"OP_ARITMETICO"}, {"OP_RELACIONAL"}, {"KEY_IF"}, 
        {"KEY_WHILE"}, {"KEY_MEM"}, {"KEY_RES"}
    };
}

bool validarTerminal(string simbolo) {
    return gramatica.find(simbolo) == gramatica.end() && simbolo != "EPSILON";
}