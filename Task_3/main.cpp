#include <iostream>

using namespace std;


template <class T>
class SmartPointer {
public:
    SmartPointer(T *object) : Pointer(object) {
        RefsCount = new uint();
        (*RefsCount)++;
        cout << "Smart pointer init" << endl;
    }

    SmartPointer(SmartPointer *pointer) {
        Link(pointer);
    }

    ~SmartPointer() {
        Release();
    }

    T* Get() const {
        if (!Pointer) {
            cout << "Pointer was removed!" << endl;
            exit(1);
        }
        return Pointer;
    }

    void Set(T *pointer) {
        Pointer = pointer;
        RefsCount = new uint();
        (*RefsCount)++;
    }

    void Release() {
        if (!Pointer)
            return;
        (*RefsCount)--;
        cout << "References count decreased: " << *RefsCount << endl;
        if ((*RefsCount) == 0)
        {
            cout << "Deleting..." << endl;
            delete Pointer;
            delete RefsCount;
        }
        Pointer = nullptr;
        RefsCount = nullptr;
    }

    SmartPointer& operator=(const SmartPointer& right) {
        if (this == &right)
            return *this;

        Release();
        Link(right);
        return *this;
    }

    T* operator->() {
        return Pointer;
    }

    T* operator*() {
        return Pointer;
    }

private:
    T* Pointer;
    uint* RefsCount;

    void Link(SmartPointer *pointer){
        Pointer = pointer->Pointer;

        RefsCount = pointer->RefsCount;
        (*RefsCount)++;
        cout << "References count increased: " << *RefsCount << endl;
    }
};

void start_test(int test_number) {
    cout << "----------------------" << endl;
    cout << "Test " << test_number << endl;
    cout << "----------------------" << endl;
}

void test_1() {
    start_test(1);

    char* pointer = new char(97); // a
    cout << *pointer << endl; // a

    auto* smart_pointer = new SmartPointer<char>(pointer); // Smart pointer init
    cout << smart_pointer->Get() << endl; // a
    auto* smart_pointer2 = new SmartPointer<char>(smart_pointer); // References count increased: 2
    cout << smart_pointer2->Get() << endl; // a

    delete smart_pointer; // References count decreased: 1
    cout << smart_pointer2->Get() << endl; // a

    delete smart_pointer2; // References count decreased: 0; Deleting...
}

void test_2() {
    start_test(2);

    char* pointer = new char(98); // b

    auto* smart_pointer = new SmartPointer<char>(pointer); // Smart pointer init
    auto* smart_pointer_2 = new SmartPointer<char>(smart_pointer); // References count increased: 2
    auto* smart_pointer_3 = new SmartPointer<char>(smart_pointer_2); // References count increased: 3

    delete smart_pointer; // References count decreased: 2
    delete smart_pointer_2; // References count decreased: 1
}

void test_3() {
    start_test(3);

    char* pointer = new char(99); // c
    char* pointer_2 = new char(100); // d

    auto* smart_pointer = new SmartPointer<char>(pointer); // Smart pointer init

    smart_pointer->Set(pointer_2); // None

    delete pointer_2; // None
}

void test_4() {
    start_test(4);

    char* pointer = new char(101); // e

    auto* smart_pointer = new SmartPointer<char>(pointer); // Smart pointer init
    auto* smart_pointer2 = smart_pointer; // None

    cout << smart_pointer2->Get() << endl; // e
    delete smart_pointer; // References count decreased: 0; Deleting...
    cout << smart_pointer2->Get() << endl; // Pointer was removed!; exit code 1

    delete smart_pointer2; // None
}

int main() {
    test_1();
    test_2();
    test_3();
    test_4();

    return 0;
}
