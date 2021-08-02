# Geodesics v1.0 – User’s Guide

Geodesics is a C++ implementation for enumerating shortest paths between two vertices 
in the Cayley graph Cay(G,S) of a finite group G with its generating set S.

Copyright (C) 2021 Dohan Kim

Author : Dohan Kim

# Getting started

## Basic commands

To compile: make

To remove the executable file and build files: make clean

There are sample input files in the “data” directory. The user may edit and use an existing input file for the user's own tasks. 

To run without a log file:
./geodesics input_file_name
(E.g.) ./geodesics sample_input.txt

To run with a log file:
./geodesics input_file_name > output_file_name
(E.g.) ./geodesics sample_input.txt > sample_output.txt


## Introduction

Geodesics is a software tool for enumerating k shortest paths (if they exist) between two vertices in the Cayley graph Cay(G, S) of a finite group G with its generating set S.

Language: Standard GNU C++ language
Supporting Environment: Standard GCC/G++ environment (Successfully tested under gcc 4.8.5 running on a Linux platform and tested under a Cygwin environment on a Windows platform )


## How to compile and run Geodesics

Geodesics requires the GNU C++ compiler to compile and run the program.
Type "g++ -v" at a command line prompt and press “Enter”. If your system is already equipped with the GNU C++ compiler, it will show you the version of the GNU C++ compiler. Otherwise, you need to install the GNU C++ compiler. For windows users: You may install the GNU C++ compiler when installing a Cygwin environment on a windows platform. For Linux/Unix users: You may consult the system administrator if you do not have a permission to use the GNU C++ compiler or the GNU C++ compiler is not available for your system.

Once the GNU C++ compiler is available for your system, uncompress the source codes of Geodesics to your home directory.
Type "make" at a command line prompt. If the compilation process is successful, you will see the executable file "geodesics" (or "geodesics.exe" on a Cygwin console). 

To run the Geodesics program, type "geodesics input_file_name", where "input_file_name" is the name of the input file.
To run the Geodesics program with the output file, type "geodesics input_file_name > output_
file_name", where "output_file_name" is the name of the intended output file name. The notation ">" is used for redirecting the standard output to the output file. The user may refer to the "data" directory to find a wide variety of input files.


## Brief description of the source codes

* main.cpp: This is the main file of the Geodesics program. It takes a file name as an input parameter and initializes the runtime environment for Geodesics.

* Geodesics.h: This file is the header file of Geodesics.cpp

* Geodesics.cpp: This file contains the rewriting related functions. In particular, this file contains the "Rewrite From Left" module and the "Balance" heuristic discussed in [1].

* KnuthBendix.h: This file is the header file of KnuthBendix.cpp

* KnuthBendix.cpp: This file contains the Knuth-Bendix procedure discussed in [1]. It finds the overlaps between the left-hand sides of rewrite rules and adds the rewrite rules using the "stack" data structure.

* Thue.h: This file is the header file of Thue.cpp

* Thue.cpp: This file contains the "Thue Resolution" discussed in [2]. It also contains the shortest path enumeration module using "Geodesics.cpp" and "KnuthBendix.cpp".

The "data" directory of the source codes of Geodesics consists of the following three directories.
(1) paper_data : This directory contains the input and output data used in our manuscript.
(2) Sims_Book_Examples: This directory contains the input and output data for a Knuth-Bendix procedure used in the book by Sims (see [1]).
(3) other_data: This directory contains the examples involving the bubble-sort, modified bubble-sort and star Cayley graphs.


## Input

The input of Geodesics basically consists of the following:
(1) The source and target vertex in Cay(G, S).
(2) A presentation of G with respect to a lexicographically ordered generating set S.
(3) The number k for finding k shortest paths (if they exist) from the source vertex to the target vertex in Cay(G, S).

Note: If k is set to 0, the program enumerates all shortest paths found from the source vertex to the target vertex in Cay(G, S).

# Input data description

An input data consists of the following fields and values:

* NumberOfGenerators: This field sets the number of generators of G for Cay(G,S).

* GeneratorList: This field sets the list of generators along with its lexicographic ordering in G for Cay(G,S). For example, if the GeneratorList is set as "a A b B", then the generator list of G is "a A b B" with the lexicographic ordering "a < A < b < B".

*NumberOfUnits: This field sets the number of inverses defined for generators.

* InversesForGenerators: This field sets the list of inverses for generators. When we consider Cayley graphs of finite groups, every generator should have its inverse. Each element in the list has the following form "a:A", which denotes the inverse of "a" is "A". We use the corresponding uppercase letter to represent the inverse element of a lowercase generator element in G.

* Identity: This field sets the identity element of G. "1" is the preferred value for this field. One may choose "e" as the identity of G unless the group has a generator element "e" in G.

* NumberK: This field sets the number k for the k shortest path enumeration from the source to the target vertex in Cay(G,S). If it is set as “0”, k is unbounded, which means that Geodesics enumerates every shortest path found from the source to the target vertex in Cay(G,S).

* SourceNode: This field sets the source vertex in Cay(G,S).

* TargetNode: This field sets the target vertex in Cay(G,S).

* NumberOfRelations: This field sets the number of relations for the presentation of G in Cay(G,S).

The relation list for a group presentation of G should be listed at the bottom of the input file. For example, the relations for the group presentation <a, A, b, B | aA=1, Aa=1, bB=1, Bb=1, aaa=1, bb=1, abab=1> are listed as follows:

1: aA, 1

2: Aa, 1

3: bB, 1

4: Bb, 1

5: aaa, 1

6: bb, 1

7: abab, 1


## Output

The output of Geodesics shows the following information:
(1) A reduced complete rewriting system for G along with its Thue resolution.
(2) The shortlex normal path and k shortest paths (if they exist) from the source vertex to the target vertex in Cay(G, S).

The following describes the sample output in an output file.

# Sample Output:
....
Generator List: a, A, b, B (a < A < b < B)

Identity Element in G: 1

....
Source Vertex in Cay(G, S): aB

Target Vertex in Cay(G, S): baba

The final reduced, complete rewriting system for G with respect to R: 

1 : aA-->1

2 : Aa-->1

3 : aa-->A

4 : AA-->a

5 : bb-->1

6 : B-->b

7 : ba-->Ab

8 : bA-->ab

....

The lexicographically minimal shortest path from vertex aB to vertex baba in Cay(G, S) is ab.
....

The number of shortest path(s) found from vertex aB to vertex baba is 4.

1: The shortest path #1 (Shortlex Normal Form): ab

2: The shortest path #2 : aB

3: The shortest path #3 : bA

4: The shortest path #4 : BA


## References

[1] C. C. Sims, Computation with finitely presented groups, Cambridge University Press, New York, NY, 1994.

[2] V. Diekert, A. J. Duncan, A. G. Myasnikov, Geodesic Rewriting Systems and Pregroups, in: O. Bogopolski, I. Bumagin, O. Kharlampovich, E. Ventura (Eds.), Combinatorial and Geometric Group Theory, Trends in Mathematics, Basel, Switzerland, 2010, pp.55-91.

[3] Dohan Kim, "Sorting on graphs by adjacent swaps using permutation groups", Computer Science Review, vol. 22, pp. 89-105, 2016.
