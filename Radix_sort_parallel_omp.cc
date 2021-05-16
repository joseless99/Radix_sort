// implementation of radix sort using bin/bucket sort
#include<cstdio>
#include <iostream>
#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
#define MAX 300000
#define Nthreads 4
// structure for a single linked list to help further in the
// sorting

int check_array_order(vector< int> arr) {
    for( int i = 1; i < arr.size(); ++i)
        if (arr[i-1] > arr[i])
            return i;
    return 0;
}

struct node {
    int data;
    node* next;
};
 
// function for creating a new node in the linked list
struct node* create(int x)
{
    node* temp = new node();
    temp->data = x;
    temp->next = NULL;
 
    return temp;
}
 
// utility function to append node in the linked list
// here head is passed by reference, to know more about this
// search pass by reference
void insert(node*& head, int n)
{
    if (head == NULL) {
        head = create(n);
        return;
    }
 
    node* t = head;
    while (t->next != NULL)
        t = t->next;
    t->next = create(n);
}
 
// utility function to pop an element from front in the list
// for the sake of stability in sorting
int del(node*& head)
{
    if (head == NULL)
        return 0;
    node* temp = head;
    // storing the value of head before updating
    int val = head->data;
 
    // updation of head to next node
    head = head->next;
 
    delete temp;
    return val;
}
 
// utility function to get the number of digits in the
// max_element
int digits(int n)
{
    int i = 1;
    if (n < 10)
        return 1;
 
    while (n >=(int)pow(10, i))
        i++;
    return i;
}
 
void radix_sort(vector<int>& arr)
{
    omp_set_num_threads(Nthreads);
    // size of the array to be sorted
    int sz = arr.size();
    int id,j,pos;
    // getting the maximum element in the array
    int max_val = *max_element(arr.begin(), arr.end());
 
    // getting digits in the maximum element
    int d = digits(max_val);
 
    // creating buckets to store the pointers
    node** bins;
 
    // array of pointers to linked list of size 10 as
    // integers are decimal numbers so they can hold numbers
    // from 0-9 only, that's why size of 10
 
    bins = new node*[10*Nthreads];

    // intializing the hash array with null to all
    for (int i = 0; i < 10*Nthreads; i++)
        bins[i] = NULL;

 
    // first loop working for a constan time only and inner
    // loop is iterating through the array to store elements
    // of array in the linked list by their digits value
    for (int i = 0; i < d; i++) 
    {   
        #pragma omp parallel shared (arr,i)private (id,j,pos)
        {   
            id=omp_get_thread_num();
            for (j = ((MAX/Nthreads)*id); j < ((MAX/Nthreads)*(id+1)); j++)
            { // bins updation
                pos=((arr[j] / (int)pow(10, i)) % 10 )+(10*id);
                insert( bins [pos],arr[j]);
            }
        }  


        int x = 0, y = 0,z=0;
        // write back to the array after each pass
        while (x < 10) {
            for(z=0;z<Nthreads;z++){
                while (bins[x+(10*z)] != NULL){
                    arr[y++] = del(bins[x+(10*z)]);
                }
            }
            x++;
        }
    }
}
 
// a utility function to print the sorted array
void print(vector<int> arr)
{
    for (int i = 0; i < arr.size(); i++)
        cout << arr[i] << " ";
    cout << endl;
}
 
int main()
{
    vector<int> arr;
    clock_t tini,tfin;

    int val;
    srand(time(NULL));
    
    for(int p=0;p<MAX;p++)
    {
        arr.push_back(rand()%101);
    }
    
    tini=omp_get_wtime(); 
   // print(arr);
    radix_sort(arr);
   // print(arr);
    tfin=omp_get_wtime();
   /** if(check_array_order(arr)!=0)
    {
        printf("Error");
    }**/
    std::cout<<tfin-tini<<endl;
    return 0;
}