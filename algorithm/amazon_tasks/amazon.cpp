#include <deque>
#include <numeric> 

/** @brief 
Amazon test
Object keeps track of the average of last N numbers in the stream
*/
class RunningAverage{
public:
	/** @param buffer_size - stream size
	*/
	explicit RunningAverage(size_t buffer_size)
		: _buffer_size(buffer_size), _use_prev_calc(false), _current_total(){} 

	/** @param number - add this number to the stream
	Pop the first number in the stream in case of overflow
	*/
	void newNumber(long number){
		if( (_buffer.size() + 1) > _buffer_size){
			_buffer.pop_front();
			_buffer.push_back(number);
			_use_prev_calc = false;
		}
		else{
			_buffer.push_back(number);
		}
		

		if(false == _use_prev_calc){
			_current_total = std::accumulate(_buffer.begin(), _buffer.end(), 0);
			_use_prev_calc = true;
		}
		else{
			_current_total = _current_total + number;
		}
	}

	/** @return Average of all numbers kept in the stream
	In case of empty stream throws an exception
	*/
	long getAverage() const {
		if(_buffer.empty()){
			throw std::runtime_error("There are no numbers in the stream yet");
		}

		return _current_total/_buffer.size();
	}
private:
	std::deque<long> _buffer;
	const size_t _buffer_size;
	mutable bool _use_prev_calc;
	mutable long _current_total;
};

void test_amazon(){
	RunningAverage r(3); // object should keep track of the average of the last three numbers in the stream

	long retval = 0;
	//r.getAverage(); // should throw an exception, because there are no numbers in the stream yet
	r.newNumber(10); // number 10 has arrived in the stream
	retval = r.getAverage(); // should return 10
	r.newNumber(20);
	retval = r.getAverage(); // should return 15
	r.newNumber(30);
	retval = r.getAverage(); // should return 20
	r.newNumber(40);
	retval = r.getAverage();
}

/////////////////////////////////////////////////////////////////////////
// 2nd task

const size_t H = 4;
const size_t W = 7;



void explore(int sea[H][W], int islands[H][W], int x, int y, int L, int* islands_num)
{
	if ((islands[x][y] == 0) && (sea[x][y] == 1))
	{
		islands[x][y] = L;
		if (x > 0)
			explore(sea, islands, (x - 1), y, L, 0);
		if (x < W - 1)
			explore(sea, islands, (x + 1), y, L, 0);
		if (y > 0)
			explore(sea, islands, x, (y - 1), L, 0);
		if (y < H - 1)
			explore(sea, islands, x, (y + 1), L, 0);
		if (islands_num) 
			++(*islands_num);
	}
}

void test_amazon2()
{
	int islands_num = 0;

	int sea[H][W] = { 
	{ 0, 0, 0, 0, 0, 1, 0 },
	{ 1, 0, 0, 1, 1, 1, 0 },
	{ 1, 0, 1, 1, 1, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1 } };

	int islands [4][7] = {};

	int L = 0;
	for (size_t y = 0; y < H; ++y){
		for (size_t x = 0; x < W; ++x){
			explore(sea, islands, x, y, L++, &islands_num);
		}
	}

	std::cout << islands_num << std::endl;

}

int main(){

	test_amazon();
	test_amazon2();	
	return 0;
}