/**
 * @file Spp/Handlers/TypeParsingHandler.h
 *
 * @copyright Copyright (C) 2019 Sarmad Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <https://alusus.org/alusus_license_1_0>.
 */
//==============================================================================

#ifndef SPP_HANDLERS_TYPEPARSINGHANDLER_H
#define SPP_HANDLERS_TYPEPARSINGHANDLER_H

namespace Spp::Handlers
{

class TypeParsingHandler : public Core::Processing::Handlers::GenericParsingHandler
{
  //============================================================================
  // Type Info

  TYPE_INFO(TypeParsingHandler, Core::Processing::Handlers::GenericParsingHandler,
            "Spp.Handlers", "Spp", "alusus.org");


  //============================================================================
  // Constructor

  public: TypeParsingHandler()
  {
  }


  //============================================================================
  // Member Functions

  public: virtual void onProdEnd(Core::Processing::Parser *parser, Core::Processing::ParserState *state);

  private: Bool parseTemplateArgs(
    Processing::ParserState *state, Core::Data::Ast::Bracket *bracket, SharedPtr<Core::Data::Ast::List> &result
  );

  private: Bool parseTemplateArg(
    Core::Processing::ParserState *state, TiObject *astNode, SharedPtr<Core::Data::Ast::List> const &result
  );

  public: virtual Bool onIncomingModifier(
    Core::Processing::Parser *parser, Core::Processing::ParserState *state,
    TioSharedPtr const &modifierData, Bool prodProcessingComplete
  );

}; // class

} // namespace

#endif
