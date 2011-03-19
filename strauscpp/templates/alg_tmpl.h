// ���������� ���������� ���������� �������� ��������

// ------------------- ���������� ����� ��������� �� ������� -------------------
namespace alg_func_pointer{
	// ��������� �������� "���������� ���� ������"
	template< typename InputIterator, typename OutputIterator, typename T >
	void copy_if_greater(InputIterator first, InputIterator last, OutputIterator out, const T& val){
		while( first != last )	{
			// ���� ������, ����������� � out
			if(*first > val){
				*out = *first;
				++out;
			}
			++first;
		}
	}

	// �������� �� ����������� � ��� �����, ��� ������� ������ ������ � ���� �������
	// ��������� ����������� ���������� �� ������� � ��������� ����������
	template<typename T>
	bool less_func(const T& a,const T& b){return a > b;}

	template<typename T>
	bool greater_func(const T& a,const T& b){return a < b;}

	template< typename InputIterator, typename OutputIterator, typename T >
	void copy_if_greater(InputIterator first, InputIterator last, OutputIterator out
		,bool comparer( const T& a, const T& b ) , const T& val){
			while( first != last )	{
				// ���� ������������� ������� 'comparer', ����������� � out
				if( comparer(val, *first) ){
					*out = *first;
					++out;
				}
				++first;
			}
	}

	// ��� ���:
	template< typename InputIterator, typename OutputIterator, typename Predicate, typename T >
	void copy_if_greater_pred(InputIterator first, InputIterator last, OutputIterator out
		,Predicate comparer, const T& val){
			while( first != last )	{
				// ���� ������������� ������� 'comparer', ����������� � out
				if( comparer(val, *first) ){
					*out = *first;
					++out;
				}
				++first;
			}
	}
	// ���������� ��������� - ������ �������� ���������� T val ��� �� ��������� �������� ����������
	// ������� ��� ��������� ���������
}


namespace alg_functor{
	// ------------------- ���������� ����� ������� -------------------
	// ������������ ���������� ����� �������:
	// - ����� ����������� operator() ��������� ���
	// - ����� ����������������, ��������� ��������� � ������� ����� �������������� ��������
	// - ����� ����������� � ������ operator() �����������

	// �������-�������� ��� ���������
	struct is_odd{
		bool operator()(const int d){
			return !(d%2);
		}
	};

	// �������-�������� � ����������
	struct greater_then{
		int _val;
		greater_then(int val) : _val(val){}
		bool operator()(const int i){
			return i > _val;
		}
	};

	// �������-������ � ����������
	template <typename T>
	struct less_then{
		T _val;
		less_then() : _val(){}
		less_then(const T& val) : _val(val){}
		bool operator()(const int i){
			return i < _val;
		}
	};

	template< typename InputIterator, typename OutputIterator, typename Predicate >
	void copy_if(InputIterator first, InputIterator last, OutputIterator out
		,Predicate comparer){
			while( first != last )	{
				// ���� ������������� ������� 'comparer', ����������� � out
				if( comparer(*first) ){
					*out = *first;
					++out;
				}
				++first;
			}
	}
}

