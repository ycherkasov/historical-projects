#pragma once

class derived_a;
class derived_b;

class base
{
public:
	base();
	virtual ~base() = 0;

	virtual void multiop( const base& b );
	virtual void multiop( const derived_a& b );
	virtual void multiop( const derived_b& b );
};

class derived_a : public base{
public:
	virtual void multiop( const base& b );
	virtual void multiop( const derived_a& b );
	virtual void multiop( const derived_b& b );
};

class derived_b : public base{
public:
	virtual void multiop( const base& b );
	virtual void multiop( const derived_a& b );
	virtual void multiop( const derived_b& b );
};
