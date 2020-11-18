#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <vector>
#include <windows.h>
#include <limits>
#include "TrieNo.h" //arquivo com as declarações da árvore

using namespace std;

//colocada no destrutor da árvore para deletar os nós alocados
void Trie::deleteTree(){
    if(Root == NULL) return;
    deleteTree(Root);
    Root = NULL;
}
void Trie::deleteTree(Node* aux){
    if(aux->is_leaf){
        delete aux;
    }else{
        if(aux->num_prefix == 0){
            delete aux;
            return;
        }else{
            for(int i=0; i<28; i++){
                if(aux->Character[i] != NULL){
                    deleteTree(aux->Character[i]);
                    aux->num_prefix -=1;
                    aux->Character[i] = nullptr;
                }
            }
            if(aux==Root)
                aux=NULL;
            else
                delete aux;
        }
    }
}

//para calcular a podicao do ponteiro
int Trie::indice(string key, int i){
    if((int)key[i] > 45){
        return key[i] - 'A';
    }else{
        return (key[i]==35) ? 27 : 26;
    }
}
//Para criar um no folha
void Trie::createLeaf(string key, Node* &N){
    N = new Node();
    N->word = key;
    N->is_leaf = true;
}

//implementing insertion
void Trie::inserir(string key){
        if(Root == NULL){
            Root = new Node();
            Node* a2;
            createLeaf(key,a2);
            Root->Character[indice(key,0)] = a2;
        }else{
            Node* ant;
            inserir(key, 0,Root, ant);
        }
}
void Trie::inserir(string key, int indK, Node* aux, Node* &prevNode){
    if(aux->is_leaf){//se o nó é uma folha
        if(aux->word == key){ // se a folha é igual à chave

            return;//no caso implementei de um jeito que o aux não vai ser nó externo
        }          //mas deixei o código assim pra não correr o risco de me perder.
    }else{//se o nó não é uma folha
        int index = indice(key,indK);
        if(aux->Character[index]==NULL){//se ele não tem filho com esse caractere
            if(index == 27){//se for o final da palavra (olhar a implementacao da funcao 'indice()')
                Node* a2;
                createLeaf(key,a2);//cria um nó folha
                aux->Character[index] = a2;//faz o aux apontar para a posição que corresponde à letra
                aux->is_endWord = true;//seta o nó como um fim de palavra
                aux->num_prefix +=1;
            }else{//se não for final da palavra
                Node* a2;
                createLeaf(key,a2);
                aux->Character[index] = a2;
                aux->num_prefix +=1;
            }

        }else if(aux->Character[index]->is_leaf){ //se aux aponta para uma folha
            //criando nós internos até que os caracteres sejam diferentes
            int i = indK;
            Node* aux2 = aux->Character[index];//guarda o endereço do nó folha
            do{
                int ind = indice(aux2->word,i);
                Node* aux3 = new Node(); //cria o nó interno
                aux->Character[ind] = aux3;//o aux aponta para a letra
                aux->num_prefix +=1;//para ir para a próxima letra
                aux = aux3;//aux vai para o novo nó criado para repetir o processo  até que as letras sejam iguais.
                i++;

            }while(key[i]==aux2->word[i]);
            aux->Character[indice(aux2->word,i)] = aux2;//faço o último nó interno criado apontar para a chave que já existia
            Node* a2;
            createLeaf(key,a2);// crio um novo nó externo para essa palavra.
            aux->Character[indice(key,i)] = a2;
            aux->num_prefix +=1;

        }else{//aponta para um nó interno
                inserir(key, indK+1, aux->Character[index], aux);
        }
    }

}

void Trie::pesquisa(vector<string> &Vet, string key){
    if(Root==NULL){
        cout<<"\nA arvore nao existe\n";
        return;
    }
    else{
        pesquisa(Vet, key, 0, Root);
    }

}
void Trie::pesquisa(vector<string> &Vet, string key, int indK, Node* &aux){
    if(aux->is_leaf){ // Se for folha
        if(aux->word == key){
            return;
        }
        int index = indice(key,indK);
        if(aux->Character[indK]==NULL){
            sugest(Vet, aux, key.length());//chama a função que seleciona as sugestões
            if(Vet.empty())
                sugest(Vet, aux, key.length()+2);//no retorno da outra função se o vector retornar vazio, eu passo
            return;                              //com um tamanho  com +2 unidades para pegar palavras maiores com o mesmo prefixo.
        }else {
            pesquisa(Vet,key,indK+1,aux->Character[index]);
            if(Vet.size()>0 && Vet.size()<8)
                sugest(Vet, aux, key.length());
        }
    }else{ // se nao for folha
        int index = indice(key,indK);
        if(aux->Character[index]==NULL){
            sugest(Vet, aux, key.length());
            if(Vet.empty())
                sugest(Vet, aux, key.length()+2);
            return;
        }else{
            pesquisa(Vet,key,indK+1,aux->Character[index]);
            if(Vet.size()>0 && Vet.size()<8)
                sugest(Vet, aux, key.length());
        }
    }
}

