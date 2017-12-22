#ifndef token_hpp
#define token_hpp

#include <string>

class TokenScanner {
private:
	string storge;
	int cur = 0;

public:
	void setInput(string st) {
		storge = st;
	}

	bool hasMoreTokens() {
		while (cur < storge.length() && (storge[cur] == ' ' || storge[cur] == '\n')) cur++;
		return cur < (int)storge.length();
	}

	string nextToken() {
		string ret;
		ret = "";
		while (storge[cur] == ' ' || storge[cur] == '\n') cur++;
		if (storge[cur] == '-' || storge[cur] == '=') {
			ret += storge[cur];
			cur++;
			return ret;
		}
		while (storge[cur] != ' ' && storge[cur] != '\n' && cur < storge.length() && storge[cur] != '=' && storge[cur] != '-') {
			if (storge[cur] == '"'){
				ret += storge[cur];
				cur++;
				while (storge[cur] != '"'){
					ret += storge[cur];
					cur++;
				}
				ret += storge[cur];
				cur++;
				break;
			}
			ret += storge[cur];
			cur++;
		}
		return ret;
	}
};

#endif