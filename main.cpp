#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "consulta.h"
#include "manipulacao.h"

int main(){
    Transacao t;
    ifstream inFile = abrirArquivoEntrada("transacoes.csv");
    ofstream outFile = abrirArquivoSaida("teste.dat");
    string linha;
    stringstream ss;
    
    for(int i=0; i<=5; i++){
        getline(inFile, linha);
    
        alocarNoFluxoAString(linha, &ss, &t);

        outFile.write(linha.c_str(), linha.length());
        // printf("\n\n Linha  iteração [%d]: %s", i, linha.c_str()); //Teste
        outFile.put('\n');

        // imprimirTransacao(t);
        
    }

    realizarConsultaData();


    fecharArquivos(&inFile, &outFile);

    return 0;
}