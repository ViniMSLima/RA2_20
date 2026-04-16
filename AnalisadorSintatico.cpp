// Integrantes do grupo (ordem alfabética):
// Vinícius Matheus Sary de Lima - ViniMSLima
//
// Nome do grupo no Canvas: RA2 20

#include <iostream> // Printar no terminal
#include <fstream>  // Arquivos
#include <string>   // Linhas de texto lidas
#include <cctype>   // isdigit() e isalpha()
#include <vector>   // Para guardar os tokens em vetor e histórico RES
#include <stack>    // Pilha
#include <map>      // Dicionário
#include <cmath>    // pow() e fmod()
#include <set>      // Para criar o .s 

struct Token {
        std::string tipo; 
        std::string valor;
    };

// Variáveis globais
std::map<std::string, double> memoriaVariaveis;
std::vector<double> historicoRES;

void lerArquivo(std::string nomeArquivo, std::vector<std::string>& linhas) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cout << "ERRO: Nao foi possivel abrir o arquivo: " << nomeArquivo << std::endl;
        return;
    }

    std::string linha;
    //Valida se a linha é vazia pra não adicionar no vetor de linhas
    while (std::getline(arquivo, linha)) {
        if (!linha.empty()) {
            linhas.push_back(linha);
        }
    }
    arquivo.close();
}

//// ALUNO 1

Token estadoLendoNumero(const std::string& linha, int& posicao) {
    std::string valor = "";
    int qtdPontos = 0;

    // Enquanto for dígito ou um ponto decimal
    while (posicao < linha.length() && (isdigit(linha[posicao]) || linha[posicao] == '.')) {
        
        if (linha[posicao] == '.') {
            qtdPontos++;
        }
        
        valor += linha[posicao];
        posicao++; 
    }

    // Validação
    if (qtdPontos > 1) {
        std::cout << "ERRO: Numero errado = " << valor << std::endl;
        return {"ERRO", valor}; // Retorna erro
    }

    // Retornar o valor
    return {"NUMERO", valor};
}

Token estadoLendoPalavra(const std::string& linha, int& posicao) {
    std::string valor = "";
    bool temMinuscula = false;

    // Enquanto for letra
    while (posicao < linha.length() && isalpha(linha[posicao])) {
        // Procurar letra minuscula
        if (islower(linha[posicao])) {
            temMinuscula = true;
        }
        
        valor += linha[posicao];
        posicao++; 
    }

    // Validação de letra minuscula
    if (temMinuscula) {
        std::cout << "ERRO: Variaveis devem ser maiusculas -> " << valor << std::endl;
        return {"ERRO", valor};
    }

    // Validação do RES
    if (valor == "RES") {
        return {"KEYWORD", valor};
    }

    // Retornar o valor
    return {"VARIAVEL", valor};
}

Token estadoLendoBarra(const std::string& linha, int& posicao) {
    posicao++; 
    if(posicao < linha.length() && linha[posicao] == '/') {
        posicao++; 
        return {"OPERADOR", "//"};
    }
    return {"OPERADOR", "/"};
}

Token estadoInicial(const std::string& linha, int& posicao) {
    // Ignorar espaços
    while (posicao < linha.length() && isspace(linha[posicao])) {
        posicao++;
    }

    // Final da linha pulando espaços = acabou
    if (posicao >= linha.length()) {
        return {"FIM", ""};
    }

    char atual = linha[posicao];

    // É um número?
    if (isdigit(atual)) { return estadoLendoNumero(linha, posicao); }

    // É uma letra?
    else if (isalpha(atual)) { return estadoLendoPalavra(linha, posicao); }

    // É a barra de divisão?
    else if (atual == '/') { return estadoLendoBarra(linha, posicao); }

    // São operadores simples ou parênteses?
    else if (atual == '+' || atual == '-' || atual == '*' || atual == '%' || atual == '^' || atual == '(' || atual == ')') {
        posicao++; // Anda uma casa, pois já lemos o caractere
        
        // Transforma o char em string para guardar no Token
        std::string valor_str(1, atual); 
        
        if (atual == '(' || atual == ')') return {"PARENTE", valor_str};
        else return {"OPERADOR", valor_str};
    }
    // Se não for nada disso, é um caractere inválido (Erro Léxico!)
    else {
        std::cout << "ERRO LEXICO: Caractere invalido encontrado: " << atual << std::endl;
        posicao++; // Pula o caractere inválido para não travar num loop infinito
        return {"ERRO", ""};
    }
}