bool Trie::sugest(vector<string> &Vet, Node* &aux, int tam){
    if(aux->is_leaf){//se o nó é folha
        Vet.push_back(aux->word);
        return true;
    }else{ //se o nó é interno
        for(int i = 0; i<28; i++){
            if(Vet.size() == 8) return true;
            if(aux->Character[i]!=NULL){
                if(aux->Character[i]->is_leaf){//se o nó for folha
                    if(aux->Character[i]->word.length() <= tam){ //Fiz assim para tentar pegar palavras parecidas
                        Vet.push_back(aux->Character[i]->word);//insere a palavra no vector
                    }
                }else{
                    sugest(Vet, aux->Character[i], tam);
                }
            }
        }//fecha o for
    }
}


void Trie::consulta(string key){
    if(Root==NULL){
        cout<<"\nA arvore nao existe\n";
        return;
    }
    else{
        consulta(key, 0, Root);
    }

}
void Trie::consulta(string key, int indK, Node* &aux){
    if(aux->is_leaf){ // Se for folha
        if(aux->word == key){
            string a = aux->word.substr(0,aux->word.length() - 1); //cria uma substring sem o último caractere
            cout<<a<<endl;
            return;
        }
        int index = indice(key,indK);
        if(aux->Character[index]!=NULL && key[indK]=='#'){
            wordsPrefix(aux);
            return;
        }else if(aux->Character[index]!=NULL){
            consulta(key,indK+1,aux->Character[index]);

        }else{
            cout<< "Prefixo inexistente\n";
        }
    }else{ // se nao for folha
        int index = indice(key,indK);
        if(key[indK]=='#'){
            wordsPrefix(aux);//função para fazer a busca específica
            return;
        }else if(aux->Character[index]!=NULL){
            consulta(key,indK+1,aux->Character[index]);
        }
    }
}

bool Trie::wordsPrefix(Node* &aux){
    if(aux->is_leaf){//se o nó é folha
        cout<<aux->word<<endl;
        return true;
    }else{ //se o nó é interno
        for(int i = 0; i<28; i++){
            if(aux->Character[i]!=NULL){
                if(aux->Character[i]->is_leaf){
                    //cout<< "entrou aqui\n";
                    string p = aux->Character[i]->word.substr(0, aux->Character[i]->word.length() - 1);
                    cout<<p<<endl;
                }else{
                    wordsPrefix(aux->Character[i]);
                }
            }
        }//fecha o for
    }
}

void gotoxy(int, int);
void maiuscula(string &);
bool montaArvore(Trie &);
void menu(vector<string> &, string);
void mostraS(vector<string> &);
void writeText(string);


