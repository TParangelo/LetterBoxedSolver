#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <set>
#include <vector> 
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <unistd.h>
#include <time.h>



using namespace std;

class prefixNode
{
    public:
        prefixNode* children[26];
        bool isValid;
    
    prefixNode() 
    {
        isValid = false;
        for(int i = 0; i < 26; i++)
        {
            children[i] = nullptr;
        }
    }
    
    void addWord(string word)
    {
        prefixNode* curr = this;
        for(char c : word)
        {
            if(curr->children[c - 'a'] == nullptr)
                curr->children[c - 'a'] = new prefixNode;
            curr = curr->children[c - 'a'];
        }
        curr->isValid = true;
    }
    
    bool isWord(string word)
    {
        prefixNode* curr = this;
        for(char c : word)
        {
            curr = curr->children[c - 'a'];
            if(curr == nullptr)
                return false;
        }
        if(curr->isValid)
            return true;
        return false;
    }
    
    bool isPrefix(string word)
    {
        prefixNode* curr = this;
        for(char c : word)
        {
            curr = curr->children[c - 'a'];
            if(curr == nullptr)
                return false;
        }
        return true;
    }

    vector<string> getWordsWithPrefix(string prefix) {
        vector<string> result;
        result.clear();
        prefixNode* node = getNodeForPrefix(prefix);

        if (node != nullptr) {
            // Perform DFS to get all words with the given prefix
            getWordsWithPrefixDFS(node, prefix, result);
        }

        return result;
    }

private:
    prefixNode* getNodeForPrefix(string prefix) {
        prefixNode* curr = this;
        for (char c : prefix) {
            curr = curr->children[c - 'a'];
            if (curr == nullptr)
                return nullptr;
        }
        return curr;
    }

    void getWordsWithPrefixDFS(prefixNode* node, string currentPrefix, vector<string>& result) {
        if (node == nullptr)
            return;

        if (node->isValid) {
            result.push_back(currentPrefix);
        }

        
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != nullptr) {
                char childChar = static_cast<char>('a' + i);
                getWordsWithPrefixDFS(node->children[i], currentPrefix + childChar, result);
            }
        }
    }

    
    
    
    
};



int getSide(char c, const vector<string>& sides)
{
    for(int i = 0; i < sides.size(); i++)
    {
        if(sides.at(i).find(c) != std::string::npos)
        {
            return i+1;
        }
    }
    return -1;
}

string solveFastest(prefixNode root, const vector<string>& sides, const vector<string>& possibleWords)
{
  string x = "\n";
  //cout << root.isWord(possibleWords.at(1)) << endl;
  unordered_set<char> letters;
  unordered_set<char> letters_need;
  for(string str : sides)
    {
      for(char c : str)
	{
	  letters.insert(c);
	  letters_need.insert(c);
	}
    }

     for (const char& x: letters)
       cout << x;

     cout << endl;
     string curLetter = "";
     unordered_set<string> built_words;
     while(!(letters_need.empty()))
      {
        int count = 0;
	      int curSide = -1;
	      int tempSide = 0;
	      string curWord = curLetter;
	      bool haveWord = false;
	      char notAllowed = ' ';
   
	      while(!haveWord)
	      {
          count++;
	        for (const char& x: letters)
	        {
		        string testWord = curWord + x; 
		        if( (x != notAllowed) &&  ((tempSide = getSide(x, sides)) != curSide) && (root.isPrefix(testWord)) && (letters_need.find(x) != letters_need.end()))
		        {
		          curWord = testWord;
		          
		          curSide = tempSide;
		          if(root.isWord(curWord))
		          {
			          haveWord = true;
			          break;
		          }
		        }
	        }
	        if(!haveWord)
	        {
		        for (const char& x: letters)
		          {
		            string testWord = curWord + x; 
  		          if( (x != notAllowed) && ((tempSide = getSide(x, sides)) != curSide) && (root.isPrefix(testWord)))
  		          {
  			          curWord = testWord;
  			          
  			          curSide = tempSide;
  			          if(root.isWord(curWord))
  			          {
  			            
  			            haveWord = true;
  			            break;
  			          }
  		          }
	       
  		        }
  	      }
    
        
        
	      if(built_words.find(curWord) != built_words.end() || !haveWord)
	      {
		      notAllowed = curWord.at(curWord.length()-1);
		      curWord.pop_back();
		      //cout << curWord << ":" << notAllowed << endl;
		      haveWord = false;

          if(count >= 15)
          {
            //cout<< "getting words with " << curLetter << endl;
            vector<string> poss = root.getWordsWithPrefix(curLetter);
          
            curWord = poss.at(rand() % poss.size()-1);
            curLetter = curWord.at(curWord.length()-1);
            haveWord = true;
            notAllowed = ' ';
            break;
          }
	      }
	   }

	 //cout << "Built word: " << curWord << endl;
   //cout << "Next Letter : " << curLetter << endl;
	 
   for(char c : curWord)
   {
    if(letters_need.find(c) != letters_need.end())
    {
      letters_need.erase(c);
    }
   }

   curLetter = curWord.at(curWord.length()-1);
	 built_words.insert(curWord);
	 x = x + curWord + "-";
  }

  //cout << "done rem" << endl;
       

  
  return x;

}





