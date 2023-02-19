#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

class csvData {
    private:
        //separacao dos dados
        int ID;
        char name[50];
        char city[50];
        char sport[50]; 
        char event[180];
        char noc[4];
    public:
        //metodos da classe
        void recebeDados();
        void mostraDado();
        void limpaDados();
        void converteDados();
        void adcionaDado();
        void mudaDado();
        void visualizaDados();
        void trocaPosicao();
        void leDado();
        void ordenaArquivos(int nomeArquivo, int inicio, int fim);
        void mergeSort(csvData *vetor, int inicio, int fim);
        void intercala(csvData *vetor, int inicio, int meio, int fim);
        void mesclaArquivos(int numeroArquivos);
        void divideArquivo(int &numeroArquivos);
        void mergeSortExterno();
        void fazTxT(string nomeArquivo);
};

void csvData::recebeDados() {
    //recebe os dados do arquivo data_athlete_event.csv
    ifstream arquivo;
    arquivo.open("data_athlete_event.csv");
    string linha;
    int contaID = -1;
    while (arquivo.good()) {
        getline(arquivo, linha);
        int tam = linha.size(), contaDado = 0, contaAspas = 0, contaPosicaoDado = 0;
        for (int i = 0; i < tam; i++) {
            //checa se o caractere e uma virgula ou se esta entre aspas
            if (linha[i] != ',' || contaAspas == 1) {
                //checa se o caractere eh um "abre aspas", se for, aumenta o contador de aspas
                if (linha[i] == '"' && contaAspas == 0) {
                    contaAspas = 1;
                } else if (linha[i] == '"' && contaAspas == 1) {
                    //checa se o caractere eh um "fecha aspas", se for, diminui o contador de aspas
                    contaAspas = 0;
                }
                if (contaDado == 0) {
                    ID = contaID;
                } else if (contaDado == 1) {
                    name[contaPosicaoDado] = linha[i];
                } else if (contaDado == 2) {
                    city[contaPosicaoDado] = linha[i];
                } else if (contaDado == 3) {
                    sport[contaPosicaoDado] = linha[i];
                } else if (contaDado == 4) {
                    event[contaPosicaoDado] = linha[i];
                } else if (contaDado == 5) {
                    noc[contaPosicaoDado] = linha[i];
                }
                contaPosicaoDado++;
            } else {
            contaDado++;
            contaPosicaoDado = 0;
            }
        }
        converteDados();     
        limpaDados();
        contaID++;
    }
    arquivo.close();
}

void csvData::mostraDado() {
    //metodo que vai mostrar os dados que estao sendo lidos
    cout << "ID: " << ID << " Nome: " << name << " Cidade: " << city << " Esporte: " << sport << " Evento: " << event << " NOC: " << noc << endl;
}

//limpador
void csvData::limpaDados() {
    ID = '\0';
    for (int i = 0; i < 90; i++) {
        name[i] = '\0';
    }
    for (int i = 0; i < 50; i++) {
        city[i] = '\0';
    }
    for (int i = 0; i < 50; i++) {
        sport[i] = '\0';
    }
    for (int i = 0; i < 180; i++) {
        event[i] = '\0';
    }
    for (int i = 0; i < 4; i++) {
        noc[i] = '\0';
    }
}

void csvData::converteDados() {
    //vai pegar os dados do arquivo data_athlete_events.csv e vai converter para binario
    char *vetorChar = new char[sizeof(csvData)];
    memcpy(vetorChar, this, sizeof(csvData));
    ofstream arquivo;
    arquivo.open("data_athlete_event.bin", ios::app | ios::binary);
    if (arquivo.is_open()) {
        arquivo.write(vetorChar, sizeof(csvData));
        arquivo.close();
    }
    else {
        cout << "Erro ao abrir o arquivo" << endl;
    }
}

