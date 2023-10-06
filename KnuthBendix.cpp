/*
 * Geodesics is a C++ implementation for enumerating shortest paths between two vertices 
 * in the Cayley graph Cay(G,S) of a finite group G with its generating set S.
 *
 * Copyright (C) 2023 Dohan Kim
 * Author : Dohan Kim
 *
 * File Name : KnuthBendix.cpp
 * Description : This file contains the module for finding the overlaps between the left-hand sides of rewrite rules and 
 * adds the rewrite rules using the "stack" data structure.
 *
 */

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "KnuthBendix.h"

using namespace std;

KnuthBendix::KnuthBendix( Geodesics *geodesics )
{
	this->geodesics = geodesics;
}

int KnuthBendix::add_rewrite_rule()
{
	while( !this->geodesics->stack.empty() )
	{
		RewriteRule rule = this->geodesics->stack.back();
		this->geodesics->stack.pop_back();
		string A = this->geodesics->rewrite_from_left( rule.left_side_rule );
		string B = this->geodesics->rewrite_from_left( rule.right_side_rule );

		if( A.compare( B ) != 0)
		{
			if( this->geodesics->shortlex_compare( A, B ) == -1 )
			{
				A.swap( B );
			}

		#ifdef _DEBUG
			if( B.empty() )
			{
				cout<<"Before Balance A, B: "<<A<<" , "<<"e"<<endl; 
			}
			else
			{
				cout<<"Before Balance A, B: "<<A<<" , "<<B<<endl; 
			}
		#endif
			string C, D;
			this->geodesics->balance( A, B, C, D );

		#ifdef _DEBUG
			if( D.empty() )
			{
				cout<<"After Balance C, D: "<<C<<" , " <<"e"<<endl;
			}
			else
			{
				cout<<"After Balance C, D: "<<C<<" , " <<D<<endl;
			}
		#endif

			RewriteRule rule;
			rule.left_side_rule = C;
			rule.right_side_rule = D;

			rule.active = true;
			this->geodesics->rewrite_rules.push_back( rule );

			for( int i = 0; i < ( int )this->geodesics->rewrite_rules.size() - 1; i++ )
			{
				if( this->geodesics->rewrite_rules[i].active == true )
				{
					if( this->geodesics->rewrite_rules[i].left_side_rule.find( A ) != string::npos )
					{
						this->geodesics->rewrite_rules[i].active = false;
						this->geodesics->stack.push_back( this->geodesics->rewrite_rules[i] );
					}
					else if( this->geodesics->rewrite_rules[i].right_side_rule.find( A ) != string::npos )
					{
						string C = this->geodesics->rewrite_from_left( this->geodesics->rewrite_rules[i].right_side_rule );
						this->geodesics->rewrite_rules[i].right_side_rule = C;
					}
				}
			}
		}
	}

    return 0;
}

int KnuthBendix::overlap( int i, int j )
{
	string Pi = this->geodesics->rewrite_rules[i].left_side_rule;
	string Pj = this->geodesics->rewrite_rules[j].left_side_rule;
	string Qi = this->geodesics->rewrite_rules[i].right_side_rule;
	string Qj = this->geodesics->rewrite_rules[j].right_side_rule;

	int m = min( Pi.length(), Pj.length() ) - 1;

	for( int k = 1; k <= m; k++ )
	{
		/* Let Pi = AB */
		string A = Pi.substr( 0, Pi.length() - k );
		string B = Pi.substr( Pi.length() - k, k );		

		if( !Pj.compare( 0, B.size(), B ) ) /* B is a prefix of Pj */
		{
			/* Let Pj = BC */
			string C = Pj.substr( B.length(), Pj.length() - B.length() );

			RewriteRule rule;
			rule.left_side_rule = A + Qj;
			rule.right_side_rule = Qi + C;

			this->geodesics->stack.push_back( rule );
			add_rewrite_rule();
			
		#ifdef _DEBUG
			cout<<"Pi:"<<Pi<<endl;
			cout<<"Pj:"<<Pj<<endl;
			cout<<"A:"<<A<<endl;
			cout<<"B:"<<B<<endl;
			cout<<"C:"<<C<<endl;

			cout<<"A + Qj: "<< A + Qj <<endl;
			cout<<"Qi + C: "<< Qi + C <<endl<<endl;
		#endif

		}
	}

	return 1;
}

int KnuthBendix::KnuthBendix_procedure()
{
	for( int i = ( int )this->geodesics->relation_list.size() - 1; i >= 0; i-- )
	{
		RewriteRule rule;
		rule.left_side_rule = this->geodesics->relation_list[i].left_side_relation;
		rule.right_side_rule = this->geodesics->relation_list[i].right_side_relation;
		this->geodesics->stack.push_back( rule );
	}

	this->add_rewrite_rule();

	int num_of_rewrite_rules = ( int ) this->geodesics->rewrite_rules.size();
	int i = 0;
 
	while( i < num_of_rewrite_rules )	
	{
		bool active_i = this->geodesics->rewrite_rules[i].active;

		for( int j = 0; j <= i ; j++ )
		{
			bool active_j = this->geodesics->rewrite_rules[j].active;

			if( active_i && active_j )
			{
				this->overlap( i, j );
			}
			
			if( j < i )
			{
				if( active_i && active_j )
				{
					this->overlap( j, i );
				}
			}
		}

		i++;
		num_of_rewrite_rules = this->geodesics->rewrite_rules.size();
	}

	return 1;
}
