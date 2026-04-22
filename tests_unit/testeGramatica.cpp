// Integrantes do grupo (ordem alfabética):
// Vinícius Matheus Sary de Lima - ViniMSLima
// Nome do grupo no Canvas: RA1 20

#include "Gramatica.hpp"
#include <stack>

void executarTeste(string nomeTeste, set<string> esperado, set<string> atual) {
    if (esperado == atual) {
        cout << "[PASSOU] \033[1;32m" << nomeTeste << "\033[0m" << endl;
    } else {
        cout << "[FALHOU] \033[1;31m" << nomeTeste << "\033[0m" << endl;
        
        cout << "  Esperado: { ";
        for (const auto& s : esperado) cout << s << " ";
        cout << "}" << endl;
        
        cout << "  Recebido: { ";
        for (const auto& s : atual) cout << s << " ";
        cout << "}" << endl;
    }
}

void rodarTestesFirst() {
    cout << "--- INICIANDO TESTES UNITARIOS: CONJUNTOS FIRST ---" << endl;

    construirGramatica();
    calcularFirst();

    executarTeste("FIRST de Terminal (NUMERO)", {"NUMERO"}, buscarFirst("NUMERO"));
    executarTeste("FIRST(programa)", {"PARENTE_ESQ"}, conjuntosFirst["programa"]);
    executarTeste("FIRST(operador_final)", {"KEY_IF", "KEY_MEM", "KEY_RES", "KEY_WHILE", "OP_ARITMETICO", "OP_RELACIONAL"}, conjuntosFirst["operador_final"]);
    executarTeste("FIRST(expressao)", {"KEY_IF", "KEY_MEM", "KEY_RES", "KEY_WHILE", "NUMERO", "OP_ARITMETICO", "OP_RELACIONAL", "PARENTE_ESQ", "VARIAVEL"}, conjuntosFirst["expressao"]);

    cout << "---------------------------------------------------" << endl;
}

void rodarTestesFollow() {
    cout << "--- INICIANDO TESTES UNITARIOS: CONJUNTOS FOLLOW ---" << endl;

    // Calcula o Follow (ele precisa que o First já esteja calculado)
    calcularFollow();

    // Teste 1: Regra Inicial
    executarTeste("FOLLOW(programa)", {"EOF"}, conjuntosFollow["programa"]);

    // Teste 2: Herdando o EOF
    executarTeste("FOLLOW(proximo_bloco)", {"EOF"}, conjuntosFollow["proximo_bloco"]);

    // Teste 3: Fechamento de expressão (deve puxar o parêntese direito)
    executarTeste("FOLLOW(expressao)", {"PARENTE_DIR"}, conjuntosFollow["expressao"]);

    // Teste 4: O mais complexo (FOLLOW de lista_operandos pega o FIRST de operador_final)
    executarTeste("FOLLOW(lista_operandos)", 
                  {"KEY_IF", "KEY_MEM", "KEY_RES", "KEY_WHILE", "OP_ARITMETICO", "OP_RELACIONAL"}, 
                  conjuntosFollow["lista_operandos"]);

    cout << "----------------------------------------------------" << endl;
}

void rodarTesteTabela() {
    cout << "--- INICIANDO CONSTRUCAO DA TABELA LL(1) ---" << endl;
    
    construirTabelaLL1();
    
    // Se chegou até aqui sem dar os "cout" de Erro Fatal, a tabela está limpa!
    // Vamos fazer um teste rápido cruzando uma linha e coluna que sabemos o resultado:
    vector<string> regraEsperada = {"lista_operandos", "operador_final"};
    
    if (tabelaLL1["expressao"]["NUMERO"] == regraEsperada) {
        cout << "[PASSOU] \033[1;32mTabela LL(1) construida sem conflitos e validada!\033[0m" << endl;
    } else {
        cout << "[FALHOU] \033[1;31mErro na montagem da Tabela LL(1)!\033[0m" << endl;
    }
    cout << "--------------------------------------------" << endl;
}

