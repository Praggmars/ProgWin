#include "strf_stringfunction.h"
#include <cmath>

#define DFDCAST(f)static_cast<double(*)(double)>(f)

namespace strf
{
	template<typename T>
	nullptr_t ReturnNullClearVector(std::vector<T>& v)
	{
		for (auto e : v)
			delete e;
		return nullptr;
	}

#pragma region operators

	Number::Number(double number)
		:m_value(number) {}
	Number::~Number() {}
	double Number::getValue()
	{
		return m_value;
	}
	unsigned Number::getRank()
	{
		return 0;
	}
	ValueType Number::getType()
	{
		return ValueType::VT_NUMBER;
	}

	Braket::Braket(Value *var)
		:m_var(var) {}
	Braket::~Braket()
	{
		if (m_var)
			delete m_var;
	}
	double Braket::getValue()
	{
		return m_var->getValue();
	}
	unsigned Braket::getRank()
	{
		return 0;
	}
	ValueType Braket::getType()
	{
		return VT_BRAKET;
	}

	Variable::Variable(double& variable)
		:m_value(variable) {}
	Variable::~Variable() {}
	double Variable::getValue()
	{
		return m_value;
	}
	unsigned Variable::getRank()
	{
		return 0;
	}
	ValueType Variable::getType()
	{
		return ValueType::VT_VAR;
	}

	Operator_Pre::Operator_Pre(std::function<double(double)> function)
		:m_function(function),
		var(nullptr) {}
	Operator_Pre::~Operator_Pre()
	{
		if (var)
			delete var;
	}
	double Operator_Pre::getValue()
	{
		return m_function(var->getValue());
	}
	unsigned Operator_Pre::getRank()
	{
		return 4;
	}
	ValueType Operator_Pre::getType()
	{
		return ValueType::VT_OP_PRE;
	}

	Operator_Post::Operator_Post(std::function<double(double)> function)
		:m_function(function),
		var(nullptr) {}
	Operator_Post::~Operator_Post()
	{
		if (var)
			delete var;
	}
	double Operator_Post::getValue()
	{
		return m_function(var->getValue());
	}
	unsigned Operator_Post::getRank()
	{
		return 4;
	}
	ValueType Operator_Post::getType()
	{
		return ValueType::VT_OP_POST;
	}

	Operator_Between::Operator_Between(std::function<double(double, double)> function, unsigned rank)
		:m_function(function),
		m_rank(rank),
		var1(nullptr),
		var2(nullptr) {}
	Operator_Between::~Operator_Between()
	{
		if (var1)
			delete var1;
		if (var2)
			delete var2;
	}
	double Operator_Between::getValue()
	{
		return m_function(var1->getValue(), var2->getValue());
	}
	unsigned Operator_Between::getRank()
	{
		return m_rank;
	}
	ValueType Operator_Between::getType()
	{
		return ValueType::VT_OP_BETWEEN;
	}

#pragma endregion

#pragma region Function

