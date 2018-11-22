#include<iostream>
#include<omp.h>

using namespace std;

//functie ce determina secventa Fibonacci secvential
void fibonacciSecvential(long long N) {
	if (N < 2) {
		cout << endl << "valoare N este prea mica";
	}
	long long t1 = 0, t2 = 1, next = 0;

	next = t1 + t2;

	while (next <= N)
	{
		t1 = t2;
		t2 = next;
		next = t1 + t2;
		cout << endl << next;
	}
}

void fibonacciParalel(long long N) {
	long long sir[89];
	sir[0] = 0;
	sir[1] = 1;
	
#pragma omp parallel
	cout << endl << sir[0] << endl << sir[1];

	int i;

#pragma omp parallel num_threads(4) shared(sir) private(i)
#pragma omp for schedule(dynamic, 4)
	{
		for (i = 2; i < 89; i++) {
			sir[i] = sir[i - 1] + sir[i - 2];
			
#pragma omp single
			cout << endl << sir[i];
		}
	}
}

void fibonacciParalel2(long long N) {
	if (N < 2) {
		cout << endl << "valoare N este prea mica";
	}
	long long t1 = 0, t2 = 1, next = 0;

	next = t1 + t2;

#pragma omp sections num_threads(4) private(t1, t2, next)
	{
		while (next <= N)
		{
			#pragma omp section
			{
				t1 = t2;
				t2 = next;
				next = t1 + t2;
			}

			#pragma omp section
			{
				cout << endl << next;
			}
		}
	}
}

void fibonacciParalel3(long long N) {
	if (N < 2) {
		cout << endl << "valoare N este prea mica";
	}
	long long t1 = 0, t2 = 1, next = 0;

#pragma omp parallel for schedule(dynamic) firstprivate(t1)
	for (next = t1 + t2; next <= N; next = t2 + next)
	{
		cout << endl << next;
	}
}

void main() {
	long long N = 1000000000000000000;

	//varianta secventiala
	cout << "-------------- VARIANTA SECVENTIALA ------------------";

	double tStart = omp_get_wtime();

	fibonacciSecvential(N);

	double tStop = omp_get_wtime();

	cout << endl << "Durata varianta secventiala: " << tStop - tStart << " secunde";

	//varianta paralela
	cout << endl << "----------------- VARIANTA PARALELA -------------------";

	tStart = omp_get_wtime();

	fibonacciParalel(N);

	tStop = omp_get_wtime();

	cout << endl << "Durata varianta paralela: " << tStop - tStart << " secunde";

	//varianta paralela 2
	cout << endl << "----------------- VARIANTA PARALELA 2 -------------------";

	tStart = omp_get_wtime();

	fibonacciParalel2(N);

	tStop = omp_get_wtime();

	cout << endl << "Durata varianta paralela2: " << tStop - tStart << " secunde";

	//varianta paralela 3
	cout << endl << "----------------- VARIANTA PARALELA 3 -------------------";

	tStart = omp_get_wtime();

	fibonacciParalel3(N);

	tStop = omp_get_wtime();

	cout << endl << "Durata varianta paralela3: " << tStop - tStart << " secunde";
}