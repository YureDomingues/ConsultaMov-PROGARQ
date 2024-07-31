#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "../inc/consulta.h"
#include "../inc/manipulacao.h"

ifstream abrirArquivoLeitura(string arquivo, char modo){
    
    map<char, ios_base::openmode> modoMap;
    
    modoMap['i'] = ios::in;
    modoMap['b'] = ios::binary | ios::in;
    modoMap['a'] = ios::app;

    if (modoMap.find(modo) == modoMap.end()) {
        cout << "Modo de abertura não reconhecido: " << modo << endl;
        exit(EXIT_FAILURE);
    }

    ifstream inFile(arquivo, modoMap[modo]);

    if(!inFile.is_open()){
        cout << "Houve um erro ao ler o arquivo " << arquivo << "!" << endl;
        cout << "Fechando..." << endl;
        exit(EXIT_FAILURE);
    } 

    return inFile;
}

ofstream abrirArquivoEscrita(string arquivo, char modo){
    map<char, ios_base::openmode> modoMap;
    
    modoMap['o'] = ios::out;
    modoMap['b'] = ios::binary | ios::out;
    modoMap['a'] = ios::app;

    if (modoMap.find(modo) == modoMap.end()) {
        cout << "Modo de abertura não reconhecido: " << modo << endl;
        exit(EXIT_FAILURE);
    }

    ofstream outFile(arquivo, modoMap[modo]);

    if(!outFile.is_open()){
        cout << "Houve um erro ao abrir o arquivo de saída '" << arquivo << "'!" << endl;
        cout << "Fechando..." << endl;
        exit(EXIT_FAILURE);
    }  

    return outFile;
}

void fluxoParaTransacao(stringstream* ss, Transacao* t){
    string dia;
    string mes;
    string ano;
    string agenciaPrincipal; 
    string contaPrincipal;
    string valor;                  
    string agenciaComplementar; 
    string contaComplementar; 

    getline(*ss,dia,',');
    getline(*ss,mes,',');
    getline(*ss,ano,',');
    getline(*ss,agenciaPrincipal,',');
    getline(*ss,contaPrincipal,',');
    getline(*ss,valor,',');
    getline(*ss,agenciaComplementar,',');
    getline(*ss,contaComplementar,',');

    t->dia = stoi(dia);
    t->mes = stoi(mes);
    t->ano = stoi(ano);
    t->agenciaPrincipal = stoi(agenciaPrincipal);
    t->contaPrincipal = stoi(contaPrincipal);
    t->valor = stod(valor);
    
    if(agenciaComplementar.length() != 0){
        t->agenciaComplementar = stoi(agenciaComplementar);
        t->contaComplementar = stoi(contaComplementar);
    }else{
        t->agenciaComplementar = -1;
        t->contaComplementar = -1;
    }
        
}

void alocarNoFluxoAString(string linha, stringstream* ss, Transacao *t){
    (*ss).str(linha);
    fluxoParaTransacao(ss, t);
    (*ss).clear();
}

void escreverDadosArquivoBin(ofstream *arqBin, string agenciaEConta, double movEspecie, double movEletronica, int numTransacoes){
    // O formato da string está AAA-CCCC (AAA é a agencia e CCCC é a conta)
    
    int agencia = stoi(agenciaEConta);
    int conta = stoi(agenciaEConta.substr(4));

    arqBin->write(reinterpret_cast<const char*>(&agencia), sizeof(int));
    arqBin->write(reinterpret_cast<const char*>(&conta), sizeof(int));
    arqBin->write(reinterpret_cast<const char*>(&movEspecie), sizeof(double));
    arqBin->write(reinterpret_cast<const char*>(&movEletronica), sizeof(double));
    arqBin->write(reinterpret_cast<const char*>(&numTransacoes), sizeof(int));
}

InfoConsolidada lerDadosArquivoBin(ifstream *arqBin){
    InfoConsolidada info;
    
    arqBin->read((char*)&info.agencia,sizeof(int));
    arqBin->read((char*)&info.conta,sizeof(int));
    arqBin->read((char*)&info.movEspecie, sizeof(double));
    arqBin->read((char*)&info.movEletronica, sizeof(double));
    arqBin->read((char*)&info.nTransacoes, sizeof(int));

    return info;
}