#include <iostream>
#include <vector>
#include <utility> // for std::pair

class HashTable {
    std::vector<std::pair<int, bool>> table; // pair for key and deletion flag
    int capacity;
    int num_elements = 0;

    // Hash function
    int hash(int key) {
        return key % capacity;
    }

    // Check if a number is prime
    bool is_prime(int n) {
        if (n <= 1) return false;
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) return false;
        }
        return true;
    }

    // Get next prime number
    int next_prime(int n) {
        while (!is_prime(n)) n++;
        return n;
    }

    // Resize the hash table
    void resize() {
        int new_capacity = next_prime(capacity * 2);
        std::vector<std::pair<int, bool>> new_table(new_capacity, {-1, false});

        for (const auto &entry : table) {
            if (entry.first != -1 && !entry.second) { // only rehash non-deleted entries
                int idx = entry.first % new_capacity;
                int probes = 0;
                while (new_table[idx].first != -1) {
                    idx = (idx + ++probes * probes) % new_capacity;
                }
                new_table[idx] = {entry.first, false}; // insert into new table
            }
        }

        table = std::move(new_table);
        capacity = new_capacity; // Update capacity
    }

public:
    // Constructor
    HashTable(int init_capacity = 7) : capacity(init_capacity) {
        table.resize(capacity, {-1, false}); // Initialize table with -1 for keys and false for deletion
    }

    // Insert a key
    void insert(int key) {
        if (search(key) != -1) return; // Ignore duplicates
        int idx = hash(key);
        int probes = 0;

        while (table[idx].first != -1 && probes < 7) { // max_probes can be adjusted
            idx = (idx + ++probes * probes) % capacity;
        }

        if (probes < 7) {
            table[idx] = {key, false}; // Insert the key
            num_elements++;
            if (num_elements > capacity * 0.7) resize(); // Resize if load factor > 0.7
        }
    }

    // Search for a key
    int search(int key) {
        int idx = hash(key);
        int probes = 0;

        while (probes < 7) {
            if (table[idx].first == key && !table[idx].second) {
                return idx; // Key found
            }
            if (table[idx].first == -1) return -1; // Key not found
            idx = (idx + ++probes * probes) % capacity; // Quadratic probing
        }
        return -1; // Key not found
    }

    // Remove a key
    void remove(int key) {
        int idx = search(key);
        if (idx != -1) {
            table[idx].second = true; // Mark as deleted
            num_elements--;
        }
    }

    // Print the hash table
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

