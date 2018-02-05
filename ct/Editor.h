//==============================================================================
//
//  Editor.h
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
#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

#include <cstddef>
#include <list>
#include <string>
#include "CxxFwd.h"
#include "SysFile.h"
#include "SysTypes.h"

using namespace NodeBase;

//------------------------------------------------------------------------------

namespace CodeTools
{
//  Source code editor.
//
//  A source code file must be laid out as follows:
//
//  <heading>         file name and copyright notice
//  <blank-line>
//  <#include-guard>  for a .h
//  <#include-bases>  #includes for baseIds_ and declIds_
//  <#include-exts>   #includes for other external headers
//  <#include-ints>   #includes for other internal headers
//  <blank-line>
//  <forwards>        forward declarations
//  <blank-line>
//  <usings>          using statements
//  <blank-line>
//  <rule>            single rule (or double rule)
//  <blank-line>
//  <code>            declarations and/or definitions
//
class Editor
{
private:
   struct SourceLine
   {
      //  Stores a line of code.
      //
      SourceLine(const std::string& code, size_t line) :
         code(code), line(line) { }

      //  The code.
      //
      std::string code;

      //  Its line number (the first line is #1, as in a *.check.txt file).
      //
      const size_t line;
   };

   //  The code is kept in a list.
   //
   typedef std::list< SourceLine > SourceList;

   //  Iterator for the source code.  This is an editor, so it doesn't bother
   //  with const iterators.
   //
   typedef std::list< SourceLine >::iterator Iter;
public:
   //  Creates an editor for the source code in FILE, which is read from INPUT.
   //
   Editor(CodeFile* file, istreamPtr& input);

   //  All of the public editing functions attempt to fix the warning reported
   //  in LOG.  They return 0 on success.  Any other result indicates an error,
   //  in which case EXPL provides an explanation.  A return value of -1 means
   //  that the file should be skipped; other values denote more serious errors.
   //
   word Read(std::string& expl);
   word SortIncludes(const WarningLog& log, std::string& expl);
   word AddInclude(const WarningLog& log, std::string& expl);
   word RemoveInclude(const WarningLog& log, std::string& expl);
   word AddForward(const WarningLog& log, std::string& expl);
   word RemoveForward(const WarningLog& log, std::string& expl);
   word AddUsing(const WarningLog& log, std::string& expl);
   word RemoveUsing(const WarningLog& log, std::string& expl);

   //  Replaces multiple blank lines with a single blank line.  Always invoked
   //  on source that was changed.
   //
   word EraseBlankLinePairs();

   //  Removes trailing spaces.  Always invoked on source that was changed.
   //
   void EraseTrailingBlanks(SourceList& list);
   word EraseTrailingBlanks();

   //  Writes out the file to PATH if it was changed during editing.  Returns 0
   //  if the file had not been changed, 1 if it was successfully written, and a
   //  negative value if an error occurred.
   //
   word Write(const std::string& path, std::string& expl);
private:
   //  Reads in the file's prolog (everything up to the first #include.)
   //
   word GetProlog(std::string& expl);

   //  Reads in the remaining #include directives.
   //
   word GetIncludes(std::string& expl);

   //  Reads in the rest of the file.
   //
   word GetEpilog();

   //  Adds a line of source code from the file.  NEVER used to add new code.
   //
   void PushBack(SourceList& list, const std::string& source);

   //  Adds an #include from the file.  NEVER used to add a new #include.
   //
   word PushInclude(std::string& source, std::string& expl);

   //  Inserts SOURCE into LIST at ITER.  Its new location is returned.
   //
   Iter Insert(SourceList& list, Iter& iter, const std::string& source);

   //  If LIST contains LINE from the original source, erases that line and
   //  returns its location (it is now an empty string).  Returns end() if
   //  LINE was not found.
   //
   Iter Erase(SourceList& list, size_t line, std::string& expl);

   //  If LIST contains a line of code that matches SOURCE, erase that line
   //  and returns its location (it is now an empty string).  Returns end() if
   //  a match was not found.
   //
   Iter Erase(SourceList& list, const std::string& source, std::string& expl);

   //  If INCLUDE specifies a file in groups 1 to 4 (see CodeFile.CalcGroup),
   //  this simplifies sorting by replacing the characters that enclose the
   //  file name.
   //
   word MangleInclude(std::string& include, std::string& expl) const;

   //  Inserts an INCLUDE directive.
   //
   word InsertInclude(std::string& include, std::string& expl);

   //  Inserts a FORWARD declaration at ITER.
   //
   word InsertForward
      (const Iter& iter, const std::string& forward, std::string& expl);

   //  Inserts a FORWARD declaration at ITER.  It is the first declaration in
   //  namespace NSPACE, so it must be enclosed in a new namespace scope.
   //
   word InsertNamespaceForward(Iter& iter,
      const std::string& nspace, const std::string& forward);

   //  Invoked after removing a forward declaration.  If the declaration was
   //  in a namespace that is now empty, erases the "namespace <name> { }".
   //
   word EraseEmptyNamespace(const Iter& iter);

   //  Invoked to report TEXT, which is assigned to EXPL.  Returns RC.
   //
   static word Report(std::string& expl, fixed_string text, word rc = 0);

   //  Comparison functions for sorting #include directives.
   //
   static bool IsSorted1(const SourceLine& line1, const SourceLine& line2);
   static bool IsSorted2(const std::string& line1, const std::string& line2);

   //  The file from which the source code was obtained.
   //
   CodeFile* file_;

   //  The stream for reading the source code.
   //
   istreamPtr input_;

   //  The number of lines read so far.
   //
   size_t line_;

   //  Set if the source code has been altered.
   //
   bool changed_;

   //  The lines of source that precede the first #include directive.
   //
   SourceList prolog_;

   //  The #include directives.
   //
   SourceList includes_;

   //  The rest of the source code.
   //
   SourceList epilog_;

   //  Characters that enclose the file name in an #include directive,
   //  depending on the group to which it belongs.
   //
   static const std::string FrontChars;
   static const std::string BackChars;
};
}
#endif