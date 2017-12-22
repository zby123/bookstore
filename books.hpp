#ifndef books_hpp
#define books_hpp

#include <cstdio>
#include "tstring.hpp"
using namespace std;

class book {

private:
	tstring<20> isbn;
	tstring<40> name, auth, keywd;
	double pri;
	int quan = 0;

public:
	void setIsbn(tstring<20> _isbn) {
		isbn = _isbn;
	}

	void setName(tstring<40> _name) {
		name = _name;
	}

	void setAuth(tstring<40> _auth) {
		auth = _auth;
	}

	void setKeywd(tstring<40> _keywd) {
		keywd = _keywd;
	}

	void setPri(double _pri) {
		pri = _pri;
	}

	void setQuan(int _quan) {
		quan = _quan;
	}

	double getPri() {
		return pri;
	}

	int getQuan() {
		return quan;
	}

	tstring<20> getIsbn() {
		return isbn;
	}

	tstring<40> getAuth() {
		return auth;
	}

	tstring<40> getName() {
		return name;
	}

	tstring<40> getKeywd() {
		return keywd;
	}

	void print() {
		if (!isbn) return;
		isbn.print();
		printf("\t");
		name.print();
		printf("\t");
		auth.print();
		printf("\t");
		keywd.print();
		printf("\t");
		printf("%.2lf", pri);
		printf("\t%d±¾\n", quan);
	}

	operator bool() const {
		return isbn;
	}

};

#endif
