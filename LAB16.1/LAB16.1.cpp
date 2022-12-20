#include <iostream>
#include <iomanip>
#include "BinFile.h"
#include "Source.cpp"
using namespace std;
void PrintFile(BinFile<int>& f, long n)
{
	int data;
	long i;
	n = (f.Size() < n) ? f.Size() : n;
	f.Reset();
	for (i = 0; i < n; i++)
	{
		if (i % 9 == 0)
			cout << endl;
		f.Read(&data, 1);
		cout << setw(5) << data << "  ";
	}
	cout << endl;
}

void LoadFile(BinFile<int>& f, int n)
{
	int i, item;
	f.Reset();
	for (i = 0; i < n; i++)
	{
		item = rand()%1000;
		f.Write(&item, i);
	}
}

int main()
{
	BinFile<int> fC("fileC", INOUT);
	LoadFile(fC, 1000);
	cout << "Первые 45 элементов исходного файла:" << endl;
	PrintFile(fC, 45);
	cout << endl;
	MergeSort(fC, 100);
	cout << "Первые 45 элементов отсортированного файла:" << endl;
	PrintFile(fC, 45);
	fC.Delete();
}

