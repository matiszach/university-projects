
### Previous In Range

Your task is to implement a data structure that efficiently handles specific queries regarding a sequence of numbers \(X\). Additionally, the elements of the sequence can be revealed online.

You will work with a sequence defined as \(X = x_0, x_1, ..., x_{n-1}\). You are required to implement the following function:

### Function Specification

**Function:**

```cpp
int prevInRange(int i, int lo, int hi);
```

This function returns the largest index \(j\) such that \(0 ≤ j ≤ i\) and \(x_j ∈ [lo, hi]\). If such an index does not exist, the function should return -1.

Formally:
```
prevInRange(i, [lo, hi]) = max{ 0 ≤ j ≤ i : x_j ∈ [lo, hi] }
```
or -1 if such an index does not exist.

### Module Interface

Your module should provide the following functions:

1. **Initialization:**
   ```cpp
   void init(const vector<int> &x);
   ```
   Initializes the initial sequence \(X\) with the values from the vector \(x\). The sequence can contain any values within the integer type range.

2. **Query Function:**
   ```cpp
   int prevInRange(int i, int lo, int hi);
   ```
   Computes the value of `prevInRange(i, [lo, hi])`. You can assume \(0 ≤ i < |X|\) and `INT_MIN ≤ lo ≤ hi ≤ INT_MAX`.

3. **Adding Elements:**
   ```cpp
   void pushBack(int v);
   ```
   Adds an element with value \(v\) to the end of the current sequence \(X\).

4. **Memory Cleanup:**
   ```cpp
   void done();
   ```
   Frees all memory used to manage the sequence \(X\).

### File Structure

- **Header File:** `prev.h` (contains declarations of the above functions)
- **Implementation File:** `prev.cpp` (implement the functions)
- **Main File:** `main.cpp` (contains interaction examples)

### Compilation Command

You can compile your program using the following command:
```
g++ @opcjeCpp main.cpp prev.cpp -o main.e
```

### Time Complexity Requirements

To achieve maximum points:
- The amortized time complexity for the functions `prevInRange` and `pushBack` should be \(O(\log z)\), where \(z\) is the range of the integer type.
- The complexity of the `init` function should be at worst \(O(|X| \log z)\).

A solution that relies solely on brute force will receive a score of 0.

### Memory Management

Your solution will also be tested using Valgrind to check for memory leaks. In case memory leaks are detected, you can run the following command:
```
valgrind --tool=memcheck --leak-check=yes ./main.e
```

### Example Interaction

Here's a simple example of how the functions might be used:

#### Input:
```cpp
vector<int> x = {5, 10, 15, 20};
init(x);

cout << prevInRange(3, 5, 15); // Output: 2 (index of value 15)
cout << prevInRange(2, 6, 15); // Output: 1 (index of value 10)

pushBack(12);
cout << prevInRange(4, 10, 15); // Output: 3 (index of value 12)

done(); // Free memory
```

#### Output:
```
2
1
3
```

This interaction demonstrates initializing the sequence, querying the previous index within a specified range, adding an element, and finally freeing the memory.
