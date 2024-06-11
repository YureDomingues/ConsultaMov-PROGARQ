#ifndef CONSULTA_H
#define CONSULTA_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

typedef struct transacao{
    int dia;
    int mes;
    int ano;
    int agencia1; 
    int conta1;
    double valor;                  
    int agencia2; 
    int conta2;   
}Transacao;

Transacao* verificaDataTransacao(Transacao* t, int mes, int ano);
void imprimirTransacao(Transacao t);
bool constaNoLog(string data);
bool dataValida(string data);
void realizarConsultaData();



#endif 