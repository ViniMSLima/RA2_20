#include "GeradorAssembly.hpp"
#include <iostream>

GeradorAssembly::GeradorAssembly() : labelCount(0), linhaAtual(0) {}

string GeradorAssembly::criarLabel(string prefixo) {
    return prefixo + "_" + to_string(labelCount++);
}

void GeradorAssembly::gerar(NoAST* raiz, string nomeArquivo) {
    saida.open(nomeArquivo);

    // 1. Constantes padrão para controle
    literais["lit_ZERO"] = 0.0;
    literais["lit_ONE"] = 1.0;

    // 2. Cabeçalho ARMv7
    saida << ".global _start\n";
    saida << "_start:\n";
    saida << "    MOV SP, #0x00800000 @ Inicializa a pilha no topo\n\n";

    // 3. Traduzindo cada linha do programa
    for (auto filho : raiz->filhos) {
        saida << "    @ --- INICIO DA LINHA " << linhaAtual << " ---\n";
        gerarCodigo(filho); 

        // MÁGICA DO RES: Salva o resultado final desta linha no buffer
        saida << "    VPOP {D0} @ Pega o resultado da expressao\n";
        saida << "    LDR R0, =buffer_resultados\n";
        saida << "    MOV R1, #" << (linhaAtual * 8) << " @ Offset (linha * 8 bytes)\n";
        saida << "    ADD R0, R0, R1\n";
        saida << "    VSTR.64 D0, [R0] @ Salva no historico\n";
        saida << "    VPUSH {D0} @ Devolve para a pilha (caso a prox. linha use)\n\n";
        
        linhaAtual++;
    }

    saida << "fim:\n    B fim\n\n";

    // 4. SEÇÃO DE DADOS (Variáveis, Constantes e Buffer)
    saida << ".data\n";
    saida << ".align 3\n";
    saida << "buffer_resultados: .space 800 @ Buffer para 100 linhas\n";
    
    for (auto const& [nome, dummy] : memorias) 
        saida << "var_" << nome << ": .double 0.0\n";

    for (auto const& [label, valor] : literais) 
        saida << label << ": .double " << valor << "\n";

    saida.close();
    cout << "[Aluno 4] Assembly gerado com sucesso: " << nomeArquivo << endl;
}

void GeradorAssembly::gerarCodigo(NoAST* no) {
    if (!no) return;

    // --- NÚMEROS ---
    if (no->tipo == "NUMERO") {
        string lab = "lit_" + to_string(literais.size());
        literais[lab] = stod(no->valor);
        saida << "    LDR R0, =" << lab << "\n";
        saida << "    VLDR.64 D0, [R0]\n";
        saida << "    VPUSH {D0}\n";
    }
    // --- VARIÁVEIS (Lê da memória) ---
    else if (no->tipo == "VARIAVEL") {
        memorias[no->valor] = 1;
        saida << "    LDR R0, =var_" << no->valor << "\n";
        saida << "    VLDR.64 D0, [R0]\n";
        saida << "    VPUSH {D0}\n";
    }
    // --- OPERADORES ARITMÉTICOS ---
    else if (no->tipo == "OP_ARITMETICO") {
        gerarCodigo(no->filhos[0]);
        gerarCodigo(no->filhos[1]);
        saida << "    VPOP {D1} @ Segundo operando\n";
        saida << "    VPOP {D0} @ Primeiro operando\n";

        if (no->valor == "+")      saida << "    VADD.F64 D0, D0, D1\n";
        else if (no->valor == "-") saida << "    VSUB.F64 D0, D0, D1\n";
        else if (no->valor == "*") saida << "    VMUL.F64 D0, D0, D1\n";
        else                       saida << "    VDIV.F64 D0, D0, D1\n";
        
        saida << "    VPUSH {D0}\n";
    }
    // --- IF (Tomada de Decisão) ---
    else if (no->tipo == "KEY_IF") {
        string lFim = criarLabel("L_END_IF");
        gerarCodigo(no->filhos[0]); // Condição
        saida << "    VPOP {D1}\n";
        saida << "    LDR R0, =lit_ZERO\n";
        saida << "    VLDR.64 D0, [R0]\n";
        saida << "    VCMP.F64 D1, D0\n";
        saida << "    VMRS APSR_nzcv, FPSCR\n";
        saida << "    BEQ " << lFim << "\n";
        gerarCodigo(no->filhos[1]); // Ação
        saida << lFim << ":\n";
    }
    // --- WHILE (Laço) ---
    else if (no->tipo == "KEY_WHILE") {
        string lStart = criarLabel("L_START_WHILE");
        string lEnd = criarLabel("L_END_WHILE");
        saida << lStart << ":\n";
        gerarCodigo(no->filhos[0]); // Condição
        saida << "    VPOP {D1}\n";
        saida << "    LDR R0, =lit_ZERO\n";
        saida << "    VLDR.64 D0, [R0]\n";
        saida << "    VCMP.F64 D1, D0\n";
        saida << "    VMRS APSR_nzcv, FPSCR\n";
        saida << "    BEQ " << lEnd << "\n";
        gerarCodigo(no->filhos[1]); // Ação
        saida << "    B " << lStart << "\n";
        saida << lEnd << ":\n";
    }
    // --- MEM (Gravação) ---
    else if (no->tipo == "KEY_MEM") {
        string var = no->filhos[0]->valor;
        memorias[var] = 1;
        saida << "    VPOP {D0} @ Pega o valor do topo\n";
        saida << "    LDR R0, =var_" << var << "\n";
        saida << "    VSTR.64 D0, [R0] @ Salva na variavel\n";
    }
    // --- RES (Busca no Histórico) ---
    else if (no->tipo == "KEY_RES") {
        int n = stoi(no->filhos[0]->valor);
        int alvo = linhaAtual - n;
        if (alvo < 0) {
            saida << "    LDR R0, =lit_ZERO\n";
            saida << "    VLDR.64 D0, [R0]\n";
        } else {
            saida << "    LDR R0, =buffer_resultados\n";
            saida << "    MOV R1, #" << (alvo * 8) << "\n";
            saida << "    ADD R0, R0, R1\n";
            saida << "    VLDR.64 D0, [R0]\n";
        }
        saida << "    VPUSH {D0}\n";
    }
}