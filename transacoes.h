#ifndef TRANSACOES_H
#define TRAnSACOES_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

/*
Estrutura da linha na base de dados:

dia,mes,ano,agencia1,conta1,valorMovimentado,agencia2,conta2

valorMovimentado > 0 -> Depósito
ValorMovimentado < 0 -> Saque

agencia2 e conta2 são opcionais, se não constarem a movimentação foi em espécie na própria agencia1 e conta1;

*/

struct Transacao{
    string dia;
    string mes;
    string ano;
    string agencia1; 
    string conta1;
    string valor;                  
    string agencia2; 
    string conta2;   
};

typedef struct Transacao Transacao;

void fluxoParaStruct(stringstream* ss, Transacao* t);
void imprimirTransacao(Transacao t);
void alocarNoFluxoAString(string linha, stringstream* ss, Transacao *t);


#endif /*TRASACOES_H*/
