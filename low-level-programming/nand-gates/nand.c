#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <errno.h>

#include "nand.h"


// Declaration of structures.

typedef struct node node_t;

typedef struct innode innode_t;

typedef struct vector vector_t;


// Nand gate.

struct nand {
    
    // Inputs array.
    innode_t **inputs;
    
    // Outputs vector.
    vector_t *outputs;
    
    // Number of inputs.
    unsigned n;
    
    // Critical path length.
    ssize_t critical_path_length;
    
    // Is visited.
    bool visited;
    
    // Is on stack.
    bool on_stack;
    
    // Signal.
    bool signal;
};


// A structure responsible for storing gates on a vector.

struct node {
    
    // The vector that the given element is on.
    vector_t *v;
    
    // Nand gate.
    nand_t *val;
    
    // Index on vector.
    unsigned idx;
    
    // Input number to which the gate is connected.
    unsigned nr;
};


// A structure storing the input state.

struct innode {
    
    // Signal connected to the input (NULL if connected to a gate).
    bool *in_signal;
    
    // Gate connected to input (NULL if connected to signal).
    nand_t *connection;
    
    // Pointer to the current gate on the gate vector that is connected to the input.
    node_t *vec_node;
};


// Vector.

struct vector {
	
	// Array.
    node_t **arr;
    
    // Capacity.
    unsigned capacity;
    
    // Current size.
    unsigned size;
};


// Malloc modification, which changes errno status in case of allocation failure.

static void *mylloc(unsigned _size) {
    
    void *tmp = malloc(_size);
    
    if (!tmp) {
        errno = ENOMEM;
    }
    
    return tmp;
}


// New vector creation.

static vector_t *make_vector(void) {
    
    // Allocating memory.
    vector_t *vec = (vector_t*)mylloc(sizeof(vector_t));
    
    if (!vec) {
        return NULL;
    }
    
    vec->arr = (node_t**)mylloc(sizeof(node_t*));
    
    if (!vec->arr) {
        free(vec);
        return NULL;
    }
    
    vec->arr[0] = NULL;
    vec->capacity = 1;
    vec->size = 0;
    
    return vec;
}


// Pushing an element at the end of the vector (doubling capacity if necessary).

static bool push_back(vector_t *v, node_t *g) {
    
    // If there is not enough capacity, we double the size of the vector.
    if (v->size == v->capacity) {
        
        node_t **tmp = (node_t**)mylloc(sizeof(node_t*) * v->capacity * 2);
        
        if (!tmp) {
            return 0;
        }
        
        for (unsigned i = 0; i < v->size; ++i) {
            tmp[i] = v->arr[i];
        }
        
        free(v->arr);
        
        v->arr = tmp;
        v->capacity *= 2;
        
        for (unsigned i = v->size; i < v->capacity; ++i) {
            v->arr[i] = NULL;
        }
    }
    
    // Putting into the last position of the vector.
    g->idx = v->size;
    v->arr[v->size] = g;
    v->size++;
    
    return 1;
}


// Removing an element from the vector.

static void erase(node_t *g) {
    
    // Swapping the removed element with the last element of the vector.
    g->v->arr[g->idx] = g->v->arr[g->v->size - 1];
    g->v->arr[g->idx]->idx = g->idx;
    g->v->size--;
    
    // Resetting the last element.
    g->v->arr[g->v->size] = NULL;
}


// Clearing pointers after the vector.

static void destroy(vector_t *v) {
    
    free(v->arr);
    free(v);
}


// Creating new nand gate.

nand_t *nand_new(unsigned n) {
    
    nand_t *temp = (nand_t*)mylloc(sizeof(nand_t));
    
    if (!temp) {
        return NULL;
    }
    
    temp->n = n;
    
    temp->visited = 0;
    temp->on_stack = 0;
    
    temp->critical_path_length = -1;
    temp->signal = 0;
    
    temp->inputs = (innode_t**)mylloc(sizeof(innode_t*) * n);
    
    if (!temp->inputs) {
        free(temp);
        return NULL;
    }
    
    temp->outputs = make_vector();
    
    if (!temp->outputs) {
        free(temp->inputs);
        free(temp);
        return NULL;
    }
    
    for (unsigned i = 0; i < n; ++i) {
        temp->inputs[i] = NULL;
    }
    
    return temp;
}


