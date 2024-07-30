#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

#include <map>

#include <tuple>

#include "../headers/consulta.h"
#include "../headers/manipulacao.h"

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
    int getMes()
    {
        return mes;
    }
    int getAno()
    {
        return ano;
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

void gerarConsolidadasBin(map<string, tuple<double, double, int>> *mapaTransacoes){

int numeroDeTransacoes = 0;

for (auto v : *mapaTransacoes){

    stringstream valorEsp;
    stringstream valorEle;
    int nTransacoesConta;

    valorEsp << fixed << showpoint;
    valorEsp << setprecision(2);
    valorEle << fixed << showpoint;
    valorEle << setprecision(2);

    string conteudo = v.first;

    valorEsp << get<0>(v.second);
    valorEle << get<1>(v.second);
    nTransacoesConta = get<2>(v.second);

    numeroDeTransacoes += nTransacoesConta;

    cout << v.first << ' ' << valorEsp.str() << ' ' << valorEle.str() << ' ' << nTransacoesConta << endl;
}

    cout << "Numero de transações do mês: " << numeroDeTransacoes << endl;
}

void realizarConsultaData()
{
    Data data;
    
    ofstream logFileOut = abrirLogFile("LOG.txt");
    ifstream inFile = abrirArquivoEntrada("transacoes.csv");
    
    Transacao t;
    string linha;
    stringstream ss;
    map<string, tuple<double, double, int>> mapaTransacoes;

    if (constaNoLog(data))
    {
        cout << "A consulta já foi realizada, abrindo banco de dados..." << endl;
        /**/


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
        
        gerarConsolidadasBin(&mapaTransacoes);
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