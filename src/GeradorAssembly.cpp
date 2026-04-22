// Integrantes do grupo (ordem alfabética):
// Vinícius Matheus Sary de Lima - ViniMSLima
// Nome do grupo no Canvas: RA1 20

#include "GeradorAssembly.hpp"
#include <iostream>

GeradorAssembly::GeradorAssembly() : labelCount(0), linhaAtual(0) {}

string GeradorAssembly::criarLabel(string prefixo) {
    return prefixo + "_" + to_string(labelCount++);
}

void GeradorAssembly::gerar(NoAST* raiz, string nomeArquivo) {
    saida.open(nomeArquivo);

    // Constantes padrão para controle
    literais["lit_ZERO"] = 0.0;
    literais["lit_ONE"]  = 1.0;

    // Cabeçalho ARMv7
    saida << ".global _start\n";
    saida << "_start:\n";
    saida << "    MOV SP, #0x00800000 @ Inicializa a pilha no topo\n\n";

    // Traduz cada instrução do programa
    for (auto filho : raiz->filhos) {
        saida << "    @ --- INICIO DA LINHA " << linhaAtual << " ---\n";
        gerarCodigo(filho);

        // Salva o resultado desta linha no buffer de histórico (para RES)
        saida << "    VPOP {D0} @ Pega o resultado da expressao\n";
        saida << "    LDR R0, =buffer_resultados\n";
        saida << "    MOV R1, #" << (linhaAtual * 8) << " @ Offset (linha * 8 bytes)\n";
        saida << "    ADD R0, R0, R1\n";
        saida << "    VSTR.64 D0, [R0] @ Salva no historico\n";
        saida << "    VPUSH {D0} @ Devolve para a pilha\n\n";

        linhaAtual++;
    }

    saida << "fim:\n    B fim\n\n";

    // Seção de dados
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

    // -------------------------------------------------------
    // NÚMEROS
    // -------------------------------------------------------
    if (no->tipo == "NUMERO") {
        string lab = "lit_" + to_string(literais.size());
        literais[lab] = stod(no->valor);
        saida << "    LDR R0, =" << lab << "\n";
        saida << "    VLDR.64 D0, [R0]\n";
        saida << "    VPUSH {D0}\n";
    }

    // -------------------------------------------------------
    // VARIÁVEIS (lê da memória)
    // -------------------------------------------------------
    else if (no->tipo == "VARIAVEL") {
        memorias[no->valor] = 1;
        saida << "    LDR R0, =var_" << no->valor << "\n";
        saida << "    VLDR.64 D0, [R0]\n";
        saida << "    VPUSH {D0}\n";
    }

    // -------------------------------------------------------
    // OPERADORES ARITMÉTICOS
    // D0 = operando esquerdo, D1 = operando direito
    // -------------------------------------------------------
    else if (no->tipo == "OP_ARITMETICO") {
        gerarCodigo(no->filhos[0]);
        gerarCodigo(no->filhos[1]);
        saida << "    VPOP {D1} @ Segundo operando\n";
        saida << "    VPOP {D0} @ Primeiro operando\n";

        if (no->valor == "+") {
            saida << "    VADD.F64 D0, D0, D1\n";
            saida << "    VPUSH {D0}\n";
        }
        else if (no->valor == "-") {
            saida << "    VSUB.F64 D0, D0, D1\n";
            saida << "    VPUSH {D0}\n";
        }
        else if (no->valor == "*") {
            saida << "    VMUL.F64 D0, D0, D1\n";
            saida << "    VPUSH {D0}\n";
        }
        else if (no->valor == "|") {
            // Divisão real
            saida << "    VDIV.F64 D0, D0, D1\n";
            saida << "    VPUSH {D0}\n";
        }
        else if (no->valor == "/") {
            // Divisão inteira
            saida << "    VCVT.S32.F64 S0, D0 @ D0 -> int\n";
            saida << "    VCVT.S32.F64 S2, D1 @ D1 -> int\n";
            saida << "    VMOV R0, S0\n";
            saida << "    VMOV R1, S2\n";
            saida << "    SDIV R0, R0, R1      @ divisao inteira\n";
            saida << "    VMOV S0, R0\n";
            saida << "    VCVT.F64.S32 D0, S0  @ resultado -> double\n";
            saida << "    VPUSH {D0}\n";
        }
        else if (no->valor == "%") {
            // Resto inteiro: rem = dividendo - (dividendo/divisor)*divisor
            saida << "    VCVT.S32.F64 S0, D0  @ dividendo -> int\n";
            saida << "    VCVT.S32.F64 S2, D1  @ divisor -> int\n";
            saida << "    VMOV R0, S0\n";
            saida << "    VMOV R1, S2\n";
            saida << "    SDIV R2, R0, R1      @ quociente\n";
            saida << "    MUL  R3, R2, R1      @ quociente * divisor\n";
            saida << "    SUB  R0, R0, R3      @ resto\n";
            saida << "    VMOV S0, R0\n";
            saida << "    VCVT.F64.S32 D0, S0  @ resto -> double\n";
            saida << "    VPUSH {D0}\n";
        }
        else if (no->valor == "^") {
            // Potenciação: loop de multiplicação
            string lLoop = criarLabel("L_POW_LOOP");
            string lEnd  = criarLabel("L_POW_END");
            saida << "    VCVT.S32.F64 S4, D1  @ expoente -> int\n";
            saida << "    VMOV R0, S4           @ R0 = contador\n";
            saida << "    LDR R1, =lit_ONE\n";
            saida << "    VLDR.64 D2, [R1]      @ D2 = acumulador (1.0)\n";
            saida << lLoop << ":\n";
            saida << "    CMP R0, #0\n";
            saida << "    BLE " << lEnd << "\n";
            saida << "    VMUL.F64 D2, D2, D0  @ acumulador *= base\n";
            saida << "    SUB R0, R0, #1\n";
            saida << "    B " << lLoop << "\n";
            saida << lEnd << ":\n";
            saida << "    VPUSH {D2}\n";
        }
    }

    // -------------------------------------------------------
    // OPERADORES RELACIONAIS
    // Empurra 1.0 (verdadeiro) ou 0.0 (falso)
    // -------------------------------------------------------
    else if (no->tipo == "OP_RELACIONAL") {
        gerarCodigo(no->filhos[0]);
        gerarCodigo(no->filhos[1]);
        saida << "    VPOP {D1} @ Segundo operando\n";
        saida << "    VPOP {D0} @ Primeiro operando\n";
        saida << "    VCMP.F64 D0, D1\n";
        saida << "    VMRS APSR_nzcv, FPSCR\n";

        string lTrue = criarLabel("L_REL_TRUE");
        string lFim  = criarLabel("L_REL_END");

        if      (no->valor == ">") saida << "    BGT " << lTrue << "\n";
        else if (no->valor == "<") saida << "    BLT " << lTrue << "\n";
        else if (no->valor == "=") saida << "    BEQ " << lTrue << "\n";

        saida << "    LDR R0, =lit_ZERO\n";
        saida << "    VLDR.64 D0, [R0]\n";
        saida << "    VPUSH {D0}           @ falso: empurra 0.0\n";
        saida << "    B " << lFim << "\n";
        saida << lTrue << ":\n";
        saida << "    LDR R0, =lit_ONE\n";
        saida << "    VLDR.64 D0, [R0]\n";
        saida << "    VPUSH {D0}           @ verdadeiro: empurra 1.0\n";
        saida << lFim << ":\n";
    }

    // -------------------------------------------------------
    // IF
    // filhos[0] = condição, filhos[1] = ação
    //
    // Estrutura da pilha VFP:
    //   - condição deixa 1.0 ou 0.0 na pilha
    //   - VPOP D1 consome esse valor
    //   - se verdadeiro, executa ação; ação pode deixar valor na pilha
    //   - VPOP descarta retorno da ação (efeito colateral já aplicado)
    //   - ao final, SEMPRE empurra 0.0 como valor de retorno do bloco
    // -------------------------------------------------------
    else if (no->tipo == "KEY_IF") {
        string lFim = criarLabel("L_END_IF");

        gerarCodigo(no->filhos[0]);          // gera condição
        saida << "    VPOP {D1} @ Resultado da condicao\n";
        saida << "    LDR R0, =lit_ZERO\n";
        saida << "    VLDR.64 D0, [R0]\n";
        saida << "    VCMP.F64 D1, D0\n";
        saida << "    VMRS APSR_nzcv, FPSCR\n";
        saida << "    BEQ " << lFim << " @ Se falso, pula acao\n";

        gerarCodigo(no->filhos[1]);          // gera ação
        saida << "    VPOP {D0} @ Descarta retorno da acao\n";

        saida << lFim << ":\n";
        // Empurra 0.0 como valor de retorno do IF (necessário para o loop em gerar())
        saida << "    LDR R0, =lit_ZERO\n";
        saida << "    VLDR.64 D0, [R0]\n";
        saida << "    VPUSH {D0} @ Valor de retorno do IF\n";
    }

    // -------------------------------------------------------
    // WHILE
    // filhos[0] = condição, filhos[1] = ação
    //
    // A cada iteração:
    //   1. Avalia condição → empurra 0.0 ou 1.0
    //   2. VPOP D1 consome resultado
    //   3. Se D1 == 0, sai
    //   4. Executa ação → pode deixar valor na pilha
    //   5. VPOP descarta retorno da ação (limpa pilha antes de reiniciar)
    //   6. Volta ao início
    // Ao sair, empurra 0.0 como valor de retorno do bloco.
    // -------------------------------------------------------
    else if (no->tipo == "KEY_WHILE") {
        string lStart = criarLabel("L_START_WHILE");
        string lEnd   = criarLabel("L_END_WHILE");

        saida << lStart << ":\n";
        gerarCodigo(no->filhos[0]);          // gera condição
        saida << "    VPOP {D1} @ Resultado da condicao\n";
        saida << "    LDR R0, =lit_ZERO\n";
        saida << "    VLDR.64 D0, [R0]\n";
        saida << "    VCMP.F64 D1, D0\n";
        saida << "    VMRS APSR_nzcv, FPSCR\n";
        saida << "    BEQ " << lEnd << " @ Se falso, sai do laco\n";

        gerarCodigo(no->filhos[1]);          // gera ação
        // *** CRÍTICO: descarta retorno da ação antes de reiniciar o loop.
        // Sem este VPOP, cada iteração empilha um valor a mais e
        // o Cpulator sofreria overflow da pilha VFP.
        saida << "    VPOP {D0} @ Descarta retorno da acao (evita acumulo na pilha)\n";
        saida << "    B " << lStart << " @ Reinicia o laco\n";
        saida << lEnd << ":\n";

        // Empurra 0.0 como valor de retorno do WHILE
        saida << "    LDR R0, =lit_ZERO\n";
        saida << "    VLDR.64 D0, [R0]\n";
        saida << "    VPUSH {D0} @ Valor de retorno do WHILE\n";
    }

    // -------------------------------------------------------
    // MEM (armazenamento em variável)
    //
    // Com a correção no ParserAST.cpp, MEM agora tem DOIS filhos:
    //   filhos[0] = expressão cujo resultado será armazenado
    //   filhos[1] = variável de destino (VARIAVEL node)
    //
    // O gerador computa filhos[0], armazena em filhos[1],
    // e devolve o valor para balancear a pilha do loop em gerar().
    // -------------------------------------------------------
    else if (no->tipo == "KEY_MEM") {
        string var = no->filhos[1]->valor;   // variável de destino
        memorias[var] = 1;

        gerarCodigo(no->filhos[0]);          // computa o valor a armazenar
        saida << "    VPOP {D0} @ Pega o valor computado\n";
        saida << "    LDR R0, =var_" << var << "\n";
        saida << "    VSTR.64 D0, [R0] @ Armazena em " << var << "\n";
        saida << "    VPUSH {D0} @ Devolve para balancear a pilha\n";
    }

    // -------------------------------------------------------
    // RES (busca no histórico de resultados)
    // -------------------------------------------------------
    else if (no->tipo == "KEY_RES") {
        int n    = stoi(no->filhos[0]->valor);
        int alvo = linhaAtual - n;

        if (alvo < 0) {
            saida << "    LDR R0, =lit_ZERO @ RES fora do intervalo, retorna 0\n";
            saida << "    VLDR.64 D0, [R0]\n";
        } else {
            saida << "    LDR R0, =buffer_resultados\n";
            saida << "    MOV R1, #" << (alvo * 8) << " @ Offset da linha " << alvo << "\n";
            saida << "    ADD R0, R0, R1\n";
            saida << "    VLDR.64 D0, [R0]\n";
        }
        saida << "    VPUSH {D0}\n";
    }
}