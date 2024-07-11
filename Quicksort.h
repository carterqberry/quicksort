#ifndef QUICKSORT_QUICKSORT_H
#define QUICKSORT_QUICKSORT_H
#include <sstream>
#include "QSInterface.h"

template<typename T>
class Quicksort : public QSInterface<T>{
public:
    Quicksort(){  //constructor
        qs_capacity = 0;
        qs_items = 0;
        qs_comparisons = 0;
        qs_exchanges = 0;
        qs_array = new T[0];
    }
    ~Quicksort(){  //deconstructor
        delete[] qs_array;
    }
    //QSInterface functions:
    //functions with more than 5 lines are found underneath class.
    virtual bool addElement(T element);  //Add an element to the QuickSort array. Dynamically grow array as needed.
    virtual bool sort(size_t left, size_t right);  //Sort the elements of a QuickSort subarray using median and partition functions.
    virtual bool sortAll(){  //Sort all elements of the QuickSort array using median and partition functions.
        sort(0, qs_items);
        return true;
    }
    virtual bool clear(){  //Removes all items from the QuickSort array.
        qs_items = 0;
        return true;
    }
    virtual size_t capacity() const {  //Return size of the QuickSort array.
        return qs_capacity;
    }
    virtual size_t size() const {  //Return number of elements in the QuickSort array.
        return qs_items;
    }
    virtual int medianOfThree(size_t left, size_t right);  //Calculates the median index, sorts first, middle, and last numbers in the array
    virtual int partition(size_t left, size_t right, size_t pivotIndex);  //Partitions a subarray around a given pivot value
    virtual std:: string stats() const {
        std::stringstream outs;
        outs << qs_comparisons << "," << qs_exchanges;
        return outs.str();
    }
    virtual std::string toString() const;  //returns comma delimited string representation of the array.
    friend std::ostream& operator<<(std::ostream& os, Quicksort& qs){
        return os << qs.toString();
    }
    //Other functions:
    void setSize(int array_size);
    bool conditions(int left, int right);
    bool conditions(int left, int right, int pivot);
    void resetCount(){
        qs_comparisons = qs_exchanges = 0;
    }

private:
    int qs_capacity;
    int qs_items;
    int qs_comparisons;
    int qs_exchanges;
    T* qs_array;
    void reallocate();
};

//private functions definitions:

template<typename T>
bool Quicksort<T>::addElement(T element) {
    int old_items = qs_items;
    if(qs_items == qs_capacity && qs_capacity != 0){
        reallocate();
    }
    qs_array[qs_items] = element;  //inserts at the next open index
    qs_items++;
    if(qs_items == old_items + 1){  //return true/false if items incremented
        return true;
    }
    else{
        return false;
    }
}

template<typename T>
bool Quicksort<T>::sort(size_t left, size_t right) {
    if(!conditions(static_cast<int>(left), static_cast<int>(right))){
        return false;
    }
    if(left - right > 1){  //if there are contents to be sorted
        size_t pivot = partition(left, right, medianOfThree(left, right));  //gets the pivot to sort on
        sort(left, pivot);  //sorts the left subarray
        sort(pivot + 1, right);  //sorts the right subarray
    }
    return true;
}

template<typename T>
int Quicksort<T>::medianOfThree(size_t left, size_t right) {
    size_t middle = (left + right) / 2;  //middle index
    if(qs_array[middle] < qs_array[left]){
        std::swap(qs_array[left], qs_array[middle]);  //swaps if the middle is greater than the right
    }
    if(qs_array[right - 1] < qs_array[middle]){
        std::swap(qs_array[middle], qs_array[right - 1]);  //swaps if the middle is greater than right
        if(qs_array[middle] < qs_array[left]){
            std::swap(qs_array[middle], qs_array[left]);  //swaps if middle is less than right
        }
    }
    return static_cast<int>(middle);  //returns the middle index
}

template<typename T>
int Quicksort<T>::partition(size_t left, size_t right, size_t pivotIndex) {
    size_t up = left + 1;
    size_t down = right - 1;
    std::swap(qs_array[left], qs_array[pivotIndex]);  //swaps the left and pivot values
    pivotIndex = left;
    do{
        while(qs_array[up] <= qs_array[pivotIndex] && up < right - 1){
            up++;  //increments up when up is less than pivot
        }
        while(qs_array[down] > qs_array[pivotIndex] && down > left){
            down--;  //decrements down when down is less than pivot
        }
        if(up < down && qs_array[up] > qs_array[down]){
            std::swap(qs_array[up], qs_array[down]);  //swaps up and down so the values are to the right of the pivot
        }
    } while(up < down);
    std::swap(qs_array[left], qs_array[down]);
    pivotIndex = down;
    return static_cast<int>(pivotIndex);
}

template<typename T>
std::string Quicksort<T>::toString() const {
    std::stringstream out;
    for(int i = 0; i < qs_items; i++){
        out << qs_array[i];
        if(i < qs_items - 1){
            out << ",";  //add a comma when not the last element
        }
    }
    return out.str();
}

template<typename T>
void Quicksort<T>::setSize(int array_size){
    delete[] qs_array;
    qs_array = new T[array_size];
    for(int i = 0; i < array_size; i++){
        qs_array[i] = 0;  //initializes array to 0
    }
    qs_capacity = array_size;
    qs_items = 0;
}

template<typename T>
bool Quicksort<T>::conditions(int left, int right) {
    //if given inputs are in the bounds, return true/false:
    if(right <= left || qs_items == 0 || right > qs_items || left > qs_items || right < 0 || left < 0){
        return false;
    }
    else{
        return true;
    }
}

template<typename T>
bool Quicksort<T>::conditions(int left, int right, int pivot) {
    //if given inputs are in the bounds, return true/false:
    if(right <= left || right <= pivot || qs_items == 0 || right > qs_items || left > qs_items || pivot > qs_items || right < 0 || left < 0 || pivot < 0){
        return false;
    }
    else{
        return true;
    }
}


//public function definition:
template<typename T>
void Quicksort<T>::reallocate() {
    T* new_array = new T[qs_capacity * 2];
    for(int i = 0; i < qs_capacity * 2; i++){
        new_array[i] = 0;  //initialized the array to 0
    }
    for(int i = 0; i < qs_items; i++){
        new_array[i] = qs_array[i];
    }
    std::swap(qs_array, new_array);
    qs_capacity = qs_capacity * 2;
    delete[] new_array;
}

#endif //QUICKSORT_QUICKSORT_H