void parseExpressao(std::string linha, std::vector<Token>& tokensDaLinha) {
    int posicao = 0;
    
    while (posicao < linha.length()) {
        Token t = estadoInicial(linha, posicao);
        
        if (t.tipo != "FIM" && t.tipo != "ERRO") {
            tokensDaLinha.push_back(t);
        }
    }
}

void testarAnalisadorLexico() {
    std::cout << "Criar funcoes de teste para o analisador lexico, cobrindo entradas validas e invalidas" << std::endl;
    
    std::vector<Token> tokensTeste;
    
    // Teste 1: Entrada Válida
    std::cout << "Teste 1 (Valido): (3.14 2.0 //)" << std::endl;
    parseExpressao("(3.14 2.0 //)", tokensTeste);
    
    // Teste 2: Número malformado
    std::cout << "\nTeste 2 (Invalido - Numero): 3.14.5" << std::endl;
    parseExpressao("3.14.5", tokensTeste);

    // Teste 3: Variável minúscula
    std::cout << "\nTeste 3 (Invalido - Variavel): (var 5 +)" << std::endl;
    parseExpressao("(var 5 +)", tokensTeste);

    // Teste 4: Caractere estranho
    std::cout << "\nTeste 4 (Invalido - Caractere estranho): (3.14 2.0 &)" << std::endl;
    parseExpressao("(3.14 2.0 &)", tokensTeste);

    // Teste 5: Uso de vírgula em número
    std::cout << "\nTeste 5 (Invalido - Uso de virgula): 3,45" << std::endl;
    parseExpressao("3,45", tokensTeste);

    // Teste 6: Parênteses desbalanceados
    std::cout << "\nTeste 6 (Invalido - Parenteses desbalanceados): ( ( 3.14 2.0 + )" << std::endl;
    parseExpressao("( ( 3.14 2.0 + )", tokensTeste);

    // Teste 7: Expressão válida com operadores e variáveis
    std::cout << "\nTeste 7 (Valido - Operadores e Memoria): ( ( 2.0 3.0 ^ ) RES CONTADOR )" << std::endl;
    parseExpressao("( ( 2.0 3.0 ^ ) RES CONTADOR )", tokensTeste);
    
    std::cout << "FIM DO TESTE" << std::endl;
}


//// ALUNO 2
void executarExpressao(const std::vector<Token>& tokens) {
    std::stack<double> pilha;
    int contadorParenteses = 0; // Validar parênteses

    for (size_t i = 0; i < tokens.size(); ++i) {
        Token t = tokens[i];

        // Validar parênteses
        if (t.tipo == "PARENTE") {
            if (t.valor == "(") contadorParenteses++;
            else contadorParenteses--;
            continue; // Parêntese não vai para a pilha
        }

        if (t.tipo == "NUMERO") {
            pilha.push(std::stod(t.valor));
        }
        else if (t.tipo == "OPERADOR") {
            if (pilha.size() < 2) {
                std::cout << "ERRO: Pilha insuficiente para operar " << t.valor << std::endl;
                return;
            }
            double b = pilha.top(); pilha.pop();
            double a = pilha.top(); pilha.pop();

            if (t.valor == "+") pilha.push(a + b);
            else if (t.valor == "-") pilha.push(a - b);
            else if (t.valor == "*") pilha.push(a * b);
            else if (t.valor == "/") {
                if (b == 0) { std::cout << "ERRO: Divisao real por zero!" << std::endl; return; }
                pilha.push(a / b);
            }
            else if (t.valor == "//") {
                if (b == 0) { std::cout << "ERRO: Divisao inteira por zero!" << std::endl; return; }
                pilha.push(std::floor(a / b));
            }
            else if (t.valor == "%") {
                if (b == 0) { std::cout << "ERRO: Resto por zero!" << std::endl; return; }
                pilha.push(std::fmod(a, b));
            }
            else if (t.valor == "^") {
                pilha.push(std::pow(a, b));
            }
        }
        else if (t.tipo == "VARIAVEL") {
            if (i + 1 < tokens.size() && tokens[i+1].valor == "MEM") {
                if (!pilha.empty()) {
                    memoriaVariaveis[t.valor] = pilha.top();
                    i++; 
                }
            } else {
                if (memoriaVariaveis.count(t.valor)) {
                    pilha.push(memoriaVariaveis[t.valor]);
                } else {
                    std::cout << "ERRO: Variavel " << t.valor << " nao iniciada." << std::endl;
                    return;
                }
            }
        }
        else if (t.tipo == "KEYWORD" && t.valor == "RES") {
            if (!pilha.empty()) {
                int n = (int)pilha.top(); pilha.pop();
                
                // MÁGICA DO ÍNDICE RELATIVO: 
                // Linha Atual (size) - N linhas para trás
                int target_idx = historicoRES.size() - n; 

                if (target_idx >= 0 && target_idx < (int)historicoRES.size()) {
                    pilha.push(historicoRES[target_idx]);
                } else {
                    std::cout << "ERRO: Indice RES " << n << " invalido. A linha alvo ainda nao existe." << std::endl;
                    return;
                }
            } else {
                std::cout << "ERRO: Faltou o numero antes do RES!" << std::endl;
                return;
            }
        }
    }

    // Checagem de parênteses no fim da linha
    if (contadorParenteses != 0) {
        std::cout << "ERRO: Parenteses desbalanceados na linha!" << std::endl;
        return;
    }

    if (!pilha.empty()) {
        double final = pilha.top();
        historicoRES.push_back(final);
        //std::cout << "SIMULACAO: Resultado = " << final << std::endl;
    }
}

