//
//  secondtry.cpp
//  sundaynewprojectfour
//
//  Created by Chelsey Wang on 6/2/19.
//  Copyright © 2019 Chelsey Wang. All rights reserved.
//

//
//  main.cpp
//  sundaynewprojectfour
//
//  Created by Chelsey Wang on 6/2/19.
//  Copyright © 2019 Chelsey Wang. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <utility>
using namespace std;


//Read in the entire contents of the old file into a string. Read the entire contents of the new file into another string.
//For all consecutive N-character sequences in the old file's string, insert that N-character sequence and the offset F where it was found in the old file's string, into a table (e.g. hash table or binary search tree). You might use 8 for N, or maybe 16.
//Once you have filled up your table with all N-byte sequences from the source file, start processing the new file's string, starting from offset j=0, until j reaches the end of the string.
//Look up the N-byte sequence which starts at offset j ([j,j+N-1]) in the new file's string in the table you created from the old file's string.
//If you find this sequence in the table, you know that that sequence is present in both the old and new versions of the file.
//Determine how long the match goes on (it might be just N bytes long, or it might extend past the first N matching bytes to be many bytes longer).
//Once you have determined how long the match is (call this L), write a Copy instruction to the diff file to copy L bytes from offset F from the source file.
//Go back to step 3a, continuing at offset j = j + L in the new file's string.
//If you don't find the current sequence (new file's string [j,j+N-1]) in the table you created, you know that the first version of the file doesn't contain the current N byte sequence.
//Write an instruction to the diff file to Add the current character.
//Increment j by one to continue past the character used in the add instruction.
//Go back to step 3a, where we'll try to find the next N-byte sequence in our table.


//by changing my hash function, if my baby string hashes to something, it means there is a perfect match--i just need to check through my entire linked list to find that match.
//if it does not hash to anything, it means that I need to search through my entire hash table for a perfect match.


//adding a vector now
//if you find a perfect match, push back into a vector. then compare with the commands in that vector, to see which one is the best--by checking, if you continue from the offset of that vector, if it would continue matching
//probably make a new int to help keep track of the amtInCom
//this will help you find the perfectest match

////////////////////////
//FIGURE OUT HOW TO NOT STORE DUPLICATE NODES//
//run strange
//figure out spaces for war and peace



const int CONST_HASH_SIZE = 15581;
const int CONST_SIZE = 16;
int id (string chunk);
struct Node
{
    Node(string chunk):mChunk(chunk){}
    string mChunk;
    //int offset;
    vector<int>offsets;
    Node* next = nullptr;
    //int getId(){return ++myId;}
    
};

void createDiff(istream& fold, istream& fnew, ostream& fdiff);

bool applyDiff(istream& fold, istream& fdiff, ostream& fnew);

int index(string baby);

int matchingString(string oldBoy, string newBoy);

bool getInt(istream& inf, int& n);

bool getCommand(istream& inf, char& cmd, int& length, int& offset);

void anothaCreateDiff(istream& fold, istream& fnew, ostream& fdiff);
void anothaaCreateDiff(istream& fold, istream& fnew, ostream& fdiff);
void noCreateDiff(istream& fold, istream& fnew, ostream& fdiff);

int main() {

//    //TESTING CREATE DIFF HERE//
    ofstream outfile("/Users/chelseyywang/Desktop/cs32proj4/cs32proj4/testdiff.txt");
    ifstream infile1("/Users/chelseyywang/Desktop/cs32proj4/cs32proj4/greeneggs1.txt");
    ifstream infile2("/Users/chelseyywang/Desktop/cs32proj4/cs32proj4/greeneggs2.txt");

    createDiff(infile1, infile2, outfile);
    outfile.close();
    // TESTING APPLY DIFF//
    ofstream ooutfile("/Users/chelseyywang/Desktop/cs32proj4/cs32proj4/testnew.txt");
    ifstream iinfile1("/Users/chelseyywang/Desktop/cs32proj4/cs32proj4/greeneggs1.txt");
    ifstream iinfile2("/Users/chelseyywang/Desktop/cs32proj4/cs32proj4/testdiff.txt");

    if (!iinfile1||!iinfile2)
    {cerr<<"yikes"<<endl;
        return 2;
    }
    cerr<<applyDiff(iinfile1, iinfile2, ooutfile);
    return 0;
}