	void Function::InitWords()
	{
		m_words[L"x"] = [this]()->Value* {return new Variable(m_x); };
		m_words[L"y"] = [this]()->Value* {return new Variable(m_y); };
		m_words[L"e"] = []()->Value* {return new Number(2.718281828459045); };
		m_words[L"pi"] = []()->Value* {return new Number(3.141592653589793); };
		m_words[L"phi"] = []()->Value* {return new Number(1.618033988749895); };
		m_words[L"phin"] = []()->Value* {return new Number(-0.618033988749895); };

		m_words[L"sqrt"] = []()->Value* {return new Operator_Pre(DFDCAST(sqrt)); };
		m_words[L"cbrt"] = []()->Value* {return new Operator_Pre(DFDCAST(cbrt)); };
		m_words[L"exp2"] = []()->Value* {return new Operator_Pre(DFDCAST(exp2)); };
		m_words[L"exp"] = []()->Value* {return new Operator_Pre(DFDCAST(exp)); };
		m_words[L"log2"] = []()->Value* {return new Operator_Pre(DFDCAST(log2)); };
		m_words[L"log"] = []()->Value* {return new Operator_Pre(DFDCAST(log)); };
		m_words[L"log10"] = []()->Value* {return new Operator_Pre(DFDCAST(log10)); };
		m_words[L"sin"] = []()->Value* {return new Operator_Pre(DFDCAST(sin)); };
		m_words[L"cos"] = []()->Value* {return new Operator_Pre(DFDCAST(cos)); };
		m_words[L"tan"] = []()->Value* {return new Operator_Pre(DFDCAST(tan)); };
		m_words[L"asin"] = []()->Value* {return new Operator_Pre(DFDCAST(asin)); };
		m_words[L"acos"] = []()->Value* {return new Operator_Pre(DFDCAST(acos)); };
		m_words[L"atan"] = []()->Value* {return new Operator_Pre(DFDCAST(atan)); };
		m_words[L"sinh"] = []()->Value* {return new Operator_Pre(DFDCAST(sinh)); };
		m_words[L"cosh"] = []()->Value* {return new Operator_Pre(DFDCAST(cosh)); };
		m_words[L"tanh"] = []()->Value* {return new Operator_Pre(DFDCAST(tanh)); };
		m_words[L"asinh"] = []()->Value* {return new Operator_Pre(DFDCAST(asinh)); };
		m_words[L"acosh"] = []()->Value* {return new Operator_Pre(DFDCAST(acosh)); };
		m_words[L"atanh"] = []()->Value* {return new Operator_Pre(DFDCAST(atanh)); };
		m_words[L"erf"] = []()->Value* {return new Operator_Pre(DFDCAST(erf)); };
		m_words[L"erfc"] = []()->Value* {return new Operator_Pre(DFDCAST(erfc)); };
		m_words[L"tgamma"] = []()->Value* {return new Operator_Pre(DFDCAST(tgamma)); };
		m_words[L"lgamma"] = []()->Value* {return new Operator_Pre(DFDCAST(lgamma)); };
		m_words[L"abs"] = []()->Value* {return new Operator_Pre(DFDCAST(abs)); };
		m_words[L"sign"] = []()->Value* {return new Operator_Pre([](double x)->double {
			if (x == 0.0)
				return 0.0;
			return x > 0 ? 1.0 : -1.0;
		}); };
		m_words[L"trunc"] = []()->Value* {return new Operator_Pre(DFDCAST(trunc)); };
		m_words[L"rint"] = []()->Value* {return new Operator_Pre(DFDCAST(rint)); };
		m_words[L"nearbyint"] = []()->Value* {return new Operator_Pre(DFDCAST(nearbyint)); };
		m_words[L"floor"] = []()->Value* {return new Operator_Pre(DFDCAST(floor)); };
		m_words[L"ceil"] = []()->Value* {return new Operator_Pre(DFDCAST(ceil)); };
		m_words[L"round"] = []()->Value* {return new Operator_Pre(DFDCAST(round)); };

		m_words[L"+"] = []()->Value* {return new Operator_Between([](double x, double y)->double {return x + y; }, 1); };
		m_words[L"-"] = []()->Value* {return new Operator_Between([](double x, double y)->double {return x - y; }, 1); };
		m_words[L"*"] = []()->Value* {return new Operator_Between([](double x, double y)->double {return x * y; }, 2); };
		m_words[L"/"] = []()->Value* {return new Operator_Between([](double x, double y)->double {return x / y; }, 2); };
		m_words[L"%"] = []()->Value* {return new Operator_Between([](double x, double y)->double {return fmod(x, y); }, 2); };
		m_words[L"^"] = []()->Value* {return new Operator_Between([](double x, double y)->double {return pow(x, y); }, 3); };
	}
	void Function::DeleteFunction()
	{
		if (m_function)
		{
			delete m_function;
			m_function = nullptr;
		}
	}
	Function::Function() :m_function(nullptr)
	{
		InitWords();
	}
	Function::~Function()
	{
		DeleteFunction();
	}
	bool Function::LoadFunction(std::wstring input)
	{
		DeleteFunction();
		FunctionCreator fc(*this, input);
		m_function = fc.CreateFunction();
		m_varcount = fc.VarCount();
		return  m_function != nullptr;
	}
	double Function::operator()(double x)
	{
		if (m_function)
		{
			m_x = x;
			m_y = x;
			return m_function->getValue();
		}
		return NAN;
	}
	double Function::operator()(double x, double y)
	{
		if (m_function)
		{
			m_x = x;
			m_y = y;
			return m_function->getValue();
		}
		return NAN;
	}

#pragma endregion

#pragma region FunctionCreator

	inline bool isOperator(wchar_t ch)
	{
		return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^';
	}
	inline bool isNumber(wchar_t ch)
	{
		return ch >= '0' && ch <= '9';
	}
	inline bool isNumElem(wchar_t ch)
	{
		return isNumber(ch) || ch == '.';
	}

