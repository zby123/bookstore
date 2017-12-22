#ifndef shell_hpp
#define shell_hpp

#include "tokenscanner.hpp"
#include "DB.hpp"
#include "books.hpp"
#include "user.hpp"
#include "tstring.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

#define error {printf("Invalid\n"); return;}

class shell {
//TODO: add log everyhere
private:
	user cur;
	book cur_b;
	bool ed = false;
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
		financeDB.add(0, 2);
		financeDB.add(1, 0);
		financeDB.add(2, 0);
		logDB.init("log");
	}

	void su(tstring<30> user_id, tstring<30> pwd = 0){
		user u;
		u = userDB.find(user_id);
		if (!u.getPurview()) error;
		tstring<30> tt = 0;
		if (pwd == tt && cur.getPurview() > u.getPurview()) {
			cur = u;
			//cur.getId().print();
			return;
		}
		else if (pwd == tt) error;
		if (!(u.getPwd() == pwd)) error;
		cur = u;
		//cur.getId().print();
	}

	void logout(){
		if (!cur.getPurview()) error;
		cur.setPurview(0);
		cur.setId("unlogin");
	}

	void useradd(tstring<30> user_id, tstring<30> pwd, int purview, tstring<20> name) {
		if (purview > cur.getPurview()) error;
		if (purview != 1 && purview != 3) error;
		user u;
		if (userDB.exist(user_id)) error;
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
		if (cur.getPurview() < 7) error;
		if (!userDB.exist(user_id)) error;
		userDB.del(user_id);
	}

	void passwd(tstring<30> user_id, tstring<30> newpwd, tstring<30> oldpwd = 0){
		string t = "";
		tstring<30> tt = t;
		if (oldpwd == tt && cur.getPurview() < 7) error;
		user u;
		u = userDB.find(user_id);
		if (!u.getPurview()) error;
		if (oldpwd != tt && !(u.getPwd() == oldpwd)) error;
		u.setPwd(newpwd);
		userDB.modify(user_id, u);
	}

	void select(tstring<20> isbn) {
		if (cur.getPurview() < 3) error;
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
		if (cur.getPurview() < 3) error;
		if (!cur_b.getIsbn()) error;
		if (bookDB.exist(isbn)) error;
		bookDB.del(cur_b.getIsbn());
		if (cur_b.getAuth()) authDB.modify(cur_b.getAuth(), cur_b.getIsbn(), isbn);
		if (cur_b.getName()) nameDB.modify(cur_b.getName(), cur_b.getIsbn(), isbn);
		if (cur_b.getKeywd()) {
			tstring<40> tkwd;
			string tmp;
			tstring<40> cur_kwd = cur_b.getKeywd();
			int t = 0, len = cur_kwd.getLen();
			while (t < len) {
				tmp = "";
				while (cur_kwd[t] != '|') {
					tmp += cur_kwd[t];
					t++;
				}
				t++;
				tkwd = tmp;
				keywdDB.modify(tkwd, cur_b.getIsbn(), isbn);
			}
		}
		cur_b.setIsbn(isbn);
		bookDB.add(cur_b.getIsbn(), cur_b);
	}

	void modifyName(tstring<40> tname) {
		if (cur.getPurview() < 3) error;
		if (!cur_b.getIsbn()) error;
		if (cur_b.getName()) {
			nameDB.del(cur_b.getName(), cur_b.getIsbn());
			nameDB.add(tname, cur_b.getIsbn());
		}
		else nameDB.add(tname, cur_b.getIsbn());
		cur_b.setName(tname);
		bookDB.modify(cur_b.getIsbn(), cur_b);
	}

	void modifyAuth(tstring<40> tauth) {
		if (cur.getPurview() < 3) error;
		if (!cur_b.getIsbn()) error;
		if (cur_b.getAuth()) {
			authDB.del(cur_b.getAuth(), cur_b.getIsbn());
			authDB.add(tauth, cur_b.getIsbn());
		}
		else authDB.add(tauth, cur_b.getIsbn());
		cur_b.setAuth(tauth);
		bookDB.modify(cur_b.getIsbn(), cur_b);	
	}

	void modifyKwd(tstring<40> tkwd) {
		if (cur.getPurview() < 3) error;
		if (!cur_b.getIsbn()) error;
		cur_b.setKeywd(tkwd);
		bookDB.modify(cur_b.getIsbn(), cur_b);
		tstring<40> ttkwd;
		string tmp;
		tstring<40> cur_kwd = cur_b.getKeywd();
		int t = 0, len = cur_kwd.getLen();
		while (t < len) {
			tmp = "";
			while (cur_kwd[t] != '|' && t < len) {
				tmp += cur_kwd[t];
				t++;
			}
			t++;
			ttkwd = tmp;
			keywdDB.del(ttkwd, cur_b.getIsbn());
		}
		t = 0, len = tkwd.getLen();
		while (t < len) {
			tmp = "";
			while (tkwd[t] != '|' && t < len) {
				tmp += tkwd[t];
				t++;
			}
			t++;
			ttkwd = tmp;
			keywdDB.add(ttkwd, cur_b.getIsbn());
		}
	}

	void modifyPrice(double pri) {
		if (cur.getPurview() < 3) error;
		if (!cur_b.getIsbn()) error;
		cur_b.setPri(pri);
		bookDB.modify(cur_b.getIsbn(), cur_b);
	}

	void import(int quan, double pri) {
		if (cur.getPurview() < 3) error;
		if (!cur_b.getIsbn()) error;
		cur_b.setQuan(cur_b.getQuan() + quan);
		bookDB.modify(cur_b.getIsbn(), cur_b);
		int tot = financeDB.find(0);
		financeDB.modify(2, financeDB.find(2) + pri);
		financeDB.add(tot + 1, -pri);
		financeDB.modify(0, tot + 1);
	}

	void buy(tstring<20> isbn, int quan) {
		if (cur.getPurview() < 1) error;
		if (!bookDB.exist(isbn)) error;
		book b = bookDB.find(isbn);
		if (b.getQuan() - quan < 0) error;
		b.setQuan(b.getQuan() - quan);
		bookDB.modify(isbn, b);
		int tot = financeDB.find(0);
		financeDB.modify(1, financeDB.find(1) + quan * b.getPri());
		financeDB.add(tot + 1, quan * b.getPri());
		financeDB.modify(0, tot + 1);
	}

	void showall() {
		auto it = bookDB.begin();
		while (it) {
			(*it).print();
			it++;
		}
	}

	void showByIsbn(tstring<20> isbn) {
		if (cur.getPurview() < 1) error;
		if (!bookDB.exist(isbn)) return;
		book u = bookDB.find(isbn);
		u.print();
	}

	void showByName(tstring<40> name) {
		if (cur.getPurview() < 1) error;
		vector<tstring<20>> tisbn;
		auto it = nameDB.find(name);
		while (it) {
			tisbn.push_back(*it);
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
		if (cur.getPurview() < 1) error;
		vector<tstring<20>> tisbn;
		auto it = authDB.find(auth);
		while (it) {
			tisbn.push_back(*it);
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
		if (cur.getPurview() < 1) error;
		vector<tstring<20>> tisbn;
		auto it = keywdDB.find(kwd);
		while (it) {
			tisbn.push_back(*it);
			it++;
		}
		sort(tisbn.begin(), tisbn.end());
		book b;
		for(int i = 0; i < tisbn.size(); i++) {
			b = bookDB.find(tisbn[i]);
			b.print();
		}
	}

	void showfinance(int time = 1e9) {
		int tot = financeDB.find(0);
		double inp = 0, oup = 0;
		if (time >= tot - 2) {
			inp = financeDB.find(1);
			oup = financeDB.find(2);
		}
		else {
			for(int i = tot; i >= tot - time + 1; i--) {
				double t = financeDB.find(i);
				if (t > 0) inp += t;
				else oup -= t;
			}
		}
		printf("+ %.2lf - %.2lf\n", inp, oup);
	}

	void load(string dis);

	void exec(string cmd) {
		try{
		TokenScanner sc;
		string tmp;
		sc.setInput(cmd);
		if (!sc.hasMoreTokens()) error;
		string token = sc.nextToken();
		if (token == "load"){
			if (!sc.hasMoreTokens()) error;
			string dis = sc.nextToken();
			if (sc.hasMoreTokens()) error;
			load(dis);
			return;
		}
		else if (token == "su"){
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			tstring<30> id = tmp;
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			tstring<30> pwd = tmp;
			if (sc.hasMoreTokens()) error;
			su(id, pwd);
			return;
		}
		else if (token == "logout"){
			if (sc.hasMoreTokens()) error;
			logout();
			return;
		}
		else if (token == "useradd"){
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			tstring<30> id = tmp;
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			tstring<30> pwd = tmp;
			string pur = sc.nextToken();
			tmp = sc.nextToken();
			tstring<20> name = tmp;
			if (sc.hasMoreTokens()) error;
			useradd(id, pwd, stoi(pur, nullptr), name);
			return;
		}

		else if (token == "register"){
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			tstring<30> id = tmp;
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			tstring<30> pwd = tmp;
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			tstring<20> name = tmp;
			if (sc.hasMoreTokens()) error;
			regist(id, pwd, name);
			return;
		}

		else if (token == "delete"){
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			tstring<30> id = tmp;
			if (sc.hasMoreTokens()) error;
			del(id);
			return;
		}

		else if (token == "passwd"){
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			tstring<30> id = tmp;
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			tstring<30> opwd = tmp;
			if (!sc.hasMoreTokens()){
				passwd(id, opwd);
				return;
			}
			tmp = sc.nextToken();
			tstring<30> npwd = tmp;
			if (sc.hasMoreTokens()) error;
			passwd(id, npwd, opwd);
			return;
		}

		else if (token == "select"){
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			tstring<20> isbn = tmp;
			if (sc.hasMoreTokens()) error;
			select(isbn);
			return;
		}

		else if (token == "modify"){
			while (sc.hasMoreTokens()) {
				tmp = sc.nextToken();
				if (tmp != "-") error;
				if (!sc.hasMoreTokens()) error;
				tmp = sc.nextToken();
				if (tmp == "ISBN"){
					if (!sc.hasMoreTokens()) error;
					tmp = sc.nextToken();
					if (tmp != "=") error;
					if (!sc.hasMoreTokens()) error;
					tmp = sc.nextToken();
					modifyIsbn(tmp);
				}

				else if (tmp == "name"){
					if (!sc.hasMoreTokens()) error;
					tmp = sc.nextToken();
					if (tmp != "=") error;
					if (!sc.hasMoreTokens()) error;
					tmp = sc.nextToken();
					if (tmp[0] != '"' || tmp[tmp.length() - 1] != '"') error;
					tmp.erase(tmp.length() - 1, 1);
					tmp.erase(0, 1);
					modifyName(tmp);
				}

				else if (tmp == "author"){
					if (!sc.hasMoreTokens()) error;
					tmp = sc.nextToken();
					if (tmp != "=") error;
					if (!sc.hasMoreTokens()) error;
					tmp = sc.nextToken();
					if (tmp[0] != '"' || tmp[tmp.length() - 1] != '"') error;
					tmp.erase(tmp.length() - 1, 1);
					tmp.erase(0, 1);
					modifyAuth(tmp);
				}

				else if (tmp == "keyword"){
					if (!sc.hasMoreTokens()) error;
					tmp = sc.nextToken();
					if (tmp != "=") error;
					if (!sc.hasMoreTokens()) error;
					tmp = sc.nextToken();
					if (tmp[0] != '"' || tmp[tmp.length() - 1] != '"') error;
					tmp.erase(tmp.length() - 1, 1);
					tmp.erase(0, 1);
					modifyKwd(tmp);
				}

				else if (tmp == "name"){
					if (!sc.hasMoreTokens()) error;
					tmp = sc.nextToken();
					if (tmp != "=") error;
					if (!sc.hasMoreTokens()) error;
					tmp = sc.nextToken();
					if (tmp[0] != '"' || tmp[tmp.length() - 1] != '"') error;
					tmp.erase(tmp.length() - 1, 1);
					tmp.erase(0, 1);
					modifyName(tmp);
				}

				else if (tmp == "price"){
					if (!sc.hasMoreTokens()) error;
					tmp = sc.nextToken();
					if (tmp != "=") error;
					if (!sc.hasMoreTokens()) error;
					tmp = sc.nextToken();
					modifyPrice(stod(tmp));
				}
			}
			return;
		}

		else if (token == "import"){
			if (!sc.hasMoreTokens()) error;
			string tmp = sc.nextToken();
			int quan = stoi(tmp);
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			double pri = stod(tmp);
			import(quan, pri);
			return;
		}

		else if (token == "show"){
			if (!sc.hasMoreTokens()){
				showall();
				return;
			}
			string tmp = sc.nextToken();
			if (tmp == "finance") {
				if (!sc.hasMoreTokens()) showfinance();
				else {
					tmp = sc.nextToken();
					if (sc.hasMoreTokens()) error;
					showfinance(stoi(tmp));
				}
				return;
			}
			if (tmp != "-") error;
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			if (tmp == "ISBN"){
				if (!sc.hasMoreTokens()) error;
				tmp = sc.nextToken();
				if (tmp != "=") error;
				if (!sc.hasMoreTokens()) error;
				tmp = sc.nextToken();
				showByIsbn(tmp);
				return;
			}

			else if (tmp == "name"){
				if (!sc.hasMoreTokens()) error;
				tmp = sc.nextToken();
				if (tmp != "=") error;
				if (!sc.hasMoreTokens()) error;
				tmp = sc.nextToken();
				if (tmp[0] != '"' || tmp[tmp.length() - 1] != '"') error;
				tmp.erase(tmp.length() - 1, 1);
				tmp.erase(0, 1);
				showByName(tmp);
				return;
			}

			else if (tmp == "author"){
				if (!sc.hasMoreTokens()) error;
				tmp = sc.nextToken();
				if (tmp != "=") error;
				if (!sc.hasMoreTokens()) error;
				tmp = sc.nextToken();
				if (tmp[0] != '"' || tmp[tmp.length() - 1] != '"') error;
				tmp.erase(tmp.length() - 1, 1);
				tmp.erase(0, 1);
				showByAuth(tmp);
				return;
			}

			else if (tmp == "keyword"){
				if (!sc.hasMoreTokens()) error;
				tmp = sc.nextToken();
				if (tmp != "=") error;
				if (!sc.hasMoreTokens()) error;
				tmp = sc.nextToken();
				if (tmp[0] != '"' || tmp[tmp.length() - 1] != '"') error;
				tmp.erase(tmp.length() - 1, 1);
				tmp.erase(0, 1);
				showByKwd(tmp);
				return;
			}
			else error;
			return;
		}

		else if (token == "buy"){
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			tstring<20> isbn = tmp;
			if (!sc.hasMoreTokens()) error;
			tmp = sc.nextToken();
			int quan = stoi(tmp);
			buy(isbn, quan);
			return;
		}

		else if (token == "exit") {
			ed = true;
			return;
		}

		else error;
		}
		catch(...) {
			error;
		}

	}
};	

void shell::load(string dis){
	if (cur.getPurview() < 7) error;
	fstream inf(dis, fstream::in);
	string cmd;
	int tot = 0;
	while (!inf.eof()) {
		++tot;
		//printf("%d: \n", tot);
		getline(inf, cmd);
		exec(cmd);
		//puts("");
		if (ed) return;
	}
	inf.close();
}

#endif