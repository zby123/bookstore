#ifndef user_hpp
#define user_hpp

class user {

private:
	char name[30];
	char id[30];
	char pwd[30];
	int auth;

public:
	user(char *_name, char *_pwd, int auth, char *_id) {

	}

};

#endif