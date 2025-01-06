#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // For pretty printing
#include <functional> // For std::function
#include <cmath> // For mathematical functions like exp, sin

class DataFrame {
private:
    std::vector<std::vector<double>> data;
    std::vector<int> row_indices;
    std::vector<std::string> col_indices;

public:
    // Constructor to initialize the DataFrame from a matrix
    DataFrame(const std::vector<std::vector<double>>& matrix) {
        data = matrix;
        for (size_t i = 0; i < matrix.size(); ++i) {
            row_indices.push_back(static_cast<int>(i)); // Row indices start from 0
        }
        for (size_t i = 0; i < matrix[0].size(); ++i) {
            col_indices.push_back("Col" + std::to_string(i)); // Column indices start from 0
        }
    }

    // Overload operator[] to access rows
    std::vector<double>& operator[](int index) {
        if (index < 0 || index >= static_cast<int>(data.size())) {
            std::cerr << "Accessing row index " << index << " out of range." << std::endl;
            throw std::out_of_range("Row index out of range.");
        }
        return data[index];
    }


    // Overload operator[] for slicing
    DataFrame operator[](const std::pair<int, int>& range) const {
        int start = range.first;
        int end = range.second;

        if (start < 0 || end >= static_cast<int>(data.size()) || start > end) {
            std::cerr << "Invalid slice range: (" << start << ", " << end << ")." << std::endl;
            throw std::out_of_range("Invalid range for slicing.");
        }

        std::vector<std::vector<double>> sliced_data;
        for (int i = start; i <= end; ++i) {
            sliced_data.push_back(data[i]);
        }

        return DataFrame(sliced_data);
    }

    // Method to print the DataFrame
    void print() const {
        if (data.empty()) {
            std::cerr << "DataFrame is empty." << std::endl;
            return;
        }
        std::cout << "RowIndex";
        for (size_t i = 0; i < data[0].size(); ++i) {
            std::cout << "\tCol" << i;
        }
        std::cout << "\n";

        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << row_indices[i];
            for (size_t j = 0; j < data[i].size(); ++j) {
                std::cout << "\t" << std::fixed << std::setprecision(2) << data[i][j];
            }
            std::cout << "\n";
        }
    }

    // Getter for data
    const std::vector<std::vector<double>>& get_data() const {
        return data;
    }

    // Getter for row indices
    const std::vector<int>& get_row_indices() const {
        return row_indices;
    }

    // Method to get a specific row
    std::vector<double> get_row(int index) const {
        if (index < 0 || index >= static_cast<int>(data.size())) {
            throw std::out_of_range("Row index out of range.");
        }
        return data[index];
    }

    // Method to add a new row
    void add_row(const std::vector<double>& new_row) {
        if (!data.empty() && new_row.size() != data[0].size()) {
            throw std::invalid_argument("New row must have the same number of columns as the DataFrame.");
        }
        data.push_back(new_row);
        row_indices.push_back(static_cast<int>(row_indices.size()));
    }

    // Method to apply a function to the entire DataFrame
    void apply(const std::function<double(double)>& func) {
        if (data.empty()) {
            std::cerr << "Cannot apply function: DataFrame is empty." << std::endl;
            return;
        }
        for (auto& row : data) {
            std::transform(row.begin(), row.end(), row.begin(), func);
        }
    }
};

int main() {
    // Example matrix
    std::vector<std::vector<double>> matrix = {
        {1.1, 2.2, 3.3},
        {4.4, 5.5, 6.6},
        {7.7, 8.8, 9.9}
    };

    // Create DataFrame
    DataFrame df(matrix);

    // Print DataFrame
    df.print();

    // Modify data using operator[]
    df[1][1] = 42.42;

    std::cout << "\nAfter modifying df[1][1]:\n";
    df.print();

    // Add a new row
    std::vector<double> new_row = {10.0, 11.1, 12.2};
    df.add_row(new_row);

    std::cout << "\nAfter adding a new row:\n";
    df.print();

    // Slice the DataFrame
    DataFrame sliced_df = df[{0, 1}];
    std::cout << "\nSliced DataFrame (rows 0 to 1):\n";
    sliced_df.print();

    // Apply a function (e.g., exponential) to the DataFrame
    df.apply([](double x) { return std::exp(x); });

    std::cout << "\nAfter applying exp() to the entire DataFrame:\n";
    df.print();

    return 0;
}
