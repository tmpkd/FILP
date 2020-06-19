#include <iostream>

using namespace std;


template <class Type>
class SmartPointer {
public:
    SmartPointer(Type *object) : Pointer(object) {
        RefsCount = new uint();
        (*RefsCount)++;
        cout << "References count init: " << *RefsCount << endl;
    }

    SmartPointer(SmartPointer *pointer) {
        Link(pointer);
    }

    ~SmartPointer() {
        Dispose();
    }

    Type* Get() const {
        if (!Pointer) {
            cout << "Pointer was removed!" << endl;
            exit(1);
        }
        return Pointer;
    }

    void Set(Type *pointer) {
        Pointer = pointer;
    }

    SmartPointer& operator=(const SmartPointer& right) {
        if (this == &right)
            return *this;

        Dispose();
        Link(right);
        return *this;
    }

    Type* operator->() {
        return Pointer;
    }

    Type* operator*() {
        return Pointer;
    }

private:
    Type* Pointer;
    uint* RefsCount;

    void Dispose() {
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

    void Link(SmartPointer *pointer){
        Pointer = pointer->Pointer;

        RefsCount = pointer->RefsCount;
        (*RefsCount)++;
        cout << "References count increased: " << *RefsCount << endl;
    }
};

void start_test(int test_number) {
    cout << "Test " << test_number << endl;
}

void end_test() {
    cout << "----------------------" << endl;
}

void test_1() {
    start_test(1);

    char* pointer = new char(97); // a
    cout << *pointer << endl; // a

    auto* smart_pointer = new SmartPointer<char>(pointer); // References count init: 1
    cout << smart_pointer->Get() << endl; // a
    auto* smart_pointer2 = new SmartPointer<char>(smart_pointer); // References count increased: 2
    cout << smart_pointer2->Get() << endl; // a

    delete smart_pointer; // References count decreased: 1
    cout << smart_pointer2->Get() << endl; // a

    delete smart_pointer2; // References count decreased: 0; Deleting...

    end_test();
}

void test_2() {
    start_test(2);

    char* pointer = new char(98); // b

    auto* smart_pointer = new SmartPointer<char>(pointer); // References count init: 1
    auto* smart_pointer2 = smart_pointer; // None

    cout << smart_pointer2->Get() << endl; // b
    delete smart_pointer; // References count decreased: 0; Deleting...
    cout << smart_pointer2->Get() << endl; // Pointer was removed!; exit code 1

    delete smart_pointer2; // None

    end_test();
}

void test_3() {
    start_test(3);

    char* pointer = new char(99); // c
    char* pointer_2 = new char(100); // d

    auto* smart_pointer = new SmartPointer<char>(pointer); // References count init: 1

    smart_pointer->Set(pointer_2); // None

    delete pointer_2; // None

    end_test();
}

int main() {
    test_1();
    test_2();
    test_3();

    return 0;
}
