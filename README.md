# Final Project
Kevin Jin and Stanley Wei

## Proposal
This is an implementation of the dynamic memory allocation function `malloc()`. The memory will be provided by a predefined block of memory. Any calls to `malloc` and `free` will allocate and return memory to and from this block. The organization of memory will be handled by a red-black tree. 

### Details
- `mCount` and `mFree` keeps track of number of times `malloc` and `free` are called
- tests should include declaration of `the_heap`
- `the_heap` should be 4MB
- implementation will allocate memory from `the_heap` declared in test 

## Compilation
- `make` to compile  
- `make clean test` to run all test cases  
- `make <test>.exec` to to build one test 
- `./<test>.exec` to run single test  
- `make clean <test>.test` to see results of single test
- `make clean -s test` to run all tests 
- `make t0.exec; gdb ./<test>.exec` to debug 