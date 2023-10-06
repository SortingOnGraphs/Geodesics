/*
 * Geodesics is a C++ implementation for enumerating shortest paths between two vertices 
 * in the Cayley graph Cay(G,S) of a finite group G with its generating set S.
 *
 * Copyright (C) 2023 Dohan Kim
 * Author : Dohan Kim
 *
 * File Name : Geodesics.h
 * Description : This file is the header file of Geodesics.cpp.
 *
 */


#ifndef _Geodesics_H
#define _Geodesics_H

#define ERROR( str ) fprintf( stderr, "%s line %d: %s\n", __FILE__, __LINE__, str ); exit(0);
#define BALANCE_PARAMETER 128

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

typedef struct
{
	string left_side_relation;
	string right_side_relation;
} Relation;

typedef struct
{
	char generator;
	string inverse;
} Inverse;

typedef struct
{
	char *input_file;
	string identity;
	string source_node;
	string target_node;
	string input_word;
	int num_of_generators;
	int num_of_units;
	int num_of_relations;
	int num_k;
	vector<char> generator_list;
	vector<Inverse> inverse_list;
	vector<Relation> relation_list;
} GeodesicsIO;

typedef struct
{
    string left_side_rule;
    string right_side_rule;
	bool active;
} RewriteRule;

class Geodesics 
{
  private:
	GeodesicsIO *geodesicsIO;	
	int num_of_generators;
	int num_of_relations;
	vector<char> generator_list;
	vector<Inverse> inverse_list;
  public:
	int num_k;
	string identity;
	string source_node;
	string target_node;
	string input_word;
	vector <RewriteRule> rewrite_rules;
	vector<Relation> relation_list;
	vector <RewriteRule> stack;
	string inverse_node( const string & str );
	Geodesics( GeodesicsIO *geodesicsIO );
	~Geodesics(){};
	string rewrite_from_left( string U );
	string convert_int_to_string( int int_val );
	int shortlex_compare( const string & first_str, const string & second_str );
	int find_generator_index( const char & ch );
	int balance( const string & str1, const string & str2, string & balanced_str1, string & balanced_str2 );
	bool find_suffix( const string & str, const string & suffix );
	int print_rewrite_rules();
};

extern GeodesicsIO geodesicsIO;
extern int geodesics_print( GeodesicsIO *geodesicsIO );
extern int geodesics_initialize( GeodesicsIO *geodesicsIO );

#endif /* _Geodesics_H */
