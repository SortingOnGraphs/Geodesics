/*
 * Geodesics is a C++ implementation for enumerating shortest paths between two vertices 
 * in the Cayley graph Cay(G,S) of a finite group G with its generating set S.
 *
 * Copyright (C) 2021 Dohan Kim
 * Author : Dohan Kim
 *
 * File Name : Geodesics.cpp
 * Description : This file contains the rewriting related functions. 
 * In particular, this file contains the "Rewrite From Left" module and the "Balance" heuristic. 
 *
 */

#include "Geodesics.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

Geodesics::Geodesics( GeodesicsIO *geodesicsIO )
{
	this->geodesicsIO = geodesicsIO;
	this->identity = geodesicsIO->identity;
	this->num_of_generators = geodesicsIO->num_of_generators;
	this->num_of_relations = geodesicsIO->num_of_relations;
	this->num_k = geodesicsIO->num_k;
	this->input_word = geodesicsIO->input_word;
	this->generator_list = geodesicsIO->generator_list;
	this->relation_list = geodesicsIO->relation_list;
	this->inverse_list = geodesicsIO->inverse_list;

#ifdef CAYLEY_GRAPH 
	this->source_node = geodesicsIO->source_node;
	this->target_node = geodesicsIO->target_node;
	this->input_word = inverse_node( this->source_node ) + this->target_node;
#endif
}

string Geodesics::inverse_node( const string & str )
{
	string str_return;

	string temp( str.rbegin(), str.rend() );

	for( int i = 0; i < ( int )temp.size(); i++ )
	{
		for( int j = 0; j < ( int )this->inverse_list.size(); j++ )
		{
			if( temp[i] == this->inverse_list[j].generator )	
			{
				str_return += this->inverse_list[j].inverse;
			}
		}
	}

	return str_return;
}

bool Geodesics::find_suffix( const string & str, const string & suffix )
{
	if ( suffix.size() > str.size() )
	{
		return false;
	}

	return equal( suffix.rbegin(), suffix.rend(), str.rbegin() );
}

string Geodesics::convert_int_to_string( int int_val )
{
	ostringstream string_val;
	string_val<<int_val;
	return string_val.str();
}

string Geodesics::rewrite_from_left( string U )
{
	string V, W = U;

	while( !W.empty() )
	{
		char x = W[0];
		W = W.substr( 1, W.length() - 1 );
		V += x;

		for( int i = 0; i < ( int ) this->rewrite_rules.size(); i++ )
		{
			string lsr = this->rewrite_rules[i].left_side_rule;
			string rsr = this->rewrite_rules[i].right_side_rule;
			bool active = this->rewrite_rules[i].active;

			if( active )
			{
				if( this->find_suffix( V, lsr ) )
				{
				#ifdef _DEBUG
					cout<<"BEFORE lsr: "<<lsr<<endl;
					cout<<"BEFORE W: "<<W<<endl;
					cout<<"BEFORE V: "<<V<<endl;
				#endif

					string R = V.substr( 0, V.length() - lsr.size() );
					W = rsr + W;
					V = R;
				#ifdef _DEBUG
					cout<<"lsr: "<<lsr<<endl;
					cout<<"R: "<<R<<endl;
					cout<<"W: "<<W<<endl;
					cout<<"V: "<<V<<endl;
				#endif
				} 
			}
		} 
	}	

	return V;
}

int Geodesics::shortlex_compare( const string & first_str, const string & second_str )
{
	if( first_str.length() > second_str.length() )
	{
		return 1;
	}
	else if( first_str.length() < second_str.length() )
	{
		return -1;
	}
	else /* first_str.length() = second_str.length() */
	{
		string first_val, second_val;

		for( int i = 0; i < ( int ) first_str.size(); i++ )
		{
			for( int j = 0; j < ( int ) this->num_of_generators; j++ )
			{
				if( first_str[i] == ( char ) this->generator_list[j] )
				{
					first_val += this->convert_int_to_string( j + 1 );
				}
				if( second_str[i] == ( char ) this->generator_list[j] )
				{
					second_val += this->convert_int_to_string( j + 1 ); 
				}
			}
		}

		if( atoi( first_val.c_str() ) > atoi( second_val.c_str() ) )
		{
			return 1;
		}
		else if( atoi( first_val.c_str() ) == atoi( second_val.c_str() ) )
		{
			return 0;
		}
		else
		{
			return -1;
		}

	#ifdef _DEBUG
		cout<<"First:"<<first_val<<endl;
		cout<<"Second:"<<second_val<<endl;
	#endif
	}
}

