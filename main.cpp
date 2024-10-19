#include <iostream>
#include <vector>

class HashTable {
    std::vector<std::pair<int, bool>> table; // bool tracks if it's a deleted entry
    int capacity, num_elements = 0, max_probes = 7;

    int hash(int key) { return key % capacity; }

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
        int new_capacity = next_prime(capacity * 2);
        std::vector<std::pair<int, bool>> new_table(new_capacity, {-1, false});
        for (auto &entry : table) {
            if (entry.first != -1 && !entry.second) {
                int idx = hash(entry.first) % new_capacity, p = 0;
                while (new_table[idx].first != -1) idx = (idx + ++p * p) % new_capacity;
                new_table[idx] = {entry.first, false};
            }
        }
        table = std::move(new_table);
        capacity = new_capacity;
    }

public:
    HashTable(int init_capacity = 7) : capacity(init_capacity) {
        table.resize(capacity, {-1, false}); // -1 indicates empty slot, bool for deleted mark
    }

    void insert(int key) {
        if (search(key) != -1) return;
        int idx = hash(key), probes = 0;
        while (table[idx].first != -1 && !table[idx].second && probes < max_probes) {
            idx = (idx + ++probes * probes) % capacity;
        }
        if (probes == max_probes) return; // Max probes reached
        table[idx] = {key, false};
        num_elements++;
        if (num_elements > capacity * 0.7) resize(); // Resize if load factor exceeds 70%
    }

    int search(int key) {
        int idx = hash(key), probes = 0;
        while (table[idx].first != -1 && probes < max_probes) {
            if (table[idx].first == key && !table[idx].second) return idx;
            idx = (idx + ++probes * probes) % capacity;
        }
        return -1; // Key not found
    }

    void remove(int key) {
        int idx = search(key);
        if (idx != -1) table[idx].second = true; // Mark as deleted, without rehashing
    }

    void print() {
        for (const auto &entry : table) {
            if (entry.first != -1 && !entry.second) std::cout << entry.first << " ";
            else std::cout << "- ";
        }
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
