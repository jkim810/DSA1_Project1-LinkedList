#include <iostream>
using namespace std;

template <typename T>
class Node{
	public:
		T data;
		Node<T> *next;
		void setNext(Node *p){}
		Node();
		Node(T data);
};


template <typename T>
Node<T>::Node(){
	//cout<<"creating node"<<endl;
	data = NULL;
}

template <typename T>
Node<T>::Node(T data){
	this->data = data;
}

template <typename T>
class List{
	public:
		Node<T> *head;
		List();
		bool isEmpty(){
			if(head->data == NULL) return 1;
			else return 0;
		};
		void insertHead(T data);
		void insertTail(T data);
		T removeHead();
};

template <typename T>
List<T>::List(){
	head = new Node<T>();
}

template <typename T>
void List<T>::insertHead(T data){
	Node<T> *p = new Node<T>(data);
	head->next = p;
}

template <typename T>
void List<T>::insertTail(T data){
	Node<T> *p = new Node<T>(data);
	if(isEmpty()){
		head->next = p;
	}else{
		Node<T> *cur = head;
		while(isEmpty()) cur = cur->next;
		cur->next = p;
	}
	
}

template <typename T>
T List<T>::removeHead(){
	if(isEmpty()) return 0;
	else{
		Node<T> *tmp = head->next;
		head->next = head->next->next;
		return tmp->data;
	}
}

int main() {
	// your code goes here
	List<int> *l = new List<int>();
	l->insertHead(3);
	l->insertHead(4);
	l->insertHead(5);
	l->insertHead(6);
	cout << l->removeHead()<<endl;
	cout << l->removeHead()<<endl;
	cout << l->removeHead()<<endl;
	cout << l->removeHead()<<endl;
	return 0;
}

