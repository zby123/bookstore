#ifndef shell_hpp
#define shell_hpp

#include "tokenscanner.h"
#include "DB.hpp"
#include "book.hpp"
#include "user.hpp"
#include "tstring.hpp"
#include <fstream>
#include <cstdio>
#include <string>

class shell {
//TODO: add log everyhere
private:
	user cur;
	book cur_b;
	DB<tstring<30>, user> userDB;
	DB<tstring<20>, book> bookDB;
	DB<int, double> financeDB;
	DB<int, tstring<100>> logDB;
	lDB<tstring<40>, tstring<20>> nameDB;
	lDB<tstring<40>, tstring<20>> authDB;
	lDB<tstring<40>, tstring<20>> keywdDB;

public:
	void init(){
		userDB.init("user");
		bookDB.init("book");
		nameDB.init("name");
		authDB.init("auth");
		keywdDB.init("keywd");
		financeDB.init("finance");
		financeDB.add(1, 0);
		financeDB.add(2, 0);
		logDB.init("log");
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
		cur.setPurview(0);
		cur.setId("unlogin")
	}

	void useradd(tstring<30> user_id, tstring<30> pwd, int purview, tstring<20> name) {
		if (purview < cur.getPurview()) error();
		user u;
		if (userDB.exist(user_id)) error();
		u.setId(user_id);
		u.setPwd(pwd);
		u.setPurview(purview);
		u.setName(name);
		userDB.add(user_id, u);
	}

	void regist(tstring<30> user_id, tstring<30> pwd, tstring<20> name){
		user u;
		u.setId(user_id);
		u.setPwd(pwd);
		u.setPurview(1);
		u.setName(name);
		userDB.add(user_id, u);
	}

	void del(tstring<30> user_id) {
		if (cur.getPurview < 7) error();
		userDB.del(user_id);
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
		if (cur.getPurview() < 3) error();
		if (!cur_b.getIsbn()) error();
		bookDB.del(cur_b.getIsbn());
		if (cur_b.getAuth()) authDB.modify(cur_b.getAuth(), cur_b.getIsbn(), isbn);
		if (cur_b.getName()) nameDB.modify(cur_b.getName(), cur_b.getIsbn(), isbn);
		if (cur_b.getKeywd()) {
			tstring<40> tkwd;
			tstring<40> cur_kwd = cur_b.getKeywd();
			int t = 0, len = cur_kwd.getLen();
			while (t < len) {
				tkwd = "";
				while (cur_kwd[t] != '|') {
					tkwd += cur_kwd[t];
					t++;
				}
				t++;
				keywdDB.modify(tkwd, cur_b.getIsbn(), isbn);
			}
		}
		cur_b.modifyIsbn(isbn);
		bookDB.add(cur_b.getIsbn(), cur_b);
	}

	void modifyName(tstring<40> tname) {
		if (cur.getPurview() < 3) error();
		if (!cur_b.getIsbn()) error();
		if (cur_b.getName()) {
			nameDB.del(cur_b.getName(), cur_b.getIsbn());
			nameDB.add(tname, cur_b.getIsbn());
		}
		else nameDB.add(tname, cur_b.getIsbn());
		cur_b.modifyName(tname);
		bookDB.modify(cur_b.getIsbn(), cur_b);
	}

	void modifyAuth(tstring<40> tauth) {
		if (cur.getPurview() < 3) error();
		if (!cur_b.getIsbn()) error();
		if (cur_b.getAuth()) {
			authDB.del(cur_b.getAuth(), cur_b.getIsbn());
			authDB.add(tauth, cur_b.getIsbn());
		}
		else authDB.add(tauth, cur_b.getIsbn());
		cur_b.modifyName(tname);
		bookDB.modify(cur_b.getIsbn(), cur_b);	
	}

	void modifyKwd(tstring<40> tkwd) {
		if (cur.getPurview() < 3) error();
		if (!cur_b.getIsbn()) error();
		tstring<40> ttkwd;
		tstring<40> cur_kwd = cur_b.getKeywd();
		int t = 0, len = cur_kwd.getLen();
		while (t < len) {
			ttkwd = "";
			while (cur_kwd[t] != '|') {
				ttkwd += cur_kwd[t];
				t++;
			}
			t++;
			keywdDB.del(tkwd, cur_b.getIsbn());
		}
		t = 0, len = tkwd.getLen();
		while (t < len) {
			ttkwd = "";
			while (tkwd[t] != '|') {
				ttkwd += tkwd[t];
				t++;
			}
			t++;
			keywdDB.add(tkwd, cur_b.getIsbn());
		}
	}

	void modifyPrice(int pri) {
		if (cur.getPurview() < 3) error();
		if (!cur_b.getIsbn()) error();
		cur_b.modifyPrice(pri);
		bookDB.modify(cur_b.getIsbn(), cur_b);
	}

	void import(int quan, double pri) {
		if (cur.getPurview() < 3) error();
		if (!cur_b.getIsbn()) error();
		cur_b.setQuan(cur_b.getQuan() + quan);
		bookDB.modify(cur_b.getIsbn(), cur_b);
		int tot = financeDB.size();
		financeDB.modify(2, financeDB.find(2) + pri);
		financeDB.add(tot + 1, -pri);
	}