void csvData::adcionaDado() {
    //esse metodo vai adcionar um novo dado no arquivo data_athlete_event.bin, o posicionando no lugar desejado
    //descobrir o tamanho do arquivo data_athlete_event.bin
    ifstream arquivo;
    arquivo.open("data_athlete_event.bin", ios::binary | ios::app);
    arquivo.seekg(0, arquivo.end);
    long int tamanhoArquivo = arquivo.tellg();
    int tamanhoDado = sizeof(csvData);
    int numeroDados = tamanhoArquivo / tamanhoDado, posicao;   
    arquivo.close();
    //recebe a posicao e os dados
    cout << "Digite a posicao que deseja adcionar o dado: ";
    cin >> posicao;
    if (cin.fail() || posicao > (numeroDados-1) || posicao < 0) {
        cout << "Posicao invalida, ultima posicao possivel: " << (numeroDados-1) << endl;
        return;
    }
    cout << "Digite o ID do dado: ";
    cin >> ID;
    //procurar se o id ja existe
    for (int i = 0; i < numeroDados; i++) {
        csvData dadoAux;
        fstream arquivo2;
        arquivo2.open("data_athlete_event.bin", ios::binary | ios::in | ios::out | ios::ate);
        arquivo2.seekg(i * sizeof(csvData), arquivo2.beg);
        arquivo2.read((char*)&dadoAux, sizeof(csvData));
        if (dadoAux.ID == ID) {
            cout << "ID ja existente" << endl;
            return;
        }
        arquivo2.close();
    }
    cout << "Digite o nome do dado: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "Digite a cidade do dado: ";
    cin.getline(city, 50);
    cout << "Digite o esporte do dado: ";
    cin.getline(sport, 50);
    cout << "Digite o evento do dado: ";
    cin.getline(event, 180);
    cout << "Digite o NOC do dado: ";
    cin.getline(noc, 4);
    csvData dadoAux;
    fstream arquivo2;
    arquivo2.open("data_athlete_event.bin", ios::binary | ios::in | ios::out | ios::ate);
    for (int i = numeroDados; i > posicao; i--) {
        //le o dado da posicao i e o coloca na posicao i+1
        arquivo2.seekg((i-1) * tamanhoDado);
        arquivo2.read((char*)&dadoAux, tamanhoDado);
        arquivo2.seekp((i) * tamanhoDado);
        arquivo2.write((char*)&dadoAux, tamanhoDado);
        dadoAux.limpaDados();
    }
    //adciona o dado na posicao desejada
    arquivo2.seekp((posicao+1) * tamanhoDado);
    arquivo2.write((char*)this, tamanhoDado);
    arquivo2.close();
}

void csvData::mudaDado() {
    //esse metodo vai mudar os dados de um dado do arquivo data_athlete_event.bin atraves de um id recebido
    fstream arquivo;
    arquivo.open("data_athlete_event.bin", ios::binary | ios::in | ios::out | ios::ate);
    if (arquivo.is_open()) {
        int id;    
        int i = 1;
        cout << "Digite o ID do dado que deseja mudar: ";
        cin >> id;
        if (cin.fail() || id < 0) {
            cout << "ID invalido" << endl;
            return;
        }
        arquivo.seekg((id+1) * sizeof(csvData), ios::beg);
        csvData dado;
        arquivo.read((char*)&dado, sizeof(csvData));
        //se por acaso o id nao for a posicao+1 do dado, ele vai procurar pelo ID
        while(dado.ID != id && !arquivo.eof()) {
            limpaDados();
            arquivo.seekp(i * sizeof(csvData), ios::beg);
            arquivo.read((char*)&dado, sizeof(csvData));
            i++;
        }
        //se o id nao for encontrado, ele vai mostrar uma mensagem de erro      
        if (dado.ID != id) {
            cout << "ID nao encontrado" << endl;
            return;
        }
        dado.mostraDado();
        arquivo.close();
        fstream arquivo;
        arquivo.open("data_athlete_event.bin", ios::binary | ios::in | ios::out | ios::ate);
        if (arquivo.is_open()) {
            cout << "Digite o novo nome: ";
            cin >> dado.name;
            cout << "Digite a nova cidade: ";
            cin >> dado.city;
            cout << "Digite o novo esporte: ";
            cin >> dado.sport;
            cout << "Digite o novo evento: ";
            cin >> dado.event;
            cout << "Digite o novo NOC: ";
            cin >> dado.noc;
            arquivo.seekp((id+1) * sizeof(csvData), ios::beg);
            arquivo.write((char*)&dado, sizeof(csvData));
            arquivo.close();
        }
        else {
            cout << "Erro ao abrir o arquivo" << endl;
        }
    }
    else {
        cout << "Erro ao abrir o arquivo" << endl;
    }
}

