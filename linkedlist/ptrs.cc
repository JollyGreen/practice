#include <iostream>
#include <memory>

using namespace std;

struct ListItem {
	int data=0;
	std::unique_ptr<ListItem> next;

	~ListItem();
};

ListItem::~ListItem() {
	cout << "deleted: " << data << endl;
}

class LinkedList {
public:
	LinkedList();
	void pop_front();
	void push_back(int data);
	void erase(int data);
	void display();
	~LinkedList();
private:
	std::unique_ptr<ListItem> root;
	ListItem *last=nullptr;	
};

LinkedList::LinkedList() {
}

void LinkedList::pop_front() {
	ListItem *ptr=root.get();

	cout << "popping front..." << endl;

	if (ptr!=nullptr) {
		if (ptr==last) last=nullptr;
		root=std::move(ptr->next);
	}
}

void LinkedList::push_back(int data) {

	cout << "pushing ..." << data << endl;

	if (root.get()==nullptr) {
		root=std::unique_ptr<ListItem>(new ListItem());
		last=root.get();
		last->data=data;

	} else {
		last->next=std::unique_ptr<ListItem>(new ListItem());
		last=last->next.get();
		last->data=data;
	}
}

void LinkedList::erase(int data) {
	ListItem *ptr=nullptr;
	ListItem *prevptr=nullptr;

	cout << "Erasing..." << data << endl;
	ptr=root.get();
	while (ptr != nullptr) {
		if (ptr->data==data) {
			if (ptr==root.get()) {
				if (last==ptr) last=nullptr;
				root=std::move(ptr->next);
			} else {
				if (last==ptr) {
					last=prevptr;
					prevptr->next=nullptr;
				} else {
					prevptr->next=std::move(ptr->next);
				}

			}
			return;
		}
		prevptr=ptr;
		ptr=ptr->next.get();
	}

}

void LinkedList::display() {
	ListItem *ptr=nullptr;

	ptr=root.get();
	while (ptr != nullptr) {
		cout << ptr->data;
		ptr=ptr->next.get();
		if (ptr != nullptr) cout << ",";
	}
	cout << endl;
}

LinkedList::~LinkedList() {
}

int main(int argc, char **argv) {
	std::unique_ptr<int> ptr;
	ptr = std::unique_ptr<int>(new int(5));

	cout << "val: " << *ptr << endl;

	LinkedList mylist;
	mylist.push_back(5);
	mylist.push_back(1);
	mylist.push_back(2);
	mylist.push_back(10);
	
	mylist.display();
	mylist.pop_front();
	mylist.display();
	mylist.erase(1);
	mylist.display();
	mylist.erase(5);
	mylist.display();
	mylist.push_back(7);
	mylist.display();
	mylist.erase(10);
	mylist.display();
	mylist.erase(2);
	mylist.display();
}
