#include <omp.h>
#include <string>
#include <vector>
#include <iostream> 
#include "MatrixParallel.cpp"

using namespace std;

bool validDouble(string input);
bool validInt(string input);

int main()
{
	srand(static_cast<int>(time(0)));
	string input;

	//Each block validates the user entered size for an array
	do
	{
		cout << "Enter the size for n: ";
		cin >> input;
		cout << endl;
	} while (!validInt(input));
	int n = stoi(input);

	do
	{
		cout << "Enter the size for k: ";
		cin >> input;
		cout << endl;
	} while (!validInt(input));
	int k = stoi(input);

	do
	{
		cout << "Enter the size for m: ";
		cin >> input;
		cout << endl;
	} while (!validInt(input));
	int m = stoi(input);

	//Asks the user if they want to auto populate matrices
	while (true)
	{
		cout << "Would you like to auto populate the matrices with random doubles? (y/n) ";
		cin >> input;
		if (input == "y" || input == "n")
		{
			break;
		}
	}

	MatrixParallel matrix1(n, k);
	MatrixParallel matrix2(k, m);

	if (input == "n")
	{
		cout << "Enter all lines of the first matrix separated by new lines. " << k << " values per line.\n";
		for (int i = 0; i < n; i++)
		{
			for (double& j : matrix1[i])
			{
				do
				{
					cin >> input;
					if (!validDouble(input))
					{
						cout << "Try that one again. Make sure you are entering decimals or integers.\nr";
					}
				} while (!validDouble(input));
				j = stoi(input);
			}
		}

		cout << "Enter all lines of the second matrix separated by new lines. " << m << " values per line.\n";
		for (int i = 0; i < k; i++)
		{
			for (double& j : matrix2[i])
			{
				do
				{
					cin >> input;
					if (!validDouble(input))
					{
						cout << "Try that one again. Make sure you are entering decimals or integers.\n";
					}
				} while (!validDouble(input));
				j = stoi(input);
			}
		}
	}
	else
	{
		matrix1.populateRand();
		matrix2.populateRand();
	}

	MatrixParallel productMatrix = MatrixParallel(n, m);
	
	//Perform the multiplication with differing numbers of threads
	for (int i = 1; i <= 12; i++)
	{
		double start = omp_get_wtime();
		productMatrix.SetNumThreads(i);
		productMatrix = matrix1 * matrix2;
		double end = omp_get_wtime();

		cout << "\nUsing " << i << " thread(s): " << end - start << " Seconds" << endl;
	}

	//cout << "Matrix 1\n";
	//matrix1.printMatrix();

	//cout << "Matrix 2\n";
	//matrix2.printMatrix();

	//cout << "Product Matrix\n";
	//productMatrix.printMatrix();

	return 0;
}

static bool validDouble(string input)
{
	double x;
	try
	{
		x = stod(input);
		return 1;
	}
	catch (exception)
	{
		return 0;
	}
}

static bool validInt(string input)
{
	bool validInt = false;
	for (char x : input)
	{
		if (!isdigit(x))
		{
			validInt = 0;
			break;
		}
		else
		{
			validInt = 1;
		}
	}

	return validInt;
}