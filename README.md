# List-Library
A Thorough Implementation of the 4 types of Linked Lists with C++ and OOP

This Library has an Inheritance based model of implementing Linked Lists.
The model looks like this

		Grand Parent:		abstract class List
		Derived:		abstract class __SLLBase__ and abstract class __DLLBase__ and an interface __CLLBase__

		Parent:			abstract class __SLLBase__
		Children:		class SLList and class CSLList (both not abstract)
						- SLList: Singly Linked List
						- CSLList: Circular Singly Linked List	(also inherits __CLLBase__)

		Parent:			abstract class __DLLBase__
		Children:		class DLList and class CDLList (both not abstract)
						- DLList: Doubly Linked List
						- CDLList: Circular Doubly Linked List  (also inherits __CLLBase__)

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
		
Support for Iterators has also been added. 

	- DLList class supports bi-directional iterators and bi-directional reverse iterators.
	- SLList class supports forward iteratrs only.
	- The Nodes.h file contains support for Iterators for TreeNode.

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
