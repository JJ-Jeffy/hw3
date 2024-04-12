#pragma once

#include "kmer_t.hpp"
#include <upcxx/upcxx.hpp>

struct HashMap {
    
    // initialize the atomic domain for the hashmap
    upcxx::atomic_domain<int> ad = upcxx::atomic_domain<int>({upcxx::atomic_op::fetch_add,upcxx::atomic_op::load});

    // initialize the distributed objects with global pointers 
    upcxx::dist_object<upcxx::global_ptr<kmer_pair>>* data_ptr;
    upcxx::dist_object<upcxx::global_ptr<int>>* used_ptr;

    // initialize the size of the hashmap and the number of processors
    size_t my_size;
    size_t size_procs;
    size_t size() const noexcept;

    HashMap(size_t size);

    ~HashMap(){
        ad.destroy();
    }

    // Most important functions: insert and retrieve
    // k-mers from the hash table.
    bool insert(const kmer_pair& kmer);
    bool find(const pkmer_t& key_kmer, kmer_pair& val_kmer);

    // Helper functions

    // // Write and read to a logical data slot in the table.
    // void write_slot(uint64_t slot, const kmer_pair& kmer);
    // kmer_pair read_slot(uint64_t slot);

    // // Request a slot or check if it's already used.
    // upcxx::future<int> request_slot(uint64_t slot);
    // bool slot_used(uint64_t slot);
};

HashMap::HashMap(size_t size) {
    my_size = size;
    size_procs = upcxx::rank_n() + 1;

    // initialize the distributed objects with size_procs
    upcxx::dist_object<upcxx::global_ptr<kmer_pair>>data(upcxx::new_array<kmer_pair>(size_procs));
    upcxx::dist_object<upcxx::global_ptr<int>>used(upcxx::new_array<int>(size_procs));

    data_ptr = &data;
    used_ptr = &used;

    // initialize the data and used pointers
    upcxx::barrier(); 
    for (size_t i = 0; i < size_procs; i++) {
        data_ptr[i] = data.fetch(i).wait(); 
        used_ptr[i] = used.fetch(i).wait();
    }   
    upcxx::barrier();
}



bool HashMap::insert(const kmer_pair& kmer) {
    uint64_t hash = kmer.hash();
    bool success = false;
    uint64_t curr_slot = hash % size();

    // index of the processor that has the current slot 
    uint64_t curr_proc = curr_slot / size_procs; 

    // loop through curr_proc and check if the slot is available. If not, then increment using the atomic domain
    for (int i = 0; i <= upcxx::rank_n(); i++){

        // update the current processor based on where we are in the loop
        // this will remain the same if we are on the first iteration
        // and will change as we move on
        curr_proc = (curr_proc + i) % upcxx::rank_n(); 


        // what is the used value of the current slot
        upcxx::global_ptr<int> used_curr_proc = used_ptr->fetch(curr_proc).wait();

        // figure out where we will start looking for the empty slots 
        uint64_t start_loc; 
        if (curr_proc == upcxx::rank_me()){
            start_loc = curr_slot % size(); 
        } else {
            start_loc = 0; 
        }

        // we know where the empty slots start, use while loop to find the first empty slot
        int probe = 0; 
        while ((probe + start_loc) < size_procs){

            // check if the slot is used
            int status = ad.fetch_add(used_curr_proc + (probe + start_loc), 1, std::memory_order_relaxed).wait();

            // if the slot is not used, then we can write the data
            if (status == 0){
                upcxx::global_ptr<kmer_pair> data_curr_proc = data_ptr->fetch(curr_proc).wait();
                upcxx::rput(kmer, data_curr_proc + (probe + start_loc)).wait();
                break;
            }
        }
    }
    return success;
}

bool HashMap::find(const pkmer_t& key_kmer, kmer_pair& val_kmer) {
    uint64_t hash = key_kmer.hash();
    bool success = false;
    uint64_t curr_slot = hash % size();

    // index of the processor that has the current slot
    uint64_t curr_proc = curr_slot / size_procs;

    // this code is similar to the insert function, but we are looking for the key
    for (int i = 0; i <= upcxx::rank_n(); i++){

        curr_proc = (curr_proc + i) % upcxx::rank_n(); 
        upcxx::global_ptr<int> used_curr_proc = used_ptr->fetch(curr_proc).wait();
        uint64_t start_loc; 
        if (curr_proc == upcxx::rank_me()){
            start_loc = curr_slot % size(); 
        } else {
            start_loc = 0; 
        }

        int probe = 0; 
        while ((probe + start_loc) < size_procs){

            // check if the slot is used
            int status = ad.load(used_curr_proc + (probe + start_loc), std::memory_order_relaxed).wait();

            // if the slot is not used, then we can write the data
            if (status == 1){
                upcxx::global_ptr<kmer_pair> data_curr_proc = data_ptr->fetch(curr_proc).wait();
                val_kmer = upcxx::rget(data_curr_proc + (probe + start_loc)).wait();
                success = true;
                break;
            }
        }
    }

    return success;
}


size_t HashMap::size() const noexcept { return my_size; }