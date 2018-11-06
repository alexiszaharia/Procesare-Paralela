#include <iostream>
#include <omp.h>

using namespace std;

const long long N = 1e5;    //10^5

long long getNumarNumerePrime(long long N) {
	long long contorPrime = 0;

	for (long long i = 2; i < N; i++) {
		bool estePrim = true;
		for (long long j = 2; j <= i / 2; j++) {
			if (i % j == 0) {
				estePrim = false;
				break;
			}
		}
		if (estePrim) {
			contorPrime += 1;
			//cout << endl << i;
		}
	}

	return contorPrime;
}

long long getNumarNumerePrimeParalel(long long N) {
	long long contorPrime = 0;
	int contorLocal = 0;

#pragma omp parallel for reduction(+:contorPrime) num_threads(4)
	{
		for (long long i = 2; i < N; i++) {
			bool estePrim = true;
			for (long long j = 2; j <= i / 2; j++) {
				if (i % j == 0) {
					estePrim = false;
					break;
				}
			}
			if (estePrim) {
				contorPrime += 1;
				//cout << endl << i;
			}
		}
	}

	return contorPrime;
}

int* sumaVectoriSecventiala(int vector1[], int vector2[], int N) {
	int* rezultat = new int[N];
	for (int i = 0; i < N; i++) {
		rezultat[i] = vector1[i] + vector2[i];
	}

	return rezultat;
}

int* sumaVectoriParalela(int vector1[], int vector2[], int N) {
	int* rezultat = new int[N];

#pragma omp parallel for num_threads(4)
	for (int i = 0; i < N; i++) {
		rezultat[i] = vector1[i] + vector2[i];
	}

	return rezultat;
}

void testPerformanta(long long(*pf)(long long), long long n) {
	double tStart = omp_get_wtime();

	long long rezultat = pf(n);

	double tfinal = omp_get_wtime();

	cout << endl << "Numar prime: " << rezultat << " determinat in " << tfinal - tStart << " secunde.";
}

void main() {
	//test solutie secventiala

	//testPerformanta(getNumarNumerePrime, N);


	//test solutie paralela
	//testPerformanta(getNumarNumerePrimeParalel, N);

	//test performanta suma elemente vector
	int vector1[N];
	int vector2[N];
	
	double tStart = omp_get_wtime();

	int* vectorRezultat = sumaVectoriParalela(vector1, vector2, N);

	double tfinal = omp_get_wtime();

	cout << endl << "Suma determinata in " << tfinal - tStart << " secunde.";
}