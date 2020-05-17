#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <vector>
#include <windows.h>
#include <limits>
#include "TrieNo.h"

using namespace std;


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

            return;
        }
    }else{//se o nó não é uma folha
        int index = indice(key,indK);
        if(aux->Character[index]==NULL){//se ele não tem filho com esse caractere
            if(index == 27){//se for o final da palavra (olhar a implementacao da funcao 'indice()')
                Node* a2;
                createLeaf(key,a2);
                aux->Character[index] = a2;
                aux->is_endWord = true;
                aux->num_prefix +=1;
            }else{//se não for final da palavra
                Node* a2;
                createLeaf(key,a2);
                aux->Character[index] = a2;
                aux->num_prefix +=1;
            }

        }else if(aux->Character[index]->is_leaf){ //se aponta para uma folha

            int i = indK;
            Node* aux2 = aux->Character[index];
            do{
                int ind = indice(aux2->word,i);
                Node* aux3 = new Node();
                aux->Character[ind] = aux3;
                aux->num_prefix +=1;
                aux = aux3;
                i++;

            }while(key[i]==aux2->word[i]);
            aux->Character[indice(aux2->word,i)] = aux2;
            Node* a2;
            createLeaf(key,a2);
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
    //cout<<aux;
    if(aux->is_leaf){ // Se for folha
        if(aux->word == key){
            return;
        }
        int index = indice(key,indK);
        if(aux->Character[indK]==NULL){
            sugest(Vet, aux, key.length());
            if(Vet.empty())
                sugest(Vet, aux, key.length()+2);
            return;
        }else {
            pesquisa(Vet,key,indK+1,aux->Character[index]);
            if(Vet.size()>0 && Vet.size()<8)
                sugest(Vet, aux, key.length());
        }
    }else{ // se nao for folha
        int index = indice(key,indK);
        //cout<<"\nnao folha"<<endl;
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
                if(aux->Character[i]->is_leaf){
                    if(aux->Character[i]->word.length() <= tam){
                        Vet.push_back(aux->Character[i]->word);
                    }
                }else{
                    sugest(Vet, aux->Character[i], tam);
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
    vector<string> vet;
    string option, word, n;

    if(!montaArvore(Arv))
        cout<<"Erro ao montar Árvore";

    fstream myfile;
    myfile.open("saida.txt", ios::out);
    myfile<<"";
    myfile.close();

    myfile.open("entrada.txt");
    if(myfile.is_open()){
        while(!myfile.eof()){
            string y;
            getline(myfile, word);
            word += ' ';
            for(int j =0; j<word.length(); j++){
                if(word[j] == ' '){
                    maiuscula(y);
                    y += '#';
                    Arv.pesquisa(vet,y);
                    n = y.substr(0,y.length()-1);
                    if(!vet.empty())
                        menu(vet, n);
                    else
                        writeText(n);
                    y = "";
                }else{
                    y += word[j];
                }
                vet.clear();
            }//close for
        }//close myfile.eof()
        myfile.close();
    }else{
        cout<<"Erro ao abrir arquivo\n";
    }
gotoxy(1,20);
return 0;
}



void maiuscula (string &palavra){
    int i=0;
    char c;
    while (palavra[i]){
        c=palavra[i];
        palavra[i] = toupper(c);
        i++;
      }
}

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
void writeText(string a){
    fstream myfile;
    myfile.open("saida.txt", fstream::app);

    if(myfile.is_open()){
        myfile << a << " ";
    }else{
        cout<<"Falha ao abrir saida.txt";
    }
}
void gotoxy(int x, int y){//posicionar cursor na tela
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){
        x-1,y-1
    });
}
