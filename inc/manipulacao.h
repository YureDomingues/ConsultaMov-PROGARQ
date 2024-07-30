/*
Este arquivo é referente à manipulação "direta" dos arquivos de entrada e saída, seja fazendo a leitura/escrita do arquivo como um todo ou fazendo pequenas modificações em transferindo para dados 

*/


#ifndef MANIPULACAO_H
#define MANIPULACAO_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

ifstream abrirArquivoEntrada(string nomeDoArquivo);
ofstream abrirArquivoSaida(string nomeDoArquivo);
void fecharArquivos(ifstream* input, ofstream* output);
void fluxoParaTransacao(stringstream* ss, Transacao* t);
void alocarNoFluxoAString(string linha, stringstream* ss, Transacao *t);
ofstream abrirLogFile(string nomeDoArquivoLog);
ofstream abrirArquivoBinSaida(string locArquivoBin);
void escreverStringEmArquivoBin(ofstream *arqBin, string agenciaEConta, double movEspecie, double movEletronica, int numTransacoes);


#endif /*TRASACOES_H*/
