@echo off
echo ===========================================
echo  COMPILANDO E RODANDO OS TESTES DO PARSER
echo ===========================================

g++ -Isrc tests_unit/testeGramatica.cpp src/AnalisadorSintatico.cpp src/ParserLL1.cpp src/LexerInterface.cpp -o testes_unitarios.exe

if %ERRORLEVEL% EQU 0 (
    echo [SUCESSO] Testes compilados! Rodando agora:
    echo.
    .\testes_unitarios.exe
) else (
    echo [FALHA] Erro ao compilar os testes.
)