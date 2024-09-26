### Approximate Arithmetic Package

In contexts where physical measurements are taken, the results are subject to some margin of error, e.g., 5m ± 10%. Each approximate value is treated as a set of possible values. This package implements arithmetic operations on such approximate values in accordance with the provided interface `ary.h`. Specifically, the package includes:

#### Definition of the `struct wartosc` type

#### Constructors:

- **`wartosc_dokladnosc(x, p)`**  
  Returns `x ± p%` (for `p > 0`).

- **`wartosc_od_do(x, y)`**  
  Returns `(x + y)/2 ± (y - x)/2` (for `x < y`).

- **`wartosc_dokladna(x)`**  
  Returns `x ± 0`.

#### Selectors:

- **`in_wartosc(x, y)`**  
  ⇔ The value `x` may be equal to `y`.

- **`min_wartosc(x)`**  
  Returns the lower bound of possible values for `x` (or `-∞` if the possible values of `x` are not bounded below, or `NaN` if `x` is empty).

- **`max_wartosc(x)`**  
  Returns the upper bound of possible values for `x` (or `∞` if the possible values of `x` are not bounded above, or `NaN` if `x` is empty).

- **`sr_wartosc(x)`**  
  Returns the arithmetic mean of `min_wartosc(x)` and `max_wartosc(x)` (or `NaN` if `min_wartosc(x)` and `max_wartosc(x)` are not finite).

#### Modifiers:

- **`plus(a, b)`**  
  = `{ x + y : in_wartosc(a, x) ∧ in_wartosc(b, y) }`.

- **`minus(a, b)`**  
  = `{ x - y : in_wartosc(a, x) ∧ in_wartosc(b, y) }`.

- **`razy(a, b)`**  
  = `{ x * y : in_wartosc(a, x) ∧ in_wartosc(b, y) }`.

- **`podzielic(a, b)`**  
  = `{ x / y : in_wartosc(a, x) ∧ in_wartosc(b, y) }`.

#### Assumptions:

- All arguments of type `double` are real numbers (i.e., they are not equal to `HUGE_VAL`, `-HUGE_VAL`, or `NaN`).
- If the result is not a real number, it should be appropriately set to one of: `HUGE_VAL`, `-HUGE_VAL`, or `NaN`.

### Simplifying Assumptions:

1. **Closed Interval Results**:  
   The package assumes that all resulting sets of values are closed intervals. If the result is an open interval, it is converted to a closed interval.

2. **Monotonicity**:  
   Arithmetic operations on approximate values are monotonic with respect to the inclusion of sets of possible values. For example, if approximate values `x`, `y`, and `z` satisfy `x ⊆ y` as sets of possible values, then:
   - `plus(x, z) ⊆ plus(y, z)`
   - `plus(z, x) ⊆ plus(z, y)`
   And similarly for other arithmetic operations.

### Examples:

You can find examples based on the above rules in the file `przyklad.c`. To compile the files (with `ary.c` and `ary.h` in the same directory), use the command:
```bash
gcc [options] przyklad.c ary.c -o przyklad.e -lm
```
The `-lm` option links the math library `math.h`. Ensure it is at the end of the compilation command.

### Floating-Point Numbers:

Floating-point numbers and operations on them can behave unexpectedly. For example, the IEEE standard provides two representations for zero (`+0.0` and `-0.0`), where `1.0 / 0.0 = HUGE_VAL` and `1.0 / (-0.0) = -HUGE_VAL`. This can be useful when dividing by an approximate value containing a one-sided neighborhood of zero, but it can be a pitfall when dividing by an exact zero.

In the definition of the `podzielic` operation, "mathematical" division is used, which is undefined when dividing by zero. You may assume that a `double` number is equal to zero if the following function returns `true`:
```c
bool iszero(double x) {
    return fabs(x) < 1e-10;
}
```

Your solution should be placed in a file named `ary.c` and match the interface specification in `ary.h`. Additionally, include the `ary.h` file with only the definition of the `struct wartosc` type.
