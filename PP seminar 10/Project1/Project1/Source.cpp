#include <iostream>
#include<fstream>
#include<string>
#include<omp.h>

using namespace std;

int countWordsStream(string fisier) {
	fstream file(fisier, ios::in);
	//fstream newFile("newwordlist.txt", ios::out | ios::app);
	if (file.is_open()) {
		int contor = 0;
		char buffer[100];
		while (!file.getline(buffer, 100).eof()) {
			//string line(buffer);
			//newFile << line << endl;
			contor += 1;
		
		}

		file.close();
		//newFile.close();
		return contor;

	}

}

int countWordsStream_2(string fisier) {
	fstream file(fisier, ios::in);	
	if (file.is_open()) {
		int contor = 0;
		string line;
		while (!(file >> line).eof()) {
		contor += 1;
        }


		file.close();
		return contor;
	}
}

int countWordsParallel(string fisier) {
	fstream file(fisier, ios::in | ios::binary);
	if (file.is_open()) {
		file.seekg(0, file.end);
		int fileLength = file.tellg();
		file.seekg(0, file.beg);

		char* content = new char[fileLength];
		file.seekp(ios::beg);

		file.read(content, fileLength);
		int contor = 0;

#pragma omp parallel for reduction(+: contor)

		for(int i = 0; i < fileLength; i++)
			if (content[i] == '\n')
				contor += 1;


		file.close();
		return contor;
	}
}

int* countLetterFrequencyParallel(string fisier) {
	fstream file(fisier, ios::in | ios::binary);
	if (file.is_open()) {
		file.seekg(0, file.end);
		int fileLength = file.tellg();
		file.seekg(0, file.beg);

		char* content = new char[fileLength];
		file.seekp(ios::beg);

		file.read(content, fileLength);
		int* frecvente = new int[26];
		for (int i = 0; i < 26; i++)
			frecvente[i] = 0;

#pragma omp parallel for shared(frecvente)

		for (int i = 0; i < fileLength; i++)
			if (content[i] != '\n')
#pragma omp critical
				frecvente[content[i] - 'a'] += 1;


		file.close();
		return frecvente;
	}
}


// cautare secventiala 

int searchSeq(int* array, int N, int key) {
	for (int i = 0; i < N; i++)
		if (array[i] == key)
			return i;
	return -1;


}

int searchParallel(int* array, int N, int key) {
	int index = -1;

#pragma omp parallel for num_threads(4)
	for (int i = 0; i < N; i++) {
		if (index != -1)
			break;
		if (array[i] == key) {
			index = i;
			break;
		}
	}
	return index;


}


int binarySearch(int* arr, int l, int r, int x)
{
	if (r >= l)
	{
		int mid = l + (r - l) / 2;

		if (arr[mid] == x)
			return mid;

		if (arr[mid] > x)
			return binarySearch(arr, l, mid - 1, x);

		return binarySearch(arr, mid + 1, r, x);
	}

	return -1;
}

void main() {
	double tStart = omp_get_wtime();
	cout << endl << "Numar cuvinte: " << countWordsStream("newwordlist.txt");
	double tFinal = omp_get_wtime();
	cout << endl << "in " << tFinal - tStart << "secunde";


	//tStart = omp_get_wtime();
	//cout << endl << "Numar cuvinte: " << countWordsStream_2("newwordlist.txt");
	//tFinal = omp_get_wtime();
	//cout << endl << "in " << tFinal - tStart << "secunde";


	tStart = omp_get_wtime();
	cout << endl << "Numar cuvinte: " << countWordsParallel("newwordlist.txt");
	tFinal = omp_get_wtime();
	cout << endl << "in " << tFinal - tStart << "secunde";

	tStart = omp_get_wtime();
	int* frecvente = countLetterFrequencyParallel("newwordlist.txt");
	tFinal = omp_get_wtime();
	cout << endl << "Frecvente generate in" << tFinal - tStart << "secunde";
	for (int i = 0; i < 26; i++)
		cout << endl << (char)('a' + i )<< "=" << frecvente[i];




	// test performanta cautare

	const int N = 1e5;
	//int array[N];
	int* array = new int[N];
	for (int i = 0; i < N; i++)
		array[i] = i + 1;

	tStart = omp_get_wtime();
	int index=searchSeq(array, N, N);
	tFinal = omp_get_wtime();
	if (index != -1)
		cout << endl << "Valoarea gasita";
	cout << endl << "durata cautare: " << tFinal - tStart;


	tStart = omp_get_wtime();
	index = searchParallel(array, N, N);
	tFinal = omp_get_wtime();
	if (index != -1)
		cout << endl << "Valoarea gasita";
	cout << endl << "durata cautare: " << tFinal - tStart;



	tStart = omp_get_wtime();
	index = binarySearch(array,0,N,N);
	tFinal = omp_get_wtime();
	if (index != -1)
		cout << endl << "Valoarea gasita";
	cout << endl << "durata cautare: " << tFinal - tStart;
}