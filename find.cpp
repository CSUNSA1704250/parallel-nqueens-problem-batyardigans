#include <iostream>  
#include <iomanip>  
#include <fstream>
#include <omp.h>

using namespace std;

bool found = false;

int SIZE=0;

void solution_print(int queens[]) {
    ofstream file("files/solutions.txt");
    file << "#Solutions for "<< SIZE << " queens\n\n";
    file << "1 ";
    for(int row=0; row<SIZE; row++) {
      file << queens[row];
      file << " ";
    }
    file << "\n";
    file.close();
}

void setQueen(int queens[], int row, int col, int id) {
  for(int i=0; i<row; i++) {
    if (queens[i]==col) {
      return;
    }
    if (abs(queens[i]-col) == (row-i) ) {
      return;
    }
  }

  queens[row]=col;

  if (found)
      return;

  if(row==SIZE-1) {
      found = true;
#pragma omp critical
      solution_print(queens);
      return;
  }
  else {
    for(int i=0; i<SIZE; i++) {
      setQueen(queens, row+1, i, id);
    }
  }

}

void solve() {
  int myid=0;

#pragma omp parallel
#pragma omp single
{
    for(int i=0; i<SIZE; i++) {
        
#pragma omp task
        setQueen(new int[SIZE], 0, i, myid);
    }
}
}

int main(int argc, char*argv[]) {

  if(argc !=2) {
    cerr << "Error.\n";
    return 0;
  }

  SIZE = atoi(argv[1]);
  double st=omp_get_wtime();
  solve();

  double en=omp_get_wtime();
  printf("Execution time:  %lf\n",en-st);

return 0;
}