// Gate deletion.

void nand_delete(nand_t *g) {
    
    // If NULL we skip.
    if (!g) {
        return;
    }
    
    // Clearing gate's inputs.
    for (unsigned i = 0; i < g->n; ++i) {
        
        if (g->inputs[i]) {
            
            if (g->inputs[i]->connection) {
                erase(g->inputs[i]->vec_node);
                free(g->inputs[i]->vec_node);
            }
            
            free(g->inputs[i]);
        }
    }
    
    free(g->inputs);
    
    // Clearing gate's outputs.
    for (unsigned i = 0; i < g->outputs->size; ++i) {
        
        // Clearing the input of the gate that the current gate enters.
        unsigned nr = g->outputs->arr[i]->nr;
        
        free(g->outputs->arr[i]->val->inputs[nr]);
        
        g->outputs->arr[i]->val->inputs[nr] = NULL;
        
        // Removing connection.
        free(g->outputs->arr[i]);
    }
    
    destroy(g->outputs);
    
    // Deleting the gate itself.
    free(g);
}


// Removing the connection from the k-th input of gate g.

void disconnect(nand_t *g, unsigned k) {
    
    // If the input is not empty then we remove the connection from there.
    if (g->inputs[k]) {
        
        if (g->inputs[k]->connection) {
            erase(g->inputs[k]->vec_node);
            free(g->inputs[k]->vec_node);
        }
        
        free(g->inputs[k]);
        g->inputs[k] = NULL;
    }
}


// Connecting 2 gates.

int nand_connect_nand(nand_t *g_out, nand_t *g_in, unsigned k) {
    
    // Invalid parameters.
    if (!g_out || !g_in || k >= g_in->n) {
        errno = EINVAL;
        return -1;
    }
    
    // Allocating memory.
    node_t *out = (node_t*)mylloc(sizeof(node_t));
    
    if (!out) {
        return -1;
    }
    
    innode_t *input = (innode_t*)mylloc(sizeof(node_t));
    
    if (!input) {
        free(out);
        return -1;
    }
    
    // Creating a connection on both sides.
    out->v = g_out -> outputs;
    out->val = g_in;
    out->nr = k;
    
    input->connection = g_out;
    input->vec_node = out;
    input->in_signal = NULL;
    
    // Connecting to output.
    if (!push_back(g_out->outputs, out)) {
        free(out);
        free(input);
        return -1;
    }
    
    // Connecting to input.
    disconnect(g_in, k);
    g_in->inputs[k] = input;
    
    return 0;
}


// Connecting signal.

int nand_connect_signal(bool const *s, nand_t *g, unsigned k) {
    
    // Invalid parameters.
    if (!s || !g || k >= g->n) {
        errno = EINVAL;
        return -1;
    }
    
    // Creating signal.
    innode_t *input = (innode_t*)mylloc(sizeof(innode_t));
    
    if (!input) {
        return -1;
    }
    
    input->in_signal = (bool*)s;
    
    input->connection = NULL;
    input->vec_node = NULL;
    
    // Connecting signal to input.
    disconnect(g, k);
    g->inputs[k] = input;

    return 0;
}


// Max of 2 integers.

static ssize_t max(ssize_t a, ssize_t b) {
    return (a > b ? a : b);
}


// Calculating the resulting signal for the gates in the graph.
// The function traverses the connection graph using DFS and in postorder
// calculates subsequent signals, verifying the correctness of the graph in the meantime
// by returning an error code (0: OK, -1: Cycle or empty input).

