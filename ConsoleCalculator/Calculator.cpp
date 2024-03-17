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

void Calculator::splitUserInput(std::string input)
{
	std::stringstream input_stream(input);

	std::string input_segment;
	while (input_stream >> input_segment)
	{
		if (isdigit(input_segment[0]))
		{
			this->user_input.push_back(std::pair<std::string, OperatorType>(input_segment, OperatorType::VALUE));
		}
		else if (isalpha(input_segment[0]))
		{
			this->user_input.push_back(std::pair<std::string, OperatorType>(input_segment, OperatorType::VARIABLE));
		}
		else
		{
			this->user_input.push_back(std::pair<std::string, OperatorType>(input_segment, OperatorType::OPERATOR));
		}
	}
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

Operation Calculator::getOperation(std::string operation) const
{
	Operation type = Operation::NONE;

	if (operation == "+")
	{
		type = Operation::ADDITION;
	}
	else if (operation == "-")
	{
		type = Operation::SUBTRACTION;
	}
	else if (operation == "*")
	{
		type = Operation::MULTIPLY;
	}
	else if (operation == "/")
	{
		type = Operation::DIVISION;
	}
	else if (operation == "**")
	{
		type = Operation::EXPONENT;
	}

	return type;
}

void Calculator::computeOperator()
{
	//main functionality in calculator, need to deal with
	this->displayResult();

	//find the operator
	int index = 0;

	do {
		index = -1;

		for (int i = 0; i < user_input.size(); ++i)
		{
			if (user_input[i].second == OperatorType::OPERATOR) {
				index = i;
				break;
			}
		}

		if (index >= 0)
		{
			//get value before and after the operator
			double first_value = std::stod(user_input[index - 1].first);
			double second_value = std::stod(user_input[index + 1].first);
			Operation operation = this->getOperation(user_input[index].first);
			//compute the total
			double result = this->Calculation(first_value, second_value, operation);

			//replace the value back to the list
			std::vector<std::pair<std::string, OperatorType>> new_list;
			for (size_t i = 0; i < user_input.size() - 2; ++i)
			{
				if (i < index - 1) {
					new_list.push_back(user_input[i]);
				}
				else if (i == index - 1) {
					new_list.push_back(std::pair<std::string, OperatorType>(std::to_string(result), OperatorType::VALUE));
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
		this->result = 0.0;
	}
	else if (user_input.size() == 1)
	{
		this->result = std::stod(user_input[0].first);
	}
}

void Calculator::displayResult() const
{
	//here, we display the final result
	std::cout << "Here is the list of your input: \n";

	for (const auto& input : user_input)
	{
		std::cout << input.first << "\t" << input.second << "\n";
	}

	std::cout << "\nThe final result of the equation is: " << this->result << "\n\n" << std::endl;
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

void Calculator::clearUserInput()
{
	this->user_input.clear();
	this->result = 0.0;
}

void Calculator::execute()
{
	std::string input;
	this->is_running = true;
	while (this->is_running)
	{
		try {
			this->clearUserInput();

			input = this->getUserInput();

			if (this->checkTerminateCommand(input))
				break;

			this->splitUserInput(input);

			//here extract value and operator from user input and compute it
			this->computeOperator();

			this->displayResult();
		}
		catch (std::exception err)
		{
			this->is_running = false;
			std::cerr << "Software Crashed due to unhandled exception: " << err.what() << std::endl;
		}
	}
}

std::ostream& Calculator::operator<<(std::ostream& stream)
{
	std::cout << "Here is the list of your input: \n";

	for (const auto& input : user_input)
	{
		stream << input.first << "\t" << input.second << "\n";
	}

	std::cout << "The final result of the equation is: " << this->result << "\n\n" << std::endl;

	return stream;
}