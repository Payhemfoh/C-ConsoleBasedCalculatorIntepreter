#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <functional>
#include <cmath>

enum OperatorType
{
	EMPTY = 0,
	VALUE = 1,
	VARIABLE = 2,
	OPERATOR = 3
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

struct Input
{
	std::string value;
	OperatorType type;
	int precedence;

	Input();
	Input(std::string value, OperatorType type, int precendence);
};

class Calculator
{
private:
	std::unordered_map<std::string, int> precedence_map = {
		{"+", 1},
		{"-", 1},
		{"*", 2},
		{"/", 2},
		{"**", 3},
	};
	std::unordered_map<std::string, Operation> operation_map = {
		{"+", Operation::ADDITION},
		{"-", Operation::SUBTRACTION},
		{"*", Operation::MULTIPLY},
		{"/", Operation::DIVISION},
		{"**", Operation::EXPONENT},
	};

	//std::vector<std::pair<std::string, OperatorType>> user_input;
	std::vector<Input> user_input;
	double result;
	bool is_running;

	void clearUserInput();
	void computeOperator();
	std::string getUserInput();
	void splitUserInput(std::string input);
	bool checkTerminateCommand(const std::string& command);

	void displayResult() const;
	int getOperatorPrecedence(std::string operation) const;
	Operation getOperation(std::string operation) const;
	double Calculation(double x, double y, Operation e) const;
public:
	void execute();

	std::ostream& operator<<(std::ostream& stream);
};

