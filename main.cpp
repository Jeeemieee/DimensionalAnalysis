#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

std::vector<std::string>StringToVector(std::string &string,char delimiter);
void FunctionDisplay(std::vector<std::string> &vector, int N);
int DataUnit(std::string a);
std::vector<std::string> NamesInput(int N);
int getIndex (std::vector<int> v, int a);
std::vector<int> RepeatingUnits(int StartI, int EndI, std::vector<std::vector<int>> &Matrix);
int MinVectorIndex (std::vector<int> vector);
int Nonzero (std::vector<int> vector);
std::vector<std::vector<int>> TransposeMatrix (std::vector<std::vector<int>> &Matrix);
std::pair<std::vector<std::vector<double>>, std::vector<std::string>> rowSwitch (std::vector<std::vector<double>> &Matrix, std::vector<std::string> &Vector, int i);
std::vector<std::vector<double>> ConvertToDoubleMatrix (std::vector<std::vector<int>> &Matrix);
std::vector<double> vectorDivision (std::vector<double> &vector, double coefficient);
std::vector<std::vector<double>> rowAddition (std::vector<std::vector<double>> &Matrix, int i);
std::pair<std::vector<std::vector<double>>, std::vector<std::string>> GaussianElimination (std::vector<std::vector<double>> &Matrix, std::vector<std::string> &Vector);
std::vector<std::vector<double>> addSolution (std::vector<std::vector<double>> &Matrix,std::vector<double> &Vector);
std::string interpretSolution (std::vector<std::vector<double>> &Matrix,std::vector<std::string> &Vector);
void displayPI (std::vector<std::string> &PIvector,std::vector<std::string> &solutionNames);
std::string reduceStringZeros (std::string &string);

/*
 * This code can be used to find dimensionless groups
 * For questions you can mail me on jaimy.g@hotmail.com
 */

