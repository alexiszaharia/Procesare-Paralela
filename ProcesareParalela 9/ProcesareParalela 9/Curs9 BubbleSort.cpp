#include<iostream>
#include<omp.h>

using namespace std;

#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#define MIN(A, B) (((A) > (B)) ? (B) : (A))
#define UP 0
#define DOWN 1

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

void odd_even_sort(int* v, int N) {
	for (int it = 0; it < N; it++) {
		if (it % 2 == 1) {
			for (int i = 2; i < N; i += 2) {
				if (v[i - 1] > v[i]) {
					swap(&v[i - 1], &v[i]);
				}
			}
		}
		else {
			for (int i = 1; i < N; i += 2) {
				if (v[i - 1] > v[i]) {
					swap(&v[i - 1], &v[i]);
				}
			}
		}
	}
}

void odd_even_sort_parallel(int* v, int N) {
//#pragma omp parallel for
	for (int it = 0; it < N; it++) {
		if (it % 2 == 1) {
			#pragma omp parallel for
			for (int i = 2; i < N; i += 2) {
				if (v[i - 1] > v[i]) {
					swap(&v[i - 1], &v[i]);
				}
			}
		}
		else {
			#pragma omp parallel for
			for (int i = 1; i < N; i += 2) {
				if (v[i - 1] > v[i]) {
					swap(&v[i - 1], &v[i]);
				}
			}
		}
	}
}

void merge(int* a, int i1, int j1, int i2, int j2)
{
	int* temp = new int[(j1 + 1 - i1) * 2 + 1];    //array used for merging
	int i, j, k;
	i = i1;    //beginning of the first list
	j = i2;    //beginning of the second list
	k = 0;

	while (i <= j1 && j <= j2)    //while elements in both lists
	{
		if (a[i] < a[j])
			temp[k++] = a[i++];
		else
			temp[k++] = a[j++];
	}

	while (i <= j1)    //copy remaining elements of the first list
		temp[k++] = a[i++];

	while (j <= j2)    //copy remaining elements of the second list
		temp[k++] = a[j++];

	//Transfer elements from temp[] back to a[]
	for (i = i1, j = 0; i <= j2; i++, j++)
		a[i] = temp[j];

	delete[] temp;
}

void mergesort(int* a, int i, int j)
{
	int mid;

	if (i < j)
	{
		mid = (i + j) / 2;
		mergesort(a, i, mid);        //left recursion
		mergesort(a, mid + 1, j);    //right recursion
		merge(a, i, mid, mid + 1, j);    //merging of two sorted sub-arrays
	}
}

void mergesort_parallel(int* a, int i, int j)
{
	int mid;

#pragma omp parallel num_threads(2)
	if (i < j)
	{
#pragma omp sections
		{
			mid = (i + j) / 2;
#pragma omp section
			mergesort(a, i, mid);        //left recursion
#pragma omp section
			mergesort(a, mid + 1, j);    //right recursion
		}
#pragma omp barrier
		merge(a, i, mid, mid + 1, j);    //merging of two sorted sub-arrays
	}
}

//--------------------BITONIC SORT---------------------------

void bitonic_sort_seq(int start, int length, int *seq, int flag)
{
	int i;
	int split_length;

	if (length == 1)
		return;

	if (length % 2 != 0)
	{
		printf("error\n");
		exit(0);
	}

	split_length = length / 2;

	// bitonic split
	for (i = start; i < start + split_length; i++)
	{
		if (flag == UP)
		{
			if (seq[i] > seq[i + split_length])
				swap(&seq[i], &seq[i + split_length]);
		}
		else
		{
			if (seq[i] < seq[i + split_length])
				swap(&seq[i], &seq[i + split_length]);
		}
	}

	bitonic_sort_seq(start, split_length, seq, flag);
	bitonic_sort_seq(start + split_length, split_length, seq, flag);
}

void printVector(int* v, int N) {
	cout << endl << "Vector:";
	for (int i = 0; i < N; i++)
		cout << " " << v[i];
}

void main() {
	const long N = 1e5;
	int valori[N];

	//generare voctor initial
	for (int i = 0; i < N; i++)
		valori[i] = N - i;

	//test odd even sort - secvential
	cout << endl << "-------------------ODD EVEN SORT-------------";
	double tStart = omp_get_wtime();

	odd_even_sort_parallel(valori, N);

	double tFinal = omp_get_wtime();

	cout << endl << "Durata de " << tFinal - tStart << " secunde";

	printVector(valori, 20);

	//test sortare cu succes
	int suma = 0;
	for (int i = 0; i < N; i++) {
		suma += (valori[i] - valori[i - 1]);
	}

	cout << endl << "Suma diferente: " << suma;

	//test sortare merge
	//generare voctor initial
	for (int i = 0; i < N; i++)
		valori[i] = N - i;

	//test merge
	cout << endl << "-------------------MERGE SORT-------------";
	tStart = omp_get_wtime();

	mergesort_parallel(valori, 0, N - 1);

	tFinal = omp_get_wtime();

	cout << endl << "Durata de " << tFinal - tStart << " secunde";

	printVector(valori, 20);

	//test sortare cu succes
	suma = 0;
	for (int i = 0; i < N; i++) {
		suma += (valori[i] - valori[i - 1]);
	}

	cout << endl << "Suma diferente: " << suma;

	//generare voctor initial
	for (int i = 0; i < N; i++)
		valori[i] = N - i;

	//test bitonic sort
	cout << endl << "-------------------BITONIC SORT-------------";
	tStart = omp_get_wtime();

	bitonic_sort_seq(0, N, valori, UP);

	tFinal = omp_get_wtime();

	cout << endl << "Durata de " << tFinal - tStart << " secunde";

	printVector(valori, 20);

	//test sortare cu succes
	suma = 0;
	for (int i = 0; i < N; i++) {
		suma += (valori[i] - valori[i - 1]);
	}

	cout << endl << "Suma diferente: " << suma;

}