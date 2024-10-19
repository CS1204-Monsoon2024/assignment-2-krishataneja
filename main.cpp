int main() {
    HashTable ht(7);  // Start with a small table size

    ht.insert(10);
    ht.insert(22);
    ht.insert(31);
    ht.insert(4);
    ht.insert(15);
    ht.insert(28);

    ht.display();

    ht.remove(22);
    ht.display();

    std::cout << "Search for 22: " << (ht.search(22) ? "Found" : "Not found") << std::cout;
    std::cout << "Search for 15: " << (ht.search(15) ? "Found" : "Not found") << std::cout;
    
    return 0;
}
