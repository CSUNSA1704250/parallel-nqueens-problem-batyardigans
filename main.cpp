#include <bits/stdc++.h>
#include <omp.h>
#include <iostream>

using namespace std;

bool found = false;

long long int ans = 0;

int  SIZE, MASK;

ofstream file("files/solutions.txt");

string allsolutions = "";

// ------------all----------

void printtable(int left,int right, int SIZE)
{   
    std::string lef = std::bitset< 32 >(left ).to_string(); // string conversion
    std::string righ = std::bitset< 32 >(right ).to_string(); // string conversion

    cout << lef <<" " <<righ <<endl;
}


void task(int y, int left, int down, int right,string board)
{
    int  bitmap, bit;
    if (y == SIZE)
    {   

        #pragma omp critical
            ans++;
        #pragma omp critical
            allsolutions+= board + '\n';
    } 
    else
    {
        bitmap = MASK & ~(left | down | right);
        while (bitmap) {
            bit = -bitmap & bitmap;
            bitmap ^= bit;
            char l=' ';
            char r=47;
            for (int i=bit ;i >= 1;i = i>>1){
                if(r< 57){
                    r++;
                }else{
                    l = 49;
                    r = 48;
                }
            }
            task(y+1, (left | bit)<<1, down | bit, (right | bit)>>1,board + l+r+' ' );
        }
    }
}

void divideTask(int n){
    SIZE = n;
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
            string board="";
            char l=' ';
            char r=47;
            for (int i=bit ;i >= 1;i = i>>1){
                if(r< 57){
                    r++;
                }else{
                    l = 49;
                    r = 48;
                }
            }
            #pragma omp task 
            task(y+1, (left | bit)<<1, down | bit, (right | bit)>>1,board+l+r+' ');
        }

    }

}

// ------------find----------

void print_find(int queens[], int SIZE){
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

void task_find(int queens[], int row, int col, int id, int SIZE){
    for(int i=0; i<row; i++){
        if (queens[i]==col) 
            return;
        if (abs(queens[i]-col) == (row-i))
            return;
    }
    queens[row]=col;
    if(found)
        return;
    if(row == SIZE-1){
        found = true;
        #pragma omp critical
        {
            print_find(queens, SIZE);
        }
        return;
    }
    else{
        for(int i=0; i<SIZE; i++){
            task_find(queens, row+1, i, id, SIZE);
        }
    }
}

void queen_find(int SIZE){
    int myid=0;
    #pragma omp parallel
    #pragma omp single
    {
        for(int i=0; i<SIZE; i++){
            #pragma omp task
            task_find(new int[SIZE], 0, i, myid, SIZE);
        }
    }
}

// ------------main----------

int main(int argc, char** argv){
    int queens = stoi(argv[2]);
    string problemType = argv[1];

    if(argc != 3)
    {
        std::cout << "ERRROR ARGS::"<< argv[0]<<"<type [all, find]> <number_of_queens> \n";
        exit(1);
    }

    if(problemType == "all"){
        auto begin = std::chrono::high_resolution_clock::now(); 
        divideTask(queens);
        std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - begin;

        // Print board size, number of solutions, and execution time. 
        std::cout << "Board Size: " << queens << std::endl; 
        std::cout << "Number of solutions: " << ans << std::endl; 
        std::cout << "Execution time: "  << std::fixed << std::setprecision(9) << elapsed.count() << " seconds." <<std::endl;
            
        file << "#Solutions for "<<queens<<" queens"<<std::endl; 
        file << ans << std::endl; 

        file << allsolutions;
        file.close();
    }

    if(problemType == "find"){
        auto begin = std::chrono::high_resolution_clock::now(); 
        queen_find(queens);
        std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - begin;
        
        std::cout << "Execution time: "  << std::fixed << std::setprecision(9) << elapsed.count() << " seconds." <<std::endl;
    }

    return 0;
}