void testarExecucao() {
    std::cout << "TESTES DE EXECUCAO" << std::endl;
    
    // Resetando para o teste
    memoriaVariaveis.clear();
    historicoRES.clear();

    // Matemática básica e histórico
    std::cout << "Teste 1: ( 5 2 + )" << std::endl;
    std::vector<Token> t1 = {{"PARENTE","("}, {"NUMERO","5"}, {"NUMERO","2"}, {"OPERADOR","+"}, {"PARENTE",")"}};
    executarExpressao(t1); // Deve dar 7

    // Memória
    std::cout << "\nTeste 2: ( 10 X MEM )" << std::endl;
    std::vector<Token> t2 = {{"PARENTE","("}, {"NUMERO","10"}, {"VARIAVEL","X"}, {"VARIAVEL","MEM"}, {"PARENTE",")"}};
    executarExpressao(t2); // X vira 10

    // Uso de variável e RES
    std::cout << "\nTeste 3: ( X 0 RES * )" << std::endl;
    std::vector<Token> t3 = {{"PARENTE","("}, {"VARIAVEL","X"}, {"NUMERO","0"}, {"KEYWORD","RES"}, {"OPERADOR","*"}, {"PARENTE",")"}};
    executarExpressao(t3); // 10 * 7 = 70

    std::cout << "FIM DOS TESTE" << std::endl;
}

