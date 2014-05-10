#include <string> // std::getline
#include <iostream>
#include <fstream>
#include <ctime>
#include "streams.h"

using namespace std;

// Программа на C++ обычно имеет дело не с файлом, а с абстрактным потоком,
// к которому можно привязывать и отвязывать файлы
// Однако можно (и удобно) использовать потоки в рамках идиомы RAII

// Потоки при работе с файлами буферизируются. 
// Если программа завершилась аварийно, сброс из буфера на диск не происходит

// Текстовые файлы форматируются и разбиваются на строки. Бинарные - ни то, ни другое.

void file_stream1(){
	// время жизни файловых потоков никак не привязано ко времени открытия файла
	// можно с помощью одного потока много кратно открывать-закрывать файл
	// или разные файлы

	// чтение строк из текстового файла
	ifstream f("file1.txt");
	while (!f.eof()){
		string s;
		getline(f, s);
		cout << s << endl;
	}
}


void show_fstream_write(){
	srand(static_cast<unsigned>(time(0)));

	// запись 10 случайных числел в файл
	ofstream f("random.txt");
	for(int i = 0; i < 10 ; i++){
		f << rand()%10 << '\n';
	}
	// потоки буферизированы, так что если функция завершается аварийно
	// последняя порция символов в файл не попадает
}

void show_fstream_read(){

	// Чтение в массив из файла
	ifstream f("random.txt");
	while(!f.eof()){
		char buf[20] = {};	// должно хватить для int
		f.getline(buf, 20);
		cout << buf << endl;
	}
}

void copy_file(const std::string& from, const std::string& to){
	ifstream inf(from);
	ofstream outf(to);
	char c = 0;
	// скопировать посимвольно
	while(inf.get(c)) outf.put(c);
}

void show_ofstream_binary_write(){
	srand(static_cast<unsigned>(time(0)));

	// генерация 10 случайных числел
	cout << "Generate random array\n";
	int a[10] = {};
	for(int i = 0; i < 10 ; i++){
		a[i] = rand();
		cout << a[i] << '\n';
	}

	// запись 10 случайных числел в бинарный файл
	// (можно передавать также другие флаги - создания, перезаписи и тп)
	ofstream f("random.bin", ios::binary);
	

	// весь массив сразу - более оптимально, чем посимвольно
	size_t sz = sizeof(a);
	f.write(reinterpret_cast<char*>(a), sz);
}

void show_ofstream_binary_read(){
	int a[10] = {};
	
	// читаем также весь массив сразу
	cout << "Read random array\n";
	ifstream f("random.bin", ios::binary);
	size_t sz = sizeof(a);
	f.read(reinterpret_cast<char*>(a), sz);
	for (int i = 0; i < 10; i++){
		cout << a[i] << '\n';
	}
}


//////////////////////////////////////////////////////////////////////////
// Сериализация

// Создадим самогенерирующийся рандомный массив произвольного размера 
// с методами сериализации
// Обычно они наследуются от интерфейса
struct RandomArray{
	
	size_t sz;
	int* arr;

	RandomArray(size_t s):sz(s),arr(new int[sz]){
		for(int i = 0; i < 10 ; i++){
			arr[i] = 0;
		}
	}

	void generate(){
		for(int i = 0; i < 10 ; i++){
			arr[i] = rand();
		}
	}

	~RandomArray(){
		delete[] arr;
	}
	
	//
	void load(ifstream& f){
		if(f.is_open()){
			// прочитали размер массива
			f.read(reinterpret_cast<char*>(&sz), sizeof(size_t));

			// прочитали сам массив
			f.read(reinterpret_cast<char*>(arr), sz*sizeof(int));
		}
	}

	//
	void save(ofstream& f){
		if(f.is_open()){
			// записали размер массива
			f.write(reinterpret_cast<char*>(&sz), sizeof(size_t));

			// записали сам массив
			f.write(reinterpret_cast<char*>(arr), sz*sizeof(int));
		}
	}
};

void show_serialize(){

	{
		RandomArray ra(10);
		ra.generate();
		ofstream f("rarray.bin", ios::binary);
		ra.save(f);
	}

	{
		RandomArray ra(10);
		ifstream f("rarray.bin", ios::binary);
		ra.load(f);
	}
}

void show_fstream(){
	file_stream1();

	show_fstream_write();
	show_fstream_read();
	string f1("random.txt");
	string f2("random2.txt");
	copy_file(f1, f2);

	show_ofstream_binary_write();
	show_ofstream_binary_read();
	show_serialize();
}