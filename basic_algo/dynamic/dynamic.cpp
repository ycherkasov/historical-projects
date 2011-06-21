#include <iostream>
#include <string>
#include <map>
#include <set>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <iosfwd>

using std::string;
using std::cout;
using std::endl;

/// Определите только один из дефайнов!!!

//#define _MONEY_
#define _FROGS_

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
    : _steps(steps), way() {


        for (int i = 0;i < SIZE;i++) {

            // инициализация стартовых условий
            if (i < steps) {

                // совсем стартовые условия
                if (i < 2){
                    way[i].ways = 1;
                    cout << "debug: " << way[i].ways << endl;
                }
                else{
                    // если кол-во шагов больше 2, надо поставить начальные условия
                    // (проинициализаировать первые STEPS ячеек)
                    way[i].ways =  last_count(i, i) ;
                }
            }

            // инициализация "лягушек"
            if(frg.find(i) != frg.end()){
                way[i].frog = true;
                way[i].ways = 0;
            }

            // инициализация монет

        }
        
        cout << "debug initial" << endl;
        print_path();
    }

    void print_path() {
        for (size_t i = 0;i < SIZE;i++) {
            cout << '\t' << way[i].ways;
            if (way[i].frog) cout << "F";
        }
        cout << endl;
    }

    /** @ brief Сложить предыдущие STEPS значений */
    unsigned last_count(size_t index, size_t steps) {
        unsigned uns = 0;

        //cout << "debug index: " << index << endl;
        for (size_t i = index - steps;i < index;i++)
            uns += way[i].ways;

        //cout << "debug uns: " << uns << endl;
        return uns;
    }

    /** @ brief Посчитать кол-во путей */
    unsigned path_count() {

        // первые STEPS-1 ячеек уже проинициализированы, начинаем со STEPS
        for (size_t i = _steps;i < SIZE;i++) {

            if (way[i].frog) {
                way[i].ways = 0;
            }
            else {
                way[i].ways = last_count(i, _steps);
            }
        }
        return way[LAST].ways;
    }

private:

    /** @ brief Класс столбик*/
    struct column {

        column() : ways(), money(), frog(false) { }

        /** @ brief Количество путей, чтобы допрыгать до столбика */
        unsigned ways;

        /** @ brief Количество монет, которые дают или отбирают */
        int money;

        /** @brief Если ли на столбике лягушка */
        bool frog;
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

std::pair<int, int> string_token(const std::string & ip_port) {
    // we should tokenize string "IP:PORT" into "IP" and "PORT"
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("-");
    tokenizer tokens(ip_port, sep);
    std::vector<string> srvstrings(tokens.begin(), tokens.end());
    std::pair<int, int> column;

    // if tokenized sucessfully
    if (srvstrings.size() == 2) {
        column.first = boost::lexical_cast<int>(srvstrings.at(0));
        column.second = boost::lexical_cast<int>(srvstrings.at(1));
    }
    else {
        std::stringstream errstream;
        errstream << "Error parse string " << ip_port;
        throw std::runtime_error(errstream.str());
    }
    return column;
}

void parse_coins(std::map<int, int>& changeme, int argc, char* argv[]) {

    for (int i = 2;i < argc;i++) {
        changeme.insert(string_token(argv[i]));
    }
}

void parse_frogs(std::set<int>& changeme, int argc, char* argv[]) {
    std::cout << "debug " << argc << endl;
    for (int i = 2;i < argc;i++) {
        int f = boost::lexical_cast<int>(argv[i]);
        std::cout << "Frog at " << f << " column" << endl;
        changeme.insert(f);
    }
}

int main(int argc, char* argv[]) {


    std::set<int> frogs;
    std::map<int, int> coins;

#ifdef _FROGS_
    if (argc < 3) {
        std::cout << "Usage: program <stepsize> <frognum1> <frognum2> ... " << std::endl;
    }
    parse_frogs(frogs, argc, argv);
#endif

#ifdef _MONEY_
    if (argc < 3) {
        std::cout << "Usage: program <stepsize> <frognum1> <column-count> ... " << std::endl;
    }
#endif

    try {

        std::string step_str(argv[1]);
        grasshopper g(boost::lexical_cast<unsigned>(step_str), frogs, coins);
        cout << g.path_count() << " ways to reach " << endl;
        g.print_path();

    }
    catch (const boost::bad_lexical_cast& e) {
        std::cout << e.what();
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }

    return 0;
}