//// ALUNO 3
std::string gerarAssembly(const std::vector<std::vector<Token>>& todasAsLinhas) {
    std::string code = ".text\n.global _start\n\n_start:\n";

    // --- INICIALIZAÇÃO ---
    code += "    @ Inicializacao: Pilha e FPU\n";
    code += "    LDR SP, =0x20000\n";
    code += "    LDR R0, =(0xF << 20)\n    MCR P15, 0, R0, C1, C0, 2\n    ISB\n";
    code += "    VMRS R0, FPEXC\n    ORR  R0, R0, #0x40000000\n    VMSR FPEXC, R0\n\n";
    
    std::set<std::string> literais; 
    literais.insert("1.0");
    literais.insert("0.0");

    for (int i = 0; i < (int)todasAsLinhas.size(); i++) {
        code += "\n    @ --- Linha " + std::to_string(i) + " ---\n";
        
        for (size_t j = 0; j < todasAsLinhas[i].size(); j++) {
            Token t = todasAsLinhas[i][j];

            if (t.tipo == "NUMERO") {
                literais.insert(t.valor);
                std::string labelNum = t.valor;
                for(char &c : labelNum) if(c == '.') c = '_';
                code += "    LDR R0, =lit_" + labelNum + "\n    VLDR.F64 D0, [R0]\n    VPUSH {D0}\n";
            }
            
            else if (t.tipo == "OPERADOR") {
                code += "    VPOP {D1}    @ B\n    VPOP {D0}    @ A\n";

                if (t.valor == "+")      code += "    VADD.F64 D2, D0, D1\n";
                else if (t.valor == "-") code += "    VSUB.F64 D2, D0, D1\n";
                else if (t.valor == "*") code += "    VMUL.F64 D2, D0, D1\n";
                else if (t.valor == "/") code += "    VDIV.F64 D2, D0, D1\n";
                
                // Divisão Inteira (//) e Resto (%)
                else if (t.valor == "//" || t.valor == "%") {
                    std::string lid = std::to_string(i) + "_" + std::to_string(j);
                    code += "    VDIV.F64 D2, D0, D1      @ D2 = resultado real\n";
                    code += "    LDR R0, =lit_0_0\n    VLDR.F64 D3, [R0]        @ Acumulador floor\n";
                    code += "    LDR R0, =lit_1_0\n    VLDR.F64 D4, [R0]        @ Passo 1.0\n";
                    code += "    @ Loop de Truncamento manual\n";
                    code += "trunc_l_" + lid + ":\n";
                    code += "    VADD.F64 D5, D3, D4\n";
                    code += "    VCMP.F64 D5, D2\n";
                    code += "    VMRS APSR_nzcv, FPSCR\n";
                    code += "    BGT trunc_e_" + lid + "    @ Se passar do valor, para\n";
                    code += "    VMOV.F64 D3, D5\n";
                    code += "    B trunc_l_" + lid + "\n";
                    code += "trunc_e_" + lid + ":\n";
                    
                    if (t.valor == "//") {
                        code += "    VMOV.F64 D2, D3          @ D2 recebe o floor\n";
                    } else {
                        code += "    VMUL.F64 D3, D3, D1      @ floor * b\n";
                        code += "    VSUB.F64 D2, D0, D3      @ a - (floor * b)\n";
                    }
                }
                else if (t.valor == "^") {
                    std::string lid = std::to_string(i) + "_" + std::to_string(j);
                    
                    code += "    LDR R0, =lit_1_0\n    VLDR.F64 D2, [R0]\n";
                    
                    code += "    MOV R1, #0\n    LDR R0, =lit_0_0\n    VLDR.F64 D4, [R0]\n    LDR R0, =lit_1_0\n    VLDR.F64 D5, [R0]\n";
                    
                    code += "pow_l_" + lid + ":\n";
                    code += "    VCMP.F64 D4, D1\n    VMRS APSR_nzcv, FPSCR\n    BGE pow_e_" + lid + "\n";
                    code += "    VMUL.F64 D2, D2, D0\n    VADD.F64 D4, D4, D5\n    B pow_l_" + lid + "\n";
                    code += "pow_e_" + lid + ":\n";
                }
                code += "    VPUSH {D2}\n";
            }

            else if (t.tipo == "VARIAVEL") {
                if (j + 1 < todasAsLinhas[i].size() && todasAsLinhas[i][j+1].valor == "MEM") {
                    code += "    VPOP {D0}\n    VPUSH {D0}\n";
                    code += "    LDR R0, =" + t.valor + "\n    VSTR.F64 D0, [R0]\n";
                    j++; 
                } else {
                    code += "    LDR R0, =" + t.valor + "\n    VLDR.F64 D0, [R0]\n    VPUSH {D0}\n";
                }
            }

            else if (t.tipo == "KEYWORD" && t.valor == "RES") {
                std::string lid = std::to_string(i) + "_" + std::to_string(j);
                
                // 1. Pega o N relativo da pilha e joga em D1
                code += "    VPOP {D1}                @ N (relativo)\n";
                
                // 2. Constrói o valor da linha atual (i) em D0 de forma cega/segura
                code += "    LDR R0, =lit_0_0\n";
                code += "    VLDR.F64 D0, [R0]        @ D0 = 0.0\n";
                if (i > 0) {
                    code += "    LDR R0, =lit_1_0\n";
                    code += "    VLDR.F64 D2, [R0]        @ D2 = 1.0\n";
                    for (int k = 0; k < i; ++k) {
                        code += "    VADD.F64 D0, D0, D2      @ Soma 1.0 para achar a linha atual\n";
                    }
                }
                
                // 3. MÁGICA: Acha o índice absoluto -> Atual(D0) - Relativo(D1)
                code += "    VSUB.F64 D0, D0, D1\n";
                
                // 4. Continua o Loop para buscar na memória usando o D0 corrigido!
                code += "    MOV R1, #0               @ Contador inteiro\n";
                code += "    LDR R0, =lit_0_0\n";
                code += "    VLDR.F64 D6, [R0]        @ Contador float aux\n";
                code += "    LDR R0, =lit_1_0\n";
                code += "    VLDR.F64 D7, [R0]        @ Passo 1.0\n";
                
                code += "res_l_" + lid + ":\n";
                code += "    VCMP.F64 D6, D0          @ Compara: contador >= indice?\n";
                code += "    VMRS APSR_nzcv, FPSCR\n";
                code += "    BGE res_e_" + lid + "    @ Se ja chegou no indice, sai\n";
                code += "    ADD R1, R1, #1           @ Incrementa R1 (inteiro)\n";
                code += "    VADD.F64 D6, D6, D7      @ Incrementa D6 (float)\n";
                code += "    B res_l_" + lid + "\n";
                code += "res_e_" + lid + ":\n";
                
                code += "    LSL R1, R1, #3           @ R1 = R1 * 8 bytes (Offset)\n";
                code += "    LDR R0, =res_history     @ R0 = Endereco base do historico\n";
                code += "    ADD R0, R0, R1           @ R0 = Base + Offset\n"; 
                code += "    VLDR.F64 D0, [R0]        @ Busca o valor no endereco exato\n";
                code += "    VPUSH {D0}\n";
            }
        }
        code += "    VPOP {D0}\n    LDR R0, =res_history\n    VSTR.F64 D0, [R0, #" + std::to_string(i * 8) + "]\n";
    }

    code += "\n    B .\n";

    code += "\n.data\n.align 3\n";
    code += "res_history: .skip " + std::to_string(todasAsLinhas.size() * 8) + "\n";
    for (auto const& [nome, valor] : memoriaVariaveis) code += nome + ": .double 0.0\n";
    code += "\n@ Pool de Literais\n";
    for (const std::string& num : literais) {
        std::string labelNum = num;
        for(char &c : labelNum) if(c == '.') c = '_';
        code += "lit_" + labelNum + ": .double " + num + "\n";
    }
    return code;
}

