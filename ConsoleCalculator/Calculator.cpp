#include "Calculator.h"
#include <functional>

static void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
}

static void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

static void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

static BracketType isbracket(const char& c) {

	switch (c)
	{
	case '(':
		return BracketType::BRACKET;
		break;
	case ')':
		return BracketType::BRACKETEND;
		break;
	case '[':
		return BracketType::SQUAREBRACKET;
		break;
	case ']':
		return BracketType::SQUAREBRACKETEND;
		break;
	case '{':
		return BracketType::CURLYBRACKET;
		break;
	case '}':
		return BracketType::CURLYBRACKETEND;
		break;
	}

	return BracketType::NOTBRACKET;
}

Input::Input()
{
	value = "";
	type = OperatorType::EMPTY;
	precedence = -1;
}

Input::Input(std::string value, OperatorType type, int precendence)
{
	this->value = value;
	this->type = type;
	this->precedence = precendence;
}

std::string Calculator::getUserInput()
{
	std::string input;
	bool is_valid_input = false;

	do {
		try {
			std::cout << "Enter your equation: (type exit or quit to terminate program)\n";
			std::getline(std::cin, input);
			
			trim(input);

			if (input.size() == 0)
				throw std::exception("No input received");

			is_valid_input = true;
		}
		catch (std::exception err)
		{
			std::cout << "Invalid input, please try again!\n";
			is_valid_input = false;
		}
	} while (!is_valid_input);
	
	return input;
}

int Calculator::findEndOfBracket(BracketType first_bracket, const std::string& input, int cur_pos)
{
	int end_bracket_pos = -1;
	int subbracket = 0;
	for (int index = cur_pos + 1; index < input.size(); ++index)
	{
		//find bracket and get position
		BracketType type2 = isbracket(input[index]);

		//handle for nested bracket
		if (type2 == first_bracket)
		{
			++subbracket;
		}
		else if (type2 == (first_bracket + 3))
		{
			--subbracket;
		}

		if (subbracket < 0)
		{
			end_bracket_pos = index;
			break;
		}

	}

	return end_bracket_pos;
}

bool Calculator::checkAndHandleBracket(std::string input, int& cur_pos, std::string& input_segment, std::vector<Input>& user_input)
{
	BracketType type = isbracket(input[cur_pos]);
	if (type == BracketType::NOTBRACKET || type >= BracketType::BRACKETEND)
		return false;

	//need to clean the input_segment also
	appendInputSegment(input_segment, user_input);

	//add multiply operator if no operator provided between value and bracket
	if (user_input.size() != 0 && user_input[user_input.size() - 1].type != OperatorType::OPERATOR) {
		input_segment = "*";
		appendInputSegment(input_segment, user_input);
	}

	//need to handle bracket
	int end_bracket_pos = findEndOfBracket(type, input, cur_pos);
	
	if (end_bracket_pos == -1)
		return false;

	//extract the substring
	std::string subequation = input.substr(cur_pos + 1, end_bracket_pos - cur_pos - 1);
	std::cout << subequation << std::endl;

	//bracket operations
	switch (type)
	{
	case BracketType::BRACKET:
		//here we use recursion to handle nested equation
		input_segment = std::to_string(processEquation(subequation));
		appendInputSegment(input_segment, user_input);
		break;
	}

	//move cursor to end of bracket sequence for next element
	cur_pos = end_bracket_pos;

	return true;
}

void Calculator::splitUserInput(std::string input, std::vector<Input>& user_input)
{
	std::string input_segment;
	for (int cur_pos = 0; cur_pos < input.size(); ++cur_pos)
	{
		if (checkAndHandleBracket(input, cur_pos, input_segment, user_input))
			continue;
		else if (isspace(input[cur_pos]))
		{
			appendInputSegment(input_segment, user_input);
			continue;
		}
		
		input_segment.push_back(input[cur_pos]);
	}

	appendInputSegment(input_segment, user_input);
}

void Calculator::appendInputSegment(std::string& input, std::vector<Input>& user_input)
{
	if (input.size() == 0)
		return;

	if (isdigit(input[0]))
	{
		user_input.push_back(Input(input, OperatorType::VALUE, -1));
	}
	else if (isalpha(input[0]))
	{
		user_input.push_back(Input(input, OperatorType::VARIABLE, -1));
	}
	else
	{
		int operator_precendence = getOperatorPrecedence(input);
		user_input.push_back(Input(input, OperatorType::OPERATOR, operator_precendence));
	}

	input.clear();
}

