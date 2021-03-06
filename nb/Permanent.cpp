//==============================================================================
//
//  Permanent.cpp
//
//  Copyright (C) 2017  Greg Utas
//
//  This file is part of the Robust Services Core (RSC).
//
//  RSC is free software: you can redistribute it and/or modify it under the
//  terms of the GNU General Public License as published by the Free Software
//  Foundation, either version 3 of the License, or (at your option) any later
//  version.
//
//  RSC is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
//  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
//  details.
//
//  You should have received a copy of the GNU General Public License along
//  with RSC.  If not, see <http://www.gnu.org/licenses/>.
//
#include "Permanent.h"
#include "Debug.h"
#include "Memory.h"

//------------------------------------------------------------------------------

namespace NodeBase
{
fn_name Permanent_ctor = "Permanent.ctor";

Permanent::Permanent()
{
   Debug::ft(Permanent_ctor);
}

//------------------------------------------------------------------------------

fn_name Permanent_delete1 = "Permanent.operator delete";

void Permanent::operator delete(void* addr)
{
   Debug::ftnt(Permanent_delete1);

   Memory::Free(addr, MemPermanent);
}

//------------------------------------------------------------------------------

fn_name Permanent_delete2 = "Permanent.operator delete[]";

void Permanent::operator delete[](void* addr)
{
   Debug::ftnt(Permanent_delete2);

   Memory::Free(addr, MemPermanent);
}

//------------------------------------------------------------------------------

fn_name Permanent_delete3 = "Permanent.operator delete(place)";

void Permanent::operator delete(void* addr, void* place) noexcept
{
   Debug::ftnt(Permanent_delete3);
}

//------------------------------------------------------------------------------

fn_name Permanent_delete4 = "Permanent.operator delete[](place)";

void Permanent::operator delete[](void* addr, void* place) noexcept
{
   Debug::ftnt(Permanent_delete4);
}

//------------------------------------------------------------------------------

fn_name Permanent_new1 = "Permanent.operator new";

void* Permanent::operator new(size_t size)
{
   Debug::ft(Permanent_new1);

   return Memory::Alloc(size, MemPermanent);
}

//------------------------------------------------------------------------------

fn_name Permanent_new2 = "Permanent.operator new[]";

void* Permanent::operator new[](size_t size)
{
   Debug::ft(Permanent_new2);

   return Memory::Alloc(size, MemPermanent);
}

//------------------------------------------------------------------------------

fn_name Permanent_new3 = "Permanent.operator new(place)";

void* Permanent::operator new(size_t size, void* place)
{
   Debug::ft(Permanent_new3);

   return place;
}

//------------------------------------------------------------------------------

fn_name Permanent_new4 = "Permanent.operator new[](place)";

void* Permanent::operator new[](size_t size, void* place)
{
   Debug::ft(Permanent_new4);

   return place;
}

//------------------------------------------------------------------------------

void Permanent::Patch(sel_t selector, void* arguments)
{
   Object::Patch(selector, arguments);
}
}