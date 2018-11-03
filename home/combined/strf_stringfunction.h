#pragma once

#include <functional>
#include <string>
#include <vector>
#include <map>


namespace strf
{

#pragma region opeators

	enum ValueType
	{
		VT_NONE = 0x0,
		VT_NUMBER = 0x1,
		VT_VAR = 0x2,
		VT_BRAKET = 0x3,
		VT_OP = 0x8,
		VT_OP_PRE = VT_OP | 0x10,
		VT_OP_POST = VT_OP | 0x20,
		VT_OP_BETWEEN = VT_OP | 0x30
	};

	class Value
	{
	public:
		virtual ~Value() = default;
		virtual double getValue() = 0;
		virtual unsigned getRank() = 0;
		virtual ValueType getType() = 0;
	};

	class Number :public Value
	{
		double m_value;
	public:
		Number(double number);
		virtual ~Number() override;
		virtual double getValue() override;
		virtual unsigned getRank() override;
		virtual ValueType getType() override;
	};

	class Braket :public Value
	{
		Value *m_var;
	public:
		Braket(Value *var);
		virtual ~Braket() override;
		virtual double getValue() override;
		virtual unsigned getRank() override;
		virtual ValueType getType() override;
	};

	class Variable :public Value
	{
		double& m_value;
	public:
		Variable(double& variable);
		virtual ~Variable() override;
		virtual double getValue() override;
		virtual unsigned getRank() override;
		virtual ValueType getType() override;
	};

	class Operator_Pre :public Value
	{
		std::function<double(double)> m_function;
	public:
		Value *var;
		Operator_Pre(std::function<double(double)> function);
		virtual ~Operator_Pre() override;
		virtual double getValue() override;
		virtual unsigned getRank() override;
		virtual ValueType getType() override;
	};

	class Operator_Post :public Value
	{
		std::function<double(double)> m_function;
	public:
		Value *var;
		Operator_Post(std::function<double(double)> function);
		virtual ~Operator_Post() override;
		virtual double getValue() override;
		virtual unsigned getRank() override;
		virtual ValueType getType() override;
	};

	class Operator_Between :public Value
	{
		std::function<double(double, double)> m_function;
		unsigned m_rank;
	public:
		Value *var1;
		Value *var2;
		Operator_Between(std::function<double(double, double)> function, unsigned rank);
		virtual ~Operator_Between() override;
		virtual double getValue() override;
		virtual unsigned getRank() override;
		virtual ValueType getType() override;
	};

#pragma endregion

	class Function
	{
		class FunctionCreator
		{
			Function& m_function;
			size_t m_iterPos;
			std::wstring m_input;
			std::vector<Value*> m_scanResult;
			ValueType m_lastScanned;
			std::exception m_error;
			bool m_hasX;
			bool m_hasY;

		private:
			void ManageOperator(wchar_t ch);
			void ManageNumber(wchar_t ch);
			void ManageBraket(wchar_t ch);
			void ManageOther(wchar_t ch);
			void ScanInput();
			void SetOperator(std::vector<Value*>& out, Operator_Pre* op);
			void SetOperator(std::vector<Value*>& out, Operator_Post* op);
			void SetOperator(std::vector<Value*>& out, Operator_Between* op);
			void MakeOperationGraph();

		public:
			FunctionCreator(Function& f, std::wstring input);
			Value* CreateFunction();
			int VarCount();
		};
	private:
		std::map<std::wstring, std::function<Value*()>> m_words;
		Value *m_function;
		double m_x;
		double m_y;
		int m_varcount;

	private:
		void InitWords();
		void DeleteFunction();

	public:
		Function();
		~Function();
		bool LoadFunction(std::wstring input);
		inline int VarCount() { return m_varcount; }
		double operator()(double x);
		double operator()(double x, double y);
	};
}