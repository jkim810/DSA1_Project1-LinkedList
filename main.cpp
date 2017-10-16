//AUTHOR: Junbum Kim
//ECE 164 Data structures and algorithms 1
//PROFESSOR: Carl Sable

#include <iostream>
#include <fstream>
#include <list>
#include <sstream>
#include <cstring>

using namespace std;

//Base class simple list using template type T
template <typename T>
class SimpleList {

	//Nested Template Node Class
	template <typename TYPE>
	class Node {
		//Encapsulation of data and next node via private members
		//Restricts the access of data and next outside of the class
		private:
			TYPE data;				//Template type
			Node <TYPE> *next;		//Pointer to next node

		public:
			//constructor for initialization
			Node <TYPE> () { next = NULL; };

			//destructor
			~Node <TYPE> (){ delete next; };

			//polymorphism of constructor
			//allows to set data and next while initializing node
			Node <TYPE> ( const TYPE &item , Node<TYPE> *ptrnext ) {
				this -> data = item;
				this -> next = ptrnext;
			}

			//sets and gets method of the private members of the Node Class
			void setData( TYPE dataToAdd){ data = dataToAdd; }
			void setNext( Node <TYPE> *nodeToAdd){ next = nodeToAdd; }
			TYPE getData(){ return data; }
			Node <TYPE> *getNext(){ return next; }
	};

	private:
		//private variables of the SimpleList class
		Node <T> *head;			//sentinel node at the start of the list
		Node <T> *curr;			//the last element of the list
		string listName;		//identification name for the Simple list. This is used to search this SimpleList from the list of SimpleLists

	protected:
		//List modification methods
		//These three function will allow LIFO and FIFO for stack and queue
		void insertFirst(T typeToAdd);
		void insertLast(T typeToAdd);
		T removeFirst();

	public:
		//constructor
		//initializes base nodes
		SimpleList(){
			head = new Node <T> ();	
			curr = new Node <T> ();
		}

		//destructor
		//frees allocated memory
		~SimpleList(){
			delete head;
		}

		//check for whether the list is empty
		bool isEmpty();

		//inherited methods from List Modification methods
		virtual void push(T data){};
		virtual T pop(){};

		//sets and gets for the Simple List identification to search from C++ lists
		void setName(string name);
		string getName();
};

//Standard list method to check for whether the list is empty
template <typename T>
bool SimpleList<T>::isEmpty () {
	if( head -> getNext() == NULL ){			// no real node exists.
		return 1;
	} else {									// nodes other than the sentinel node exist.
		return 0;
	}
}

//Set name function for the simple list
template <typename T>
void SimpleList<T>::setName(string name){
	listName = name;
}

//get name function for the simple list
template <typename T>
string SimpleList<T>::getName(){
	return listName;
}

//LIST MODIFICATION METHODS
//Connect the current node to the next node
template <typename T>
void SimpleList<T>::insertFirst (T typeToAdd){
	Node <T> *first = new Node <T> ();			//create a node to add to the list
	first -> setData(typeToAdd);				//set the data of the node
	if(isEmpty()){								//if list is empty
		head -> setNext(first);					//connect the node to the sentinel node
		first -> setNext(NULL);					//set the next pointer of the added node as NULL since it does not point to anything
	} else {									//if list is not empty
		first -> setNext(head -> getNext());	//connect the first element behind the node to be added
		head -> setNext(first);					//connect the adding node to head
	}
}

//Connect a node to the last node of the list
template <typename T>
void SimpleList<T>::insertLast (T typeToAdd){
	Node <T> *last = new Node <T> ();			//create a node to add to the list
	last -> setData(typeToAdd);					//set the data of the node
	if(isEmpty()){								//if list is empty
		head -> setNext(last);					//connect the node to the sentinel node
		last -> setNext(NULL);					//set the pointer of the last node as NULL since it does not point to anything
		curr = last;							//set curr to last so that we can keep track of the last node
	} else {									//if list is not empty
		curr -> setNext(last);					//connect the node to add behind the last node
		last -> setNext(NULL);					//set the pointer of the last node as NULL since it does not point to anything
		curr = last; 							//set curr to last so that we can keep track of the last node
	}
}

//remove the last node of the list
template <typename T>
T SimpleList<T>::removeFirst(){
// Deletes the node from the list and returns the deleted node
// check if there is a next node
	if (!isEmpty()){
		// if there is a next, return the value of next.
		T temp = head -> getNext() -> getData();
		// set second element as the first
		head -> setNext(head -> getNext() -> getNext());
		   	return temp;
	} else {
		return 0;
	}
}

//Declaration of Inherited class stack 
template <typename T>
class Stack : public SimpleList<T>{
public:
	virtual void push(T data);
	virtual T pop();
};

//Inheritance of SimpleList class function insertFirst to push for stack (First in)
template <typename T>
void Stack <T>::push(T data){
	SimpleList<T>::insertFirst(data);
}

