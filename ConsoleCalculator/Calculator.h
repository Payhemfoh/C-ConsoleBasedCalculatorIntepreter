#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <functional>
#include <cmath>

enum OperatorType
{
	VALUE = 0,
	VARIABLE = 1,
	OPERATOR = 2
};

enum Operation
{
	NONE = 0,
	ADDITION = 1,
	SUBTRACTION = 2,
	MULTIPLY = 3,
	DIVISION = 4,
	EXPONENT = 5
};

class Calculator
{
private:
	std::vector<std::pair<std::string, OperatorType>> user_input;
	double result;
	bool is_running;

	void clearUserInput();
	void computeOperator();
	std::string getUserInput();
	void splitUserInput(std::string input);
	bool checkTerminateCommand(const std::string& command);

	void displayResult() const;
	Operation getOperation(std::string operation) const;
	double Calculation(double x, double y, Operation e) const;
public:
	void execute();

	std::ostream& operator<<(std::ostream& stream);
};

