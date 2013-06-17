/*
 * File Name: Match.h
 * Date:      June 14, 2013
 * Author:    Milan Sobat
 * Student #: 0469245
 * Course:    INFO-5056
 * Purpose:   Represents a matched item. Contains the file the item was found, the
			  line it was found on, and the line itself.
 */

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