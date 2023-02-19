#include "csvData.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    string nomeArquivo;
    cout << "Digite o nome do arquivo: ";
    cin >> nomeArquivo;
    //le o arquivo binario e escreve em um txt
    csvData dado;
    dado.fazTxT(nomeArquivo);
    return 0;
}