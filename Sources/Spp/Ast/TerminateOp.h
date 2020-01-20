/**
 * @file Spp/Ast/TerminateOp.h
 * Contains the header of class Spp::Ast::TerminateOp.
 *
 * @copyright Copyright (C) 2020 Sarmad Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <https://alusus.org/alusus_license_1_0>.
 */
//==============================================================================

#ifndef SPP_AST_TERMINATEOP_H
#define SPP_AST_TERMINATEOP_H

namespace Spp::Ast
{

class TerminateOp : public Core::Data::Node,
                    public Binding, public MapContaining<TiObject>,
                    public Core::Data::Ast::MetaHaving, public Core::Data::Printable
{
  //============================================================================
  // Type Info

  TYPE_INFO(TerminateOp, Core::Data::Node, "Spp.Ast", "Spp", "alusus.org");
  IMPLEMENT_INTERFACES(
    Core::Data::Node, Binding, MapContaining<TiObject>,
    Core::Data::Ast::MetaHaving, Core::Data::Printable
  );
  OBJECT_FACTORY(TerminateOp);


  //============================================================================
  // Member Variables

  private: TioSharedPtr operand;


  //============================================================================
  // Implementations

  IMPLEMENT_METAHAVING(TerminateOp);

  IMPLEMENT_BINDING(Binding,
    (prodId, TiWord, VALUE, setProdId(value), &prodId),
    (sourceLocation, Core::Data::SourceLocation, SHARED_REF, setSourceLocation(value), sourceLocation.get())
  );

  IMPLEMENT_MAP_CONTAINING(MapContaining<TiObject>,
    (operand, TiObject, SHARED_REF, setOperand(value), operand.get())
  );

  IMPLEMENT_AST_MAP_PRINTABLE(TerminateOp);


  //============================================================================
  // Constructors & Destructor

  IMPLEMENT_EMPTY_CONSTRUCTOR(TerminateOp);

  IMPLEMENT_ATTR_CONSTRUCTOR(TerminateOp);

  IMPLEMENT_ATTR_MAP_CONSTRUCTOR(TerminateOp);

  public: virtual ~TerminateOp()
  {
    DISOWN_SHAREDPTR(this->operand);
  }


  //============================================================================
  // Member Functions

  public: void setOperand(TioSharedPtr const &o)
  {
    UPDATE_OWNED_SHAREDPTR(this->operand, o);
  }
  private: void setOperand(TiObject *o)
  {
    this->setOperand(getSharedPtr(o));
  }

  public: TioSharedPtr const& getOperand() const
  {
    return this->operand;
  }

}; // class

} // namespace

#endif
