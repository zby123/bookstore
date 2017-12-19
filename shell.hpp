#ifndef shell_hpp
#define shell_hpp

#include "scanner.hpp"
#include "DB.hpp"
#include "book.hpp"
#include "user.hpp"
#include "tstring.hpp"
#include <fstream>
#include <string>

class shell {

private:
	user cur;
	book cur_b;
	DB<tstring<30>, user> userDB;
	DB<tstring<20>, book> bookDB;
	DB<tstring<40>, tstring<20>> nameDB;
	DB<tstring<40>, tstring<20>> authDB;
	DB<tstring<40>, tstring<20>> keywdDB;
	//TODO: finance

public:
	void init(){
		userDB.init("user.bin");
		bookDB.init("book.bin");
		nameDB.init("name.bin");
		authDB.init("auth.bin");
		keywdDB.init("keywd.bin");
	}

	void exec(string cmd);

	void load(string dis){
		if (cur.getPurview() < 7) error();
		fstream inf(dis, fstream::in);
		string cmd;
		while (~scanf("%s", cmd)) {
			exec(cmd);
		}
		inf.close();
	}

	void su(tstring<30> user_id, tstring<30> pwd){
		user u;
		u = userDB.find(user_id);
		if (!u.getPurview()) error();
		bool suc = u.login(pwd);
		if (cur.getPurview() > u.getPurview()) suc = true;
		if (!suc) error();
		cur = u;
	}

	void logout(){
		*cur = nullptr;
	}

	void useradd(tstring<30> user_id, tstring<30> pwd, int purview, tstring<20> name) {
		if (purview < cur.getPurview()) error();
		user u;
		if (userDB.exist(user_id)) error();
		u.modifyId(user_id);
		u.setPwd(pwd);
		u.setPurview(purview);
		u.setName(name);
		userDB.add(user_id, u);
	}

	void regist(tstring<30> user_id, tstring<30> pwd, tstring<20> name){
		user u;
		u.modifyId(user_id);
		u.setPwd(pwd);
		u.setPurview(1);
		u.setName(name);
		userDB.add(user_id, u);
	}

	void del(tstring<30> user_id) {
		if (cur.getPurview < 7) error();
		userDB.remove(user_id);
	}

	void passwd(tstring<30> user_id, tstring<30> newpwd, tstring<30> oldpwd = ""){
		if (oldpwd == "" && cur.getPurview < 7) error();
		user u;
		u = userDB.find(user_id);
		if (!u.getPurview()) error();
		u.setPwd(newpwd);
		userDB.modify(user_id, u);
	}

	void select(tstring<20> isbn) {
		if (cur.getPurview() < 3) error();
		book bk;
		if (bookDB.exist(isbn)) {
			cur_b = bookDB.find(isbn);
			return;
		}
		else {
			bk.setIsbn(isbn);
			bookDB.add(isbn, bk);
			cur_b = bk;
		}
	}

	void modifyIsbn(tstring<20> isbn) {
		if (cur_b.getIsbn()) error();
		bookDB.del(cur_b.getIsbn());
		if (cur_b.getAuth()) authDB.del(cur_b.getAuth());
		if (cur_b.getName()) nameDB.del(cur_b.getName());
		if (cur_b.getKeywd()) {
			tstring<40> tkwd;
			tstring<40> cur_kwd = cur_b.getKeywd();
			int t = 0;
			while (t != )
		}
		cur_b.modifyIsbn(isbn);
		bookDB.add(cur_b.getIsbn(), cur_b);
		if (cur_b.getName()) nameDB.add(cur_b.getName(), cur_b.getIsbn());
	}

};

#endif