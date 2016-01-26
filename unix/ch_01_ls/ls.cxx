#include <iostream>
#include <stdexcept>
extern "C"{
    #include <dirent.h>
}

// example to demonstrate dirent API as simple 'ls'

using std::exception;

class dir_reader{
public:
  dir_reader(const char* dir){
    if( NULL == (_dp = opendir(dir) ) ){
      throw std::logic_error("Unable to open directory");
    }
  }
  void list(){
    while( NULL != (_dirp = readdir(_dp)) ){
	std::cout << _dirp->d_name << std::endl;
    }  
  }
  ~dir_reader(){
    closedir(_dp);
  }
private:
  DIR* _dp;
  dirent* _dirp;
};

int main(int ac, char* av[]){

  if(ac != 2){
    std::cout << "Usage: catalog name" << std::endl;
    return 0;
  }	

  dir_reader r(av[1]);
  r.list();
  return 0;
}

