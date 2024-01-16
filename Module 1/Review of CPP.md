# Review of CPP

## Arithmetic Types
- bool
- char
- short
- int
- long
- long long
- float
- double
- long double

## Data Types
Constexxpr: Assigned during compilation
Const: Assigned durring runtime 
Enum:
Size_t: what is returned by size() functions and is guranteed to represent the max size of any object in memory.
Lvalues: Are required for the following operations (acessing data at memory address)
        - &x
        - *x
        - x[i]
        - foo->bar == (*foo).bar
Rvalues:

## Heap vs Stack

## Declaration vs Defenition
Declarations that are defenitions:
int x;
string h;

Declarations that are not defenitions:
void f(int);
class c;
extern int n;

## Casting
1. Static_cast: Converts among 'related types'. restores a pointer from a void*
2. dynamic_cast: For 'downcast' is-a tests in class hiearchies
3. reinterpret_cast: for low-level tomfoolery, casting from int to pointer, casting among unrelated types

## Structs vs Classes

Structs group data together. There are no members or relationships. 

## Floats
Sign bit (S): 1 (negative)
Exponent (E): Bias (127) + actual exponent (4) = 131 (in binary: 10000011)
Mantissa (F): The binary representation of the fractional part 0.625 is 101, and the leading 1 is implicit.

## Coercion
Def: The implicit conversion of one data type to another. 