int main() {
    //Preliminary code information for to user
    std::cout << "This script can calculate dimensionless parameters based on an inputted solution variable and a function of physical variables (e.g. solution variable = f(var1, var2, var3, ...)).\n";
    std::cout << "Possible unit input consists of SI-units and derived units such as; N, J, W, Pa, V.\n";

    //Number of variables input
    std::cout << "How many variables does the function consist off? ";
    int NumberVariables{};
    std::cin >> NumberVariables;
    if (NumberVariables<1)// Error message variables input
    {
        std::cout << "ERROR: The number of variables must be an positive integer of 1 or higher";
        std::exit(-1);
    }

    //Variable name input
    std::vector<std::string> Names{NamesInput(NumberVariables)};
    std::cout << "\nThe function that will be solved is:\n";
    FunctionDisplay(Names,NumberVariables);

    //Variable unit input
    std::cout << "\nContinue with the input of the units for all variables below.\n";
    std::cout << "Navigating through the unit input can be done using the keywords Previous and Next to retry the input or skip the inputted variable.\n";
    std::vector<std::vector<int>> Data(NumberVariables+1,std::vector<int>(7,0)); //kg,m,s,K,mol,A,cd
    for (int i{0};i<NumberVariables+1;i++)
    {
        std::cout << "Input the units for " << Names[i] << " as a function (e.g. kg^1*m^1*s^-2): ";
        std::string function;
        std::cin >> function;

        //Preliminary check if input is correct
        int NumberUnits{0};
        //Skip preliminary check if input is Previous or Next
        if(function == "Previous" || function == "Next")
        {
            function +=  "^";
        }
        for (int j{0}; (j = function.find('^',j))!=std::string::npos; j++)
        {
            NumberUnits++;
        }
        if (NumberUnits<1)
        {
            std::cout << "WARNING: The function input for " << Names[i] << " is incorrect!\n";
            std::cout << "Please retry the input of variable " << Names[i] << std::endl;
            i--;
            continue;
        }

        //Separate all units into a vector of units
        std::vector<std::string> Units(StringToVector(function,'*'));

        //Separate the vector entries and input all data in the Data vector
        for (int j{0}; j<Units.size();j++)
        {
            //Unit definition
            std::stringstream UnitEntry(Units[j]);
            std::string Unit;
            std::getline(UnitEntry,Unit,'^');

            //Power definition and conversion from string to int
            std::stringstream SSpower;
            SSpower << Units[j].substr(Units[j].find('^')+1);
            std::string Spower = Units[j].substr(Units[j].find('^')+1);
            int Power{};
            SSpower >> Power;

            //Power data saved to matrix based on the inputted units
            if ((Unit=="kg" || Unit=="m" || Unit=="s" || Unit=="K" || Unit=="mol" || Unit=="A" || Unit=="cd") && Power!=0 && Spower.empty()==0)
            {//SI-units
                Data[i][DataUnit(Unit)] += Power;
            }
            /*Several derived units can be found below, this can be extended by creating a new else if term similar to this example.
             * The order of the units does not matter
             * The number of the units does not matter, when using more than 3 copy the input methodology of <UNIT 2>/<UNIT 3>
             * Only replace the terms with <angled brackets>
             * Do not forget to add the symbol to line 35 :)

             else if (Unit == "<DERIVED UNIT SYMBOL>" && Power!=0 && Spower.empty()==0)
            {//<DERIVED UNIT NAME>
                Units[j] = "<UNIT 1>^"+std::to_string((Power * <POWER OF UNIT 1>));
                Units.push_back("<UNIT 2>^"+std::to_string((Power * <POWER OF UNIT 2>)));
                Units.push_back("<UNIT 3>^"+std::to_string((Power * <POWER OF UNIT 3>)));
                j--;
            }*/
            else if (Unit == "N" && Power!=0 && Spower.empty()==0)
            {//Newton
                Units[j] = "kg^"+std::to_string((Power * 1));
                Units.push_back("m^"+std::to_string((Power * 1)));
                Units.push_back("s^"+std::to_string((Power * -2)));
                j--;
            }
            else if (Unit == "J" && Power!=0 && Spower.empty()==0)
            {//Joule
                Units[j] = "kg^"+std::to_string((Power * 1));
                Units.push_back("m^"+std::to_string((Power * 2)));
                Units.push_back("s^"+std::to_string((Power * -2)));
                j--;
            }
            else if (Unit == "W" && Power!=0 && Spower.empty()==0)
            {//Watt
                Units[j] = "kg^"+std::to_string((Power * 1));
                Units.push_back("m^"+std::to_string((Power * 2)));
                Units.push_back("s^"+std::to_string((Power * -3)));
                j--;
            }
            else if (Unit == "Pa" && Power!=0 && Spower.empty()==0)
            {//Pascal
                Units[j] = "kg^"+std::to_string((Power * 1));
                Units.push_back("m^"+std::to_string((Power * -1)));
                Units.push_back("s^"+std::to_string((Power * -2)));
                j--;
            }
            else if (Unit == "V" && Power!=0 && Spower.empty()==0)
            {//Volt
                Units[j] = "kg^"+std::to_string((Power * 1));
                Units.push_back("m^"+std::to_string((Power * 2)));
                Units.push_back("s^"+std::to_string((Power * -3)));
                Units.push_back("A^"+std::to_string((Power * -1)));
                j--;
            }
            else if (Unit == "Previous" && i>0)
            {
                i--;
                Data[i] = std::vector<int>(7,0);
                i--;
                break;
            }
            else if (Unit == "Previous" && i<=0)
            {
                i--;
                Data[i] = std::vector<int>(7,0);
                break;
            }
            else if (Unit == "Next")
            {
                break;
            }
            else //Error for incorrect definition units.
            {
                std::cout << "WARNING: Component " << (j+1) << " of variable " << Names[i] << " is not in the correctly defined.\n";
                std::cout << "Please retry the input of variable " << Names[i] << std::endl;
                i--;
                break;
            }
        }
    }

    //Check the amount of times every unit is used
    std::vector<int> repeatingUnits = RepeatingUnits(0,NumberVariables+1,Data);
    if(NumberVariables+1 <= Nonzero(repeatingUnits))
    {
        //check if the number of variables is indeed larger than the number of units. If not PI = 0 and thus no solution possible
        std::cout << "\nNot enough variables are left to get a solution, re-examine the inputted variable units.";
        std::exit(-1);
    }

    std::vector<std::string> UnitData = {"kg","m","s","K","mol","A","cd"}; //A vector with SI-units
    //If a unit is only used once delete the variable that uses only that unit.
    while (getIndex(repeatingUnits,1)!=-1)
    {
        int Index{getIndex(repeatingUnits,1)};
        for (int i{0}; i < NumberVariables + 1; i++) {
            //Output error code when the solution variable is dependent on a unit not existing in the other variables
            if (Data[i][Index] != 0 && i == 0)
            {
                std::cout << "The solution variable, " << Names[0] << ", is dependent on a unit not existing in the function of variables. Therefore no solution is possible.";
                exit(-1);
            }
            //If a variable is the only variable with a certain unit, delete it so the calculation can continue.
            else if (Data[i][Index] != 0)
            {
                std::cout << "The unit " << UnitData[Index] << " is only present in variable "
                          << Names[i] << " and will therefore be removed.";
                Data.erase(Data.begin() + i);
                Names.erase(Names.begin() + i);
                NumberVariables--;
                repeatingUnits = RepeatingUnits(0,NumberVariables+1,Data);
            }
        }
        //show the actual function that will be solved when variables are deleted out
        if(getIndex(repeatingUnits,1)==-1 && NumberVariables+1> Nonzero(RepeatingUnits(0,NumberVariables+1,Data)))
        {
            std::cout << "\nThe remaining function that will be solved will therefore be: ";
            FunctionDisplay(Names,NumberVariables);
        }
        //Show error if not enough variables exist to get a proper solution
        else
        {
            std::cout << "\nNot enough variables are left to get a solution, re-examine the inputted variable units.";
            std::exit(-1);
        }
    }

    std::vector<int> repeatingUnitsVariables = RepeatingUnits(1,NumberVariables+1,Data);
    std::vector<std::string> variableNames; //Contains the names of the repeating variables
    std::vector<std::vector<int>> variableData; //Contains the data of the repeating variables

    //Find the variables that will be used as repeating variables and put them in their own matrix
    while(MinVectorIndex(repeatingUnitsVariables)!=-1)
    {
        int Index{MinVectorIndex(repeatingUnitsVariables)};
        for(int i{1}; i<NumberVariables+1;i++)
        {
            if(Data[i][Index] !=0)
            {
                variableData.push_back(Data[i]);
                variableNames.push_back(Names[i]);
                Data.erase(Data.begin() + i);
                Names.erase(Names.begin() + i);
                repeatingUnitsVariables[Index] = 0;
                NumberVariables--;
                break;
            }
        }
    }

    //Prepare a system of equations that can be solved using Gaussian elimination
    std::vector<std::vector<int>> variableDataTransposed{TransposeMatrix(variableData)};
    std::vector<std::vector<double>>systemEquations;
    std::vector<std::vector<double>>solutionData;
    solutionData = ConvertToDoubleMatrix(Data);

    //Solving the system of equations and preparing the solution for output
    std::vector<std::string> PI;
    for (int i{0};i<solutionData.size();i++)
    {
        systemEquations = ConvertToDoubleMatrix(variableDataTransposed);
        addSolution(systemEquations,solutionData[i]);
        GaussianElimination(systemEquations,UnitData);
        PI.push_back(interpretSolution(systemEquations,variableNames));
    }
    std::cout<<std::endl;

    //Output the final solution(s)
    std::cout << "The final solution is:\n";
    displayPI(PI,Names);

    std::cout << "\n" << "\nNote: the solution might seem complex or with unexpected independent variables.\n";
    std::cout << "If so, please retry with a different  order of variables. ";
    std::cout << "The variables inputted first tend to be used as independent variables over variables inputted last.";

    return 0;
}
int getIndex (std::vector<int> v, int a)
{//Find the vector entry that has the value of int a
    auto itr = std::find(v.begin(), v.end(), a);
    if (itr != v.cend())
    {
        return std::distance(v.begin(), itr);
    }
    else
    {
        return -1;
    }
}
std::vector<std::string> NamesInput(int N)
{//Make a vector of variable names inputted by the user
    std::vector<std::string> v(N+1);
    for (int i{0}; i<N+1;i++)
    {
        std::string input;
        if(i == 0)
        {
            std::cout << "Input the solution variable name: ";
            std::cin >> input;
        }
        else
        {
            std::cout << "Input the name of variable " << i << ": ";
            std::cin >> input;
        }
        v[i] = input;
    }
    return v;
}
int DataUnit(std::string a)
{//all the unit data that corresponds the strings to row numbers to separate the data based on their unit
    int index;
    if(a == "kg")
    {
        index = 0;
    }
    else if (a == "m")
    {
        index = 1;
    }
    else if (a == "s")
    {
        index = 2;
    }
    else if (a == "K")
    {
        index = 3;
    }
    else if (a == "mol")
    {
        index = 4;
    }
    else if (a == "A")
    {
        index = 5;
    }
    else if (a == "cd")
    {
        index = 6;
    }
    else
    {
        index = -1;
    }
    return index;
}
std::vector<std::string>StringToVector(std::string &string,char delimiter)
{//transform a string to a vector of multiple strings based on a delimiter
    std::stringstream data(string);
    std::string entry;
    std::vector<std::string> vector;
    while (std::getline(data, entry, delimiter)) {
        vector.push_back(entry);
    }
    return vector;
}
void FunctionDisplay(std::vector<std::string> &vector, int N)
{//Display the function in a desired manner
    std::cout << vector[0] << " = f(";
    for (int i{1}; i < N+1; i++)
    {
        if (i < N)
        {
            std::cout << vector[i] << ", ";
        }
        else
        {
            std::cout << vector[i];
        }
    }
    std::cout << ")\n";
}
std::vector<int> RepeatingUnits(int StartI, int EndI, std::vector<std::vector<int>> &Matrix)
{//Make a vector with how many times certain units are used to determine what variables are used as repeating variables
    std::vector<int> vector(7,0);
    for (int i{StartI}; i<EndI;i++)
    {
        for (int j{0};j<8;j++)
        {
            if(Matrix[i][j]!=0)
            {
                vector[j]++;
            }
        }
    }
    return vector;
}
int MinVectorIndex (std::vector<int> vector)
{//Find the index of the smallest nonzero entry in the vector
    int index{};
    int min{100};
    for(int i{0};i<vector.size();i++)
    {
        if(vector[i]!=0 && vector[i]<=min)
        {
            min = vector[i];
            index = i;
        }
    }
    if(vector[index] == 0)
    {
        index = -1;
    }
    return index;
}
int Nonzero (std::vector<int> vector)
{//find the number of nonzero elements in a vector
    int counter{0};
    for(int i{0};i<vector.size();i++)
    {
        if(vector[i]!=0)
        {
            counter++;
        }
    }
    return counter;
}
std::vector<std::vector<double>> ConvertToDoubleMatrix (std::vector<std::vector<int>> &Matrix)
{//Convert an integer matrix to a double matrix
    size_t M{Matrix.size()};
    size_t N{Matrix[0].size()};
    std::vector<std::vector<double>> DoubleMatrix(M,std::vector<double>(N,0.0));
    for(int i{0};i<N;i++)
    {
        for(int j{0};j<M;j++)
        {
            if(Matrix[j][i] == 0)
            {
                continue;
            }
            else
            {
                DoubleMatrix[j][i] = static_cast<double>(Matrix[j][i]);
            }
        }
    }
    return DoubleMatrix;
}
std::vector<std::vector<int>> TransposeMatrix (std::vector<std::vector<int>> &Matrix)
{//Transpose a matrix
    size_t M{Matrix.size()};
    size_t N{Matrix[0].size()};
    std::vector<std::vector<int>> MatrixTransposed(N,std::vector<int>(M,0));
    for(int i{0};i<N;i++)
    {
        for(int j{0};j<M;j++)
        {
            MatrixTransposed[i][j] = Matrix[j][i];
        }
    }
    return MatrixTransposed;
}
std::pair<std::vector<std::vector<double>>, std::vector<std::string>> rowSwitch (std::vector<std::vector<double>> &Matrix, std::vector<std::string> &Vector, int i)
{//switch rows to get the nonzero pivot on the main diagonal
    size_t M{Matrix.size()};
    std::string RowNames(Vector[i]);
    std::vector<double> RowData(Matrix[i]);

    for(int j{i+1};j<M;j++)
    {
        if(Matrix[j][i] == 0.0 || j == i)
        {
            continue;
        }
        else
        {
            Matrix[i] = Matrix[j];
            Matrix[j] = RowData;
            Vector[i] = Vector[j];
            Vector[j] = RowNames;
            break;
        }
    }
    auto pair = std::make_pair(Matrix,Vector);
    return pair;
}
std::vector<double> vectorDivision (std::vector<double> &vector, double coefficient)
{//Divide all vector entries by a coefficient
    size_t N{vector.size()};
    for(int i{0};i<N;i++)
    {
        if(vector[i] == 0.0)
        {
            continue;
        }
        else
        {
            vector[i] /= coefficient;
        }
    }
    return vector;
}
std::vector<std::vector<double>> rowAddition (std::vector<std::vector<double>> &Matrix, int i)
{//Row addition used in Gaussian elimination
    size_t M{Matrix.size()};
    size_t N{Matrix[i].size()};
    for(int j{0};j<M;j++)
    {
        if(Matrix[j][i] == 0.0 || j == i)
        {
            continue;
        }
        else
        {
            double coefficient{Matrix[j][i]};
            for(int k{0};k<N;k++)
            {
                Matrix[j][k] -= coefficient*Matrix[i][k];
            }
        }
    }
    return Matrix;
}
std::pair<std::vector<std::vector<double>>, std::vector<std::string>> GaussianElimination (std::vector<std::vector<double>> &Matrix, std::vector<std::string> &Vector)
{//Gaussian elimination
    size_t N{Matrix[0].size()};
    for (int i{0}; i<N-1;i++)
    {
        if (Matrix[i][i] == 0.0)
        {
            rowSwitch(Matrix,Vector,i);
        }
        vectorDivision(Matrix[i],Matrix[i][i]);
        rowAddition(Matrix,i);
    }
    auto pair = std::make_pair(Matrix,Vector);
    return pair;
}
std::vector<std::vector<double>> addSolution (std::vector<std::vector<double>> &Matrix,std::vector<double> &Vector)
{//Add solution column to the system of equations
    size_t M{Matrix.size()};
    for (int j{0};j<M;j++)
    {
        Matrix[j].push_back(Vector[j]);
    }
    return Matrix;
}
std::string interpretSolution (std::vector<std::vector<double>> &Matrix,std::vector<std::string> &Vector)
{//translate the solution data from doubles to a string
    size_t M{Vector.size()};
    std::string solution;
    std::string power;
    for(int i{0};i<M;i++)
    {
        if(round(Matrix[i][M]*1000)/1000 == 0)
        {
            continue;
        }
        power = std::to_string(Matrix[i][M]);
        reduceStringZeros(power);
        solution += Vector[i] + "^" + power + "*";
    }
    solution.resize(solution.size()-1);
    return solution;
}
std::string reduceStringZeros (std::string &string)
{
    int size;
    size = string.size();
    for(int i{size-1};i>0;i--)
    {
        if(string[i] == '0' || string[i] == '.')
        {
            string.resize(i);
        }
        else
        {
            break;
        }
    }
    return string;
}
void displayPI (std::vector<std::string> &PIvector,std::vector<std::string> &solutionNames)
{//Display the final result
    size_t PI{solutionNames.size()};

    if(PI == 1)
    {
        std::cout << solutionNames[0] + "^~" << " = " << solutionNames[0] << "/(" << PIvector[0] << ")";
    }
    else
    {
        std::cout << solutionNames[0] << " = " << "f^~(";
        for (int i {1}; i<PI;i++)
        {
            if(i<PI-1)
            {
                std::cout << solutionNames[i] << "/(" << PIvector[i] << "), ";
            }
            else
            {
                std::cout << solutionNames[i] << "/(" << PIvector[i] << ")";
            }
        }
        std::cout << ")*" << PIvector[0];
    }
}
