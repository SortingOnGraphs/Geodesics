/*
 * Geodesics is a C++ implementation for enumerating shortest paths between two vertices 
 * in the Cayley graph Cay(G,S) of a finite group G with its generating set S.
 *
 * Copyright (C) 2023 Dohan Kim
 * Author : Dohan Kim
 *
 * File Name : Thue.cpp
 * Description : This file contains the "Thue Resolution" module. 
 * It also contains the shortest path enumeration module using "Geodesics.cpp" and "KnuthBendix.cpp".
 *
 */

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "Thue.h" 

using namespace std;

Thue::Thue( Geodesics *geodesics )
{
	this->geodesics = geodesics;
	this->end_of_search = false;
}

int Thue::Thue_resolution()
{
	for( int i = 0; i < ( int ) this->geodesics->rewrite_rules.size(); i++ )
	{
		string lsr = this->geodesics->rewrite_rules[i].left_side_rule;
		string rsr = this->geodesics->rewrite_rules[i].right_side_rule;
		bool active = this->geodesics->rewrite_rules[i].active;

		if( active )
		{
			if( lsr.length() > rsr.length() )
			{
				LengthDecreasingRule ldr;
				ldr.left_side_rule = lsr;
				ldr.right_side_rule = rsr;
				this->length_decreasing_rules.push_back( ldr );
			}
			else /* lsr.length() = rsr.length() */
			{
				LengthPreservingRule lpr;
				lpr.left_side_rule = lsr;
				lpr.right_side_rule = rsr;
				this->length_preserving_rules.push_back( lpr );

				LengthPreservingRule lpr_symmetry;
				lpr_symmetry.left_side_rule = rsr;
				lpr_symmetry.right_side_rule = lsr;
				this->length_preserving_rules.push_back( lpr_symmetry );
			}
		}
	}

	return 1;
}

string Thue::shortlex_normal_form( string & str )
{
	string shortlex_normal = this->geodesics->rewrite_from_left( str );

	return shortlex_normal;
}

size_t Thue::replace_reduced_word( string & str1, string & word_found, size_t str1_index, string & str2_left, string & str2_right )
{
	string str = str1;
	
	size_t pos = str.find( str2_left, str1_index );

	if( pos != string::npos )
	{
		word_found = str.replace( pos, str2_left.length(), str2_right );
	}
	else
	{
		word_found = string();
	}
	
	return pos;
}

vector <ReducedWord> Thue::return_words_found( string & cur_level_word )
{
	vector <ReducedWord> vec_words;

	for( int i = 0; i < ( int ) this->length_preserving_rules.size(); i++ )
	{
		string lsr = this->length_preserving_rules[i].left_side_rule;
		string rsr = this->length_preserving_rules[i].right_side_rule;
		string word_found;
		size_t pos = replace_reduced_word( cur_level_word, word_found, 0, lsr, rsr );

		while( pos != string::npos )
		{
			bool visited = false;

			for( int j = 0; j < ( int ) this->reduced_words.size(); j++ )
			{
				if( word_found == this->reduced_words[j].current_reduced_word )
				{
					visited = true;
				}
			}

			if( visited == false )
			{
				ReducedWord rword;
				rword.current_reduced_word = word_found;
				rword.parent_reduced_word = cur_level_word;
				rword.applied_rule = this->length_preserving_rules[i];
				this->reduced_words.push_back( rword );
				vec_words.push_back( rword );

				if( ( int )this->reduced_words.size() == this->geodesics->num_k )
				{
					this->end_of_search = true;
					return vec_words;
				}

			}

			pos = replace_reduced_word( cur_level_word, word_found, pos + 1, lsr, rsr );
		}
	}

	return vec_words;
}