//Inheritance of SimpleList class function removeFirst to pop for stack (Last out)
template <typename T>
T Stack <T>::pop(){
	return SimpleList<T>::removeFirst();
}

//Declaration of Inherited class queue
template <typename T>
class Queue : public SimpleList<T>{
public:
	virtual void push(T data);
	virtual T pop();
};

//Inheritance of SimpleList class function insertLast to push for queue (First in)
template <typename T>
void Queue <T>::push(T data){
	SimpleList<T>::insertLast(data);
}

//Inheritance of SimpleList class function removeFirst to pop for queue (First out)
template <typename T>
T Queue <T>::pop(){
	return SimpleList<T>::removeFirst();
}

//Search SimpleList from the list of SimpleLists using the name of the SimpleList
template <typename T>
SimpleList<T> *search(list<SimpleList <T> *> listToPop, string key){
	//define an iterator to traverse through the standard list
	typename list<SimpleList<T> *>::iterator i;
	//SimpleList pointer to return if the SimpleList is found
	SimpleList<T> *temp;
	//Traverse through every single element in the list
	for(i = listToPop.begin(); i!=listToPop.end(); i++){
		//if a list that matches the name is found
		if((*i) -> getName() == key){
			//reference the pointer of SimpleList to the content of the found SimpleList
			temp = *i;
			//return the SimpleList pointer
			return temp;
		}
	}
	//If no matching SimpleList is found, return null
	//This return of null will help us log errors
	return NULL;
}

