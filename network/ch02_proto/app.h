/* 
 * File:   app.h
 * Author: ycherkasov
 *
 * Created on 13 Январь 2011 г., 15:59
 */

#ifndef APP_H
#define	APP_H

#include <string>
#include <map>

enum app_mode_t {
    mode_client,
    mode_server
};

/**@brief The only application class */
class app {

public:

    /**@brief Maps cmd-line param string to enum */
    typedef std::map<std::string, app_mode_t> mode_map_t;

    /**@brief The only construcor */
    app();

    /**@brief Init cmd-line params  */
    void parse_command_line(int argc, char **argv);

    /**@brief Init parameter maps */
    void init_maps();

    /**@brief Application main loop */
    void start();

    /**@brief Help */
    void usage();

private:
    app(const app& orig);
    app& operator=(const app& orig);

private:

    /**@brief Application params from command line or config */
    struct app_params {
        bool _help;
        app_mode_t _mode;
        std::string _proto;
        std::string _addr;
        std::string _service;
        std::string _network_handler;
    }_params;

    /**@brief Get mode enum from string param */
    app_mode_t get_mode_value(const char* s);

    /**@brief Client/Server modes */
    mode_map_t _modes;
};

#endif	/* APP_H */

