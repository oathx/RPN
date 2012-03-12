/******************************************************************************
   Copyright 2012 Kevin Burk
   
   This file is part of RPN.
  
   RPN is free software: you can redistribute it and/or modify it under the
   terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.
  
   RPN is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
   details.
  
   You should have received a copy of the GNU General Public License along
   with RPN.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include "../parsers/infix.h"
#include "comma.h"

namespace RPN
{
	CommaNode::CommaNode(): Node()
	{
		//Nothing else to do...
	}
	
	Node::Flags CommaNode::flags() const
	{
		return Node::Flags(Node::PRESENTS_OP | Node::SUCCEEDS_OP | Node::INFIX);
	}
	
	void CommaNode::infixParse(InfixParser& parser, Parser::Token& token) const
	{
		(void)(token); //Unused
		
		while(parser.hasStack())
		{
			if(parser.top().node->isBracket())
			{
				return;
			}
			else
			{
				parser.shunt();
			}
		}
	}
}