	void Function::FunctionCreator::ManageOperator(wchar_t ch)
	{
		if (m_lastScanned == VT_NONE)
		{
			if (ch != '-')
				throw std::exception("invalid operator");
			m_scanResult.push_back(new Number(-1.0));
			m_scanResult.push_back(m_function.m_words[L"*"]());
		}
		else
		{
			if (m_lastScanned & VT_OP)
				throw std::exception("operator dublicate");

			std::wstring str{ ch };
			m_scanResult.push_back(m_function.m_words[str]());
		}
		m_iterPos++;
		m_lastScanned = VT_OP;
	}
	void Function::FunctionCreator::ManageNumber(wchar_t ch)
	{
		if (!(m_lastScanned == VT_NONE || (m_lastScanned & VT_OP)))
			throw std::exception("invalid expression");
		std::wstring number;
		bool hasDot = false;
		do
		{
			if (ch == '.')
			{
				if (hasDot)
					throw std::exception("invalid number");
				hasDot = true;
			}
			number += ch;
			ch = m_input[++m_iterPos];
		} while (isNumElem(ch));
		m_scanResult.push_back(new Number(std::stod(number)));
		m_lastScanned = VT_NUMBER;
	}
	void Function::FunctionCreator::ManageBraket(wchar_t ch)
	{
		if (m_lastScanned == VT_NUMBER || m_lastScanned == VT_VAR || m_lastScanned == VT_BRAKET)
			throw std::exception("invalid expression");
		size_t counter = m_iterPos + 1;
		std::wstring expression;
		unsigned braketCounter = 1;
		do
		{
			if (counter >= m_input.length())
				throw std::exception("unclosed braket");
			ch = m_input[counter++];
			if (ch == '(')
				braketCounter++;
			if (ch == ')')
				braketCounter--;
			expression += ch;
		} while (braketCounter > 0);
		expression.pop_back();
		FunctionCreator fc(m_function, expression);
		Value *braket = fc.CreateFunction();
		m_hasX |= fc.m_hasX;
		m_hasY |= fc.m_hasY;
		if (braket == nullptr)
			throw m_error;
		m_scanResult.push_back(new Braket(braket));
		m_iterPos = counter;
		m_lastScanned = VT_BRAKET;
	}
	void Function::FunctionCreator::ManageOther(wchar_t ch)
	{
		if (!(m_lastScanned == VT_NONE || (m_lastScanned & VT_OP)))
			throw std::exception("missing symbol");
		std::wstring wordstr;
		do
		{
			wordstr += ch;
			ch = m_input[++m_iterPos];
			if (m_iterPos >= m_input.length())
				break;
		} while (!isOperator(ch) && ch != '(');
		auto word = m_function.m_words.find(wordstr);
		if (word == m_function.m_words.end())
		{
			std::string errorMessage = "unknown symbol: ";
			for (size_t i = 0; i < wordstr.length(); i++)
				errorMessage += (char)wordstr[i];
			throw std::exception(errorMessage.c_str());
		}
		Value *val = word->second();
		m_scanResult.push_back(val);
		m_lastScanned = val->getType();
	}

	void Function::FunctionCreator::ScanInput()
	{
		wchar_t ch;
		m_iterPos = 0;
		while (m_iterPos < m_input.length())
		{
			if ((ch = m_input[m_iterPos]) == ' ')
			{
				++m_iterPos;
				continue;
			}
			if (isOperator(ch))
				ManageOperator(ch);
			else if (isNumElem(ch))
				ManageNumber(ch);
			else if (ch == '(')
				ManageBraket(ch);
			else
				ManageOther(ch);
		}
	}

	void Function::FunctionCreator::SetOperator(std::vector<Value*>& out, Operator_Pre* op)
	{
		if (++m_iterPos >= m_scanResult.size())
			throw std::exception("invalid operator");
		op->var = m_scanResult[m_iterPos];
	}
	void Function::FunctionCreator::SetOperator(std::vector<Value*>& out, Operator_Post* op)
	{
		op->var = out[out.size() - 1];
		out.pop_back();
	}
	void Function::FunctionCreator::SetOperator(std::vector<Value*>& out, Operator_Between* op)
	{
		if (++m_iterPos >= m_scanResult.size())
			throw std::exception("invalid operator");
		op->var1 = out[out.size() - 1];
		op->var2 = m_scanResult[m_iterPos];
		out.pop_back();
	}
	void Function::FunctionCreator::MakeOperationGraph()
	{
		std::vector<Value*> tmpResult;
		for (size_t r = 4; r > 0; r--)
		{
			for (m_iterPos = 0; m_iterPos < m_scanResult.size(); m_iterPos++)
			{
				Value *v = m_scanResult[m_iterPos];
				m_scanResult[m_iterPos] = nullptr;
				if (v->getRank() == r && (v->getType() & VT_OP))
				{
					switch (v->getType())
					{
					case VT_OP_PRE:
						SetOperator(tmpResult, (Operator_Pre*)v);
						break;
					case VT_OP_POST:
						SetOperator(tmpResult, (Operator_Post*)v);
						break;
					case VT_OP_BETWEEN:
						SetOperator(tmpResult, (Operator_Between*)v);
						break;
					}
				}
				tmpResult.push_back(v);
			}
			m_scanResult.swap(tmpResult);
			tmpResult.clear();
		}
	}

	Value* Function::FunctionCreator::CreateFunction()
	{
		try
		{
			if (m_input.empty())
				throw std::exception("no function");
			ScanInput();
			MakeOperationGraph();
		}
		catch (std::exception& ex)
		{
			m_error = ex;
			for (auto v : m_scanResult)
				if (v)
					delete v;
			return nullptr;
		}
		return m_scanResult[0];
	}

	int Function::FunctionCreator::VarCount()
	{
		int count = 0;
		if (m_hasX) count++;
		if (m_hasY) count++;
		return count;
	}

	Function::FunctionCreator::FunctionCreator(Function& f, std::wstring input) :m_function(f)
	{
		m_input = input;
		m_lastScanned = VT_NONE;
	}

#pragma endregion

}