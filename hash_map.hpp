#pragma once

#include "kmer_t.hpp"
#include <upcxx/upcxx.hpp>

struct HashMap {
    
    // initialize the atomic domain for the hashmap
    upcxx::atomic_domain<int> atomicDomain; 

    // initialize the global pointers for the data and used arrays
    upcxx::global_ptr<kmer_pair>* data_ptr;
    upcxx::global_ptr<uint64_t>* used_ptr;

    // initialize the size of the hashmap and the number of processors
    size_t my_size;
    size_t size_procs;
    size_t size() const noexcept;

    HashMap(size_t size);

    // Most important functions: insert and retrieve
    // k-mers from the hash table.
    bool insert(const kmer_pair& kmer);
    bool find(const pkmer_t& key_kmer, kmer_pair& val_kmer);

    // Helper functions

    // Write and read to a logical data slot in the table.
    void write_slot(uint64_t slot, const kmer_pair& kmer);
    kmer_pair read_slot(uint64_t slot);

    // Request a slot or check if it's already used.
    upcxx::future<int> request_slot(uint64_t slot);
    bool slot_used(uint64_t slot);
};

HashMap::HashMap(size_t size) {
    my_size = size;
    size_procs = upcxx::rank_n()+1; 
    
    // create the distributed object here for data and used 
    upcxx::dist_object<upcxx::global_ptr<kmer_pair>> data(upcxx::new_array<kmer_pair>(size_procs));
    upcxx::dist_object<upcxx::global_ptr<int>> used(upcxx::new_array<int>(size_procs));

    // initialize the atomic domain for the hashmap
    upcxx::atomic_domain<int> ad = upcxx::atomic_domain<int>({upcxx::atomic_op::load, upcxx::atomic_op::fetch_add});

    // initialize the global pointers for the data and used arrays
    data_ptr = data;
    used_ptr = used;
}   

bool HashMap::insert(const kmer_pair& kmer) {
    uint64_t hash = kmer.kmer.hash();
    uint64_t probe = 0;
    bool success = false;
    while (!success && probe < size()) {
        uint64_t slot = (hash + probe++) % size();
        probe += 1;
        success = request_slot(slot).wait()>0 ? true : false;
        if (success) {
            write_slot(slot, kmer);
        }
    }
    return success;
}

bool HashMap::find(const pkmer_t& key_kmer, kmer_pair& val_kmer) {
    uint64_t hash = key_kmer.hash();
    uint64_t probe = 0;
    bool success = false;
    while(!success && probe < size()) {
        uint64_t slot = (hash + probe++) % size();
        probe += 1;
        
        val_kmer = read_slot(slot);
        if (val_kmer.kmer == key_kmer) {
            success = true;
        }
    }
}

bool HashMap::slot_used(uint64_t slot) { 
    uint64_t curr = slot/size_procs;
    uint64_t offset = slot%size_procs;
    upcxx::global_ptr<uint64_t> used_ptr = used[curr] + offset;
    return ad.load(used_ptr).wait()>0 ? true : false;
}

void HashMap::write_slot(uint64_t slot, const kmer_pair& kmer) { 
    uint64_t curr = slot/size_procs;
    uint64_t offset = slot%size_procs;
    upcxx::global_ptr<kmer_pair> data_ptr = data[curr] + offset;
    ad.store(data_ptr, kmer).wait();
}

kmer_pair HashMap::read_slot(uint64_t slot) { 
    uint64_t curr = slot/size_procs;
    uint64_t offset = slot%size_procs;
    upcxx::global_ptr<kmer_pair> data_ptr = data[curr] + offset;
    return ad.load(data_ptr).wait();
}

bool HashMap::request_slot(uint64_t slot) {
    uint64_t curr = slot/size_procs;
    uint64_t offset = slot%size_procs;
    upcxx::global_ptr<uint64_t> used_ptr = used[curr] + offset;
    return ad.fetch_add(used_ptr, 1).wait();
}

size_t HashMap::size() const noexcept { return my_size; }

size_t HashMap::size_procs() const noexcept { return size_procs; }

// destroy the hashmap atomic domain
~HashMap() {
    ad.destroy();
}