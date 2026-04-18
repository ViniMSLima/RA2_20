@echo off
echo ===========================================
echo  COMPILANDO O PROJETO PRINCIPAL (FASE 2)
echo ===========================================

g++ -Isrc src/main.cpp src/Fase1_Lexico.cpp src/LexerInterface.cpp src/AnalisadorSintatico.cpp src/ParserLL1.cpp src/ParserAST.cpp -o AnalisadorSintatico.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCESSO] Compilacao concluida sem erros!
    echo Para executar o seu compilador, digite no terminal:
    echo .\AnalisadorSintatico.exe teste1.txt
) else (
    echo.
    echo [FALHA] Erro durante a compilacao. Verifique o codigo.
)