// Integrantes do grupo (ordem alfabética):
// Vinícius Matheus Sary de Lima - ViniMSLima
// Nome do grupo no Canvas: RA1 20

#include "AST.hpp"
#include "Gramatica.hpp"

NoAST* gerarArvore(vector<Token> tokens) {
    stack<string>  pilhaSintatica;
    stack<NoAST*>  pilhaSemantica;

    // Regra de Ouro do LL(1): EOF embaixo, símbolo inicial em cima
    pilhaSintatica.push("EOF");
    pilhaSintatica.push("programa");

    // Marcador de fim de fita
    Token tokenEOF;
    tokenEOF.tipo  = "EOF";
    tokenEOF.valor = "EOF";
    tokenEOF.linha = -1;
    tokens.push_back(tokenEOF);

    int ponteiro = 0;

    while (!pilhaSintatica.empty()) {
        string topo       = pilhaSintatica.top();
        Token  tokenAtual = tokens[ponteiro];

        // EPSILON não consome nada
        if (topo == "EPSILON") {
            pilhaSintatica.pop();
            continue;
        }

        // ── Terminal: deve casar com o token atual ──────────────────────
        if (validarTerminal(topo) || topo == "EOF") {
            if (topo == tokenAtual.tipo) {
                pilhaSintatica.pop();

                // ========================================================
                // CONSTRUÇÃO DA AST (ações semânticas)
                // ========================================================

                // Folhas: literais e variáveis
                if (topo == "NUMERO" || topo == "VARIAVEL") {
                    pilhaSemantica.push(new NoAST(topo, tokenAtual.valor));
                }

                // Operadores binários aritméticos e relacionais
                else if (topo == "OP_ARITMETICO" || topo == "OP_RELACIONAL") {
                    NoAST* noOp = new NoAST(topo, tokenAtual.valor);
                    if (pilhaSemantica.size() >= 2) {
                        NoAST* dir = pilhaSemantica.top(); pilhaSemantica.pop();
                        NoAST* esq = pilhaSemantica.top(); pilhaSemantica.pop();
                        noOp->adicionarFilho(esq);
                        noOp->adicionarFilho(dir);
                    }
                    pilhaSemantica.push(noOp);
                }

                // Comandos binários de fluxo: IF e WHILE
                // filhos[0] = condição, filhos[1] = ação
                else if (topo == "KEY_IF" || topo == "KEY_WHILE") {
                    NoAST* noCmd = new NoAST(topo, tokenAtual.valor);
                    if (pilhaSemantica.size() >= 2) {
                        NoAST* acao     = pilhaSemantica.top(); pilhaSemantica.pop();
                        NoAST* condicao = pilhaSemantica.top(); pilhaSemantica.pop();
                        noCmd->adicionarFilho(condicao);
                        noCmd->adicionarFilho(acao);
                    }
                    pilhaSemantica.push(noCmd);
                }

                else if (topo == "KEY_MEM") {
                    NoAST* noMem = new NoAST(topo, tokenAtual.valor);
                    if (pilhaSemantica.size() >= 2) {
                        NoAST* destino = pilhaSemantica.top(); pilhaSemantica.pop();
                        NoAST* valor   = pilhaSemantica.top(); pilhaSemantica.pop();
                        noMem->adicionarFilho(valor);    // filhos[0] = expressão de valor
                        noMem->adicionarFilho(destino);  // filhos[1] = variável de destino
                    } else if (!pilhaSemantica.empty()) {
                        // fallback: apenas um item (ex: bloco vazio)
                        noMem->adicionarFilho(pilhaSemantica.top());
                        pilhaSemantica.pop();
                    }
                    pilhaSemantica.push(noMem);
                }

                // RES: apenas um filho — o número de linhas anteriores
                else if (topo == "KEY_RES") {
                    NoAST* noRes = new NoAST(topo, tokenAtual.valor);
                    if (!pilhaSemantica.empty()) {
                        noRes->adicionarFilho(pilhaSemantica.top());
                        pilhaSemantica.pop();
                    }
                    pilhaSemantica.push(noRes);
                }

                // Parênteses, START e END são ignorados na AST

                ponteiro++; // avança a fita
            } else {
                cout << "\n[ERRO SINTATICO] Linha " << tokenAtual.linha
                     << ": Esperava '" << topo << "' mas recebeu '"
                     << tokenAtual.tipo << "' (" << tokenAtual.valor << ")" << endl;
                return nullptr;
            }
        }

        // ── Não-Terminal: consulta a Tabela LL(1) ───────────────────────
        else {
            vector<string> regra = tabelaLL1[topo][tokenAtual.tipo];

            if (regra.empty()) {
                cout << "\n[ERRO SINTATICO] Linha " << tokenAtual.linha
                     << ": Nenhuma regra na tabela para ["
                     << topo << "] e [" << tokenAtual.tipo << "]" << endl;
                return nullptr;
            }

            pilhaSintatica.pop();
            if (regra[0] != "EPSILON") {
                for (int i = (int)regra.size() - 1; i >= 0; i--) {
                    pilhaSintatica.push(regra[i]);
                }
            }
        }
    }

    // ── Monta o nó raiz com todos os comandos em ordem ──────────────────
    NoAST* raiz = new NoAST("RAIZ", "PROGRAMA_COMPLETO");

    vector<NoAST*> temp;
    while (!pilhaSemantica.empty()) {
        temp.push_back(pilhaSemantica.top());
        pilhaSemantica.pop();
    }
    for (int i = (int)temp.size() - 1; i >= 0; i--) {
        raiz->adicionarFilho(temp[i]);
    }

    return raiz;
}