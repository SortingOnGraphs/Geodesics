/*
 * Geodesics is a C++ implementation for enumerating shortest paths between two vertices 
 * in the Cayley graph Cay(G,S) of a finite group G with its generating set S.
 *
 * Copyright (C) 2023 Dohan Kim
 * Author : Dohan Kim
 *
 * File Name : main.cpp
 * Description : This is the main file of the Geodesics program. 
 * It takes a file name as an input parameter and initializes the runtime environment for Geodesics.
 *
 */

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "Geodesics.h"
#include "KnuthBendix.h"
#include "Thue.h"

using namespace std;

GeodesicsIO geodesicsIO;

int geodesicsIO_initialize( GeodesicsIO *geodesicsIO )
{
	char line[1024], buf[1024], buf2[1024];
	FILE *fptr = NULL;

	if( ( fptr = fopen( geodesicsIO->input_file, "r" ) ) == NULL )
	{
		ERROR( "FILE OPEN ERROR." );
	}

	while ( fgets( line, sizeof( line ), fptr ) != NULL )
	{
		size_t i = strspn( line, " \t\n\v" );

		if ( line[i] == '#' )
			continue;
		else if( !strncmp( line, "NumberOfGenerators", 18 ) )
		{
			memset( buf, '\0' , sizeof( buf) );
			sscanf( line,"%*s %s", buf );
			geodesicsIO->num_of_generators = atoi( buf );
		}
		else if( !strncmp( line, "NumberOfUnits", 13 ) )
		{
			memset( buf, '\0' , sizeof( buf) );
			sscanf( line,"%*s %s", buf );
			geodesicsIO->num_of_units = atoi( buf );
		}
		else if( !strncmp( line, "NumberOfRelations", 17 ) )
		{
			memset( buf, '\0' , sizeof( buf) );
			sscanf( line,"%*s %s", buf );
			geodesicsIO->num_of_relations = atoi( buf );
		}
		else if( !strncmp( line, "NumberK", 7 ) )
		{
			memset( buf, '\0' , sizeof( buf) );
			sscanf( line,"%*s %s", buf );
			geodesicsIO->num_k = atoi( buf );
		}
		else if( !strncmp( line, "SourceNode", 10 ) )
		{
			memset( buf, '\0' , sizeof( buf) );
			sscanf( line,"%*s %s", buf );
			geodesicsIO->source_node = ( string ) buf;
		}
		else if( !strncmp( line, "TargetNode", 10 ) )
		{
			memset( buf, '\0' , sizeof( buf) );
			sscanf( line,"%*s %s", buf );
			geodesicsIO->target_node = ( string ) buf;
		}
		else if( !strncmp( line, "InputWord", 9 ) )
		{
			memset( buf, '\0' , sizeof( buf) );
			sscanf( line,"%*s %s", buf );
			geodesicsIO->input_word = ( string ) buf;
		}
		else if( !strncmp( line, "GeneratorList", 13 ) ) 
		{
			char *temp_ptr = NULL;
			int index = 0;
			memset( buf, '\0' , sizeof( buf ) );
			strcpy( buf, line );
			temp_ptr = strtok( buf, "\t ");

			while( temp_ptr != NULL && index < geodesicsIO->num_of_generators )
			{
				temp_ptr = strtok( NULL, "\t\n " );

				if( temp_ptr[1] == '\0' || temp_ptr[1] == '\n' || temp_ptr[1] == '\r' )
				{
					geodesicsIO->generator_list.push_back( temp_ptr[0] );
				}
				else
				{
					ERROR( "A generator should be a single character." );
				}

				index++; 
			}
		}
		else if( !strncmp( line, "InversesForGenerators", 21 ) ) 
		{
			char generator, inverse[64], *temp_ptr = NULL;
			int index = 0;
			memset( buf, '\0' , sizeof( buf ) );
			strcpy( buf, line );
			temp_ptr = strtok( buf, "\t ");

			while( temp_ptr != NULL && index < geodesicsIO->num_of_units )
			{
				temp_ptr = strtok( NULL, "\t\n " );

				sscanf( temp_ptr,"%[^:]:%s", &generator, inverse );

				Inverse inv;
				inv.generator = generator;
				inv.inverse = inverse;
			
				geodesicsIO->inverse_list.push_back( inv );

				index++; 
			}
		}
		else if( !strncmp( line, "Identity", 8 ) )
		{
			memset( buf, '\0' , sizeof( buf) );
			sscanf( line,"%*s %s", buf );
			geodesicsIO->identity = string( buf );
		}
		else 
		{
			if( (int)line[i] < 48 || (int)line[i] > 57 ) // Accept only numbers
				continue;

			memset( buf, '\0', sizeof( buf) );
			memset( buf2, '\0', sizeof( buf2) );
			sscanf( line,"%*s %[^','], %s", buf, buf2 );

			Relation relation;
			relation.left_side_relation = string( buf );
			relation.right_side_relation = string( buf2 );


			if( relation.right_side_relation.compare( geodesicsIO->identity ) == 0 )
			{
				relation.right_side_relation = "";	
			}

			geodesicsIO->relation_list.push_back( relation );
		}
	}

	return 1;
}

