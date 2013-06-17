/*
 * File Name: MatchCollector.h
 * Date:      June 14, 2013
 * Author:    Milan Sobat
 * Student #: 0469245
 * Course:    INFO-5056
 * Purpose:   Represents a container for matches found. Contains a vector of 
			  shared_ptr<Match> for proper dynamic memory allocation.
 */
#ifndef __MATCHCOLLECTOR_H__
#define __MATCHCOLLECTOR_H__

#include <memory>
#include <vector>
#include "Match.h"

typedef std::shared_ptr<Match> match_ptr;

class MatchCollector : public Match {
private:
	std::vector<match_ptr> _vecMatches;

public:
	MatchCollector() : Match() {}
	virtual ~MatchCollector() {}

	void addMatch( Match* match ) {
		match_ptr ptr( match );
		_vecMatches.push_back( ptr );
	}

	void removeAllMatches() {
		_vecMatches.clear();
	}

	vector<match_ptr> getVector() { return _vecMatches; }
};

#endif