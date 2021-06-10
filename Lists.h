#pragma once
#include <iostream>
#include <algorithm>
using namespace std;

// Single Node
template<typename T>
struct SingleNode {
	T data;
	SingleNode<T>* next = NULL;
};

// Binary Node
template <typename T>
struct BinaryNode {
	BinaryNode<T>* prev;
	T data;
	BinaryNode<T>* next;
};

template<typename Ty>
void __swap(Ty& a, Ty& b) {
	Ty temp = a;
	a = b;
	b = temp;
}

// this is for debugging of dynamically declared List<> variables
#define bind_name(x) x->name = #x

// for class type use init_name()

template<typename T>
class List abstract {
public:
	// public variables
	char const* name = "";
	virtual int size() = 0;
	virtual void log_properties(char const* entry = "", bool log_obj_details = false, bool show_log_count = true) = 0;
	// insertion removal
	virtual List& insert(T val, int index) = 0;
	virtual List& remove(int index) = 0;
	// push and pop
	virtual List& pop_back() = 0;
	virtual List& pop_front() = 0;
	virtual List& push_front(T value) = 0;
	virtual List& push_back(T value) = 0;
	// linear search
	virtual int linear_search(T elem) = 0;
	virtual int ilinear_search(T elem) = 0;
	// maybe possible in Double Linked List
	// virtual bool binary_search() = 0;
	virtual List& sort() = 0;
	virtual List& reverse_by_links() = 0;
	virtual void show() = 0;
};

// Abstract Base Class for Single Linked List, will be extended by Singly Linked List, and Circular Single Linked list
template<typename T>
class __SLLBase__ abstract : public List<T> {
	
	friend ostream& operator<<(ostream& os, __SLLBase__<T>& sll) {
		sll.show();
		return os;
	}

public:

	using pointer = SingleNode<T>*;
	using value = SingleNode<T>;

	pointer __head__ = NULL;
	pointer __tail__ = NULL;
	int __size__ = 0;
	__SLLBase__() = default;

	__SLLBase__(initializer_list<T> init_l) {
		typename initializer_list<T>::iterator it = init_l.begin();
		this->__head__ = new value{ NULL };
		this->__head__->data = *it;
		this->__head__->next = NULL;
		this->__tail__ = this->__head__;
		this->__size__ = 1;
		it++;
		while (it != init_l.end()) {
			pointer temp = new value{ NULL };
			temp->data = *it;
			temp->next = NULL;
			this->__tail__->next = temp;
			this->__tail__ = temp;
			++it;
			this->__size__++;
		}
	}

	int size() { return this->__size__; }

	// copy and move constructor are not neede to be made here

	/* base class will provide 
		insertion 
		deletion
		sorting
		searching
		displaying
		push_front
		push_back
		pop_front
		pop_back
	*/

	//for Manual Traversing

	SingleNode<T>* begin() {
		return this->__head__;
	}

	SingleNode<T>* end() {
		return this->__tail__->next;
	}

	// the functions that directly nulls out things without deleting
	// useful w.r.t move semantics
	void null_out() { this->__head__ = NULL; this->__tail__ = NULL; this->__size__ = 0; }

	// appends in the beginning
	__SLLBase__& push_front(T val) {
		pointer temp = new value{ NULL };
		temp->data = val;
		temp->next = this->__head__;
		this->__head__ = temp;
		this->__size__++;

		return *this;
	}

	// appends at the end
	__SLLBase__& push_back(T val) {
		if (this->__size__ == 0) {
			this->__head__ = new value{ NULL };
			this->__head__->data = val;
			this->__head__->next = NULL;
			this->__tail__ = this->__head__;
			this->__size__++;
		}
		else {
			pointer temp = new value{ NULL };
			temp->data = val;
			this->__tail__->next = temp;
			this->__tail__ = temp;
			this->__size__++;
		}
		return *this;
	}

