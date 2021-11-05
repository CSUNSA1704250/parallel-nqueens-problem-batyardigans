#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include<sstream>
using namespace std;

void generate_dot(ofstream &file, vector<int> &vec){
    file.open("files/graph.dot");
    file << "digraph D {\n\n";
    file << "node [shape=plaintext]\n\n";
    file << "some_node [\n";
    file << "label=<\n";
    file << "<table border=";
    file << char(34) << "0" << char(34);
    file << " cellborder=";
    file << char(34) << "1" << char(34);
    file << " cellspacing=";
    file << char(34) << "0" << char(34);
    file << " >\n";

    for(int i = vec.size()-1; i >= 0; i--){
        file << "<tr> ";
        for(int j = 0; j < vec.size(); j++){
            file << " <td>";
            if(j == vec[i]){
                file << " &#9813; ";
            }
            file << " </td>";
        }
        file << " </tr>\n";
    }

    file << "</table>>\n";
    file << "];\n\n";
    file << "}\n";

    file.close();
}

int main(){
    ifstream archivo("files/solutions.txt");
    string linea;
    for(int i=0; i<3; i++){
        getline(archivo, linea);
    }

    vector<int> vec_num;
    stringstream ss(linea);

    int value;
    while (ss >> value){
        vec_num.push_back(value);

    }
    
    
    ofstream file;
    generate_dot(file, vec_num);
    return 0;
}