//// ALUNO 4
void exibirResultados(const std::vector<std::string>& linhas, const std::vector<double>& resultados) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "                   TRADUTOR RPN -> ARMv7\n";
    std::cout << std::string(60, '=') << "\n";
    printf(" %-4s | %-32s | %-10s\n", "ID", "EXPRESSAO", "RESULTADO");
    std::cout << std::string(60, '-') << "\n";

    for (size_t i = 0; i < linhas.size(); ++i) {
        if (i < resultados.size()) {
            printf(" %02zu   | %-32s | %.4f\n", i+1, linhas[i].c_str(), resultados[i]);
        }
    }
    std::cout << std::string(60, '=') << "\n";
    std::cout << "[INFO] Arquivos 'saida.s' e 'tokens.txt' atualizados.\n";
}

//// MAIN
int main(int argc, char* argv[]) {
    // Validação de Argumento
    if(argc != 2) {
        std::cout << "\nERRO: Arquivo de entrada nao fornecido!" << std::endl;
        std::cout << "Uso: ./meu_compilador teste.txt\n" << std::endl;
        return 1;
    }

    // Leitura
    std::vector<std::string> linhas;
    lerArquivo(argv[1], linhas);
    if (linhas.empty()) return 1;

    std::vector<std::vector<Token>> todasAsLinhas;
    std::vector<Token> listaGeralTokens;

    // Processamento
    for (const std::string& l : linhas) {
        std::vector<Token> tokens;
        parseExpressao(l, tokens);
        if (!tokens.empty()) {
            executarExpressao(tokens);
            todasAsLinhas.push_back(tokens);
            for(auto t : tokens) listaGeralTokens.push_back(t); //Salvando todos os tokens em uma lista geral para gerar o arquivo de tokens depois
        }
    }

    // Gerando o arquivo de assembly com todas as linhas
    std::string assembly = gerarAssembly(todasAsLinhas);
    std::ofstream outAsm("saida.s");
    outAsm << assembly;
    outAsm.close();

    // Gerando o arquivo de tokens com todos os tokens lidos
    std::ofstream outTok("tokens.txt");
    for(auto t : listaGeralTokens) outTok << t.tipo << "," << t.valor << "\n";
    outTok.close();

    // Exibição
    exibirResultados(linhas, historicoRES);

    return 0;
}