// light-vector

template <typename T>
class lvector
{
	T* m_p;
	int m_n;
public:

	/** пустой ветор */
	lvector();

	/** ветор размером n*/
	lvector(int n);

	/** ветор */
	lvector(int n, const T& x);
	lvector(const lvector& v);
	~lvector();
	void clear();
	int size();
	T& front();
	T& back();
	T& at(int n);
	T& operator[](int n);
	void push_back(const T& t);
	
	
	class iterator
	{
		T* m_p;
	public:
		iterator(){m_p = 0;};
		iterator(T* pT){m_p = pT;};
		T& operator*(){return *m_p;};
		iterator& operator++()
		{
			++m_p;
			return *this;
		}
		iterator& operator--()
		{
			--m_p;
			return *this;
		}
		bool operator==(const iterator& it)
		{
			return m_p == it.m_p;
		}
		bool operator!=(const iterator& it)
		{
			return m_p != it.m_p;
		}
	};
	iterator begin()
	{
		return iterator(m_p);
	}
	iterator end()
	{
		return iterator(&m_p[m_n]);
	}
};

template<typename T> lvector<T>::lvector()
{
	m_p = 0;
	m_n = 0;
}

template<typename T> lvector<T>::lvector(int n)
{
	m_n = n;
	m_p = new T[m_n];
}

template<typename T> lvector<T>::lvector(int n, const T& x)
{
	m_n = n;
	m_p = new T(x)[m_n];
  //for(int i = 0; i < n; i++) m_p[i] = x;
}

template<typename T> lvector<T>::lvector(const lvector& v)
{
	m_n = v.m_n;
	m_p = new T[m_n];
	for(int i = 0; i < m_n; i++) m_p[i] = v.m_p[i];
}

template<typename T> lvector<T>::~lvector()
{
	delete[] m_p;
}

template<typename T> void lvector<T>::clear()
{
	delete m_p[];
	m_n = 0;
	m_p = 0;
}

template<typename T> int lvector<T>::size()
{
	return m_n;
}

template<typename T> T& lvector<T>::front()
{
	if(m_n) return *m_p;
}

template<typename T> T& lvector<T>::back()
{
	if(m_n) return m_p[m_n - 1];
}

template<typename T> T& lvector<T>::at(int n)
{
	if((i>0) && (i<m_n)) return m_p[i];
	else throw std::out_of_range();
}

template<typename T> T& lvector<T>::operator[](int n)
{
	return m_p[n];
}

template<typename T> void lvector<T>::push_back(const T& t)
{
	T* pNew = new T[m_n + 1];
	for(int i = 0; i < m_n; i++) pNew[i] = m_p[i];
	pNew[m_n++] = t;
	delete[] m_p;
	m_p = pNew;
}


int test_user_vector();