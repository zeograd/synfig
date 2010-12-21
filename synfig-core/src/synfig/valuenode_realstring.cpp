/* === S Y N F I G ========================================================= */
/*!	\file valuenode_realstring.cpp
**	\brief Implementation of the "RealString" valuenode conversion.
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2008 Chris Moore
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "valuenode_realstring.h"
#include "valuenode_const.h"
#include "canvas.h"
#include "general.h"

#endif

/* === U S I N G =========================================================== */

using namespace std;
using namespace etl;
using namespace synfig;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

ValueNode_RealString::ValueNode_RealString(const ValueBase &value):
	LinkableValueNode(value.get_type())
{
	switch(value.get_type())
	{
	case ValueBase::TYPE_STRING:
		set_link("real",ValueNode_Const::create(Real(0)));
		set_link("width",ValueNode_Const::create(int(0)));
		set_link("precision",ValueNode_Const::create(int(3)));
		set_link("zero_pad",ValueNode_Const::create(bool(false)));
		break;
	default:
		throw Exception::BadType(ValueBase::type_local_name(value.get_type()));
	}
}

LinkableValueNode*
ValueNode_RealString::create_new()const
{
	return new ValueNode_RealString(get_type());
}

ValueNode_RealString*
ValueNode_RealString::create(const ValueBase &x)
{
	return new ValueNode_RealString(x);
}

ValueNode_RealString::~ValueNode_RealString()
{
	unlink_all();
}

ValueBase
ValueNode_RealString::operator()(Time t)const
{
	if (getenv("SYNFIG_DEBUG_VALUENODE_OPERATORS"))
		printf("%s:%d operator()\n", __FILE__, __LINE__);

	Real real((*real_)(t).get(Real()));
	int width((*width_)(t).get(int()));
	int precision((*precision_)(t).get(int()));
	int zero_pad((*zero_pad_)(t).get(bool()));

	switch (get_type())
	{
	case ValueBase::TYPE_STRING:
		return strprintf(strprintf("%%%s%d.%df",
								   zero_pad ? "0" : "",
								   width,
								   precision).c_str(), real);
	default:
		break;
	}

	assert(0);
	return ValueBase();
}

String
ValueNode_RealString::get_name()const
{
	return "realstring";
}

String
ValueNode_RealString::get_local_name()const
{
	return _("Real String");
}

bool
ValueNode_RealString::set_link_vfunc(int i,ValueNode::Handle value)
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
	case 0: CHECK_TYPE_AND_SET_VALUE(real_, ValueBase::TYPE_REAL);
	case 1: CHECK_TYPE_AND_SET_VALUE(width_, ValueBase::TYPE_INTEGER);
	case 2: CHECK_TYPE_AND_SET_VALUE(precision_, ValueBase::TYPE_INTEGER);
	case 3: CHECK_TYPE_AND_SET_VALUE(zero_pad_, ValueBase::TYPE_BOOL);
	}
	return false;
}

ValueNode::LooseHandle
ValueNode_RealString::get_link_vfunc(int i)const
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
	case 0: return real_;
	case 1: return width_;
	case 2: return precision_;
	case 3: return zero_pad_;
	}

	return 0;
}

int
ValueNode_RealString::link_count()const
{
	return 4;
}

String
ValueNode_RealString::link_name(int i)const
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
		case 0: return "real";
		case 1: return "width";
		case 2: return "precision";
		case 3: return "zero_pad";
	}
	return String();
}

String
ValueNode_RealString::link_local_name(int i)const
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
		case 0: return _("Real");
		case 1: return _("Width");
		case 2: return _("Precision");
		case 3: return _("Zero Padded");
	}
	return String();
}

int
ValueNode_RealString::get_link_index_from_name(const String &name)const
{
	if (name=="real") return 0;
	if (name=="width") return 1;
	if (name=="precision") return 2;
	if (name=="zero_pad") return 3;

	throw Exception::BadLinkName(name);
}

bool
ValueNode_RealString::check_type(ValueBase::Type type)
{
	return
		type==ValueBase::TYPE_STRING;
}

LinkableValueNode::Vocab
ValueNode_RealString::get_children_vocab_vfunc()const
{
	LinkableValueNode::Vocab ret;

	ret.push_back(ParamDesc(ValueBase(),"real")
		.set_local_name(_("Real"))
		.set_description(_("Value to convert to string"))
	);

	ret.push_back(ParamDesc(ValueBase(),"width")
		.set_local_name(_("Width"))
		.set_description(_("Width of the string"))
	);

	ret.push_back(ParamDesc(ValueBase(),"precision")
		.set_local_name(_("Precision"))
		.set_description(_("Number of decimal places"))
	);

	ret.push_back(ParamDesc(ValueBase(),"zero_pad")
		.set_local_name(_("Zero Padded"))
		.set_description(_("When checked, the string is left filled with zeros to match the width"))
	);


	return ret;
}
