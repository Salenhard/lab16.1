#pragma once
#include <fstream>
#include <iostream>
using namespace std;
enum Access
{
	IN, OUT, INOUT
};

enum SeekType
{
	BEG, CUR, END
};
template<class T>
class BinFile
{
private:
	fstream f;
	Access accessType;
	string fname;
	int fileOpen;

	int Tsize;
	int filesize;
	void Error(const char* msg);
	
public:
	BinFile(const string& fileName, Access atype = OUT);
	~BinFile();
	BinFile(BinFile<T>& bf);
	void Delete();
	void Close();
	int EndFile();
	long Size();
	void Reset();
	void Seek(long pos, SeekType mode);
	int Read(T *A, int n);
	void Write(T* A, int n);
	T Peek();
	void Write(const T& data, long pos);
	T Read(long pos);
	void Append(T item);
	void Clear();
};

template<class T>
BinFile<T>::BinFile(const string& fileName, Access atype)
{	
	ofstream s;
	s.open(fileName);
	s.close();
	if (atype == IN)
		f.open(fileName, ios::in | ios::trunc | ios::binary);
	else if (atype == OUT)
		f.open(fileName, ios::out | ios::trunc | ios::binary);
	else
		f.open(fileName, ios::in | ios::out | ios::binary);
	if (!f)
		Error("Конструктор BinFile: не могу открыть файл");
	else fileOpen = 1;
	accessType = atype;
	Tsize = sizeof(T);
	if (accessType == IN || accessType == INOUT) {
		f.seekg(0, ios::end);
		filesize = f.tellg() / Tsize;
		f.seekg(0, ios::beg);
	}
	else
		filesize = 0;
	fname = fileName;
}

template<class T>
T BinFile<T>::Read(long pos) {
	T data;
	if (!fileOpen)
		Error("BinFile Read(long pos): Файл закрыт");
	if (accessType == OUT)
		Error("BinFile Read(long pos): Недопустимая операция доступа к файлу");
	else if(pos < 0 || pos >= filesize)
		Error("BinFile Read(long pos): Недопустимая операция доступа к файлу");
	f.seekg(pos * Tsize, ios::beg);
	f.read((char*)&data, Tsize);
	if (f.tellg() / Tsize >= filesize)
		f.clear(ios::eofbit);
	return data;
}

template<class T>
void BinFile<T>::Write(T* A, int n) {
	long previousRecords;
	if(accessType == IN)
		Error("BinFile Write(T* A, int n): Недопустимая операция доступа к файлу");
	if(!fileOpen)
		Error("BinFile Write(T* A, int n): Файл закрыт");
	previousRecords = f.tellg() / Tsize;
	if (previousRecords + n > filesize)
		filesize += previousRecords + n - filesize;

	f.write((char*)A, Tsize * n);
}


template<class T>
void BinFile<T>::Clear() {
	if(accessType == IN)
		Error("BinFile Clear(): Недопустимая операция доступа к файлу");
	f.close();
	Delete();
	ofstream c;
	c.open(fname);
	c.close();
	if (accessType == OUT)
		f.open(fname, ios::out | ios::trunc | ios::binary);
	else 
		f.open(fname, ios::in | ios::out | ios::binary);
	if (!f)
		Error("BinFile Clear(): не могу повторно открыть файл");
	filesize = 0;
}

template<class T>
int BinFile<T>::EndFile() {
	return f.eof();
}

template<class T>
void BinFile<T>::Close()
{
	f.close();
	fileOpen = 0;
}

template<class T>
BinFile<T>::BinFile(BinFile<T>& bf)
{
	bf.Close();
	accessType = bf.accessType;
	fname = bf.fname;
	Tsize = bf.Tsize;
	filesize = bf.filesize;
	if (accessType == IN)
		f.open(fname, ios::in | ios::trunc | ios::binary);
	else if (accessType == OUT)
		f.open(fname, ios::out | ios::trunc | ios::binary);
	else
		f.open(fname, ios::in | ios::out | ios::binary);
	if (!f)
		Error("Конструктор BinFile: не могу открыть файл");
	else fileOpen = 1;
}

template<class T>
long BinFile<T>::Size() {
	return filesize;
}

template <class T>
void BinFile<T>::Error(const char* msg) {
	cerr << msg;
	exit(1);
}

template <class T>
T BinFile<T>::Peek() {
	T data;
	f.read((char*)&data, Tsize);
	return data;
}

template <class T>
void BinFile<T>::Reset() {
	f.seekg(0, ios::beg);
}

template <class T>
void BinFile<T>::Seek(long pos, SeekType mode) {
	
		switch (mode)
		{
		case(BEG): {
				f.seekg(pos, ios::beg);
				break;
			}

		case(CUR): {
				f.seekg(pos, ios::cur);
				break;
			}
		case(END): {
				f.seekg(pos, ios::end);
				break;
			}
		default:
			break;
		}
	
}

template<class T>
void BinFile<T>::Delete() {
	f.close();
	if (remove(fname.c_str()))
		Error("Ошибка удаления файла");
}

template <class T>
int BinFile<T>::Read(T* A, int n){
	if (!fileOpen)
		Error("BinFile Read(long pos): Файл закрыт");
	if (accessType == OUT)
		Error("BinFile Read(long pos): Недопустимая операция доступа к файлу");
	f.read((char*)A, Tsize * n);
	if (accessType = IN);
	if (f.tellg() / Tsize >= filesize)
		f.clear(ios::eofbit);	
	return 1;
}

template <class T>
void BinFile<T>::Append(T item) {
	T* data = new T();
	*data = item;
	if (accessType == IN)
		Error("BinFile Write(T* A, int n): Недопустимая операция доступа к файлу");
	if (!fileOpen)
		Error("BinFile Write(T* A, int n): Файл закрыт");
	f.seekg(0, ios::end);
	f.write((char*)data, Tsize);
	filesize = f.tellg() / Tsize;
	delete data;
}

template <class T>
void BinFile<T>::Write(const T& data, long pos) {
	long previousRecords;
	if (accessType == IN)
		Error("BinFile Write(T* A, int n): Недопустимая операция доступа к файлу");
	if (!fileOpen)
		Error("BinFile Write(T* A, int n): Файл закрыт");
	f.seekg(pos, ios::beg);
	f.write((char*)data, Tsize);
	filesize = f.tellg() / Tsize;
}

template <class T>
BinFile<T>::~BinFile() {
	Close();
}