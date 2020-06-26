#include "map"
#include <iostream>

using namespace std;

class Block {
public:
    size_t size;
    bool is_free = true;
    void* start_ptr;
    Block* buddy;
    Block* parent;

    Block(size_t b_size, void* b_start_ptr)
    {
        size = b_size;
        start_ptr = b_start_ptr;
        buddy = this;
        parent = this;
    }
};

class Allocator {
public:
    size_t size;
    map<void*, Block*> blocks_dict;

    Allocator(size_t a_size)
    {
        size = a_size;
        void* block_ptr = malloc(size);
        auto* block = new Block(size, block_ptr);
        blocks_dict[block_ptr] = block;
    };

    void* alloc(size_t alloc_req_size)
    {
        if (alloc_req_size > size)
            exit(2);
        map<void*, Block*>::iterator iter;
        size_t min_size = size;
        auto* block = new Block((size_t)0, (void*)nullptr);
        for (iter = blocks_dict.begin(); iter != blocks_dict.end(); iter++)
        {
            if (!iter->second->is_free)
                continue;
            if (alloc_req_size == iter->second->size)
            {
                block = iter->second;
                break;
            }
            if (alloc_req_size < iter->second->size && iter->second->size < min_size)
            {
                block = iter->second;
                min_size = iter->second->size;
            }
        }

        if (min_size / 2 >= alloc_req_size)
        {
            auto* new_block = new Block(min_size / 2, block->start_ptr);
            auto* buddy = new Block(min_size / 2, (void*)((size_t)(block->start_ptr) + min_size / 2));
            new_block->is_free = false;
            new_block->parent = block;
            buddy->parent = block;
            buddy->buddy = new_block;
            new_block->buddy = buddy;
            blocks_dict[new_block->start_ptr] = new_block;
            blocks_dict[buddy->start_ptr] = buddy;

            cout << "New block with size " << new_block->size << " : " << new_block->start_ptr << " --> " << (void*)((size_t)new_block->start_ptr + min_size/2) << endl;

            return new_block->start_ptr;
        }

        cout << "New block with size " << block->size << " : " << block->start_ptr << " --> " << (void*)((size_t)block->start_ptr + min_size) << endl;

        return block->start_ptr;
    }

    void dealloc(void* block_start_ptr)
    {
        Block* block = blocks_dict[block_start_ptr];
        block->is_free = true;
        while (true)
        {
            if (!block->buddy->is_free || block->buddy == block)
                break;
            Block* buddy = block->buddy;

            if (block->start_ptr < buddy->start_ptr)
            {
                Block* other = buddy;
                buddy = block;
                block = other;
            }

            map<void*, Block*>::iterator iter;
            for (iter = blocks_dict.begin(); iter != blocks_dict.end(); iter++)
            {
                if (iter->second == buddy)
                    blocks_dict.erase(iter);
            }

            blocks_dict[block->start_ptr] = block->parent;
            block = block->parent;
            block->size *= 2;
        }
    }
};
