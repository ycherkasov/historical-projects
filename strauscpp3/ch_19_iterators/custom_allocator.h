#pragma once

namespace std {

// ������� ���������, �������������� ������ - ��� ������ ����������
// �����, �������������� ����������� ��������� �������������� STL
template <typename T>
class custom_allocator{
public:
	// ����������� �����-�������
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;

	// ��������� �������������� ������� ����
	// ��� ����� ������������, ���� � ��������� ������
	// ����� ������� �� ������ ���� ������, � �����-����
	// ��������� ���������� (����, ���-�������) � ��������������
	// �������� ��� ��� ������
	template <typename U>
	struct rebind{
		typedef custom_allocator<U> other;
	};

	// ��������� �� ����� ���������, ������� ������������ � ���������� �����������
	custom_allocator() throw() {
		cout << "Type: " << typeid(T).name() << endl;
		cout << "Ref: " << typeid(reference).name() << endl;
		cout << "Ptr: " << typeid(pointer).name() << endl;
	}
	custom_allocator(const custom_allocator&) throw(){}
	~custom_allocator() throw() {}
	
	// ����������� ��� ��������� �����
	// ��������, ��� ����� ������, �������� �������� ����, 
	// ���������� �������� ���-�������
	template <typename U>
	custom_allocator(const custom_allocator<U>&) throw() {}

	// ������� �������
	pointer address(reference val){
		return &val;
	}

	const_pointer address(const_reference val){
		return &val;
	}

	// ������������ ���������� ���������
	size_type max_size() const throw(){
		return numeric_limits<size_t>::max() / sizeof(T);
	}

	// �������� ������ ��� n ��������� ��� �������������
	// �������� ����� �� ��������������!
	// ����� ������� �������� ������� �� ����������
	// ����� ���� ����������� ��� ���������� new, ��������, ��������� �� ���������� ���� ������
	pointer allocate(size_type num, custom_allocator<void>::const_pointer hint = nullptr){
		return reinterpret_cast<pointer>( ::operator new(num * sizeof(T)) );
	}

	// ������������� ������ �������� ����������� ����� ��������� val
	void construct(pointer p, const T& val){
		new (reinterpret_cast<void*>(p)) T(val);
	}

	// �������� ������ �������� �� �����
	void destroy(pointer p){
		// ���� �� �������, ����� ���������� ���������� ����
		p->~T();
	}
	
	// �������� ������ �����
	// � ����� ������� ������� ������ ���� ����������
	void deallocate(pointer p, size_type){
		// � ����� ������ deallocate �� ���� �������� ��� �������� nullptr
		::operator delete(reinterpret_cast<void*>(p));
	}
private:
	// ����� ��������� �������������� ������� (� �������������)
	// ����� ��������� ������ (��� �������� �������, �������� ����������� � ��)
};

// ����������� ������ ���� ��������� ������������� ��� void
template <>
class custom_allocator<void>{
public:
	// ����������� �����-�������
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef void value_type;
	typedef void* pointer;
	typedef const void* const_pointer;

	template <typename U>
	struct rebind{
		typedef custom_allocator<U> other;
	};
};

// ��������� �������� == ��������, ��� ��� ������������� ���������������
// ��� ������, ��� ������ ����� �������� �����, � ���������� ������
template <typename T1, typename T2>
bool operator==(const custom_allocator<T1>&, const custom_allocator<T2>&) throw(){
	return true;
}

template <typename T1, typename T2>
bool operator!=(const custom_allocator<T1>&, const custom_allocator<T2>&) throw(){
	return false;
}


} //namespace std