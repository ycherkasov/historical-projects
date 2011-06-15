#include <iostream>
#include <string>
#include <map>
#include <set>
#include <boost/lexical_cast.hpp>
#include <iosfwd>

using std::string;
using std::cout;
using std::endl;

/// Определите только один из дефайнов!!!

#define _SIMPLE_
//#define _MONEY_
//#define _FROGS_

/*
 * Задача о кузнечике - изветсная задача на динамическое программирование
 * Путь есть SIZE-1 столбцов, по которым может прыгать кузнечик 
 * Он может прыгать от 1 до STEPS столбцов (задается в ком. строке)
 * Задача определяет количество различных вариантов путей от 1 до N столбика
 * В усложненном варианте присутствуют лягушки (нельзя прыгать на столбик)
 * и монетки (вычислить оптимальный путь)
 * 
 */

// Пример:
// 1-5 номера столба. Можно передвитаться на 1 и 2.
// 1   2   3   4   5
// |---|---|---|---|
// 1   1   2   3   5
// количество различных способов (в данном примере равно соотв. числу Фибоначчи)

typedef std::set<int> frog_cols;
typedef std::map<int, int> coins_cols;

class grasshopper {
public:

    grasshopper(unsigned steps, frog_cols& frg, coins_cols& ccl)
        :_steps(steps),way(){

        // совсем стартовые условия
        way[0].ways = 1;
        way[1].ways = 1;
        
        // если кол-во шагов больше 2, надо поставить начальные условия
        // (проинициализаировать первые STEPS ячеек)
        if(steps > 2){
            //cout << "debug steps > 2" << endl;
            for(size_t i = 2 ; i < steps ; i++){

#ifdef _FROG_
                if(frg.find(i) != ccl.end()){
                    way[i].ways = 0;
                }
                else{
                    way[i].ways = last_count(i, i);
                }
#endif
                // например, 2-я ячейка, сложить последние 2 (0 и 1)
                way[i].ways = last_count(i, i);
#ifdef _MONEY_
                if(ccl.find(i) != ccl.end()){
                    way[i].money = ccl.at(i);
                }
#endif
            }
        }

        //cout << "debug initial" << endl;
        print_path();
    }

    void print_path(){
        for(size_t i = 0 ; i < SIZE ; i++){
            cout << '\t' << way[i].ways ;
        }
        cout << endl;
    }

    /** @ brief Сложить предыдущие STEPS значений */
    unsigned last_count(size_t index, size_t steps){
        unsigned uns = 0;
        //cout << "debug index: " << index << endl;
        for(size_t i = index - steps ; i < index ; i ++)
            uns += way[i].ways;
        //cout << "debug uns: " << uns << endl;
        return uns;
    }

    /** @ brief Посчитать кол-во путей */
    unsigned path_count() {

        // первые STEPS-1 ячеек уже проинициализированы, начинаем со STEPS
        for(size_t i = _steps ; i < SIZE ; i++){
            way[i].ways = last_count(i, _steps);
        }
        return way[LAST].ways;
    }

private:

    /** @ brief Класс столбик*/
    struct column{
        column():ways(),money(){}
        
        /** @ brief Количество путей, чтобы допрыгать до столбика */
        unsigned ways;

        /** @ brief Количество монет, которые дают или отбирают */
        int money;
    };
    
    /// на сколько можем прыгать вперед
    unsigned _steps;

    /// размер пути
    const static int SIZE = 10;

    /// индекс последнего столбика
    const static int LAST = SIZE - 1;

    /// здесь храним весь путь
    column way[SIZE];
};

void parse_frogs(){}
void parse_coins(){}

int main(int argc, char* argv[]) {


    std::set<int> frogs;
    std::map<int, int> coins;
    
#ifdef _FROGS_
    if (argc < 3) {
        std::cout << "Usage: program <stepsize> <frognum1> <frognum2> ... " << std::endl;
    }
#endif

#ifdef _MONEY_
    if (argc < 3) {
        std::cout << "Usage: program <stepsize> <frognum1> <column-count> ... " << std::endl;
    }
#endif


#ifdef _SIMPLE_
    if (argc != 2) {
        std::cout << "Usage: program <stepsize>" << std::endl;
    }
#endif


    try {

        std::string step_str(argv[1]);
        grasshopper g(boost::lexical_cast<unsigned>(step_str), frogs, coins);
        cout << g.path_count() << " ways to reach " << endl;
        g.print_path();

    }
    catch (const boost::bad_lexical_cast& e) {}
    catch (const std::exception& e) {}
    
    return 0;
}
