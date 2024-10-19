#include <iostream>
#include <vector>
#include <utility> 

class HashTable {
    std::vector<std::pair<int, bool>> table; 
    int capacity, num_elements = 0;


    int hash(int key) {
        return key % capacity;
    }


    bool is_prime(int n) {
        if (n <= 1) return false;
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) return false;
        }
        return true;
    }


    int next_prime(int n) {
        while (!is_prime(n)) n++;
        return n;
    }

 
    void resize() {
        int new_capacity = next_prime(capacity * 2);
        std::vector<std::pair<int, bool>> new_table(new_capacity, {-1, false});

        for (const auto &entry : table) {
            if (entry.first != -1 && !entry.second) { 
                int idx = entry.first % new_capacity;
                int probes = 0;
                while (new_table[idx].first != -1) {
                    idx = (idx + ++probes * probes) % new_capacity;
                }
                new_table[idx] = {entry.first, false}; 
            }
        }

        table = std::move(new_table);
        capacity = new_capacity; 
    }

public:

    HashTable(int init_capacity = 7) : capacity(init_capacity) {
        table.resize(capacity, {-1, false});
    }

    void insert(int key) {
        if (search(key) != -1) return; 
        int idx = hash(key);
        int probes = 0;

        while (table[idx].first != -1 && probes < 7) { 
            idx = (idx + ++probes * probes) % capacity;
        }

        if (probes < 7) {
            table[idx] = {key, false}; 
            num_elements++;
            if (num_elements > capacity * 0.7) resize(); 
        }
    }


    int search(int key) {
        int idx = hash(key);
        int probes = 0;

        while (probes < 7) {
            if (table[idx].first == key && !table[idx].second) {
                return idx; 
            }
            idx = (idx + ++probes * probes) % capacity; 
        }
        return -1; 
    }

    
    void remove(int key) {
        int idx = search(key);
        if (idx != -1) {
            table[idx].second = true; 
            num_elements--;
        }
    }

   
    void print() {
        for (const auto &entry : table) {
            if (entry.first != -1 && !entry.second) {
                std::cout << entry.first << " ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
};

int main() {
    HashTable ht;

    // Example test cases
    ht.insert(1); ht.print();
    ht.insert(6); ht.print();
    ht.insert(15); ht.print();
    ht.insert(25); ht.print();
    ht.remove(15); ht.print();
    ht.insert(29); ht.print();
    std::cout << "Found at: " << ht.search(22) << std::endl;

    return 0;
}
