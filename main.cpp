#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_set>

using namespace std;

void getDimensions(string fileName, size_t & gridWidth, size_t & gridHeight)
{
	ifstream puzzleFile(fileName);

	char currChar;

	cout << "Getting grid dimensions from '"<< fileName << "'... ";

	while(puzzleFile.get(currChar) && currChar != '\n'){
		if(currChar != ' ') { ++gridWidth; }
	}

	while (puzzleFile.get(currChar)) {
        if (currChar == '\n') { ++gridHeight; }
    }
    ++gridHeight; // because we already read through the first row when calculating width

    puzzleFile.close();

}

void loadDictionary(string fileName, unordered_set<string> & dictionary)
{
	ifstream inputFile(fileName);
	string currWord;
	while(inputFile >> currWord){
		if(currWord.length() >= 4){ //<---- FEEL FREE TO CHANGE THE IMPOSED-MINIMUM-CHARACTER-COUNT PER WORD! (I would strongly discourage '1' or '2'.)
			dictionary.insert(currWord);
			//cout << "Loading '" << currWord << "' to dictionary. " << endl;
		}
	}
	inputFile.close();
}

int main(){

	string puzzleIQ = "stackof_puzzle.txt"; //<---- FEEL FREE TO CHANGE WHICH FILE THE ALGORITHM CONSIDERS!

	size_t gridWidth = 0;
	size_t gridHeight = 0;

	getDimensions(puzzleIQ,gridWidth,gridHeight);
	cout << "complete!" << endl;
	
	char letterGrid[gridWidth][gridHeight];

	cout << "Populating letterGrid with file contents... ";
	ifstream puzzleFile(puzzleIQ);

	for(size_t currY = 0; currY < gridHeight; ++currY){
		char currChar;
		size_t currX = 0;
		while(puzzleFile.get(currChar) && currChar != '\n'){
			if(currChar != ' '){
				//cout << "Attempting to insert '" << currChar << "' into index: [" << currX << "][" << currY << "]" << endl;
				letterGrid[currY][currX] = currChar; //reversed X and Y...?
				++currX;

			}
		}
	}
	cout << "complete!" << endl;
	puzzleFile.close();
	

	// cout << "Now printing 2D array..." << endl;
	// for (size_t i = 0; i < gridHeight; ++i) {
    //     for (size_t j = 0; j < gridWidth; ++j) {
    //         std::cout << letterGrid[i][j] << ' ';
    //     }
    //     cout << endl;
    // }

	unordered_set<string> dictionary;
	cout << "Populating dictionary with file contents... ";
	loadDictionary("words.txt",dictionary);
	// cout << "Now printing dictionary entries..." << endl;
	// for(unordered_set<string>::iterator it = dictionary.begin(); it != dictionary.end(); ++it){
	// 	cout << *it << endl;
	// }
	cout << "complete!" << endl;
	cout << "Searching " << gridWidth << "x" << gridHeight << " grid for " << dictionary.size() << " possible words... ";


	multimap< string, pair < pair<size_t,size_t>,pair<size_t,size_t> > > WATC;
	// WORD FOUND^^			  X_in^^	 ^^Y_in  X_end^^		 ^^Y_end


	for(unordered_set<string>::iterator it = dictionary.begin(); it != dictionary.end(); ++it){
		//cout << "-----------> Searching matrix for '" << *it << "'." << endl;
		for(int y = 0; y < gridWidth; ++y){
			for(int x = 0; x < gridHeight; ++x){
				if((*it)[0] == letterGrid[y][x] || (*it)[0] == toupper(letterGrid[y][x]) ){ 
					//cout << "'" << (*it)[0] << "' found at position [" << y << "," << x << "]." << endl;
					int tempLength = (*it).length();

					//THE CARDINAL DIRECTIONS
					if(!(x + tempLength - 1 > gridWidth - 1)){ //EAST
						bool wordFound = true;
						int xx = 1; //because *it[0] has already been confirmed
						while(xx < tempLength){
							if( letterGrid[y][x + xx] != (*it)[xx]){ wordFound = false; break; }
							++xx;
						}
						if(wordFound){ 
							//cout << "> '" << *it << "' has been found east of [" << y << "," << x << "]." << endl; 
							WATC.insert( make_pair(*it,make_pair(make_pair(y,x), make_pair(y,x+xx-1) ) ) );
						}
					}


					if(!(x - tempLength + 1 < 0)){ //WEST
						bool wordFound = true;
						int xx = 1; //because *it[0] has already been confirmed
						while(xx < tempLength){
							if( letterGrid[y][x - xx] != (*it)[xx]){ wordFound = false; break; }
							++xx;
						}
						if(wordFound){ 
							//cout << "> '" << *it << "' has been found west of [" << y << "," << x << "]." << endl; 
							WATC.insert( make_pair(*it,make_pair(make_pair(y,x), make_pair(y,x-xx+1) ) ) );
						}
					}


					if(!(y + tempLength - 1 > gridHeight - 1)){ //SOUTH
						bool wordFound = true;
						int yy = 1; //because *it[0] has already been confirmed
						while(yy < tempLength){
							if( letterGrid[y + yy][x] != (*it)[yy]){ wordFound = false; break; }
							++yy;
						}
						if(wordFound){ 
							//cout << "> '" << *it << "' has been found south of [" << y << "," << x << "]." << endl; 
							WATC.insert( make_pair(*it,make_pair(make_pair(y,x), make_pair(y+yy-1,x) ) ) );
						}
					}

					if(!(y - tempLength + 1 < 0)){ //NORTH
						bool wordFound = true;
						int yy = 1; //because *it[0] has already been confirmed
						while(yy < tempLength){
							if( letterGrid[y - yy][x] != (*it)[yy]){ wordFound = false; break; }
							++yy;
						}
						if(wordFound){ 
							//cout << "> '" << *it << "' has been found north of [" << y << "," << x << "]." << endl; 
							WATC.insert( make_pair(*it,make_pair(make_pair(y,x), make_pair(y-yy+1,x) ) ) );
						}
					}

					//THE ORDINAL DIRECTIONS
					if(!(x + tempLength - 1 > gridWidth - 1 || y + tempLength - 1 > gridHeight - 1 )){ //SOUTHEAST
						bool wordFound = true;
						int xx = 1;
						int yy = 1;
						while(xx < tempLength){
							if( letterGrid[y + yy][x + xx] != (*it)[xx]){ wordFound = false; break; }
							//cout << letterGrid[y + yy][x + xx] << " matches " << (*it)[xx] << endl;
							++xx;
							++yy;
						}
						if(wordFound){ 
							//cout << "> '" << *it << "' has been found southeast of [" << y << "," << x << "]." << endl; 
							WATC.insert( make_pair(*it,make_pair(make_pair(y,x), make_pair(y+yy-1, x+yy-1) ) ) );
						}
					}

					if(!(x + tempLength - 1 > gridWidth - 1 || y - tempLength + 1 < 0 )){ //NORTHEAST
						bool wordFound = true;
						int xx = 1;
						int yy = 1;
						while(xx < tempLength){
							if( letterGrid[y - yy][x + xx] != (*it)[xx]){ wordFound = false; break; }
							//cout << letterGrid[y + yy][x + xx] << " matches " << (*it)[xx] << endl;
							++xx;
							++yy;
						}
						if(wordFound){ 
							//cout << "> '" << *it << "' has been found northeast of [" << y << "," << x << "]." << endl; 
							WATC.insert( make_pair(*it,make_pair(make_pair(y,x), make_pair(y-yy+1,x+xx-1) ) ) );
						}
					}

					if(!(x - tempLength + 1 < 0 || y + tempLength - 1 > gridHeight - 1 )){ //SOUTHWEST
						bool wordFound = true;
						int xx = 1;
						int yy = 1;
						while(xx < tempLength){
							if( letterGrid[y + yy][x - xx] != (*it)[xx]){ wordFound = false; break; }
							//cout << letterGrid[y + yy][x + xx] << " matches " << (*it)[xx] << endl;
							++xx;
							++yy;
						}
						if(wordFound){ 
							//cout << "> '" << *it << "' has been found southwest of [" << y << "," << x << "]." << endl; 
							WATC.insert( make_pair(*it,make_pair(make_pair(y,x), make_pair(y+yy-1,x-xx+1) ) ) );
						}
					}

					if(!(x - tempLength + 1 < 0 || y - tempLength + 1 < 0 )){ //NORTHWEST
						bool wordFound = true;
						int xx = 1;
						int yy = 1;
						while(xx < tempLength){
							if( letterGrid[y - yy][x - xx] != (*it)[xx]){ wordFound = false; break; }
							//cout << letterGrid[y + yy][x + xx] << " matches " << (*it)[xx] << endl;
							++xx;
							++yy;
						}
						if(wordFound){ 
							//cout << "> '" << *it << "' has been found northwest of [" << y << "," << x << "]." << endl; 
							WATC.insert( make_pair(*it,make_pair(make_pair(y,x), make_pair(y-yy+1,x-xx+1) ) ) );
						}
					}
				}
			}
		}
	}
	cout << "complete!" << endl;
	cout << "-----------------------------------------------------------------" << endl;
	cout << "Algorithm has found " << WATC.size() << " words. Printing them now..." << endl;
	cout << "-----------------------------------------------------------------" << endl;
	for(multimap < string, pair < pair<size_t,size_t>,pair<size_t,size_t> > >::iterator it = WATC.begin(); it!=WATC.end(); ++it){
		cout << "> Word '" << it->first << "' is present from [" << it->second.first.first << "," << it->second.first.second << "] to [" << it->second.second.first << "," << it->second.second.second << "]." << endl; 
	}

	return 0;

}