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

Transacao *verificaDataTransacao(Transacao *t, int mes, int ano)
{
    if (t->mes == mes && t->ano == ano)
    {
        return t;
    }
    else
    {
        return NULL;
    }
}

class Data{   

    int dia;
    int mes;
    int ano;

public:
    Data()
    {
        coletaData();
    }
    Data(int mes, int ano)
    {
        this->mes = mes;
        this->ano = ano;
    }
    Data(int dia, int mes, int ano){
        this->dia = dia;
        this->mes = mes;
        this->ano = ano;
    }
    int getDia(){
        return dia;
    }
    int getMes()
    {
        return mes;
    }
    int getAno()
    {
        return ano;
    }
    string getData(){
        stringstream dt;
        dt << dia << "/" << mes << "/" <<ano;
        return dt.str();

    }
    bool dataValida()
    {
        return (mes >= 1 && mes <= 12) && (ano > 1900);
    }
    void coletaData()
    {
        bool validDate = false;

        while (!validDate)
        {

            cout << "Digite o mês da busca: ";
            cin >> this->mes;
            cout << "Agora digite o ano: ";
            cin >> this->ano;

            if (dataValida())
            {
                validDate = true;
            }
            else
            {
                cout << "A data digitada é invalida.\n"
                     << endl;
            }
        }
    }
    string dataToString()
    {
        return (to_string(mes) + '/' + to_string(ano));
    }
    bool dataIgual(int ano, int mes)
    {
        return (ano == this->ano && mes == this->mes);
    }
};

string getTimeStr(){
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());

    string s(20, '\0'); //YYYY-MM-DD HH:MM:SS\0    (20 caracteres)
    strftime(&s[0], s.size(), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return s;
}

void gerarConsolidadasBin(map<string, tuple<double, double, int> > *mapaTransacoes, Data data){

    int numeroDeTransacoes = 0;
    stringstream nomeArquivoBinario;
    stringstream mes;
    stringstream ano; 
    string pastaBinarios = "./bin/";

    mes << std::setw(2) << std::setfill('0') << data.getMes();
    ano << std::setw(4) << std::setfill('0') << data.getAno();

    nomeArquivoBinario << "consolidadas" << mes.str() << ano.str() << ".bin";

    ofstream arqBin = abrirArquivoEscrita(pastaBinarios + nomeArquivoBinario.str(), 'b');
    
    for(auto v : *mapaTransacoes){

        stringstream valorEsp;
        stringstream valorEle;
        int nTransacoesConta;

        valorEsp << fixed << showpoint;
        valorEsp << setprecision(2);
        valorEle << fixed << showpoint;
        valorEle << setprecision(2);

        string agenciaEConta = v.first; //retorna o vetor "Agencia-Conta" que serve de chave no map

        valorEsp << get<0>(v.second); //retorna valor Movimentado em Espécie pela conta
        valorEle << get<1>(v.second); //retorna valor Movimentado Eletronicamente pela conta
        nTransacoesConta = get<2>(v.second); //returna numero de transacoes realizada pela conta 

        numeroDeTransacoes += nTransacoesConta; //Acumulador para todas as transações efetuadas no mês

        escreverDadosArquivoBin(&arqBin, agenciaEConta, get<0>(v.second), get<1>(v.second), nTransacoesConta);

        //ImprimindoInformaçõesNoConsole
        cout << agenciaEConta << ' ' << valorEsp.str() << ' ' << valorEle.str() << ' ' << nTransacoesConta << endl;

        /*No arquivo binário eu estou escrevendo as informações como (int int double double int), diferente do que estou
        fazendo no cout do console, que printa a string. Essa mudança é apenas para que o arquivo não fique 'legível' no binario
        e também para ter precisão de duas casas decimais quando aparece na tela.*/
    }

    cout << "Numero de transações do mês: " << numeroDeTransacoes << endl;

    arqBin.close();
}

// Função auxiliar
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

void realizarFiltragem(ifstream *arqBinario, ofstream *logFile){
    bool uniao;
    double valorMinimoEsp = 0;
    double valorMinimoEle = 0;
    int numeroRegistros = 0;
    string e_ou;
    stringstream dadosLog;
    InfoConsolidada info;
    
    cout << "O filtro irá procurar todos as transações que movimentaram em espécie no mínimo: ";
    cin >> valorMinimoEsp;
    cout << "O filtro irá procurar todos as transações que movimentaram eletronicamente no mínimo: ";
    cin >> valorMinimoEle;

    do{
        cout << "Você deseja que o filtro seja E ou OU ";
        cin >> e_ou;
    }while(e_ou != "E" && e_ou != "e"  && e_ou != "OU" && e_ou != "ou");
    
    cout << endl;

    if(e_ou.compare("E")){
        uniao = false;
    }else{
        uniao = true;
    }

    while(true){
        info = lerDadosArquivoBin(arqBinario);
        
        if(arqBinario->eof()){
            break;
        }

        if(uniao){
            if (valorMinimoEsp <= info.movEspecie || valorMinimoEle <= info.movEletronica ){
                    numeroRegistros++;
                    cout << info.agencia << "-" << info.conta << " " << info.movEspecie << " " << info.movEletronica << " " << info.nTransacoes << endl;
            }
        }else{
            if(valorMinimoEsp <= info.movEspecie && valorMinimoEle <= info.movEletronica){
                numeroRegistros++;
                cout << info.agencia << "-" << info.conta << " " << info.movEspecie << " " << info.movEletronica << " " << info.nTransacoes << endl;
            }
        }
    }

    cout << "Número de registros: " << numeroRegistros << endl;

    dadosLog << getTimeStr().substr(0, 19) << ' ' << valorMinimoEsp << ' ' << valorMinimoEle << ' ' << e_ou << ' ' << numeroRegistros;

    logFile->write(dadosLog.str().c_str(), dadosLog.str().length());
    logFile->put('\n');
}

void naoRealizarFiltragem(ifstream *arqBinario, ofstream *logFile){
    InfoConsolidada info;
    int numeroRegistros = 0;

     while(true){
        info = lerDadosArquivoBin(arqBinario);
        
        if(arqBinario->eof()){
            break;
        }

        numeroRegistros++;
        cout << info.agencia << "-" << info.conta << " " << info.movEspecie << " " << info.movEletronica << " " << info.nTransacoes << endl;   

     }

     cout << "Número de registros: " << numeroRegistros << endl;

}

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