void csvData::visualizaDados() {
    //esse metodo vai mostrar os dados do arquivo data_athlete_event.bin da posicao x ate a posicao y
    ifstream arquivo;    
    arquivo.open("data_athlete_event.bin", ios::binary);
    if (arquivo.is_open()) {
        int posicaoInicial, posicaoFinal;
        cout << "Digite a posicao inicial: ";
        cin >> posicaoInicial;
        if (cin.fail() || posicaoInicial < 0) {
            cout << "Posicao invalida" << endl;
            return;
        }
        cout << "Digite a posicao final: ";
        cin >> posicaoFinal;
        if (cin.fail() || posicaoFinal < 0) {
            cout << "Posicao invalida" << endl;
            return;
        }      
        //posicionar o ponteiro no inicio do arquivo
        arquivo.seekg((posicaoInicial+1) * sizeof(csvData), ios::beg);
        csvData dado;
        for (int i = posicaoInicial; i <= posicaoFinal; i++) {
            //ler o dado da posicao i e mostrar
            arquivo.read((char*)&dado, sizeof(csvData));
            dado.mostraDado();
        }
        arquivo.close();
    }
    else {
        cout << "Erro ao abrir o arquivo" << endl;
    }
}

void csvData::trocaPosicao() {
    //esse metodo vai trocar a posicao de dois dados do arquivo data_athlete_event.bin
    fstream arquivo;
    arquivo.open("data_athlete_event.bin", ios::binary | ios::in | ios::out | ios::ate);
    if (arquivo.is_open()) {
        int posicao1, posicao2;
        cout << "Digite a posicao 1: ";        
        cin >> posicao1;
        if (cin.fail() || posicao1 < 0) {
            cout << "Posicao invalida" << endl;
            return;
        }

        cout << "Digite a posicao 2: ";        
        cin >> posicao2;
        if (cin.fail() || posicao2 < 0) {
            cout << "Posicao invalida" << endl;
            return;
        }

        csvData dado1, dado2;
        //le os dados das posicoes 1 e 2
        arquivo.seekg((posicao1+1) * sizeof(csvData), ios::beg);
        arquivo.read((char*)&dado1, sizeof(csvData));
        arquivo.seekg((posicao2+1) * sizeof(csvData), ios::beg);
        arquivo.read((char*)&dado2, sizeof(csvData));
        //escreve os dados das posicoes 1 e 2
        arquivo.seekp((posicao1+1) * sizeof(csvData), ios::beg);
        arquivo.write((char*)&dado2, sizeof(csvData));
        arquivo.seekp((posicao2+1) * sizeof(csvData), ios::beg);
        arquivo.write((char*)&dado1, sizeof(csvData));
        arquivo.close();
    }
    else {
        cout << "Erro ao abrir o arquivo" << endl;
    }
}

void csvData::leDado() {
    //esse metodo vai ler um dado do arquivo data_athlete_event.bin atraves de um id recebido
    fstream arquivo;
    arquivo.open("data_athlete_event.bin", ios::binary | ios::ate | ios::in | ios::out);
    if (arquivo.is_open()) {
        int id, i = 0;
        cout << "Digite o ID do dado que deseja ler: ";
        cin >> id;
        if (cin.fail() || id < 0) {
            cout << "ID invalido" << endl;
            return;
        }
        arquivo.seekg((id+1) * sizeof(csvData), ios::beg);
        csvData dado;
        arquivo.read((char*)&dado, sizeof(csvData));        
        //se por acaso o id nao for a posicao+1 do dado, ele vai procurar pelo ID
        while(dado.ID != id && !arquivo.eof()) {
            limpaDados();
            arquivo.seekp(i * sizeof(csvData), ios::beg);
            arquivo.read((char*)&dado, sizeof(csvData));
            i++;
        }
        dado.mostraDado();
        arquivo.close();
    } else {
        cout << "Erro ao abrir o arquivo" << endl;
    }
}

void csvData::divideArquivo(int &numeroArquivos) {
    //esse metodo vai dividir o arquivo data_athlete_event.bin em varios arquivos menores
    cout << "Dividindo o arquivo..." << endl;
    ifstream arquivo;
    arquivo.open("data_athlete_event.bin", ios::binary);
    if (arquivo.is_open()) {
        //calcula o numero de arquivos menores
        arquivo.seekg(0, ios::end);
        int numeroDados = arquivo.tellg() / sizeof(csvData);
        arquivo.seekg(sizeof(csvData), ios::beg);
        numeroArquivos = (numeroDados / 50000) + 1;
        //cria os arquivos
        for (int i = 0; i < numeroArquivos; i++) {
            ofstream arquivo;
            arquivo.open("data_athlete_event" + to_string(i) + ".bin", ios::binary);
            arquivo.close();
        }
        //le os dados do arquivo original e escreve nos arquivos menores
        for (int i = 0; i < numeroArquivos; i++) {
            int j = 0;
            while(arquivo.good() && j < 50000) {
                csvData dado;
                arquivo.read((char*)&dado, sizeof(csvData));
                ofstream arquivo;
                arquivo.open("data_athlete_event" + to_string(i) + ".bin", ios::binary | ios::app);
                if (arquivo.is_open()) {
                    arquivo.write((char*)&dado, sizeof(csvData));
                    arquivo.close();
                } else {
                    cout << "Erro ao abrir o arquivo auxiliar: " << i << endl;
                }
                j++;
            }
        }
        arquivo.close();
    } else {
        cout << "Erro ao abrir o arquivo na divisao" << endl;
    }
    cout << "Arquivo dividido em " << numeroArquivos << " arquivos!" << endl;
}

