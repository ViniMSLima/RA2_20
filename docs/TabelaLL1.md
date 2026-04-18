
## Tabela de Análise LL(1)

A tabela abaixo cruza os Não-Terminais com os Terminais utilizando os conjuntos calculados. Não há nenhuma ambiguidade (células com mais de uma regra), atestando a integridade da gramática LL(1).

*(Nota: Na coluna `OPs / KEYs`, agrupamos os operadores aritméticos, relacionais e as palavras-chave reservadas para condensar a visualização. A derivação ocorre de forma direta para o token lido).*

| Não-Terminal | PARENTE_ESQ (`(`) | PARENTE_DIR (`)`) | START | END | NUMERO | VARIAVEL | OPs / KEYs | EOF |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **programa** | `programa -> PARENTE_ESQ START PARENTE_DIR proximo_bloco` | | | | | | | |
| **proximo_bloco** | `proximo_bloco -> PARENTE_ESQ corpo_instrucao` | | | | | | | `proximo_bloco -> ε` |
| **corpo_instrucao** | `corpo_instrucao -> expressao PARENTE_DIR proximo_bloco` | | | `corpo_instrucao -> END PARENTE_DIR` | `corpo_instrucao -> expressao PARENTE_DIR proximo_bloco` | `corpo_instrucao -> expressao PARENTE_DIR proximo_bloco` | `corpo_instrucao -> expressao PARENTE_DIR proximo_bloco` | |
| **expressao** | `expressao -> lista_operandos operador_final` | | | | `expressao -> lista_operandos operador_final` | `expressao -> lista_operandos operador_final` | `expressao -> lista_operandos operador_final` | |
| **lista_operandos** | `lista_operandos -> operando lista_operandos` | | | | `lista_operandos -> operando lista_operandos` | `lista_operandos -> operando lista_operandos` | `lista_operandos -> ε` | |
| **operando** | `operando -> PARENTE_ESQ expressao PARENTE_DIR` | | | | `operando -> NUMERO` | `operando -> VARIAVEL` | | |
| **operador_final**| | | | | | | `operador_final -> [TOKEN LIDO]` | |
