#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>

// template function for simple message output
template <class messageType>
void pOL(messageType message){
	std::cout << message;
}

/*
Base class "storage" has private: messages for display, and private container
for answer from user;

public containers, methods and destructor: destructor simply send "come again"
message, boolean "approved" for validating input from user, methods for send
messages input validity or result, boolean "mode" for initilizing mode of
program - number research or range research, also created a virtual empty
methods for overloading it in the derrived class and using that overloaded
methods by the base class pointer to derrived class.
*/
class storage{

    // There is no need to share this containers and methods with derrived
    // class and main program
    private:
    // messages represent as strings, they kept private because doesn't uses
    // in other parts of programm
	std::string modeHeader = "\nChoose your goal of research: ";
    std::string incorrect = "\nThis input is incorrect.\n";
    std::string comeAgain = "\nThat's all for now, run again.\n";
    
    // char container for answer letter from user kept stored privetly
	char answer;	
	
	// Main program need this containers and methods
	public:
	// true - for number, false - for range
	bool mode = true;
	// approved = true - correct input, false - incorrect input
	bool approved = true;

	// Method that show to user options of the program
	void openDialog(){
		// Move to dialog header additional messages
		modeHeader += "\n\n(s/S) - for varifing single number";
		modeHeader += " being Mersenne prime,";
		modeHeader += "\n(r/R) - for showing all Mersenne primes";
		modeHeader += " in the given range.\n\n";
		// output message for user
		pOL(modeHeader);
		// read mode from user
		std::cin >> answer;
	}
    // Method print corresponding message about "correct" or "incorrect" input
	void validateDialog(){
		// Setting range-search mode
		if ( (answer == 's') || (answer == 'S') ) mode = true;
		// Setting range-search mode
		else if ( (answer == 'r') || (answer == 'R') ) mode = false;
		// Not allow to program work when input invalid
		else approved = false;
		// Immidiately send error message in case wrong input
		if (!approved) pOL(incorrect);
	}
	// Prepared virtual empty method for collecting number for research
	virtual void getNumber(){}
	// Prepared virtual empty method for collecting range for research
	virtual void getRange(){}
	// Virtual empty method for sening result message if base class pointer
	// will initilize it - because of keyword "virtual" derrived class overload
	// method closeDialog() will be used	
	virtual void closeDialog(){}

	// Destructor simply send message to user asking run program again 
	~storage(){
		pOL(comeAgain);
	}
};

/*
Derrived class numberResearcher contain:
private messages for user, container for collecting number, clock_t variables
for counting time;

protected containers - vector for prime numbers, boolean for primes and time
exeception error, method getPrimes() push prime numbers before gived end point,
method checkPrime() find devisors for given number, if didn't find - ti's prime,
method checkMNumber() return status of number being Mersenne - this method need
privious checking number to be prime;

public methods such as getNumber() for collecting number, closeDialog() for
sending result to user and bunch of empty methods openDialog(), validateDialog()
and virtual getRange() that accessible from main program. Some methods empty
because they is overloads base non-virtual methods so main program will call
base methods.
*/
class numberResearcher : public storage
{
    // There is no need to share this containers with derrived class
    // and main program
    private:
    // messages represent as strings for number research dialogs
    std::string enterNumber = "\nEnter number for research: ";
    std::string MersennePrime = "\nYou enter a Mersenne prime number.\n";
    std::string notPrime = "\nIt's not a prime number.\n";
    std::string justPrime = "\nIt's just a prime number, but not Mersenne((\n";
	
	// integer container for collected number from user
	unsigned long long int number;
	// clock_t container keeps start time process of prime checking
	clock_t prime_start;
	// clock_t container keeps paused time process of prime checking
	clock_t prime_pause;
	
	// research method with bool return shared with range research class
	protected:
	// integer vector container for prime numbers for given length
	std::vector<unsigned long long int> pVec;
	// bool container, true - prime number, false - not prime
	bool prime_status = false;
	// bool container, true - time error, false - everything fine
	bool time_failure = false;