double Calculator::Calculation(double x, double y, Operation e) const
{
	double result = 0.0;
	switch (e)
	{
	case Operation::ADDITION:
		result = x + y;
		break;
	case Operation::SUBTRACTION:
		result = x - y;
		break;
	case Operation::MULTIPLY:
		result = x * y;
		break;
	case Operation::DIVISION:
		if (y != 0.0)
			result = x / y;
		break;
	case Operation::EXPONENT:
		result = pow(x, y);
	}

	return result;
}

int Calculator::getOperatorPrecedence(std::string operation) const
{
	if (precedence_map.find(operation) == precedence_map.end())
		return 0;

	return precedence_map.at(operation);
}

Operation Calculator::getOperation(std::string operation) const
{
	if (operation_map.find(operation) == operation_map.end())
		return Operation::NONE;
	
	return operation_map.at(operation);
}

void Calculator::computeOperator(std::vector<Input>& user_input, double& result)
{
	//main functionality in calculator, need to deal with
	this->displayInputStack(user_input);

	//find the operator
	int index = 0;

	do {
		index = -1;

		std::vector<Input>::iterator itr = std::max_element(user_input.begin(), user_input.end(),
			[](const Input& a, const Input& b) {return a.precedence < b.precedence; });

		int max_precedence = itr->precedence;

		if (max_precedence >= 0)
		{
			for (int i = 0; i < user_input.size(); ++i)
			{
				if (user_input[i].type == OperatorType::OPERATOR && user_input[i].precedence == max_precedence) {
					index = i;
					break;
				}
			}
		}

		if (index >= 0)
		{
			//get value before and after the operator
			double first_value = std::stod(user_input[index - 1].value);
			double second_value = std::stod(user_input[index + 1].value);
			Operation operation = this->getOperation(user_input[index].value);
			//compute the total
			double result = this->Calculation(first_value, second_value, operation);

			//replace the value back to the list
			std::vector<Input> new_list;
			for (size_t i = 0; i < user_input.size() - 2; ++i)
			{
				if (i < index - 1) {
					new_list.push_back(user_input[i]);
				}
				else if (i == index - 1) {
					new_list.push_back(Input(std::to_string(result), OperatorType::VALUE, -1));
					//new_list.push_back(std::pair<std::string, OperatorType>(std::to_string(result), OperatorType::VALUE));
				}
				else
				{
					new_list.push_back(user_input[i+2]);
				}
			}
			user_input = new_list;
		}
	} while (index != -1);
	
	if (user_input.size() < 1)
	{
		result = 0.0;
	}
	else if (user_input.size() == 1)
	{
		result = std::stod(user_input[0].value);
	}
}

void Calculator::displayInputStack(const std::vector<Input>& user_input) const
{
	//here, we display the splited input
	std::cout << "Here is the list of your input: \n";

	for (const auto& input : user_input)
	{
		std::cout << input.value << "\t" << input.type << "\n";
	}
}

void Calculator::displayResult(const double& result) const
{
	//here, we display the final result
	std::cout << "\nThe final result of the equation is: " << result << "\n\n" << std::endl;
}

//if user input exit or quit, we will terminate the program
//return true when keyword is matched
bool Calculator::checkTerminateCommand(const std::string& command)
{
	if (command == "quit"  || command == "exit")
	{
		this->is_running = false;
		return true;
	}

	return false;
}

/*
void Calculator::clearUserInput(std::vector<Input>& user_input, double& result) const
{
	user_input.clear();
	result = 0.0;
}
*/
void Calculator::execute()
{
	std::string input;
	this->is_running = true;

	while (this->is_running)
	{
		try {
			input = this->getUserInput();

			if (this->checkTerminateCommand(input))
				break;

			processEquation(input);
		}
		catch (std::exception err)
		{
			this->is_running = false;
			std::cerr << "Software Crashed due to unhandled exception: " << err.what() << std::endl;
		}
	}
}

double Calculator::processEquation(const std::string& input)
{
	std::vector<Input> user_input;
	double result;

	this->splitUserInput(input, user_input);

	//here extract value and operator from user input and compute it
	this->computeOperator(user_input, result);

	this->displayResult(result);

	return result;
}