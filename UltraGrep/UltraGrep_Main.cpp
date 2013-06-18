/*
 * File Name: UltraGrep_Main.cpp
 * Date:      June 14, 2013
 * Author:    Milan Sobat
 * Student #: 0469245
 * Course:    INFO-5056
 * Purpose:   Recursively searches a filesystem for text/code files and lists all
			  the lines that contain a given regular expression.
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
vector<wstring> extensions;

// Code from: http://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t
// Post #2
// All credit goes to author
// Takes in a string and converts to wstring.
inline void string_to_wstring(wstring &ws, const string &s)
{
    wstring wsTmp(s.begin(), s.end());

    ws = wsTmp;
}

// Code from: http://www.cplusplus.com/forum/general/47680/
// Post #4
// All credit goes to author
// Splits string into vector of substrings.
inline void split_string(const wstring& str, const wstring& delim, vector<wstring>& output)
{
    size_t start = 0, found = str.find_first_of(delim);

    while (found != string::npos)
    {
        if (found > start)
            output.push_back( str.substr(start, found - start) );

        start = ++found;
        found = str.find_first_of(delim, found);
    }
    if (start < str.size())
        output.push_back( str.substr(start) );
} 

/*
 * Function Name: print_output()
 * Purpose:       Prints matches found.
 * Accepts:       Nothing.
 * Returns:       Nothing.
 */
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
}

/*
 * Function Name: scan_verbose( wpath const& f,  wregex const& reg )
 * Purpose:       Recursively scans the filesystem and prints out the files it scans.
 *				  If a match is found it is output.
 * Accepts:       'f' representing the path to search, 'reg' reprsenting the expression 
 *				  to search for.
 * Returns:       Nothing.
 */
void scan_verbose( wpath const& f,  wregex const& reg ) {
	for ( wdirectory_iterator d(f), e; d != e; ++d ) {
		wstring fileExt = d->path().extension();
		if ( extensions.empty() ) 
			extensions.push_back( L"txt" );

		for ( wstring i : extensions ) {
			wstring extElement = L"." + i;
			if ( !is_directory( d->status() ) && fileExt == extElement ) {	
				bool nothingFound = true;
				wstring dPath =  f / d->path();
				wcout << endl;
				wcout << "Scanning: " << dPath << endl;
				for ( int i = 0; i < (dPath.size() + 10); ++i )
					wcout << "-";
				wcout << endl;
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
							print_output();
							matches.removeAllMatches();
							nothingFound = false;
						}
					}
					
					
				}// end while ( !fileStream.eof() )
				if ( nothingFound ) 
						wcout << L" -- Nothing found -- " << endl;
				wcout << endl;
				fileStream.close();
			}// end if ( !is_directory( d->status() ) && fileExt == L".txt" ) 
		}// end for 
		if ( is_directory( d->status() ) )
			scan_verbose( f / d->path(), reg );
	}
}

/*
 * Function Name: scan( wpath const& f,  wregex const& reg )
 * Purpose:       Recursively scans the filesystem with passed arguements.
 * Accepts:       'f' representing the path to search, 'reg' reprsenting the expression 
 *				  to search for.
 * Returns:       Nothing.
 */
void scan( wpath const& f,  wregex const& reg ) {
	//wstring indent( i, L'\t' );
	//wcout << indent << "Folder = " << system_complete( f ) << endl;

	for ( wdirectory_iterator d(f), e; d != e; ++d ) {
		//wcout << indent << d->path() << (is_directory( d->status() ) ? L" [dir]" : L"") << " ext=" << d->path().extension() << endl;
		wstring fileExt = d->path().extension();
		if ( extensions.empty() ) 
			extensions.push_back( L"txt" );

		for ( wstring i : extensions ) {
			wstring extElement = L"." + i;
			if ( !is_directory( d->status() ) && fileExt == extElement ) {	
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
				}// end while ( !fileStream.eof() )
				fileStream.close();
			}// end if ( !is_directory( d->status() ) && fileExt == L".txt" ) 
		}// end for 
		if ( is_directory( d->status() ) )
			scan( f / d->path(), reg );
	}
}

int main ( int argc, char **argv ) {
	// Add args to a vector for arguement checking
	vector<wstring> args;
	for( int i = 0; i < argc; ++i ) {
		string arg = argv[i];
		wstring wideArg;
		string_to_wstring( wideArg, arg );
		args.push_back( wideArg );
	}

	// UltraGrep takes in at least 3 arguements
	if ( argc < 3 ) {
		wcerr << L"Error: Invalid command line." << endl;
		return EXIT_FAILURE;
	}


	wstring const introMsg = L"UltraGrep - Milan Sobat 2013\n";
	wstring wstrPath, regExp;
	wpath path;

	// i.e. -- ultragrep . word --
	if ( argc == 3 ) {
		wcin >> wstrPath >> regExp;

		if ( wstrPath == L"." )
			path = L"..";
		else 
			path = wstrPath;

		wregex reg( regExp );

		if ( !exists( path ) ) {
			wcerr << L"Can't find: " << path << endl;
			return EXIT_FAILURE;
		}

		wcout << path << (is_directory( path ) ? L" [dir]" : L"")<< endl;

		scan( path, reg );

		wcout << introMsg << endl;

		print_output();
	}

	if ( argc == 4 ) {
		// i.e. -- ultragrep . lorem .txt.h.cpp --
		if ( args[1] != L"-v" ) {
			wstring extList;
			wcin >> wstrPath >> regExp >> extList;

			if ( wstrPath == L"." )
				path = L"..";
			else 
				path = wstrPath;

			wregex reg( regExp );

			split_string( extList, L".", extensions );
		
			scan( path, reg );

			wcout << introMsg << endl;

			print_output();
		} 
		// i.e. -- ultragrep -v . Lorem -- 
		if ( args[1] == L"-v" ) {
			//verbose
			wstring fill;
			wcin >> fill >> wstrPath >> regExp;

			if ( wstrPath == L"." )
				path = L"..";
			else 
				path = wstrPath;

			wregex reg( regExp );

			wcout << introMsg << endl;
			
			scan_verbose( path, reg );
		}
	}

	// i.e. -- ultragrep -v . Lorem .txt.cpp -- 
	if ( argc == 5 ) {
		wstring fill, extList;
		wcin >> fill >> wstrPath >> regExp >> extList;

		if ( wstrPath == L"." )
			path = L"..";
		else 
			path = wstrPath;

		wregex reg( regExp );

		split_string( extList, L".", extensions );

		wcout << introMsg << endl;
		
		scan_verbose( path, reg );
	}

	return 0;
}