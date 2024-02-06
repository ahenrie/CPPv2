# Placement New Operator
A special purpose operator of the new operator. 
- Does in-place initialization
- No memory is allocated, since the memory to initialize will already exists.
- The user gets to choose where the new initialized data structure goes in memory.

```CPP
//makes pointers to the next spot in the lists
//makes a linked list structure of 12-byte nodes omtp this 120 byte buffer
void* data = new char[120];

void* freePtr = nullptr;
void* ptr = data;
void* next = reinterrupt_cast<char*>(ptr) + 12; //grab the pointer type for next then add 12

for (size_t i = 0; i < 9; i++) {
  new(ptr) void* (next);

  //advance to next virtual node
  ptr = next;
  next = reinterpret_cast<char*>(ptr) + 12;
}
```

# Templates
In C++, templates are a powerful feature that allows you to write generic classes and functions. Templates enable you to define functions or classes that can work with any data type, rather than being restricted to a specific data type. This makes your code more flexible and reusable.

```CPP
template<typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

int main() {
    int x = 5, y = 10;
    swap(x, y);  // Swaps the values of x and y

    double a = 3.14, b = 6.28;
    swap(a, b);  // Swaps the values of a and b

    return 0;
}
```
