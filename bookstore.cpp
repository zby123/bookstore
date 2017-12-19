#include <iostream>
#include "user.hpp"
#include "books.hpp"
#include "DB.hpp"
#include "cmd.hpp"
#include "tokenscanner.hpp"

int main(){

	DB userDB<user>;
	DB bookDB<book>;

	fstream db("user.bin");
	if (!db){
		userDB.init("user.bin");
		bookDB.init("book.bin");
		user root("root", "sjtu", 7);
		userDB.add(root);
	}
	else {
		userDB.init("user.bin");
		bookDB.init("book.bin");
	}

	fstream fin("command.txt");
	shell cmd;

	if (!fin) {
		//TODO 
	}
	else {
		cmd.exec("su root sjtu");
		cmd.exec("load command.txt");
	}

	return 0;
}