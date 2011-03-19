// ������������ ������� ��������� ���������� � ���������� ����������
// ���������� "������� �� 0"


void oor_exception_ex(std::vector<int>& v);
void set_zero_div_exception();
int zero_div();


// ����� ������������
class triangle{
public:
	// ���������� ������ ������������
	class triangle_exception{
	public:
		virtual const char* what() const throw() = 0;
	};

	class wrong_param : public triangle_exception{
	public:
		wrong_param(double p) throw() : _param(p){}
		
		// ����� �������� ��� ��������� 
		virtual const char* what() const throw()  { return "Wrong param check!\n"; }

		// ������� ��������� ��������
		double get_wrong_param(){ return _param; }

	private:
		double _param;
	};

	// ��������� ��� ���������������:
	// ������ - �� �����������!
	class non_triangle_exception : public triangle_exception{
	// ��� ������ ��������� �� ������������� throw() - �� ������ ���������!
	// ���� ���� ��������� ����� ������ - ��������� ������ �� terminate()
	public:
		non_triangle_exception(double a, double b, double c) throw() : _a(a), _b(b), _c(c){}
		
		// ����� �������� ��� ��������� 
		virtual const char* what() const throw()  { return "Non-triangle check!\n"; }
		
		// ����� �������� ������ ������������ �������
		double wrong_size() throw() {
			if(_a+_b < _c) return _c;
			if(_a+_c < _b) return _b;
			return _a;
		}
	private:
		double _a;
		double _b;
		double _c;
	};

	class test_triangle_ex : public triangle_exception {
	public:
		test_triangle_ex(int p) throw() : _param(p){}
		virtual const char* what() const throw()  { return "Test exception check!\n"; } 
	private:
		int _param;
	};
	// ------------------- triangle  methods -------------------
	// ���������� ������� � ��������� �������� ��� �����������
	triangle(double a, double b, double c);
	~triangle();

	double perimeter();
	double square();
	void throw_test() const;

	// ������������ ����������,
	// ��� ��������� ������ ��������� ��������� �� terminate()
	// � VC �� ��������, �� ���� warning ((
	// warning C4297: 'triangle::test_nothrow' : function assumed not to throw an exception but does
	void test_nothrow() const throw();
	void test_throw_some1() const throw(test_triangle_ex);
	void test_throw_some2() const throw(triangle_exception);

private:
	int* _test;

	double _a;
	double _b;
	double _c;

	double _S;
	double _P;
};