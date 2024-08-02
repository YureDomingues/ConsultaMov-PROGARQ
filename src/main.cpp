#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "../inc/consulta.h"
#include "../inc/manipulacao.h"

int main(){
    bool whileBreak = false;
    char resp;

    while (!whileBreak)
    {
        cout << "\nDeseja realizar uma nova busca? (s/n)" << endl;
        cin >> resp;
        if (resp == 's' || resp == 'S')
        {
            realizarConsultaData();
        }
        else if (resp == 'n' || resp == 'N')
        {
            whileBreak = true;
            cout << "Fechando..." << endl;
        }
        else
            cout << "Escolha uma opção válida" << endl;
    }

    return 0;
}