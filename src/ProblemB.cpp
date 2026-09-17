#include <bits/stdc++.h>
#include <string>
using namespace std;
using namespace chrono;

int t, nr, nc, r, c;
unordered_map<int, unordered_map<string, unsigned long>> hashMap;


bool isCorrect(vector<int> &lines, vector<int> &cols){
    
    if(nr > nc){
        for (int i = 0; i < nr; i++) {
            if(lines[i] != 0 || (i<nc && cols[i] != 0)){ return false; }
        }
    } else{
        for (int i = 0; i < nc; i++) {
            if(cols[i] != 0 || (i<nr && lines[i] != 0)){ return false; }
        }
    }

    return true;
}


string dpKey(vector<int> &cols){
    string key= "aaaaaaaa";

    for (int i = 0; i < nc; i++) {
        key[cols[i]]++;
    }

    return key;
}


unsigned long numberSolutions(vector<int> &lines, vector<int> &cols, int currentLine, int currentCol){
    bool dp= false;
    int depth;
    string key;

    //Checks - - -
    if((lines[currentLine] > ((nc-1) - currentCol))
    || (cols[currentCol] > ((nr-1) - currentLine))){
        //Not enough cols or lines to solve
        return 0;
    }

    //Matrix is correct
    if(currentLine == nr-1 && currentCol == nc-1 && isCorrect(lines, cols)){
        return 1;
    }

    //Matrix out of bounds
    if(lines[currentLine] < 0 || cols[currentCol] < 0){
        return 0;
    }

    //Lines/Cols - - -
    if(currentCol < nc-1){
        currentCol++;
    } else if(currentLine < nr-1){
        dp= true;
        currentCol= 0;
        currentLine++;

        //DP - - -
        key= dpKey(cols);
        depth= (nr-1) - currentLine;
        if(hashMap.find(depth) != hashMap.end()){
            if(hashMap[depth].find(key) != hashMap[depth].end()){
                return hashMap[depth][key];
            }
        }
    } else{ return 0; }

    //Recursion - - -
    //Put skull
    lines[currentLine]--;
    cols[currentCol]--;
    unsigned long yes_skull= numberSolutions(lines, cols, currentLine, currentCol);

    //Dont put skull
    lines[currentLine]++;
    cols[currentCol]++;
    unsigned long no_skull= numberSolutions(lines, cols, currentLine, currentCol);

    //Store and return value - - -
    if(dp){ hashMap[depth][key]= no_skull + yes_skull; }
    return no_skull + yes_skull;
}


int main(){

    cin >> t;
    while (t--){

        //Initialize vars
        cin >> nr >> nc;

        //Store number elements needed on rows/cols
        cin >> r >> c;
        vector<int> lines(nr, r);
        vector<int> cols(nc, c);

        //Start timer
        //auto start_time = high_resolution_clock::now();

        //Full matrix
        if(r == nc && c == nr){
            cout << "1\n";
        } else{
            //Calculate number solutions
            unsigned long result = numberSolutions(lines, cols, 0, -1);
            cout << result << "\n";
        }

        //Clear hashmap
        hashMap.clear();
    
        //auto end_time = high_resolution_clock::now();
        //auto duration = duration_cast<milliseconds>(end_time - start_time);
        //cout << "Time taken: " << duration.count() << " milliseconds" << endl;
    }

    return 0;
}