int main(){
    Trie Arv;
    char Esc='A';
    vector<string> vet;
    string option, word, n;

    fstream myfile;
    if(!montaArvore(Arv))//função que insere as palavras do arquivo na árvore
        cout<<"Erro ao montar Árvore";

//menu principal
    while(Esc != 's'){
        system("cls");
        string pre;
        int op = 1;
        cout<< "----------------------------------\n";
        gotoxy(7,2);
        cout<< "Testando Arvore Trie\n";
        cout<< "----------------------------------\n";
        cout<< "Escolha uma das opções abaixo:\n\n";            //Menu principal
        cout<< "Digite [A] para verificar o arquivo\n";
        cout<< "Digite [P] para consultar palavras pelo prefixo\n";
        cout<< "Digite [s] para sair do programa\n";
        cout<< "\nEscolha... [";
        gotoxy(14,10);
        cout<< "]";
        gotoxy(13,10);
        cin>> Esc;
        switch(Esc){
            case 'a':
            case 'A':
                myfile.open("saida.txt", ios::out);
                myfile<<"";      //para limpar o arquivo de saída
                myfile.close();

                myfile.open("entrada.txt");
                if(myfile.is_open()){
                    while(!myfile.eof()){
                        string y;
                        getline(myfile, word);
                        word += ' ';//adiciona um espaço no final da linha
                        for(int j =0; j<word.length(); j++){//percorrea string

                            if(word[j] == ' '){ //se encontrar um espaço
                                maiuscula(y);//converte a palavra para maiúscula
                                y += '#';//adiciona caractere de fim da palavra
                                Arv.pesquisa(vet,y);//pesquisa a palavra
                                n = y.substr(0,y.length()-1);//substring (sem o últiimo caractere)
                                if(!vet.empty())
                                    menu(vet, n);
                                else
                                    writeText(n);
                                y = "";//limpa a variável para a próxima palavra.

                            }else{
                                y += word[j];//adiciona o caractere
                            }
                            vet.clear();
                        }//close for

                    }//close myfile.eof()
                    cout<<"\nArquivo analisado com sucesso!\n";
                    myfile.close();
                }else{
                    cout<<"Erro ao abrir arquivo\n";
                }
                gotoxy(1,18);
                system("pause");
                system("cls");
                break;
            //Consulta de prefixo
            case 'p':
            case 'P':
                while(op){
                    system("cls");
                    char sn;
                    cout << "insira um prefixo: ";
                    cin >> pre;
                    pre += '#';
                    maiuscula(pre);
                    Arv.consulta(pre);
                    cout<< "Deseja fazer outra pesquisa? [s]/[n]\n";
                    cin >> sn;
                    op = (sn=='n' || sn=='N') ? 0 : 1;
                }
                break;
            case 's':
            case 'S':
                break;
            default:
                cout<<"\nOpcao invalida!!!";
        }//fecha o switch

    }//fecha while princial
gotoxy(1,20);
return 0;
}

//converte uma string para letras maiusculas
void maiuscula (string &palavra){
    int i=0;
    char c;
    while (palavra[i]){
        c=palavra[i];
        palavra[i] = toupper(c);
        i++;
      }
}

//inicializa a árvore e insere todas as palavras do arquivo Palavras.txr
bool montaArvore(Trie &A){
    fstream myfile ("palavras.txt");
    string word;
    if(myfile.is_open()){
        while(!myfile.eof()){
            getline(myfile, word);
            word += '#';
            A.inserir(word);
        }
        myfile.close();
        return true;
    }else{
        cout<<"Nao foi possivel abrir o arquivo";
        return false;
    }
}

//mostra as sujestões de palavras para o corretor ortográfico
void mostraS(vector<string> &V){
    string M;
    int i=1,x=40,y=3;
    gotoxy(x,y);
    y+=2;
    cout<<"| Ou digite o numero da sujestao desejada:";
    for(auto c: V){
        gotoxy(x,y);
        M = c.substr(0,c.length()-1);
        cout<<"| "<<i++<<"_ "<<M<<endl;
        y+=1;
    }
}
//menu para quando a palavra do corretor estiver incorreta
void menu(vector<string> &V, string P){
    system("cls");
    int op,s=1;
    string palavra;
    gotoxy(27,1);
    cout<<"A palavra '"<<P<<"' nao consta!\n";
    gotoxy(20,2);
    cout<<"===========================================";
    cout<<"\nDigite [10] para escrever uma palavra";
    mostraS(V);
    gotoxy(1,5);
    while(s==1){
        try{

            cout<<"Escolha... ";
            cin>>op;
            if(cin.fail()){
                throw 'a';
            }else
                s=0;

        }catch(...){

            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Value must be numeric\n";
        }

    }
    if(op == 10){
        cout<<"Insira a palavra\n";
        cin>>palavra;
        writeText(palavra);
    }else{
        string a = V[op-1].substr(0,V[op-1].length()-1);
        writeText(a);
    }
}

//escreve o texto corrigido no arquivo saida.txt
void writeText(string a){
    fstream myfile;
    myfile.open("saida.txt", fstream::app);

    if(myfile.is_open()){
        myfile << a << " ";
    }else{
        cout<<"Falha ao abrir saida.txt";
    }
    myfile.close();
}
//posicionar cursor na tela
void gotoxy(int x, int y){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){
        x-1,y-1
    });
}
