#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_set>

using namespace std;

vector<double> selectValuesByIdentifier(const vector<double>& mask, const vector<vector<double>>& matrix) {
    vector<double> selectedValues;

    // Convert mask to a set for efficient lookup
    unordered_set<double> maskSet(mask.begin(), mask.end());

    // Iterate through the matrix and select values with matching identifiers
    for (const auto& row : matrix) {
        if (row.size() >= 2 && maskSet.count(row[0]) > 0) {
            selectedValues.push_back(row[1]); // Select the value from the second column
        }
    }

    return selectedValues;
}
int main() {
    // Example matrix: first column is identifier, second column is the value
    vector<vector<double>> matrix = {
        {1.0, 10.0},
        {2.0, 20.0},
        {3.0, 30.0},
        {4.0, 40.0}
    };

    // Mask containing identifiers
    vector<double> mask = {3.0, 1.0, 5.0};

    // Get selected values
    vector<double> result = selectValuesByIdentifier(mask, matrix);

    // Output the result
    for (double val : result) {
        cout << val << endl;
    }

    return 0;
}