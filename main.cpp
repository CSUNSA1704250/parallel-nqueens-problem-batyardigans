#include <iostream>  
#include <omp.h>
#include <chrono>
#include <sys/time.h>
#include <pthread.h>
#include <vector>
#include <iomanip>      // std::setprecision
#include<string>
#include <fstream>  

int boardSize,NTHREADS;
int ** board = NULL;
int * count = NULL;
int nsolution;
std::vector<std::vector<std::string>> solution;



struct args_thread {

	int row;
	int column;

    args_thread(int row, int column) {
		this->row = row;
		this->column = column;
 	}
};

void task(int row, int IDthread){
    int start,end;
    if(row > 0){
        start = 0;
        end =  boardSize-1 ;
    }else{
        start = IDthread ;
        end = IDthread;
    }


    if(row==boardSize) {
        count[IDthread]++;
        std::string temp ="";
        for(int i=0;i<boardSize;i++)
            temp += std::to_string(board[IDthread][i]) + ' ';
        solution[IDthread].push_back(temp);
    }
    int j=0,i;
    // try to fill next row    
    for (i = start; i <= end; i++) {
        //#pragma omp parallel num_threads(2) SAD too low
        for(j=0; j<row; j++) {
            // vertical attacks
            if (board[IDthread][j]==i || abs(board[IDthread][j]-i) == (row-j)) {
                break;
            }
        }
        if(j==row) {
           board[IDthread][row] = i;
            task( row+1, IDthread);
        }
        
    }
    
}

void* decorateTask(void *arg)
{
    args_thread test = *(struct args_thread*) arg;
    task(0, test.column);
}

void divideTask() {
    pthread_t pool[NTHREADS];
    std::vector<int*> sumas;

    for(int i = 0; i < NTHREADS; i++) {
            args_thread* args = new args_thread(0, i);
            pthread_create(&pool[i], NULL,decorateTask,args);
    }
    
    for (int i = 0; i < NTHREADS; i++){
        pthread_join(pool[i], NULL);
    }
    int total = 0;
    for(int i=0;i< NTHREADS;i++){
        total += count[i];
    }
    nsolution = total;
} 

int main(int argc, char*argv[]) {

    if(argc != 3)
    {
        std::cout << "ERRROR ARGS::"<< argv[0]<<"<type [all, find]> <number_of_queens> \n";
        exit(1);
    }

    boardSize = atoi(argv[2]);
    board = new int*[boardSize];
    count = new int[boardSize];
    solution.resize(boardSize);
    NTHREADS = boardSize;

    for(int i=0;i<boardSize;i++){
        count[i] = 0;
        board[i] = new int[boardSize];
        for(int j=0;j<boardSize;j++){
            board[i][j] = 0;
        }    
    }

    auto begin = std::chrono::high_resolution_clock::now(); 
    divideTask();
    std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - begin;
    double execution_time = elapsed.count();


    // Print board size, number of solutions, and execution time. 
    std::cout << "Board Size: " << boardSize << std::endl; 
    std::cout << "Number of solutions: " << nsolution << std::endl; 
    std::cout << "Execution time: "  << std::fixed << std::setprecision(9) << execution_time << " seconds." <<std::endl;
    std::cout << "Number of thread: "  << std::setprecision(9) << NTHREADS << " threads." <<std::endl;
    
    std::ofstream outfile ("files/solutions.txt");
    outfile << "#Solutions "<<"for "<<boardSize<<" queens\n";
    outfile << nsolution << '\n';
    
    int n = 1;
    for(int i=0;i<solution.size();i++){
        for(int j=0;j<solution[i].size();j++){
            outfile << std::to_string(n)<<' '<<solution[i][j]<<'\n';
            n++;
        }
    }
    long pos = outfile.tellp();   
    outfile.seekp(pos - 1); 
    outfile.close();


    return 0;
}