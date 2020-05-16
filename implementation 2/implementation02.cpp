#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <vector>
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

void Trie::pesquisa(string key){
    if(Root==NULL){
        cout<<"\nA arvore nao existe\n";
        return;
    }
    else{
        pesquisa(key, 0, Root);
    }

}
void Trie::pesquisa(string key, int indK, Node* &aux){
    cout<<aux;
    if(aux->is_leaf){ // Se for folha
        if(aux->word == key){
            cout<<"\n\nA palavra '"<<key<<"' existe\n";
            return;
        }
        int index = indice(key,indK);
        if(aux->Character[indK]==NULL){
            cout<<"\n\nA palavra '"<<key<<"' nao existe\n";
            return;
        }else {
            pesquisa(key,indK+1,aux->Character[index]);
        }
    }else{ // se nao for folha
        int index = indice(key,indK);
        cout<<"\nnao folha"<<endl;
        if(aux->Character[index]==NULL){
            cout<<"\n\nA palavra '"<<key<<"' nao existe\n";
            return;
            cout<<"cannot show\n";
        }else{
            pesquisa(key,indK+1,aux->Character[index]);
        }
    }
}

void maiuscula(string &);
bool montaArvore(Trie &);

int main(){
    Trie Arv;
    string option, word;

    if(!montaArvore(Arv))
        cout<<"Erro ao montar Árvore";

    fstream myfile ("entrada.txt");

    if(myfile.is_open()){
        while(!myfile.eof()){
            string y;
            getline(myfile, word);
            word += ' ';
            for(int j =0; j<word.length(); j++){
                if(word[j] == ' '){
                    maiuscula(y);
                    y += '#';
                    Arv.pesquisa(y);
                    y = "";
                }else{
                    y += word[j];
                }
            }//close for
        }//close myfile.eof()
        myfile.close();
    }else{
        cout<<"Erro ao abrir arquivo\n";
    }

    cout<<"\n_____Our_Trie_Tree_____\n";
    cout<<"\ndigite 'n.' quando cansar\n\n";
    int kk;
    while(option != "n.#"){
        cout<<"Entre com a pesquisa(sem '#')";
        cin>>option;
        maiuscula(option);
        option += '#';
        Arv.pesquisa(option);
    }
    Arv.deleteTree();
    Arv.pesquisa("carro#");
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


    /*Trie Arv;

    Arv.pesquisa("carro#");

    Arv.inserir("carro#");
    cout<<"\ninseriu -carro-\n";
    Arv.inserir("cama#");
    cout<<"inseriu -cama-\n";
    Arv.inserir("preto#");
    cout<<"inseriu -preto-\n";
    Arv.inserir("lata#");
    cout<<"inseriu -lata-\n";
    Arv.inserir("prata#");
    cout<<"inseriu -prata-\n";
    Arv.inserir("campo#");
    cout<<"inseriu -campo-\n";
    Arv.inserir("leite#");
    cout<<"inseriu -leite-\n";
    Arv.inserir("lataria#");
    cout<<"inseriu -lataria-\n";
*/
