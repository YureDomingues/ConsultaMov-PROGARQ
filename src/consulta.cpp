#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

#include <chrono>
#include <ctime>

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

class Data
{   
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


// Função auxiliar
bool constaNoLog(Data data)
{
    ifstream inFile = abrirArquivoEntrada("LOG.txt");
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

void gerarConsolidadasBin(map<string, tuple<double, double, int> > *mapaTransacoes, Data data){

    int numeroDeTransacoes = 0;
    stringstream nomeArquivoBinario;
    stringstream mes;
    stringstream ano; 
    string pastaBinarios = "./bin/";

    mes << std::setw(2) << std::setfill('0') << data.getMes();
    ano << std::setw(4) << std::setfill('0') << data.getAno();

    nomeArquivoBinario << "consolidadas" << mes.str() << ano.str() << ".bin";

    ofstream arqBin = abrirArquivoBinSaida(pastaBinarios + nomeArquivoBinario.str());
    
    for(auto v : *mapaTransacoes){

        stringstream valorEsp;
        stringstream valorEle;
        int nTransacoesConta;

        valorEsp << fixed << showpoint;
        valorEsp << setprecision(2);
        valorEle << fixed << showpoint;
        valorEle << setprecision(2);

        string agenciaEConta = v.first;

        valorEsp << get<0>(v.second);
        valorEle << get<1>(v.second);
        nTransacoesConta = get<2>(v.second);

        numeroDeTransacoes += nTransacoesConta;

        escreverDadosArquivoBin(&arqBin, agenciaEConta, get<0>(v.second), get<1>(v.second), nTransacoesConta);

        //ImprimindoInformaçõesNoConsole
        cout << agenciaEConta << ' ' << valorEsp.str() << ' ' << valorEle.str() << ' ' << nTransacoesConta << endl;
    }

    cout << "Numero de transações do mês: " << numeroDeTransacoes << endl;

    arqBin.close();
}

void realizarConsultaData(){
    Data data;
    
    ofstream logFileOut = abrirLogFile("LOG.txt");
    ifstream inFile = abrirArquivoEntrada("transacoes.csv");
    
    Transacao t;
    string linha;
    stringstream ss;

    map<string, tuple<double, double, int>> mapaTransacoes;
    
    int e_ou = -1;

    if (constaNoLog(data)){
        char escolha;
        bool filtra = false;
        bool uniao;
        double valorMinimoEsp = 0;
        double valorMinimoEle = 0;
        int numeroRegistros = 0;
        
        cout << "A consulta já foi realizada..." << endl;
        cout << "Você deseja realizar uma filtragem? (S/N)" << endl;
        cin >> escolha;

        if(escolha == 'S' || escolha == 's'){
            filtra = true;
            cout << "O filtro irá procurar todos as transações que movimentaram em espécie no mínimo: ";
            cin >> valorMinimoEsp;
            cout << "O filtro irá procurar todos as transações que movimentaram eletronicamente no mínimo: ";
            cin >> valorMinimoEle;

            cout << "Você deseja que o filtro seja E(0) ou OU(1): ";
            cin >> e_ou;

            if(e_ou == 0){
                uniao = false;
            }else{
                uniao = true;
            }
            
        }
   

        stringstream mes, ano;
        stringstream locArquivoBin;
        InfoConsolidada info;

        mes << std::setw(2) << std::setfill('0') << data.getMes();
        ano << std::setw(4) << std::setfill('0') << data.getAno();
        
        // cout << mes.str() << "/" << ano.str() << endl;

        locArquivoBin << "./bin/" << "consolidadas" << mes.str() << ano.str() << ".bin"; 

        ifstream inpCons = abrirArquivoBinEntrada(locArquivoBin.str());

        while(!inpCons.eof()){
            info = lerDadosArquivoBin(&inpCons);
            if(filtra){
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

            }else{
                numeroRegistros++;
                cout << info.agencia << "-" << info.conta << " " << info.movEspecie << " " << info.movEletronica << " " << info.nTransacoes << endl;   
            }
             
        }

        cout << "Número de registros: " << numeroRegistros << endl;

        stringstream dadosLog;
        string fluxoEscolha;

        if(e_ou == 0){
            fluxoEscolha = "E";
        }else{
            fluxoEscolha = "OU";
        }

        dadosLog << getTimeStr().substr(0, 19) << ' ' << valorMinimoEsp << ' ' << valorMinimoEle << ' ' << fluxoEscolha << ' ' << numeroRegistros;

        logFileOut.write(dadosLog.str().c_str(), dadosLog.str().length());
        logFileOut.put('\n');
    }
    else
    {
        logFileOut.write(data.dataToString().c_str(), data.dataToString().length());
        logFileOut.put('\n');

        while (getline(inFile, linha)){

        alocarNoFluxoAString(linha, &ss, &t);

        if (data.dataIgual(t.ano, t.mes)){
            
            string agenciaEConta = to_string(t.agenciaPrincipal) + "-" + to_string(t.contaPrincipal);

            if (t.contaComplementar == -1 || t.agenciaComplementar == -1)
            {
                get<0>(mapaTransacoes[agenciaEConta]) += max(t.valor, -t.valor); // Valor Especie
            }
            else
            {
                get<1>(mapaTransacoes[agenciaEConta]) += max(t.valor, -t.valor); // Valor Eletronico
            }

            get<2>(mapaTransacoes[agenciaEConta])++;
        }
        
        }
        
        gerarConsolidadasBin(&mapaTransacoes, data);
    }

    logFileOut.close();
    inFile.close();
}

void imprimirTransacao(Transacao t)
{
    cout << "data: " << t.dia << '/' << t.mes << '/' << t.ano << endl;
    cout << "agenciaPrincipal: " << t.agenciaPrincipal << endl;
    cout << "contaPrincipal: " << t.contaPrincipal << endl;
    cout << "valor: " << t.valor << endl;

    if (t.agenciaComplementar != -1)
    {
        cout << "agenciaComplementar: " << t.agenciaComplementar << endl;
        cout << "contaComplementar: " << t.contaComplementar << endl;
    }

    cout << endl;
}