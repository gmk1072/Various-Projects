#include <memory>
#include <iostream>
#include <map>

std::map<void*, int> refCount;//void* = just an address

template <typename p>
class UniqPtr
{
private:
	p* ptr = nullptr;
public:
	UniqPtr() {}//construct
	~UniqPtr() {
		std::cout << "Destructing unique pointer" << std::endl;
		if (ptr) {//the only thing that = 0(false) is when ptr is a nullptr
			std::cout << "Deleting unique pointer" << std::endl;
			delete ptr;
		}
	}//destruct...
	UniqPtr(p* _ptr) {
		ptr = _ptr;
	}
	UniqPtr(UniqPtr<p>& other) {  //& in parameter gets a reference
		ptr = new p;
		*ptr = *(other.ptr);
	}//copy constructor      
	p* get() { return ptr; }
	p& operator*() { return *ptr; }//p& passes the reference, not the value
};

template <typename p>
class SharedPtr
{
private:
	p* ptr = nullptr;
public:
	SharedPtr() {}//construct
	p* get() { return ptr; }
	p& operator*() { return *ptr; }//p& passes the reference, not the value

	~SharedPtr() {
		std::cout << "Destructing shared pointer" << std::endl;
		std::cout << "Pointer:\t" << ptr << "\tCount:\t" << refCount[ptr] << std::endl;
		if (ptr && --refCount[(void*)ptr] == 0) {//the only thing that = 0(false) is when ptr is a nullptr      //0 == --refCount[ptr] makes it so you can't by accident do --refCount[ptr] = 0
				std::cout << "Deleting shared pointer" << std::endl;
				delete ptr;
		}
	}//destruct...
	SharedPtr(p* _ptr) {
		ptr = _ptr;
		refCount[(void*)ptr]++;
	}
	SharedPtr(SharedPtr<p>& other) {  //& in parameter gets a reference
		ptr = other.ptr;
		refCount[(void*)ptr]++;
	}//copy constructor      
};

template <typename p>
class WeakPtr
{
private:
	p* ptr = nullptr;
public:
	p* get() { return ptr; }
	p& operator*() { return *ptr; }//p& passes the reference, not the value
	WeakPtr() {}//construct

	~WeakPtr() {
		std::cout << "Destructing weak pointer" << std::endl;
		std::cout << "Pointer:\t" << ptr << "\tCount:\t" << refCount[ptr] << std::endl;
	}//destruct...
	WeakPtr(p* _ptr) {
		ptr = _ptr;
	}
	WeakPtr(SharedPtr<p>& other) {  //& in parameter gets a reference
		ptr = other.get();
	}//copy constructor      
};
void testFunc()
{
	//testing raw pointers, will crash if both are deleted
	/*
	{//creates a section of scope
		int* ptr1 = new int;
		*ptr1 = 10;
		int* ptr2 = ptr1;
		std::cout << std::endl << "print ptr1: " << *ptr1 << std::endl;
		*ptr2 = 20;
		std::cout << std::endl << "ptr2 = 20" << std::endl;
		std::cout << std::endl << "print ptr1: " << ptr1 << ":\t" << *ptr1 << std::endl << "Print ptr2: " << ptr2 << ":\t" << *ptr2 << std::endl;

		delete ptr1;
		std::cout << "deleting ptr1" << std::endl;
		delete ptr2;
		std::cout << "deleting ptr2" << std::endl;
		//char someVar;
	}
	//someVar can't be reference out here since it is out of scope
	/**/


	//testing custom unique pointer
	/*
	{
		UniqPtr<int> ptr1(new int);
		*ptr1 = 10;

		UniqPtr<int> ptr2(ptr1);
		//std::cout << std::endl << "print ptr1: " << *ptr1 << std::endl;
		*ptr2 = 20;
		
		std::cout << std::endl << "print ptr1: " << ptr1.get() << ":\t" << *ptr1 << std::endl << "Print ptr2: " << ptr2.get() << ":\t" << *ptr2 << std::endl;
	}
	/**/

	//testing custom shared pointer
	/*
	{
		SharedPtr<int> ptr1(new int);
		*ptr1 = 10;

		SharedPtr<int> ptr2(ptr1);
		//std::cout << std::endl << "print ptr1: " << *ptr1 << std::endl;
		*ptr2 = 20;

		std::cout << std::endl << "print ptr1: " << ptr1.get() << ":\t" << *ptr1 << std::endl << "Print ptr2: " << ptr2.get() << ":\t" << *ptr2 << std::endl;
	}
	/**/

	//testing custom weak pointer
	/**/
	{
		SharedPtr<int> ptr1(new int);
		*ptr1 = 10;

		WeakPtr<int> ptr2(ptr1);
		*ptr2 = 20;

		std::cout << std::endl << "print ptr1: " << ptr1.get() << ":\t" << *ptr1 << std::endl << "Print ptr2: " << ptr2.get() << ":\t" << *ptr2 << std::endl;
	}
	/**/
	std::cout << "done." << std::endl;
}

int main()
{
	testFunc();
	// "Gavin Keirstead"
	//std::map<char, int> charCount;
	//charCount['g'] = 1;
	//charCount['a'] = 2;
	//charCount['v'] = 1;
	//charCount['i'] = 2;
	//charCount['n'] = 1;
	//charCount['k'] = 1;
	//charCount['e'] = 2;
	//charCount['r'] = 1;
	//charCount['s'] = 1;
	//charCount['t'] = 1;
	//charCount['d'] = 1;

	//std::cout << " there are " << charCount['n'] << " number of n's" << std::endl;

	return 0;
}