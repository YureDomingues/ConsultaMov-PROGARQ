#include "transacoes.h"

void fluxoParaStruct(stringstream* ss, Transacao* t){
    getline(*ss,t->dia,',');
    getline(*ss,t->mes,',');
    getline(*ss,t->ano,',');
    getline(*ss,t->agencia1,',');
    getline(*ss,t->conta1,',');
    getline(*ss,t->valor,',');
    getline(*ss,t->agencia2,',');
    getline(*ss,t->conta2,',');
}

void imprimirTransacao(Transacao t){
    cout << "data: " << t.dia << '/' << t.mes << '/' << t.ano << endl;
    cout << "agencia1: " << t.agencia1 << endl;
    cout << "conta1: " << t.conta1 << endl;
    cout << "valor: " << t.valor << endl;

    if(t.agencia2.compare("")){
        cout << "agencia2: " << t.agencia2 << endl;
        cout << "conta2: " << t.conta2 << endl;
    }

    cout << endl;
}

void alocarNoFluxoAString(string linha, stringstream* ss, Transacao *t){
    (*ss).str(linha);
    fluxoParaStruct(ss, t);
    (*ss).clear();
}