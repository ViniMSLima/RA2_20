# Compilador RPN - Fase 2 (Analisador Sintático)

Este projeto consiste no desenvolvimento de um **Analisador Sintático (Parser)** descendente recursivo do tipo **LL(1)** para uma linguagem baseada em Notação Polonesa Reversa (RPN), com suporte à geração de código **Assembly ARMv7**.

---

## 🏛️ Informações Institucionais
* **Instituição:** Pontifícia Universidade Católica do Paraná (PUC-PR)
* **Curso:** Engenharia de Computação
* **Período:** 9º Período
* **Disciplina:** Linguagens Formais e Compiladores (LFC)
* **Professor:** Frank de Alcantara

---

## 👥 Grupo
* **Vinícius Lima** - ViniMSLima

---

## 🛠️ Instruções de Compilação e Execução

### **Compilação**
Para compilar o projeto no ambiente Windows (utilizando G++), utilize o script de automação fornecido:

```powershell
.\compilar.bat
```

Ou, se preferir o comando manual via terminal:

```powershell
g++ -Isrc src/*.cpp -o AnalisadorSintatico.exe
```

Execução
O programa recebe o arquivo que deve ser lido como argumento de linha de comando:

```PowerShell
.\AnalisadorSintatico.exe teste1.txt
```

---

## 🧠 Sintaxe das Estruturas de Controle
A linguagem utiliza a lógica de Notação Polonesa Reversa (RPN). As estruturas de controle consomem os blocos (expressões) que as precedem:

IF (Tomada de Decisão): * Sintaxe: ( ( Condição ) ( Ação ) IF )

O primeiro bloco é avaliado; se o resultado for diferente de zero (Verdadeiro), o segundo bloco (ação) é executado.

WHILE (Laço de Repetição): * Sintaxe: ( ( Condição ) ( Ação ) WHILE )

O bloco de ação é repetido enquanto o bloco de condição resultar em um valor verdadeiro (não-zero).

---

## ⚙️ Geração de Código Assembly
Após a análise sintática bem-sucedida, o compilador gera automaticamente o arquivo:

---

## 📄 saida.s

Este código segue a arquitetura **ARMv7** e está pronto para o simulador Cpulator (DE1-SoC).

**Precisão:** Utiliza ponto flutuante de 64 bits (double) via instruções VFP.
**Histórico:** Implementa o comando RES através de um buffer de resultados em memória (.data), permitindo acessar valores de linhas anteriores de forma determinística.

--- 

## 📂 Organização da Documentação
A documentação técnica foi segmentada na pasta /docs conforme exigido pelo edital:

* **Gramatica.md:** Regras de produção em EBNF.
* **FirstFollow.md:** Conjuntos FIRST e FOLLOW.
* **TabelaLL1.md:** Tabela de análise sintática.
* **ArvoreExemplo.md:** Exemplo visual da árvore (AST) gerada.

---

## Robustez e Erros
O parser reporta erros sintáticos detalhados, indicando o número da linha e o token esperado, garantindo conformidade com os critérios de robustez do projeto.