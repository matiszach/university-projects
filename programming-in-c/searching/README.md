### Searching

Ala has chosen a number \(x\) within the range from 1 to \(n\). Bolek is trying to guess this number. The only questions Bolek can ask are of the form: "Is \(x < y\)?" for any number \(y\) of his choosing. For each of these questions, Bolek receives a response of "Yes" or "No." The goal for Bolek is to guess \(x\) while minimizing the number of questions asked. Unfortunately, Ala can sometimes lie. Your task is to write a program that plays this game as Bolek.

The role of Ala is played by a library, and you will receive its interface in the file `wys.h`. At the beginning of your interaction with the library, you should call the function:

```cpp
void dajParametry(int &n, int &k, int &g);
```

This function will provide you with three numbers: \(n\) (the range), \(k\) (the maximum number of times Ala can lie in one game), and \(g\) (the number of games to be played with the given parameters \(n\) and \(k\)). The first game begins after this function is called.

In each game, you can only ask Ala questions using the function:

```cpp
bool mniejszaNiz(int y);
```

The result of this function is Ala's response to the question "Is \(x < y\)?" for the unknown number \(x\). Specifically, if the result is `true`, it means Ala answers "Yes," and if the result is `false`, it means Ala answers "No." Ala's answer does not have to be truthful, but during a single game, she can lie at most \(k\) times.

At the end of the game, you should provide Ala with your answer using the function:

```cpp
void odpowiedz(int x);
```

Calling this function will automatically start a new game if there are still \(g\) games to be played. Otherwise, calling this function will end the execution of your program.

Your program should ask the minimum possible number of queries in the following sense: If for the given \(n\) and \(k\), Bolek can guess the answer using \(z\) questions regardless of the value of \(x\) and when Ala chooses to lie (if at all), then your program must always provide the correct answer using at most \(z\) questions.

Your program should function for the parameters: \(1 ≤ n ≤ 12\), \(0 ≤ k ≤ 3\), \(1 ≤ g ≤ 10,000\). The execution time of the program should not matter as long as it is not excessively poor (a few seconds is acceptable).

A sample strategy for Ala can be found in the file `wyslib.cpp`. A sample, non-optimal solution implementing Bolek's strategy can be found in the file `wys_naive.cpp`. (NOTE: This solution has been slightly improved.) You can compile this solution along with the provided library using the command below, with the same compilation options as in the previous two tasks:

```bash
g++ @opcjeCpp wys_naive.cpp wyslib.cpp -o wys_naive
```
