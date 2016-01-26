#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <deque>
#include <iterator>
#include <fstream>
#include <map>
#include <algorithm>

using namespace std;
#define MAXINT 200000000

struct village{
	village(int ind):coordinate(ind), distance_left(MAXINT), distance_right(MAXINT), closest_left(-1), closest_right(-1){}
	int coordinate;
	int closest_left;
	int closest_right;
	int distance_left;
	int distance_right;
	int closest(){return (distance_left<distance_right)?(closest_left):(closest_right);}
};



class vault{
public:
	vault():_village_size(), _vault_size(){}

	void read_file_data(){
		std::vector< std::vector<int> > all_integers;
		// поток ввода связать с файлом источника
		std::ifstream from_file( "input.txt" );
		std::string line;
		while ( getline( from_file, line ) ) {
			std::istringstream is( line );
			all_integers.push_back( 
				std::vector<int>( std::istream_iterator<int>(is),
				std::istream_iterator<int>() ) );
		}

		if(all_integers.size() != 4){
			exit(0);
		}

		if(all_integers[0].size() != 1 ){
			exit(0);
		}
		if(all_integers[2].size() != 1 ){
			exit(0);
		}


		_village_size = all_integers[0][0];
		//_villages.reserve(_village_size);
		_vault_size = all_integers[2][0];
		//_vaults.reserve(_vault_size);
		if(all_integers[1].size() != _village_size){
			exit(0);
		}
		if(all_integers[3].size() != _vault_size){
			exit(0);
		}

		_villages.swap(all_integers[1]);
		_vaults.swap(all_integers[3]); 
		
		int vault_no = 1;
		int minvault = -1000000;

		std::vector<int>::iterator it = _vaults.begin();
		while( it != _vaults.end() ){
			vaults_with_index.insert( std::make_pair( (*it), vault_no ) );
			++it; ++vault_no;
		}
        _vaults.push_back(minvault);
		std::sort(_vaults.begin(), _vaults.end());
		vaults_with_index[minvault] = -1;

		std::vector<int>::iterator it2 = _villages.begin();
		while( it2 != _villages.end()){
			_villages_full.push_back( village( (*it2) ) );
			++it2;
		}
	}

    void process(){
        std::vector<village>::iterator it = _villages_full.begin();
        int vault_index = 0;

        while( it != _villages_full.end() ){

            // todo : do some jobs
            while(vault_index < _vault_size){

                while((*it).coordinate <= _vaults[vault_index + 1]){
                    (*it).closest_left = vaults_with_index[ _vaults[vault_index] ]; // closest no right
                    (*it).closest_right = vaults_with_index[_vaults[vault_index + 1]];
                    (*it).distance_left = (*it).coordinate - _vaults[vault_index]; // distacnce to closest right
                    (*it).distance_right = _vaults[vault_index + 1] - (*it).coordinate;
                    ++it;
                }
                vault_index++;
            }

            //////////////////////////////////////////////////////
            while( (*it).coordinate > _vaults[vault_index] ){
                (*it).closest_left = vaults_with_index[ _vaults[vault_index] ]; // closest no right
                (*it).distance_left = (*it).coordinate - _vaults[vault_index]; // distacnce to closest right
                ++it;
                if( it == _villages_full.end()) break;
            }

        }
    }
		
	void print(){
        ofstream myfile;
        myfile.open ("output.txt");
		std::vector< village >::iterator it = _villages_full.begin();
		while(it != _villages_full.end()){
			myfile << (*it).closest() << ' ';
			++it;
		}
        myfile.close();
	}

private:
	int _village_size;
	int _vault_size;
	std::vector<int> _vaults;
	std::vector<int> _villages;
	std::vector<village> _villages_full;
	std::map<int, int> vaults_with_index;
};

int main(int argc, char* argv[]) {
	vault v;
	v.read_file_data();
	v.process();
	v.print();
	return 0;
}
