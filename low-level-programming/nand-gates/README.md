# NAND Gates Circuit DLL

## Warning

The Project uses system-specific functionalities and works only on `Linux`.

## Description

This project is a dynamically loaded C library that implements the functionality of creating logic circuits with NAND gates.

The library provides following functions:
#### `nand_t * nand_new(unsigned n);`
- Creates a new `NAND` gate with `n` inputs and `1` output.
- Returns a pointer to the `NAND` gate structure, or `NULL` if memory allocation failed. If an error occurs, `errno` is set to `ENOMEM`.

#### `void nand_delete(nand_t *g);`
- Disconnects the input and output signals of the specified `NAND` gate and frees all the memory it uses.
- Does nothing if `g` is `NULL`. After calling this function, the passed pointer becomes invalid.

#### `int nand_connect_nand(nand_t *g_out, nand_t *g_in, unsigned k);`
- Connects the output of `g_out` to the `k`-th input of `g_in`, disconnecting any signal that was previously connected to this input.
- Returns:
  - `0` on success.
  - `-1` if any pointer is `NULL`, `k` is invalid, or memory allocation fails. In this case, `errno` is set to `EINVAL` or `ENOMEM`.

#### `int nand_connect_signal(bool const *s, nand_t *g, unsigned k);`
- Connects the boolean signal `s` to the `k`-th input of the `NAND` gate `g`, disconnecting any previously connected signal.
- Returns:
  - `0` on success.
  - `-1` if any pointer is `NULL`, `k` is invalid, or memory allocation fails. In this case, `errno` is set to `EINVAL` or `ENOMEM`.

#### `ssize_t nand_evaluate(nand_t **g, bool *s, size_t m);`
- Evaluates the output signals of the provided `NAND` gates and computes the length of the critical path.
- Returns:
  - The length of the critical path on success, and `s[i]` contains the output value of the `i`-th `NAND` gate.
  - `-1` if any pointer is `NULL`, `m` is zero, or memory allocation fails. In this case, `errno` is set to `EINVAL`, `ECANCELED`, or `ENOMEM`, and the contents of `s` are undefined.

#### `ssize_t nand_fan_out(nand_t const *g);`
- Returns the number of inputs connected to the output of the specified `NAND` gate.
- Returns:
  - The number of connected inputs on success.
  - `-1` if `g` is `NULL`. In this case, `errno` is set to `EINVAL`.

#### `void* nand_input(nand_t const *g, unsigned k);`
- Returns a pointer to the signal (either `bool*` or `nand_t*`) connected to the `k`-th input of the `NAND` gate `g`.
- Returns:
  - A pointer to the signal or `NULL` if nothing is connected to the input.
  - `NULL` if `g` is `NULL` or `k` is invalid. In this case, `errno` is set to `EINVAL`.

#### `nand_t* nand_output(nand_t const *g, ssize_t k);`
- Iterates through the gates connected to the output of the `NAND` gate `g`.
- Returns:
  - A pointer to the `NAND` gate connected to the output of `g`.
  - The result is undefined if the parameters are incorrect.

## Usage

To build the library, simply use the provided `Makefile`:
```
make all
```

And to clean up:
```
make clean
```
