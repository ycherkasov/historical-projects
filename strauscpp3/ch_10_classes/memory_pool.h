#pragma once
#include <new>
#include <cstring>
// � ���� ������ ��������������� �������� ����������,
// ����������� �� ���� ������ (������� ������ �������������� �������,
// ����������� �� N �������� ����������� ��� �� �������������� M �������)

// �����-��� ������
template <typename T>
class memory_pool{
public:
	memory_pool() :
		block_size(sizeof(T)),
		head(::operator new(block_size * pool_size)){
		::memset(head, 0x0, block_size * pool_size);
	}

	~memory_pool() {
		::operator delete(head);
	}

	void* alloc(size_t s) {

		// �������� ������ ��� ������ ������������ �������
		if (s != sizeof(T))
		{
			return ::operator new(s);
		}

		// ������ ������ ��������� ���� - �� ��������� �� 0
		unsigned char* ret = static_cast<unsigned char*>(head);
		for (size_t i = 0; (*ret != 0); ++i)
		{
			ret += block_size;
			if (i == pool_size)
				// pool is over, use standard new instead
				return ::operator new(s);
		}
		return ret;

	}

	void free(void* p, size_t s) const {
		// �������� ������ ��� ������ ������������ �������
		if (s != sizeof(T))
		{
			::operator delete(p);
		}

		// ������ ���� ����� ����� == 0, ���� ��������� ���������
		unsigned char* free_byte = static_cast<unsigned char*>(p);
		*free_byte = 0;
	}

private:
	const size_t block_size;
	void* head;
	static size_t pool_size;
};

// static 
template <typename T>
size_t memory_pool<T>::pool_size = 512;

class memory_pool_item{
public:
	memory_pool_item();
	virtual ~memory_pool_item();

	// new/delete overload that use memory pool
	static void* operator new(size_t s);
	static void operator delete(void* p, size_t s);

	void test();
private:
	int i;
	static memory_pool<memory_pool_item> pool;
};