int Geodesics::find_generator_index( const char & ch )
{
	for( int i = 0; i < ( int )this->inverse_list.size(); i++ )
	{
		if( ch == this->inverse_list[i].generator )
		{
			return i;
		}
	}
	
	return -1;
}

int Geodesics::balance( const string & str1, const string & str2, string & balanced_str1, string & balanced_str2 )
{
	balanced_str1 = str1;
	balanced_str2 = str2;

	while( true ) /* The following loop is controlled by 'Continue' and 'break' statements */
	{
		char last1 = *balanced_str1.rbegin();
		char last2 = *balanced_str2.rbegin();
		
		string L = balanced_str1.substr( 0, balanced_str1.size() - 1 );

		int index = find_generator_index( last1 ); 

		if( index != -1 )
		{
			string last1_inverse = this->inverse_list[index].inverse;

			if( last1 == last2 )
			{
				string M = balanced_str2.substr( 0, balanced_str2.size() - 1 );
				balanced_str1 = L;
				balanced_str2 = M;
				continue;
			}
			else if( balanced_str1.size() + last1_inverse.size() < BALANCE_PARAMETER ) 
			{
				string P = rewrite_from_left( balanced_str2 + last1_inverse ); 
				
				if( shortlex_compare( L, P ) == 1 ) /* L > P */
				{
					balanced_str1 = L;
					balanced_str2 = P;
					continue;
				}
				else if( shortlex_compare( L, P ) == -1 ) /* L < P */
				{
					RewriteRule rule;
					rule.left_side_rule = P;
					rule.right_side_rule = L;
					this->stack.push_back( rule );
				}
			}
			else
			{
				ERROR( "FAILURE OF THE BALANCE HEURISTIC" );
			}
		}

		char first1 = *balanced_str1.begin();
		char first2 = *balanced_str2.begin();

		string S = balanced_str1.substr( 1, balanced_str1.size() - 1 );

		int index2 = find_generator_index( first1 ); 

		if( index2 != -1 )
		{
			string first1_inverse = this->inverse_list[index2].inverse;

			if( first1 == first2 )
			{
				string T = balanced_str2.substr( 1, balanced_str2.size() - 1 );
				balanced_str1 = S;
				balanced_str2 = T;
				continue;
			}
			else if( balanced_str1.size() + first1_inverse.size() < BALANCE_PARAMETER ) 
			{
				string Q = rewrite_from_left( first1_inverse + balanced_str2 ); 

				if( shortlex_compare( S, Q ) == 1 ) /* S > Q */
				{
					balanced_str1 = S;
					balanced_str2 = Q;
					continue;
				}
				else if( shortlex_compare( S, Q ) == -1 ) /* S < Q */
				{
					RewriteRule rule;
					rule.left_side_rule = Q;
					rule.right_side_rule = S;
					this->stack.push_back( rule );
				}
			}
		}

		break;
	}

	return 1;
}

int Geodesics::print_rewrite_rules()
{
	int j = 0;

	cout<<"The final reduced, complete rewriting system for G with respect to R: "<<endl;

	for( int i = 0; i < ( int ) this->rewrite_rules.size(); i++ )
	{
		string lsr = this->rewrite_rules[i].left_side_rule;
		string rsr = this->rewrite_rules[i].right_side_rule;
		bool active = this->rewrite_rules[i].active;

		if( active )
		{
			if( !rsr.empty() )
			{
				cout<<j+1<<" : "<<lsr<<"-->"<<rsr<<endl;

			}
        	else
			{
				cout<<j+1<<" : "<<lsr<<"-->"<<this->identity<<endl;
			}

			j++;
		}
	}

	return 1;
}
