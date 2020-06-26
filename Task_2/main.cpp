#include "map"
#include "buddy_alloc_algo.cpp"

using namespace std;

int main()
{
    auto* allocator = new Allocator((size_t)128);
    void* block_start_ptr_1 = allocator->alloc((size_t)50);
    void* block_start_ptr_2 = allocator->alloc((size_t)25);
    allocator->dealloc(block_start_ptr_1);
    allocator->dealloc(block_start_ptr_2);
    return 0;
}
