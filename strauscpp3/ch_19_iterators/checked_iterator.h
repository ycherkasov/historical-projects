#pragma once

#include <utility>
#include <iterator>


// ����� ���������, ��������� ���������� ��� ������ �� ������� ����������
// ���-�� �������� ����������� � ���������� �������

// ����� �� �������, ��� ����� �������� ������ ���������� �� ���������
// ������� ����� ����������� ��������������� �� �������

template <typename Container, typename Iter = typename Container::iterator>
class Checked_iterator : public std::iterator_traits<Iter>{
public:

	// ��������� ������������� ��������� �� �����������
	void valid(Iter p) const {

		if (cont->end() == p){
			return;
		}

		for(Iter pp = cont->begin() : pp != cont->end(); ++p){
			if(pp == p)
				return;
		}

		throw_oor();
	}

	// ������������� �������� �� ���������
	friend bool operator == (const Checked_iterator& a, const Checked_iterator& b){
		return ( (a.cont == b.cont) && (a.iter == b.iter) );
	}

	// ������������� ����������� � ����������
	Checked_iterator(Container& c, Iter p): cont(&c), iter(p){
		valid(p);
	}

	// ������������� ����������� � ���������� �� ��� ������
	Checked_iterator(Container& c): cont(&c), iter(c.begin()){}

	// ������� ��������� - operator* 
	reference operator*() const {
		if(iter == c->end()){
			throw std::out_of_range("OOR");
		}
		return iter;
	}

	// ������� ��������� - operator->
	reference operator->() const {
		// ����������� ���������� *
		return &*iter;
	}

	// ������� ��������� - operator+
	Checked_iterator operator+( difference_type d ) const {
		if(  
			( ( c->end() - iter) < d  ) || // ���� ��� �������� ������� �� �����
			(d < -(iter  - c->begin()) ) ){ // ��� �� ������

				throw std::out_of_range("OOR");	// ����������
		}
		// ������� ��������
		return Checked_iterator(cont, iter+d);
	}

	// ������� ��������� - operator[]
	reference operator[]( difference_type d ) const {
		// ���� ��� ���������� ������� �� ����� ��� ���� �� ��������� ���
		if( ( ( cont->end() - iter) <= d  ) || 
			// ��� �� ������
			(d < -(iter  - cont->begin()) ) ){ 
				throw_oor();
		}
		// ������� ��������
		return iter[d];
	}

	// ������� ��������� - operator++
	Checked_iterator& operator++(  ) {

		if(iter == cont->end()){
			throw_oor();
		}
		++iter;
		return *this;
	}

	// ������� ��������� - operator++(int)
	Checked_iterator operator++( int ) {
		Checked_iterator temp = *this;
		++(*this);
		return temp;
	}

	// ������� ��������� - operator--
	Checked_iterator& operator--(  ) {

		if(iter == cont->begin()){
			throw_oor();
		}
		--iter;
		return *this;

	}

	// ������� ��������� - operator--(int)
	Checked_iterator operator--( int ) {
		Checked_iterator temp = *this;
		--(*this);
		return temp;
	}


	// ��������� �� ������� - ��� RA
	difference_type index() const {
		return iter - c.begin();
	}

	// ��������� �������� ���������
	Iter unchecked() const {
		return iter;
	}
protected:

	// ��� ���������� �������� ��� ������ �� ������� - 
	// ��� � ������ �������� "�����������"
	void throw_oor() const {
		throw std::out_of_range("OOR");
	}


private:
	// ������� �������� ��� ��������
	Iter iter;

	// ������� ���������
	Container* cont;
};

// �.�. �� �� ����� �������� ��� ��������� � ��������� � ��������� ����������,
// �� ����� ������������� ��������� ��� ��� �������������

template <typename C>
class Checked_container : public C{
public:

	typedef Checked_iterator<C> iterator;
	typedef Checked_iterator<C, typename C::const_iterator> const_iterator;

	// ��������� �� ���������
	Checked_container():C(){}

	// ��������� � ��������� �������
	explicit Checked_container(size_t n):C(n){}

	// ������� ������� � ������ � �����
	iterator begin(){
		return iterator(*this, C::begin());
	}

	const_iterator begin() const {
		return const_iterator(*this, C::begin());
	}

	iterator end(){
		return iterator(*this, C::end());
	}

	const_iterator end() const {
		return const_iterator(*this, C::begin());
	}

	//  []
	typename C::reference operator[] (typename C::size_type n){
		return Checked_iterator<C> (*this) [n];
	}

	//  �������� ������� ���������
	C& base(){
		return this;
	};

};