//Matt Code//
struct WordPair {
    string firstWord;
    string secondWord;
};

vector<WordPair> getTwoWords(const vector<string>& possWords, const unordered_set<char>& letters) {
    auto compareBySize = [](const string& a, const string& b) {
        return a.size() > b.size();
    };
    vector<string> sortedWords = possWords;
    sort(sortedWords.begin(), sortedWords.end(), compareBySize);

    WordPair result;
    vector<WordPair> results;

    for (const auto& firstWord : sortedWords) {
        unordered_set<char> remainingLetters = letters;
        for (char ch : firstWord) {
            auto it = remainingLetters.find(ch);
            if (it != remainingLetters.end()) {
                remainingLetters.erase(it);
            }
        }

        for (const auto& secondWord : sortedWords) {
            if (firstWord.back() == secondWord.front()) {
                unordered_set<char> remainingLettersCopy = remainingLetters;
                for (char ch : secondWord) {
                    auto it = remainingLettersCopy.find(ch);
                    if (it != remainingLettersCopy.end()) {
                        remainingLettersCopy.erase(it);
                    }
                }

                if (remainingLettersCopy.empty()) {
                    result.firstWord = firstWord;
                    result.secondWord = secondWord;
                    results.push_back(result);
                }
            }
        }
    }
    return results;
}






int main(int argc, char* argv[])
{
    prefixNode root;
    
    int numSides;
    vector<string> sides;
    vector<string> possibleWords;

    if(0)   //sides through command line
    {
     try{
         numSides = argc - 1;

         for(int i = 1; i < numSides + 1; i++)
         {
            sides.push_back(argv[i]);
          }
        } catch(exception e) { cout << "Wrong input format, should be: '{Letters on each side}*" << endl; }
    }

    if(1) //sides through input file 
    {
      if(argc != 3)
    {
        cout << "Incorrect format" << endl;
        return 0;
    }

      string inLine;
      ifstream myFile;
      myFile.open(argv[1]);

      while(getline(myFile, inLine))
      {
        sides.push_back(inLine);
      }
    }


    unordered_set<char> letters;
    for(string str : sides)
    {
        for(char c : str)
        {
            letters.insert(c);
        }
    }
    
    
     for (const char& x: letters) std::cout << x << " ";
        std::cout <<  std::endl;
    
    
    
    int dictCount = 0;
    int buildCount = 0;
    
    auto start1 = std::chrono::high_resolution_clock::now();
    string line;
    ifstream infile("dictionary.txt");
    while(getline(infile, line))
    {
        //cout<< "checking " << line << endl;
        dictCount++;
        bool buildable = true;
        
        char prevChar = ' ';
        int prevSide = -1;
        
        if(line.length() < 3)
            continue;
        
        for(char c : line)
        {
            int tempSide;
            if(letters.find(c) == letters.end() || prevChar == c || prevSide == (tempSide = getSide(c, sides))) //prevSide == (tempSide = getSide(c, sides)
            {
                buildable = false;
                break;
            }
            prevChar = c;
            prevSide = tempSide;
        }
        if(buildable)
        {
            root.addWord(line);
            possibleWords.push_back(line);
            //cout << line << " added" << endl;
            buildCount++;
        }
        
    }
    infile.close();
    
    //cout << "Build total: " << buildCount << endl;
    



    string quickest;
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration1 = end1 - start1;

    ofstream myFile2;
	myFile2.open(argv[2]);
    for(string str : sides)
    {
      myFile2 << str << " ";
    }
	  myFile2 << "\n" << "caculating all buildable words from dictionary took " << duration1.count() << " seconds" << endl;
    myFile2 << "Total playable words: " << buildCount << endl;
    myFile2 << "\n" << endl;

    auto start2 = std::chrono::high_resolution_clock::now();
    quickest = solveFastest(root, sides, possibleWords);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;
    myFile2 << "caculating quckiest words took " << duration2.count() << " seconds" << endl;
    quickest.pop_back();
    myFile2 << "Solution: " << quickest << endl;   
    
    myFile2 << endl;


		//Matt Code
    auto start3 = std::chrono::high_resolution_clock::now();
    vector<WordPair> combos = getTwoWords(possibleWords, letters);
    auto end3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration3 = end3 - start3;
    myFile2 << "caculating twoWord solutions took " << duration3.count() << " seconds" << endl;
    for(WordPair combo: combos){
        myFile2 << "First: " << combo.firstWord << "\n";
        myFile2 << "Second: " << combo.secondWord << "\n" << "\n";
    }	    

    std::chrono::duration<double> duration4 = end3 - start1;
    myFile2 << "\ntotal Runtime: " << duration4.count() << " seconds" << endl;
    myFile2.close();
    return 0;
}

