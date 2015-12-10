#pragma once

class static_initializer
{
public:
    // change counter here
    static_initializer();
    ~static_initializer();
protected:
    static void check_init_resourses();
    static void init_resourses();
    static void deinit_resourses();
private:
    static int hard_to_init_resourse;
    static size_t static_counter;
};
