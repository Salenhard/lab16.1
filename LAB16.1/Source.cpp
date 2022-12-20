#include"BinFile.h"
#include<iostream>
using namespace std;
template<class T>

void swap(int i, int j, T* a) {				// перемещение соседних элементов
	T temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}

template<class T>
void QuickSort(T* a, int first, int last) {					// сортировка быстрая
	int mid;
	int f = first, l = last;
	mid = a[(f + l) / 2]; //вычисление опорного элемента
	do
	{
		while (a[f] < mid) f++;
		while (a[l] > mid) l--;
		if (f <= l) //перестановка элементов
		{
			swap(f, l, a);
			f++;
			l--;
		}
	} while (f < l);
	if (first < l) QuickSort(a, first, l);
	if (f < last) QuickSort(a, f, last);
}


template<class T>
void MergeSort(BinFile<T>& fC, int blockSize) {
	BinFile<T> fA("fileA", INOUT);
	BinFile<T>fB("fileB", INOUT);
	int size = int(fC.Size()), n = blockSize;
	int k = 1, useA = 1, readCount = 0;
	T* A;
	fC.Reset();
	if (size <= blockSize) {
		A = new T[size];
		if (A == NULL) {
			cerr << "MergeSort: ошибка распределения памяти" << endl;
			exit(1);
		}
		fC.Read(A, size);
		QuickSort(A, 0, (int)size - 1);
		fC.Clear();
		fC.Write(A, size);
		delete[]A;
		return;
	}
	else
	{
		A = new T[blockSize];
		if (A == NULL) {
			cerr << "MergeSort: ошибка распределения памяти" << endl;
			exit(1);
		}
		while (!fC.EndFile()) {
			readCount = fC.Read(A, blockSize);
			if (readCount == 0)
				break;
			QuickSort(A, 0, readCount - 1);
			if (useA)
				fA.Write(A, readCount);
			else
				fB.Write(A, readCount);
			useA = !useA;
		}
		delete[] A;
	}
	Merge(fA, fB, fC, blockSize);
	k *= 2;
	n = k * blockSize;
	while (n < size)
	{
		Split(fA, fB, fC, k, blockSize);
		Merge(fA, fB, fC, n);
		k *= 2;
		n = k * blockSize;
	}
	fA.Delete();
	fB.Delete();
}
template<class T>
void Split(BinFile<T>& fA, BinFile<T>& fB, BinFile<T>& fC, int k, int blockSize) {
	int useA = 1;
	int i = 0;
	int readCount;

	T* A = new T[blockSize];
	if (A == NULL)
	{
		cerr << "MergeSort: ошибка распределения памяти" << endl;
		exit(1);
	}

	fA.Clear();
	fB.Clear();
	fC.Reset();

	while (!fC.EndFile())
	{
		readCount = fC.Read(A, blockSize);
		if (readCount == 0)
			break;
		if (useA)
			fA.Write(A, readCount);
		else
			fB.Write(A, readCount);
		if (++i == k)
		{
			i = 0;
			useA = !useA;
		}
	}
	delete[] A;
}
template<class T>
void Merge(BinFile<T>& fA, BinFile<T>& fB, BinFile<T>& fC, int n)
{
	int currA = 1, currB = 1;
	T dataA, dataB;
	int haveA, haveB;
	fA.Reset();
	fB.Reset();
	fC.Clear();
	fA.Read(&dataA, 1);
	fB.Read(&dataB, 1);
	for (;;)
	{
		if (dataA <= dataB)
		{
			fC.Write(&dataA, 1);
			if ((haveA = fA.Read(&dataA, 1)) == 0 || ++currA > n)
			{
				fC.Write(&dataB, 1);
				currB++;
				CopyTail(fB, fC, currB, n);
				if (!haveA)
					break;
				currA = 1;
				if ((haveB = fB.Read(&dataB, 1)) == 0) {
					fC.Write(&dataA, 1);
					currA = 2;
					break;
				}
				currB = 1;
			}
		}
		else
		{
			fC.Write(&dataB, 1);
			if ((haveB = fB.Read(&dataB, 1)) == 0 || ++currB > n)
			{
				fC.Write(&dataA, 1);
				currA++;
				CopyTail(fA, fC, currA, n);
				currB = 1;
				if ((haveA = fA.Read(&dataA, 1)) == 0)
					break;
				currA = 1;
			}
		}
	}
	if (haveA && !haveB)
		CopyTail(fA, fC, currA, n);
}

template<class T>
void CopyTail(BinFile<T>& fX, BinFile<T>& fY, int& currRunPos, int n)
{
	T data;
	while (currRunPos <= n) {
		if (fX.Read(&data, 1) == 0)
			return;
		currRunPos++;
		fY.Write(&data, 1);
	}
}
