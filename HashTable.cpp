#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

template <typename T>
class Hashtable {
public:
    Hashtable(int size) : table(size) {}

    void insert(T data) {
        int index = hashFunction(data);
        table[index].push_back(data);
    }

    bool search(T data) {
        int index = hashFunction(data);
        for (const T& item : table[index]) {
            if (item == data) {
                return true;
            }
        }
        return false;
    }

    void deleteValue(T data) {
        int index = hashFunction(data);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (*it == data) {
                table[index].erase(it);
                return;
            }
        }
    }

    bool isEquivalent(const Hashtable<T>& other) {
        if (table.size() != other.table.size()) {
            return false;
        }

        for (int i = 0; i < table.size(); ++i) {
            if (table[i].size() != other.table[i].size()) {
                return false;
            }

            for (const T& item : table[i]) {
                if (!other.search(item)) {
                    return false;
                }
            }
        }

        return true;
    }

private:
    int hashFunction(T data) {
        // Implement your own hash function here
        // For simplicity, let's use a modulo operation
        return data % table.size();
    }

    unordered_map<int, list<T>> table;
};
