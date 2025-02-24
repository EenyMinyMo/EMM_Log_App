#include <iostream>

class Member {
public: 
	Member() {
		std::cout << "Member constructor " << this <<"\n";
	}
	Member(const Member &m) {
		std::cout << "Member constructor& " << this <<"\n";
	}
	Member(const Member &&m) {
		std::cout << "Member constructor&& " << this <<"\n";
	}

	Member& operator=(const Member &m) {
		std::cout << "Member operator= " << this <<"\n";
		return *this;
	}
	Member& operator=(const Member &&m) {
		std::cout << "Member operator=&& " << this <<"\n";
		return *this;
	}

	~Member() {
		std::cout << "Member destructor " << this <<"\n";
	}
};

class Base {
	Member m;
	int t;

	void checkT() {
		if (t == 2) {
			throw 1;
		}
	}
public:
	Base() : m(), t(0) {
		std::cout << "Base constructor t(" << t << ") " << this <<"\n";
		checkT();
	}
	Base(const Base &b): m(b.m), t(b.t + 1) {
		std::cout << "Base constructor t(" << t << ")& " << this <<"\n";
		checkT();
	}
	Base(const Base &&b): m(std::move(b.m)), t(std::move(b.t + 1)) {
		std::cout << "Base constructor t(" << t << ")&& " << this <<"\n";
		checkT();
	}

	Base& operator=(const Base &b) {
		t = b.t + 1;
		std::cout << "Base operator= t(" << t << ") " << this <<"\n";

		m = b.m;

		checkT();
		return *this;
	}
	Base& operator=(const Base &&b) {
		t = std::move(b.t + 1);
		std::cout << "Base operator=&& t(" << t << ") " << this <<"\n";

		m = std::move(b.m);
		
		checkT();
		return *this;
	}

	~Base() {
		std::cout << "Base destructor t(" << t << ") " << this <<"\n";
	}
};

int main() {
	try {
		//Member m;
		Base b;
		Base b1 = b;
		Base b2 = b1;
		Base b3 = b2;
		Base b4 = b3;
	} catch (int &i) {
		std::cout << "catch " << i << '\n';
	}

	return 0;
}