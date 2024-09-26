### Three values

Imagine a highway, represented as a straight line, along which `n` motels are located. The motels are numbered sequentially from 1 to `n` along the highway. Each motel belongs to a specific motel chain, described by a number ranging from 1 to `n`.

Bajtek told Bitek that while driving on the highway, he stopped at three different motels for the night, each time staying at a motel from a different chain. Now, Bitek is curious about how far apart these three motels were. He wants to find both the closest and farthest sets of three motels from different chains.

Formally, Bitek wants to determine three motels `A`, `B`, and `C`, positioned in this order along the highway, and belonging to different chains, such that:

- The **maximum** of the distances from `A` to `B` and from `B` to `C` is minimized (the "closest" set).
- The **minimum** of the distances from `A` to `B` and from `B` to `C` is maximized (the "farthest" set).

Write a program that reads from `stdin` the number of motels `n`, followed by the description of each motel in sequence along the highway (for each motel, its chain number and the distance from the start of the highway), and outputs two numbers:
1. The minimized maximum of the distances (for the closest set).
2. The maximized minimum of the distances (for the farthest set).

If Bajtek made a mistake and there are no sets of three motels from different chains, both output numbers should be zero. The exact format for input and output is described in the example below (line breaks included at the end of lines).

You can assume that the number of motels is positive and will not exceed one million, and all numbers in the input will be non-negative integers that fit in the `int` data type. It may happen that several motels are located at the same point on the highway, or even that Bajtek stayed in two or three motels at the same location.

A solution that correctly computes only one of the two required values may receive half of the points for the task.

### Compilation Command:

```bash
gcc [options] trz.c -o trz.e
```

### Input Format:

The input consists of the following:

1. An integer `n`, the number of motels.
2. For each motel, two integers: 
   - The **chain number** to which the motel belongs.
   - The **distance** of the motel from the start of the highway.

### Output Format:

Two integers:

1. The minimized maximum distance for the closest set.
2. The maximized minimum distance for the farthest set.

### Example:

#### Input:
```
9
1 2
2 6
2 9
1 13
1 17
3 20
1 26
3 27
1 30
```

#### Output:
```
7 10
```

### Explanation:

- **Closest set:** Motels 3, 4, and 6 (at distances 9, 13, and 20 from the start) belong to different chains (chains 2, 1, and 3) and minimize the maximum distance between them.
- **Farthest set:** Motels 2, 6, and 9 (at distances 6, 20, and 30) belong to different chains (chains 2, 3, and 1) and maximize the minimum distance between them.

### Additional Considerations:

- Ensure your program is optimized to handle up to one million motels efficiently.
- You can assume that the pointer to the input data is valid and that all provided values will conform to the problem's constraints.
