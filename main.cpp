// spell checker
// spellcheck.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
using namespace std;



// spell check function
void spellCheck(istream& inf, istream& wordlistfile, ostream& outf);


int main()
{
    // open input file
    ifstream inf("/Users/liaolily/Desktop/input1.txt");
    
    if(!inf)
    {
        cout << "cannot open input file: input.txt" << endl;
        exit(1);
    }
    
    // open word list file
    ifstream wordlistfile("/Users/liaolily/Desktop/wordlist.txt");
    
    if(!wordlistfile)
    {
        cout << "cannot open wordlist file: wordlist.txt" << endl;
        exit(1);
    }
    
    // open output file
    ofstream outf("output.txt");
    
    if(!outf)
    {
        cout << "cannot open output file: output.txt" << endl;
        exit(1);
    }
    
    // call spell check
    spellCheck(inf, wordlistfile, cout);
    
    
    // close all file's
    inf.close();
    wordlistfile.close();
    outf.close();
    
    // pause
    cin.get();
    
    return 0;
}

// spell check
void spellCheck(istream& inf, istream& wordlistfile, ostream& outf)
{
    
    // hashtable class
    class Hashtable
    {
        
    private:
        
        vector <string>* items;
        
    public:
        
        // make hash table of size sz
        Hashtable(int sz)
        {
            items = new  vector <string> [10000];
        }
        
        
        unsigned int hash (string toHash) {
            unsigned int result = 0;
            for (int i = 0; i < toHash.length(); i++) {
                result = result * 101 + toHash[i];
            }
            return result % 10000;
        }

        // insert item into hash
        void insert(string s)
        {
                       // put word into hash table
            items[Hashtable::hash(s)].push_back(s);
        }
        
        // return true if contains item
        bool contains(string s)
        {
            
            
            // find word
            bool found = false;
            for(unsigned int i=0;i<items[Hashtable::hash(s)].size()&&!found;i++)
            {
                if(items[Hashtable::hash(s)] == s)
                {
                    found = true;
                }
            }
            
            return found;
        }
        
        // convert to upper case
        static string upper(string s)
        {
            string s2;
            
            for(unsigned int i=0;i<s.size();i++)
            {
                s2.append(1,toupper(s[i]));
            }
            
            return s2;
        }
    };
    
    // commonly used variables
    unsigned int i,j,k; // loop counters
    string word,word1,word2; // words
    Hashtable ht(10000); // hash table of words
    list<string> suggestions; // list of suggestions
    string line; // read line of text
    char w[80]; // to manipulate word
    bool found; // word found in hashtable
    
    
    // read in wordlist
    
    // get number words
    unsigned int n = 0;
    wordlistfile >> n;
    
    // for all words in file
    for(i=0;i < n;i++)
    {
        // read word from file
        wordlistfile >> word;
        
        // put in hash table
        word = ht.upper(word);
        ht.insert(word);
    }
    
    
    // read words from file
    // look for misplelled words
    while(inf)
    {
        // read line
        getline(inf,line);
        
        istringstream sin(line);
        
        if(!inf) break;
        
        // loop for all words in line
        while(sin)
        {
            // read word
            sin >> word;
            word = ht.upper(word);
            
            // look up word in hash table
            if(!ht.contains(word))
            {
                outf << line << endl;
                outf << "word not found: " << word << endl;
                
                suggestions.clear(); // clear suggestions
                
                // swapping each adjacent pair of character in the word
                for(i=0;i<word.size()-1;i++)
                {
                    for(j=0;j<word.size();j++)
                        w[j] = word[j];
                    w[j] = 0;
                    
                    char x = w[i];
                    char y = w[i+1];
                    w[i+1] = x;
                    w[i] = y;
                    
                    // look up word in hash table
                    if(ht.contains(word))
                    {
                        
                        // check for duplicates
                        found = false;
                        list<string>::iterator itr;
                        for(itr=suggestions.begin(); itr!=suggestions.end() && !found; itr++)
                        {
                            if(*itr==word2)
                                found = true;
                        }
                        
                        if(!found)
                            suggestions.push_back(word);
                    }
                }
                
                
                // insert letters A to Z between adjacent characters
                for(i=0;i<=word.size();i++)
                {
                    for(j=0;j<26;j++)
                    {
                        word2 = word;
                        
                        word2.insert(i,1,(char)('A'+j));
                        
                        // look up word in hash table
                        if(ht.contains(word2))
                        {
                            // check for duplicates
                            found = false;
                            list<string>::iterator itr;
                            for(itr=suggestions.begin(); itr!=suggestions.end() && !found; itr++)
                            {
                                if(*itr==word2)
                                    found = true;
                            }
                            
                            if(!found)
                                suggestions.push_back(word2);
                        }
                    }
                }
                
                // deleting each character from word
                for(i=0;i<word.size();i++)
                {
                    
                    word2 = word;
                    
                    word2.erase(i,1);
                    
                    // look up word in hash table
                    if(ht.contains(word2))
                    {
                        // check for duplicates
                        found = false;
                        list<string>::iterator itr;
                        for(itr=suggestions.begin(); itr!=suggestions.end() && !found; itr++)
                        {
                            if(*itr==word2) 
                                found = true;
                        }
                        
                        if(!found)
                            suggestions.push_back(word2);
                    }
                    
                }
                
                // replacing each letter with  A to Z 
                for(i=0;i<word.size();i++)
                {
                    for(j=0;j<26;j++)
                    {
                        
                        for(k=0;k<word.size();k++)
                            w[k] = word[k];
                        w[k] = 0;
                        
                        w[i] = (char)('A' + j);
                        
                        // look up word in hash table
                        if(ht.contains(w))
                        {
                            // check for duplicates
                            found = false;
                            list<string>::iterator itr;
                            for(itr=suggestions.begin(); itr!=suggestions.end() && !found; itr++)
                            {
                                if(*itr==word2) 
                                    found = true;
                            }
                            
                            if(!found)
                                suggestions.push_back(w);
                        }
                    }
                }
                
                // split words
                for(i=1;i<word.size()-1;i++)
                {
                    // split words
                    word1 = word.substr(0,i);
                    word2 = word.substr(i,word.size()-i+1);
                    
                    // two words found
                    if(ht.contains(word1) && ht.contains(word2))
                    {
                        // check for duplicates
                        found = false;
                        list<string>::iterator itr;
                        for(itr=suggestions.begin(); itr!=suggestions.end() && !found; itr++)
                        {
                            if(*itr==word2) 
                                found = true;
                        }
                        
                        if(!found)
                            suggestions.push_back(word1 + " " + word2);
                    }
                }
                
                // print out suggestions
                if(suggestions.size() > 0)
                {
                    outf << "perhaps you meant: " << endl;
                    
                    // sort suggestions
                    suggestions.sort();
                    
                    // print suggestions
                    list<string>::iterator itr;
                    for(itr=suggestions.begin(); itr!=suggestions.end(); itr++)
                    {
                        outf << "\t" << *itr << endl;
                    }
                }
                
                else outf << "No suggestions" << endl;
            }
        }
    }
}


