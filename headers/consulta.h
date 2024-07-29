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

class TransacaoConsolidada{
    double valorMovimentadoEmEspecie;
    double valorMovimentadoEletronicamente;

    public:
        TransacaoConsolidada(double valorEspecie,double valorEletronica){
            valorMovimentadoEmEspecie = valorEspecie;
            valorMovimentadoEletronicamente = valorEletronica;
        }
        void adicionarMovimentacaoEspecie(double valorEspecie){
            valorMovimentadoEmEspecie += valorEspecie;
        }
        void adicionarMovimentacaoEletronica(double valorEletronica){
            valorMovimentadoEletronicamente += valorEletronica;
        }
};

Transacao* verificaDataTransacao(Transacao* t, int mes, int ano);
void imprimirTransacao(Transacao t);
void realizarConsultaData();



#endif 