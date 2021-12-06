# DimensionalAnalysis
A C++ code that can be used to do dimensional analysis

The code is fairly simple and works quite straightforward, the expected input is a function that consists of a solution variable equal to a function of variables.
E.g., for the drag force on a sphere, this would look like D = f(U,d,rho,mu).

1) The code starts by asking how many variables the function of variables consists of. This is thus the number of variables in the function without the solution variable
and for the example of the drag force this would be 4.
2) The following steps ask to input the names for all the variables and finishes with a presentation of the function. If the inputted function differs from what you desire
the code should be re-run and the input can be tried again.
3) When the inputted function is correct the code continues with the input of the units as a function in the form of kg^1*m^1*s^-2. Note that all units need to be supplied
with their power, even if the power is 1, next to this the code only takes integers as power input.
4) The code will now calculate and output the final result. Note that if the results seem too complex the input should be done in a different order, the variabels inputted
first tend to be used as independent variables over the variables inputted later.

The code contains several error messages to prevent abuse and should ensure correct output. When it detects wrong input it will notify the user what went wrong and when 
possible it allows re-input of the incorrect data. Please, if you find an unexpected error or inefficiency in the code, contact me on my email jaimy.g@hotmail.com. This was
a small project for myself to improve my C++ coding skills and I would greatly benefit from your feedback for future projects.
