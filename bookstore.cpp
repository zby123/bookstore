#include <iostream>
#include <fstream>
#include <algorithm>
#include "user.hpp"
#include "books.hpp"
#include "DB.hpp"
#include "shell.hpp"
#include "tstring.hpp"

int main(){

	fstream fin;
	fin.open("command.txt", fstream::in);
	fstream test;
	test.open("user.bin");
	if (!test) {
		DB<tstring<30>, user> userDB;
		userDB.init("user");
		user root;
		root.setId("root");
		root.setPwd("sjtu");
		root.setPurview(7);
		root.setName("root");
		userDB.add("root", root);
		test.close();
	}
	shell cmd;
	cmd.init();

	if (!fin) {
		//TODO 
	}
	else {
		cmd.exec("su root sjtu");
		cmd.exec("load command.txt");
	}
	
	fin.close();

	return 0;
}