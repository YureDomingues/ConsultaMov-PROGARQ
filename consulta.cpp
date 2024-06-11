#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "consulta.h"
#include "manipulacao.h"

Transacao* verificaDataTransacao(Transacao* t, int mes, int ano){
    if(t->mes == mes && t->ano == ano){
        return t;
    }else{
        return NULL;
    }
}

bool dataValida(string data) {
    int mes, ano;
    char separador;

    stringstream ss(data);
    ss >> mes >> separador >> ano;
    
    if(separador != '/' || ss.fail()){
        return false;
    }

    if (mes < 1 || mes > 12) {
        return false;
    }

    if (ano <= 0) {
        return false;
    }

    return true;
}

void realizarConsultaData(){
    string data;
    bool validDate = false;
    
    while(!validDate){
        cout << "Digite a data da Consulta no formato MM/AAAA: ";
        cin >> data;

        validDate = dataValida(data);

        if(!validDate){
            cout << "Data Invalida!!" << endl;
        }
    }

    if(constaNoLog(data)){
        /*Todo o código de acesso ao consolidadasMMAAAA.bin*/
    }else{
        ofstream logFileOut = abrirArquivoSaida("LOG.txt");
        logFileOut.write(data.c_str(), data.length());

        /*Código de consulta*/

        logFileOut.close();
    }

}

bool constaNoLog(string data){
    ifstream inFile = abrirArquivoEntrada("LOG.txt");
    string linha;
    bool consta = false;

    while (getline(inFile, linha)) {
        if(linha == data)
            consta = true;
    }

    return consta;
}

void imprimirTransacao(Transacao t){
    cout << "data: " << t.dia << '/' << t.mes << '/' << t.ano << endl;
    cout << "agencia1: " << t.agencia1 << endl;
    cout << "conta1: " << t.conta1 << endl;
    cout << "valor: " << t.valor << endl;

    if(t.agencia2 != -1){
        cout << "agencia2: " << t.agencia2 << endl;
        cout << "conta2: " << t.conta2 << endl;
    }

    cout << endl;
}