void csvData::intercala(csvData *vetor, int inicio, int meio, int fim) {
    //esse metodo vai intercalar os dados de dois vetores, usando city como chave de ordenacao principal e ID como secundaria
    int i = inicio, j = meio + 1;
    int tamanho = fim - inicio + 1;
    csvData *vetorAuxiliar = new csvData[tamanho];
    for(int k = 0; k < tamanho; k++) {
        if ((i <= meio) && (j <= fim)) {
            string cidade1 = vetor[i].city;
            string cidade2 = vetor[j].city;
            //compara as cidades dos dados
            if (cidade1 < cidade2) {
                //se a cidade do vetor da esquerda for menor, pega o proximo do vetor da esquerda
                vetorAuxiliar[k] = vetor[i];
                i++;
            } else if (cidade1 > cidade2) {
                //se a cidade do vetor da direita for menor, pega o proximo do vetor da direita
                vetorAuxiliar[k] = vetor[j];
                j++;
            } else {
                //se as cidades forem iguais, compara os IDs
                if (vetor[i].ID < vetor[j].ID) {
                    //se o ID do vetor da esquerda for menor, pega o proximo do vetor da esquerda
                    vetorAuxiliar[k] = vetor[i];
                    i++;
                } else {
                    //se o ID do vetor da direita for menor, pega o proximo do vetor da direita
                    vetorAuxiliar[k] = vetor[j];
                    j++;
                }
            }
        } else if (i > meio) {
            //se o vetor da esquerda acabou, pega o proximo do vetor da direita
            vetorAuxiliar[k] = vetor[j];
            j++;
        } else {
            //se o vetor da direita acabou, pega o proximo do vetor da esquerda
            vetorAuxiliar[k] = vetor[i];
            i++;
        }
    }
    for (int k = 0; k < tamanho; k++) {
        //escreve os dados ordenados no vetor original
        vetor[inicio + k] = vetorAuxiliar[k];
    }
    delete[] vetorAuxiliar;
}

void csvData::mergeSort(csvData *vetor, int inicio, int fim) {
    //esse metodo vai ordenar os dados de um vetor
    if (inicio < fim) {
        int meio = (inicio + fim) / 2;
        mergeSort(vetor, inicio, meio);
        mergeSort(vetor, meio + 1, fim);
        intercala(vetor, inicio, meio, fim);
    }
}

void csvData::ordenaArquivos(int numeroArquivos, int inicio, int fim) {
    //esse metodo vai ordenar os arquivos auxiliares
    csvData *vetor = new csvData[50000];
    for (int i = 0; i < numeroArquivos; i++) {
        for (int j = 0; j < 50000; j++) {
            vetor[j].limpaDados();
        }
        ifstream arquivo;
        arquivo.open("data_athlete_event" + to_string(i) + ".bin", ios::binary);
        if (arquivo.is_open()) {
            for (int j = 0; j < 50000; j++) {
                arquivo.read((char*)&vetor[j], sizeof(csvData));
            }
            //ordenar o vetor
            mergeSort(vetor, inicio, fim);
            arquivo.close();
            //abrir o arquivo para escrever
            ofstream arquivo;
            arquivo.open("data_athlete_event" + to_string(i) + ".bin", ios::binary);
            if (arquivo.is_open()) {
                //escrever o vetor ordenado no arquivo
                arquivo.seekp(0, ios::beg);
                for (int j = 0; j < 50000; j++) {
                    if (vetor[j].city[0] != '\0') {
                        arquivo.write((char*)&vetor[j], sizeof(csvData));
                    }
                }
                arquivo.close();
                cout << "Arquivo " << i << " ordenado!" << endl;
            } else {
                cout << "Erro ao abrir o arquivo auxiliar: " << i << "para escrever na ordenacao" << endl;
            }
        } else {
            cout << "Erro ao abrir o arquivo auxiliar: " << i << "para ler na ordenacao" << endl;
        }
    }
}

