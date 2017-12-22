#ifndef DB_hpp
#define DB_hpp

#include<string>
#include<fstream>
#include<algorithm>
#define rc reinterpret_cast<char*>
using namespace std;

const int B_size = 50;

template <class K, class C>
class DB {

private:
	tstring<30> dis;

public:

	string getdis() {
		return dis;
	}

	struct block {
		K ind[B_size + 1], mx = 1e9;
		C storge[B_size + 1];
		int nxt = 0, tot = 0, pre = 0;
	};

	void init(string _dis) {
		dis = _dis;
		fstream test;
		test.open(_dis + ".bin", fstream::in);
		if (!test) {
			test.open(_dis + ".bin", fstream::trunc | fstream::out | fstream::binary);
			int tot = 1;
			test.write(rc(&tot), sizeof(int));
			tot = 1;
			test.write(rc(&tot), sizeof(int));
			block t;
			test.write(rc(&t), sizeof(block));
			test.close();
		}
	}

	C find(K key) {
		fstream bin;
		string _dis = dis;
		bin.open(_dis + ".bin", fstream::in | fstream::binary);
		int tot;
		bin.read(rc(&tot), sizeof(int));
		int cur;
		bin.read(rc(&cur), sizeof(int));
		block tb;
		C ret;
		while (cur) {
			bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
			bin.read(rc(&tb), sizeof(block));
			if (key < tb.mx) {
				for(int j = 0; j < tb.tot; j++) if (key == tb.ind[j]) {
					ret = tb.storge[j];
					break;
				}
				bin.close();
				return ret;
			}
			cur = tb.nxt;
		}
		bin.close();
		return ret;
	}

	bool exist(K key) {
		return find(key);
	}

