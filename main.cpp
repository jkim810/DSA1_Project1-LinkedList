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
	next = NULL;
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
			if(head->next == NULL) return 1;
			else return 0;
		};
		void insertHead(T data);
		void insertTail(T data);
		T removeHead();
		
		virtual void push(T data);
		virtual T pop();
};

template <typename T>
List<T>::List(){
	head = new Node<T>();
}

template <typename T>
void List<T>::insertHead(T data){
	Node<T> *p = new Node<T>(data);
	p->next = head->next;
	head->next = p;
}

template <typename T>
void List<T>::insertTail(T data){
	Node<T> *p = new Node<T>(data);
	if(isEmpty()){
		head->next = p;
	}else{
		Node<T> *cur = head;
		while(cur->next!=NULL) cur = cur->next;
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


template <typename T>
void List<T>::push(T data){
	//stack
	insertHead(data);
	//queue
	insertTail(data);
}

template <typename T>
T List<T>::pop(){
	return removeHead();
}

template <typename T>
class Stack : public List<T>{
	public:
		void push(T data){
			List<T>::insertHead(data);
		}
		T pop(){
			return List<T>::removeHead();
		}
};

template <typename T>
class Queue : public List<T>{
	public:
		void push(T data){
			List<T>::insertTail(data);
		}
		T pop(){
			return List<T>::removeHead();
		}
};

int main() {
	// your code goes here
	Stack<int> *s = new Stack<int>();
	Queue<int> *q = new Queue<int>();
	
	for(int i = 0; i < 5; i++){
		s->push(i);
		q->push(i);
	}
	
	for(int i = 0; i < 5; i++) cout << s->pop() << endl;
	
	cout << "------" << endl;

	for(int i = 0; i < 5; i++) cout << q->pop() << endl;

	return 0;
}

