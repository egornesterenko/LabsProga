#include <iostream>
#include <fstream>
using namespace std;

class hashTable {
private:

    struct Node {
        long long hash = -1;
        string word;
        string data;
        Node * next = nullptr;
    };
    struct key {
        long long key = -1;
        Node * next = nullptr;
    };
    int m = 0, i = 0;
    key * table = new key[m];

public:

    explicit hashTable(int k) {
        m = k;
        table = new key[m];
    }

    long long hashStr(string data) {
        const int p = 31;
        long long hash = 0, pow = 1;
        for (int i = 0; data[i]; i++) {
            hash += abs((data[i] - 'a' + 1)) * pow;
            pow *= p;
        }
        return hash;
    }

    string findWord(string data) {
        string word;
        int i = 0;
        while (data[i] != ';' && data[i]) {
            word += (char)tolower(data[i]);
            i++;
        }
        return word;
    }

    string search(string word) {
        string check;
        for(int i = 0; word[i]; i++)
            check += (char)tolower(word[i]);
        long long key = hashStr(check) % m;
        if(table[key].key == key) {
            Node * temp = table[key].next;
            while(temp->next != nullptr && temp->word != check)
                temp = temp->next;
            if(temp->word == check)
                return temp->data;
            return "Can't find this word! =(";
        }
        else
            return "Can't find this word! =(";
    }

    void addToHashTable(key * arr, const string data) {
        string word = findWord(data);
        long long hash = abs(hashStr(word));
        long long key = hash % m;
        auto * list = new Node;
        list->hash = hash;
        list->word = word;
        list->data = data;
        if(arr[key].key == key) {
            list->next = arr[key].next;
            arr[key].next = list;
        }
        else {
            arr[key].key = key;
            arr[key].next = list;
        }
    }

    void resize(key * arr) {
        m *= 2;
        auto * arr2 = new key[m];
        for(int i = 0; i < m / 2; i++) {
            if(arr[i].next != nullptr) {
                Node * temp = arr[i].next;
                while(temp != nullptr) {
                    addToHashTable(arr2, temp->data);
                    temp = temp->next;
                }
                delList(arr[i].next);
            }
        }
        delete[] arr;
        table = arr2;
    }

    void add(const string data) {
        i++;
        if(m - (m / 5) < i)
            resize(table);
        addToHashTable(table, data);
    }

    void delList(Node * list) {
        while(list != nullptr) {
            Node * temp = list;
            list = list->next;
            delete temp;
        }
    }

    void del(key * arr) {
        for(int i = 0; i < m; i++)
            if(arr[i].next != nullptr)
                delList(arr[i].next);
        delete [] arr;
    }

    ~hashTable() {
        del(table);
    }

};

int main() {

    ifstream dict("/home/ragnaros/Labs/Lab_4/dict_processed.txt");
    hashTable table(20);

    if(dict.is_open()) {
        while(dict.good()) {
            string temp;
            getline(dict, temp);
            table.add(temp);
        }
        string word;
        cout << "Input your word: ";
        cin >> word;
        cout << table.search(word);
    }
    else
        cout << "Isn't open";

    dict.close();

    return 0;
}
