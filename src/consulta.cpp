#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "../headers/consulta.h"
#include "../headers/manipulacao.h"

#include <map>

using namespace std;

Transacao* verificaDataTransacao(Transacao* t, int mes, int ano){
    if(t->mes == mes && t->ano == ano){
        return t;
    }else{
        return NULL;
    }
}

class Data{
    int mes;
    int ano;

    public:
    Data(){
        coletaData();
    }
    Data(int mes, int ano){
        this->mes = mes;
        this->ano = ano;
    }
    int getMes(){
        return mes;
    }
    int getAno(){
        return ano;
    } 
    bool dataValida(){
        return (mes >= 1 && mes <= 12) && (ano > 1900);
    }
    void coletaData(){
        bool validDate = false;
    
        while(!validDate){
            
            cout << "Digite o mês da busca: ";
            cin >> this->mes;
            cout << "Agora digite o ano: ";
            cin >> this->ano;

            if(dataValida()){
                validDate = true;
            }else{
                cout << "A data digitada é invalida.\n" << endl;
                
            }


        }
    }
    string dataToString(){
        return (to_string(mes)+'/'+to_string(ano));
    }
    bool dataIgual(int ano, int mes){
        return (ano == this->ano && mes == this->mes);
    }
};

//Função auxiliar
bool constaNoLog(Data data){
    ifstream inFile = abrirArquivoEntrada("LOG.txt");
    string linha;
    bool consta = false;

    while (getline(inFile, linha)) {
        if(linha == data.dataToString())
            consta = true;
    }

    inFile.close();
    return consta;
}

void gerarConsolidadasBin(Data data){
    Transacao t;
    ifstream inFile = abrirArquivoEntrada("transacoes.csv");
    ofstream outAux = abrirArquivoSaida("auxiliar-consulta.txt");
    ofstream log = abrirLogFile("LOG.txt");
    string linha;
    stringstream ss;
    map<string, TransacaoConsolidada> mapaTransacoes; 
    TransacaoConsolidada tConsolidada(0,0);
    int numeroDeTransacoes = 0;

    while(getline(inFile, linha)){

        alocarNoFluxoAString(linha, &ss, &t);

        if(data.dataIgual(t.ano, t.mes)){
            numeroDeTransacoes++;
            string agenciaEConta = to_string(t.agenciaPrincipal) + "-" + to_string(t.contaPrincipal);

            tConsolidada.adicionarMovimentacaoEspecie(t.valor);
            
            mapaTransacoes[agenciaEConta] = tConsolidada;
            mapaTransacoes.insert(make_pair(agenciaEConta, tConsolidada));
        }

        alocarNoFluxoAString(linha, &ss, &t);

    }

    map<string, TransacaoConsolidada>::iterator it;

    for(it=mapaTransacoes.begin(); it!=mapaTransacoes.end(); ++it){
        string conteudo = it->first;
        outAux.write(it->first.c_str(), it->first.length());
        outAux.put('\n');
    }

    cout << "Numero de transações do mês: " << numeroDeTransacoes << endl;
    
    inFile.close();
    outAux.close();
    log.close();
}

void realizarConsultaData(){
    Data data;
    ofstream logFileOut = abrirLogFile("LOG.txt");
    
    if(constaNoLog(data)){
        cout << "A consulta já foi realizada, abrindo banco de dados..." << endl;
        /**/
    }else{
        logFileOut.write(data.dataToString().c_str(), data.dataToString().length());
        logFileOut.put('\n'); 

        gerarConsolidadasBin(data);

        
    }

    logFileOut.close();
}



void imprimirTransacao(Transacao t){
    cout << "data: " << t.dia << '/' << t.mes << '/' << t.ano << endl;
    cout << "agenciaPrincipal: " << t.agenciaPrincipal << endl;
    cout << "contaPrincipal: " << t.contaPrincipal << endl;
    cout << "valor: " << t.valor << endl;

    if(t.agenciaComplementar != -1){
        cout << "agenciaComplementar: " << t.agenciaComplementar << endl;
        cout << "contaComplementar: " << t.contaComplementar << endl;
    }

    cout << endl;
}