	// deletes from beginning
	__SLLBase__& pop_front() {
		if (this->__size__ == 0) {
			return *this;
		}
		else {
			pointer temp = this->__head__;
			this->__head__ = this->__head__->next;
			delete temp;
			this->__size__--;
			return *this;
		}
	}

	// deletes from end
	__SLLBase__& pop_back() {
		if (this->__size__ == 0) {
			return *this;
		}
		else {
			pointer temp = this->__head__;

			for (int i = 1; i < this->__size__ - 1; i++) temp = temp->next;

			this->__tail__->next = NULL;
			delete this->__tail__;
			this->__tail__ = temp;
			this->__tail__->next = NULL;
			this->__size__--;
			return *this;
		}
	}

	// inserts at index, provided the index is correct
	__SLLBase__& insert(T val, int index) {
		if (index == 0) { this->push_front(val); }
		else if (index == this->__size__) { this->push_back(val); }

		else if (index > 0 and index < this->__size__) {
			pointer _left = this->__head__;

			pointer insertee = new value{ NULL };
			insertee->data = val;
			insertee->next = NULL;

			for (int i = 0; i < index - 1; i++) _left = _left->next;
			//cout << _left->data << endl;

			pointer _right = _left->next;

			_left->next = insertee;
			insertee->next = _right;

			this->__size__++;
		}
		return *this;
	}

	// removes the value et the provided index
	__SLLBase__& remove(int index) {
		if (index == this->__size__) { this->pop_back(); }
		else if (index == 0) { this->pop_front(); }

		else if (index > 0 and index < this->__size__) {
			pointer _left = this->__head__;

			for (int i = 0; i < index - 1; i++) _left = _left->next;

			// cout << _left->data << endl;
			pointer _mid = _left->next;
			// cout << _mid->data << endl;
			pointer _right = _mid->next;
			// cout << _right->data << endl;

			_left->next = _right;
			delete _mid;
			this->__size__--;
		}
		return *this;
	}

	__SLLBase__& reverse_by_links() {
		// using sliding pointers
		pointer p = this->__head__;
		pointer q = NULL;
		pointer r = NULL;

		while (p) {
			r = q;
			q = p;
			p = p->next;
			q->next = r;
		}

		this->__head__ = q;

		return *this;
	}

	T max() {
		T _max = this->__head__->data;
		pointer temp = __head__;

		while (temp) {
			if (temp->data > _max) _max = temp->data;
			temp = temp->next;
		}

		return _max;
	}

	T min() {
		T _min = this->__head__->data;
		pointer temp = __head__;

		while (temp) {
			if (temp->data < _min) _min = temp->data;
			temp = temp->next;
		}

		return _min;
	}

	void show() {
		pointer temp = this->__head__;
		while (temp) {
			cout << temp->data << " ";
			temp = temp->next;
		}cout << endl;
	}

	void log_properties(char const* entry = "", bool log_obj_details = false, bool show_log_count = true) {
		static int _i = 0;

		if (show_log_count)
			cout << "Log Record Count: " << _i << endl;

		if (entry != "")
			cout << "Debug Entry: " << entry << endl;

		if (log_obj_details) {
			cout << "Object Address: " << this << endl;
			cout << "Object Type: " << typeid(*this).name() << endl;
		}

		cout << "Object Name: " << this->name << endl;
		cout << "Content: "; this->show();
		cout << "Size: " << this->__size__ << endl;
		cout << endl;
		_i++;
	}

	// only possible search type in an sll
	int linear_search(T elem) {
		pointer temp = this->__head__;
		int index = 0;
		while (temp) {
			if (temp->data == elem) {
				return index;
			}
			temp = temp->next;
			index++;
		}
		return -1;
	}

	// searches and puts the search result in the begining
	int ilinear_search(T elem) {
		pointer temp = this->__head__;
		pointer ttemp = NULL;
		int index = 0;

		if (temp->data == elem) {
			return index;
		}

		while (temp) {
			if (temp->data == elem) {
				ttemp->next = temp->next;
				temp->next = __head__;
				__head__ = temp;
				return index;
			}
			ttemp = temp;
			temp = temp->next;
			index++;
		}
		return -1;
	}

