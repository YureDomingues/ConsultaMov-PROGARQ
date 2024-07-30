#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "../inc/consulta.h"
#include "../inc/manipulacao.h"

ifstream abrirArquivoEntrada(string nomeDoArquivo){
    ifstream inFile(nomeDoArquivo);

    if(!inFile.is_open()){
        cout << "Houve um erro ao ler o arquivo " << nomeDoArquivo << "!" << endl;
        exit(EXIT_FAILURE);
    }  

    return inFile;
}

ofstream abrirArquivoSaida(string nomeDoArquivo){
    ofstream outFile(nomeDoArquivo);

    if(!outFile.is_open()){
        cout << "Houve um erro ao abrir o arquivo de saída '" << nomeDoArquivo << "'!" << endl;
        exit(EXIT_FAILURE);
    }  

    return outFile;
}

ofstream abrirLogFile(string nomeDoArquivoLog){
    ofstream logFile(nomeDoArquivoLog, ios::app); //abrindo em modo append
    if(!logFile.is_open()){
        cout << "Houve um erro ao abrir o LOG!" << endl;
        exit(EXIT_FAILURE);
    }  
    return logFile;
}

ofstream abrirArquivoBinSaida(string locArquivoBin){
    ofstream binFile(locArquivoBin, ios::binary); 
    if(!binFile.is_open()){
        cout << "Houve um erro ao abrir o arquivo " << locArquivoBin << "!" << endl;
        exit(EXIT_FAILURE);
    }  
    return binFile;
}

ifstream abrirArquivoBinEntrada(string locArquivoBin){
    ifstream binFile(locArquivoBin, ios::binary); 
    if(!binFile.is_open()){
        cout << "Houve um erro ao abrir o arquivo " << locArquivoBin << "!" << endl;
        exit(EXIT_FAILURE);
    }  
    return binFile;
}

void fecharArquivos(ifstream* input, ofstream* output){
    input->close();
    output->close();
}

void fluxoParaTransacao(stringstream* ss, Transacao* t){
    string dia;
    string mes;
    string ano;
    string agenciaPrincipal; 
    string conta1;
    string valor;                  
    string agencia2; 
    string contaComplementar; 

    getline(*ss,dia,',');
    getline(*ss,mes,',');
    getline(*ss,ano,',');
    getline(*ss,agenciaPrincipal,',');
    getline(*ss,conta1,',');
    getline(*ss,valor,',');
    getline(*ss,agencia2,',');
    getline(*ss,contaComplementar,',');

    t->dia = stoi(dia);
    t->mes = stoi(mes);
    t->ano = stoi(ano);
    t->agenciaPrincipal = stoi(agenciaPrincipal);
    t->contaPrincipal = stoi(conta1);
    t->valor = stof(valor);

    //Estou assumindo que se não tiver uma segunda agencia também não terá uma segunda conta
    if(agencia2.length() != 0){
        t->agenciaComplementar = stoi(agencia2);
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