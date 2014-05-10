#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <deque>
#include <iterator>
#include <fstream>

//#define RESULT(x) print_result(x)
#define RESULT(x) push_result(x)

class sliding_window_minimum_test{

public:
    sliding_window_minimum_test(const char* in_filename, const char* out_filename)
        : _window_size()
        , _array_size()
        , _input_filename(in_filename)
        , _output_filename(out_filename){}

    void check_consistency(){
        if( (_array_size < 1 || _array_size > 150000) ||
            (_window_size < 1 || _window_size > 10000) ){
                exit(1);
        }
    }

    void read_file_data(){
        std::vector< std::vector<int> > all_integers;
        // поток ввода связать с файлом источника
        std::ifstream from_file( _input_filename );
        std::string line;
        while ( getline( from_file, line ) ) {
            std::istringstream is( line );
            all_integers.push_back( 
                std::vector<int>( std::istream_iterator<int>(is),
                std::istream_iterator<int>() ) );
        }

        if(all_integers.size() != 2){
            exit(1);
        }

        if(all_integers[0].size() != 2 ){
            exit(1);
        }

        
        _array_size = all_integers[0][0];
        _window_size = all_integers[0][1];
        if(all_integers[1].size() != _array_size){
            exit(1);
        }

        if(_array_size < _window_size){
            exit(1);
        }
            
        _array.swap(all_integers[1]);
    }

    void read_test_data(){
        int a[] = {1, 3, 2, 3, 4, 5, 3, 1};
        _array_size = sizeof(a)/sizeof(a[0]);
        _array.assign(a, a + _array_size);
        _window_size = 3;
    }

    void post_process(){
        if(_result_array.empty()) return;
        int min = _result_array.front();
        for(int i = 0 ; i < _window_size ; i++){
            if(min > _result_array.front()){
                min = _result_array.front();
            }
            _result_array.pop_front();
        }
        _result_array.push_front(min);
    }

    void iterate(){
        read_file_data();
        //read_test_data();
        check_consistency();
        sliding_window_minimum(_array);
        post_process();
        write_result();
        
    }

    inline void push_result(int result){
        _result_array.push_back(result);
    }

    inline void print_result(int result){
        std::cout << result << ' ';
    }


    void write_result(){
        std::ofstream to_file( _output_filename );
        if(!to_file.is_open()) return;
        std::ostream_iterator<int> to_it_begin(to_file, "\n");
        std::copy(_result_array.begin(), _result_array.end(), to_it_begin);
    }

    void sliding_window_minimum(const std::vector<int>& arr) {
        std::deque< std::pair<int, unsigned> > min_pairs;
        if(arr.size() < _array_size)
            return;
        for (unsigned i = 0; i < _array_size; i++) {

            // Удалить из очереди все элементы, больше чем входящий элемент
            while (!min_pairs.empty() && min_pairs.back().first >= arr.at(i))
                min_pairs.pop_back();

            // Добавить элемент в хвост очереди, включая его индекс выхода из окна
            min_pairs.push_back(std::make_pair(arr.at(i), i + _window_size));

            // Удалить голову очереди, если она вышла за пределы окна
            while((!min_pairs.empty()) && (min_pairs.front().second <= i) )
                min_pairs.pop_front();

            if(!min_pairs.empty())
                push_result(min_pairs.front().first);
        }
    }

private:
    std::vector<int> _array;
    std::deque<int> _result_array;
    int _window_size;
    int _array_size;
    const char* _input_filename;
    const char* _output_filename;
};

int main(){
    sliding_window_minimum_test t("input.txt", "output.txt");
    t.iterate();

    return 0;
}

