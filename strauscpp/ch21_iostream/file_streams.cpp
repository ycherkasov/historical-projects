#include <iostream>
#include <fstream>
#include <ctime>
#include "streams.h"

using namespace std;

// ��������� �� C++ ������ ����� ���� �� � ������, � � ����������� �������,
// � �������� ����� ����������� � ���������� �����
// ������ ����� (� ������) ������������ ������ � ������ ������ RAII

// ������ ��� ������ � ������� ��������������. 
// ���� ��������� ����������� ��������, ����� �� ������ �� ���� �� ����������

// ��������� ����� ������������� � ����������� �� ������. �������� - �� ��, �� ������.

void show_fstream_write(){
	srand(static_cast<unsigned>(time(0)));

	// ������ 10 ��������� ������ � ����
	ofstream f("random.txt");
	for(int i = 0; i < 10 ; i++){
		f << rand()%10 << '\n';
	}
}

void show_fstream_read(){

	// ������ � ������ �� �����
	ifstream f("random.txt");
	while(!f.eof()){
		char buf[20] = {};	// ������ ������� ��� int
		f.getline(buf, 20);
		cout << buf << endl;
	}
}

void copy_file(const std::string& from, const std::string& to){
	ifstream inf(from);
	ofstream outf(to);
	char c = 0;
	// ����������� �����������
	while(inf.get(c)) outf.put(c);
}

void show_ofstream_binary_write(){
	srand(static_cast<unsigned>(time(0)));

	// ��������� 10 ��������� ������
	int a[10] = {};
	for(int i = 0; i < 10 ; i++){
		a[i] = rand();
	}

	// ������ 10 ��������� ������ � �������� ����
	// (����� ���������� ����� ������ ����� - ��������, ���������� � ��)
	ofstream f("random.bin", ios::binary);
	

	// ���� ������ ����� - ����� ����������, ��� �����������
	f.write(reinterpret_cast<char*>(a), sizeof(a));
}

void show_ofstream_binary_read(){
	int a[10] = {};
	
	// ������ ����� ���� ������ �����
	ifstream f("random.bin", ios::binary);
	f.read(reinterpret_cast<char*>(a), sizeof(a));
}


//////////////////////////////////////////////////////////////////////////
// ������������

// �������� ������������������ ��������� ������ ������������� ������� 
// � �������� ������������
// ������ ��� ����������� �� ����������
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
			// ��������� ������ �������
			f.read(reinterpret_cast<char*>(&sz), sizeof(size_t));

			// ��������� ��� ������
			f.read(reinterpret_cast<char*>(arr), sz*sizeof(int));
		}
	}

	//
	void save(ofstream& f){
		if(f.is_open()){
			// �������� ������ �������
			f.write(reinterpret_cast<char*>(&sz), sizeof(size_t));

			// �������� ��� ������
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
	show_fstream_write();
	show_fstream_read();
	string f1("random.txt");
	string f2("random2.txt");
	copy_file(f1, f2);

	show_ofstream_binary_write();
	show_ofstream_binary_read();
	show_serialize();
}