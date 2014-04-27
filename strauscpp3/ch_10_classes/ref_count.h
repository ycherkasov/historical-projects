#pragma once
#include <cstring>

// See Meyers 2-29
// Ref count string example
class rc_string{
public:

	// constructor from char, crates a new string
	explicit rc_string(const char* val) :_value(new string_value(val)){}

	// copy constructor increments ref counter
	rc_string(const rc_string& rhs) :
		_value((rhs._value->_shareable) ? (rhs._value) : (new string_value(rhs._value->_data))){
		// attach new value
		if (rhs._value->_shareable)
			++_value->_ref_count;
	}

	~rc_string(){
		// cleanup if this refcount == 0
		check_delete();
	}

	rc_string& operator=(const rc_string& rhs){
		if (_value == rhs._value)
		{
			return *this;
		}

		if (rhs._value->_shareable)
		{
			// cleanup if this refcount == 0
			check_delete();

			// attach new value
			_value = rhs._value;
			++_value->_ref_count;
		}
		else
		{
			_value = new string_value(rhs._value->_data);
		}

		return *this;
	}

	// accessor - just return value
	const char& operator[](size_t index) const {
		return _value->_data[index];
	}

	// mutator - detach new value
	char& operator[](size_t index) {
		 if (_value->_ref_count > 1)
		 {
			 --_value->_ref_count;
			 _value = new string_value(_value->_data);
		 }
		 // we could save a pointer to returned object
		 // so it should be detached next time
		 _value->_shareable = false;
		 return _value->_data[index];
	}

protected:
	void check_delete(){

		if (0 == --_value->_ref_count){
			delete _value;
		}
	}
private:


	struct string_value{
		string_value(const char* init_val) :
		_ref_count(1), 
		_data( new char[strlen(init_val)+sizeof(char)] ),
		_shareable(true){
			strcpy(_data, init_val);
		}

		~string_value(){ delete[] _data; }
		size_t _ref_count;
		char* _data;
		bool _shareable;
	};

	string_value* _value;
};

// Basic class for ref count
class rc_object
{
public:
	rc_object(){}
	rc_object(const rc_object&){}
	virtual  ~rc_object() = 0;

	void add_ref();
	void remove_ref();
	void mark_unshareable();

	bool is_shared() const;
	bool is_shareable() const;
protected:
	
private:
	size_t _ref_count;
	bool _shareable;
};