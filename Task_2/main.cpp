#include "map"
#include "buddy_alloc_algo.cpp"

using namespace std;

void start_test(int test_number) {
    cout << "----------------------" << endl;
    cout << "Test " << test_number << endl;
    cout << "----------------------" << endl;
}

void test1() {
    start_test(1);

    auto* allocator = new Allocator((size_t)128);

    void* block_start_ptr_1 = allocator->alloc((size_t)50);
    void* block_start_ptr_2 = allocator->alloc((size_t)25);
    cout << endl << "Release first block" << endl;
    allocator->dealloc(block_start_ptr_1);
    cout << "Release second block" << endl;
    allocator->dealloc(block_start_ptr_2);
}

void test2() {
    start_test(2);

    auto* allocator = new Allocator((size_t)1024);

    void* a = allocator->alloc(512);
    void* b = allocator->alloc(60);

    cout << endl << "Release big block" << endl << endl;
    allocator->dealloc(a);

    void* c = allocator->alloc(5);
    void* d = allocator->alloc(256);

    cout << endl << "Release second medium block" << endl;
    allocator->dealloc(d);
    cout << "Release very small block" << endl;
    allocator->dealloc(c);
    cout << "Release first medium block" << endl;
    allocator->dealloc(b);
}

int main()
{
    test1();
    test2();

    return 0;
}