	void modify(K key, C val) {
		fstream bin;
		string _dis = dis;
		bin.open(_dis + ".bin", fstream::in | fstream::out | fstream::binary);
		int tot;
		bin.read(rc(&tot), sizeof(int));
		int cur;
		bin.read(rc(&cur), sizeof(int));
		block tb;
		while (cur) {
			bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
			bin.read(rc(&tb), sizeof(block));
			if (key < tb.mx || key == tb.mx) {
				for(int j = 0; j < tb.tot; j++) if (key == tb.ind[j]) {
					tb.storge[j] = val;
					break;
				}
				bin.seekp(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
				bin.write(rc(&tb), sizeof(block));
				bin.close();
				return;
			}
			cur = tb.nxt;
		}
		bin.close();
		return;
	}

	void add(K key, C val) {
		fstream bin;
		string _dis = dis;
		bin.open(_dis + ".bin", fstream::in | fstream::out | fstream::binary);
		int tot;
		bin.read(rc(&tot), sizeof(int));
		int cur;
		bin.read(rc(&cur), sizeof(int));
		block tb;
		while (cur) {
			bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
			bin.read(rc(&tb), sizeof(block));
			if (key < tb.mx) {
				for(int j = 0; j <= tb.tot; j++) if (key < tb.ind[j] || j == tb.tot) {
					for(int k = tb.tot; k > j; k--)  {
						tb.ind[k] = tb.ind[k - 1];
						tb.storge[k] = tb.storge[k - 1];
					}
					tb.ind[j] = key;
					tb.storge[j] = val;
					if (j == 0 && tb.pre) {
						block qb;
						bin.seekg(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
						bin.read(rc(&qb), sizeof(block));
						qb.mx = key;
						bin.seekp(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
						bin.write(rc(&qb), sizeof(block));
					}
					tb.tot++;
					if (tb.tot >= B_size) {
						block qb;
						for(int k = 0; k < B_size / 2; k++) {
							qb.ind[k] = tb.ind[k + B_size / 2];
							qb.storge[k] = tb.storge[k + B_size / 2];
						}
						qb.nxt = tb.nxt;
						tb.nxt = tot + 1;
						qb.pre = cur;
						qb.tot = tb.tot = B_size / 2;
						qb.mx = tb.mx;
						tb.mx = qb.ind[0];
						bin.seekp(sizeof(int) * 2 + sizeof(block) * tot, ios::beg);
						bin.write(rc(&qb), sizeof(block));
						bin.seekp(0, ios::beg);
						tot++;
						bin.write(rc(&tot), sizeof(int));
					}
					bin.seekp(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
					bin.write(rc(&tb), sizeof(block));
					bin.close();
					return;
				}
			}
			cur = tb.nxt;
		}
		bin.close();
		return;
	}

	void del(K key) {
		fstream bin;
		string _dis = dis;
		bin.open(_dis + ".bin", fstream::in | fstream::out | fstream::binary);
		int tot;
		bin.read(rc(&tot), sizeof(int));
		int cur;
		bin.read(rc(&cur), sizeof(int));
		block tb;
		while (cur) {
			bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
			bin.read(rc(&tb), sizeof(block));
			if (key < tb.mx) {
				for(int j = 0; j < tb.tot; j++) if (key == tb.ind[j]) {
					for(int k = j; k < tb.tot; k++) {
						tb.ind[k] = tb.ind[k + 1];
						tb.storge[k] = tb.storge[k + 1];
					}
					if (j == 0 && tb.pre) {
						block qb;
						bin.seekg(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
						bin.read(rc(&qb), sizeof(block));
						qb.mx = tb.ind[0];
						bin.seekp(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
						bin.write(rc(&qb), sizeof(block));
					}
					tb.tot--;
					if (!tb.tot && tot != 1) {
						if (tb.pre) {
							block qb;
							bin.seekg(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
							bin.read(rc(&qb), sizeof(block));
							qb.mx = tb.mx;
							qb.nxt = tb.nxt;
							bin.seekp(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
							bin.write(rc(&qb), sizeof(block));
						}
						else {
							int st = tb.nxt;
							bin.seekp(sizeof(int), ios::beg);
							bin.write(rc(&st), sizeof(int));
						}
						block qb;
						bin.seekg(sizeof(int) * 2 + sizeof(block) * (tot - 1), ios::beg);
						bin.read(rc(&qb), sizeof(block));
						memcpy(&tb, &qb, sizeof(block));
						if (tb.pre) {
							block fb;
							bin.seekg(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
							bin.read(rc(&fb), sizeof(block));
							qb.nxt = cur;
							bin.seekp(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
							bin.write(rc(&qb), sizeof(block));
						}
						tot--;
						bin.seekp(0, ios::beg);
						bin.write(rc(&tot), sizeof(int));
					}
					bin.seekp(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
					bin.write(rc(&tb), sizeof(block));
					bin.close();
					return;
				}
			}
			cur = tb.nxt;
		}
		bin.close();
		return;
	}

	class iterator {
		friend class DB;

	private:
		int cur, pos, mx;
		tstring<30> dis = 0;

	public:
		iterator(){
			pos = mx = 0;
			cur = 1;
		}

		iterator(int t) {
			pos = mx = 0;
			cur = 1;
		}
		
		iterator(const iterator &) = default;

		//~iterator() {if (d) delete d;}
		
		iterator &operator=(const iterator &) = default;

		iterator &operator++()
		{
			fstream bin;
			string _dis = dis;
			bin.open(_dis + ".bin", fstream::in | fstream::binary);
			bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1));
			block tb;
			bin.read(rc(&tb), sizeof(block));
			if (pos == tb.tot - 1) cur++, pos = 0;
			else pos++;
			bin.close();
			return (*this);
		}
		
		iterator operator++(int)
		{
			fstream bin;
			string _dis = dis;
			bin.open(_dis + ".bin", fstream::in | fstream::binary);
			bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1));
			block tb;
			bin.read(rc(&tb), sizeof(block));
			if (pos == tb.tot - 1) cur++, pos = 0;
			else pos++;
			bin.close();
			return (*this);
		}

		C operator*() const
		{
			fstream bin;
			string _dis = dis;
			bin.open(_dis + ".bin", fstream::in | fstream::binary);
			bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1));
			block tb;
			bin.read(rc(&tb), sizeof(block));
			bin.close();
			return tb.storge[pos];
		}
		
		C * operator->() const
		{
			return &(operator*());
		}

		operator bool() const {
			return cur <= mx;
		}
	};

	iterator begin() {
		iterator ret;
		ret.cur = 1; ret.pos = 0;
		fstream bin;		
		string _dis = dis;
		bin.open(_dis + ".bin", fstream::in | fstream::binary);
		int tot;
		bin.read(rc(&tot), sizeof(int));
		bin.close();
		ret.mx = tot;
		ret.dis = dis;
		return ret;
	}

	iterator it0() {
		iterator ret = 0;
		return ret;
	}

};

template <class K, class C>
class lDB {

private:
	string dis;
	int tots;

public:

	struct block {
		K ind[B_size + 1], mx = 1e9;
		DB<C, C> storge[B_size + 1];
		int nxt = 0, tot = 0, pre = 0;
	};

	void init(string _dis) {
		dis = _dis;
		fstream test;
		test.open(dis + ".bin", fstream::in);
		if (!test) {
			test.open(dis + ".bin", fstream::trunc | fstream::out | fstream::binary);
			int tot = 1;
			test.write(rc(&tot), sizeof(int));
			tot = 1;
			test.write(rc(&tot), sizeof(int));
			tots = 0;
			test.write(rc(&tots), sizeof(int));
			block t;
			test.write(rc(&t), sizeof(block));
			test.close();
		}
	}

	auto find(K key) {
		fstream bin;
		bin.open(dis + ".bin", fstream::in | fstream::binary);
		int tot;
		bin.read(rc(&tot), sizeof(int));
		int cur;
		bin.read(rc(&cur), sizeof(int));
		bin.read(rc(&tots), sizeof(int));
		block tb;
		auto ret = tb.storge[0].it0();
		while (cur) {
			bin.seekg(sizeof(int) * 3 + sizeof(block) * (cur - 1), ios::beg);
			bin.read(rc(&tb), sizeof(block));
			if (key < tb.mx) {
				for(int j = 0; j < tb.tot; j++) if (key == tb.ind[j]) {
					bin.close();
					return (tb.storge[j].begin());
				}
				return ret;
			}
			cur = tb.nxt;
		}
		bin.close();
		return ret;
	}

	void modify(K key, C val, C nval) {
		fstream bin;
		bin.open(dis + ".bin", fstream::in | fstream::binary);
		int tot;
		bin.read(rc(&tot), sizeof(int));
		int cur;
		bin.read(rc(&cur), sizeof(int));
		bin.read(rc(&tots), sizeof(int));
		block tb;
		while (cur) {
			bin.seekg(sizeof(int) * 3 + sizeof(block) * (cur - 1), ios::beg);
			bin.read(rc(&tb), sizeof(block));
			if (key <tb.mx || key ==tb.mx) {
				for(int j = 0; j < tb.tot; j++) if (key == tb.ind[j]) {
					tb.storge[j].del(val);
					tb.storge[j].add(nval, nval);
					break;
				}
				bin.close();
				return;
			}
			cur = tb.nxt;
		}
		bin.close();
		return;
	}

	void add(K key, C val) {
		fstream bin;
		bin.open(dis + ".bin", fstream::in | fstream::out | fstream::binary);
		int tot;
		bin.read(rc(&tot), sizeof(int));
		int cur;
		bin.read(rc(&cur), sizeof(int));
		bin.read(rc(&tots), sizeof(int));
		block tb;
		while (cur) {
			bin.seekg(sizeof(int) * 3 + sizeof(block) * (cur - 1), ios::beg);
			bin.read(rc(&tb), sizeof(block));
			if (key <tb.mx) {
				for(int j = 0; j <= tb.tot; j++){ 
					if (key == tb.ind[j]) {
						tb.storge[j].add(val, val);
						return;
					}
					if (key < tb.ind[j] || j == tb.tot) {
						for(int k = tb.tot; k > j; k--)  {
							tb.ind[k] = tb.ind[k - 1];
							tb.storge[k] = tb.storge[k - 1];
						}
						tb.ind[j] = key;
						tots++;
						bin.seekp(sizeof(int) * 2, ios::beg);
						bin.write(rc(&tots), sizeof(int));
						tb.storge[j].init(dis + to_string(tots));
						tb.storge[j].add(val, val);
						if (j == 0 && tb.pre) {
							block qb;
							bin.seekg(sizeof(int) * 3 + sizeof(block) * (tb.pre - 1), ios::beg);
							bin.read(rc(&qb), sizeof(block));
							qb.mx = key;
							bin.seekp(sizeof(int) * 3 + sizeof(block) * (tb.pre - 1), ios::beg);
							bin.write(rc(&qb), sizeof(block));
						}
						tb.tot++;
						if (tb.tot >= B_size) {
							block qb;
							for(int k = 0; k < B_size / 2; k++) {
								qb.ind[k] = tb.ind[k + B_size / 2];
								qb.storge[k] = tb.storge[k + B_size / 2];
							}
							qb.nxt = tb.nxt;
							tb.nxt = tot + 1;
							qb.pre = cur;
							qb.tot = tb.tot = B_size / 2;
							qb.mx = tb.mx;
							tb.mx = qb.ind[0];
							bin.seekp(sizeof(int) * 3 + sizeof(block) * tot, ios::beg);
							bin.write(rc(&qb), sizeof(block));
							bin.seekp(0, ios::beg);
							tot++;
							bin.write(rc(&tot), sizeof(int));
						}
						bin.seekp(sizeof(int) * 3 + sizeof(block) * (cur - 1), ios::beg);
						bin.write(rc(&tb), sizeof(block));
						bin.close();
						return;
					}
				}
			}
		cur = tb.nxt;
		}
		bin.close();
		return;
	}

	void del(K key, C val) {
		fstream bin;
		bin.open(dis + ".bin", fstream::in | fstream::out | fstream::binary);
		int tot;
		bin.read(rc(&tot), sizeof(int));
		int cur;
		bin.read(rc(&cur), sizeof(int));
		block tb;
		while (cur) {
			bin.seekg(sizeof(int) * 3 + sizeof(block) * (cur - 1), ios::beg);
			bin.read(rc(&tb), sizeof(block));
			if (key < tb.mx) {
				for(int j = 0; j < tb.tot; j++) if (key == tb.ind[j]) {
					tb.storge[j].del(val);
					if (j == 0 && tb.pre) {
						block qb;
						bin.seekg(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
						bin.read(rc(&qb), sizeof(block));
						qb.mx = tb.ind[0];
						bin.seekp(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
						bin.write(rc(&qb), sizeof(block));
					}
					return;
				}
			}
			cur = tb.nxt;
		}
		bin.close();
		return;
	}

};

#endif