	// Method calculate all primes for end point
	void getPrimes(unsigned long long int point){
		// temporary container
		unsigned long long int k;
		// bool container, true - number is prime
		bool result;
		// first number in the container
		pVec.push_back(2);
		// calculate prime numbers with odd divisors
		for (unsigned long long int i=1; (2*i+1) <= (point/2); i++){
			result = true;
			k = 2*i + 1;
			// first divise number with prime that alreay in the list
			for (std::vector<unsigned long long int>::iterator it=pVec.begin();
			     ( ( it != pVec.end() ) && ( *it <= (k/2) ) && result); it++){
			     		// if something left from division this not prime
			     		if ( !( k % (*it) ) ) result = false;
			     	}
			// then find divisors from other odd numbers
			for (unsigned long long int j = pVec.back();
			     ( j <= (k/2) ) && result; ){
				// if something left from division this not prime
				if ( !( k % j ) ) result = false;
				// to the next odd number
				j += 2;
			}
			// puch to vector only primes
			if ( result ) pVec.push_back(k);
		}
	}

	// Method check if number is prime
	void checkPrime(unsigned long long int x){
	    // starting point of time count
	    prime_start = clock();
	    // initial value of status - false - not prime
		prime_status = false;
		// prime number
		if ( x == 2 ){
			prime_status = true;
			return;
		}
		// every even number are not prime
		if ( !(x % 2) ){
			prime_status = false;
			return;
		}
		// divise with every odd number until find devisor,
		// time exceeded or numbers exceeded
		for (unsigned long long int i = 1; (2*i+1)<=(x/2); i++){
	        // pause time count for verifying with time limit
	        prime_pause = clock() - prime_start;
	        // anything bigger than 1 second is failure
			if ( prime_pause*1.0/CLOCKS_PER_SEC > 1 ){
				time_failure = true;
				return;
			}
			// in case finding devisor stop cycle
			if  ( !( x % (2*i+1) ) ){
				prime_status = false;
				return;
			}
		}
		// at this point every number is prime
	    prime_status = true;
	    return;
	};

	// Method return true in case number being prime
	bool checkMNumber(unsigned long long int x){
	    // temporary double container with power needs to recieve inputed x
	    // result will be whole in case Marsenne number
	    double c = log(x+1)/log(2);
	    // result will be true if to numbers equal
	    return !((int)c-c);
	};

	// Main program need this methods
	public:
	// Empty method for calling bace class openDialog()
	void openDialog(){}
	// Empty method for calling bace class validateInput()
	void validateDialog(){}
	// Method collect number from user
	void getNumber(){
		// Send simple message to user
		pOL(enterNumber);
		std::cin >> number;
	}
	// Prepared virtual empty method for collecting range for research
	virtual void getRange(){}
	// Method sends result to user
	void closeDialog(){
		// Only here research method calls for this class
		//getPrimes(number);
		checkPrime(number);
		if (prime_status){
			if (checkMNumber(number)) pOL(MersennePrime);
			else pOL(justPrime);
		}
		else pOL(notPrime);
	}
};

/*
Derrived class rangeResearcher contain:
private method for operate with inherited checkPrime(), messages for user,
containers for collecting range and vector for lab numbers in the range,
clock_t variables for counting time;

public methods such as getRange() for collecting number, closeDialog() for
sending result to user and bunch of empty methods openDialog() and
validateDialog() that accessible from main program. Some methods empty because
they is overloads base non-virtual methods so main program will call base
methods.
*/
class rangeResearcher : public numberResearcher
{
    // There is no need to share this containers and methods with derrived
    // class and main program
	private:
	// messages represent as strings for range research dialogs
    std::string enterLrange = "\nEnter first number in the range: ";
    std::string enterRrange = "\nEnter last number in the range: ";
    std::string MersenneRange = "\nAll Mersenne primes in the given range [";
    std::string limitRange = "\nAll Mersenne primes in the corrected range [";
    std::string back = "] are:\n\n{ ";
    std::string notMe = "\nNot a single Mersenne prime in the given range((\n";
    std::string limit = "\nCalculations ended with time limit error.\n";

