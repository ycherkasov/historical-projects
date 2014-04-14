// ���������� ���������� ���������� �������� ��������

// ------------------- ���������� ����� �������� � ������� -------------------
namespace alg_adapter{
	
	// ������� - ��� �������, ���� ���������� �������� �������� � ���� �� ����������
	// � ����� ������ �������� ����� ��������� ������������ ����� ���������
	// �� ����� ��� ������� ����������� ������ ��������������

	// �.�. �� ��������� ���������� �� �������� ��������,
	// � �������� �������� ����� ������, � ������� �� �������� ������ � ����������� ���������

	// ��������� 2 �������:

	// ��� ������� ���������
	template <typename Predicate, typename T>
	class bind_first{
	public:
		bind_first(const Predicate& p, const T& t) : pred(p), val(t){}
		bool operator()(const T& right) {
			return pred(val, right);
		}
	private:
		Predicate pred;
		const T val;
	};

	// ��� ������� ���������
	template <typename Predicate, typename T>
	class bind_second{
	public:
		bind_second(const Predicate& p, const T& t) : pred(p), val(t){}
		bool operator()(const T& left) /*const*/{	// ������������� ��� ������ - ������ ������ const?
			return pred(left, val);
		}
	private:
		Predicate pred;
		const T val;
	};

	// ��������� ��� ������������
	template<typename T>
	struct less_f{
		bool operator()(const T& a,const T& b){return a < b;}
	};

	template<typename T>
	struct greater_f{
		bool operator()(const T& a,const T& b){return a > b;}
	};

	// ��������� �������-�������� ��� ��������, 
	// ����� �� ��������� ��� ������������� ��������� �� ����� ���
	template<typename Predicate, typename T> 
	bind_first<Predicate, T> bind_left(const Predicate& p, const T& left){
		T value(left);
		return bind_first<Predicate, T>(p, value);
	}

	template<typename Predicate, typename T> 
	bind_second<Predicate, T> bind_right(const Predicate& p, const T& right){
		T value(right);
		return bind_second<Predicate, T>(p, value);
	}
}


// ------------------- ������� ��������� �� ������� � ��� ������������� � ������� 
namespace alg_adapter_func {

	// ������� �������
	bool is_odd_func( int a ){
		return  !(a%2);
	}

	// �������� �������
	bool greater_func( int a, int b ){
		return  a > b;
	}

	// ������ ������ �������� � �������-�������� ��� ���������� �� �������:
	
	// ��� ������� ������� 
	template <typename ResultT, typename T>
	class unary_function_ptr {
	public:
		// ��������� ��������� �� �������, �������������� ���� ������
		explicit unary_function_ptr( ResultT (*PF)(T) ) : pfun(PF){}
		
		// ��������� �������
		ResultT operator()(T arg) const {
			return pfun(arg);
		}
	private:
		// ���������, ����������� � ������������ ���� ��������
		ResultT (*pfun)(T);
	};

	// ������� ��� ����������� ������ ������� �������� ���������
	template <typename ResultT, typename T> inline
		unary_function_ptr<ResultT, T> ptrfun( ResultT (*PF)(T) ) {
			return unary_function_ptr<ResultT, T>(PF);
	}

	// ��� �������� ������� 
	template <typename ResultT, typename T1, typename T2>
	class binary_function_ptr{
	public:
		// ��������� ��������� �� �������, �������������� ���� ������
		explicit binary_function_ptr( ResultT (*PF)(T1,T2) ) : pfun(PF){}

		// ��������� �������
		ResultT operator()(T1 left, T2 right) const{
			return pfun(left, right);
		}
	private:
		// ���������, ����������� ��� ��������� � ������������ ����
		ResultT (*pfun)(T1, T2);
	};

	// ������� ��� ����������� ������  ������� ��������� ���������
	template <typename ResultT, typename T1, typename T2> inline
		binary_function_ptr<ResultT, T1, T2> ptrfun( ResultT (*PF)(T1, T2) ){
			return binary_function_ptr<ResultT, T1, T2>(PF);
	}
	// � ����� ���������� (� � ��������������� �������� �������)
	// ��������� ����� ����� ���������� ������� �������
}

// ------------------- ������� ��������� �� ����� -------------------
namespace alg_adapter_method{

	// ����� � �������, ������� ����� ����������
	class constant{
	public:
		constant() : _val(){}
		constant(int v) : _val(v){}
		bool greater(int t){
			return _val > t;
		}
		bool is_odd(){
			return !(_val%2);
		};

	private:
		int _val;
	};

	// �� ����� ������� ����� �����������
	template <typename Arg>
	struct unary_predicate{
		typedef Arg	argument_type;
	};

	// ...�� ����� ����
	template <typename Arg1, typename Arg2>
	struct binary_predicate{
		typedef Arg1	first_argument_type;
		typedef Arg2	secind_argument_type;
	};

	// ��������� �����-��������
	template <typename T>
	struct greater : public binary_predicate<T,T>{
		bool operator()(const T& left,const T& right){
			return left > right;
		}
	};

	// ��������� �����-������
	template<class Predicate>
	class bind_second : public unary_predicate< typename Predicate::first_argument_type >{
	public:

		// ������� ������������ typedef � ���������
		typedef unary_predicate<typename Predicate::first_argument_type> base;
		typedef typename base::argument_type argument_type;

		// ������������� ��������
		bind_second(const Predicate& p, const typename Predicate::second_argument_type& right) :
			, predicate(p), value(right){}

		// ���������� ���������
		void operator()(argument_type left){
			return predicate(left, value);
		}

	private:
		Predicate predicate;
		typename Predicate::second_argument_type value;
	};

	// ��������� �����-������� ��� ������� �������
	// (��������� ��� �������, ����������� ����� � ��� ���������)
	template <typename Obj>
	class unary_method_ptr : public unary_predicate<Obj>{
	public:
		explicit unary_method_ptr( bool (Obj::*p)() ) : ptrmethod(p){}

		// ���������� ������ ������ Obj
		bool operator()(Obj obj){
			return (obj.*ptrmethod)();
		}
	private:
		// ��������� �� ����� ������ Obj, ������������ T
		bool (Obj::*ptrmethod)();
	};

	// ��������� �����-������� ��� �������� �������
	// (��������� ��� �������, ����������� ����� � ��� ���������)
	template <typename Obj, typename T>
	class binary_method_ptr : public binary_predicate<Obj,T>{
	public:
		explicit binary_method_ptr( bool (Obj::*p)(T val) ) : ptrmethod(p){}
		
		// ���������� ������ ������ Obj
		bool operator()(Obj obj, T val){
			return (obj.*ptrmethod)(val);
		}
	private:
		// ��������� �� ����� ������ Obj, ������������ T
		bool (Obj::*ptrmethod)(T val);
	};

	// �������-������� ��� ��������, ��������� ��������� �� ������� ����� ������ Obj
	template <typename Obj> inline
	unary_method_ptr<Obj> memfun( bool (Obj::*pm)() ){
			return unary_method_ptr<Obj>(pm);
	}

	// �������-������� ��� ��������, ��������� ��������� �� �������� ����� ������ Obj
	template <typename Obj, typename T> inline
	binary_method_ptr<Obj,T> memfun( bool (Obj::*pm)(T val) ){
		return binary_method_ptr<Obj,T>(pm);
	}
}


