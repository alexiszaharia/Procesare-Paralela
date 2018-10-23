#include <iostream>
#include <thread>
#include <chrono>
#include <set>
#include <mutex>

using namespace std;

static const int MAX_LIMIT = 1000000; //10^6
static set<int> prime;
static int nrPrime = 0;

//varianta secventiala - genereaza toate numerele prime intre 1 si limita
void generarePrime(set<int> &prime, int limitaInferioara, int limitaSuperioara) {
	prime.insert(1);
	for (int i = limitaInferioara; i < limitaSuperioara; i++) {
		bool estePrim = true;
		for (int j = 2; j <= i / 2; j++) {
			if (i % j == 0) {
				estePrim = false;
				break;
			}
		}
		if (estePrim)
			prime.insert(i);
	}
}

static int NR_CURENT = 0;
mutex mutexGlobal;

void verificaPrim(set<int> &prime, int& nrCurent, int limitaSuperioara) {
	while (nrCurent < limitaSuperioara)
	{
		mutexGlobal.lock();
		nrCurent++;
		int valoare = nrCurent;
		mutexGlobal.unlock();
		bool estePrim = true;
		for (int i = 2; i < limitaSuperioara / 2; i++) {
			if (valoare % i == 0) {
				estePrim = false;
				break;
			}
		}
		if (estePrim) {
			prime.insert(valoare);
		}
	}
}

void main() {
	std::chrono::system_clock::time_point tStart = std::chrono::system_clock::now();

	//generarePrime(prime, 2, MAX_LIMIT / 2); //15 secunde
	//generarePrime(prime, MAX_LIMIT / 2, MAX_LIMIT); //43 secunde

	//fara load balancing - 39 secunde
	//thread t1(generarePrime, std::ref(prime), 2, MAX_LIMIT / 2);
	//thread t2(generarePrime, std::ref(prime), MAX_LIMIT / 2, MAX_LIMIT);

	//t1.join();
	//t2.join();

	thread t1(generarePrime, std::ref(prime), 2, MAX_LIMIT - (MAX_LIMIT * 0.8));
	thread t2(generarePrime, std::ref(prime), MAX_LIMIT - (MAX_LIMIT * 0.8), MAX_LIMIT);

	t1.join();
	t2.join();

	//cu load balancing
	/*thread t1(verificaPrim, std::ref(prime), std::ref(NR_CURENT), MAX_LIMIT);
	thread t2(verificaPrim, std::ref(prime), std::ref(NR_CURENT), MAX_LIMIT);

	t1.join();
	t2.join();*/

	std::chrono::system_clock::time_point tFinal = std::chrono::system_clock::now();

	cout << endl << "Numere prime : " << prime.size();
	cout << endl << "Timp procesare (ms) : " << chrono::duration_cast<chrono::milliseconds>(tFinal - tStart).count();
}