//#include <iostream>
//#include <sstream>  // for istringstream and ostringstream
////#include <string>
//#include <cassert>
//using namespace std;
//
//void runtest(string oldtext, string newtext)
//{
//    istringstream oldFile(oldtext);
//    istringstream newFile(newtext);
//    ostringstream diffFile;
//    createDiff(oldFile, newFile, diffFile);
//    string result = diffFile.str();
//    cout << "The diff file length is " << result.size()
//    << " and its text is " << endl;
//    cout << result << endl;
//
//    oldFile.clear();   // clear the end of file condition
//    oldFile.seekg(0);  // reset back to beginning of the stream
//    istringstream diffFile2(result);
//    ostringstream newFile2;
//    assert(applyDiff(oldFile, diffFile2, newFile2));
//    assert(newtext == newFile2.str());
//}
//
//int main()
//{
//    runtest("There's a bathroom on the right.",
//            "There's a bad moon on the rise.");
//    runtest("ABCDEFGHIJBLAHPQRSTUVPQRSTUV",
//            "XYABCDEFGHIJBLETCHPQRSTUVPQRSTQQELF");
//    cout << "All tests passed" << endl;
//}


int matchingString(string oldBoy, string newBoy)
{
    //cerr<<"MATCHING STRING: "<<oldBoy<<" {and} "<<newBoy<<endl;
    int counter = 0;
    for (int i = 1; i<=CONST_SIZE; i++)
    {
        if (oldBoy.substr(0, i) == newBoy.substr(0, i))
        {
            //cerr<<"char in common: "<<oldBoy[i - 1]<<"&"<<newBoy[i - 1]<<endl;
            counter++;
        }
    }
    return counter;
}


string read(istream &in)
{
    string sumthin;
    char buffer[4096];
    while (in.read(buffer, sizeof(buffer)))
    {
        sumthin.append(buffer, sizeof(buffer));
    }
    sumthin.append(buffer, in.gcount());
    return sumthin;
}


