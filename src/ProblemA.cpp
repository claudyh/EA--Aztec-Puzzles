#include <bits/stdc++.h>
using namespace std;
//using namespace chrono;

int t, r, c, maxMoves, bestMoves, temp;


void printMatrix(const vector<vector<int>> &matrix){
    for (const auto &row : matrix){
        for (int value : row){
            cout << value << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}


bool isValidMatrix(const vector<vector<int>> &matrix){
    //Check if has the right numbers and same number occurences
    array<int, 5> countArray = {0};
    int row_index=1;

    //Store count of numbers on an array
    for (const auto &row : matrix){
        for (int value : row){
            if(value < 1 || value > r)
                return false;
            countArray[value - 1]++;
        }
        row_index++;
    }

    //Check if all have the same count
    for (int index = 0; index < countArray.size() - 1; index++){
        if (countArray[index] == 0 || countArray[index + 1] == 0)
            break;
        else if (countArray[index + 1] != countArray[index])
            return false;
    }

    return true;
}


int isDesiredMatrix(const vector<vector<int>> &matrix, int moves_left, int &row_init, int &row_end){
    vector<int> movesArray(r+1, 0);

    int valid = 1;
    for (int i = 0; i < r; ++i){
        for (int j = 0; j < c; ++j){
            if (matrix[i][j] != i + 1)
                valid = 0;

            //Improvment 4: if there's not enough moves to put number on right line, treasure is lost
            movesArray[matrix[i][j]] += abs(matrix[i][j] - (i+1));       
            if(movesArray[matrix[i][j]] > moves_left){
                return -1;
            }
        }
    }

    //Improvment 5: if a line is correct, lock the line
    if(!valid){
        //Top to bottom
        for (int k = 1; k < r; k++){
            if(movesArray[k] == 0){
                row_init++;
            } else{
                break;
            }
        }

        //Bottom to top
        for (int l = r; l > 0; l--){
            if(l == r && movesArray[l] == 0){
                row_end -= 2;
            } else if(l != r && movesArray[l] == 0){
                row_end--;
            } else{
                break;
            }
        }
    }

    return valid;
}


void rotateSquare(vector<vector<int>> &matrix, int row, int col, const int rotation){
    if (rotation == 0){ 
        //left
        temp = matrix[row][col];
        matrix[row][col] = matrix[row][col + 1];
        matrix[row][col + 1] = matrix[row + 1][col + 1];
        matrix[row + 1][col + 1] = matrix[row + 1][col];
        matrix[row + 1][col] = temp;
    } else{
        //right or double right
        for(int i = 0; i < rotation; i++){
            temp = matrix[row][col];
            matrix[row][col] = matrix[row + 1][col];
            matrix[row + 1][col] = matrix[row + 1][col + 1];
            matrix[row + 1][col + 1] = matrix[row][col + 1];
            matrix[row][col + 1] = temp;
        }
    }
}


int minMovesRecursive(vector<vector<int>> &matrix, int moves, pair<int,int> parent){
    int result = -1;
    int total_rotations= 0;
    int row_init=0, row_end= r;

    //Check if matrix is solved
    int desired= isDesiredMatrix(matrix, bestMoves-moves, row_init, row_end);
    if (desired == 1){
        //cout << "---------------------- isDesiredMatrix(matrix) ----------------------" << endl;
        //printMatrix(matrix);
        return moves;
    } else if(desired == -1){
        return -1;
    }

    //Check if current moves are bigger than best or max moves
    if (moves >= bestMoves){
        return -1;
    }

    if(row_end==r){
        row_end--;
    }
    //Go through the matrix
    for (int row = row_init; row < row_end; row++){
        for (int col = 0; col < matrix[0].size() - 1; col++){
            
            //Check if coordinates are diferent from parent
            if (row == parent.first && col == parent.second)
                continue;
            
            //Debug
            //cout << "\nSquare: " << row << ", " << col << " [" << moves << "]\n";
           
            //Improvment 2: if square has 4 equal numbers, dont do rotations
            if((matrix[row][col] == matrix[row + 1][col]) && (matrix[row][col] == matrix[row][col + 1]) && (matrix[row][col] == matrix[row + 1][col + 1]) && (matrix[row + 1][col] == matrix[row][col + 1]) && (matrix[row + 1][col] == matrix[row + 1][col + 1]) && (matrix[row][col + 1] == matrix[row + 1][col + 1])){
                total_rotations= 0;
            } //Improvment 3: if square has same diagonals, dont do right and double right
            else if((matrix[row][col] == matrix[row + 1][col + 1]) && (matrix[row][col + 1] == matrix[row + 1][col])){
                total_rotations= 1;
            } else {
                total_rotations= 3;
            }

            //Do all possible rotations
            for (int rotation = 0; rotation < total_rotations; rotation++){

                //Improvment 1: if desired matrix found, dont search more on that level
                if (bestMoves == moves + 1){
                    break;
                }
            
                //Rotate square
                rotateSquare(matrix, row, col, rotation);

                //Debug
                //cout << "Rotated " << ((rotation == 0) ? "Left" : (rotation == 1) ? "Right" : "Double Right") << ":" << endl;
                //printMatrix(matrix);

                //Recursive call (to traverse all squares)
                int rotatedMoves = minMovesRecursive(matrix, moves + ((rotation == 2) ? 2 : 1), make_pair(row, col));

                //Un-rotate square (to mantain original matrix)
                rotateSquare(matrix, row, col, (rotation == 0) ? 1 : (rotation == 1) ? 0 : 2);
                
                //Check if moves didnt reach best/max moves
                if (rotatedMoves != -1){
                    
                    //Store best moves value
                    result = (result == -1) ? rotatedMoves : min(result, rotatedMoves);
                    
                    if (rotatedMoves < bestMoves){
                        bestMoves = rotatedMoves;
                        //cout << "Best Moves so far: " << bestMoves << endl;
                    }
                }
            }
        }
    }
    return result;
}


int main(){

    cin >> t;
    while (t--){

        //Initialize vars
        cin >> r >> c >> maxMoves;
        bestMoves = maxMoves;
        vector<vector<int>> matrix(r, vector<int>(c));

        //Store values on matrix
        for (int i = 0; i < r; ++i){
            for (int j = 0; j < c; ++j){
                cin >> matrix[i][j];
            }
        }

        //Start timer
        //auto start_time = high_resolution_clock::now();

        //Check if matrix is valid
        if (!isValidMatrix(matrix)){
            
            cout << "the treasure is lost!\n";

        } else{
            
            //Check if matrix already solved
            int ri=0, re=0;
            int desired= isDesiredMatrix(matrix, maxMoves, ri, re);
            if (desired == 1){
                cout << 0 << endl;
                continue;
            }

            //Check if we have moves
            if (maxMoves == 0){
                cout << "the treasure is lost!\n";
                continue;
            }

            //Calculate best moves
            int result = minMovesRecursive(matrix, 0, make_pair(-1, -1));

            if (result != -1)
                cout << result << endl;
            else
                cout << "the treasure is lost!\n";
        }
    
    //auto end_time = high_resolution_clock::now();
    //auto duration = duration_cast<milliseconds>(end_time - start_time);
    //cout << "Time taken: " << duration.count() << " milliseconds" << endl;
    
    }

    return 0;
}
