/*
Este arquivo é referente à manipulação "direta" dos arquivos de entrada e saída, seja fazendo a leitura/escrita do arquivo como um todo ou retornando um tipo de dado da leitura de um arquivo (O que acontece com a estrutura infoConsolidada)

*/


#ifndef MANIPULACAO_H
#define MANIPULACAO_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

typedef struct infoConsolidada{
    int agencia;
    int conta;
    double movEspecie;
    double movEletronica;
    int nTransacoes;
}InfoConsolidada;

ifstream abrirArquivoLeitura(string arquivo, char modo);
ofstream abrirArquivoEscrita(string arquivo, char modo);
void fluxoParaTransacao(stringstream* ss, Transacao* t);
void alocarNoFluxoAString(string linha, stringstream* ss, Transacao *t);
void escreverDadosArquivoBin(ofstream *arqBin, string agenciaEConta, double movEspecie, double movEletronica, int numTransacoes);
InfoConsolidada lerDadosArquivoBin(ifstream *arqBin);


#endif /*TRASACOES_H*/