void createDiff(istream& fold, istream& fnew, ostream& fdiff)
{
    //copying files into strings
    
    
    string oldFile = read(fold);
//    string s;
//    while (getline(fold, s))
//    {
//        oldFile+=s;
////        if ()
////        oldFile+='\n';
//    }
    string newFile = read(fnew);
//    string t;
//    while (getline(fnew, t))
//    {
//        newFile+=t;
//    }
    string diffFile = "";
    vector<string>diff;
    if (oldFile.size()<1000)
    {
        cerr<<"OldFIle"<<oldFile<<endl;
    }
    if (newFile.size()<1000)
    {
        cerr<<"NEWFIle"<<newFile;
    }
    cerr <<"oldFile size: " <<oldFile.size() <<endl;
    cerr <<"newFile size: " <<newFile.size() <<endl;
    //inserting into a hash table
    Node* hashTable[CONST_HASH_SIZE];
    for (int i = 0; i<CONST_HASH_SIZE; i++)
    {
        hashTable[i] = nullptr;
    }
    
    if (oldFile.length() <= CONST_SIZE) //if file is really short, just save it all into a single bucket
    {
        string temp = oldFile;
        int num = index(temp); //calculating where it should go in the hash table
        Node* p = new Node(temp);
        p->offsets.push_back(0);
        hashTable[num] = p; //setting a pointer to hashTable's pointer
    }
    if (oldFile.length() > CONST_SIZE)
    {
        for (int i = 0; i<oldFile.length()-CONST_SIZE; i++) //creating my hash table
        {
            string temp = oldFile.substr(i, CONST_SIZE); //takes a baby chunk out of old file and sets to temp
            int num = index(temp); //calculating where it should go in the hash table based off ascii
            bool fixed = false;
            if (hashTable[num] == nullptr)
            {
                Node *p = new Node(temp);
                p->offsets.push_back(i);
                hashTable[num] = p;
            }
            else //means collision
            {
                Node* t = hashTable[num];
                while (t->next!=nullptr)
                {
                    if (t->mChunk == temp)
                    {
                        t->offsets.push_back(i);
                        fixed = true;
                        break;
                    }
                    t = t->next;
                }
                if (!fixed)
                {
                    Node* newGuy = new Node(temp);
                    newGuy->offsets.push_back(i);
                    newGuy->next = nullptr;
                    t->next = newGuy;
                }
                fixed = false;
            }
        }
    }
    cerr<<"anything"<<endl;
//    for (int hasht = 0; hasht<CONST_HASH_SIZE; hasht++)
//    {
//        Node* tempnode = hashTable[hasht];
//        if (tempnode!=nullptr)
//        {
//            while (tempnode!=nullptr)
//            {
//                cerr<<"chunk: "<<tempnode->mChunk<<endl;
//                for (int o = 0; o<tempnode->offsets.size(); o++)
//                {
//                    cerr<<"offsets: "<<tempnode->offsets[o]<<endl;
//                }
//                tempnode = tempnode->next;
//            }
//        }
//    }
//    for (int hasht = 0; hasht<CONST_HASH_SIZE; hasht++)
//    {
//        if (hashTable[hasht]!=nullptr)
//        {
//            Node* tempnode = hashTable[hasht];
//            while (tempnode!=nullptr)
//            {
//                cerr<<"chunk: "<<tempnode->mChunk;
//            }
//            tempnode = tempnode->next;
//        }
//    }
    //cerr<<"made a hashtable"<<endl;
    //PROCESSING NEW STRING
    for (int i = 0; i<newFile.length(); i++) //take a chunk of the new string //TOOK OUT THE MINUS CONST SIZE
    {
       // cerr<<"INFINITE LOOP? "<<i<<endl ;
        vector<int>matches; //new vector each iteration
        string temp = newFile.substr(i, CONST_SIZE); //baby string of new string
        cerr << "Examining string at location i=" <<i<<endl;
//        if (temp[0] == '\n' || temp[0] == '\r')
//        {
//            continue; //increments i
//        }
        cerr<<"BABY NEW STRING: "<<temp<<endl;
        int location = index(temp); //hashing the baby new string
       // cerr<<"hash of baby string: "<<location<<endl;
        int quavo; //saving offset location, in old string, of the best match in a linkedlist
        int amtInCom = 0;
        bool LLnull = false;
        string str;
        bool perfect = false;
        bool foundMatch = false;
        //checking if there is a perfect match
        Node* p = hashTable[location]; //set an iterating pointer to head
        //this while loop is only searching for perfect matches
        //TRYING TO FIX THE BUG WHERE I DIDN'T ACCOUNT FOR AN EMPTY LINKED LIST//
        if (p == nullptr)
        {
            //IF IT IS NULLPOINTER THAT MEANS THAT THERE IS NO MATCH, SO JUST ADD,
            int lilpump = 0;
            int sharedAmt2 = 0;
            bool foundInAll2 = false;
            int matchLength2 = 0;
            int matchOffset2 = 0;
            if (matches.empty() && !foundMatch && !perfect)
            {
                for (int marvin = 0; marvin<CONST_HASH_SIZE; marvin++)
                {
                    Node* doug = hashTable[marvin];
                    while (doug!=nullptr)
                    {
                        if (doug!=nullptr&&sharedAmt2<matchingString(doug->mChunk, temp))
                        {
                            sharedAmt2 = matchingString(doug->mChunk, temp);
                            lilpump = doug->offsets[0];
                        }
                        doug = doug->next;
                    }
                }
                if (sharedAmt2>0)
                    foundInAll2 = true;
                
                if (foundInAll2 == true && sharedAmt2>5)
                {
                    matchLength2 = sharedAmt2;
                    matchOffset2 = lilpump;
                    str = "C" + to_string(sharedAmt2) + "," + to_string(lilpump);
                    diff.push_back(str);
                    cerr<<"1VVVPUSHING copy instruction into the vector: "<<str<<endl;
                    i = i+sharedAmt2-1;
                    continue;
                }
//                else if (foundInAll2 == true && sharedAmt2<=5)
//                {
//                    str = "A"+to_string(sharedAmt2)+":"+oldFile.substr(lilpump, sharedAmt2);
//                    diff.push_back(str);
//                    i = i+sharedAmt2-1;
//                    continue;
//                }
//
            }
        }
        
        
        //IF BABY STRING HASHES TO A LINKED LIST WITH NODES, THIS MEANS THERE ARE ~~~POTENTIAL~~~ MATCHES
        
        while (p!=nullptr) //iterate through the list
        {cerr<<"checking through linked list for matches"<<endl;
            if (p->mChunk == temp) //if exact match
            {cerr<<"if exact match"<< p->offsets[0] <<endl;
                perfect = true;
                quavo = p->offsets[0];
                amtInCom = CONST_SIZE;
                //cerr<<"before for"<<endl;
                for (int off = 0; off<p->offsets.size(); off++)
                {
                    //cerr<<"in for"<<endl;
                    matches.push_back(p->offsets.at(off));
                }
                //cerr<<"after for"<<endl;
                //if this is an exact match
                //search through the vector
                
            }//end of "if perfect match"
            p = p->next;
        }
        //compare vector
        int matchOffset=0;
        int matchLength=0;
        for (int v = 0; v<matches.size(); v++)//goes through vector
        {
            //while loop to figure out how much after the match chunk it matches
            //calculating how much it matches means probably taking a substring of its current offset+chunksize+i for old and for new;
            //string temp = newFile.substr(i+CONST_SIZE, 1); //incrementing offset to check how much of it matches
            //old string is the string located inside a node; so do oldFile.substr(p->offset + i, 1)
            //TO MAKE SURE SUBSTRING IS NOT GOING OUT OF BOUNDS -- MAKE CHECKS IN THE WHILE LOOP
            //(eye+CONST_SIZE+i<newFile.size() && matches[v]->offset+eye+CONST_SIZE<oldFile.size())
            //cerr <<"checking offset " <<matches.at(v) <<endl;
            if (v == 0) //if first iteration, set the compare values to the shabangs
            {
                
                cerr <<"First iteration" <<endl;
                int eye = 0;
                int matchCount = CONST_SIZE;
                while (eye+CONST_SIZE+i<newFile.size() && matches[v]+eye+CONST_SIZE<oldFile.size()
                       && newFile.substr(eye+CONST_SIZE+i, 1) == oldFile.substr(matches[v]+eye+CONST_SIZE, 1))
                {
                    matchCount++;
                    eye++;
                }
                
                matchOffset = matches[v];
                matchLength = matchCount;
            }
            else
            {
                int eye = 0;
                int matchCount = CONST_SIZE;
                while (eye+CONST_SIZE+i<newFile.size() && matches[v]+eye+CONST_SIZE<oldFile.size() &&
                       newFile.substr(eye+CONST_SIZE+i, 1) == oldFile.substr(matches[v]+eye+CONST_SIZE, 1))
                {
                    matchCount++;
                    eye++;
                }
                //cerr <<"Match count for offset " <<matches[v] <<" is " << matchCount <<endl;
                if (matchLength<matchCount)
                {
                    matchLength = matchCount;
                    matchOffset = matches[v];
                    cerr<<"matchlength: "<<matchLength<<endl;
                    cerr<<"match offset: "<<matchOffset<<endl;
                }
            }
        }//end of searching through vector for best match
        
        /////SEARCHING THORUGH ENTIRE HASH HERE
        /////////////////////////////////////////
        
        int lilxan = 0;
        int sharedAmt = 0;
        bool foundInAll = false;
        if (matches.empty() && !foundMatch && !perfect)
        {
            for (int marvin = 0; marvin<CONST_HASH_SIZE; marvin++)
            {
                Node* doug = hashTable[marvin];
                while (doug!=nullptr)
                {
                    if (doug!=nullptr&&sharedAmt<matchingString(doug->mChunk, temp))
                    {
                        sharedAmt = matchingString(doug->mChunk, temp);
                        lilxan = doug->offsets[0];
                    }
                    doug = doug->next;
                }
            }
            if (sharedAmt>0)
                foundInAll = true;
        
            if (foundInAll == true && sharedAmt>6)
            {matchLength = sharedAmt;
                matchOffset = lilxan;
                str = "C" + to_string(sharedAmt) + "," + to_string(lilxan);
                diff.push_back(str);
                cerr<<"1VVVPUSHING copy instruction into the vector: "<<str<<endl;
                i = i+sharedAmt-1;
                continue;
            }
        
        //checking if bestMatch can be merged
        if (!diff.empty())
        {cerr<<"if vector is not empty"<<endl;
            string merged = "";
            cerr<<"tentative copy length and offset: "<<matchLength<<","<<matchOffset<<endl;
            int a = diff.size() - 1;
            cerr<<"searching last block in vector"<<endl; //SHOULD NOT BE SEARCHING THROUGH WHOLE VECTOR
            //these vars are only used if we find a match within the vector
            string block = diff[a];
            //cerr<<"block: "<<block<<endl;
            string vOffset = "";
            string vLen = "";
            int finalLen;
            if (block[0] == 'C') //because it is within vector it should start at C
            {cerr<<"  if found copy instruction in the vector"<<endl;
                int j;
                int endOfSize;
                cerr<<"  searching for length of previous copy instruction"<<endl;
                for (j = 1; j<block.size(); j++)//searching for the offset value
                {
                    if (block[j] >= '0' && block[j]<='9')
                    {
                        vLen = vLen + block[j];
                    }
                    else if (block[j] == ',')
                    {
                        endOfSize = j;
                        break;
                    }
                    else //if there is no number and no comma
                        return; //this should cut out of the "if this is an instruction"
                }//end of for loop
                cerr<<"length: "<<vLen<<endl;
                //cerr<<"searching for offset of prev instruc"<<endl;
                for(int k = endOfSize+1; k<block.size(); k++)//initialize k to right after comma
                {cerr<<"k"<<k<<endl;
                    if (block[k]>='0'&&block[k]<='9')//if it is a number
                    {
                        //cerr<<"bolck k"<<block[k]<<endl;
                        //cerr<<"for loop finding offset value: "<<to_string(block[k])<<endl;
                        vOffset = vOffset + block[k];
                        
                    }
                    else
                        break; //stop copying into string that holds the vOffset
                }
                cerr<<"offset value: "<<vOffset<<endl;
                if (vLen == ""||vOffset == "")
                {
                    cerr<<"hit return"<<endl;
                    return; //if a value for length or offset is not recorded, get outta there!!
                }
                int intLen = stoi(vLen);
                int intOff = stoi(vOffset); //this is the offset of an instruction already in the vector
                //if the offset of an instruction, plus its size, is one less than your current offset, merge instructions
                cerr<<"temp quavo: "<<matchOffset<<"temp amtinCOm: "<<matchLength<<endl;
                if ((intOff + intLen) == matchOffset)//if the copy instruction can be merged SECOND REMOVE +1
                {
                    finalLen = intLen + matchLength;
                    merged = "C" + to_string(finalLen) + "," + vOffset;
                    foundMatch = true;
                    cerr<<"found match: "<<foundMatch<<endl;
                    cerr<<"if it can be merged"<<endl;
                    diff[a] = merged; //changing the string located inside of that vector spot
                    cerr<<"VVVMERGING at: "<<a<<"copy instruction: "<<diff[a]<<endl;
                }
            }//end of "if this is a copy instruction"
            //}//end of for loop
        }//end of searching through the vector (if statement) end of if not empty
        
        //NEW IF STATEMENT
        
        
                //IF IT IS NULLPOINTER THAT MEANS THAT THERE IS NO MATCH, SO JUST ADD,
                
                //////////////////////////////////////////////////////////////////////////
                ///BUT YOU SHOULD SEARCH THROUGH THE WHOLE HASHTABLE FOR THE BEST MATCH///
                //////////////////////////////////////////////////////////////////////////
                
                cerr<<"if linked list is empty so p==null"<<endl;
                amtInCom = 1;
                LLnull = true;
                if (!diff.empty())
                {
                    cerr<<"if vector is not empty"<<endl;
                    int end = diff.size() - 1;
                    string endChunk = diff[end];
                    string ogAdd = "";
                    int colon = -1;
                    string mergeAdd = "";
                    string newSize = "";//take into account how we are adding another char
                    cerr<<"last thing in vectpr"<<diff[end]<<endl;
                    if (endChunk[0] == 'A')//if last instruction was an add, search for a colon
                    {cerr<<"if last instr was add"<<endl;
                        for (int b = 1; b<endChunk.size(); b++)//look for what chars they had to add
                        {
                            if (endChunk[b] == ':')
                                colon = b;
                        }
                    }
                    if (endChunk[0] == 'A' && colon != -1)//if the last instruction was add and contains a colon
                    {//cerr<<"got the colon"<<endl;
                        for (int d = 1; d<colon; d++)
                        {
                            newSize = newSize + endChunk[d];
                        }
                        for (int c = colon+1; c<endChunk.size(); c++) //copy over all the og added chars
                        {
                            ogAdd = ogAdd + endChunk[c];
                        }
                        //ADD A PLUS ONE DO NOT FORGET//
                        int newIntSize = 1+stoi(newSize); //adding 1 to size of previous add instruction
                        //cerr<<"tracking"<<ogAdd<<endl;
                        mergeAdd = "A" + to_string(newIntSize) + ":" + ogAdd + temp.substr(0, 1);
                        diff[end] = mergeAdd;
                        cerr<<"VVVMERGING add instructions: "<<mergeAdd<<endl;
                    }
                    else //if last was not an add instruction, pushback
                    {
                        str = "A1:" + temp.substr(0, 1);
                        diff.push_back(str);
                        cerr<<"1VVVPUSHING back an add instruction: "<<str<<endl;
                    }
                }//end of "if not empty"
                else
                {
                    str = "A1:" + temp.substr(0, 1);
                    diff.push_back(str);
                    cerr<<"2VVVPUSHING back an add instruction: "<<str<<endl;
                }
                continue;
            }
        
        
        //GONNA ADD AN ELSE OVER HERE
        
        else if (foundMatch == false &&!perfect) //&&not perfect
        {
            cerr<<"if we didnt find a match or if the vector is empty"<<endl;
            str = "C" + to_string(matchLength) + "," + to_string(matchOffset);
            diff.push_back(str);
            cerr<<"1VVVPUSHING copy instruction into the vector: "<<str<<endl;
        }
        else if (diff.empty()&&!perfect)
        {
            cerr<<"if we didnt find a match or if the vector is empty"<<endl;
            str = "C" + to_string(matchLength) + "," + to_string(matchOffset);
            diff.push_back(str);
            cerr<<"2VVVPUSHING copy instruction into the vector: "<<str<<endl;
        }
        else
        {
            if (!foundMatch)
            {
                str = "C"+ to_string(matchLength) + "," + to_string(matchOffset);
                diff.push_back(str);
                //cerr<<"substr"<<oldFile.substr(matchOffset, matchLength) <<endl;
                cerr<<"3VVVPUSHING copy instruction into the vector: "<<str<<endl;
            }
        }
        i = i+matchLength-1;
        continue;
        //}
        
        //end of while loop for a NOT EMPTY linked list
        //there is not a direct match
        //so do a char by char search?//call a function that does the char by char searching
        //this part will compare which has the most char matches;//then it will record the offset of the old chunk that returns the most matches//and record the number of char matches
        //cerr<<"else";
        Node* q = hashTable[location]; //THIS IS REALLY SKETCHY// SHOULD MAKE AN IF STATEMENT FOR IF THERE IS NOTHING IN LIST
        if(q!=nullptr && !perfect)
        {cerr<<"if q!=null"<<endl;
            if (q->next == nullptr) //if only one node //BAD ACCESS??
            {
                cerr<<"if its next pointer is null"<<endl;
                for (int jack = 0; jack<q->offsets.size(); jack++)
                {
                    ///...CODE!?!????!?!!!??!?
                }
            }
            else
            {
                for (Node* p = hashTable[location]; p!=nullptr; p = p->next) //checking entire linked list, consecutive ones at a time
                {
                    if (p == hashTable[location]) //if first iteration
                    {
                        quavo = p->offsets.at(0);
                        amtInCom = matchingString(p->mChunk, temp);
                        cerr<<"p->chunk: "<<p->mChunk<<endl;
                        cerr<<"temp: "<<temp<<endl;
                        //cerr<<"2amtInCom: "<<amtInCom<<endl;
                    }
                    if (amtInCom<matchingString(p->mChunk, temp))//if second node's data is bigger
                    {
                        quavo = p->offsets.at(0);
                        amtInCom = matchingString(p->mChunk, temp);
                        //cerr<<"3amtInCom: "<<amtInCom<<endl;
                    }
                }
                cerr<<"compared with entire linked list"<<endl;
            }
        }
        //after picking best match
        if (amtInCom == 0 && !LLnull) //if nothing in common then do add instruction OR SEARCH THROUGH ENTIRE HASHTABLE
        {
            if (!diff.empty())
            {
                cerr<<"if vector is not empty"<<endl;
                int end = diff.size() - 1;
                string endChunk = diff[end];
                string ogAdd = "";
                int colon = -1;
                string mergeAdd = "";
                string newSize = "";//take into account how we are adding another char
                cerr<<"last thing in vectpr"<<diff[end]<<endl;
                if (endChunk[0] == 'A')//if last instruction was an add, search for a colon
                {cerr<<"if last instr was add"<<endl;
                    for (int b = 1; b<endChunk.size(); b++)//look for what chars they had to add
                    {
                        if (endChunk[b] == ':')
                            colon = b;
                    }
                }
                if (endChunk[0] == 'A' && colon != -1)//if the last instruction was add and contains a colon
                {//cerr<<"got the colon"<<endl;
                    for (int d = 1; d<colon; d++)
                    {
                        newSize = newSize + endChunk[d];
                    }
                    for (int c = colon+1; c<endChunk.size(); c++) //copy over all the og added chars
                    {
                        ogAdd = ogAdd + endChunk[c];
                    }
                    //ADD A PLUS ONE DO NOT FORGET//
                    int newIntSize = 1+stoi(newSize); //adding 1 to size of previous add instruction
                    //cerr<<"tracking"<<ogAdd<<endl;
                    mergeAdd = "A" + to_string(newIntSize) + ":" + ogAdd + temp.substr(0, 1);
                    diff[end] = mergeAdd;
                    //cerr<<"VVVMERGING add instructions: "<<mergeAdd<<endl;
                }
                else //if last was not an add instruction, pushback
                {
                    str = "A1:" + temp.substr(0, 1);
                    diff.push_back(str);
                   // cerr<<"VVVPUSHING back an add instruction: "<<str<<endl;
                }
            }//end of "if not empty"
            else
            {
                str = "A1:" + temp.substr(0, 1);
                diff.push_back(str);
               // cerr<<"VVVPUSHING back an add instruction: "<<str<<endl;
            }
            amtInCom = 1; //so for loop can increment
        }
        else if (amtInCom == 1 && !LLnull)//do add 1
        {cerr<<"if amout in common is 1"<<endl;
            if (!diff.empty())
            {
                cerr<<"if vector is not empty"<<endl;
                int end = diff.size() - 1;
                string endChunk = diff[end];
                string ogAdd = "";
                int colon = -1;
                string mergeAdd = "";
                string newSize = "";//take into account how we are adding another char
                if (endChunk[0] == 'A')//if last instruction was an add, search for a colon
                {
                    for (int b = 1; b<endChunk.size(); b++)//look for what chars they had to add
                    {
                        if (endChunk[b] == ':')
                            colon = b;
                    }
                }
                if (endChunk[0] == 'A' && colon != -1)//if the last instruction was add and contains a colon
                {
                    for (int d = 1; d<colon; d++)
                    {
                        newSize = newSize + endChunk[d];
                    }
                    for (int c = colon+1; c<endChunk.size(); c++) //copy over all the og added chars
                    {
                        ogAdd = ogAdd + endChunk[c];
                    }
                    
                    //ADD A PLUS ONE DO NOT FORGET//
                    int newIntSize = 1+stoi(newSize); //adding 1 to size of previous add instruction
                    //cerr<<"tracking"<<ogAdd<<endl;
                    mergeAdd = "A" + to_string(newIntSize) + ":" + ogAdd + oldFile.substr(quavo, amtInCom);
                    diff[end] = mergeAdd;
                    //cerr<<"VVVMERGING add instructions: "<<mergeAdd<<endl;
                }
                else //if last was not an add instruction, pushback
                {
                    str = "A1:" + oldFile.substr(quavo, amtInCom);
                    diff.push_back(str);
                    //cerr<<"VVVPUSHING back an add instruction: "<<str<<endl;
                }
            }//end of "if not empty"
            else
            {
                str = "A1:" + oldFile.substr(quavo, 1);
                diff.push_back(str);
                //cerr<<"VVVPUSHING back an add instruction: "<<str<<endl;
            }
        } //end of "if amtincom == 1"
        //else or a default;
        //if amtInCom > 1
   //     else if (amtInCom > 1 &&!perfect)
//        {
//            cerr<<"if more than one in common"<<endl;
//            //COPY AND PASTE THE MERGING SHABANG OVER HERE//
//            if (!diff.empty())
//            {cerr<<"if vector is not empty"<<endl;
//                string merged = "";
//                //                            for (int a = 0; a<diff.size(); a++)
//                //                            {
//                cerr<<"tentative copy length and offset: "<<amtInCom<<","<<quavo<<endl;
//                int a = diff.size() - 1;
//                cerr<<"searching last block in vector"<<endl; //SHOULD NOT BE SEARCHING THROUGH WHOLE VECTOR
//                //these vars are only used if we find a match within the vector
//                string block = diff[a];
//                //cerr<<"block: "<<block<<endl;
//                string vOffset = "";
//                string vLen = "";
//                int finalLen;
//                if (block[0] == 'C') //because it is within vector it should start at C
//                {cerr<<"  if found copy instruction in the vector"<<endl;
//                    int j;
//                    int endOfSize;
//                    cerr<<"  searching for length of previous copy instruction"<<endl;
//                    for (j = 1; j<block.size(); j++)//searching for the offset value
//                    {
//                        if (block[j] >= '0' && block[j]<='9')
//                        {
//                            vLen = vLen + block[j];
//                        }
//                        else if (block[j] == ',')
//                        {
//                            endOfSize = j;
//                            break;
//                        }
//                        else //if there is no number and no comma
//                            return; //this should cut out of the "if this is an instruction"
//                    }//end of for loop
//                    cerr<<"length: "<<vLen<<endl;
//                    //cerr<<"searching for offset of prev instruc"<<endl;
//                    for(int k = endOfSize+1; k<block.size(); k++)//initialize k to right after comma
//                    {cerr<<"k"<<k<<endl;
//                        if (block[k]>='0'&&block[k]<='9')//if it is a number
//                        {
//                            //cerr<<"bolck k"<<block[k]<<endl;
//                            //cerr<<"for loop finding offset value: "<<to_string(block[k])<<endl;
//                            vOffset = vOffset + block[k];
//
//                        }
//                        else
//                            break; //stop copying into string that holds the vOffset
//                    }
//                    cerr<<"offset value: "<<vOffset<<endl;
//                    if (vLen == ""||vOffset == "")
//                    {
//                        cerr<<"hit return"<<endl;
//                        return; //if a value for length or offset is not recorded, get outta there!!
//                    }
//                    int intLen = stoi(vLen);
//                    int intOff = stoi(vOffset); //this is the offset of an instruction already in the vector
//                    //if the offset of an instruction, plus its size, is one less than your current offset, merge instructions
//                    cerr<<"temp quavo: "<<quavo<<"temp amtinCOm: "<<amtInCom<<endl;
//                    if ((intOff + intLen) == quavo)//if the copy instruction can be merged SECOND REMOVE +1
//                    {
//                        finalLen = intLen + amtInCom;
//                        merged = "C" + to_string(finalLen) + "," + vOffset;
//                        foundMatch = true;
//                        cerr<<"found match: "<<foundMatch<<endl;
//                        cerr<<"if it can be merged"<<endl;
//                        diff[a] = merged; //changing the string located inside of that vector spot
//                        cerr<<"VVVMERGING at: "<<a<<"copy instruction: "<<diff[a]<<endl;
//                    }
//                }//end of "if this is a copy instruction"
//                //}//end of for loop
//            }//end of searching through the vector (if statement) end of if not empty
//            if (foundMatch == false &&!perfect) //&&not perfect
//            {
//                cerr<<"if we didnt find a match or if the vector is empty"<<endl;
//                str = "C" + to_string(amtInCom) + "," + to_string(quavo);
//                diff.push_back(str);
//                cerr<<"1VVVPUSHING copy instruction into the vector: "<<str<<endl;
//            }
//            else if (diff.empty()&&!perfect)
//            {
//                cerr<<"if we didnt find a match or if the vector is empty"<<endl;
//                str = "C" + to_string(amtInCom) + "," + to_string(quavo);
//                diff.push_back(str);
//                cerr<<"2VVVPUSHING copy instruction into the vector: "<<str<<endl;
//            }
//            else
//            {
//                if (!foundMatch)
//                {
//                    str = "C"+ to_string(amtInCom) + "," + to_string(quavo);
//                    diff.push_back(str);
//                    cerr<<"3VVVPUSHING copy instruction into the vector: "<<str<<endl;
//                }
//            }
//        } //end of else if amtinCom>1
        
        //            }
        //            else// if (amtInCom == 0) //if there is no match, which should not be possible
        //                    {
        //                        quavo = p->offset;
        //                        str = "A1:" + oldFile.substr(quavo, 1);
        //                        diff.push_back(str);
        //                    }
        //make a vector
        
        i = i + amtInCom - 1;
        cerr<<"got to end of for loop"<<endl;
    }//end of for loop that iterates through the new string
    
    cerr<<"fully exited for loop, so finished processing new file"<<endl;
    for (int i = 0; i<diff.size(); i++)
    {
        cerr<<i<<". diff chunk: "<<diff[i]<<endl;
        diffFile = diffFile + diff[i];
    }
    cerr<<"string--diff file: "<<diffFile<<endl;
    cerr <<"diff file size: " <<diffFile.size() <<endl;
    //////////////////////////DO THIS PART//////////////////////////
    //CONVERT STRING DIFFFILE INTO A REAL FILE OUT HERE FOR THE THIRD PARAMETER/////
    /////////////////////////DO THIS PART///////////////////////////
    
    //ofstream outfile("/Users/chelseyywang/Desktop/cs32proj4/cs32proj4/babydiff.txt");
    if ( ! fdiff )           // Did the creation fail?
    {
        cerr << "Error: Cannot create results.txt!" << endl;
        // ... return with failure ...
    }
   // fdiff.clear();
    //COMMENTING THIS OUT FOR A SECOND
    for(int zz = 0; zz<diffFile.size(); zz++){
        fdiff <<diffFile.at(zz);
        //cerr<<"for loop: "<<diffFile.at(zz)<<endl;
    }
    
 //  fdiff <<diffFile<< endl;
    
    //fdiff.clear();
//    fold.clear();
//    fnew.clear();void close();
}


