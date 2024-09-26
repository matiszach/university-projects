### Origami

Write a program for origami enthusiasts to analyze how many layers of paper are present at a given point on a cleverly folded sheet of paper. The program should read from `stdin` the description of how origami sheets are created and questions about the number of layers in specified points on the sheets. The first line of the input contains two numbers `n` and `q`, representing the number of sheets and the number of queries, respectively. Following this, there are `n` lines describing the sheets and `q` questions asking for the number of layers in specified points on the sheets.

#### Sheet Description

The description of the `i`-th sheet (1 ≤ i ≤ n) is found in line `i + 1`. The description can be in one of the following three formats:

1. **P x1 y1 x2 y2**  
   This describes a closed rectangle with sides parallel to the coordinate axes, with the lower-left corner at point P1 = (x1, y1) and the upper-right corner at point P2 = (x2, y2). Point P1 must be to the left and below point P2. If a pin is stuck inside (or on the edges of) this rectangle, the paper is pierced once; in other cases, the paper is not pierced.

2. **K x y r**  
   This describes a closed circle with its center at point (x, y) and radius `r`.

3. **Z k x1 y1 x2 y2**  
   This describes a sheet created by folding the `k`-th sheet (1 ≤ k < i) along a line passing through points P1 = (x1, y1) and P2 = (x2, y2) - these must be different points. The paper is folded such that the right side of the line (when viewed from P1 to P2) is flipped over to the left side. For such a folded sheet, piercing the paper on the right side of the line results in 0 layers; piercing exactly on the line returns the same number of layers as the sheet before folding; and on the left side of the line, the result is the number of layers before folding plus the number of layers at the corresponding folded point.

#### Query Format

Each of the `j`-th queries (1 ≤ j ≤ q) is found in line `n + j + 1` and has the format:

- **k x y**, where `k` is the sheet number (1 ≤ k ≤ n) and `(x, y)` is the point where we stick a pin to determine the number of layers on the `k`-th sheet at that point.

The program should output `q` lines - the answers to the queries about the number of layers.

### Notes:
- The numbers representing the coordinates of points or the radius of a circle are floating-point numbers.
- There are no time complexity restrictions. Exponential solutions will be allowed.
- Precision-sensitive solutions will be accepted, as our tests will not be malicious with respect to precision.

### Example Input:

```
4 5
P 0 0 2.5 1
Z 1 0.5 0 0.5 1
K 0 1 5
Z 3 0 1 -1 0
1 1.5 0.5
2 1.5 0.5
2 0 0.5
3 4 4
4 4 4
```

### Example Output:

```
1
0
2
1
2
```

### Explanation:

1. For the first query, the pin is stuck at (1.5, 0.5) on sheet 1, which is within the rectangle, so the paper is pierced 1 time.
2. For the second query, the pin is at (1.5, 0.5) on sheet 2, which is created by folding the first sheet along the line. In this case, the point is outside the folded region, so the result is 0 layers.
3. The third query checks point (0, 0.5) on sheet 2, which was folded over, and the pin hits 2 layers due to the fold.
4. The fourth query checks point (4, 4) on sheet 3, which is a circle, and the point is outside the circle, resulting in 1 layer.
5. The fifth query checks point (4, 4) on sheet 4, which is a folded sheet, resulting in 2 layers due to overlapping.

### Additional Considerations:
- Be mindful of floating-point precision when performing geometric calculations.
- Handle the folding logic carefully to ensure correct layer counting based on the side of the fold and overlapping points.

The solution should efficiently handle the inputs and produce the correct number of layers at each query point based on the complex paper folding operations described in the input.
