/*
	OrderChecker.h
	Very simple class designed to test whether or not a root file is time ordered.
	Meant to be used with newly converted from binary data. Only for development.

	Written by G.W. McCann Oct. 2020
*/
#ifndef ORDERCHECKER_H
#define ORDERCHECKER_H

class OrderChecker {
public:
	OrderChecker();
	~OrderChecker();
	bool IsOrdered(const std::string& filename);
};

#endif
