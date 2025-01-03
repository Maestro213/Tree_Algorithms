#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <cmath>

using namespace std;

void sortSample(vector<double>& inputVector){
    // sort std vector
    return sort(inputVector.begin(),inputVector.end());
};

// A helper function to print the matrix
void printMatrix(const vector<vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (auto val : row) {
            cout << val << "  ";
        }
        cout << "\n";
    }
    cout << endl;
}

void printVector(const vector<double>& vector) {
    for (const auto& val : vector) {
            cout << val << "  ";
        }
        cout << "\n"<<endl;
}

vector<double> columnSelector(vector<vector<double>>& matrix, int col) {
    vector<double> columnA;
        for(int i = 0; i < matrix.size();i++) { 
            columnA.push_back(matrix[i][col]);
        };
    return columnA;
}


vector<vector<double>> FilterMask(
    const vector<double>& mask, const vector<vector<double>>& matrix) {
    vector<vector<double>> selectedRows;

    // Convert mask to a set for efficient lookup
    unordered_set<double> maskSet(mask.begin(), mask.end());

    // Iterate through the matrix and select rows with matching identifiers
    for (const auto& row : matrix) {
        if (row.size() >= 2 && maskSet.count(row[0]) > 0) {
            selectedRows.push_back(row); // Add identifier and value as a row
        }
    }

    return selectedRows;
}


// Sort matrix by one column
void sortRows(vector<vector<double>>& matrix, int col = 1) {    
    sort(matrix.begin(),
              matrix.end(),
              [col](const vector<double>& lhs, const std::vector<double>& rhs) {
                  return lhs[col] < rhs[col];
              });
}

//Calculate Entropy 
double entropy(vector<double>& p_vals) {

    //Calculating entropy with standard formula sum_i {-p_i * log_2 p_i}
    double Entropy = accumulate(p_vals.begin(), p_vals.end(), 0.0, 
                                [](double sum, double p){
                                    return sum - p * log2(p);
                                    }
                                );
    return Entropy;
};

//Split by median
pair<vector<vector<double>>,vector<vector<double>>> splitbyMedian(vector<vector<double>>& sample, int colSplit = 1) {

    int n_obs = sample.size();
    vector<double> v = columnSelector(sample,1);

    sortRows(sample, colSplit);
    int split_idx = 1;
    while (v[split_idx]==v[split_idx-1]){
        split_idx++; 
    }
    
    //Calculating entropy with standard formula sum_i {-p_i * log_2 p_i}
    
    vector<vector<double>> subSampleA(sample.begin(), sample.begin() + split_idx);
    vector<vector<double>> subSampleB(sample.begin() + split_idx, sample.end());
    sortRows(subSampleA, 0);
    sortRows(subSampleB, 0);
    return {subSampleA,subSampleB};
};



vector<double> countFreq(vector<vector<double>>& sample, int col = 1) {
    // Extract the specified column
    vector<double> v = columnSelector(sample, 1); 
    int n_obs = v.size();
    if (n_obs == 0) return {}; // Return empty vector if no observations

    // Sort the column to group identical elements
    sort(v.begin(), v.end());

    // Vector to store frequencies of unique elements
    vector<double> unique_freq;
   
    // Initialize counters
    int count = 1;
    
    // Traverse the sorted vector and count frequencies
    for (int i = 1; i < n_obs; ++i) {
       
        if (v[i] == v[i - 1]) {
            // Increment count if the current element is the same as the previous
            ++count;
            
        } else {
            // Store the count of the previous unique element
            unique_freq.push_back(count);
            // Reset count for the new unique element
            count = 1;
        
        }
        
    }
    // Store the count for the last unique element
    unique_freq.push_back(count);
    transform(unique_freq.begin(),unique_freq.end(), unique_freq.begin(), [n_obs](double val) {return val/n_obs;});
    return unique_freq;

};

double informationGain(vector<vector<double>>& sampleX, vector<vector<double>>& sampleY){
    double ig;
    double total = sampleX.size(); 
    auto [leftSet, rightSet] = splitbyMedian(sampleX, 2);
    vector<vector<double>> leftSampleY, rightSampleY;

    leftSampleY = FilterMask(columnSelector(leftSet,0), sampleY);
    rightSampleY = FilterMask(columnSelector(rightSet,0), sampleY);
    double n_left = leftSampleY.size();

    vector<double> leftp = countFreq(leftSampleY);
    vector<double> rightp = countFreq(rightSampleY);
    double leftEntropy = entropy(leftp);double rightEntropy = entropy(rightp);
    cout << "left Entropy = "<< leftEntropy <<" right Entropy = " << rightEntropy <<endl;
    ig = 1 - n_left/total*leftEntropy - (1-n_left/total)*rightEntropy;

    return ig;
};




int main() {

    vector<double> v = {1, 4, 3, 2, 5, 5, 0};
    vector<double> p_vals = {0.1, 0.2, 0.3, 0.1}; 
    // Sort vector in ascending order
    cout <<"\n Entropy = " << p_vals.size() <<endl;
    vector<double> subSampleA, subSampleB;
    cout <<  "\n";


    vector<std::vector<double>> matrixX = {
        {0.0,  7.2, 0.001},
        {1.0,  5.4, 0.001},
        {2.0,  2.2, 0.01},
        {3.0,  5.5, 0.001},

    };

    vector<std::vector<double>> matrixY = {
        {0.0, 0.0},
        {1.0, 0.0},
        {2.0, 1.0},
        {3.0, 1.0},

    };
    
    // Split by the median of the 2nd column (index = 1)
    auto [leftSet, rightSet] = splitbyMedian(matrixX, 2);

    // Print results
    std::cout << "Left subset:\n";
    printMatrix(leftSet);

    std::cout << "\nRight subset:\n";
    printMatrix(rightSet); 
    std::cout << "\nRight subset idx column:\n";
    vector<double> idx = columnSelector(rightSet,0);
    
    printVector(idx);
    vector<vector<double>> subSetAMatrixY;
    subSetAMatrixY = FilterMask(idx, matrixY);
    cout << "\nY subset:\n";

    printMatrix(subSetAMatrixY);
    printVector(countFreq(subSetAMatrixY));

    cout<< "informationGain: \n";
    cout<<informationGain(matrixX,matrixY);

    return 0;

};