int main(){
	//VARIABLES
	string inputName;		//name of input file
	string outputName;		//name of output file
	string inLine;			//read in line. The commands will be processed line by line, sequentially
	string command; 		//first parameter
	string listName; 		//second parameter
	string param;	 		//third parameter
	char dataType;			//data type name of the list (i,s,d)

	//VARIABLES TO ASSIST STRING TOKENIZATION FOR COMMANDS
	size_t index = 0;		//index used to tokenize string
	string delim = " "; 	//delimeter of space to tokenize the string commands

	//PUSH VARIABLES
	int intToPush;			//int to push
	string stringToPush;	//string to push
	double doubleToPush;	//double to push

	//Input and Output File Stream
	ifstream inFile;
	ofstream outFile;

	//Prompt User for input and output text file
	cout << "Enter command text file name:";
	cin >> inputName;
	cout << "Enter output file name:";
	cin >> outputName;

	//STRINGS FOR LOGGING VARIOUS ERRORS
	string processing = "PROCESSING COMMAND: ";
	string valuePopped = "Value popped: ";
	string listExistsError = "ERROR: This name already exists!";
	string listEmptyError = "ERROR: This list is empty!";
	string listNotFoundError = "ERROR: This name does not exist!";
	string parameterError = "ERROR: This is not a valid parameter. A valid parameter is either \"stack\" or \"queue\"";
	string dataTypeError = "ERROR: This is not a valid data type. A valid data type is either \"int,\" \"string,\" or \"double\"";
	string commandError = "ERROR: This is not a valid command. A valid command is either \"create,\" \"push,\" or \"pop\"";
	
	//OPEN FILES TO READ AND WRITE
	inFile.open(inputName.c_str());
	outFile.open(outputName.c_str());

	//standard list of integer, double, and string list
	list<SimpleList<int> *> listSLi;
	list<SimpleList<double> *> listSLd;
	list<SimpleList<string> *> listSLs;

	//pointers to SimpleList extracted from standard list that will be used to create, push, and pop data types
	SimpleList<int> *pSLi;
	SimpleList<string> *pSLs;
	SimpleList<double> *pSLd;
	
	//Read in command line by line until the end of read file
	while (!inFile.eof()){
		
		//Read line
		getline(inFile, inLine);

		//Make sure the last read in line is not registered.
		if(inLine==""){
			break;
		}
		//log processing command
		outFile<< processing << inLine << endl;

		//tokenize first substring and store it in command
		index = inLine.find(delim);
		command = inLine.substr(0,index);
    	inLine = inLine.substr(index + delim.size(), inLine.size());	//deletes the stored substring

    	//tokenize second substring and store it in list_name
        index = inLine.find(delim);
		listName = inLine.substr(0,index);
		inLine = inLine.substr(index + delim.size(), inLine.size());	//deletes the stored substring
    	
    	//store the leftover string in param
    	//if there is no third variable, we will just assign it the value of second variable and not use this
		param = inLine;

		//Define the data type of the list being accessed (i,s,d)
		dataType = listName.at(0);

		//if command is create, create stack or queue
		if(command == "create"){													//Command is to create
			if(dataType == 'i' && search(listSLi, listName) == NULL){				//The data type is an int and list with the same name does not exist
				if(param == "stack"){												//if third parameter is stack
					pSLi = new Stack<int>();										//create a stack
					pSLi -> setName(listName);										//set the name of the stack
					listSLi.push_front(pSLi);										//push the stack to the standard list
				}else if(param == "queue"){											//if third parameter is queue
					pSLi = new Queue<int>();										//create a queue
					pSLi -> setName(listName);										//set the name of the queue
					listSLi.push_front(pSLi);										//push the queue to the standard list
				}else{
					outFile<<parameterError<<endl;									//if third parameter is not stack or queue report error.
				}																	//This will not occur for this assignment since we assumed inputs are correct

			}else if(dataType == 's' && search(listSLs, listName) == NULL){			//The data type was a string
				if(param == "stack"){												//Same thing as above in the case of int
					pSLs = new Stack<string>();
					pSLs -> setName(listName);
					listSLs.push_front(pSLs);
				}else if(param == "queue"){
					pSLs = new Queue<string>();
					pSLs -> setName(listName);
					listSLs.push_front(pSLs);
				}else{
					outFile<<parameterError<<endl;
				}

			}else if(dataType == 'd' && search(listSLd, listName) == NULL){			//The data type was a double
				if(param == "stack"){												//Same thing as above in the case of int
					pSLd = new Stack<double>();
					pSLd -> setName(listName);
					listSLd.push_front(pSLd);
				}else if(param == "queue"){
					pSLd = new Queue<double>();
					pSLd -> setName(listName);
					listSLd.push_front(pSLd);
				}else{
					outFile<<parameterError<<endl;
				}

			}else if(search(listSLi,listName)||search(listSLs,listName)||search(listSLd,listName)){
				//if list is found in any one of the list, log that a list with the same name exist
				outFile<<listExistsError<<endl;
			}else{		//if the data type does not correspond to i, s, or d, log data type error
				outFile<< dataTypeError << endl;
			}

		} else if (command == "push"){							//PUSH OPERATION
			if(dataType == 'i'){								//if the data type was an int
				//TYPE CASTING
				//intToPush = stoi(param);						//**stoi introduced in C++11 and not working in cygwin gcc compiler
				stringstream parser(param);						//declare a string stream that would cast string to int
				parser >> intToPush;							//input the stream into an int

				pSLi = search(listSLi,listName);				//search SimpleList from standard list
				if(pSLi != NULL){								//if SimpleList with the name exists
					pSLi -> push(intToPush);					//push the int
				}else{											//if SimpleList is not found
					outFile<<listNotFoundError<<endl;			//log list not found error
				}

			}else if(dataType == 's'){							//The data type was a string

				stringToPush = param;							//set string to push as param
				pSLs = search(listSLs,listName);				//similar code as above in the case of int

				if(pSLs != NULL){
					pSLs -> push(stringToPush);
				}else{
					outFile<<listNotFoundError<<endl;
				}

			}else if(dataType == 'd'){							//The data type was a double
				//TYPE CASTING
				//doubleToPush = stod(param);//stod introduced in C++11 and not working in cygwin gcc compiler
				stringstream parser(param);						//similar code as above in the case of int
				parser >> doubleToPush;

				pSLd = search(listSLd,listName);
				if(pSLd != NULL){
					pSLd -> push(doubleToPush);
				}else{
					outFile<<listNotFoundError<<endl;
				}

			}else{
				outFile<< dataTypeError << endl;				//if data type is not i,s,d, log data type error
			}

		} else if (command == "pop"){							//POP OPERATION
			if(dataType == 'i'){								//The data type was an int
				pSLi = search(listSLi, listName);				//search SimpleList from standard list
				if(pSLi!=NULL&&!(pSLi->isEmpty())){				//if list exist and is not empty
					outFile<< valuePopped <<pSLi->pop()<<endl;	//pop the element and log the popped value
				}else if(pSLi!=NULL&&pSLi ->isEmpty()){			//if list exists but is empty
					outFile<<listEmptyError<<endl;				//log list empty error since we cannot pop from an empty list
				}else{
					outFile<<listNotFoundError<<endl;			//if no SimpleList was found, log list not found error
				}

			}else if(dataType == 's'){							//The data type was a string
				pSLs = search(listSLs, listName);				//repeat the similar case as in above case of int
				if(pSLs!=NULL&&!(pSLs->isEmpty())){
					outFile<< valuePopped <<pSLs->pop()<<endl;
				}else if(pSLs!=NULL&&pSLs ->isEmpty()){
					outFile<<listEmptyError<<endl;
				}else{
					outFile<<listNotFoundError<<endl;
				}

			}else if(dataType == 'd'){							//The data type was a double
				pSLd = search(listSLd, listName);				//repeat the similar case as in above case of int
				if(pSLd!=NULL&&!(pSLd->isEmpty())){
					outFile<< valuePopped <<pSLd->pop()<<endl;
				}else if(pSLd!=NULL&&pSLd ->isEmpty()){
					outFile<<listEmptyError<<endl;
				}else{
					outFile<<listNotFoundError<<endl;
				}

			}else{
				outFile<< dataTypeError << endl;				//if no matching data type was found, log data type error
			}
		} else {
			outFile << commandError << endl;					//if no matching command was found, log command error
		}
	}

	//Close file stream
	inFile.close();
	outFile.close();
	
}
