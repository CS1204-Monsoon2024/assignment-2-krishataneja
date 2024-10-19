#include <iostream>
#include <vector>
#include <cmath>

class HashTable {
private:
    std::vector<int> table;
    std::vector<bool> deleted;
    int current_size;
    int num_elements;
    const double load_factor_threshold = 0.8;

    int hash(int key) {
        return key % current_size;
    }

    int quadraticProbe(int key, int i) {
        return (hash(key) + i * i) % current_size;
    }

    bool isPrime(int n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

    int nextPrime(int n) {
        while (!isPrime(n)) {
            n++;
        }
        return n;
    }

    void resizeTable() {
        int old_size = current_size;
        int new_size = nextPrime(2 * old_size);
        std::vector<int> new_table(new_size, -1);
        std::vector<bool> new_deleted(new_size, false);

        current_size = new_size;
        num_elements = 0;

        // Rehash all the non-deleted keys from the old table
        for (int i = 0; i < old_size; i++) {
            if (table[i] != -1 && !deleted[i]) {
                insertRehash(table[i], new_table, new_deleted);
            }
        }

        table = new_table;
        deleted = new_deleted;
    }

    void insertRehash(int key, std::vector<int>& new_table, std::vector<bool>& new_deleted) {
        int i = 0;
        int idx;
        while (true) {
            idx = (hash(key) + i * i) % current_size;
            if (new_table[idx] == -1 && !new_deleted[idx]) {
                new_table[idx] = key;
                return;
            }
            i++;
        }
    }

public:
    HashTable(int initial_size) {
        current_size = nextPrime(initial_size);
        table.resize(current_size, -1);  // -1 indicates empty
        deleted.resize(current_size, false);  // To mark deleted spots
        num_elements = 0;
    }

    void insert(int key) {
        if ((double)num_elements / current_size >= load_factor_threshold) {
            resizeTable();
        }

        int i = 0;
        int idx;
        while (true) {
            idx = quadraticProbe(key, i);
            if (table[idx] == -1 || deleted[idx]) {  // Empty or previously deleted spot
                table[idx] = key;
                deleted[idx] = false;
                num_elements++;
                return;
            }
            i++;
        }
    }

    bool search(int key) {
        int i = 0;
        int idx;
        while (i < current_size) {
            idx = quadraticProbe(key, i);
            if (table[idx] == key && !deleted[idx]) {
                return true;
            }
            if (table[idx] == -1) {  // Stop searching when an empty slot is found
                return false;
            }
            i++;
        }
        return false;
    }

    void remove(int key) {
        int i = 0;
        int idx;
        while (i < current_size) {
            idx = quadraticProbe(key, i);
            if (table[idx] == key && !deleted[idx]) {
                deleted[idx] = true;  // Mark as deleted
                num_elements--;
                return;
            }
            if (table[idx] == -1) {  // Stop searching when an empty slot is found
                return;
            }
            i++;
        }
    }

    void display() {
        for (int i = 0; i < current_size; i++) {
            if (table[i] == -1) {
                std::cout << "[ ] ";
            } else if (deleted[i]) {
                std::cout << "[D] ";  // D for deleted
            } else {
                std::cout << "[" << table[i] << "] ";
            }
        }
        std::cout << std::endl;
    }
};

