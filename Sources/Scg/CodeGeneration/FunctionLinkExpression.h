/**
 * @file Scg/CodeGeneration/FunctionLinkExpression.h
 *
 * @copyright Copyright (C) 2016 Rafid Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <http://alusus.net/alusus_license_1_0>.
 */
//==============================================================================

#ifndef SCG_FUNCTIONLINKEXPRESSION_H
#define SCG_FUNCTIONLINKEXPRESSION_H

// STL header files
#include <vector>

// Alusus Core header files
#include <core.h>

// Alusus SCG header files
#include <CodeGeneration/FunctionalExpression.h>
#include <CodeGeneration/typedefs.h>
#include <exceptions.h>
#include <Types/ValueTypeSpec.h>

namespace Scg
{
class CodeGenerator;
class DeclareExtFunction;
class FunctionalExpression;

/**
 * Parses an AST block representing a function link.
 *
 * An example of an AST block is shown below, which is generated
 * from the following line of code:
 *
 * test(ptr[int], string, string) : ptr[int];
 *
 * Expression.Exp -- [LIST]:
 *  Expression.LowerLinkExp -- [LIST]:
 *   Expression.FunctionalExp -- [LIST]:
 *    Subject.Subject1 -- [ROUTE]: 0
 *     Subject.Parameter -- [TOKEN]: IDENTIFIER_TOKEN ("test")
 *    Expression.ParamPassExp -- [ROUTE]: 0
 *     Expression.Exp -- [LIST]:
 *      Expression.ListExp -- [LIST]:
 *       Expression.FunctionalExp -- [LIST]:
 *        Subject.Subject1 -- [ROUTE]: 0
 *         Subject.Parameter -- [TOKEN]: IDENTIFIER_TOKEN ("ptr")
 *        Expression.ParamPassExp -- [ROUTE]: 1
 *         Expression.Exp -- [LIST]:
 *          Subject.Subject1 -- [ROUTE]: 0
 *           Subject.Parameter -- [TOKEN]: IDENTIFIER_TOKEN ("int")
 *       Subject.Subject1 -- [ROUTE]: 0
 *        Subject.Parameter -- [TOKEN]: IDENTIFIER_TOKEN ("string")
 *       Subject.Subject1 -- [ROUTE]: 0
 *        Subject.Parameter -- [TOKEN]: IDENTIFIER_TOKEN ("string")
 *   [TOKEN]: CONSTANT_59 (":")
 *   Expression.FunctionalExp -- [LIST]:
 *    Subject.Subject1 -- [ROUTE]: 0
 *     Subject.Parameter -- [TOKEN]: IDENTIFIER_TOKEN ("ptr")
 *    Expression.ParamPassExp -- [ROUTE]: 1
 *     Expression.Exp -- [LIST]:
 *      Subject.Subject1 -- [ROUTE]: 0
 *       Subject.Parameter -- [TOKEN]: IDENTIFIER_TOKEN ("int")
 */
class FunctionLinkExpression
{
private:
  SharedPtr<ValueTypeSpec> retType;
  Core::Data::SourceLocationRecord sourceLocation;
  SharedPtr<FunctionalExpression> arguments;

public:
  FunctionLinkExpression(CodeGenerator *gen,
                         const Core::Basic::SharedPtr<Core::Basic::TiObject> &item);

  /**
   * Returns the return type name of the function we are linking to.
   * @return A string containing the return type name.
   */
  SharedPtr<ValueTypeSpec> const& getReturnType() const
  {
    return this->retType;
  }

  /**
   * Converts the function link expression to a DeclareExtFunction expression.
   *
   * @return A pointer to a DeclareExtFunction object. The object should be
   * freed by the caller.
   */
  SharedPtr<DeclareExtFunction> toDeclareExtFunction();
};

} // namespace

#endif