	// integer container for begin of range
	unsigned long long int lRange;
	// integer container for end of range
	unsigned long long int rRange;
	// integer container for new end of range in case time limit error
	unsigned long long int LIMITrRange;
	// integer vector container for all Mersenne prime numbers in the range
	std::vector<unsigned long long int> mVec;
	// clock_t container keeps start time process of range checking
	clock_t start_operate;
	// clock_t container keeps paused time process of range checking
	clock_t time_new;	

	// Method called operate() and keep Mersenne prime number
	void operate(unsigned long long int start, unsigned long long int end){
		// temporary container keep new iteration of checked number
	    unsigned long long int next = start;
	    // calculate new "candidate" by Mersenne formula
	    for (int i = log(start+1)/log(2);  ; i++){
	    	// next calculated number with power i
	        next = pow(2,i) - 1;
	        // break when end limit exceeded
	        if ( next > end ) break;
			// need to check if this prime number
	        checkPrime(next);
	        // pause clock and count time of loop break if limit resieved
	        time_new = clock() - start_operate;
			if ( time_new*1.0/CLOCKS_PER_SEC > 1 )
				time_failure = true;
			// keep new end of range in case time exeception
	        if ( time_failure ){
	        	LIMITrRange = pow(2,(i-1)) - 1;
	        	break;
	        }
	        // keep old end of range if not
	        else LIMITrRange = rRange;
	        // push Mersenne primes to vector until limit exceeded
	        if ( prime_status && (next >= start) ) mVec.push_back(next);
	    }
	}

	// Main program need this methods
	public:
	// Empty method for calling bace class openDialog()
	void openDialog(){}
	// Empty method for calling bace class validateInput()
	void validateDialog(){}
	// Method collect range from user
	void getRange(){
		pOL(enterLrange);
		std::cin >> lRange;
		pOL(enterRrange);
		std::cin >> rRange;
	}
	// Method sends result to user
	void closeDialog(){
		// starting point of time count operating
		start_operate = clock();
		// research in the range starts here
		operate(lRange, rRange);
		// send empty result vector to user
		if (mVec.empty()) pOL(notMe);
		else {
			// send find any lab number message to user
			if (time_failure){
				pOL(limit);
				pOL(limitRange);
			}
			else pOL(MersenneRange);
			pOL(lRange);
			pOL(',');
			pOL(' ');
			pOL(LIMITrRange);
			pOL(back);
			// printing by vector iterator in the for-loop
			for (std::vector<unsigned long long int>::iterator it = mVec.begin();
			     it != mVec.end(); it++){
				pOL(*it);
				pOL(' ');
			}
			pOL("}.\n");
		}
	}
};

// Main program starting point
int main() {

	// declaring clock variable
	clock_t t;
	// Initiliazing pointer to base class
	storage *entry = new storage;
	// Initiliazing dialog with user
	entry -> openDialog();
	// Initiliazing analisis of input from user
	entry -> validateDialog();

	// single-number mode
	if ( (entry -> approved) && (entry -> mode) )
	{
		// Initiliazing pointer to numberResearcher class
		storage *check = new numberResearcher;
		// Initiliazing collecting number from user 
		check -> getNumber();
		// start clock
		t = clock();
		// Closing dialog with user 
		check -> closeDialog();
	}

	// range of the numbers mode
	if ( (entry -> approved) && !(entry -> mode) )
	{
		// Initiliazing pointer to rangeResearcher class
		storage *check = new rangeResearcher;
		// Initiliazing collecting range from user
		check -> getRange();
		// start clock
		t = clock();
		// Closing dialog with user 
		check -> closeDialog();
	}

	// getting time mesuare
	t = clock() - t;

	// send elapsed time for calculating to user
	pOL("\nTime spend program working: ");
	pOL(t*1.0/CLOCKS_PER_SEC);
	pOL(" seconds.\n");

	// deleting base class pointer
	delete entry;

    return 0;
}