// Simulador LL(1) puro: Retorna true se a sintaxe for válida, false se der erro.
bool simularParserLL1(vector<string> tokens) {
    stack<string> pilha;
    
    // Regra de Ouro do LL(1): Empilha o EOF e depois o Símbolo Inicial
    pilha.push("EOF");
    pilha.push("programa"); 

    // Adiciona o EOF no final da fita de tokens para o parser saber a hora de parar
    tokens.push_back("EOF");
    int ponteiro = 0;

    while (!pilha.empty()) {
        string topo = pilha.top();
        string tokenAtual = tokens[ponteiro];

        // Se o topo for EPSILON, ele simplesmente some (desempilha)
        if (topo == "EPSILON") {
            pilha.pop();
            continue;
        }

        // Se o topo for um Terminal ou EOF, tem que casar exatamente com a fita!
        if (validarTerminal(topo) || topo == "EOF") {
            if (topo == tokenAtual) {
                pilha.pop();   // Deu match! Tira da pilha...
                ponteiro++;    // ...e avança a fita!
            } else {
                return false; // ERRO SINTÁTICO: O token não casou.
            }
        } 
        // Se for Não-Terminal, pergunta pro nosso GPS (A Tabela LL1)
        else {
            vector<string> regra = tabelaLL1[topo][tokenAtual];
            
            // Se a célula estiver vazia, o parser não sabe o que fazer (Erro Sintático)
            if (regra.empty()) {
                return false; 
            }

            pilha.pop(); // Tira o Não-Terminal da pilha

            // Empilha a nova regra (sempre de trás pra frente no LL1!)
            if (regra[0] != "EPSILON") {
                for (int i = regra.size() - 1; i >= 0; i--) {
                    pilha.push(regra[i]);
                }
            }
        }
    }
    
    // Se a pilha esvaziou e lemos todos os tokens, o código é válido!
    return ponteiro == tokens.size();
}
void rodarTestesSimulador() {
    cout << "\n=======================================================" << endl;
    cout << " BATERIA DE TESTES OFICIAIS DO PARSER (AVALIACAO)" << endl;
    cout << "=======================================================" << endl;

    // ---------------------------------------------------------
    // 1. ERROS LÉXICOS (Tokens inválidos)
    // ---------------------------------------------------------
    cout << "\n[1] TESTANDO ERROS LEXICOS" << endl;
    // O parser deve barrar se receber um token que nao existe na gramatica (ex: BALEIA)
    vector<string> testeLexico = {"PARENTE_ESQ", "START", "PARENTE_DIR", "PARENTE_ESQ", "BALEIA", "PARENTE_DIR"};
    if (!simularParserLL1(testeLexico)) cout << "[PASSOU] \033[1;32mRejeitou Token Invalido (Erro Lexico barrado)\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mAceitou Token Invalido\033[0m" << endl;

    // ---------------------------------------------------------
    // 2. EXPRESSÕES VÁLIDAS SIMPLES E ANINHADAS
    // ---------------------------------------------------------
    cout << "\n[2] TESTANDO EXPRESSOES SIMPLES E ANINHADAS" << endl;
    // Simples: ( START ) ( 10 5 + ) ( END )
    vector<string> testeSimples = {"PARENTE_ESQ", "START", "PARENTE_DIR", 
                                   "PARENTE_ESQ", "NUMERO", "NUMERO", "OP_ARITMETICO", "PARENTE_DIR", 
                                   "PARENTE_ESQ", "END", "PARENTE_DIR"};
    if (simularParserLL1(testeSimples)) cout << "[PASSOU] \033[1;32mExpressao Simples ( 10 5 + )\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mExpressao Simples ( 10 5 + )\033[0m" << endl;

    // Aninhada: ( START ) ( ( 10 5 + ) 2 * ) ( END )
    vector<string> testeAninhada = {"PARENTE_ESQ", "START", "PARENTE_DIR", 
                                    "PARENTE_ESQ", "PARENTE_ESQ", "NUMERO", "NUMERO", "OP_ARITMETICO", "PARENTE_DIR", "NUMERO", "OP_ARITMETICO", "PARENTE_DIR", 
                                    "PARENTE_ESQ", "END", "PARENTE_DIR"};
    if (simularParserLL1(testeAninhada)) cout << "[PASSOU] \033[1;32mExpressao Aninhada ( ( 10 5 + ) 2 * )\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mExpressao Aninhada ( ( 10 5 + ) 2 * )\033[0m" << endl;

    // ---------------------------------------------------------
    // 3. ESTRUTURAS DE CONTROLE VÁLIDAS
    // ---------------------------------------------------------
    cout << "\n[3] TESTANDO ESTRUTURAS DE CONTROLE (IF / WHILE)" << endl;
    // IF: ( START ) ( ( 10 5 > ) ( X MEM ) IF ) ( END )
    vector<string> testeIF = {"PARENTE_ESQ", "START", "PARENTE_DIR", 
                              "PARENTE_ESQ", "PARENTE_ESQ", "NUMERO", "NUMERO", "OP_RELACIONAL", "PARENTE_DIR", "PARENTE_ESQ", "VARIAVEL", "KEY_MEM", "PARENTE_DIR", "KEY_IF", "PARENTE_DIR", 
                              "PARENTE_ESQ", "END", "PARENTE_DIR"};
    if (simularParserLL1(testeIF)) cout << "[PASSOU] \033[1;32mEstrutura de Decisao (IF)\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mEstrutura de Decisao (IF)\033[0m" << endl;

    // WHILE: ( START ) ( ( X 10 < ) ( X 1 + ) WHILE ) ( END )
    vector<string> testeWHILE = {"PARENTE_ESQ", "START", "PARENTE_DIR", 
                                 "PARENTE_ESQ", "PARENTE_ESQ", "VARIAVEL", "NUMERO", "OP_RELACIONAL", "PARENTE_DIR", "PARENTE_ESQ", "VARIAVEL", "NUMERO", "OP_ARITMETICO", "PARENTE_DIR", "KEY_WHILE", "PARENTE_DIR", 
                                 "PARENTE_ESQ", "END", "PARENTE_DIR"};
    if (simularParserLL1(testeWHILE)) cout << "[PASSOU] \033[1;32mEstrutura de Repeticao (WHILE)\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mEstrutura de Repeticao (WHILE)\033[0m" << endl;

    // ---------------------------------------------------------
    // 4. ENTRADAS INVÁLIDAS (Erros sintáticos)
    // ---------------------------------------------------------
    cout << "\n[4] TESTANDO ENTRADAS INVALIDAS (ERROS SINTATICOS)" << endl;
    // Infixo no lugar de RPN: ( START ) ( 10 + 5 ) ( END )
    vector<string> testeErroInfixo = {"PARENTE_ESQ", "START", "PARENTE_DIR", 
                                      "PARENTE_ESQ", "NUMERO", "OP_ARITMETICO", "NUMERO", "PARENTE_DIR", 
                                      "PARENTE_ESQ", "END", "PARENTE_DIR"};
    if (!simularParserLL1(testeErroInfixo)) cout << "[PASSOU] \033[1;32mBloqueou Notacao Infixa (10 + 5)\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mBloqueou Notacao Infixa (10 + 5)\033[0m" << endl;

    // Faltando o operador no final: ( START ) ( 10 5 ) ( END )
    vector<string> testeErroSemOp = {"PARENTE_ESQ", "START", "PARENTE_DIR", 
                                     "PARENTE_ESQ", "NUMERO", "NUMERO", "PARENTE_DIR", 
                                     "PARENTE_ESQ", "END", "PARENTE_DIR"};
    if (!simularParserLL1(testeErroSemOp)) cout << "[PASSOU] \033[1;32mBloqueou Falta de Operador\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mBloqueou Falta de Operador\033[0m" << endl;

    // Faltando o START e o END (Gramatica exige o bloco principal)
    vector<string> testeErroEstrutura = {"PARENTE_ESQ", "NUMERO", "NUMERO", "OP_ARITMETICO", "PARENTE_DIR"};
    if (!simularParserLL1(testeErroEstrutura)) cout << "[PASSOU] \033[1;32mBloqueou Falta de START/END\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mBloqueou Falta de START/END\033[0m" << endl;

    // ---------------------------------------------------------
    // 5. CASOS EXTREMOS (Aninhamento profundo e blocos vazios)
    // ---------------------------------------------------------
    cout << "\n[5] TESTANDO CASOS EXTREMOS" << endl;
    // Expressao Vazia (apenas iniciar e terminar o programa)
    vector<string> testeVazio = {"PARENTE_ESQ", "START", "PARENTE_DIR", "PARENTE_ESQ", "END", "PARENTE_DIR"};
    if (simularParserLL1(testeVazio)) cout << "[PASSOU] \033[1;32mExpressao Vazia: ( START ) ( END )\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mExpressao Vazia: ( START ) ( END )\033[0m" << endl;

    // Aninhamento Profundo: ( START ) ( ( ( ( 1 1 + ) 2 + ) 3 + ) 4 + ) ( END )
    vector<string> testeExtremoAninhamento = {
        "PARENTE_ESQ", "START", "PARENTE_DIR", 
        "PARENTE_ESQ", "PARENTE_ESQ", "PARENTE_ESQ", "PARENTE_ESQ", "NUMERO", "NUMERO", "OP_ARITMETICO", "PARENTE_DIR", 
        "NUMERO", "OP_ARITMETICO", "PARENTE_DIR", "NUMERO", "OP_ARITMETICO", "PARENTE_DIR", "NUMERO", "OP_ARITMETICO", "PARENTE_DIR", 
        "PARENTE_ESQ", "END", "PARENTE_DIR"
    };
    if (simularParserLL1(testeExtremoAninhamento)) cout << "[PASSOU] \033[1;32mAninhamento Profundo Nivel 4\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mAninhamento Profundo Nivel 4\033[0m" << endl;

    cout << "=======================================================\n" << endl;
}


int main() {
    rodarTestesFirst();
    rodarTestesFollow();
    rodarTesteTabela();
    rodarTestesSimulador();
    return 0;
}