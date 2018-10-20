#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

class Element
{
public:
	Element() {};
	Element(double value, char sign) :value(value), sign(sign) {}

	double value = 0;
	char sign = 0;

	bool valid() noexcept { return (bool)(value||sign); }
	void clear() noexcept { value = 0; sign = 0; }
	bool isNumber() noexcept { return !sign; }
};

bool isCharacter(char c)
{
	return std::strchr("-+*/()", c);
}

int sizeOfTheNumber(double nr)
{
	int a = nr;
	int i = 0;
	while(a)
	{
		a /= 10;
		i++;
	}
	if (i == 0) { i = 1; }

	return i;
}


void removeSpaces(std::string& a) 
{

	for(auto i = a.begin(); i!=a.end(); i++)
	{
		if(*i==' ')
		{
			a.erase(i);
			i--;
			//if (i == a.end())break;
		}
	}

}

int shrinkEvaluation(std::vector<Element> &elements)
{
	int count = 0;
	for (auto i = 0; i < elements.size() - 1; i++)
	{

		if (elements[i + 1].isNumber())
		{
			if (elements[i].sign == '-')
			{
				elements[i + 1].value *= -1;
				elements.erase(elements.begin() + i);
				i--;
				count++;
			}
			else if (elements[i].sign == '+')
			{
				elements.erase(elements.begin() + i);
				i--;
				count++;
			}
		}


	}

	return count;
}

int doGrade2(std::vector<Element> &elements)
{
	int count = 0;
	for (auto i = 1; i < elements.size() - 1; i++)
	{
		if(i!=0)
		{
			if (elements[i + 1].isNumber() && elements[i - 1].isNumber())
			{
				if (elements[i].sign == '/')
				{
					elements[i - 1].value /= elements[i + 1].value; ///! division by zero
					elements.erase(elements.begin() + i);
					elements.erase(elements.begin() + i);
					i -= 2;
					count++;
				}
				else if (elements[i].sign == '*')
				{
					elements[i - 1].value *= elements[i + 1].value;
					elements.erase(elements.begin() + i);
					elements.erase(elements.begin() + i);
					i -= 2;
					count++;
				}
			}
		}
	
	}
	return count;
}

int doGrade1(std::vector<Element> &elements)
{
	Element total(0, 0);

	for(auto i:elements)
	{
		total.value += i.value;
	}

	elements.erase(elements.begin(), elements.end());
	elements.push_back(total);

	return 0;
}

std::vector<Element> convert(std::string s)
{
	enum  State
	{
		none,
		number,
		zecimal
	};
	
	State currentState = State::number;

	std::vector<Element> elements;
	elements.reserve(12);
  Element currentElement;
	double zecimalPart = 0;

	for(auto i = s.begin(); i!=s.end(); i++)
	{
		if(isCharacter(*i))
		{	  //not a number
			
			if(currentElement.valid())
			{
				zecimalPart /= std::pow(10, sizeOfTheNumber(zecimalPart));
				currentElement.value += zecimalPart;
				elements.push_back(currentElement);
				currentElement.clear();
				zecimalPart = 0;
				currentState = number;
			}
			

			currentElement.sign = *i;
			elements.push_back(currentElement);
			currentElement.clear();

		}else // a number
		{
			if(*i=='.')
			{
				currentState = zecimal;
			}else
			{
				if(currentState == zecimal)
				{
					zecimalPart *= 10;
					zecimalPart += *i - '0';
				}else
				{
					currentElement.value *= 10;
					currentElement.value += *i - '0';
				}
			
			}

		}		

	
	}

	if (currentElement.valid())
	{
		zecimalPart /= std::pow(10, sizeOfTheNumber(zecimalPart));
		currentElement.value += zecimalPart;
		elements.push_back(currentElement);
		currentElement.clear();
		zecimalPart = 0;
		currentState = number;
	}
	///


	return elements;
}

Element evaluate(std::vector<Element> &elements)
{

begin:
	while (shrinkEvaluation(elements));

	int lastParanthesePos = 0;


	for(int i=0; i<elements.size(); i++)
	{
		if(elements[i].sign == '(')
		{
			lastParanthesePos = i;
		}else if(elements[i].sign == ')')
		{
			std::vector<Element> temp;
			temp.reserve(i - lastParanthesePos);
			
			for(int ii=lastParanthesePos + 1; ii < i; ii++)
			{
				temp.push_back(elements[ii]);
			}
				////
			evaluate(temp);

			for(int x=0; x < i - lastParanthesePos; x++)
			{
				elements.erase(elements.begin() + lastParanthesePos + 1);
			}
			//elements.erase(elements.begin() + 1);
			elements[lastParanthesePos] = temp[0];
			goto begin;
		}
	
	}


	while (doGrade2(elements));

	while (doGrade1(elements));

//	if(elements.size()!=1)
//	{
//		puts("size gresit");
//	}

	return elements[0];
}


double calc(std::string expression)
{
	removeSpaces(expression);

	auto expr = convert(expression);


	evaluate(expr);

	//while (doGrade2(expr));

	//while (doGrade1(expr));

	//for(auto i: expr)
	//{
		//std::printf("%+lf, %c\n", i.value, i.sign);
	
	//}

	//std::puts(expression.c_str());
	return expr[0].value;
}