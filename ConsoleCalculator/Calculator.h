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

enum BracketType
{
	NOTBRACKET = 0,
	BRACKET = 1,
	SQUAREBRACKET = 2,
	CURLYBRACKET = 3,
	BRACKETEND = 4,
	SQUAREBRACKETEND = 5,
	CURLYBRACKETEND = 6
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
	bool is_running;

	//void clearUserInput(std::vector<Input>& user_input, double& result) const;
	std::string getUserInput();
	bool checkTerminateCommand(const std::string& command);
	double processEquation(const std::string& input);
	void splitUserInput(std::string input, std::vector<Input>& user_input);
	void appendInputSegment(std::string& input, std::vector<Input>& user_input);
	int findEndOfBracket(BracketType first_bracket, const std::string& input, int cur_pos);
	bool checkAndHandleBracket(std::string input, int& cur_pos, std::string& input_segment, std::vector<Input>& user_input);
	void computeOperator(std::vector<Input>& user_input, double& result);
	
	void displayInputStack(const std::vector<Input>& user_input) const;
	void displayResult(const double& result) const;
	int getOperatorPrecedence(std::string operation) const;
	Operation getOperation(std::string operation) const;
	double Calculation(double x, double y, Operation e) const;
public:
	void execute();
};

