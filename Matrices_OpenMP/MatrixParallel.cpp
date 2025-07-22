#include <iostream>
#include <vector>
#include <string>
#include <omp.h>

using namespace std;

class MatrixParallel
{
public:	
	MatrixParallel(int x, int y)
	{
		matrix = vector<vector<double>>(x, vector<double>(y));
	}

	void printMatrix()
	{
		//Prints each value in the matrix
		for (vector<double> i : this->matrix)
		{
			for (double j : i)
			{
				cout << j << " ";
			}
			cout << endl;
		}
	}

	void populateRand()
	{		
		//Populates the matrix's matrix with random integers from -1000 to 1000
		for (vector<double>& i : this->matrix)
		{
			for (double& j : i)
			{
				j = rand() % 2001 - 1000;
			}
		}
	}

	void SetNumThreads(int numThreads)
	{
		//Used for evaluating the most optimal thread count for multiplication
		omp_set_num_threads(numThreads);
	}

    MatrixParallel operator*(MatrixParallel matrix2)
    {
		if (this->matrix[0].size() == matrix2.matrix.size())
		{
			MatrixParallel productMatrix(this->matrix.size(), matrix2.matrix[0].size());

			//Iterate through each element of the created productMatrix and calculate their value. The loop is split evenly between each thread at compile time
			int total = this->matrix.size() * matrix2.matrix[0].size();
			#pragma omp parallel for collapse(2)
			for (int idx = 0; idx < total; idx++) 
			{
				//Calculate x and y pos of the given index
				int x = idx / matrix2.matrix[0].size();
				int y = idx % matrix2.matrix[0].size();

				//Calculate the value based on matrix multiplication rules
				double indexVal = 0;
				for (int k = 0; k < this->matrix[0].size(); k++)
				{
					indexVal += this->matrix[x][k] * matrix2.matrix[k][y];
				}

				//Set the value after the for loop to potentially reduce false sharing
				productMatrix[x][y] = indexVal;
			}

			return productMatrix;
		}
		else
		{
			cout << "Cannot multiply different sized matrices.\n";
		}
    }

	vector<double>& operator[] (int index)
	{
		if (index < 0 || index >= this->matrix.size()) {
			throw std::out_of_range("Index out of bounds");
		}

		return this->matrix[index];
	}

private:
	vector<vector<double>> matrix;
};