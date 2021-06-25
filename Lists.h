#ifndef _LISTS_H_
#define _LISTS_H_

#include <iostream>
#include "Nodes.h"

using namespace std;

/*
# List-Library
A Thorough Implementation of the 4 types of Linked Lists with C++ and OOP

This Library has an Inheritance based model of implementing Linked Lists.
The model looks like this

		Grand Parent:	abstract class List
		Derived:		abstract class __SLLBase__ and abstract class __DLLBase__

		Parent:			abstract class __SLLBase__
		Children:		class SLList and class CSLList (both not abstract)
						- SLList: Singly Linked List
						- CSLList: Circular Singly Linked List, also inherits __CLLBase__ interface

		Parent:			abstract class __DLLBase__
		Children:		class DLList and class CDLList (both not abstract)
						- DLList: Doubly Linked List
						- CDLList: Circular Doubly Linked List, also inherits __CLLBase__ interface

class List provides all the methods that a Linked List has to offer, of which includes:

		- insertion
		- deletion
		- sorting
		- sorted insertion
		- pop back
		- pop front
		- push back
		- push front
		- linear search
		- intelligent linear search (putting the most searched node at head)
		- min and max element
		- display to console
		- debugging info by log_properties function

More Description of the Model:

	- The derived classes __SLLBase__ and __DLLBase__ implements the function inherited from there parent List

	- __SLLBase__ and __DLLBase__ provide constructors to their derived classes, however copy and move construtors
	  are to be implemented by the derived classes themselves.

	- The List class provides two types of heads and two types of tails as public variables to their derived classes
		_head_ and _tail_	:	the head and tail for __SLLBase__ and its derivatives, respectively
		__head__ and __tail__	:	the head and tail for __DLLBase__ and its derivatives, respectively

	- The Destructors for all the derivatives is located in the List class itself, to prevent memory leaks,
	  during runtime polymorphism.

	- The 'C' or 'Circular' versions of the two types isn't implemented to be circular in its usual state,
	  circularity may not be a problem to destruct those objects but it can cause hindrance to the working
	  of other functions, rendering them a rigourous modification which defeats the purpose of the 'C'
	  versions implmented by inheritance. however, two functions i.e. circularise() and straighten() are provided
	  in their definitions and as their names suggests they should be used accordingly.

	- while using the runtime polymorphism the objects of derived class really get constrained to the functions that
	  parent class has offered which doesn't leave room for the actual benefits that the derived class offers
	  for example: concatenation, merging.
	  even the debug logging gets constrained to at most the details of object and debug entry.

*/

namespace Py {

// this is for debugging of dynamically declared List<> variables
#define bind_name(x) x->name = #x

// for class type use 
#define init_name(x) x.name = #x

	// just a utility function
	template<typename Ty>
	void __swap(Ty& a, Ty& b) {
		Ty temp = a;
		a = b;
		b = temp;
	}

//#################################################################################################################
//		Here are some Code executers that execute a particular code (which will be provided by you)
//		on each node of any type of Linked List, these are like python decorators, your code gets
//		embedded in another code and gets executed

// pitch in the typename, object and the code you want to run on SLL or CSLL
#define execute_on_sll(Type, Object, Code) for(SingleNode<Type>* sn = Object.begin(); sn != Object.end(); sn = sn->next) {Code}

// pitch in the typename, object and the code you want to run on DLL or CDLL
#define execute_on_dll(Type, Object, Code) for(BinaryNode<Type>* bn = Object.begin(); bn != Object.end(); bn = bn->next) {Code}

//#################################################################################################################
												/* Grand Parent Level */

	// Abstract Base Class for all the Linked List, that will be implmented
	template<typename T>
	class List abstract {
		using pointer_sll = SingleNode<T>*;
		using value_sll = SingleNode<T>;

		using pointer_dll = BinaryNode<T>*;
		using value_dll = BinaryNode<T>;

		friend ostream& operator<<(ostream& os, List<T>& l) {
			l.show();;
			return os;
		}

	protected:
		pointer_sll _head_ = NULL;
		pointer_sll _tail_ = NULL;

		pointer_dll __head__ = NULL;
		pointer_dll __tail__ = NULL;

	public:
		// public variable name
		char const* name = "none";

		// all the virtual functions a Linked List should override and implement when extending this class

		// returns size
		virtual int size() = 0;

		// insertion 
		virtual List& insert(T val, int index) = 0;

		// removal
		virtual T remove(int index) = 0;

		// popping an element at the back
		virtual T pop_back() = 0;

		// popping an element from front
		virtual T pop_front() = 0;

		// pushing an element at the back
		virtual List& push_front(T val) = 0;

		// pushing an element in front
		virtual List& push_back(T val) = 0;

		// type restrictions and naming conventions are causing difficulty for the implemetation
		// 	   begin()
		// 	   end()
		// 	   head()
		// 	   tail()
		// so these are bound to be contained in child classes of List

		// sorting
		virtual List& sort() = 0;

		// sorted insert
		virtual List& sorted_insert(T elem) = 0;

		// reversal
		virtual List& reverse_by_links() = 0;

		// intelligent linear search
		virtual int ilinear_search(T elem) = 0;

		// linear search
		virtual int linear_search(T elem) = 0;

		// max element
		virtual T max() = 0;

		// min element
		virtual T min() = 0;

		// a convienient display
		virtual void show() {}

		// a deallocator
		virtual List& clear() = 0;

		// debug details in runtime
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
				pointer_sll _temp_ = NULL;
				while (this->_head_) {
					_temp_ = this->_head_;
					this->_head_ = this->_head_->next;
					delete _temp_;
					_temp_ = NULL;
				}
			}
			else if (this->__head__) {
				pointer_dll __temp__ = NULL;
				while (this->__head__) {
					__temp__ = this->__head__;
					this->__head__ = this->__head__->next;
					delete __temp__;
					__temp__ = NULL;
				}
			}
			else {}
		}
	};

