#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

class dataframe {
    
    private:
        std::vector<std::vector<double>> data;
        std::vector<int> row_indices;

    public: 
        //Constructor to initialize the Dataframe from a matrix 
        dataframe(std::vector<std::vector<double>>& matrix) {
            data = matrix;
            for (size_t i = 0; i< matrix.size(); ++i) {
                row_indices.push_back(static_cast<int>(i)); //Row indices start from 0
            }
        }
         // Overload operator[] to access rows
        std::vector<double>& operator[](int index) {
            if (index < 0 || index >= static_cast<int>(data.size())) {
                throw std::out_of_range("Row index out of range.");
            }
            return data[index];
        }

        // Overload operator[] for slicing
        dataframe operator[](const std::pair<int, int>& range) const {
            int start = range.first;
            int end = range.second;

            if (start < 0 || end >= static_cast<int>(data.size()) || start > end) {
                throw std::out_of_range("Invalid range for slicing.");
            }

            std::vector<std::vector<double>> sliced_data;
            for (int i = start; i <= end; ++i) {
                sliced_data.push_back(data[i]);
            }

            return dataframe(sliced_data);
        }

        // Method to print the DataFrame
        void print() const {
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
};

int main() {
    // Example matrix
    std::vector<std::vector<double>> matrix = {
        {1.1, 2.2, 3.3},
        {4.4, 5.5, 6.6},
        {7.7, 8.8, 9.9}
    };

    // Create DataFrame
    dataframe df(matrix);

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
    dataframe sliced_df = df[{0, 2}];
    std::cout << "\nSliced DataFrame (rows 0 to 1):\n";
    sliced_df.print();

    return 0;
}
