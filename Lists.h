#pragma once
#include <iostream>
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
	BinaryNode<T>* prev = NULL;
	T data;
	BinaryNode<T>* next = NULL;
};

template<typename Ty>
void __swap(Ty& a, Ty& b) {
	Ty temp = a;
	a = b;
	b = temp;
}

// this is for debugging of dynamically declared List<> variables
#define bind_name(x) x->name = #x

// for class type use init_name() will be found in PyBasics.h

template<typename T>
class List {
public:
	using pointer_sll = SingleNode<T>*;
	using value_sll = SingleNode<T>;

	pointer_sll _head_ = NULL;
	pointer_sll _tail_ = NULL;

	using pointer_dll = BinaryNode<T>*;
	using value_dll = BinaryNode<T>;

	pointer_dll __head__ = NULL;
	pointer_dll __tail__ = NULL;

	// public variable name
	char const* name = "none";

	// all the virtual functions a Linked List should override and implement when extending this class
	virtual int size() = 0;

	// insertion removal
	virtual List& insert(T val, int index) = 0;
	virtual List& remove(int index) = 0;

	// push and pop
	virtual List& pop_back() = 0;
	virtual List& pop_front() = 0;
	virtual List& push_front(T val) = 0;
	virtual List& push_back(T val) = 0;

	// maybe possible in Double Linked List
	// virtual bool binary_search() = 0;

	// sorting and sorted insert
	virtual List& sort() = 0;
	virtual List& sorted_insert(T elem) = 0;

	// reversal
	virtual List& reverse_by_links() = 0;

	// convienient display
	virtual void show() = 0;

	// intelligent linear search
	virtual int ilinear_search(T elem) = 0;

	// linear search
	virtual int linear_search(T elem) = 0;

	// max and min element
	virtual T max() = 0;

	virtual T min() = 0;