void csvData::mesclaArquivos(int numeroArquivos) {
    //esse metodo vai mesclar os arquivos auxiliares em um arquivo final
    int indices[numeroArquivos];
    for (int i = 0; i < numeroArquivos; i++) {
        indices[i] = 0;
    }
    //abrir os arquivos auxiliares
    ifstream arquivos[numeroArquivos];
    for (int i = 0; i < numeroArquivos; i++) {
        arquivos[i].open("data_athlete_event" + to_string(i) + ".bin", ios::binary);
        if (!arquivos[i].is_open()) {
            cout << "Erro ao abrir o arquivo auxiliar: " << i << "na mescla" << endl;
        }
    }
    //abrir o arquivo final
    ofstream arquivoFinal;
    arquivoFinal.open("data_athlete_event_final.bin", ios::binary);
    if (!arquivoFinal.is_open()) {
        cout << "Erro ao abrir o arquivo final na mescla" << endl;
        return;
    }
    //mesclar os arquivos auxiliares 
    bool arquivosAbertos = true;
    while (arquivosAbertos) {
        arquivosAbertos = false;
        csvData menor;
        int indiceMenor = -1;
        for (int i = 0; i < numeroArquivos; i++) {
            arquivos[i].seekg(0, ios::end);
            int tamanho = arquivos[i].tellg() / sizeof(csvData);
            arquivos[i].seekg(0, ios::beg);
            if (indices[i] < tamanho) {
                arquivosAbertos = true;
                csvData aux;
                arquivos[i].seekg(indices[i] * sizeof(csvData), ios::beg);
                arquivos[i].read((char*)&aux, sizeof(csvData));
                if (indiceMenor == -1) {
                    menor = aux;
                    indiceMenor = i;
                } else {
                    string cidade1 = aux.city;
                    string cidade2 = menor.city;
                    //compara as cidades dos dados
                    if (cidade1 < cidade2) {
                        //se a cidade do vetor da esquerda for menor, pega o proximo do vetor da esquerda
                        menor = aux;
                        indiceMenor = i;
                    } else if (cidade1 > cidade2) {
                        //se a cidade do vetor da direita for menor, pega o proximo do vetor da direita
                    } else {
                        //se as cidades forem iguais, compara os IDs
                        if (aux.ID < menor.ID) {
                            //se o ID do vetor da esquerda for menor, pega o proximo do vetor da esquerda
                            menor = aux;
                            indiceMenor = i;
                        } else {
                            //se o ID do vetor da direita for menor, pega o proximo do vetor da direita
                        }
                    }
                }
            }
        }
        if (indiceMenor != -1) {
            arquivoFinal.write((char*)&menor, sizeof(csvData));
            indices[indiceMenor]++;
        }
    }
    //fechar os arquivos
    for (int i = 0; i < numeroArquivos; i++) {
        arquivos[i].close();
    }
    arquivoFinal.close();
    cout << "Arquivos mesclados, arquivo final gerado com sucesso!" << endl;
}

void csvData::mergeSortExterno() {
    //esse metodo vai chamar os metodos necessarios para ordenar o arquivo
    int numeroArquivos;
    divideArquivo(numeroArquivos);
    if (numeroArquivos > 1) {
        ordenaArquivos(numeroArquivos, 0, 49999);
        mesclaArquivos(numeroArquivos);
        for (int i = 0; i < numeroArquivos; i++)
            remove(("data_athlete_event" + to_string(i) + ".bin").c_str());
    } else {
        cout << "Arquivo pequeno demais" << endl;
    }
}

void csvData::fazTxT(string nomeArquivo) {
    //esse metodo transforma o arquivo binario em um arquivo txt para facilitar visualizacao
    ifstream arquivo;
    arquivo.open(nomeArquivo + ".bin", ios::binary);
    if (arquivo.is_open()) {
        ofstream arquivoTxt;
        arquivoTxt.open(nomeArquivo + ".txt");
        if (arquivoTxt.is_open()) {
            csvData dado;
            while (arquivo.read((char*)&dado, sizeof(csvData))) {
                arquivoTxt << dado.city << ";" << dado.ID << ";" << dado.name << endl;
            }
        } else {
            cout << "Erro ao abrir o arquivo txt" << endl;
        }
    } else {
        cout << "Erro ao abrir o arquivo binario" << endl;
    }
    cout << "Arquivo txt gerado com sucesso!" << endl;
}
