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
#include <filesystem>
#include <vector>
#include <regex>

using namespace std;
using namespace std::tr2::sys;

void scan_filesystem( wpath const& f,  wregex const& reg, unsigned i = 0 ) {
	wstring indent( i, L'\t' );
	wcout << indent << "Folder = " << system_complete( f ) << endl;

	for ( wdirectory_iterator d(f), e; d != e; ++d ) {
		//wcout << indent << d->path() << (is_directory( d->status() ) ? L" [dir]" : L"") << " ext=" << d->path().extension() << endl;

		if ( !is_directory( d->status() ) ) {	
			wstring foldername = f.filename();
			wstring dPath =  d->path();
			wstring completePath = system_complete( f );
			completePath.erase( completePath.length() - 18, 18 );
			wstring finalStr = completePath + foldername + L"\\" + dPath;
			wifstream fileStream( finalStr.c_str(), ios::in );
			if ( !fileStream.is_open() ) {
				wcerr << L"Unable to open file: " << d->path() << endl;
				continue;
			}

			fileStream.close();
		}

		if ( is_directory( d->status() ) )
			scan_filesystem( f / d->path(), reg, i+1 );
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

		scan_filesystem( path, reg );
	}

	return 0;
}