static int get_path(nand_t *v) {
    
    // If the vertex is in the current DFS branch, it means that a cycle has occurred.
    if (v->on_stack) {
        return -1;
    }
    
    // If the vertex has been visited, we do not enter it a second time.
    if (v->visited) {
        return 0;
    }
    
    // Marking that the vertex is visited and that it is in the current DFS branch.
    v->visited = 1;
    v->on_stack = 1;
    
    // Detecting empty inputs.
    for (unsigned i = 0; i < v->n; ++i) {
        
        // Checking whether input is empty.
        if (!v->inputs[i]) {
            return -1;
        }
    }
    
    // Calculating the longest critical path.
    ssize_t path_length = 0;
    bool gate_signal = 1;
    
    for (unsigned i = 0; i < v->n; ++i) {
        
        // There is a gate on the input, so we need to recursively calculate the result for it.
        if (v->inputs[i]->connection) {
            
            int status = get_path(v->inputs[i]->connection);
            
            if (status == -1) {
                return -1;
            }
            
            gate_signal &= v->inputs[i]->connection->signal;
            
            path_length = max(path_length,
                    v->inputs[i]->connection->critical_path_length);
        }
        
        // There is a signal at the input, so we can immediately get its result.
        else {
            gate_signal &= *v->inputs[i]->in_signal;
        }
    }
    
    // Saving the results to the vertex.
    if (v->n > 0) {
        
        // We extend the path only if the gate is not empty.
        ++path_length;
    }
    
    v->critical_path_length = path_length;
    v->signal = gate_signal ^ 1;
    
    // When leaving a vertex, we remove it from the current DFS branch.
    v->on_stack = 0;
    
    return 0;
}


// Restoring the vertex state to that before calculating the longest critical path.

static void clean_up(nand_t *v) {
    
    // If it has not been visited or is empty then we skip it.
    if (!v || !v->visited) {
        return;
    }
    
    // Resetting values.
    v->visited = 0;
    v->on_stack = 0;
    v->critical_path_length = 0;
    v->signal = 0;
    
    // Recursive call for all inputs.
    for (unsigned i = 0; i < v->n; ++i) {
        
        // If the input is not empty and there is a connection to the gateway.
        if (v->inputs[i] && v->inputs[i]->connection) {
            clean_up(v->inputs[i]->connection);
        }
    }
}


// Calculating the length of the longest critical path.

ssize_t nand_evaluate(nand_t **g, bool *s, size_t m) {
    
    // Invalid parameters.
    if (!g || !s || m == 0) {
        errno = EINVAL;
        return -1;
    }
    
    for (unsigned i = 0; i < m; ++i) {
        
        if (!g[i]) {
            errno = EINVAL;
            return -1;
        }
    }
    
    // Calculating the longest critical path.
    int status = 0;
    ssize_t max_length = 0;
    
    for (unsigned i = 0; i < m; ++i) {
        
        if (!g[i]->visited) {
            
            status = get_path(g[i]);
            
            if (status == -1) {
                break;
            }
        }
        
        max_length = max(max_length, g[i]->critical_path_length);
    }
    
    // Calculating the longest critical path failed.
    if (status == -1) {
        
        errno = ECANCELED;
        
        for (unsigned i = 0; i < m; ++i) {
            if (g[i]->visited) {
                clean_up(g[i]);
            }
        }
        
        return -1;
    }
    
    // Retrieving the score from the gates and resetting the graph.
    for (unsigned i = 0; i < m; ++i) {
        s[i] = g[i]->signal;
    }
    
    for (unsigned i = 0; i < m; ++i) {
        
        if (g[i]->visited) {
            clean_up(g[i]);
        }
    }
    
    return max_length;
}


// Number of connections to gate g output.

ssize_t nand_fan_out(nand_t const *g) {
    
    // Invalid parameter.
    if (!g) {
        errno = EINVAL;
        return -1;
    }
    
    return g->outputs->size;
}


// Checking the k-th input of gate g.

void* nand_input(nand_t const *g, unsigned k) {
    
    // Invalid parameters.
    if (!g || k >= g->n) {
        errno = EINVAL;
        return NULL;
    }
    
    if (!g->inputs[k]) {
        errno = 0;
        return NULL;
    }
    
    // Returning the result.
    if (g->inputs[k]->connection) {
        return g->inputs[k]->connection;
    }
    
    return g->inputs[k]->in_signal;
}


// Getting k-th output of gate g.

nand_t *nand_output(nand_t const *g, ssize_t k) {
    
    // Invalid parameters.
    if (!g || k < 0 || k >= nand_fan_out(g)) {
        return NULL;
    }
    
    return g->outputs->arr[k]->val;
}