	// bubble sort, since insertion sort requires bidirectional iterators
	__SLLBase__& sort() {
		// bubble sort
		for (int i = 0; i < this->__size__; i++) {
			pointer t_head = this->__head__->next;
			pointer tt_head = this->__head__;
			for (int j = 0; j < this->__size__ - i - 1; j++) {
				if (t_head and (t_head->data < tt_head->data)) {
					__swap(t_head->data, tt_head->data);
				}
				t_head = t_head->next;
				tt_head = tt_head->next;
			}
		}
		return *this;
	}

	// assuming linked list is already sorted
	__SLLBase__& sorted_insert(T elem) {
		pointer _right = this->__head__->next;
		pointer _left = this->__head__;
		while (_right) {
			if (_right->data > elem) break;
			_right = _right->next;
			_left = _left->next;
		}

		pointer temp = new value{ NULL };
		temp->data = elem;
		temp->next = NULL;

		_left->next = temp;
		temp->next = _right;

		this->__size__++;
		return *this;
	}

	~__SLLBase__() {
		while (this->__head__) {
			SingleNode<T>* q = this->__head__;
			__head__ = __head__->next;
			delete q;
		}
		this->__size__ = 0;
	}
};

template<typename T>
class __DLLBase__ abstract : public List<T>{
	using pointer = BinaryNode<T>*;
	using value = BinaryNode<T>;

	pointer __head__ = NULL;
	pointer __tail__ = NULL;
	int __size__ = 0;

	__DLLBase__() = default;

	__DLLBase__(initializer_list<T> init_l) {

	}

	~__DLLBase__() {}
};

// Singly Linked List
template<typename T>
class SLList : public __SLLBase__<T>{
	using pointer = SingleNode<T>*;
	using value = SingleNode<T>;
public:
	SLList() : __SLLBase__<T>() {}

	SLList(initializer_list<T> init_l) : __SLLBase__<T>(init_l) {}

	int size() { return this->__size__; }

	/*
	Base class already provides all the methods and facilities
	so here only those functions will be implemented which require a parameter of same type
	like copy and move construct
	concat and merge
	operator=
	*/

	// copy construct
	SLList(const SLList& obj) {

		if (obj.__size__ > 0) {
			this->__head__ = new value{ NULL };
			this->__head__->data = obj.__head__->data;
			this->__head__->next = NULL;
			this->__tail__ = this->__head__;
			this->__size__++;

			pointer obj_pt = obj.__head__->next;
			while (obj_pt) {
				pointer temp = new value{ NULL };
				temp->data = obj_pt->data;
				temp->next = NULL;
				this->__tail__->next = temp;
				this->__tail__ = temp;
				obj_pt = obj_pt->next;
				this->__size__++;
			}
		}
		else {
			this->__head__ = NULL;
			this->__tail__ = NULL;
			this->__size__ = 0;
		}
	}

	// move construct
	SLList(SLList&& obj) {
		this->__head__ = obj.__head__;
		this->__tail__ = obj.__tail__;
		this->__size__ = obj.__size__;
		obj.null_out();
	}

	// for lhs values
	SLList& operator= (const SLList& obj) {
		this->~SLList();

		if (obj.__size__ > 0) {
			this->__head__ = new value{ NULL };
			this->__head__->data = obj.__head__->data;
			this->__head__->next = NULL;
			this->__tail__ = this->__head__;
			this->__size__++;

			pointer obj_pt = obj.__head__->next;
			while (obj_pt) {
				pointer temp = new value{ NULL };
				temp->data = obj_pt->data;
				temp->next = NULL;
				this->__tail__->next = temp;
				this->__tail__ = temp;
				obj_pt = obj_pt->next;
				this->__size__++;
			}
		}
		else {
			this->__head__ = NULL;
			this->__tail__ = NULL;
			this->__size__ = 0;
		}

		return *this;
	}

