#include <iostream>
#include "user.hpp"
#include "books.hpp"
#include "DB.hpp"
#include "cmd.hpp"
#include "tokenscanner.hpp"

int main(){

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