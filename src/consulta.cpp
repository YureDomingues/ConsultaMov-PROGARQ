#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

#include <ctime>
#include <chrono>

#include <map>
#include <tuple>

#include "../inc/consulta.h"
#include "../inc/manipulacao.h"

using namespace std;

void imprimirPrimeiraLinha(){
    cout << "|--------|---------|---------|--|" << endl; 
}

//Essa função de imprimir Linha fica em loop, por isso não foi colocada na mesma função de impressão

void imprimirLinhaTransacao(string agenciaEConta, double movEspecie, double movEletronica, int numeroTransacoes){
    cout << "|" << setw(8) << agenciaEConta  << "|" 
         << setw(9) << fixed << setprecision(2) << movEspecie << "|" 
         << setw(9) << fixed << setprecision(2) << movEletronica << "|" 
         << setw(2) << numeroTransacoes << "|" << endl;
};

void imprimirInfoFinal(int registros, int transacoes){
    cout << "|--------|---------|---------|--|" << endl;
    cout << "|AG-CONTA|MOV.ESP. |MOV.ELET.|NT|" << endl;
    cout << "|--------|---------|---------|--|" << endl;
    cout << "|" << setw(32) <<              "|" << endl;
    cout << "|Número de registros: " << setw(10) << registros << "|" << endl;
    cout << "|Numero de transações: " << setw(9) << transacoes << "|" << endl;
    cout << "|--------|---------|---------|--|" << endl;

    //Obs: O número de transações é diferente do número de registros. Uma conta pode fazer mais de uma transação, mas ela é apenas um registro

}

//Função para retornar uma string contendo a Data e Horario no formato YYYY-MM-DD HH:MM:SS
string getTimeStr(){
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());

    string s(20, '\0'); //YYYY-MM-DD HH:MM:SS\0    (20 caracteres)
    strftime(&s[0], s.size(), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return s;
}


//Função que gera os arquivos binários e imprime na tela quando uma data é consultada pela primeira vez
void gerarConsolidadasBin(map<string, tuple<double, double, int> > *mapaTransacoes, Data data){

    int numeroDeTransacoes = 0;
    int numeroDeRegistros = 0;
    stringstream nomeArquivoBinario;
    stringstream mes;
    stringstream ano; 
    string pastaBinarios = "./bin/";

    mes << std::setw(2) << std::setfill('0') << data.getMes();
    ano << std::setw(4) << std::setfill('0') << data.getAno();

    nomeArquivoBinario << "consolidadas" << mes.str() << ano.str() << ".bin";

    ofstream arqBin = abrirArquivoEscrita(pastaBinarios + nomeArquivoBinario.str(), 'b');
    
    imprimirPrimeiraLinha();

    for(auto v : *mapaTransacoes){

        double valorEsp;
        double valorEle;
        int nTransacoesConta;

        numeroDeRegistros++;

        string agenciaEConta = v.first; //retorna o vetor "Agencia-Conta" que serve de chave no map

        valorEsp = get<0>(v.second); //retorna valor Movimentado em Espécie pela conta
        valorEle = get<1>(v.second); //retorna valor Movimentado Eletronicamente pela conta
        nTransacoesConta = get<2>(v.second); //returna numero de transacoes realizada pela conta 

        numeroDeTransacoes += nTransacoesConta; //Acumulador para todas as transações efetuadas no mês

        escreverDadosArquivoBin(&arqBin, agenciaEConta, get<0>(v.second), get<1>(v.second), nTransacoesConta);

        imprimirLinhaTransacao(agenciaEConta, valorEsp, valorEle, nTransacoesConta);
    }

    imprimirInfoFinal(numeroDeRegistros, numeroDeTransacoes);

    arqBin.close();
}

//A consulta pela existência do arquivo binário é feita pelo arquivo LOG, se existir uma data no log, o arquivo binario referente à ela também existe
bool constaNoLog(Data data){

    ifstream inFile = abrirArquivoLeitura("LOG.txt", 'i');
    string linha;
    bool consta = false;

    while (getline(inFile, linha))
    {
        if (linha == data.dataToString())
            consta = true;
    }

    inFile.close();
    return consta;
}

string gerarStringAgenciaEConta(int agencia, int conta){
    stringstream ss;
    ss << agencia << "-" << conta;
    return (ss.str());
}

