using namespace std;

class Trie;

class Trie {
private:
    class Node {
    public:
        Node* Character[28];
        int num_prefix;
        bool is_leaf;
        bool is_endWord;
        string word;
        Node(){
            is_endWord = false;
            num_prefix = 0;
            is_leaf = false;
            for(int i = 0;i<28;i++){
                Character[i] = NULL;
            }
        }
    };
    void inserir(string key, int indK, Node* aux, Node* &prevNode);
    void deleteTree(Node* aux);
    void pesquisa(string key, int indK, Node* &aux);
    Node* Root;
public:
    Trie(){
        Root = NULL;
    }
    ~Trie(){
        deleteTree();
    }
    void inserir(string key);
    void pesquisa(string key);
    string valRoot(){return Root->word;}
    void createLeaf(string key, Node* &N);
    int indice(string key, int i);
    void deleteTree();
};
