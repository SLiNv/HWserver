#include "tokenSearch.h"

/*A helper function that is used to construct a table for the keyword
 in linear time with respect to the keyword given. This helper function
 is used in the Knuth–Morris–Pratt token searching algorithm for single
 tokens in order to eliminate redundant comparisons in the student string.
 The expected arguments are an integer buffer the same size as the string
 keyword and a keyword that accepts any ASCII character. The behavior
 for the function with a buffer less than the size of the keyword is
 not predictable and should not be used.*/
void buildTable ( int* V, const std::string& keyword ) {
	int j = 0;

	//Table initialization
	V[0] = -1;
	V[1] = 0;
	for ( unsigned int i = 2; i < keyword.size(); i++ ) {
		if ( keyword[i - 1] == keyword[j] ) {
			j++;
			V[i] = j;
		} else if ( j > 0 ) {
			j = V[j];
			i--;
		} else {
			V[i] = 0;
		}
	}
}
/*searchToken looks for a token specified in the second argument in the
 student output. The algorithm runs in linear time with respect to the
 length of the student output and preprocessing for the algorithm is
 linear with respect to the token. Overall, the algorithm runs in O(N + M)
 time where N is the length of the student and M is the length of the token.*/
TestResults* searchToken ( const std::string& student,
			   const std::vector<std::string>& token_vec ) {

	//Build a table to use for the search
	Tokens* diff = new Tokens();
	diff->num_tokens = 1;
	//buildTable( V, token );
	assert (token_vec.size() == 1);
	int V[token_vec[0].size()];
	//int V[token.size()];
	buildTable( V, token_vec[0] );

	std::cout << "searching for " << token_vec[0] << std::endl;

	int m = 0;
	int i = 0;
	while ( m + i < student.size() ) {
	  //if ( student[i + m] == token[i] ) {
		if ( student[i + m] == token_vec[0][i] ) {
		  //if ( i == token.size() - 1 ) {
			if ( i == token_vec[0].size() - 1 ) {
				diff->tokens_found.push_back( m );
				std::cout << "found! " << std::endl;
				diff->setGrade(1);	
				return diff;
			}

			i++;
		} else {
			m += i - V[i];
			if ( V[i] == -1 )
				i = 0;
			else
				i = V[i];
		}
	}

	diff->tokens_found.push_back( -1 );

	std::cout << "not found!" << std::endl;
	diff->setGrade(0);

	return diff;
}
/*searchAllTokens looks for tokens delimited by newline characters in the
 student output. The algorithm runs in linear time with respect to the
 length of the student output and preprocessing for the algorithm is
 linear with respect to the token. Overall, the algorithm runs in O(N + M)
 time where N is the length of the student and M is the length of the token.*/
TestResults* searchAllTokens ( const std::string& student,
			       const std::vector<std::string>& token_vec ) {
	Tokens* difference = new Tokens();
	difference->partial = false;
	difference->harsh = true;


	difference->setGrade(0);

	//std::vector< std::string > tokenlist;
	std::vector< std::string > tokenlist = token_vec;
	//tokenlist = splitTokens( tokens );

	difference->num_tokens = tokenlist.size();
	for ( unsigned int i = 0; i < tokenlist.size(); i++ ) {
		difference->tokens_found.push_back(
				RabinKarpSingle( tokenlist[i], student ) );
	}
	return difference;
}

TestResults* searchAnyTokens ( const std::string& student,
			       const std::vector<std::string>& token_vec ) {
	Tokens* difference = new Tokens();
	difference->partial = false;
	difference->harsh = false;
	//std::vector< std::string > tokenlist;
	std::vector< std::string > tokenlist = token_vec;
	  //tokenlist = splitTokens( tokens );
	difference->num_tokens = tokenlist.size();
	for ( unsigned int i = 0; i < tokenlist.size(); i++ ) {
		difference->tokens_found.push_back(
				RabinKarpSingle( tokenlist[i], student ) );
	}
	difference->setGrade(0);
	return difference;
}

TestResults* searchTokens ( const std::string& student,
			    const std::vector<std::string>& token_vec ) {
	Tokens* difference = new Tokens();
	difference->partial = true;
	//std::vector< std::string > tokenlist;
	std::vector< std::string > tokenlist = token_vec;
	//tokenlist = splitTokens( tokens );
	difference->num_tokens = tokenlist.size();
	for ( unsigned int i = 0; i < tokenlist.size(); i++ ) {
		difference->tokens_found.push_back(
				RabinKarpSingle( tokenlist[i], student ) );
	}
	difference->setGrade(0);
	return difference;
}

/*	Looks for a single token in a string using the Rabin-Karp rolling hash
 method.  Returns starting index if found, -1 if not.					*/
int RabinKarpSingle ( std::string token, std::string searchstring ) {
	long hash = 0;
	long goalhash = 0;
	unsigned int tlen = ( unsigned int ) token.size();
	if ( searchstring.size() < token.size() ) {
		return -1;
	}
	for ( int i = 0; i < tlen; i++ ) // Set up goal hash
			{
		goalhash += token[i];
	}
	for ( int i = 0; i < tlen; i++ ) // Set up first hash
			{
		hash += searchstring[i];
	}
	for ( int i = 0; i <= searchstring.size() - tlen; i++ ) {
		// Check if hashes then strings are equal, and if so return index
		if ( hash == goalhash && searchstring.substr( i, tlen ) == token ) {
			return i;
		}
		hash += searchstring[i + tlen];
		hash -= searchstring[i];
	}
	return -1;
}


std::vector< std::string > splitTokens ( const std::string& tokens ) {
	std::vector< std::string > tokenlist;
	std::string tmpstr; // Create empty token variable

	// Start at 1 to avoid first double quote
	for ( int i = 1; i < tokens.size(); i++ ) {
		// If we're at a delimiter...
		if ( tokens[i] == '\"' && tokens[i + 1] == '\n'
				&& tokens[i + 2] == '\"' ) {
			if ( tmpstr != "" ) {
				tokenlist.push_back( tmpstr );
			}
			tmpstr.clear();
			i = i + 2; // Skip to end of said delimiter
		} else if ( ( tokens.size() - i == 2 ) && tokens[i] == '\"'
				&& tokens[i + 1] == '\n' ) {
			if ( tmpstr != "" ) {
				tokenlist.push_back( tmpstr );
			}
			tmpstr.clear();
			i = i + 1;
		} else if ( ( tokens.size() - i == 1 ) && tokens[i] == '\"' ) {
			if ( tmpstr != "" ) {
				tokenlist.push_back( tmpstr );
			}
			tmpstr.clear();
		}

		else {
			tmpstr += tokens[i];
		}
	}
	if ( tmpstr != "" ) {
		tokenlist.push_back( tmpstr );
	}
	return tokenlist;
}

