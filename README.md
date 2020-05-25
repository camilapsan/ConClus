/**************************************************************************
*	 ConClus software
*
*	 Copyright (c) 2015 Camila Santos, Mariá Nascimento
*
*	 Paper: doi:10.1016/j.eswa.2016.01.026
*
*	 This file is part of ConClus software.
*
* 	 ConClus software is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    ConClus software is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*****************************************************************************/

1. Dependencies
	C++ 2011

---------------------------------------------------------
2. Compile
	
	bash compile.sh
	
	OR
	
	mkdir obj
	make

---------------------------------------------------------
3. Run (terminal)
	./ConClus_release.out <folder_input/> <file_input> <folder_output/> <lambda> <weighted>

	Obs: < > is used only to indicate the parameters.
	
	Example:
			mkdir output
			./ConClus_release.out "graph_example/" "karate" "output/" 0

	---------------
	#Inputs
		The parameters for the algorithm are: 
	
		<folder_input/>: path to the folder where the input digraph is located, must end with /

		<file_input>: file name of the input digraph in pajek format and extension .net
				if the file is karate.net, the parameter is only "karate"

		<folder_output/>: folder to output the partitions and resulting files
			Output folder must exist previously
			The algorithm will automatically generate output files.

		<lambda>: value of the lambda (>0) parameter for adjusted modularity
			Classic modularity: <lambda>= 1

		<weighted>: 1 for weighted digraphs
				    0 for unweighted digraphs
	---------------
	#Output:
		Partition: "part_<file_input>_<lambda>.000000.com" in <folder_output>

---------------------------------------------------------
