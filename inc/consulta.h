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

void realizarConsultaData();



#endif 