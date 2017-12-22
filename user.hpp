#ifndef user_hpp
#define user_hpp

#include "tstring.hpp"
using namespace std;

class user {

private:
	tstring<20> name;
	tstring<30> id;
	tstring<30> pwd;
	int purview = 0;

public:
	void setName(const tstring<20> &_name) {
		name = _name;
	}

	void setId(const tstring<30> &_id) {
		id = _id;
	}

	void setPwd(const tstring<30> &_pwd) {
		pwd = _pwd;
	}

	void setPurview(int _pur) {
		purview = _pur;
	}

	tstring<20> getName() {return name;}

	tstring<30> getId() {return id;}

	tstring<30> getPwd() {return pwd;}

	bool login(const tstring<30> &_pwd) {
		return _pwd == pwd;
	}

	int getPurview() {return purview;}

	operator bool() const {
		return purview != 0;
	}

};

#endif