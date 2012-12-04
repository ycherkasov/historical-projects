#ifndef	DEFINE_STRUCTREDEXCEPTION_H
#define	DEFINE_STRUCTREDEXCEPTION_H
#include <windows.h>
#include <eh.h>
#include <exception>
#include <stdexcept>

// В этом хедере описан класс, "заворачивающий" SEH в исключения C++

#define NOTHROW throw()
#define THROWS(X) throw(X)

// If you want your C++ exception code to catch SEH exceptions, 
// you need to build the code with /EHa.

/***	A Microsoft Windows Strucutred Exception.*/
class structured_exception : public std::runtime_error{
public:
    /**	*	Default Constructor	*/
    structured_exception(const char* const strMessage) NOTHROW;
    
    /**	*	Copy Constructor	*/
    structured_exception(const structured_exception& rhs) NOTHROW;
    
    /**	*	Destructor	*/
    virtual ~structured_exception() NOTHROW;
    
    /**	*	Assignment operator	*/
    structured_exception& operator = (const structured_exception& rhs) NOTHROW;
    
    /**	*	Enables the class to convert Windows Structured Exception. 	*/
    static void enableStructuredExceptions() NOTHROW;

private:
    static void handlerStructuredException(unsigned int, EXCEPTION_POINTERS*) THROWS(structured_exception);
};
#endif	//	DEFINE_STRUCTREDEXCEPTION_H//
