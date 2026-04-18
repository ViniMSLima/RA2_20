#include "AST.hpp"
#include "Gramatica.hpp" // Precisamos do mapa validarTerminal e tabelaLL1

NoAST* parsear(vector<Token> tokens) {
    stack<string> pilhaSintatica;
    stack<NoAST*> pilhaSemantica; // Nossa creche de nós

    // Regra de Ouro do LL(1)
    pilhaSintatica.push("EOF");
    pilhaSintatica.push("programa");

    // Adiciona o EOF no final da fita de tokens
    Token tokenEOF; 
    tokenEOF.tipo = "EOF"; 
    tokenEOF.valor = "EOF";
    tokenEOF.linha = -1; // EOF não tem linha
    tokens.push_back(tokenEOF);

    int ponteiro = 0;

    while (!pilhaSintatica.empty()) {
        string topo = pilhaSintatica.top();
        Token tokenAtual = tokens[ponteiro];

        if (topo == "EPSILON") {
            pilhaSintatica.pop();
            continue;
        }

        // Se for Terminal, tem que casar com o token!
        if (validarTerminal(topo) || topo == "EOF") {
            if (topo == tokenAtual.tipo) {
                pilhaSintatica.pop(); // Deu match, sai da pilha sintática

                // ==========================================
                // 🌳 MÁGICA DA AST (ALUNO 2) AQUI!
                // ==========================================
                
                if (topo == "NUMERO" || topo == "VARIAVEL") {
                    pilhaSemantica.push(new NoAST(topo, tokenAtual.valor));
                } 
                else if (topo == "OP_ARITMETICO" || topo == "OP_RELACIONAL") {
                    NoAST* noOp = new NoAST(topo, tokenAtual.valor);
                    if (pilhaSemantica.size() >= 2) {
                        NoAST* filhoDir = pilhaSemantica.top(); pilhaSemantica.pop();
                        NoAST* filhoEsq = pilhaSemantica.top(); pilhaSemantica.pop();
                        noOp->adicionarFilho(filhoEsq);
                        noOp->adicionarFilho(filhoDir);
                    }
                    pilhaSemantica.push(noOp);
                }
                else if (topo == "KEY_IF" || topo == "KEY_WHILE") {
                    NoAST* noCmd = new NoAST(topo, tokenAtual.valor);
                    if (pilhaSemantica.size() >= 2) {
                        NoAST* acao = pilhaSemantica.top(); pilhaSemantica.pop();
                        NoAST* condicao = pilhaSemantica.top(); pilhaSemantica.pop();
                        noCmd->adicionarFilho(condicao);
                        noCmd->adicionarFilho(acao);
                    }
                    pilhaSemantica.push(noCmd);
                }
                else if (topo == "KEY_MEM" || topo == "KEY_RES") {
                    NoAST* noUnario = new NoAST(topo, tokenAtual.valor);
                    if (!pilhaSemantica.empty()) {
                        NoAST* filho = pilhaSemantica.top(); pilhaSemantica.pop();
                        noUnario->adicionarFilho(filho);
                    }
                    pilhaSemantica.push(noUnario);
                }
                // Nota: Parênteses, START e END são ignorados na AST.

                ponteiro++; // Avança a fita
            } else {
                cout << "\n[ERRO SINTATICO] Linha " << tokenAtual.linha 
                     << ": Esperava '" << topo << "' mas recebeu '" 
                     << tokenAtual.tipo << "' (" << tokenAtual.valor << ")" << endl;
                return nullptr;
            }
        } 
        // Se for Não-Terminal, consulta o GPS (Tabela LL1 do Aluno 1)
        else {
            vector<string> regra = tabelaLL1[topo][tokenAtual.tipo];
            
            if (regra.empty()) {
                cout << "\n[ERRO SINTATICO] Linha " << tokenAtual.linha 
                     << ": Nenhuma regra na tabela para [" << topo 
                     << "] e [" << tokenAtual.tipo << "]" << endl;
                return nullptr;
            }

            pilhaSintatica.pop();
            if (regra[0] != "EPSILON") {
                for (int i = regra.size() - 1; i >= 0; i--) {
                    pilhaSintatica.push(regra[i]);
                }
            }
        }
    }

    // ==========================================
    // FINALIZANDO A ÁRVORE
    // ==========================================
    // Criamos um nó raiz principal para pendurar todos os comandos do programa
    NoAST* raiz = new NoAST("RAIZ", "PROGRAMA_COMPLETO");
    
    // Desempilha os comandos que sobraram e inverte para ficar na ordem cronológica certa
    vector<NoAST*> temporario;
    while(!pilhaSemantica.empty()) {
        temporario.push_back(pilhaSemantica.top());
        pilhaSemantica.pop();
    }
    for(int i = temporario.size() - 1; i >= 0; i--) {
        raiz->adicionarFilho(temporario[i]);
    }

    return raiz;
}