int geodesicsIO_print( GeodesicsIO *geodesicsIO )
{
	cout<<endl<<"Number of Generators in G: "<<geodesicsIO->num_of_generators<<endl;
	cout<<"Number of Relations for G: " <<geodesicsIO->num_of_relations<<endl;
	cout<<"Number K: " <<geodesicsIO->num_k<<endl;
	cout<<"Generator List: ";

	for( int i = 0 ; i < geodesicsIO->num_of_generators ; i++ )
	{
		if( i != geodesicsIO->num_of_generators - 1 )
		{
			cout<<geodesicsIO->generator_list[i]<<", ";
		}
		else
		{
			cout<<geodesicsIO->generator_list[i];
		}
	}

	cout<<" (";

	for( int j = 0 ; j < geodesicsIO->num_of_generators ; j++ )
	{
		if( j != geodesicsIO->num_of_generators - 1 )
		{
			cout<<geodesicsIO->generator_list[j]<<" < ";
		}
		else
		{
			cout<<geodesicsIO->generator_list[j];
		}
	}

	cout<<")";

	cout<<endl;

	cout<<"Identity Element in G: "<<geodesicsIO->identity<<endl<<endl;

#ifndef CAYLEY_GRAPH
	cout<<"Input Word: "<<geodesicsIO->input_word<<endl<<endl;
#endif

	cout<<"Relation List R: "<<endl;

	for( int i = 0 ; i < geodesicsIO->num_of_relations ; i++ )
	{
		cout<<geodesicsIO->relation_list[i].left_side_relation<<" , ";

		if( geodesicsIO->relation_list[i].right_side_relation.empty() )
		{
			cout<<geodesicsIO->identity<<endl;
		}
		else
		{
			cout<<geodesicsIO->relation_list[i].right_side_relation<<endl;
		}
	}

	cout<<endl<<"Inverse List For Generators: "<<endl;


	for( int k = 0 ; k < ( int )geodesicsIO->inverse_list.size() ; k++ )
	{
		cout<<geodesicsIO->inverse_list[k].generator<<" : "<<geodesicsIO->inverse_list[k].inverse<<endl;
	}

#ifdef CAYLEY_GRAPH
	cout<<endl<<"Source Vertex in Cay(G, S): "<<geodesicsIO->source_node<<endl; 
	cout<<"Target Vertex in Cay(G, S): "<<geodesicsIO->target_node<<endl; 
#endif

	cout<<endl;

	fflush( stdout );

	return 1;
}

int main(int argc, char **argv)
{
	char ptr[256];

	if( argc != 2 )
	{
		ERROR( "Usage: ./geodesics parameter_input_file." );
	}

	geodesicsIO.input_file = argv[1];
	strncpy( ptr, argv[1], 256 );
	geodesicsIO_initialize( &geodesicsIO );
	geodesicsIO_print( &geodesicsIO );

	Geodesics geodesics( &geodesicsIO );

	KnuthBendix knuthbendix( &geodesics );
	knuthbendix.KnuthBendix_procedure();

	geodesics.print_rewrite_rules();

	Thue thue( &geodesics );
	thue.Thue_resolution();
	thue.print_Thue_resolution();

	string shortlex_normal_word = thue.shortlex_normal_form( geodesics.input_word );

#ifdef CAYLEY_GRAPH
	cout<<endl<<"The lexicographically minimal shortest path from vertex "<<geodesics.source_node;
	cout<<" to vertex "<<geodesics.target_node<<" in Cay(G, S) is "<<shortlex_normal_word<<"."<<endl;
#else
	cout<<endl<<"The shortlex normal form of the word "<<geodesics.input_word<<" is "<<shortlex_normal_word<<"."<<endl;
#endif

	thue.enumerate_reduced_words( shortlex_normal_word );

	return 1;
}