	void buy(tstring<20> isbn, int quan) {
		if (cur.getPurview() < 1) error();
		if (!bookDB.exist(isbn)) error();
		book b = bookDB.find(isbn);
		if (b.getQuan() - quan < 0) error();
		b.setQuan(b.getQuan() - quan);
		bookDB.modify(isbn, b);
		int tot = financeDB.size();
		financeDB.modify(1, financeDB.find(1) + quan * b.getPrice());
		financeDB.add(tot + 1, quan * b.getPrice());
	}

	void showByIsbn(tstring<20> isbn) {
		if (cur.getPurview() < 1) error();
		if (!bookDB.exist(isbn)) error();
		book u = bookDB.find(isbn);
		u.print();
	}

	void showByName(tstring<40> name) {
		if (cur.getPurview() < 1) error();
		vector<tstring<20>> tisbn;
		auto it = namdDB.find(name);
		while (it) {
			tisbn.push(*it);
			it++;
		}
		sort(tisbn.begin(), tisbn.end());
		book b;
		for(int i = 0; i < tisbn.size(); i++) {
			b = bookDB.find(tisbn[i]);
			b.print();
		}
	}

	void showByAuth(tstring<40> auth) {
		if (cur.getPurview() < 1) error();
		vector<tstring<20>> tisbn;
		auto it = authDB.find(auth);
		while (it) {
			tisbn.push(*it);
			it++;
		}
		sort(tisbn.begin(), tisbn.end());
		book b;
		for(int i = 0; i < tisbn.size(); i++) {
			b = bookDB.find(tisbn[i]);
			b.print();
		}
	}

	void showByKwd(tstring<40> kwd) {
		if (cur.getPurview() < 1) error();
		vector<tstring<20>> tisbn;
		auto it = kwdDB.find(kwd);
		while (it) {
			tisbn.push(*it);
			it++;
		}
		sort(tisbn.begin(), tisbn.end());
		book b;
		for(int i = 0; i < tisbn.size(); i++) {
			b = bookDB.find(tisbn[i]);
			b.print();
		}
	}

	void buy(tstring<20> isbn, int quan) {
		if (cur.getPurview() < 1) error();
		if (!bookDB.exist(isbn)) error();
		book b = bookDB.find(isbn);
		if (b.getQuan() < quan) error();
		b.setQuan(b.getQuan() - quan);
		bookDB.modify(isbn, b);

	}

	void showfinance(int time = 1e9) {
		int tot = financeDB.size();
		double inp = 0, oup = 0;
		if (time >= tot - 2) {
			inp = financeDB.find(1);
			oup = financeDB.find(2);
		}
		else {
			for(int i = tot; i >= tot - time + 1; i--) {
				double t = financeDB.find(i);
				if (t > 0) inp += t;
				else opu -= t;
			}
		}
		printf("+ %.2lf - %.2lf\n", inp, oup);
	}

	void exec(string cmd) {
		TokenScanner sc;
		sc.ignoreWhitespace();
		sc.scanStrings();
		sc.setInput(cmd);
		if (!sc.hasMoreTokens()) error();
		string token = sc.nextToken();
		switch (token) {
			case "load":
				if (!sc.hasMoreTokens()) error();
				string dis = sc.nextToken();
				if (sc.hasMoreTokens()) error();
				load(dis);
				break;

			case "su":
				if (!sc.hasMoreTokens()) error();
				tstring<30> id = sc.nextToken();
				if (!sc.hasMoreTokens()) error();
				tstring<30> pwd = sc.nextToken();
				if (sc.hasMoreTokens()) error();
				su(id, pwd);
				break;

			case "logout":
				if (sc.hasMoreTokens()) error();
				logout();
				break;

			case "useradd":
				if (!sc.hasMoreTokens()) error();
				tstring<30> id = sc.nextToken();
				if (!sc.hasMoreTokens()) error();
				tstring<30> pwd = sc.nextToken();
				string pur = sc.nextToken();
				if (getTokenType(pur) != NUMBER) error();
				tstring<20> name = sc.nextToken();
				if (sc.hasMoreTokens()) error();
				useradd(id, pwd, stoi(pur, nullptr), name);
				break;

			case "register":
				if (!sc.hasMoreTokens()) error();
				tstring<30> id = sc.nextToken();
				if (!sc.hasMoreTokens()) error();
				tstring<30> pwd = sc.nextToken();
				if (!sc.hasMoreTokens()) error();
				tstring<20> name = sc.nextToken();
				if (sc.hasMoreTokens()) error();
				registe(id, pwd, name);
				break;

			case "delete":
				if (!sc.hasMoreTokens()) error();
				tstring<30> id = sc.nextToken();
				if (sc.hasMoreTokens()) error();
				del(id);
				break;

			case "passwd":
				if (!sc.hasMoreTokens()) error();
				tstring<30> id = sc.nextToken();
				if (!sc.hasMoreTokens()) error();
				tstring<30> opwd = sc.nextToken();
				if (!sc.hasMoreTokens()){
					passwd(id, opwd);
					break;
				}
				tstring<30> npwd = sc.nextToken();
				if (sc.hasMoreTokens()) error();
				passwd(id, npwd, opwd);
				break;


		}
	}

};	

#endif