int index(string baby)
{
    std::hash<string>hashFun;
    return hashFun(baby)%CONST_HASH_SIZE;
}


///////////////////////////////
///////// APPLY DIFF //////////
///////////////////////////////


bool getInt(istream& inf, int& n)
{
    char ch;
    if (!inf.get(ch)  ||  !isascii(ch)  ||  !isdigit(ch))
        return false;
    inf.unget();
    inf >> n;
    return true;
}



bool getCommand(istream& inf, char& cmd, int& length, int& offset)
{
    if (!inf.get(cmd))
    {
        cmd = 'x';  // signals end of file
        return true;
    }
    char ch;
    switch (cmd)
    {
        case 'A':
            return getInt(inf, length) && inf.get(ch) && ch == ':';
        case 'C':
            return getInt(inf, length) && inf.get(ch) && ch == ',' && getInt(inf, offset);
        case '\r':
        case '\n':
            return true;
    }
    return false;
}

// OG ONE THAT DOESN'T WORK
 
 
bool applyDiff(istream& fold, istream& fdiff, ostream& fnew)
{
    if (!fold||!fdiff)
        {
            cerr<<"yikes"<<endl;
            return 2;
        }
//    string oldFile = "";
//    string s;
//    while (getline(fold, s))
//    {
//        oldFile+=s;
//    }
   // fnew.clear();
    string oldFile = read(fold);
    char com = 'o';
    int len=0;
    int migos=0;
    cerr <<"hello" <<endl;
    while (com!='x')
    {
//        getCommand(fdiff, com, len, migos);
//        cerr<<"com: "<<com<<endl;
        if (!getCommand(fdiff, com, len, migos))
        {
            //cerr<<"false"<<endl;
            return false;
        }
        if (migos>oldFile.length()||migos<0)
        {
            return false;
        }
        if (len>oldFile.length()-migos)
        {
            return false;
        }
        cerr<<"hi"<<endl;
        
        if (com == 'A')
        {
            //do stuff with case A
            cerr<<"add"<<endl;
            //newFile = newFile + oldFile.substr(migos, len);
            while (len > 0)
            {
                fnew<<char(fdiff.get());
                len--;
            }
        }
        else if(com == 'C')
        {
            cerr<<"copy"<<endl;
            fnew<<oldFile.substr(migos, len);
        }
        cerr<<"wassup"<<endl;
    }
    return true;
}
