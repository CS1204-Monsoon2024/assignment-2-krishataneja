#include <iostream>
#include <vector>
#include <utility> // for std::pair

class HashTable {
    std::vector<std::pair<int, bool>> table; // pair for key and a flag for deletion
    int capacity; // current capacity of the hash table
    int num_elements; // number of elements in the hash table

    // Hash function
    int hash(int key) const {
        return key % capacity;
    }

    // Check if a number is prime
    bool is_prime(int n) const {
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
                int i = 0;

                while (new_table[idx].first != -1) { // Quadratic probing
                    idx = (idx + (i * i)) % new_capacity;
                    i++;
                }
                new_table[idx] = {entry.first, false}; // insert into new table
            }
        }

        table = std::move(new_table);
        capacity = new_capacity; // Update capacity
    }

public:
    // Constructor
    HashTable(int init_capacity = 7) : capacity(init_capacity), num_elements(0) {
        table.resize(capacity, {-1, false}); // Initialize table
    }

    // Insert a key
    void insert(int key) {
        if (search(key) != -1) return; // Ignore duplicates
        int idx = hash(key);
        int i = 0;

        while (table[idx].first != -1) { // Quadratic probing
            idx = (idx + (i * i)) % capacity;
            i++;
        }

        table[idx] = {key, false}; // Insert the key
        num_elements++;
        if (static_cast<double>(num_elements) / capacity >= 0.8) {
            resize(); // Resize if load factor > 0.8
        }
    }

    // Search for a key
    int search(int key) {
        int idx = hash(key);
        int i = 0;

        while (i < capacity) {
            if (table[idx].first == key && !table[idx].second) {
                return idx; // Key found
            }
            if (table[idx].first == -1) return -1; // Key not found
            idx = (idx + (i * i)) % capacity; // Quadratic probing
            i++;
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
    void printTable() {
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
    HashTable ht(7); // Initialize with size 7

    // Example test cases
    ht.insert(1); ht.printTable();
    ht.insert(6); ht.printTable();
    ht.insert(15); ht.printTable();
    ht.insert(25); ht.printTable();
    ht.remove(15); ht.printTable();
    ht.insert(29); ht.printTable();

    int find = ht.search(22);
    std::cout << "Found at: " << find << std::endl;

    return 0;
}

