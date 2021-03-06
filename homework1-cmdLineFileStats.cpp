#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

int main(int argc, char *argv[])
{
    ifstream inFile;
    map<string, int> wordCount;
    map<int, vector<string>> byCount;
    string word = "";
    string sLine = "";
    int longestLine = 0;
    int longestWord = 0;
    int numLongestLines = 1;
    int numFiles = 0;
    int numLongestWord = 0;
    int length = 0;
    int size = 0;
    int q = 0;
    bool mFlag = false;
    bool cFlag = false; //initialization of the
    bool bFlag = false; //possible flags
    bool xFlag = false;
    //a flag is an argument whose first character is a dash
    //so any file names or paths that start with a dash are
    //considered flags
    //therefore I do not have to check for the
    //case in which a directory or file's name starts with a -
    for (int j = 1; j < argc; j++)
    {
        if (argv[j][0] == '-')
        {
            switch (argv[j][1])
            {
            case 'm':
                mFlag = true;
                break;
            case 'c':
                cFlag = true;
                break;
            case 'b':
                bFlag = true;
                break;
                //if the flag is unrecognized
            default: //xFlag set to false, string printed
                     //then the loops break and return main
                xFlag = true;
                cout << argv[j] << " UNRECOGNIZED FLAG" << endl;
                break;
            }
        }
        else
        {
            numFiles++;
        }
        if (xFlag) //stops the main loop
        {
            break;
        }
    }
    if (xFlag) // returns main and terminates the program safely
    {
        return 0;
    }
    else if (numFiles == 0)
    {
        cout << "NO FILES" << endl;
        return 0;
    }
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            inFile.open(argv[i]);
            if (inFile.is_open())
            {
                longestWord = 0;
                longestLine = 0;
                numLongestLines = 1;
                wordCount.erase(wordCount.begin(), wordCount.end());
                while (getline(inFile, sLine))
                { //changes sLine
                    istringstream line(sLine);
                    length = 0;
                    if (!bFlag) {
                        length = sLine.length();
                        if (length > longestLine)
                        {
                            longestLine = length;
                            numLongestLines = 1;
                        }
                        else if (length == longestLine)
                        {
                            numLongestLines++;
                        }
                    }
                    while (line >> word)
                    {
                        if (bFlag)
                        {
                            length += word.length();
                            if (word.length() != 0) length++;
                            if (length > longestLine)
                            {
                                longestLine = length;
                                numLongestLines = 1;
                            }
                            else if (length == longestLine)
                            {
                                numLongestLines++;
                            }
                        }
                        if (word.length() == (unsigned)longestWord)
                        {
                            wordCount[word]++;
                        }
                        if ((unsigned)longestWord < word.length())
                        {
                            wordCount.erase(wordCount.begin(), wordCount.end());
                            wordCount[word] = 1;
                            longestWord = word.length();
                        }
                    }
                }
                if (longestLine != 0 && bFlag) longestLine--; // to account for no spaces at the end of the line
                if (longestLine == 0)
                {
                    longestLine = 0;
                    numLongestLines = 1;
                }
                inFile.close();
            }
            else
            {
                cout << argv[i] << " FILE NOT FOUND" << endl;
                continue;
            }
        }
        else
        {
            continue;
        }
        cout << argv[i] << ":" << endl;
        if (mFlag && (longestLine != 0))
        {
            byCount.erase(byCount.begin(), byCount.end());
            map<string, int>::iterator cit;
            for (cit = wordCount.begin(); cit != wordCount.end(); cit++)
            {
                //goes to the map index for the count of the word
                //then adds the word to the vector at that index
                byCount[cit->second].push_back(cit->first);
            }
            //gets the last key in the map which should be the highest occurring words
            map<int, vector<string>>::reverse_iterator sit = byCount.rbegin();
            //assigns the quantity of highest occurring words to numLongestWord
            numLongestWord = sit->first;
            //trims the vector at that key location
            byCount[numLongestWord].shrink_to_fit();
            //sorts the vector at that key location
            sort(byCount[numLongestWord].begin(), byCount[numLongestWord].end());
            vector<string>::iterator qit = byCount[numLongestWord].begin();
            size = byCount[numLongestWord].size();
            q = 0;
            while (qit != byCount[numLongestWord].end())
            {
                if (q < size - 1)
                {
                    if (cFlag)
                    {
                        cout << *qit << "(" << numLongestWord << "), ";
                    }
                    else
                    {
                        cout << *qit << ", ";
                    }
                }
                else
                {
                    if (cFlag)
                    {
                        cout << *qit << "(" << numLongestWord << ")";
                    }
                    else
                    {
                        cout << *qit;
                    }
                }
                q++;
                qit++;
            }
        }
        else
        {
            map<string, int>::iterator it;
            q = 0;
            size = wordCount.size();
            for (it = wordCount.begin(); it != wordCount.end(); it++)
            {//sorted and comma-separated longest words
                if (q < size - 1)
                {
                    if (cFlag)
                    {
                        cout << it->first << "(" << wordCount[it->first] << "), ";
                    }
                    else
                    {
                        cout << it->first << ", ";
                    }
                }
                else
                {
                    if (cFlag)
                    {
                        cout << it->first << "(" << wordCount[it->first] << ")";
                    }
                    else
                    {
                        cout << it->first;
                    }
                }
                q++;
            }
        }
        if (!wordCount.empty())
        {
            cout << endl;
        }
        cout << longestLine;
        if (cFlag)
        {
            cout << "(" << numLongestLines << ")";
        }
        cout << endl;
    }
}