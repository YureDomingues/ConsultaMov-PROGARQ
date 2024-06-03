#include "transacoes.h"

int main(){
    Transacao t;
    ifstream inFile("transacoes.csv");
    ofstream outFile("teste.dat");
    string linha;
    stringstream ss;

    if(!inFile.is_open() || !outFile.is_open()){
        cout << "Falha ao abrir algum dos arquivos!" << endl;
        exit(EXIT_FAILURE);
    }

    
    for(int i=0; i<=5; i++){
        getline(inFile, linha);
    
        alocarNoFluxoAString(linha, &ss, &t);

        outFile.write(linha.c_str(), linha.length());
        printf("\n\n Linha  iteração [%d]: %s", i, linha.c_str());
        outFile.put('\n');

        // imprimirTransacao(t);
    }


    inFile.close();
    outFile.close();


    return 0;
}