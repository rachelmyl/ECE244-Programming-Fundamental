#include <iostream>
#include <string>
using namespace std;


// Allocates an array of N ints and fills it with the Fibonacci sequence
// The sequence is defined by F[i] = F[i-1] + F[i-2] with F[0]=F[1]=1
int* makeFibonacciArray(int N)
{
    int *fibArray = new int[N];
    fibArray[0]=1;
    fibArray[1]=1;
    for(int j=2;j<N;++j)
        fibArray[j] = fibArray[j-1]+fibArray[j-2];
    return fibArray;
}


// Allocates an array of N ints and fills it with the sequence N!
// The sequence is defined by F[i] = i*F[i-1] with F[0]=1
int* makeFactorialArray(int N)
{
    int *facArray = new int[5];

    facArray[0]=1;
    for(int i=1; i< N; ++i)
        facArray [i] = i*facArray[i-1];
    return facArray;
}


// Prints N elements of a sequence, giving a title line with the sequence name 
// and N. Then prints one element per line, with an indent of 2 spaces.
void printSequence(string name,int* sequence,int N)
{
    cout << "First " << N << " elements of sequence \"" << name << "\" are:\n";
    for(int i=0;i<N;++i)
        cout << "  " << sequence[i] << endl;
}


int main() {

    int *seq = makeFibonacciArray(10);

    printSequence("fibonacci",seq,10);

    seq = makeFactorialArray(6);

    printSequence("factorial",seq,6);

    int *fac = seq;

    int *yet_another = makeFactorialArray(5);

    delete[] fac;
    delete[] seq;
}
