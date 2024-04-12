#pragma once

#include "kmer_t.hpp"
#include <upcxx/upcxx.hpp>
#include <map>

struct HashMap {
    using dobj_map_t = upcxx::dist_object<std::unordered_map<std::string,std::string>>;
    dobj_map_t local_map;

    std::vector<kmer_pair> data;
    std::vector<int> used;

    size_t my_size;

    size_t size() const noexcept;

    HashMap(size_t size);

    // map the key to a target process
    int get_target_rank(const std::string &key) {
    return std::hash<std::string>{}(key) % upcxx::rank_n();
    }



    // Most important functions: insert and retrieve
    // k-mers from the hash table.
    // bool insert(const kmer_pair& kmer);
    // bool find(const pkmer_t& key_kmer, kmer_pair& val_kmer);
    // upcxx::future<> insert(const kmer_pair& kmer, const std::string &val);
    upcxx::future<> insert(const kmer_pair& kmer);
    upcxx::future<std::string> find(const pkmer_t& key_kmer, kmer_pair& val_kmer);

    // Helper functions

    // Write and read to a logical data slot in the table.
    void write_slot(uint64_t slot, const kmer_pair& kmer);
    kmer_pair read_slot(uint64_t slot);

    // Request a slot or check if it's already used.
    bool request_slot(uint64_t slot);
    bool slot_used(uint64_t slot);
};

HashMap::HashMap(size_t size) {
    my_size = size;
    data.resize(size);
    used.resize(size, 0);
}

// bool HashMap::insert(const kmer_pair& kmer) {
//     uint64_t hash = kmer.hash();
//     uint64_t probe = 0;
//     bool success = false;
//     do {
//         uint64_t slot = (hash + probe++) % size();
//         success = request_slot(slot);
//         if (success) {
//             write_slot(slot, kmer);
//         }
//     } while (!success && probe < size());
//     return success;
// }


// upcxx::future<> HashMap::insert(const kmer_pair& kmer, const std::string &val){
//     // get key
//     uint64_t hash = kmer.hash();
//     std::string value = kmer.kmer_str();
//     // return empty upcxx:: future by default
//     return upcxx::rpc(get_target_rank(std::to_string(hash)), 
//         // lambda to insert key-value pair
//         [](dobj_map_t &lmap, const uint64_t &hash, const std::string &value) {
//             // insert into the local map at the target
//             lmap->insert({kmer, value});
//         }, local_map, hash, value);


upcxx::future<> HashMap::insert(const kmer_pair& kmer){
    // get key
    uint64_t hash = kmer.hash();
    std::string value = kmer.kmer_str();
    // return empty upcxx:: future by default
    return upcxx::rpc(get_target_rank(std::to_string(hash)), 
        // lambda to insert key-value pair
        [](dobj_map_t &lmap, const uint64_t &hash, const std::string &value) {
            // insert into the local map at the target
            lmap->insert({std::to_string(hash), value});
        }, local_map, hash, value);

}




// bool HashMap::find(const pkmer_t& key_kmer, kmer_pair& val_kmer) {
//     uint64_t hash = key_kmer.hash();
//     uint64_t probe = 0;
//     bool success = false;
//     do {
//         uint64_t slot = (hash + probe++) % size();
//         if (slot_used(slot)) {
//             val_kmer = read_slot(slot);
//             if (val_kmer.kmer == key_kmer) {
//                 success = true;
//             }
//         }
//     } while (!success && probe < size());
//     return success;
// }

upcxx::future<std::string> HashMap::find(const pkmer_t& key_kmer, kmer_pair& val_kmer) {
    std::string key = key_kmer.get();
    return upcxx::rpc(get_target_rank(key),
    // lambda to find the key
    [](dobj_map_t &lmap, const std::string &key) -> std::string {
        auto element = lmap->find(key);
        if(element == lmap->end()) return std::string(); // did not find it
        else return element->second; // key found and we return the value
    }, local_map, key);
}



bool HashMap::slot_used(uint64_t slot) { return used[slot] != 0; }

void HashMap::write_slot(uint64_t slot, const kmer_pair& kmer) { data[slot] = kmer; }

kmer_pair HashMap::read_slot(uint64_t slot) { return data[slot]; }

bool HashMap::request_slot(uint64_t slot) {
    if (used[slot] != 0) {
        return false;
    } else {
        used[slot] = 1;
        return true;
    }
}

size_t HashMap::size() const noexcept { return my_size; }
