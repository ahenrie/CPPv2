# Cont Review

## Pointers

Pointers are variables in C++ that store the memory address of another variable. 
They are a powerful feature of the language and are used in various scenarios. 
Here's an overview of pointers and where they are commonly used:

1. Declaration and Initialization:
   ``` CPP
   int* ptr;` // This is how you declare a pointer to an integer
   int value = 42;
   int* ptr = &value; // This pointer called ptr now holds the address of value.
   ```
2. Dereferencing: Accessing the stored value at the address
   ``` C++
   int value = 42;
   int* ptr = &value;
   cout << *ptr; // This will output the value at the pointer address.
   ```
3. Dynamic Memory Allocation:
Pointers are commonly used for dynamic memory allocation using 'new' and 'delete' operators. This is usefule
when memory needs to be allocated at runtime.
  ```C++
  int* dynamicPtr = new int; // dynamically allocates memory for an integer
  *dynamicPtr = 10;
  // ...
  delete dynamicPtr; // deallocates the memory
  ```

4. Arrays and Pointers
  ```CPP
  int arr[5] = {1, 2, 3, 4, 5};
  int* arrPtr = arr; // arrPtr points to the first element of arr
  ```

5. Functions and Pointers
  ``` CPP
  void myFunc(int x) {
  // something happens
  }

  void (*funcPtr)(int) = myFunction; // pointer to a function
   ```

## Project 1 Overview - Circular Buffers

