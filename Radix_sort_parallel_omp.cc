/**
 *@file Radix_sort_parallel_omp.cc
 *@author: Juan Jose Ropero Cerro 
 *@brief: Codigo de implementacion paralela del algoritmo de ordenacion Radix de vectores con OMP
 *
 *Este codigo ha sido creado a partir del codigo secuencial incluido en este repositorio y tomado de la pagina https://www.geeksforgeeks.org/radix-sort/
**/
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
/**
 *@brief Funcion que lleva a cabo la ordenacion del vectr arr con el algoritmo Radix
 *
 **/
void radix_sort(vector<int>& arr)
{
    omp_set_num_threads(Nthreads);
    
    //Tamaño del vector
    int sz = arr.size();
    int id,j,pos;//Variable usadas en la seccion paralela privadas a cad hilo
    
    //Conseguimos el elemento mayor del vector
    int max_val = *max_element(arr.begin(), arr.end());
 
    // Conseguimos el maximo numero de digitos del mayor elemento del vector
    int d = digits(max_val);
 
    node** bins;
 
    
    bins = new node*[10*Nthreads];

    for (int i = 0; i < 10*Nthreads; i++)
        bins[i] = NULL;

 
    for (int i = 0; i < d; i++) 
    { 
        //Seccion paralela del algoritmo radix
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
        //Rescritura del vector leyendo las cubetas de los hilos
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
