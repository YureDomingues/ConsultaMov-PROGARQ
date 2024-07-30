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
    int agenciaPrincipal; 
    int contaPrincipal;
    double valor;                  
    int agenciaComplementar; 
    int contaComplementar;   
}Transacao;

typedef struct infoConsolidada{
    int agencia;
    int conta;
    double movEspecie;
    double movEletronica;
    int nTransacoes;
}InfoConsolidada;

Transacao* verificaDataTransacao(Transacao* t, int mes, int ano);
void imprimirTransacao(Transacao t);
void realizarConsultaData();



#endif 