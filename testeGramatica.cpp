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
    cout << "--- INICIANDO SIMULADOR LL(1) COM ENTRADAS DIVERSAS ---" << endl;

    // Caso 1: O programa mínimo viável -> ( START ) ( END )
    vector<string> teste1 = {"PARENTE_ESQ", "START", "PARENTE_DIR", "PARENTE_ESQ", "END", "PARENTE_DIR"};
    if (simularParserLL1(teste1)) cout << "[PASSOU] \033[1;32mPrograma Vazio\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mPrograma Vazio\033[0m" << endl;

    // Caso 2: Comando simples -> ( START ) ( 10 5 + ) ( END )
    vector<string> teste2 = {"PARENTE_ESQ", "START", "PARENTE_DIR", 
                             "PARENTE_ESQ", "NUMERO", "NUMERO", "OP_ARITMETICO", "PARENTE_DIR", 
                             "PARENTE_ESQ", "END", "PARENTE_DIR"};
    if (simularParserLL1(teste2)) cout << "[PASSOU] \033[1;32mComando Simples RPN\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mComando Simples RPN\033[0m" << endl;

    // Caso 3: Comando aninhado complexo -> ( START ) ( ( X 2 / ) MEM ) ( END )
    vector<string> teste3 = {"PARENTE_ESQ", "START", "PARENTE_DIR", 
                             "PARENTE_ESQ", "PARENTE_ESQ", "VARIAVEL", "NUMERO", "OP_ARITMETICO", "PARENTE_DIR", "KEY_MEM", "PARENTE_DIR", 
                             "PARENTE_ESQ", "END", "PARENTE_DIR"};
    if (simularParserLL1(teste3)) cout << "[PASSOU] \033[1;32mComando Aninhado com MEM\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mComando Aninhado com MEM\033[0m" << endl;

    // Caso 4: ERRO SINTÁTICO PROPOSITAL -> Faltando operador final ( START ) ( 10 5 ) ( END )
    vector<string> testeErro1 = {"PARENTE_ESQ", "START", "PARENTE_DIR", 
                                 "PARENTE_ESQ", "NUMERO", "NUMERO", "PARENTE_DIR", 
                                 "PARENTE_ESQ", "END", "PARENTE_DIR"};
    if (!simularParserLL1(testeErro1)) cout << "[PASSOU] \033[1;32mBloqueou Erro Sintatico (Sem operador)\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mBloqueou Erro Sintatico (Sem operador)\033[0m" << endl;

    // Caso 5: ERRO SINTÁTICO PROPOSITAL -> Notação Infixa ( START ) ( 10 + 5 ) ( END )
    vector<string> testeErro2 = {"PARENTE_ESQ", "START", "PARENTE_DIR", 
                                 "PARENTE_ESQ", "NUMERO", "OP_ARITMETICO", "NUMERO", "PARENTE_DIR", 
                                 "PARENTE_ESQ", "END", "PARENTE_DIR"};
    if (!simularParserLL1(testeErro2)) cout << "[PASSOU] \033[1;32mBloqueou Erro Sintatico (Infixo)\033[0m" << endl;
    else cout << "[FALHOU] \033[1;31mBloqueou Erro Sintatico (Infixo)\033[0m" << endl;

    cout << "-------------------------------------------------------" << endl;
}


int main() {
    rodarTestesFirst();
    rodarTestesFollow();
    rodarTesteTabela();
    rodarTestesSimulador();
    return 0;
}