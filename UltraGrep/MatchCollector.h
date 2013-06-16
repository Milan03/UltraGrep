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

	vector<match_ptr> getVector() { return _vecMatches; }
};

#endif