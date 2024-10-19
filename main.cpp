#include <iostream>
#include <vector>

class HashTable {
    std::vector<int> table;
    int size, num_elements = 0, max_probes = 5;

    int hash(int key) { return key % size; }

    bool is_prime(int n) {
        if (n < 2) return false;
        for (int i = 2; i * i <= n; ++i) if (n % i == 0) return false;
        return true;
    }

    int next_prime(int n) {
        while (!is_prime(n)) n++;
        return n;
    }

    void resize() {
        int new_size = next_prime(size * 2);
        std::vector<int> new_table(new_size, -1);
        for (int k : table) if (k != -1) {
            int idx = hash(k), p = 0;
            while (new_table[idx] != -1 && p < max_probes) idx = (idx + ++p * p) % new_size;
            new_table[idx] = k;
        }
        table = new_table;
        size = new_size;
    }

public:
    HashTable(int init_size = 7) : size(init_size) { table.resize(size, -1); }

    void insert(int key) {
        if (search(key) != -1) return;
        int idx = hash(key), p = 0;
        while (table[idx] != -1 && p < max_probes) idx = (idx + ++p * p) % size;
        if (p == max_probes) return;
        table[idx] = key; num_elements++;
        if (num_elements > size * 0.75) resize();
    }

    int search(int key) {
        int idx = hash(key), p = 0;
        while (table[idx] != -1 && p < max_probes) {
            if (table[idx] == key) return idx;
            idx = (idx + ++p * p) % size;
        }
        return -1;
    }

    void remove(int key) { 
        int idx = search(key); 
        if (idx != -1) table[idx] = -1; 
    }

    void print() {
        for (int k : table) std::cout << (k == -1 ? "- " : std::to_string(k) + " ");
        std::cout << std::endl;
    }
};

int main() {
    HashTable ht;
    ht.insert(1); ht.print();
    ht.insert(6); ht.print();
    ht.insert(15); ht.print();
    ht.insert(25); ht.print();
    ht.remove(15); ht.print();
    ht.insert(29); ht.print();
    std::cout << "Found at: " << ht.search(22) << std::endl;
    return 0;
}