int Thue::enumerate_reduced_words( string & shortlex_normal_word )
{
	int level = 0;

	vector <ReducedWord> current_level_reduced_words;
	vector <ReducedWord> next_level_reduced_words;

	ReducedWord rword;
	rword.parent_reduced_word = string();
	rword.current_reduced_word = shortlex_normal_word; 
	this->reduced_words.push_back( rword );
	current_level_reduced_words.push_back( rword );

	cout<<endl<<"Level: "<<level<<endl;
	level++;
		
	cout<<"The reduced Word in this level: "<<current_level_reduced_words[0].current_reduced_word<<"."<<endl;

	if( this->geodesics->num_k == 1 )
	{
		print_reduced_words();
		exit( 1 );
	}

	while( !current_level_reduced_words.empty() )
	{
		ReducedWord cur_level_word = current_level_reduced_words.back();
		current_level_reduced_words.pop_back();

		vector <ReducedWord> words_found = return_words_found( cur_level_word.current_reduced_word );

		for( int i = 0; i < ( int ) words_found.size(); i++ )
		{
			next_level_reduced_words.push_back( words_found[i] );
		}

		if( current_level_reduced_words.empty() || this->end_of_search )
		{
			if( !next_level_reduced_words.empty() )
			{
				cout<<endl<<"Level: "<<level<<endl;
			}

			level++;

			current_level_reduced_words = next_level_reduced_words;
			next_level_reduced_words.clear();
		
			for( int i = 0; i < ( int ) current_level_reduced_words.size(); i++ )
			{
				string lsr = current_level_reduced_words[i].applied_rule.left_side_rule;
				string rsr = current_level_reduced_words[i].applied_rule.right_side_rule;

				cout<<"The reduced word #"<<i + 1<<" in this level: "<<current_level_reduced_words[i].current_reduced_word;
				cout<<" ( Parent node: "<<current_level_reduced_words[i].parent_reduced_word<<" , ";
				cout<<"Applied rule: "<<lsr<<"-->"<<rsr<<" )."<<endl;

			}

			if( this->end_of_search )
			{
				print_reduced_words();
				return 1;
			}

		}
	}

	print_reduced_words();

	return 1;
}

int Thue::print_reduced_words()
{

#ifdef CAYLEY_GRAPH
	cout<<endl<<"The number of shortest path(s) found from vertex "<<this->geodesics->source_node<<" to vertex ";
	cout<<this->geodesics->target_node<<" is "<<this->reduced_words.size()<<"."<<endl<<endl;

	for( int i = 0; i < ( int ) this->reduced_words.size(); i++ )
	{
		if( i == 0 )
		{
			cout<<i+1<<": "<<"The shortest path #1 (Shortlex Normal Form): "<<this->reduced_words[i].current_reduced_word<<endl;
		}
		else
		{
			cout<<i+1<<": "<<"The shortest path #"<<i + 1<<" : "<<this->reduced_words[i].current_reduced_word<<endl;
		}
	}
#else
	cout<<endl<<"The number of reduced word(s) found for "<<this->geodesics->input_word<<" is "<<this->reduced_words.size()<<"."<<endl<<endl;

	for( int i = 0; i < ( int ) this->reduced_words.size(); i++ )
	{
		if( i == 0 )
		{
			cout<<i+1<<": "<<"The reduced word #1 (Shortlex Normal Form): "<<this->reduced_words[i].current_reduced_word<<endl;
		}
		else
		{
			cout<<i+1<<": "<<"The reduced word #"<<i + 1<<" : "<<this->reduced_words[i].current_reduced_word<<endl;
		}
	}

#endif

	return 1;
}

int Thue::print_Thue_resolution()
{
	int j = 0;

	cout<<endl<<this->length_decreasing_rules.size()<<" Length Decreasing Rules : "<<endl; 

	for( int i = 0; i < ( int ) this->length_decreasing_rules.size(); i++ )
	{
		string lsr = this->length_decreasing_rules[i].left_side_rule;
		string rsr = this->length_decreasing_rules[i].right_side_rule;

		if( !rsr.empty() )
		{
			cout<<j+1<<" : "<<lsr<<"-->"<<rsr<<endl;
		}
		else
		{
			cout<<j+1<<" : "<<lsr<<"-->"<<this->geodesics->identity<<endl;
		}	
		
		j++;
	}

	cout<<endl<<this->length_preserving_rules.size()<<" Length Preserving Rules ( After Thue Resolution ) : "<<endl;

	for( int i = 0; i < ( int ) this->length_preserving_rules.size(); i++ )
	{
		string lsr = this->length_preserving_rules[i].left_side_rule;
		string rsr = this->length_preserving_rules[i].right_side_rule;

		cout<<j+1<<" : "<<lsr<<"-->"<<rsr<<endl;
		
		j++;
	}

	return 1;
}
