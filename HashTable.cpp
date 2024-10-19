#include "HashTable.h"

HashTable::HashTable(int initial_size) {
    size = initial_size;
    table = std::vector<int>(size, -1);
    num_elements = 0;
    max_probing_limit = size / 2;  // Adjust as needed
}

int HashTable::hash_function(int key) {
    return key % size;
}

bool HashTable::is_prime(int num) {
    if (num <= 1) {
        return false;
    }
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

int HashTable::next_prime(int num) {
    while (!is_prime(num)) {
        ++num;
    }
    return num;
}

void HashTable::resize() {
    int new_size = next_prime(size * 2);
    std::vector<int> new_table(new_size, -1);
    size = new_size;

    for (int i = 0; i < table.size(); ++i) {
        if (table[i] != -1) {
            int key = table[i];
            int value = table[i + 1];
            int new_index = hash_function(key);
            int probe_count = 0;

            while (new_table[new_index] != -1 && probe_count < max_probing_limit) {
                new_index = (new_index + (probe_count * probe_count)) % size;
                ++probe_count;
            }

            if (probe_count >= max_probing_limit) {
                std::cout << "Max probing limit reached!" << std::endl;
                return;
            }

            new_table[new_index] = key;
            new_table[new_index + 1] = value;
        }
    }

    table = new_table;
}

void HashTable::insert(int key, int value) {
    if (search(key) != -1) {
        std::cout << "Duplicate key insertion is not allowed" << std::endl;
        return;
    }

    int index = hash_function(key);
    int probe_count = 0;

    while (table[index] != -1 && probe_count < max_probing_limit) {
        index = (index + (probe_count * probe_count)) % size;
        ++probe_count;
    }

    if (probe_count >= max_probing_limit) {
        std::cout << "Max probing limit reached!" << std::endl;
        return;
    }

    table[index] = key;
    table[index + 1] = value;
    num_elements++;

    if (static_cast<double>(num_elements) / size >= 0.8) {
        resize();
    }
}

int HashTable::search(int key) {
    int index = hash_function(key);
    int probe_count = 0;

    while (table[index] != -1 && probe_count < max_probing_limit) {
        if (table[index] == key) {
            return index;
        }
        index = (index + (probe_count * probe_count)) % size;
        ++probe_count;
    }

    return -1;
}

void HashTable::remove(int key) {
    int index = search(key);
    if (index == -1) {
        std::cout << "Element not found" << std::endl;
        return;
    }

    table[index] = -1;
    table[index + 1] = -1;
    num_elements--;
}

void HashTable::printTable() {
    for (int i = 0; i < size; ++i) {
        if (table[i] != -1) {
            std::cout << table[i] << " ";
        } else {
            std::cout << "- ";
        }
    }
    std::cout << std::endl;
}
