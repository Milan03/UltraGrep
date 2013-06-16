#ifndef __MATCH_H__
#define __MATCH_H__

#include <string>

using namespace std;


class Match {
private:
	int _numLine;
	wstring _strLine;
	wstring _fileName;

public:
	Match() : _numLine(0), _strLine(L""), _fileName(L"") {}
	Match( wstring fileName ) : _numLine(0), _strLine(L""), _fileName( fileName ) {}
	virtual ~Match() {}

	int getLineNumber() { return _numLine; }
	void setLineNumber ( int lineNum ) { _numLine = lineNum; };
	wstring getLine() { return _strLine; }
	void setLine ( wstring line ) { _strLine = line; };
	wstring getFile() { return _fileName; }
};


#endif