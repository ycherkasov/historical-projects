#pragma once
#include <deque>
// ƒемонстраци€ применени€ шаблонных параметров шаблона
// - Ўаблонный параметр определ€етс€ ключевым словом class
// - »м€ шаблонного параметра шаблона не может быть использовано в шаблоне
// - ≈сли им€ шаблонного параметра шаблона нигде не используетс€, его можно опустить
// - Ўаблонный параметр шаблона, имеющий параметры по умолчанию, 
//   должен повтор€ть их в списке параметров
template < 
	typename T, 
	template <typename ELEM, typename = std::allocator<ELEM> > class CONT = std::deque >
class tmpl_stack {
public:
	tmpl_stack();
	void push(const T& p);
	void pop();
	const T& top() const;
	bool empty() const;

	// Ўаблонный метод
	template<typename TCOPY,template <typename, typename> class CONT_COPY>
	tmpl_stack<T,CONT>& operator=(const tmpl_stack<TCOPY,CONT_COPY>& s);

private:
	CONT<T> _copyable_stack;
	// ELEM _elem; - аргумент шаблонного параметра шаблона использовать нельз€
};

template <typename T, template <typename, typename> class CONT>
tmpl_stack<T,CONT>::tmpl_stack(){}

template <typename T, template <typename, typename> class CONT>
void tmpl_stack<T,CONT>::push(const T &p){
	_copyable_stack.push_back(p);
}

template <typename T, template <typename, typename> class CONT>
void tmpl_stack<T,CONT>::pop(){
	_copyable_stack.pop_back();
}

template <typename T, template <typename, typename> class CONT>
const T& tmpl_stack<T,CONT>::top() const {
	return _copyable_stack.back();
}

template <typename T, template <typename, typename> class CONT>
bool tmpl_stack<T,CONT>::empty() const{
	return _copyable_stack.empty();
}


// —амый сложный метод - шаблоннй метод в шаблонном классе, 
// содержащем сложный шаблонный параметр
template <typename T, template <typename, typename> class CONT>
template <typename TCOPY,template <typename, typename> class CONT_COPY>
tmpl_stack<T, CONT>& tmpl_stack<T, CONT>::operator=( const tmpl_stack<TCOPY, CONT_COPY>& s ) {

	tmpl_stack<TCOPY, CONT_COPY>  tmp(s);
	_copyable_stack.clear();

	//  опируем почленно с приведением типа (чтобы провер€лось в момент компил€ции)
	while( !tmp.empty() ){
		_copyable_stack.push_back( static_cast<T>( tmp.top() ) );
		tmp.pop();
	}
	return *this;
}
