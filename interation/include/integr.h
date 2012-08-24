#ifndef _integr_h_
#define _integr_h_

#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>
#include <boost/random/lagged_fibonacci.hpp>
#include <boost/random/uniform_real_distribution.hpp>
// todo: replace with more concrete headers
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <iostream>

class num_integrte{
public:
    typedef double (*function_t)(double);

    num_integrte(function_t func):_function(func){}

    double rectangle_integrate(double a, double b, size_t steps){

        // todo: close enough
        if(a == b){
            return 0;
        }

        double begin = (b>a)?a:b;
        double end = (b>a)?b:a;

        double interval = (end - begin)/steps;
        double half_int = interval/2;
        double result = 0.;

        for(size_t i = 1 ; i < steps; i++){
            double point = begin + interval*i - half_int;
            result += f(point);
        }
        result *= interval;
        return result;
    }

    double trapeziodal_integrate(double a, double b, size_t steps){
        // todo: close enough
        if(a == b){
            return 0;
        }

        double begin = (b>a)?a:b;
        double end = (b>a)?b:a;

        double interval = (end - begin)/steps;

        double result = (f(begin) + f(end))/2;

        for(size_t i = 1 ; i < (steps-1); i++){
            double point = begin + interval*i;
            result += f(point);
        }
        result *= interval;
        return result;
    }

    double simpson_integrate(double a, double b, size_t steps){

        // todo: close enough
        if(a == b){
            return 0;
        }

        double begin = (b>a)?a:b;
        double end = (b>a)?b:a;

        double interval = (end - begin)/steps;
        double h = interval/6;

        double result = 0.;
        double coeff1 = 0.;
        double coeff2 = 0.;

        for(size_t i = 0 ; i < steps; i++){
            // x(k)
            double x_k = begin + interval*(i+1);
            // x(k-1)
            double x_k_1 = begin + interval*i;
            double point1 = x_k;
            double point2 = (x_k_1 + x_k)/2;
            coeff1 += f(point1);
            coeff2 += f(point2);
        }
        coeff1 -= f(end);
        result = h * (f(begin) + f(end) + 2*coeff1 + 4*coeff2);
        return result;
    }

protected:
    double f(double x){

        if(_function == 0){
            throw std::exception("Function is undefined");
        }

        return _function(x);
    }

// todo: accessor
protected:
    function_t _function;
private:
    num_integrte(const num_integrte&);
};


class num_integrte_mt : public num_integrte{
public:

    num_integrte_mt(function_t func) :
      num_integrte(func),
      result(),
      _thread_count(1)
      {}

    num_integrte_mt(function_t func, size_t thread_count) :
      num_integrte(func),
      result(),
      _thread_count(thread_count)
      {}

    double mt_integrate(double a, double b, size_t steps, size_t tasks){

        double begin = (b>a)?a:b;
        double end = (b>a)?b:a;
        double interval = (end - begin)/tasks;

        {
            boost::asio::io_service io;
            boost::asio::io_service::work asio_work(io);
            boost::thread_group th_group;
            for (size_t i = 0; i < _thread_count; ++i){
                th_group.create_thread(boost::bind(&boost::asio::io_service::run, &io));
            }

            for(size_t i = 0 ; i < tasks; i++){
                double x0 = begin + (interval*i);
                double x1 = begin + (interval*(i+1));
                //cout << "x" << i << " = " << x0 << ", x" << i+1 << " = " << x1 << endl;
                io.post(boost::bind(&num_integrte_mt::threaded_integrate, this, x0, x1, steps/tasks));
            }
            io.stop();
            th_group.join_all();
        }
        return result;
    }

    void threaded_integrate(double a, double b, size_t steps){
        //double subresult = rectangle_integrate(a, b, steps, func);
        //double subresult = trapeziodal_integrate(a, b, steps, func);
        double subresult = simpson_integrate(a, b, steps);
        interlocked_add<double>(result, subresult);
    }



    template <typename T>
    void interlocked_add(volatile T& val, const T add){
        boost::mutex::scoped_lock l(m);
        val += add;
    }

private:
    boost::mutex m;
    volatile double result;
    size_t _thread_count;
private:
    num_integrte_mt(const num_integrte_mt&);
};

class monte_carlo {
public:
    
    typedef double (*function_t)(double);

    monte_carlo(function_t func):_function(func){}

    bool sub_function(function_t func, double x, double y){
        double real_y = func(x);
        bool res = true;
        if(real_y < 0){
            res = ((y < 0) && (y > real_y));
        }
        else if(real_y > 0){
            res = ((y > 0) && (y < real_y));
        }
        return res;
    }

    double monte_carlo_integrate(double a, double b, double up, double low){
        // todo: close enough
        if((a == b) || (up == low)){
            return 0;
        }


        double begin = (b>a)?a:b;
        double end = (b>a)?b:a;
        size_t in_function = 0;
        size_t total = 100000;
        double square = (end-begin)*(up-low);

        boost::random::lagged_fibonacci3217 gen;
        for(size_t i = 0 ; i < total ; i++){
            boost::random::uniform_real_distribution<> dist1(begin, end);
            boost::random::uniform_real_distribution<> dist2(low, up);
            double x = dist1(gen);
            double y = dist2(gen);

            if(sub_function(_function, x, y)) {
                in_function++;
            }
        }


        double result = static_cast<double>(in_function)/total;
        return result*square;
    }
private:
    function_t _function;
};

#endif //_integr_h_