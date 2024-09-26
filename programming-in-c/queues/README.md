### Queues

Your task is to simulate the events occurring in a workday of a government office with multiple service counters. There are `m` service counters numbered from 0 to `m-1`. Clients queue up at each counter, and when a clerk is available, they serve the first client in the queue. Once a client is served, they leave the office and do not return for the day. Additionally, there are various other situations that occur during the workday, and your task is to implement functions to handle these situations.

Here are the descriptions of the functions you need to implement:

### 1. `void otwarcie_urzedu(int m)`
This function is called once and only once at the beginning of the simulation. It informs the system that there are `m` service counters in the office (along with one special counter). At this point, there are no clients in the office.

### 2. `interesant* nowy_interesant(int k)`
A new client enters the office and immediately queues up at counter `k`. This function should create a new instance of type `interesant` and return a pointer to this instance.

### 3. `int numerek(interesant* i)`
Returns the unique number assigned to the client `i`. Each client is assigned a sequential number starting from 0 upon entering the office.

### 4. `interesant* obsluz(int k)`
The clerk at counter `k` attempts to serve the next client. If there is a client in the queue, the first client is served and leaves the office; the function returns a pointer to that client. If no clients are in the queue, the function should return `NULL`.

### 5. `void zmiana_okienka(interesant* i, int k)`
A client `i` realizes they are in the wrong queue and moves from their current queue to the end of the queue at counter `k`. You can assume that just before calling this function, client `i` was in some queue other than counter `k`.

### 6. `void zamkniecie_okienka(int k1, int k2)`
The clerk at counter `k1` takes a break. All the clients waiting in the queue at counter `k1` are redirected to counter `k2` and join the end of the queue there in the same order they were in queue `k1`. Clients who queue up later at `k1` may still be served later.

### 7. `std::vector<interesant*> fast_track(interesant* i1, interesant* i2)`
A special counter opens temporarily, where clients can be served immediately. A group of clients standing consecutively in a queue realizes this and moves to the special counter, where they are served immediately in the same order they were in the original queue. The group of clients is given by the first (`i1`) and last (`i2`) clients in the group; if `i1 = i2`, only client `i1` is served. The function returns a `vector` of clients served at the special counter, in the order they were served.

### 8. `void naczelnik(int k)`
The head of the office occasionally steps out of their office and reverses the order of the clients in queue `k`. If there is at most one client in the queue, the operation has no effect.

### 9. `std::vector<interesant*> zamkniecie_urzedu()`
This function is called once at the end of the day. All remaining clients are served quickly, and the office closes for the day. The function returns a `vector` of all the remaining clients in the order they are served: first, all clients at counter 0 (in the order they are served), then all clients at counter 1, and so on.

### Memory Management
Your library does not free memory for any client. This responsibility is left to the user of the library, who will free memory only after clients leave the office. The user will free memory using the `free` function, so your library must allocate memory using `malloc`.

### Compilation
Use the following command to compile your program:

```
g++ @opcjeCpp main.cpp kol.cpp -o main.e
```

Differences in `opcjeCpp` compared to the `opcje` for C:
- The `-Wvla` (which treated usage of variable-length arrays as an issue) and `-Wjump-misses-init` (C-specific option) flags have been removed.
- The `-std=c17` flag has been replaced with `-std=c++17`.

### Time Complexity
To achieve maximum points, the time complexity of each function must be proportional to the size of its parameters and output. The exceptions are `otwarcie_urzedu` and `zamkniecie_urzedu`, which may additionally use `O(m)` time.

### Example Input:
```
otwarcie_urzedu(3)

interesant* i1 = nowy_interesant(0)
interesant* i2 = nowy_interesant(1)
interesant* i3 = nowy_interesant(2)

interesant* o1 = obsluz(0) # Should return i1
interesant* o2 = obsluz(1) # Should return i2
interesant* o3 = obsluz(2) # Should return i3

void zmiana_okienka(i2, 2)

void zamkniecie_okienka(2, 1)

vector<interesant*> clients = zamkniecie_urzedu()
```

### Example Output:
```
otwarcie_urzedu(3)

i1 = 0
i2 = 1
i3 = 2

o1 = i1
o2 = i2
o3 = i3

clients = [i2, i3]
```
