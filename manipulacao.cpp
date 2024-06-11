#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "consulta.h"
#include "manipulacao.h"

ifstream abrirArquivoEntrada(string nomeDoArquivo){
    ifstream inFile(nomeDoArquivo);

    if(!inFile.is_open()){
        cout << "Houve um erro ao ler o arquivo de entrada." << endl;
        exit(EXIT_FAILURE);
    }  

    return inFile;
}

ofstream abrirArquivoSaida(string nomeDoArquivo){
    ofstream outFile(nomeDoArquivo);

    if(!outFile.is_open()){
        cout << "Houve um erro ao abrir o arquivo de saída." << endl;
        exit(EXIT_FAILURE);
    }  

    return outFile;
}

void fecharArquivos(ifstream* input, ofstream* output){
    input->close();
    output->close();
}

void fluxoParaTransacao(stringstream* ss, Transacao* t){
    string dia;
    string mes;
    string ano;
    string agencia1; 
    string conta1;
    string valor;                  
    string agencia2; 
    string conta2; 

    getline(*ss,dia,',');
    getline(*ss,mes,',');
    getline(*ss,ano,',');
    getline(*ss,agencia1,',');
    getline(*ss,conta1,',');
    getline(*ss,valor,',');
    getline(*ss,agencia2,',');
    getline(*ss,conta2,',');

    t->dia = stoi(dia);
    t->mes = stoi(mes);
    t->ano = stoi(ano);
    t->agencia1 = stoi(agencia1);
    t->conta1 = stoi(conta1);
    t->valor = stof(valor);

    //Estou assumindo que se não tiver uma segunda agencia também não terá uma segunda conta
    if(agencia2.length() != 0){
        t->agencia2 = stoi(agencia2);
        t->conta2 = stoi(conta2);
    }else{
        t->agencia2 = -1;
        t->conta2 = -1;
    }
        
}

void alocarNoFluxoAString(string linha, stringstream* ss, Transacao *t){
    (*ss).str(linha);
    fluxoParaTransacao(ss, t);
    (*ss).clear();
}