	// for rhs values
	SLList& operator=(SLList&& obj) noexcept{
		this->__head__ = obj.__head__;
		this->__tail__ = obj.__tail__;
		this->__size__ = obj.__size__;
		obj.null_out();
		return *this;
	}

	// utility functions
	SLList& concat(SLList<T> sll) {
		//cout << "__tail__->data: " << __tail__->data << endl;
		//cout << "__tail__->next: " << __tail__->next << endl;
		//cout << "obj->__head__->data: " << sll.__head__->data << endl;
		pointer temp = sll.__head__;
		this->__tail__->next = temp;
		this->__head__;
		this->__tail__ = sll.__tail__;
		this->__size__ += sll.__size__;

		// since we already have a copy of the linked list we can assign it to ours
		// and null out the copy so it doesn't delete the memory referenced by its 
		// pointers __head__, __tail__ on the heap while getting destroyed
		sll.null_out();

		return *this;
	}

	SLList& merge(SLList<T> sll) {
		// using another linked list
		SLList<T> temporary_sll;
		
		pointer this_pointer = this->__head__;
		pointer sll_pointer = sll.__head__;

		// merging starts

		while (this_pointer and sll_pointer) {
			if (this_pointer->data <= sll_pointer->data) {
				temporary_sll.push_back(this_pointer->data);
				this_pointer = this_pointer->next;
			}else {
				temporary_sll.push_back(sll_pointer->data);
				sll_pointer = sll_pointer->next;
			}
		}

		while (this_pointer) {
			temporary_sll.push_back(this_pointer->data);
			this_pointer = this_pointer->next;
		}

		while (sll_pointer) {
			temporary_sll.push_back(sll_pointer->data);
			sll_pointer = sll_pointer->next;
		}

		// merging finished

		// deleting both lists properly
		this->~SLList();		// because it 'this' gets the owner ship of temporary_sll
		sll.~SLList();			// because it was a copy of passed SLList 

		// reinitializing
		this->__head__ = temporary_sll.__head__;
		this->__tail__ = temporary_sll.__tail__;
		this->__size__ = temporary_sll.__size__;

		// because the owner ship has been moved to 'this'
		temporary_sll.null_out();		
		// nulling out prevents the destructor from clearing the memory on heap referenced by temporary_sll

		return *this;
	}

	// tests the linked list for loops
	bool has_loops() {
		pointer p = this->__head__;
		pointer q = this->__head__->next;

		while (q) {
			if (p == q) return true;
			p = p->next;
			q = q->next;
			q = q ? q->next : q;
		}

		return false;
	}

	// removes duplicates in a sorted list
	SLList& remove_duplicates() {
		if (this->__size__ > 1) {
			pointer _left = this->__head__;
			pointer _right = this->__head__->next;

			while (_right) {
				if (_left->data == _right->data) {
					pointer mid = _right;
					_right = _right->next;
					delete mid;
					_left->next = _right;
					this->__size__--;
				}
				else {
					_left = _left->next;
					_right = _right->next;
				}
			}
		}
		return *this;
	}

	// stores the elements and reverses them,then reassigns
	// however it is a malpractice as the elements can be a whole record of a student
	// and it can prove to be costly(memory-wise) to swap elements
	SLList& reverse_by_elements() {
		// storing list data in an array
		T* list_data = new T[this->__size__]{};
		int i = 0;
		pointer temp = this->__head__;
		while (temp) {
			list_data[i++] = temp->data;
			temp = temp->next;
		}

		/*cout << "collected data" << endl;
		for (i = 0; i < this->__size__; i++) {
			cout << list_data[i] << endl;
		}*/

		for (i = 0; i < this->__size__ / 2; i++) {
			__swap(list_data[i], list_data[this->__size__ - i - 1]);
		}

		/*cout << "reversed data" << endl;
		for (i = 0; i < this->__size__; i++) {
			cout << list_data[i] << endl;
		}*/

		i = 0;
		temp = this->__head__;
		while (temp) {
			temp->data = list_data[i++];
			temp = temp->next;
		}

		delete[] list_data;
		list_data = nullptr;

		return *this;
	}

