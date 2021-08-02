/*
 * Geodesics is a C++ implementation for enumerating shortest paths between two vertices 
 * in the Cayley graph Cay(G,S) of a finite group G with its generating set S.
 *
 * Copyright (C) 2021 Dohan Kim
 * Author : Dohan Kim
 *
 * File Name : KnuthBendix.h
 * Description : This file is the header file of KnuthBendix.cpp
 *
 */


#ifndef _KNUTHBENDIX_H
#define _KNUTHBENDIX_H

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <iostream>

#include "Geodesics.h"

using namespace std;

class KnuthBendix 
{
  private:
	Geodesics *geodesics;	
  public:
	KnuthBendix( Geodesics *geodesics );
	~KnuthBendix(){};
	int add_rewrite_rule();
	int overlap( int i, int j );
	int KnuthBendix_procedure();
};

#endif /* _KNUTHBENDIX_H */
