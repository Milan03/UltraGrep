/*
 * File Name: UltraGrep_Main.cpp
 * Date:      June 14, 2013
 * Author:    Milan Sobat
 * Student #: 0469245
 * Course:    INFO-5056
 * Purpose:   
 */

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <regex>
#include "Match.h"
#include "MatchCollector.h"

using namespace std;
using namespace std::tr2::sys;

MatchCollector matches = MatchCollector();

void print_output() {
	vector<match_ptr> vecMatches = matches.getVector();
	int matchOccurance = 1;
	
	wcout << vecMatches.size() << " matches found: " << endl;
	for ( size_t i = 0; i < vecMatches.size(); ++i ) {		
		wcout << L"File: " << vecMatches[i]->getFile() << endl;
		if ( i > 0 && vecMatches[i-1]->getLineNumber() == vecMatches[i]->getLineNumber() ) {
			++matchOccurance;
			wcout << L"[" << vecMatches[i]->getLineNumber() << L":" << matchOccurance << L"]: " << vecMatches[i]->getLine() << endl;
		} else {
			matchOccurance = 1;
			wcout << L"[" << vecMatches[i]->getLineNumber() << L"]: " << vecMatches[i]->getLine() << endl;
		}
		wcout << endl;
	}
	int br;
}

void scan_current_directory( wpath const& f,  wregex const& reg, unsigned i = 0 ) {
	wstring indent( i, L'\t' );
	//wcout << indent << "Folder = " << system_complete( f ) << endl;

	for ( wdirectory_iterator d(f), e; d != e; ++d ) {
		//wcout << indent << d->path() << (is_directory( d->status() ) ? L" [dir]" : L"") << " ext=" << d->path().extension() << endl;
		wstring fileExt = d->path().extension();
		if ( !is_directory( d->status() ) && fileExt == L".txt" ) {	
			wstring dPath =  f / d->path();
			wifstream fileStream( dPath.c_str(), ios::in );
			if ( !fileStream.is_open() ) {
				wcerr << L"Unable to open file: " << d->path() << endl;
				continue;
			}
			
			
			int lineCount = 0;
			while ( !fileStream.eof() ) {
				int matchOccurance = 0;
				++lineCount;
				vector<wstring> tokens;
				wstring line;
				getline( fileStream, line );
				wistringstream iss( line );
				copy( istream_iterator<wstring, wchar_t, char_traits<wchar_t>>(iss),
					istream_iterator<wstring, wchar_t, char_traits<wchar_t>>(),
					back_inserter< vector<wstring> >(tokens));

				wsmatch m;
				
				for ( const wstring& wstr : tokens ) {
					if ( regex_search( wstr, m, reg ) ) {
						Match* newMatch = new Match( dPath );
						newMatch->setLineNumber( lineCount );
						newMatch->setLine( line );
						matches.addMatch( newMatch );
					}
				}
			}
			fileStream.close();
		}
		if ( is_directory( d->status() ) )
			scan_current_directory( f / d->path(), reg, i+1 );
	}
}

int main ( int argc, char **argv ) {
	if ( argc < 3 ) {
		wcerr << L"Error: Invalid command line." << endl;
		return EXIT_FAILURE;
	}

	if ( argc == 3 ) {
		wstring wstrPath, regExp;
		wpath path;
		wcin >> wstrPath >> regExp;

		if ( wstrPath == L"." )
			path = L"..";

		wregex reg( regExp );

		if ( !exists( path ) ) {
			wcerr << L"Can't find: " << path << endl;
			return EXIT_FAILURE;
		}

		wcout << path << (is_directory( path ) ? L" [dir]" : L"")<< endl;

		scan_current_directory( path, reg );

		print_output();
	}

	return 0;
}