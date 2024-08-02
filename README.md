### Trabalho da disciplina de Programação com Arquivos

#### Autoria: 

[Yure Domingues](https://github.com/YureDomingues)
[Isabella Torres](https://github.com/isabellahtorres)

#### Organização dos arquivos

Para o bom funcionamento do programa é necessário que as pastas e arquivos estejam dispostos da seguinte forma:

📁 bin
📁 inc
├── consulta.h
├── manipulacao.h
📁 src
├── consulta.cpp
├── main.cpp
├── manipulacao.cpp
Makefile
transacoes.csv
README

#### Compilação

Para facilitar o processo de desenvolvimento, criamos um arquivo Makefile com os comandos:

    make all (ou make): Compila o programa
    make run: Compila usando g++ e abre o main.out em seguida
    make clean: Deleta todos os arquivos binarios, o executavel e o LOG.txt

Caso não queira compilar usando usando o Makefile, pode usar o comando

g++  -Wall -std=c++17   "src/main.cpp" "src/consulta.cpp" "src/manipulacao.cpp" -o main.out

