# Documentação Teórica do Analisador Sintático (Fase 2)

**Responsabilidade:** Aluno 1  
**Objetivo:** Definição da Gramática, Conjuntos FIRST/FOLLOW e Tabela de Análise LL(1) sem conflitos (Fatorada à Esquerda).

---

## 1. Gramática EBNF (Fatorada)

A gramática abaixo foi estruturada e fatorada à esquerda no nível do escopo do programa para eliminar conflitos e garantir determinismo estrito LL(1).

    programa          -> PARENTE_ESQ START PARENTE_DIR proximo_bloco
    proximo_bloco     -> PARENTE_ESQ corpo_instrucao | ε
    corpo_instrucao   -> END PARENTE_DIR | expressao PARENTE_DIR proximo_bloco
    expressao         -> lista_operandos operador_final
    lista_operandos   -> operando lista_operandos | ε
    operando          -> NUMERO | VARIAVEL | PARENTE_ESQ expressao PARENTE_DIR
    operador_final    -> OP_ARITMETICO | OP_RELACIONAL | KEY_IF | KEY_WHILE | KEY_MEM | KEY_RES

---

## 2. Conjuntos FIRST e FOLLOW

### Conjuntos FIRST
* **FIRST(programa)** = { PARENTE_ESQ }
* **FIRST(proximo_bloco)** = { PARENTE_ESQ, ε }
* **FIRST(corpo_instrucao)** = { END, NUMERO, VARIAVEL, PARENTE_ESQ } U FIRST(operador_final)
* **FIRST(expressao)** = { NUMERO, VARIAVEL, PARENTE_ESQ } U FIRST(operador_final)
* **FIRST(lista_operandos)** = { NUMERO, VARIAVEL, PARENTE_ESQ, ε }
* **FIRST(operando)** = { NUMERO, VARIAVEL, PARENTE_ESQ }
* **FIRST(operador_final)** = { OP_ARITMETICO, OP_RELACIONAL, KEY_IF, KEY_WHILE, KEY_MEM, KEY_RES }

### Conjuntos FOLLOW
* **FOLLOW(programa)** = { EOF }
* **FOLLOW(proximo_bloco)** = { EOF }
* **FOLLOW(corpo_instrucao)** = { EOF }
* **FOLLOW(expressao)** = { PARENTE_DIR }
* **FOLLOW(lista_operandos)** = { FIRST(operador_final) }
* **FOLLOW(operando)** = { NUMERO, VARIAVEL, PARENTE_ESQ } U FIRST(operador_final)

---

## 3. Tabela de Análise LL(1)

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