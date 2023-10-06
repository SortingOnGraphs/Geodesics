/*
 * Geodesics is a C++ implementation for enumerating shortest paths between two vertices 
 * in the Cayley graph Cay(G,S) of a finite group G with its generating set S.
 *
 * Copyright (C) 2023 Dohan Kim
 * Author : Dohan Kim
 *
 * File Name : Thue.h
 * Description : This file is the header file of Thue.cpp.
 *
 */


#ifndef _THUE_H
#define _THUE_H

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <iostream>

#include "Geodesics.h"

using namespace std;

typedef struct
{
	string left_side_rule;
	string right_side_rule;
} LengthDecreasingRule;


typedef struct
{
	string left_side_rule;
	string right_side_rule;
} LengthPreservingRule;

typedef struct
{
	string current_reduced_word;
	string parent_reduced_word;
	LengthPreservingRule applied_rule;
} ReducedWord;

class Thue 
{
  private:
	Geodesics *geodesics;	
	bool end_of_search;
  public:
	Thue( Geodesics *geodesics );
	~Thue(){};
	vector <LengthDecreasingRule> length_decreasing_rules;
	vector <LengthPreservingRule> length_preserving_rules;
	vector <ReducedWord> reduced_words;
	int Thue_resolution();
	string shortlex_normal_form( string & str );
	size_t replace_reduced_word( string & str1, string & word_found, size_t str1_index, string & str2_left, string & str2_right );
	vector <ReducedWord> return_words_found( string & cur_level_word );
	int enumerate_reduced_words( string & shortlex_normal_word );
	int print_reduced_words();
	int print_Thue_resolution();
};

#endif /* _THUE_H */
