#pragma once

// didn't inplemented anywhere

#define _USE_EXPORT

#ifdef _USE_EXPORT
#define EXPORT export
#else
#define EXPORT export
#endif // _USE_EXPORT

//export
//template <typename T> 
//void print_typeof (T const&);


//EXPORT
#if 0
export template <typename T>
class separation_model
{
public:
	separation_model(const T& t);
	~separation_model(void);
	T test() const;
private:
	T _x;
};
#endif // 0
