#ifndef RPN_LIBHEADER
	#include <sstream>
#endif

#include "exception.h"
#include "expression.h"
#include "evaluator.h"
#include "node.h"
#include "parsers/all.h"

namespace RPN
{
	typedef std::vector<const Node*>::iterator EItr;
	typedef std::vector<const Node*>::const_iterator ECItr;
	
	Expression::Expression()
	{
		//Nothing to do...
	}
	
	Expression::Expression(const std::string& string, const RPN::Context& context, Format format)
	{
		parse(string, context, format);
	}
	
	Expression::~Expression()
	{
		clear();
	}
	
	Evaluator* Expression::buildEvaluator() const
	{
		Evaluator* ret = new Evaluator();
		ret->reserve(mMaxAvailable);
		return ret;
	}
	
	void Expression::clear()
	{
		EItr end = mStack.end();
		for(EItr i = mStack.begin(); i < end; ++i)
		{
			(*i)->dereference();
		}
		
		mStack.clear();
		mIsCached = false;
		mIsVolatile = false;
	}
	
	double Expression::evaluate() const
	{
		if(mIsCached)
		{
			return mResult;
		}
		
		Evaluator evaluator;
		return evaluate(evaluator);
	}
	
	double Expression::evaluate(Evaluator& evaluator) const
	{
		if(mIsCached)
		{
			return mResult;
		}
		
		evaluator.reserve(evaluator.size() + mMaxAvailable);
		
		ECItr end = mStack.end();
		for(ECItr i = mStack.begin(); i < end; ++i)
		{
			evaluator.push_back((*i)->evaluate(evaluator));
		}
		
		double ret = evaluator.pop();
		
		if(!mIsVolatile)
		{
			mResult = ret;
			mIsCached = true;
		}
		
		return ret;
	}
	
	void Expression::parse(const std::string& string, const Context& context, Format format)
	{
		if(format == INFIX)
		{
			InfixParser parser(string, context);
			parser.store(*this);
		}
		else if(format == POSTFIX)
		{
			PostfixParser parser(string, context);
			parser.store(*this);
		}
		else
		{
			std::ostringstream mess;
			mess << "Unknown format: " << format;
			throw Exception(mess.str());
		}
	}
	
	Expression& Expression::operator <<(const Node* node)
	{
		node->reference();
		mStack.push_back(node);
		mIsVolatile |= node->isVolatile();
		return *this;
	}
}
