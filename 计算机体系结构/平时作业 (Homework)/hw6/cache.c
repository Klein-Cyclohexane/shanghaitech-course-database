#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 16
#define NUM_BLOCKS 4
#define FULLY_ASSOCIATIVE 1

typedef struct {
    uint32_t tag;           /* Tag of the block */
    int valid;              /* Valid bit */
    int last_used;          /* For LRU replacement policy */
} CacheBlock;

typedef struct {
    CacheBlock blocks[NUM_BLOCKS];
    int access_counter;     /* Counter for LRU */
} Cache;

/* Cache statistics */
typedef struct {
    int total_accesses;     /* Total memory accesses */
    int inst_hits;          /* Instruction cache hits */
    int data_hits;          /* Data cache hits */
    int misses;             /* Cache misses */
} CacheStats;

static Cache cache;
static CacheStats stats;

static uint32_t get_tag(uint32_t address) {
    /* In a fully associative cache, the tag is the block address */
    return address / BLOCK_SIZE;
}

static int find_block(uint32_t tag) {
    int i;
    for (i = 0; i < NUM_BLOCKS; i++) {
        if (cache.blocks[i].valid && cache.blocks[i].tag == tag) {
            return i;  /* Block found */
        }
    }
    return -1;  /* Block not found */
}

static int find_lru_block() {
    int lru_index = 0;
    int min_last_used = cache.blocks[0].last_used;
    int i;
    
    for (i = 1; i < NUM_BLOCKS; i++) {
        if (!cache.blocks[i].valid) {
            return i;  /* Return first invalid block */
        }
        if (cache.blocks[i].last_used < min_last_used) {
            min_last_used = cache.blocks[i].last_used;
            lru_index = i;
        }
    }
    
    return lru_index;
}

static int cache_access_internal(uint32_t address, int is_data) {
    uint32_t tag = get_tag(address);
    int block_index = find_block(tag);
    
    stats.total_accesses++;
    
    if (block_index != -1) {
        /* Cache hit */
        cache.blocks[block_index].last_used = cache.access_counter++;
        if (is_data) {
            stats.data_hits++;
        } else {
            stats.inst_hits++;
        }
        return 1;  /* Hit */
    } else {
        /* Cache miss */
        stats.misses++;
        
        /* Find the LRU block to replace */
        block_index = find_lru_block();
        
        /* Update the block */
        cache.blocks[block_index].tag = tag;
        cache.blocks[block_index].valid = 1;
        cache.blocks[block_index].last_used = cache.access_counter++;
        
        return 0;  /* Miss */
    }
}

void init_cache_system() {
    int i;
    for (i = 0; i < NUM_BLOCKS; i++) {
        cache.blocks[i].valid = 0;
        cache.blocks[i].last_used = 0;
    }
    cache.access_counter = 0;

    stats.total_accesses = 0;
    stats.inst_hits = 0;
    stats.data_hits = 0;
    stats.misses = 0;
}

void add_inst_access(uint32_t address) {
    cache_access_internal(address, 0);  /* 0 for instruction */
}

void add_data_access(uint32_t address) {
    cache_access_internal(address, 1);  /* 1 for data */
}


void print_cache_statistics() {
    printf("Total memory accesses: %d.\n", stats.total_accesses);
    printf("Instruction cache hit: %d.\n", stats.inst_hits);
    printf("Data cache hit: %d.\n", stats.data_hits);
    printf("Total cache misses: %d.\n", stats.misses);
}