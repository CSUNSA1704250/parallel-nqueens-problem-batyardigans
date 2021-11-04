#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

long long int ans = 0;

int  SIZE, MASK;

vector<vector<int>> res;


void task(int y, int left, int down, int right,vector<int>& r)
{
    int  bitmap, bit;
    if (y == SIZE)
    {   
        #pragma omp critical
        ans++;
    } 
    else
    {
        bitmap = MASK & ~(left | down | right);
        while (bitmap) {
            bit = -bitmap & bitmap;
            bitmap ^= bit;
            task(y+1, (left | bit)<<1, down | bit, (right | bit)>>1,r);
        }
    }
}

void divideTask(int n){
    SIZE = n;
    res.resize(SIZE);
    #pragma omp parallel
    #pragma omp single
    {
        int y = 0,left = 0,right = 0,down = 0;
        int  bitmap, bit;
        MASK = (1 << SIZE) - 1;
        bitmap = MASK & ~(left | down | right);
        while (bitmap) {
            bit = -bitmap & bitmap;
            bitmap ^= bit;
            #pragma omp task 
            task(y+1, (left | bit)<<1, down | bit, (right | bit)>>1,res[y]);
        }

    }

}

int main(int argc, char** argv){
    int queens = stoi(argv[2]);
    
    if(argc != 3)
    {
        std::cout << "ERRROR ARGS::"<< argv[0]<<"<type [all, find]> <number_of_queens> \n";
        exit(1);
    }

    auto begin = std::chrono::high_resolution_clock::now(); 
    divideTask(queens);
    std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - begin;

        // Print board size, number of solutions, and execution time. 
    std::cout << "Board Size: " << queens << std::endl; 
    std::cout << "Number of solutions: " << ans << std::endl; 
    std::cout << "Execution time: "  << std::fixed << std::setprecision(9) << elapsed.count() << " seconds." <<std::endl;

    return 0;
}