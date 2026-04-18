#include "Gramatica.hpp"

// Instanciando o cache de conjuntos First
map<string, set<string>> conjuntosFirst;

set<string> buscarFirst(string simbolo) {
    set<string> resultado;

    if (validarTerminal(simbolo) || simbolo == "EPSILON") {
        resultado.insert(simbolo);
        return resultado;
    }

    if (!conjuntosFirst[simbolo].empty()) {
        return conjuntosFirst[simbolo];
    }

    for (const auto& producao : gramatica[simbolo]) {
        bool producaoPodeSerVazia = true;

        for (const string& palavra : producao) {
            set<string> firstDaPalavra = buscarFirst(palavra);
            
            for (const string& f : firstDaPalavra) {
                if (f != "EPSILON") resultado.insert(f);
            }

            if (firstDaPalavra.find("EPSILON") == firstDaPalavra.end()) {
                producaoPodeSerVazia = false;
                break;
            }
        }

        if (producaoPodeSerVazia) {
            resultado.insert("EPSILON");
        }
    }

    conjuntosFirst[simbolo] = resultado;
    return resultado;
}

void calcularFirst() {
    for (const auto& par : gramatica) {
        buscarFirst(par.first);
    }
}

// Instanciando o cache do Follow
map<string, set<string>> conjuntosFollow;

void calcularFollow() {
    // Regra 1: O símbolo principal recebe EOF
    conjuntosFollow["programa"].insert("EOF");

    bool mudou = true;
    // O loop continua rodando até que nenhuma regra adicione nada novo
    while (mudou) {
        mudou = false;

        for (const auto& par : gramatica) {
            string ladoEsquerdo = par.first;

            for (const auto& producao : par.second) {
                // Varre cada palavra na produção procurando Não-Terminais
                for (size_t i = 0; i < producao.size(); ++i) {
                    string simboloAtual = producao[i];

                    // FOLLOW só se calcula para Não-Terminais
                    if (validarTerminal(simboloAtual) || simboloAtual == "EPSILON") continue;

                    size_t tamanhoOriginal = conjuntosFollow[simboloAtual].size();

                    // Regra 2: Olha para o que está na frente (beta)
                    bool epsilonNoFirstDoResto = true;
                    for (size_t j = i + 1; j < producao.size(); ++j) {
                        string beta = producao[j];
                        set<string> firstBeta = buscarFirst(beta);

                        for (const string& f : firstBeta) {
                            if (f != "EPSILON") conjuntosFollow[simboloAtual].insert(f);
                        }

                        // Se beta não some, atua como parede
                        if (firstBeta.find("EPSILON") == firstBeta.end()) {
                            epsilonNoFirstDoResto = false;
                            break;
                        }
                    }

                    // Regra 3: Se não tem nada na frente ou tudo na frente pode sumir (EPSILON),
                    // herda o FOLLOW do lado esquerdo (o "pai")
                    if (epsilonNoFirstDoResto) {
                        for (const string& f : conjuntosFollow[ladoEsquerdo]) {
                            conjuntosFollow[simboloAtual].insert(f);
                        }
                    }

                    // Se o conjunto cresceu, avisa o while para rodar de novo
                    if (conjuntosFollow[simboloAtual].size() > tamanhoOriginal) {
                        mudou = true;
                    }
                }
            }
        }
    }
}

map<string, map<string, vector<string>>> tabelaLL1;

void construirTabelaLL1() {
    // Varre toda a gramática
    for (const auto& par : gramatica) {
        string naoTerminal = par.first;

        for (const auto& producao : par.second) {
            // Passo 1: Achar o FIRST dessa produção inteira
            set<string> firstDaProducao;
            bool producaoPodeSerVazia = true;

            for (const string& palavra : producao) {
                set<string> firstPalavra = buscarFirst(palavra);
                for (const string& f : firstPalavra) {
                    if (f != "EPSILON") firstDaProducao.insert(f);
                }
                if (firstPalavra.find("EPSILON") == firstPalavra.end()) {
                    producaoPodeSerVazia = false;
                    break;
                }
            }
            if (producaoPodeSerVazia) firstDaProducao.insert("EPSILON");

            // Passo 2: Regra do FIRST -> Preenche as células
            for (const string& terminal : firstDaProducao) {
                if (terminal != "EPSILON") {
                    // SE JÁ TIVER REGRA LÁ, É CONFLITO!
                    if (!tabelaLL1[naoTerminal][terminal].empty()) {
                        cout << "ERRO FATAL: Conflito LL(1) em [" << naoTerminal << "][" << terminal << "]" << endl;
                    } else {
                        tabelaLL1[naoTerminal][terminal] = producao;
                    }
                }
            }

            // Passo 3: Regra do FOLLOW (se a produção pode ser EPSILON)
            if (producaoPodeSerVazia) {
                for (const string& terminalFollow : conjuntosFollow[naoTerminal]) {
                    if (!tabelaLL1[naoTerminal][terminalFollow].empty()) {
                         // Ignoramos re-inserção da mesma regra, mas acusamos se for regra diferente
                         if (tabelaLL1[naoTerminal][terminalFollow] != producao) {
                             cout << "ERRO FATAL: Conflito LL(1) em [" << naoTerminal << "][" << terminalFollow << "]" << endl;
                         }
                    } else {
                        tabelaLL1[naoTerminal][terminalFollow] = producao;
                    }
                }
            }
        }
    }
}