#include "Fase1_Lexico.hpp"
#include <fstream>
#include <sstream>

void executarFase1(string arquivoEntrada, string arquivoSaida) {
    ifstream arquivo(arquivoEntrada);
    ofstream saida(arquivoSaida);

    if (!arquivo.is_open() || !saida.is_open()) {
        cout << "[ERRO FASE 1] Nao foi possivel abrir os arquivos." << endl;
        return;
    }

    string linhaTexto;
    int numeroLinha = 1; // NOSSO CONTADOR DE LINHAS
    
    while (getline(arquivo, linhaTexto)) {
        stringstream ss(linhaTexto);
        string palavra;
        
        while (ss >> palavra) {
            string tipo;
            
            if (palavra == "(" || palavra == ")") tipo = "PARENTE";
            else if (palavra == "START" || palavra == "END" || palavra == "IF" || palavra == "WHILE" || palavra == "MEM" || palavra == "RES") tipo = "KEYWORD";
            else if (palavra == "+" || palavra == "-" || palavra == "*" || palavra == "/" || palavra == "|" || palavra == "%" || palavra == "^" || palavra == ">" || palavra == "<" || palavra == "=") tipo = "OPERADOR";
            else if (isdigit(palavra[0])) tipo = "NUMERO"; 
            else tipo = "VARIAVEL"; 

            // AGORA ELE SALVA: TIPO,VALOR,LINHA
            saida << tipo << "," << palavra << "," << numeroLinha << endl;
        }
        numeroLinha++; // Avança a linha
    }

    arquivo.close();
    saida.close();
    cout << "[FASE 1] Analise Lexica concluida! Tokens salvos com rastreio de linha." << endl;
}