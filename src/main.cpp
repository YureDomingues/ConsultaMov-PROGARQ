#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "../inc/consulta.h"
#include "../inc/manipulacao.h"

int main(){
int whilebreak = 0;
    char resp[1];

    while (whilebreak == 0)
    {
        cout << "------------------------------------------------" << endl;
        cout << "Deseja realizar uma busca? (s/n)" << endl;
        cin >> resp[1];
        if (resp[1] == 's' || resp[1] == 'S')
        {
            realizarConsultaData();
        }
        else if (resp[1] == 'n' || resp[1] == 'N')
        {
            whilebreak = 1;
            cout << "Busca finalizada" << endl;
        }
        else
            cout << "Resposta incorreta" << endl;
    }

    return 0;
}