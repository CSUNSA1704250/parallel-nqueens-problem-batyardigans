[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-f059dc9a6f8d3a56e377f745f24479a46679e63a5d9fe6f495e02850cd0d8118.svg)](https://classroom.github.com/online_ide?assignment_repo_id=6079037&assignment_repo_type=AssignmentRepo)

# N-queens

## Files
* `solutions.txt` Show all solutions to the problem

```
#Solutions for <n_queens> queens
<n_solutions>
1 <pos_0>  <pos_1> . . . <pos_n_queens - 1>
2 <pos_0>  <pos_1> . . . <pos_n_queens - 1>
3 <pos_0>  <pos_1> . . . <pos_n_queens - 1>
.
.
.
n_solutions <pos_0>  <pos_1> . . . <pos_n_queens - 1>
```

* `graph.dot` Show first solution N-queens
* `graph.png` Show image of the first solution


## Compilation and Execution
N-queens compilation
```
g++ -o bin/main -pthread main.cpp
```
N-queens execution `<nameofprogram> -problemType [all, find] -N <queens>` example:
```
./bin/main find 13
```
Generate dot file and image of the first solution N-queens
```
make image
```