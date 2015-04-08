#pragma once

namespace meyers_proxy {
// See Meyers 2-30
// Ref count string with char-proxy object example


// Basic class for ref count *value* (!)
class rc_object
{
public:
	// owner should set correct ref_count!
	rc_object() :_ref_count(0), _shareable(true){}

	// owner should set correct ref_count!
	rc_object(const rc_object&) :_ref_count(0), _shareable(true){}

	rc_object& operator=(const rc_object&){
		// refcount value should not be really assigned
		// wrapping objects assigned instead
		return *this;
	}

	// could not create sepatately
	virtual  ~rc_object() = 0;

	void add_ref(){
		++_ref_count;
	}

	void remove_ref(){
		check_clear();
	}

	// modifying operation is performed, references to representation could exist
	void mark_unshareable(){
		_shareable = false;
	}

	// object is already shared
	bool is_shared() const {
		return _ref_count > 1;
	}

	// whether performed modifying operations
	bool is_shareable() const{
		return _shareable;
	}

	// cleanup if all references were removed
	void check_clear(){
		if (--_ref_count == 0)
			delete this;
	}
protected:

private:
	size_t _ref_count;
	bool _shareable;
};

// pure virtual destructor should be defined
rc_object::~rc_object(){}

// smart pointer for saving ref count value
// should be contained in wrapping object (not in value!)
template <typename T>
class rc_ptr
{
public:
	rc_ptr(T* ptr = nullptr) :_ptr(ptr){
		re_init();
	}

	rc_ptr(const rc_ptr& rhs) :_ptr(rhs._ptr){
		re_init();
	}

	rc_ptr& operator=(const rc_ptr& rhs){

		// check self
		if (_ptr == rhs._ptr){
			return *this;
		}

		// 
		if (_ptr){
			T* old_ptr = _ptr;
			_ptr = rhs._ptr;
			re_init();

			if (old_ptr){
				old_ptr->remove_ref();
			}

		}

		return *this;
	}

	~rc_ptr(){
		if (_ptr)
			_ptr->remove_ref();
	}

	T* operator->() const {
		return _ptr;
	}

	T& operator*() const {
		return *_ptr;
	}
protected:
	void re_init(){

		if (_ptr == nullptr){
			return;
		}

		// unable to use shared copy anymore
		// detach value
		if (false == _ptr->is_shareable()){
			_ptr = new T(*_ptr);
		}

		// new reference 
		// (nevermind for the new or existing object)
		_ptr->add_ref();
	}
private:

	// T should point to rc_object-derived instance 
	// and provide copy-constructor
	T* _ptr;
	// Smart pointer could even contain a holder for library classes
	// see Meyers 2-29
};



// Ref count usage
class rc_string2
{
	// all refcount logics was moved to base class
	// value class MUST provide copy constructor!
	// Also that class is internal, so that noone 
	// accidentally created it on the stack
	// It uses 'delete this' and could be created
	// in the heap only!
	struct string_value : public rc_object{

		// init value
		string_value(const char* init_val) :
		_data(new char[strlen(init_val) + sizeof(char)]){

			strcpy(_data, init_val);
		}

		// copy value - only after modifying operations
		string_value(const string_value& rhs) :
			_data(new char[strlen(rhs._data) + sizeof(char)]){

			strcpy(_data, rhs._data);
		}

		~string_value(){
			delete[] _data;
		}

		char* _data;
	};

	class char_proxy
	{
	public:
		char_proxy(rc_string2& str, size_t index) :
			_holder(str),
			_index(index){}

		// In case of 'lvalue usage' we chack detach
		// It means we are able to change the value
		char_proxy& operator=(const char_proxy& rhs){
			check_detach();
			_holder._value->_data[_index] = rhs._holder._value->_data[rhs._index];
			return *this;
		}

		// Same as previous operator
		char_proxy& operator=(char rhs){
			check_detach();
			_holder._value->_data[_index] = rhs;
			return *this;

		}

		void check_detach(){

			if (_holder._value->is_shared()){
				_holder._value = new string_value(_holder._value->_data);
			}
		}

		// after dereference we can change the value outside
		// Make unsgareable
		char* operator&(){
			check_detach();
			_holder._value->mark_unshareable();
			return &(_holder._value->_data[_index]);
		}

		// Const dereference version, unable to change outside
		const char* operator&() const{
			return &(_holder._value->_data[_index]);
		}

		// In case or 'rvalue usage' just cast to char
		// It means that we are not going to change the value
		operator char() const{
			return _holder._value->_data[_index];
		}
	private:
		rc_string2& _holder;
		size_t _index;
	};
public:

	friend class char_proxy;

	rc_string2(const char* init_val) : _value(new string_value(init_val)){}

	const char_proxy operator[](size_t index) const {
		return char_proxy(const_cast<rc_string2&>(*this), index);
	}

	char_proxy operator[](size_t index){
		return char_proxy(*this, index);
	}

private:


	rc_ptr<string_value> _value;
};

} //namespace meyers_proxy