//#################################################################################################################
												  /* Parent Level */

	// Abstract Base Class for Single Linked List, will be extended by Singly Linked List, and Circular Single Linked list
	template<typename T>
	class __SLLBase__ abstract : public List<T> {
		using pointer = SingleNode<T>*;
		using value = SingleNode<T>;

		friend ostream& operator<<(ostream& os, __SLLBase__<T>& sll) {
			sll.show();
			return os;
		}

	public:
		int __size__ = 0;

	protected:
		void _copy_values_(pointer begin, pointer end) {

			this->_head_ = new value{ NULL };
			this->_head_->data = begin->data;
			this->_head_->next = NULL;
			this->_tail_ = this->_head_;
			this->__size__ = 1;
			begin = begin->next;

			while (begin) {
				pointer temp = new value{ NULL };
				temp->data = begin->data;
				temp->next = NULL;
				this->_tail_->next = temp;
				this->_tail_ = temp;
				begin = begin->next;
				this->__size__++;
			}
		}

	public:
		using forward_iterator = _SingleNode_Iterator_<T>;
		using const_forward_iterator = _Const_SingleNode_Iterator_<T>;

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

		_SingleNode_Iterator_<T> begin() {
			return _SingleNode_Iterator_<T>(this->_head_);
		}

		_SingleNode_Iterator_<T> end() {
			return _SingleNode_Iterator_<T>(this->_tail_->next);
		}

		_SingleNode_Iterator_<T> head() {
			return _SingleNode_Iterator_<T>(this->_head_);
		}

		_SingleNode_Iterator_<T> tail() {
			return _SingleNode_Iterator_<T>(this->_tail_);
		}

		_Const_SingleNode_Iterator_<T> cbegin() {
			return _Const_SingleNode_Iterator_<T>(this->_head_);
		}

		_Const_SingleNode_Iterator_<T> cend() {
			return _Const_SingleNode_Iterator_<T>(this->_tail_->next);
		}

	protected:
		// the functions that directly nulls out things without deleting
		// useful w.r.t move semantics
		void null_out() { this->_head_ = NULL; this->_tail_ = NULL; this->__size__ = 0; }

	public:
		// appends in the beginning
		__SLLBase__& push_front(T val) noexcept override {
			// initializing a node
			pointer temp = new value{ NULL };
			temp->data = val;
			temp->next = NULL;

			// connecting the node
			temp->next = this->_head_;
			this->_head_ = temp;

			if (this->__size__ == 0) this->_tail_ = this->_head_;

			this->__size__++;
			return *this;
		}

		// appends at the end
		__SLLBase__& push_back(T val) noexcept override {
			if (this->__size__ == 0) {
				this->_head_ = new value{ NULL };
				this->_head_->data = val;
				this->_head_->next = NULL;
				this->_tail_ = this->_head_;
			}
			else {
				pointer temp = new value{ NULL };
				temp->data = val;
				temp->next = NULL;

				this->_tail_->next = temp;
				this->_tail_ = temp;

				temp = NULL;
			}
			this->__size__++;
			return *this;
		}

		// deletes from beginning
		T pop_front() noexcept override {
			T val{};
			// empty list case covered
			if (this->__size__ == 0) {
				// do nothing
			}
			else {
				pointer temp = this->_head_;
				// head goes to head's next
				this->_head_ = this->_head_->next;
				// temp is deleted
				val = temp->data;
				delete temp;
				// nulling out temp
				temp = NULL;
				// reducing the size
				this->__size__--;
			}
			return val;
		}

		// deletes from end
		T pop_back() noexcept override {
			T val{};
			// empty list case covered
			if (this->__size__ == 0) {
				// do nothing
			}
			else {
				pointer temp = this->_head_;

				// traverses the temp pointer to one before tail
				for (int i = 1; i < this->__size__ - 1; i++) temp = temp->next;

				// nulling tail's next
				this->_tail_->next = NULL;
				val = this->_tail_->data;
				// deleting tail data
				delete this->_tail_;
				// initializing tail to temp
				this->_tail_ = temp;
				// nulling temp
				temp = NULL;
				// setting the tail's next to NULL
				this->_tail_->next = NULL;
				// reducing the size
				this->__size__--;
			}
			return val;
		}

		// inserts at index, provided the index is correct
		__SLLBase__& insert(T val, int index) noexcept override {
			// since insertion at begining and end has head and tail manipulation we transfer 
			// control to push_front or push_back accordingly
			if (index == 0) { this->push_front(val); }
			else if (index == this->__size__) { this->push_back(val); }

			// insertion in the middle will be handled here, range (this->_head_, this->_tail_);
			else if (index > 0 and index < this->__size__) {
				pointer _left = this->_head_;

				pointer insertee = new value{ NULL };
				insertee->data = val;
				insertee->next = NULL;

				// iterating _left to one before position of index
				for (int i = 0; i < index - 1; i++) _left = _left->next;

				// fetching the _right pointer
				pointer _right = _left->next;

				// connecting insertee to the list
				_left->next = insertee;
				insertee->next = _right;

				// nulling temporary pointers
				_left = _right = insertee = NULL;

				this->__size__++;
			}
			return *this;
		}

		// removes the value at the provided index
		T remove(int index)  noexcept override {
			// for boundary cases pop back and front will help
			if (index == this->__size__) { return this->pop_back(); }
			else if (index == 0) { return this->pop_front(); }

			else if (index > 0 and index < this->__size__) {
				T val{};
				pointer _left = this->_head_;

				// setting the value to one before the area of removal
				for (int i = 0; i < index - 1; i++) _left = _left->next;

				// putting the value to be deleted in _mid
				pointer _mid = _left->next;

				// putting the value after _mid in _right
				pointer _right = _mid->next;

				// connecting _left to _right
				_left->next = _right;

				val = _mid->data;
				// deleteing mid
				delete _mid;

				// nulling all the pointers
				_left = _mid = _right = NULL;
				this->__size__--;
				return val;
			}
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

			// since tail is now screwed we need to retraverse to intialize tail
			p = this->_head_;
			while (p) {
				this->_tail_ = p;
				p = p->next;
			}

			p = q = r = NULL;

			return *this;
		}

		T max()  noexcept override {
			T _max = this->_head_->data;
			pointer temp = this->_head_;

			while (temp) {
				if (temp->data > _max) _max = temp->data;
				temp = temp->next;
			}
			temp = NULL;
			return _max;
		}

		T min()  noexcept override {
			T _min = this->_head_->data;
			pointer temp = this->_head_;

			while (temp) {
				if (temp->data < _min) _min = temp->data;
				temp = temp->next;
			}
			temp = NULL;
			return _min;
		}

		void show() noexcept override {
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

		// linear search, returns first appearance
		int linear_search(T elem)  noexcept override {
			pointer temp = this->_head_;
			int index = 0;
			while (temp) {
				if (temp->data == elem) {

					// nulling before returning
					temp = NULL;
					return index;
				}
				temp = temp->next;
				index++;
			}
			temp = NULL;
			return -1;
		}

		// searches and puts the search result in the begining
		int ilinear_search(T elem) noexcept override {
			pointer temp = this->_head_;
			pointer ttemp = NULL;
			int index = 0;

			// skipping the cached result, i.e. if the earlier search query was same, return the head->data
			if (temp->data == elem) { return index; }

			// only traversing to the tail
			while (temp != this->_tail_) {
				if (temp->data == elem) {
					ttemp->next = temp->next;
					temp->next = this->_head_;
					this->_head_ = temp;

					// nulling before returning
					temp = ttemp = NULL;
					return index;
				}
				ttemp = temp;
				temp = temp->next;
				index++;
			}

			// taking into account that the searched query might be at the end of the list
			if (temp->data == elem) { // as temp might have become the tail
				// tail needs to be made head and ttemp needs to be made tail
				// ttemp is the pointer one behind temp
				// now temp needs to be connected to head and made head, while ttemp needs to be made tail
				temp->next = this->_head_;
				ttemp->next = NULL;
				this->_head_ = temp;
				this->_tail_ = ttemp;

				// nulling before returning
				temp = ttemp = NULL;
				return index;
			}

			// nulling before returning
			temp = ttemp = NULL;
			return -1;
		}

	private:
		// takes the order as:	ll -> l -> r -> rr
		// the order to:		ll -> r -> l -> rr
		void _swap_links_(pointer ll, pointer l, pointer r, pointer rr) {
			ll->next = r;
			r->next = l;
			l->next = rr;
		} // tested and works fine

	public:
		// bubble sort, since insertion sort requires bidirectional iterators
		__SLLBase__& sort() noexcept override {
			if (this->__size__ == 0 or this->__size__ == 1) {}

			else if (this->__size__ == 2) {
				if (this->_head_->data > this->_tail_->data) {
					this->_tail_->next = this->_head_;
					this->_head_->next = NULL;
					__swap(this->_head_, this->_tail_);
				}
			}

			// sorting link wise using sliding pointers, range [head,tail] and size >= 3
			if (this->__size__ >= 3) {
				// putting the min element in the begining
				this->ilinear_search(this->min());

				// putting a sentinel in the end
				this->push_back(T{});

				for (int i = 0; i < this->__size__ - 1; i++) {
					pointer ll = this->_head_;
					pointer l = ll->next;
					pointer r = l->next;
					pointer rr = r->next;

					while (rr) {
						if (l->data > r->data) {
							// here links will be swapped along with data
							this->_swap_links_(ll, l, r, rr);
							// but the pointers are not in order now. making them in order
							__swap(l, r);
						}
						ll = l;
						l = r;
						r = rr;
						rr = rr->next;
					}

					// this->_tail_ = r;
					ll = l = r = rr = NULL;
				}
				// removing the sentinel
				this->pop_back();
			}

			return *this;
		}

		// assuming linked list is already sorted
		__SLLBase__& sorted_insert(T elem) noexcept override {
			pointer _right = this->_head_;
			pointer _left = NULL;

			while (_right) {
				if (_right->data > elem) break;
				_left = _right;
				_right = _right->next;
			}

			// considering insertions at the boundary
			if (this->begin() == _right) { this->push_front(elem); }
			else if (this->end() == _right) { this->push_back(elem); }

			// considerring insertion in middle
			else {
				// initializing a new node
				pointer temp = new value{ NULL };
				temp->data = elem;
				temp->next = NULL;

				// connecting the node
				_left->next = temp;
				temp->next = _right;

				// increasing the size
				this->__size__++;
			}
			return *this;
		}

		__SLLBase__& clear()  noexcept override {
			// deallocating memory
			pointer _temp_ = NULL;
			while (this->_head_) {
				_temp_ = this->_head_;
				this->_head_ = this->_head_->next;
				delete _temp_;
				_temp_ = NULL;
			}
			this->__size__ = 0;
			return *this;
		}

		~__SLLBase__() {
			// before this the List destructor will be called and this makes the SLL data cleared
			// iam gonna ensure that
			this->__size__ = 0;
		}
	};

	// Abstract Base Class for Double Linked List, will be extended by Double Linked List, and Circular Double Linked list
	template<typename T>
	class __DLLBase__ abstract : public List<T> { // gets more perkier functions as it is more usable as compared to SLL and CSLL
		using pointer = BinaryNode<T>*;
		using value = BinaryNode<T>;

		friend ostream& operator<<(ostream& os, __DLLBase__<T>& dll) {dll.show();return os;}

	public:
		using iterator = _BinaryNode_Iterator_<T>;
		using const_iterator = _Const_BinaryNode_Iterator_<T>;
		using reverse_iterator = _BinaryNode_Reverse_Iterator_<T>;
		using const_reverse_iterator = _Const_BinaryNode_Reverse_Iterator_<T>;

		// size has to be kept private if it gets modified by outside interference member functions will not work right
		int __size__ = 0;

		// classic methods of construction
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

		// extra helpful constructors
		// - initialization through an array
		// - initialization through iterators
		// - intialization through end and begin pointers
// i dont want to provide support for direct Array Object intialization as it will require PyBasics.h to be include in this

		// raw array intialization support
		template<size_t s>
		__DLLBase__(T (&arr) [s]) {
			int i = 0;
			this->__head__ = new value{ NULL };
			this->__head__->data = arr[i];
			this->__head__->next = NULL;
			this->__head__->prev = NULL;
			this->__tail__ = this->__head__;
			this->__size__++;
			++i;
			while (i < s) {
				pointer temp = new value{ NULL };
				temp->data = arr[i];
				temp->prev = NULL;
				temp->next = NULL;

				this->__tail__->next = temp;
				temp->prev = this->__tail__;
				this->__tail__ = temp;
				temp = NULL;
				++i;
				this->__size__++;
			}
		}

		// two pointers to a contiguous memory block based initialization
		__DLLBase__(T* begin, T* end) {
			int i = 0;
			int s = end - begin;
			this->__head__ = new value{ NULL };
			this->__head__->data = *(begin + i);
			this->__head__->next = NULL;
			this->__head__->prev = NULL;
			this->__tail__ = this->__head__;
			this->__size__++;
			++i;
			while (i < s) {
				pointer temp = new value{ NULL };
				temp->data = *(begin + i);
				temp->prev = NULL;
				temp->next = NULL;

				this->__tail__->next = temp;
				temp->prev = this->__tail__;
				this->__tail__ = temp;
				temp = NULL;
				++i;
				this->__size__++;
			}
		}

		// the iterator based constructor might not work if implemented here and called from derived class
		// so it is implmented in derived class itself

		// Iterator Support for Traversal

		_BinaryNode_Iterator_<T> begin() {
			return _BinaryNode_Iterator_<T>(this->__head__);
		}

		_BinaryNode_Iterator_<T> end() {
			return _BinaryNode_Iterator_<T>(this->__tail__->next);
		}

		_BinaryNode_Iterator_<T> head() {
			return _BinaryNode_Iterator_<T>(this->__head__);
		}

		_BinaryNode_Iterator_<T> tail() {
			return _BinaryNode_Iterator_<T>(this->__tail__);
		}

		_Const_BinaryNode_Iterator_<T> cbegin() {
			return _Const_BinaryNode_Iterator_<T>(this->__head__);
		}

		_Const_BinaryNode_Iterator_<T> cend() {
			return _Const_BinaryNode_Iterator_<T>(this->__tail__->next);
		}

		_BinaryNode_Reverse_Iterator_<T> rbegin() {
			return _BinaryNode_Reverse_Iterator_<T>(this->__tail__);
		}

		_BinaryNode_Reverse_Iterator_<T> rend() {
			return _BinaryNode_Reverse_Iterator_<T>(this->__head__->prev);
		}

		_Const_BinaryNode_Reverse_Iterator_<T> crbegin() {
			return _Const_BinaryNode_Reverse_Iterator_<T>(this->__tail__);
		}

		_Const_BinaryNode_Reverse_Iterator_<T> crend() {
			return _Const_BinaryNode_Reverse_Iterator_<T>(this->__head__->prev);
		}

		// ---x--- Iterator Support

		int size() { return this->__size__; }

	protected:
		// useful for move semantics
		void null_out() { this->__head__ = NULL; this->__tail__ = NULL; this->__size__ = 0; }

		void __copy_values__(pointer begin, pointer end) {
			this->__head__ = new value{ NULL };
			this->__head__->data = begin->data;
			this->__head__->next = NULL;
			this->__head__->prev = NULL;
			this->__tail__ = this->__head__;
			begin = begin->next;
			this->__size__ = 1;

			while (begin) {
				pointer temp = new value{ NULL };
				temp->data = begin->data;
				temp->prev = NULL;
				temp->next = NULL;

				this->__tail__->next = temp;
				temp->prev = this->__tail__;
				this->__tail__ = temp;
				temp = NULL;
				begin = begin->next;
				this->__size__++;
			}
		}

	public:
		/*	Push and Pop at Front and Back		*/
		/*	All are constant time operations	*/

		// deleting the last element
		T pop_back()  noexcept override {
			T val{};
			if (this->__size__ == 0) {/* Do Nothing */ }
			else {
				pointer temp = this->__tail__;
				this->__tail__ = this->__tail__->prev;
				this->__tail__->next = NULL;
				val = temp->data;
				delete temp;
				temp = NULL;
				this->__size__--;
			}
			return val;
		}

		// deleting the begining element
		T pop_front() noexcept override {
			T val{};
			if (this->__size__ == 0) {}
			else {
				pointer temp = this->__head__;
				this->__head__ = this->__head__->next;
				this->__head__->prev = NULL;
				val = temp->data;
				delete temp;
				temp = NULL;
				this->__size__--;
			}
			return val;
		}

		// inserting an element in the begining
		__DLLBase__& push_front(T val)  noexcept override {
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

		// inserting an element at the last
		__DLLBase__& push_back(T val)  noexcept override {
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

		/*		Insertion and Removal			*/
		/*	All are linear time operations		*/

		// inserts provided element on the required index
		__DLLBase__& insert(T val, int index)  noexcept override {
			// boundary cases will be handled by push front and back
			if (index == 0) { this->push_front(val); }
			else if (index == this->__size__) { this->push_back(val); }

			// insertion range: (__head__,__tail__)
			else {
				pointer temp = this->__head__;

				// traversing temp to insertion point
				for (int i = 0; i < index; i++) temp = temp->next;

				// getting the previous node
				pointer _left = temp->prev;

				// initializing the middle node
				pointer _mid = new value{ NULL };
				_mid->data = val;
				_mid->next = NULL;
				_mid->prev = NULL;

				// now connecting _mid to _left
				_left->next = _mid;
				_mid->prev = _left;

				// now connecting temp to _mid
				_mid->next = temp;
				temp->prev = _mid;

				// nulling spare pointers
				temp = _mid = _left = NULL;

				this->__size__++;
			}
			return *this;
		}

		// removes the element from the required index
		T remove(int index)  noexcept override {
			// again boundary cases will be managed by pop front and back
			if (index == 0) { return this->pop_front(); }
			else if (index == this->__size__) { return this->pop_back(); }

			// removal range(__head__, __tail__)
			else {
				T val{};
				pointer temp = this->__head__;
				for (int i = 0; i < index; i++) temp = temp->next;

				pointer _left = temp->prev;
				pointer _mid = temp;

				temp = temp->next;
				val = _mid->data;
				delete _mid;

				// connecting left to temp
				_left->next = temp;
				temp->prev = _left;

				this->__size__--;
				return val;
			}
		}

		/*	Searches	*/
		// bound to be implemented here accordingly
		int ilinear_search(T elem) noexcept override {
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

		/*	Sorting and Sorted Insertion	*/

		__DLLBase__& sort() noexcept override {
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
			// assuming the list is already sorted
			pointer temp = this->__head__;

			// finding the position of insertion
			while (temp) {
				if (temp->data > elem) break;
				temp = temp->next;
			}

			// considering boundary insertions
			if (this->begin() == temp) { this->push_front(elem); }
			else if (this->end() == temp) { this->push_back(elem); }

			// considering insertions in middle
			else {
				pointer _left = temp->prev;

				// initializing a new node
				pointer _new_node = new value{ NULL };
				_new_node->data = elem;
				_new_node->prev = NULL;
				_new_node->next = NULL;

				// connecting the left to new node
				_new_node->prev = _left;
				_left->next = _new_node;

				// connecting new node to temp
				_new_node->next = temp;
				temp->prev = _new_node;

				// nulling the pointers out
				_new_node = NULL;
				temp = NULL;
				_left = NULL;

				this->__size__++;
			}
			return *this;
		}

		/*	Reversal	*/
		__DLLBase__& reverse_by_links() noexcept override {
			// using sliding pointers
			pointer p = this->__head__;

			while (p) {
				pointer temp = p->next;
				p->next = p->prev;
				p->prev = temp;

				p = p->prev;

				if (p != NULL and p->next == NULL) this->__head__ = p;
			}

			// since the tail is screwed
			p = this->__head__;
			while (p) {
				this->__tail__ = p;
				p = p->next;
			}

			p = NULL;

			return *this;
		}

		// only works for DLL objects
		void reverse_show() {
			pointer dl_ptr = this->__tail__;
			do {
				cout << dl_ptr->data << " ";
				dl_ptr = dl_ptr->prev;
			} while (dl_ptr);
			cout << endl;
		}

		// convienient display
		void show()  noexcept override {
			pointer dl_ptr = this->__head__;
			while (dl_ptr) {
				cout << dl_ptr->data << " ";
				dl_ptr = dl_ptr->next;
			}cout << endl;
		}

		// linear search, returns first appearance
		int linear_search(T elem) noexcept override {
			int index = 0;
			pointer temp = this->__head__;
			while (temp) {
				if (temp->data == elem) {
					return index;
				}
				temp = temp->next;
				index++;
			}
			temp = NULL;
			return -1;
		}

		// max and min element
		T max()  noexcept override {
			T max = this->__head__->data;
			pointer temp = this->__head__;
			while (temp) {
				if (temp->data > max) {
					max = temp->data;
				}
				temp = temp->next;
			}
			temp = NULL;
			return max;
		}

		T min()  noexcept override {
			T min = this->__head__->data;
			pointer temp = this->__head__;
			while (temp) {
				if (temp->data < min) {
					min = temp->data;
				}
				temp = temp->next;
			}
			temp = NULL;
			return min;
		}

		/*		Some Perk Functions for DLL and CDLL	*/
		// _merge_ to be overridden as merge

		// give indices for the subset that needs to be erased
		__DLLBase__& erase(int begin, int end) {
			// to make sure that the parameters are in bound
			if (begin < this->__size__ and end <= this->__size__) {

				// whole list needs to be erased
				if ((end - begin == this->__size__)) { this->clear(); }

				// putting a sentinal
				this->push_back(T{});

				if ((end - begin < this->__size__)) {
					// creating a pointer traversing it to the spot and then triggering a delete sequence

					pointer temp = this->__head__;
					int i = 0;
					for (; i < begin; i++) temp = temp->next;

					pointer temp_prev = temp->prev;

					pointer _goat_ = NULL;
					// triggerring the delete sequence
					while (i < end) {
						_goat_ = temp;
						temp = temp->next;
						delete _goat_;
						_goat_ = NULL;
						i++;
					}

					// now connecting temp_prev with temp
					temp_prev->next = temp;
					temp->prev = temp_prev;

					this->__size__ -= (end - begin);
				}

				// removing the sentinal
				this->pop_back();
			}
			return *this;
		}

		// provide a functor or lambda
		template<class Func>
		__DLLBase__& remove_if(Func function) {
			// clearing off invalid entities from extremes
			while (function(this->__tail__->data))this->pop_back();
			while (function(this->__head__->data))this->pop_front();

			pointer temp = this->__head__;
			while (temp) {
				if (function(temp->data)) {
					// making a copy
					pointer copy_temp = temp;
					// if there exists a sequence of values to be removed
					while (function(copy_temp->data)) {
						// delete manouver keeps taking on until the consecutive occurrences of elem get deleted
						// taking out the prev and next of copy_temp;
						pointer prev_temp = copy_temp->prev;
						pointer next_temp = copy_temp->next;

						// linking prev_temp and next_temp
						prev_temp->next = next_temp;
						next_temp->prev = prev_temp;

						// deallocating the memory for copy_temp;
						delete copy_temp;

						// now copy_temp should point on next_temp and prev_temp and next_temp needs to be nulled
						copy_temp = next_temp;

						next_temp = prev_temp = NULL;

						// all of this causes an invalidation to temp so temp should also be copy_temp
						temp = copy_temp;
						// by this temp has already moved on to the next node
						this->__size__--;
					}
				}
				temp = temp->next;
			}
			return  *this;
		}

	protected:
		void __concat__(pointer head, pointer tail) {
			// connecting the tail to head
			this->__tail__->next = head;
			head->prev = this->__tail__;
			
			this->__tail__ = tail;
		}

	private:
		struct is_elem {
			T t;
			is_elem(T _t) : t{ _t } {}
			bool operator()(T e) {
				return t == e;
			}
		};

	/*	
	Mechanism

	* node = <0>
	* link = -
	* pointer = ^
	* times = 2
	list looks like ->            <0>-<0>-<0>-<0>-<0>-<0>
								   ^ head              ^tail

								  <0>-<0>-<0>-<0>-<0>-<0>           // traversing head_of_second to no. of times
					  head_of_first^       ^head_of_second

								  <0>-<0>-<0>-<0>-<0>-<0>
					  head_of_first^       ^head_of_second
									   ^prev_of_head_of_second

								  <0>-<0>   <0>-<0>-<0>-<0>         // link is broken by prev_of_head_of_second, which is then grounded NULL
					  head_of_first^         ^head_of_second

								  <0>-<0>	<0>-<0>-<0>-<0>         // both lists are reversed by links causing the heads to become tail
					  head_of_first^                     ^head_of_second
					  tail_of_first^                     ^tail_of_second

	> Now the Heads are traversed back to their positions

								  <0>-<0>   <0>-<0>-<0>-<0>
					  head_of_first^         ^head_of_second
						  tail_of_first^                 ^tail_of_second

	> tail_of_first and head_of_second establish a link and are grounded NULL
								  <0>-<0>-<0>-<0>-<0>-<0>
					  head_of_first^                   ^tail_of_second

	> Now the head_of_first is made head, and tail_of_second is made tail

	> And then the whole list is then reversed to complete the procedure of rotation
	*/
		void __rotate__(int point) {
			// creating two lists, and then reversing them
			pointer _head_of_first_ = this->__head__;
			pointer _head_of_second_ = this->__head__;

			// _head_of_second_ will be traversed to the point of rotation
			int i = 0;
			while (i < point) {
				_head_of_second_ = _head_of_second_->next;
				i++;
			}

			// next of previous node of _head_of_second_ will be made NULL
			// creating a bifurcation
			pointer prev_head_of_second = _head_of_second_->prev;
			prev_head_of_second->next = NULL;
			_head_of_second_->prev = NULL;

			// no more required now
			prev_head_of_second = NULL;

			// now two segments have been made

			// reversing first segment
			pointer p = _head_of_first_;
			while (p) {
				pointer temp = p->next;
				p->next = p->prev;
				p->prev = temp;

				p = p->prev;

				if (p != NULL and p->next == NULL) this->__head__ = p;
			}

			// reversing second segment
			p = _head_of_second_;
			while (p) {
				pointer temp = p->next;
				p->next = p->prev;
				p->prev = temp;

				p = p->prev;

				if (p != NULL and p->next == NULL) this->__head__ = p;
			}

			// reversal has converted the head to tail
			pointer _tail_of_first_ = _head_of_first_;

			// traversing _head_of_first_ to its original position, i.e. being head of first list segment
			while (_head_of_first_->prev) {
				_head_of_first_ = _head_of_first_->prev;
			}

			/* // for debugging purposes, will show first segment
			p = _head_of_first_;
			while (p) {
				cout << p->data << " ";
				p = p->next;
			}
			cout << endl;
			*/

			pointer _tail_of_second_ = _head_of_second_;

			// traversing _head_of_second_ to its original position, i.e. being head of second list segment
			while (_head_of_second_->prev) {
				_head_of_second_ = _head_of_second_->prev;
			}

			/* // for debugging purposes, will show the second segment of the list
			p = _head_of_second_;
			while (p) {
				cout << p->data << " ";
				p = p->next;
			}
			cout << endl;
			*/

			// connecting tail_of_first to head_of_second
			_tail_of_first_->next = _head_of_second_;
			_head_of_second_->prev = _tail_of_first_;

			// no more required now
			_tail_of_first_ = _head_of_second_ = NULL;

			/* // for debugging purposes
			p = _head_of_first_;
			while (p) {
				cout << p->data << " ";
				p = p->next;
			}
			cout << endl;*/

			this->__head__ = _head_of_first_;
			this->__tail__ = _tail_of_second_;

			// no more required Now
			_tail_of_second_ = _head_of_first_ = NULL;

			// finalizing the process of rotation
			this->reverse_by_links();
		}

	public:
		__DLLBase__& remove_all(T elem) {
			this->remove_if(is_elem(elem));
			return *this;
		}

		__DLLBase__& rotate(int times = 1, bool clockwise_or_anti_cloackwise = false) {
			if (times > 0 and times < this->__size__) {
				if (clockwise_or_anti_cloackwise) this->__rotate__(times);
				else this->__rotate__(this->__size__ - times);
			}
			return *this;
		}

		__DLLBase__& clear() noexcept override {
			// clearing the previously allocated memory
			pointer __temp__ = NULL;
			while (this->__head__) {
				__temp__ = this->__head__;
				this->__head__ = this->__head__->next;
				delete __temp__;
				__temp__ = NULL;
			}
			this->null_out();
			return *this;
		}

		// debug detals info
		void log_properties(char const* entry = "", bool log_obj_details = false, bool show_log_count = true) {
			List<T>::log_properties(entry, log_obj_details, show_log_count);
			cout << "Content: "; this->show();
			cout << "Size: " << this->__size__ << endl;
			cout << endl;
		}

		~__DLLBase__() { this->__size__ = 0; }
	};

	// This Abstract Class acts as an Interface for Circular Linked List classes
	template<typename T>
	class __CLLBase__ abstract {
	protected:
		bool is_circular = false;
	public:
		virtual void straighten() = 0;
		virtual void circularize() = 0;
		virtual bool test_circularity() = 0;
	};

//#################################################################################################################
												/* Child Level */
						/*	And also the level where all types of linked lists are implemented	*/

	/*
			Base class already provides all the methods and facilities
			so here only those functions will be implemented which require a parameter of same type
			like copy and move construct
			concat and merge
			operator=
	*/

	// Singly Linked List
	template<typename T>
	class SLList : public __SLLBase__<T> {
		using pointer = SingleNode<T>*;
		using value = SingleNode<T>;
	public:
		SLList() : __SLLBase__<T>() {}

		SLList(initializer_list<T> init_l) : __SLLBase__<T>(init_l) {}

		int size() { return this->__size__; }

		// copy construct
		SLList(const SLList& obj) {
			if (obj.__size__ > 0) {
				this->_copy_values_(obj._head_, obj._tail_);
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
			this->clear();

			this->__size__ = 0;

			if (obj.__size__ > 0) {
				this->_copy_values_(obj._head_, obj._tail_);
			}
			else {
				this->_head_ = NULL;
				this->_tail_ = NULL;
				this->__size__ = 0;
			}

			return *this;
		}

		// for rhs values
		SLList& operator= (SLList&& obj) noexcept {
			// clearing old memory
			this->clear();

			this->null_out();

			this->_head_ = obj._head_;
			this->_tail_ = obj._tail_;
			this->__size__ = obj.__size__;

			obj.null_out();
			return *this;
		}

		// utility functions
		SLList& concat(SLList<T> sll) {
			if (this->__size__ > 0) {
				//cout << "_tail_->data: " << _tail_->data << endl;
				//cout << "_tail_->next: " << _tail_->next << endl;
				//cout << "obj->_head_->data: " << sll._head_->data << endl;
				pointer temp = sll._head_;
				this->_tail_->next = temp;

				this->_tail_ = sll._tail_;
				this->__size__ += sll.__size__;

				// since we already have a copy of the linked list we can assign it to ours
				// and null out the copy so it doesn't delete the memory referenced by its 
				// pointers _head_, _tail_ on the heap while getting destroyed
				sll.null_out();
			}
			else { this->operator=(sll); }
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
	// Circular Singly Linked List
	template<typename T>
	class CSLList : public __SLLBase__<T>, public __CLLBase__<T> {
		using pointer = SingleNode<T>*;
		using value = SingleNode<T>;

		// incase anything goes wrong remove the inheritance and enable next line of code
		// bool is_circular = false;

	public:
		// a circular linked list should not be empty
		CSLList() = delete;

		CSLList(initializer_list<T> init_l) : __SLLBase__<T>(init_l) {
			// in the original state this list is linear
			this->is_circular = false;
		}

		// copy construct
		CSLList(const CSLList& obj) {
			if (obj.__size__ > 0) {
				this->_copy_values_(obj._head_, obj._tail_);
			}
			else {
				this->_head_ = NULL;
				this->_tail_ = NULL;
				this->__size__ = 0;
			}
		}

		// move construct
		CSLList(CSLList&& obj) {
			this->_head_ = obj._head_;
			this->_tail_ = obj._tail_;
			this->__size__ = obj.__size__;
			obj.null_out();
		}

		// for lhs values
		CSLList& operator= (const CSLList& obj) {
			// clearing old memory
			this->clear();

			if (obj.__size__ > 0) {
				this->_copy_values_(obj._head_, obj._tail_);
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

		void straighten() {
			if (this->is_circular) {
				this->_tail_->next = NULL;
				this->is_circular = false;
			}
		}

		void circularize() {
			if (!this->is_circular) {
				this->_tail_->next = this->_head_;
				this->is_circular = true;
			}
		}

		CSLList& clear() noexcept override {
			this->straighten();
			__SLLBase__<T>::clear();
			return *this;
		}

		CSLList& sort() noexcept override {
			this->straighten();
			__SLLBase__<T>::sort();
			return *this;
		}

		void show() noexcept override {
			this->straighten();
			__SLLBase__<T>::show();
		}

		// tests circularity of the list
		bool test_circularity() {
			if (this->__size__ == 1 and this->_head_ == this->_tail_) { return true; }

			pointer p = this->_head_;
			pointer q = this->_head_->next;

			while (q) {
				if (p == q) {
					this->is_circular = true;
					return true;
				}

				p = p->next;
				q = q->next;
				q = q ? q->next : q;
			}

			this->is_circular = false;
			return false;
		}

		~CSLList() {
			if (this->is_circular) this->straighten();
		}
	};

	// Doubly Linked List
	template<typename T>
	class DLList : public __DLLBase__<T> {
		using pointer = BinaryNode<T>*;
		using value = BinaryNode<T>;

	public:
		DLList() : __DLLBase__<T>() {}
		DLList(initializer_list<T> init_l) : __DLLBase__<T>(init_l) {}

		// array based initialization
		template<size_t s>
		DLList(T(&arr)[s]) : __DLLBase__<T>(arr) {}

		// Iterator based initialization, supports iterators that have an overloaded ++ and * operator
		// i.e. supports initialization from vector<>, deque<>, list<>
		template<typename _Iter>
		DLList(_Iter begin, _Iter end) {
			_Iter it = begin;
			this->__head__ = new value{ NULL };
			this->__head__->data = *it;
			this->__head__->next = NULL;
			this->__head__->prev = NULL;
			this->__tail__ = this->__head__;
			this->__size__++;
			++it;
			while (it != end) {
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

		// copy, move constructors and assignments
		DLList(const DLList& obj) {
			if (obj.__size__ > 0) {
				this->__copy_values__(obj.__head__, obj.__tail__);
			}
			else {
				this->__head__ = NULL;
				this->__tail__ = NULL;
				this->__size__ = 0;
			}
		}

		DLList(DLList&& obj)noexcept {
			this->__head__ = obj.__head__;
			this->__tail__ = obj.__tail__;
			this->__size___ = obj.__size__;
			obj.null_out();
		}

		DLList& operator=(const DLList& obj) {
			// clearing the previously allocated memory
			this->clear();

			if (obj.__size__ > 0) {
				this->__copy_values__(obj.__head__, obj.__tail__);
			}
			else {
				this->__head__ = NULL;
				this->__tail__ = NULL;
				this->__size__ = 0;
			}

			return*this;
		}

		DLList& operator=(DLList&& obj) noexcept {
			// clearing the previously allocated memory
			while (this->__head__) {
				pointer temp = this->__head__;
				this->__head__ = this->__head__->next;
				delete temp;
			}

			this->null_out();

			this->__head__ = obj.__head__;
			this->__tail__ = obj.__tail__;
			this->__size__ = obj.__size__;
			obj.null_out();

			return*this;
		}

		// for concatenation
		DLList& concat(DLList<T> obj) {
			if (this->__size__ > 0) {
				// joining the two lists
				this->__concat__(obj.__head__, obj.__tail__);

				this->__size__ += obj.__size__;
				obj.null_out();
			}
			else { this->operator=(obj); }
			return *this;
		}

		~DLList() {}
	};

	/*
	In original state the CDLL wil be DLL, it will become circular once circularize() is called
	but as a counter to circularize, straighten function is also provided, also circularize and straighten
	should be used in pairs.
	for example
		to print the content using an external pointer to the list it can be done this way

		CDLList<int> cdl {3,4,5,2,4};			// creating an object
		cdl.circularize();						// circularising the list
		BinaryNode<T>* temp = cdl.begin();		// initializing an external pointer by begin()
		do {									// since in CSLList the head is the only pointer, end() also returns head so we use do while loop
			cout << temp->data << " ";			// using the loop to traverse all data
			temp = temp->next;
		}while (temp != cdl.end());
		cdl.straighten();						// straightening the data as it will cause failures during destruction of object
	*/
	// Circular Doubly Linked List
	template<typename T>
	class CDLList :public __DLLBase__<T>, public __CLLBase__<T> {
		using pointer = BinaryNode<T>*;
		using value = BinaryNode<T>;

		// incase anything goes wrong remove the inheritance and enable next line of code
		// bool is_circular = false;

	public:
		// a circular linked list cannot be empty
		CDLList() = delete;

		CDLList(initializer_list<T> init_l) : __DLLBase__<T>(init_l) {
			this->is_circular = false;
		}

		// array based initialization
		template<size_t s>
		CDLList(T(&arr)[s]) : __DLLBase__<T>(arr) {}

		// Iterator based initialization, supports iterators that have an overloaded ++ and * operator
		// i.e. supports initialization from vector<>, deque<>, list<>
		template<typename _Iter>
		CDLList(_Iter begin, _Iter end) {
			_Iter it = begin;
			this->__head__ = new value{ NULL };
			this->__head__->data = *it;
			this->__head__->next = NULL;
			this->__head__->prev = NULL;
			this->__tail__ = this->__head__;
			this->__size__++;
			++it;
			while (it != end) {
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

		CDLList(const CDLList& obj) {
			if (obj.__size__ > 0) {
				this->__copy_values__(obj.__head__, obj.__tail__);
			}
			else {
				this->__head__ = NULL;
				this->__tail__ = NULL;
				this->__size__ = 0;
			}
		}

		CDLList(CDLList&& obj) noexcept {
			this->__head__ = obj.__head__;
			this->__tail__ = obj.__tail__;
			this->__size___ = obj.__size__;
			obj.null_out();
		}

		CDLList& operator=(const CDLList& obj) {
			// clearing the previously allocated memory
			this->clear();

			if (obj.__size__ > 0) {
				this->__copy_values__(obj.__head__, obj.__tail__);
			}
			else {
				this->__head__ = NULL;
				this->__tail__ = NULL;
				this->__size__ = 0;
			}

			return*this;
		}

		CDLList& operator= (CDLList&& obj) noexcept {
			// clearing the previously allocated memory
			while (this->__head__) {
				pointer temp = this->__head__;
				this->__head__ = this->__head__->next;
				delete temp;
			}

			this->null_out();

			this->__head__ = obj.__head__;
			this->__tail__ = obj.__tail__;
			this->__size__ = obj.__size__;
			obj.null_out();

			return*this;
		}

		void circularize() {
			if (!this->is_circular) {
				this->__tail__->next = this->__head__;
				this->__head__->prev = this->__tail__;
				this->is_circular = true;
			}
		}

		void straighten() {
			if (this->is_circular) {
				this->__tail__->next = NULL;
				this->__head__->prev = NULL;
				this->is_circular = false;
			}
		}

		CDLList& clear() noexcept override {
			this->straighten();
			__DLLBase__<T>::clear();
			return *this;
		}

		CDLList& sort() noexcept override {
			this->straighten();
			__DLLBase__<T>::sort();
			return *this;
		}

		// for concatenation
		CDLList& concat(CDLList<T> obj) {
			this->straighten();

			if (this->__size__ > 0) {
				// joining the two lists
				this->__concat__(obj.__head__, obj.__tail__);

				this->__size__ += obj.__size__;
				obj.null_out();
			}
			else { this->operator=(obj); }
			return *this;
		}

		void show() noexcept override {
			this->straighten();
			__DLLBase__<T>::show();
		}

		bool test_circularity() {
			if (this->__size__ == 1 and this->__head__ == this->__tail__) { return true; }

			pointer p = this->__head__;
			pointer q = this->__head__->next;

			while (q) {
				if (p == q) {
					this->is_circular = true;
					return true;
				}

				p = p->next;
				q = q->next;
				q = q ? q->next : q;
			}

			this->is_circular = false;
			return false;
		}

		~CDLList() {
			if (this->is_circular) this->straighten();
		}
	};

}	// namespace Py

#endif // _LISTS_H_







// a big hokum starts from here that suffered ill implementation
//#################################################################################################################

/* implementation of variadic type args Linked List. would contain any type of data */
/*
template<typename T>
struct VariadicNode {
	VariadicNode<void>* prev = NULL;
	T data;
	VariadicNode<void>* next = NULL;

	bool has_value() { return true; }
};

template<>
struct VariadicNode<void> {
	VariadicNode<void>* prev = NULL;
	VariadicNode<void>* next = NULL;
	bool has_value() { return false; }
};

// Non-Templated Variable arg type Linked List
class VLList {
	using void_pointer = VariadicNode<void>*;
	using void_value = VariadicNode<void>;

	template<typename T>
	using type_pointer = VariadicNode<T>*;

	template<typename T>
	using type_value = VariadicNode<T>;

public:
	type_pointer<int> __head__ = new type_value<int>{};
	void_pointer __secondary_head__ = new void_value{ NULL };
	type_pointer<int> __tail__ = new type_value<int>{};

	int __size__ = 0;

	VLList() {
		// connecting the temporary mid to head
		this->__head__->next = this->__secondary_head__;
		this->__secondary_head__->prev = (void_pointer)this->__head__;

		// connecting the temporary mid to tail
		this->__tail__->prev = this->__secondary_head__;
		this->__secondary_head__->next = (void_pointer)this->__tail__;
	}

	template<typename T>
	void push_back(T elem) {
		if (this->__size__ == 0) {
			void_pointer v_temp = new void_value{ NULL };

			type_pointer<T> temp = new type_value<T>{ NULL };
			temp->data = elem;
			temp->next = NULL;
			temp->prev = NULL;

			//the thing is the secondary head should now be considered as head to link the temp node;
			this->__secondary_head__->next = (void_pointer)temp;
			temp->prev = this->__secondary_head__;

			// now v_temp should be linked to temp;
			temp->next = v_temp;
			v_temp->prev = (void_pointer)temp;

			// connecting the v_temp to tail;
			this->__tail__->prev = v_temp;
			v_temp->next = (void_pointer)this->__tail__;
		}
	}

private:
	std::ostream& __print__(std::ostream& os) { return os; }

	template <typename T>
	std::ostream& __print__(std::ostream& os, T&& t) {
		return os << std::forward<T>(t);
	}

	template <typename T, typename U, typename... Args>
	std::ostream& __print__(std::ostream& os, T&& t, U&& u, Args&&... args) {
		return __print__(__print__(os, std::forward<T>(t)) << ' ', std::forward<U>(u), std::forward<Args>(args)...);
	}

};
*/