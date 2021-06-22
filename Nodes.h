#ifndef NODES_H
#define NODES_H

#include <iostream>
using namespace std;

namespace Py {

	template<typename T>
	struct SingleNode {
		T data;
		SingleNode<T>* next;
	};

	template<typename T>
	struct BinaryNode {
		BinaryNode<T>* prev;
		T data;
		BinaryNode<T>* next;
	};
}

#endif // !NODES_H

