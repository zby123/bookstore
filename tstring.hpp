#ifndef tstring_hpp
#define tstring_hpp

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;

template <int size>
class tstring {

private:
	char m[size];
	int len = 0;

public:

	tstring() {
		len = 0;
	}

	tstring(int t) {
		if (t == 0) {
			len = 0;			
		}
		else{
			len = size;
			for(int i = 0; i < size; i++) m[i] = 127;
		}
	}

	tstring (const char *b) {
		len = min(size, (int)strlen(b));
		for(int i = 0; i < len; i++) m[i] = b[i];
	}

	tstring (char *b) {
		len = min(size, (int)strlen(b));
		for(int i = 0; i < len; i++) m[i] = b[i];
	}

	tstring(string &b) {
		len = min(size, (int)b.length());
		for(int i = 0; i < len; i++) m[i] = b[i];
	}

	int getLen() {
		return len;
	}

	void print() {
		for(int i = 0; i < len; i++) cout << m[i];
	}

	template <int s2>
	tstring(tstring<s2> &b) {
		len = min(b.getLen(), size);
		for(int i = 0; i < len; i++) m[i] = b[i];
	}

	operator bool () const {
		return len != 0;
	}

	operator string () const {
		string ret = "";
		for(int i = 0; i < len; i++) ret += m[i];
			return ret;
	}

	char operator [] (int pos) {
		if (pos > len) return '\0';
		return m[pos];
	}

	template <int s2>
	bool operator == (tstring<s2> &b) {
		if (b.getLen() != len) return false;
		for(int i = 0; i < len; i++) {
			if (m[i] != b[i]) return false;
		}
		return  true;
	}

	template <int s2>
	bool operator < (tstring<s2> &b) {
		int tlen = min(len, b.getLen());
		for(int i = 0; i < tlen; i++) {
			if (m[i] < b[i]) return true;
			if (m[i] > b[i]) return false;
		}
		if (len < b.getLen()) return true;
		return  false;
	}

};

#endif