void realizarFiltragem(ifstream *arqBinario, ofstream *logFile){
    double valorMinimoEsp = 0;
    double valorMinimoEle = 0;
    int numeroDeRegistros = 0;
    int numeroDeTransacoes = 0;
    string e_ou;
    stringstream dadosLog;
    InfoConsolidada info;
    
    cout << "O filtro irá procurar todos as transações que movimentaram em espécie no mínimo: ";
    cin >> valorMinimoEsp;
    cout << "O filtro irá procurar todos as transações que movimentaram eletronicamente no mínimo: ";
    cin >> valorMinimoEle;

    do{
        cout << "Você deseja que o filtro seja E ou OU: ";
        cin >> e_ou;
    }while(e_ou != "E" && e_ou != "e"  && e_ou != "OU" && e_ou != "ou");
    
    cout << endl;

    imprimirPrimeiraLinha();

    while(true){
        info = lerDadosArquivoBin(arqBinario);
        
        if(arqBinario->eof()){
            break;
        }

        if(e_ou == "OU" || e_ou == "ou"){
            if (valorMinimoEsp <= info.movEspecie || valorMinimoEle <= info.movEletronica ){
                numeroDeRegistros++;
                numeroDeTransacoes += info.nTransacoes;
                imprimirLinhaTransacao(gerarStringAgenciaEConta(info.agencia, info.conta), info.movEspecie, info.movEletronica, info.nTransacoes);
            }
        }else{
            if(valorMinimoEsp <= info.movEspecie && valorMinimoEle <= info.movEletronica){
                numeroDeRegistros++;
                numeroDeTransacoes += info.nTransacoes;
                imprimirLinhaTransacao(gerarStringAgenciaEConta(info.agencia, info.conta), info.movEspecie, info.movEletronica, info.nTransacoes);
            }
        }
    }

    imprimirInfoFinal(numeroDeRegistros, numeroDeTransacoes);

    dadosLog << getTimeStr().substr(0, 19) << ' ' << valorMinimoEsp << ' ' << valorMinimoEle << ' ' << e_ou << ' ' << numeroDeRegistros;

    logFile->write(dadosLog.str().c_str(), dadosLog.str().length());
    logFile->put('\n');
}

void naoRealizarFiltragem(ifstream *arqBinario, ofstream *logFile){
    InfoConsolidada info;
    int numeroDeRegistros = 0;
    int numeroDeTransacoes = 0;

    imprimirPrimeiraLinha();

    while(true){
        info = lerDadosArquivoBin(arqBinario);
        
        if(arqBinario->eof()){
            break;
        }

        numeroDeRegistros++;
        numeroDeTransacoes += info.nTransacoes;

        imprimirLinhaTransacao(gerarStringAgenciaEConta(info.agencia, info.conta), info.movEspecie, info.movEletronica, info.nTransacoes);

     }

     imprimirInfoFinal(numeroDeRegistros, numeroDeTransacoes);

}

//Função principal chamada na main que dá inicio a todo processo de consulta
void realizarConsultaData(){
    Data data;
    
    ofstream logFileOut = abrirArquivoEscrita("LOG.txt", 'a');
    ifstream inFile = abrirArquivoLeitura("transacoes.csv", 'i');
    
    Transacao t;
    string linha;
    stringstream ss;

    //Formato mapa<"Agencia-Conta", <valorMovEsp, valorMovEle, nTransacoes>>;
    map<string, tuple<double, double, int>> mapaTransacoes;


    if (constaNoLog(data)){
        char escolha;
        stringstream mes, ano;
        stringstream locArquivoBin;
        
        cout << "\nA consulta já foi realizada..." << endl;
        cout << "\nVocê deseja realizar uma filtragem? (S/N)" << endl;

        do{
            cin >> escolha;
        }while(escolha != 'S' && escolha != 's' && escolha != 'N' && escolha != 'n');
        

        mes << std::setw(2) << std::setfill('0') << data.getMes(); //Mês com 2 digitos
        ano << std::setw(4) << std::setfill('0') << data.getAno(); //Ano com 4 dígitos

        locArquivoBin << "./bin/" << "consolidadas" << mes.str() << ano.str() << ".bin"; 
        ifstream arqBinario = abrirArquivoLeitura(locArquivoBin.str(), 'b');
        
        if(escolha == 'S' || escolha == 's'){
            realizarFiltragem(&arqBinario, &logFileOut);
        }else{
            naoRealizarFiltragem(&arqBinario, &logFileOut);
        }
            
        arqBinario.close();
    }
    else{
        cout << "\nRealizando a consulta pela primeira vez..." << endl;

        logFileOut.write(data.dataToString().c_str(), data.dataToString().length());
        logFileOut.put('\n');

        while (getline(inFile, linha)){

            alocarNoFluxoAString(linha, &ss, &t);

            if (data.dataIgual(t.ano, t.mes)){
                
                string agenciaEConta = to_string(t.agenciaPrincipal) + "-" + to_string(t.contaPrincipal);

                if (t.contaComplementar == -1 || t.agenciaComplementar == -1)
                {
                    get<0>(mapaTransacoes[agenciaEConta]) += max(t.valor, -t.valor); // Valor movimentado em especie
                }
                else
                {
                    get<1>(mapaTransacoes[agenciaEConta]) += max(t.valor, -t.valor); // Valor movimentado eletronicamente
                }

                get<2>(mapaTransacoes[agenciaEConta])++;
            }
        
        }

        gerarConsolidadasBin(&mapaTransacoes, data);
    }

    logFileOut.close();
    inFile.close();
}