	// __SLLBase__ class will take care of deleting the data
	~SLList() {}
};

// Circular Singly Linked List
/* 
In original state the CSLL wil be SLL, it will become circular once circularize() is called
but as a counter to circularize, straighten function is also provided, also circularise and straighten 
should be used in pairs. 
for example
	to print the content using an external pointer to the list it can be done this way

	CSLList<int> csl {3,4,5,2,4};			// creating an object
	csl.circularize();						// circularising the list
	SingleNode<T>* temp = csl.begin();		// initializing an external pointer by begin() 
	do {									// since in CSLList the head is the only pointer, end() also returns head so we use do while loop
		cout << temp->data << " ";			// using the loop to traverse all data
		temp = temp->next;
	}while (temp != csl.end());
	csl.straighten();						// straightening the data as it will cause failures during destruction of object
*/
template<typename T>
class CSLList : public __SLLBase__<T> {
	using pointer = SingleNode<T>*;
	using value = SingleNode<T>;
public:
	void straighten() { if (this->__tail__) this->__tail__->next = NULL; }
	void circularize() { this->__tail__->next = this->__head__; }

	// a circular linked list should not be empty
	CSLList() = delete;

	CSLList(initializer_list<T> init_l) : __SLLBase__<T>(init_l) {}

	// copy construct
	CSLList(const CSLList& obj) {
		if (obj.__size__ > 0) {
			this->__head__ = new value{ NULL };
			this->__head__->data = obj.__head__->data;
			this->__head__->next = NULL;
			this->__tail__ = this->__head__;
			this->__size__++;

			pointer obj_pt = obj.__head__->next;
			while (obj_pt) {
				pointer temp = new value{ NULL };
				temp->data = obj_pt->data;
				temp->next = NULL;
				this->__tail__->next = temp;
				this->__tail__ = temp;
				obj_pt = obj_pt->next;
				this->__size__++;
			}
		}
		else {
			this->__head__ = NULL;
			this->__tail__ = NULL;
			this->__size__ = 0;
		}
	}

	// move construct
	CSLList(CSLList&& obj) {
		this->__head__ = obj.__head__;
		this->__tail__ = obj.__tail__;
		this->__size__ = obj.__size__;
		obj.null_out();
	}

	// for lhs values
	CSLList& operator= (const CSLList& obj) {
		this->~CSLList();

		if (obj.__size__ > 0) {
			this->__head__ = new value{ NULL };
			this->__head__->data = obj.__head__->data;
			this->__head__->next = NULL;
			this->__tail__ = this->__head__;
			this->__size__++;

			pointer obj_pt = obj.__head__->next;
			while (obj_pt) {
				pointer temp = new value{ NULL };
				temp->data = obj_pt->data;
				temp->next = NULL;
				this->__tail__->next = temp;
				this->__tail__ = temp;
				obj_pt = obj_pt->next;
				this->__size__++;
			}
		}

		else {
			this->__head__ = NULL;
			this->__tail__ = NULL;
			this->__size__ = 0;
		}

		return *this;
	}

	// for rhs values
	CSLList& operator=(CSLList&& obj) noexcept {
		this->__head__ = obj.__head__;
		this->__tail__ = obj.__tail__;
		this->__size__ = obj.__size__;
		obj.null_out();
		return *this;
	}

	void test_circularity() {
		// triggers an infinite loop
		this->circularize();
		pointer temp = this->__head__;
		while (temp) {
			cout << temp->data << " ";
			temp = temp->next;
		}
	}

	~CSLList() {}
};

