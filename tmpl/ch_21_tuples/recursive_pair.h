#pragma once


// ���������� ������� std::pair
template <typename T1, typename T2>
struct Pair{
	Pair():v1(),v2(){}
	Pair(T1 const& t1,T2 const& t2):v1(t1),v2(t2){}
	T1 v1;
	T2 v2;
};

// ������������� ��� ������ ���� (���� � ������� �������������� �������� ����� �����)
template <typename T1>
struct Pair<T1, void>{
	Pair():v1(){}
	Pair(T1 const& t):v1(t){}
	T1 v1;
};

// ����� ��������� ������� ����������� ��������� � Pair
typedef Pair<int, Pair<char, Pair<double, long> > > tuple4;

//////////////////////////////////////////////////////////////////////////
// ������ ��� ���������� ���� �� �������

// ��������� ������ - � ����� ������ ���������� void
template <int N, typename T>
struct recursive_tuple{
	typedef void Result;
};

/* ������������ �����
	  .
	 / \
	/  /\
	A  B C
*/

// ������������� ��� n-�� ����
// ������������ �� �����, ��� n-� ��� ����������� ���� - ��� n-1 ��� ����, ������������ � ���
template <int N, typename A, typename B, typename C>
struct recursive_tuple<N, Pair<A, Pair<B, C> > >{
	typedef typename recursive_tuple<N-1, Pair<B,C> >::Result Result;
};


// ������������� ��� 2-�� ���� ����������� ����
template <typename A, typename B, typename C>
struct recursive_tuple<2, Pair<A, Pair<B, C> > >{
	typedef B Result;
};

// ������������� ��� 1-�� ���� ����������� ����
template <typename A, typename B, typename C>
struct recursive_tuple<1, Pair<A, Pair<B, C> > >{
	typedef A Result;
};

// ������������� ��� 2-�� ���� ������� ���� (��������� 2 ���� � �������)
template <typename A, typename B>
struct recursive_tuple<2, Pair<A, B> >{
	typedef B Result;
};

// ������������� ��� 1-�� ���� ������� ���� (��������� 2 ���� � �������)
template <typename A, typename B>
struct recursive_tuple<1, Pair<A, B> >{
	typedef A Result;
};




//////////////////////////////////////////////////////////////////////////
// ������ ��������� ����������� ���������� n-� �������� �������

// ��������� ������ ��� ���������� �������� - ���������� �����������
template <int N, typename T>
struct TupleValue{
	static void get(T&){}
	static void get(T const&){}
};

// ������������� ��� n-�� ����
template <int N,  typename A, typename B, typename C>
struct TupleValue<N, Pair<A, Pair<B,C> > >{

	static typename recursive_tuple<N-1, Pair<B,C> >::Result& 
		get(Pair<A, Pair<B,C> >& d){
			return TupleValue<N-1, Pair<B,C> >::get(d.v2);
	}

	static typename recursive_tuple<N-1, Pair<B,C> >::Result const& 
		get(Pair<A, Pair<B,C> > const& d){
			return TupleValue<N-1, Pair<B,C> >::get(d.v2);
	}
};

// ������������� ��� 1-�� ���� ����������� ����
template <typename A, typename B, typename C>
struct TupleValue<1, Pair<A, Pair<B,C> > >{

	static A& get(Pair<A, Pair<B,C> >& d){
		return d.v1;
	}

	static A const& get(Pair<A, Pair<B,C> > const& d){
		return d.v1;
	}
};

// ������������� ��� 2-�� ���� ����������� ����
template <typename A, typename B, typename C>
struct TupleValue<2, Pair<A, Pair<B,C> > >{

	static B& get(Pair<A, Pair<B,C> >& d){
		return d.v2.v1;
	}

	static B const& get(Pair<A, Pair<B,C> > const& d){
		return d.v2.v1;
	}
};

// ������������� ��� 1-�� ���� ������� ���� (��������� 2 ���� � �������)
template <typename A, typename B>
struct TupleValue<1, Pair<A,B> >{
	static A& get(Pair<A,B>& d){
		return d.v1;
	}

	static A const& get(Pair<A,B> const& d){
		return d.v1;
	}
};

// ������������� ��� 2-�� ���� ������� ���� (��������� 2 ���� � �������)
template <typename A, typename B>
struct TupleValue<2, Pair<A,B> >{
	static B& get(Pair<A,B>& d){
		return d.v2;
	}

	static B const& get(Pair<A,B> const& d){
		return d.v2;
	}
};



// ������� �������
template <int N, typename A, typename B>
typename recursive_tuple<N, Pair<A,B> >::Result& value(Pair<A,B>& d){
	return TupleValue<N, Pair<A,B> >::get(d);
}
//
template <int N, typename A, typename B>
typename recursive_tuple<N, Pair<A,B> >::Result const& value(Pair<A,B> const& d){
	return TupleValue<N, Pair<A,B> >::get(d);
}
