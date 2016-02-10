#pragma once

namespace piggy
{
	class type
	{
	public:
		int kind;
		int size;
		int align;
		bool nosign; //unsigned
	};
}