#include <iostream>
#include <fstream>
#include "Quicksort.h"

using namespace std;

#ifdef _MSC_VER // Memory leak check
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

int main(int argc, char* argv[]) {
    VS_MEM_CHECK               // Enable memory leak check

    //open input and output files
    if(argc < 3){
        cerr << "Please provide name of input and output files.";
        return 1;
    }
    cout << "Input file: " << argv[1] << endl;
    ifstream in(argv[1]);   //reading from input file
    if(!in.is_open()){
        cerr << "Unable to open " << argv[1] << " for input.";
        return 2;
    }
    cout << "Output file: " << argv[2] << endl;
    ofstream out(argv[2]);   //writing to output file
    if(!out){
        in.close();
        cerr << "Unable to open " << argv[2] << " for output.";
        return 3;
    }

    Quicksort<int> quicksort;

    string line;
    while(getline(in, line)){
        string command;
        try{
            if(line.size() == 0){
                continue;  // if nothing in line, skip line
            }
            istringstream iss(line);
            iss >> command;
            out << command << " ";
            if(command == "QuickSort"){
                int val = 0;
                iss >> val;
                quicksort.setSize(val);
                out << val << " OK";;
            }
            else if(command == "AddToArray"){
                string addstr;
                int val;
                while(iss >> val){
                    if(!quicksort.addElement(val)){
                        throw string("Error");
                        break;
                    }
                    addstr = addstr + to_string(val) + ",";
                }
                addstr = addstr.substr(0, addstr.length() - 1);
                out << " " << addstr << " OK";
            }
            else if(command == "Capacity"){
                out << quicksort.capacity();
            }
            else if(command == "Clear"){
                quicksort.clear();
                out << "OK";
            }
            else if(command == "Size"){
                out << quicksort.size();
            }
            else if(command == "Sort"){
                int left = 0;
                int right = 0;
                iss >> left >> right;
                quicksort.resetCount();
                if(quicksort.sort(left, right)){
                    out << left << "," << right << " OK";
                }
                else{
                    throw string("Error");
                }
            }
            else if(command == "SortAll"){
                quicksort.resetCount();
                if(quicksort.sortAll()){
                    out << "OK";
                }
                else{
                    throw string("Error");
                }
            }
            else if(command == "MedianOfThree"){
                int left = 0;
                int right = 0;
                iss >> left >> right;
                out << left << "," << right << " =";
                if(!quicksort.conditions(left, right)){
                    out << "-1";
                }
                else{
                    out << quicksort.medianOfThree(left, right);
                }
            }
            else if(command == "Partition"){
                int left = 0;
                int right = 0;
                int pivot = 0;
                iss >> left >> right >> pivot;
                out << left << "," << right << "," << pivot << " = ";
                if(!quicksort.conditions(left, right, pivot)){
                    out << "-1";
                }
                else{
                    out << quicksort.partition(left, right, pivot);
                }
            }
            else if(command == "PrintArray"){
                if(quicksort.size() == 0){
                    out << "Empty";
                }
                else{
                    out << quicksort;
                }
            }
            else if(command == "Stats"){
                out << quicksort.stats();
            }
            else{
                throw string("Error"); //if the command is unknown throw an error
            }
            out << endl;
        }
        catch(string& error){
            out << error << endl;
        }
    }
    return 0;
}