	// debug detals info
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
		_i++;
	}

	~List() {

		if (this->_head_) {
			pointer_sll _temp_ = this->_head_;
			while (this->_head_) {
				this->_head_ = this->_head_->next;
				delete _temp_;
				_temp_ = NULL;
			}
		}

		else if (this->__head__) {
			pointer_dll __temp__ = this->__head__;
			while (this->__head__) {
				this->__head__ = this->__head__->next;
				delete __temp__;
				__temp__ = NULL;
			}
		}
		else {}
	}
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
	
	int __size__ = 0;

	__SLLBase__() = default;

	__SLLBase__(initializer_list<T> init_l) {
		typename initializer_list<T>::iterator it = init_l.begin();
		this->_head_ = new value{ NULL };
		this->_head_->data = *it;
		this->_head_->next = NULL;
		this->_tail_ = this->_head_;
		this->__size__ = 1;
		it++;
		while (it != init_l.end()) {
			pointer temp = new value{ NULL };
			temp->data = *it;
			temp->next = NULL;
			this->_tail_->next = temp;
			this->_tail_ = temp;
			++it;
			this->__size__++;
		}
	}

	int size() { return this->__size__; }

	// copy and move constructor are not needed to be made here

	//for Manual Traversing

	SingleNode<T>* begin() {
		return this->_head_;
	}

	SingleNode<T>* end() {
		return this->_tail_->next;
	}

	// the functions that directly nulls out things without deleting
	// useful w.r.t move semantics
	void null_out() { this->_head_ = NULL; this->_tail_ = NULL; this->__size__ = 0; }

	// appends in the beginning
	__SLLBase__& push_front(T val) {
		pointer temp = new value{ NULL };
		temp->data = val;
		temp->next = this->_head_;
		this->_head_ = temp;
		this->__size__++;

		return *this;
	}

	// appends at the end
	__SLLBase__& push_back(T val) {
		if (this->__size__ == 0) {
			this->_head_ = new value{ NULL };
			this->_head_->data = val;
			this->_head_->next = NULL;
			this->_tail_ = this->_head_;
			this->__size__++;
		}
		else {
			pointer temp = new value{ NULL };
			temp->data = val;
			this->_tail_->next = temp;
			this->_tail_ = temp;
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
			pointer temp = this->_head_;
			this->_head_ = this->_head_->next;
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
			pointer temp = this->_head_;

			for (int i = 1; i < this->__size__ - 1; i++) temp = temp->next;

			this->_tail_->next = NULL;
			delete this->_tail_;
			this->_tail_ = temp;
			this->_tail_->next = NULL;
			this->__size__--;
			return *this;
		}
	}

	// inserts at index, provided the index is correct
	__SLLBase__& insert(T val, int index) {
		if (index == 0) { this->push_front(val); }
		else if (index == this->__size__) { this->push_back(val); }

		else if (index > 0 and index < this->__size__) {
			pointer _left = this->_head_;

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
			pointer _left = this->_head_;

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
		pointer p = this->_head_;
		pointer q = NULL;
		pointer r = NULL;

		while (p) {
			r = q;
			q = p;
			p = p->next;
			q->next = r;
		}

		this->_head_ = q;

		return *this;
	}

	T max() {
		T _max = this->_head_->data;
		pointer temp = this->_head_;

		while (temp) {
			if (temp->data > _max) _max = temp->data;
			temp = temp->next;
		}

		return _max;
	}

	T min() {
		T _min = this->_head_->data;
		pointer temp = this->_head_;

		while (temp) {
			if (temp->data < _min) _min = temp->data;
			temp = temp->next;
		}

		return _min;
	}

	void show() {
		pointer temp = this->_head_;
		while (temp) {
			cout << temp->data << " ";
			temp = temp->next;
		}cout << endl;
	}

	void log_properties(char const* entry = "", bool log_obj_details = false, bool show_log_count = true) {
		List<T>::log_properties(entry, log_obj_details, show_log_count);
		cout << "Content: "; this->show();
		cout << "Size: " << this->__size__ << endl;
		cout << endl;
	}

	// only possible search type in an sll
	int linear_search(T elem) {
		pointer temp = this->_head_;
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
		pointer temp = this->_head_;
		pointer ttemp = NULL;
		int index = 0;

		if (temp->data == elem) {
			return index;
		}

		while (temp) {
			if (temp->data == elem) {
				ttemp->next = temp->next;
				temp->next = this->_head_;
				this->_head_ = temp;
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
			pointer t_head = this->_head_->next;
			pointer tt_head = this->_head_;
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
		pointer _right = this->_head_->next;
		pointer _left = this->_head_;
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
		// before this the List destructor will be called and this makes the SLL data cleared
		// iam gonna ensure that
		this->__size__ = 0;
	}
};

template<typename T>
class __DLLBase__ abstract : public List<T> {
	using pointer = BinaryNode<T>*;
	using value = BinaryNode<T>;

	// size has to be kept private if it gets modified by outside interference member functions will not work right
	int __size__ = 0;

public:
	__DLLBase__() = default;

	__DLLBase__(initializer_list<T> init_l) {
		typename initializer_list<T>::iterator it = init_l.begin();

		this->__head__ = new value{ NULL };
		this->__head__->data = *it;
		this->__head__->next = NULL;
		this->__head__->prev = NULL;
		this->__tail__ = this->__head__;
		this->__size__++;
		++it;
		while (it != init_l.end()) {
			pointer temp = new value{ NULL };
			temp->data = *it;
			temp->prev = NULL;
			temp->next = NULL;

			this->__tail__->next = temp;
			temp->prev = this->__tail__;
			this->__tail__ = temp;
			temp = NULL;
			++it;
			this->__size__++;
		}
	}

	// copy construc and move construct, and operator= have to be implemented but in the derived class :)

	BinaryNode<T>* begin() { return this->__head__; }

	BinaryNode<T>* end() { return this->__tail__; }

	int size() { return this->__size__; }

	// push and pop
	__DLLBase__& pop_back() {
		if (this->__size__ == 0) {}
		else {
			pointer temp = this->__tail__;
			this->__tail__ = this->__tail__->prev;
			this->__tail__->next = NULL;
			delete temp;
			temp = NULL;
			this->__size__--;
		}
		return*this;
	}

	__DLLBase__& pop_front() {
		if (this->__size__ == 0) {}
		else {
			pointer temp = this->__head__;
			this->__head__ = this->__head__->next;
			this->__head__->prev = NULL;
			delete temp;
			temp = NULL;
			this->__size__--;
		}
		return*this;
	}

	__DLLBase__& push_front(T val) {
		if (this->__size__ == 0) {
			this->__head__ = new value{ NULL };
			this->__head__->prev = NULL;
			this->__head__->data = val;
			this->__head__->next = NULL;
			// now the actual thing
			this->__tail__ = this->__head__;
			this->__size__++;
		}
		else {
			pointer temp = new value{ NULL };
			temp->prev = NULL;
			temp->data = val;
			temp->next = NULL;

			// now connecting temp to head
			temp->next = this->__head__;
			this->__head__->prev = temp;

			// making temp the head;
			this->__head__ = temp;
			this->__size__++;
		}
		return *this;
	}

	__DLLBase__& push_back(T val) {
		if (this->__size__ == 0) {
			this->__head__ = new value{ NULL };
			this->__head__->prev = NULL;
			this->__head__->data = val;
			this->__head__->next = NULL;
			// now the actual thing
			this->__tail__ = this->__head__;
			this->__size__++;
		}
		else {
			pointer temp = new value{ NULL };
			temp->prev = NULL;
			temp->data = val;
			temp->next = NULL;

			// now connecting temp to the tail
			this->__tail__->next = temp;
			temp->prev = this->__tail__;

			// making temp the tail
			this->__tail__ = temp;
			this->__size__++;
		}
		return *this;
	}

	// insertion removal
	__DLLBase__& insert(T val, int index) {
		if (index == 0) { this->push_front(val); }
		else if (index == this->__size__) { this->push_back(val); }
		else {
			pointer temp = this->__head__;
			for (int i = 0; i < index; i++) temp = temp->next;

			pointer _left = temp->prev;
			pointer _mid = new value{ NULL };
			_mid->data = val;
			_mid->next = NULL;
			_mid->prev = NULL;

			// now connecting _mids to _left
			_left->next = _mid;
			_mid->prev = _left;

			// now connecting temp to _mid
			_mid->next = temp;
			temp->prev = _mid;

			// nulling spare pointers
			_left = NULL;
			_mid = NULL;
			temp = NULL;

			this->__size__++;
		}
		return *this;
	}

	__DLLBase__& remove(int index) {
		if (index == 0) { this->pop_front(); }
		else if (index == this->__size__) { this->pop_back(); }
		else {
			pointer temp = this->__head__;
			for (int i = 0; i < index; i++) temp = temp->next;

			pointer _left = temp->prev;
			pointer _mid = temp;

			temp = temp->next;
			delete _mid;

			// connecting left to temp
			_left->next = temp;
			temp->prev = _left;

			this->__size__--;
		}
		return *this;
	}

	// bound to be implemented here accordingly
	int ilinear_search(T elem) {
		int index = 0;
		pointer temp = this->__head__;

		if (temp->data == elem) { return 0; }

		while (temp) {
			if (temp->data == elem) {
				// making temp a head
				if (temp == this->__tail__) {
					temp = temp->prev;
					this->__tail__->next = this->__head__;
					this->__head__->prev = this->__tail__;
					this->__head__ = this->__tail__;
					this->__head__->prev = NULL;
					temp->next = NULL;
					this->__tail__ = temp;
				}
				else {
					pointer _left = temp->prev;
					pointer _right = temp->next;

					// now that the left and right units have been secured
					// left will be connected to right
					_left->next = _right;
					_right->prev = _left;

					// nulling them
					_left = _right = NULL;

					// temp will be conected to head and head will be temp
					temp->prev = NULL;
					temp->next = this->__head__;
					this->__head__->prev = temp;
					this->__head__ = temp;

					temp = NULL;
				}
				// returning index
				return index;
			}
			temp = temp->next;
			index++;
		}
		return -1;
	}

	// maybe possible in Double Linked List
	bool binary_search() {
		// try implementing binary search

		return false;
	}

	// sorting and sorted insert
	__DLLBase__& sort() {
		// implement insertion sort here

		// putting the minimum element in the sorted position
		// this will make the minimum element be at first spot
		this->ilinear_search(this->min());
		// because the minmum element requires the head to be also put in the correct position
		// so making ilinear_search do the thing is better

		// putting a sentinal at last
		this->push_back(T{});

		// this part sorts (this->__head__, this->__tail__)
		pointer temp = this->__head__->next;
		while (temp->next) {
			pointer temp_next = temp->next;
			pointer current_temp = temp;
			pointer temp_prev = temp->prev;

			bool have_to_relink = false;

			while ((temp_prev) and (temp_prev->data > current_temp->data)) {
				if (temp_prev->prev) temp_prev = temp_prev->prev;
				// ensures that temp_prev is decreased to point where the current_temp is to be linked 
				have_to_relink = true;
			}

			// any relinking from the middle of the list will be handled here
			if (have_to_relink) {
				// creating the link between the left entity of 'temp' to the right entity of 'temp'
				pointer current_temp_prev = current_temp->prev;
				current_temp_prev->next = temp_next;
				temp_next->prev = current_temp_prev;
				// ensures that current_temp is delinked from the main list

				pointer temp_prev_next = temp_prev->next;

				// fitting the current_temp after temp_prev
				temp_prev->next = current_temp;
				current_temp->prev = temp_prev;

				// connecting temp_prev_next to current_temp;
				current_temp->next = temp_prev_next;
				temp_prev_next->prev = current_temp;

			}
			temp = temp->next;
		}

		// remving the sentinal
		this->pop_back();

		return *this;
	}

	__DLLBase__& sorted_insert(T elem) {

		return *this;
	}

	// reversal
	__DLLBase__& reverse_by_links() {

		return *this;
	}

	// bound to be used here
	void reverse_show() {
		pointer dl_ptr = this->__tail__;
		do {
			cout << dl_ptr->data << " ";
			dl_ptr = dl_ptr->prev;
		} while (dl_ptr);
		cout << endl;
	}

	// all of the functions below this can be inherited from List<> even the destructor
	// convienient display
	void show() {
		pointer dl_ptr = this->__head__;
		while (dl_ptr) {
			cout << dl_ptr->data << " ";
			dl_ptr = dl_ptr->next;
		}cout << endl;
	}

	// linear search
	int linear_search(T elem) {
		int index = 0;
		pointer temp = this->__head__;
		while (temp) {
			if (temp->data == elem) {
				return index;
			}
			temp = temp->next;
			index++;
		}
		return -1;
	}

	// max and min element
	T max() {
		T max = this->__head__->data;
		pointer temp = this->__head__;
		while (temp) {
			if (temp->data > max) {
				max = temp->data;
			}
			temp = temp->next;
		}
		return max;
	}

	T min() {
		T min = this->__head__->data;
		pointer temp = this->__head__;
		while (temp) {
			if (temp->data < min) {
				min = temp->data;
			}
			temp = temp->next;
		}
		return min;
	}

	// debug detals info
	void log_properties(char const* entry = "", bool log_obj_details = false, bool show_log_count = true) {
		List<T>::log_properties(entry, log_obj_details, show_log_count);
		cout << "Content: "; this->show();
		cout << "Size: " << this->__size__ << endl;
		cout << endl;
	}

	~__DLLBase__() {}
};

template<typename T>
class DLList : public __DLLBase__<T> {
	using pointer = BinaryNode<T>*;
	using value = BinaryNode<T>;

public:
	DLList() : __DLLBase__<T>() {}
	DLList(initializer_list<T> init_l) : __DLLBase__<T>(init_l) {}

	// copy, move constructs, and assignments to be implmented here

	~DLList() {}
};

// Singly Linked List
template<typename T>
class SLList : public __SLLBase__<T> {
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
	SLList(const SLList& obj){
		if (obj.__size__ > 0) {
			this->_head_ = new value{ NULL };
			this->_head_->data = obj._head_->data;
			this->_head_->next = NULL;
			this->_tail_ = this->_head_;
			this->__size__++;

			pointer obj_pt = obj._head_->next;
			while (obj_pt) {
				pointer temp = new value{ NULL };
				temp->data = obj_pt->data;
				temp->next = NULL;
				this->_tail_->next = temp;
				this->_tail_ = temp;
				obj_pt = obj_pt->next;
				this->__size__++;
			}
		}
		else {
			this->_head_ = NULL;
			this->_tail_ = NULL;
			this->__size__ = 0;
		}
	}

	// move construct
	SLList(SLList&& obj) {
		this->_head_ = obj._head_;
		this->_tail_ = obj._tail_;
		this->__size__ = obj.__size__;
		obj.null_out();
	}

	// for lhs values
	SLList& operator= (const SLList& obj) { 
		// clearing old memory
		while (this->_head_) {
			pointer temp = this->_head_;
			this->_head_ = this->_head_->next;
			delete temp;
		}

		this->__size__ = 0;

		if (obj.__size__ > 0) {
			this->_head_ = new value{ NULL };
			this->_head_->data = obj._head_->data;
			this->_head_->next = NULL;
			this->_tail_ = this->_head_;
			this->__size__++;

			pointer obj_pt = obj._head_->next;
			while (obj_pt) {
				pointer temp = new value{ NULL };
				temp->data = obj_pt->data;
				temp->next = NULL;
				this->_tail_->next = temp;
				this->_tail_ = temp;
				obj_pt = obj_pt->next;
				this->__size__++;
			}
		}
		else {
			this->_head_ = NULL;
			this->_tail_ = NULL;
			this->__size__ = 0;
		}

		return *this;
	}

	// for rhs values
	SLList& operator=(SLList&& obj) noexcept {
		this->_head_ = obj._head_;
		this->_tail_ = obj._tail_;
		this->__size__ = obj.__size__;
		obj.null_out();
		return *this;
	}

	// utility functions
	SLList& concat(SLList<T> sll) {
		//cout << "_tail_->data: " << _tail_->data << endl;
		//cout << "_tail_->next: " << _tail_->next << endl;
		//cout << "obj->_head_->data: " << sll._head_->data << endl;
		pointer temp = sll._head_;
		this->_tail_->next = temp;
		this->_head_;
		this->_tail_ = sll._tail_;
		this->__size__ += sll.__size__;

		// since we already have a copy of the linked list we can assign it to ours
		// and null out the copy so it doesn't delete the memory referenced by its 
		// pointers _head_, _tail_ on the heap while getting destroyed
		sll.null_out();

		return *this;
	}

	SLList& merge(SLList<T> sll) {
		// using another linked list
		SLList<T> temporary_sll;

		pointer this_pointer = this->_head_;
		pointer sll_pointer = sll._head_;

		// merging starts

		while (this_pointer and sll_pointer) {
			if (this_pointer->data <= sll_pointer->data) {
				temporary_sll.push_back(this_pointer->data);
				this_pointer = this_pointer->next;
			}
			else {
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
		this->_head_ = temporary_sll._head_;
		this->_tail_ = temporary_sll._tail_;
		this->__size__ = temporary_sll.__size__;

		// because the owner ship has been moved to 'this'
		temporary_sll.null_out();
		// nulling out prevents the destructor from clearing the memory on heap referenced by temporary_sll

		return *this;
	}

	// tests the linked list for loops
	bool has_loops() {
		pointer p = this->_head_;
		pointer q = this->_head_->next;

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
			pointer _left = this->_head_;
			pointer _right = this->_head_->next;

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
		pointer temp = this->_head_;
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
		temp = this->_head_;
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
	void straighten() { if (this->_tail_) this->_tail_->next = NULL; }
	void circularize() { this->_tail_->next = this->_head_; }

	// a circular linked list should not be empty
	CSLList() = delete;

	CSLList(initializer_list<T> init_l) : __SLLBase__<T>(init_l) {}

	// copy construct
	CSLList(const CSLList& obj) : __SLLBase__<T>(obj) {}

	// move construct
	CSLList(CSLList&& obj) {
		this->_head_ = obj._head_;
		this->_tail_ = obj._tail_;
		this->__size__ = obj.__size__;
		obj.null_out();
	}

	// for lhs values
	CSLList& operator= (const CSLList& obj) {
		this->~CSLList();

		if (obj.__size__ > 0) {
			this->_head_ = new value{ NULL };
			this->_head_->data = obj._head_->data;
			this->_head_->next = NULL;
			this->_tail_ = this->_head_;
			this->__size__++;

			pointer obj_pt = obj._head_->next;
			while (obj_pt) {
				pointer temp = new value{ NULL };
				temp->data = obj_pt->data;
				temp->next = NULL;
				this->_tail_->next = temp;
				this->_tail_ = temp;
				obj_pt = obj_pt->next;
				this->__size__++;
			}
		}

		else {
			this->_head_ = NULL;
			this->_tail_ = NULL;
			this->__size__ = 0;
		}

		return *this;
	}

	// for rhs values
	CSLList& operator=(CSLList&& obj) noexcept {
		this->_head_ = obj._head_;
		this->_tail_ = obj._tail_;
		this->__size__ = obj.__size__;
		obj.null_out();
		return *this;
	}

	void test_circularity() {
		// triggers an infinite loop
		this->circularize();
		pointer temp = this->_head_;
		while (temp) {
			cout << temp->data << " ";
			temp = temp->next;
		}
	}

	~CSLList() {}
};

