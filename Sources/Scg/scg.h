/**
 * @file Scg/scg.h
 *
 * @copyright Copyright (C) 2014 Rafid Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <http://alusus.net/alusus_license_1_0>.
 */
//==============================================================================

#ifndef __scg_h__
#define __scg_h__

#include <prerequisites.h>
#include <precompiled_headers.h>

#include <CodeGeneration/CodeGenerator.h>
#include <Containers/Block.h>
#include <Containers/List.h>
#include <Containers/Module.h>
#include <exceptions.h>
#include <AstNode.h>
#include <CodeGenUnit.h>
#include <Instructions/CallFunction.h>
#include <Instructions/CondGotoStatement.h>
#include <Instructions/DeclareExtFunction.h>
#include <Instructions/DefineFunction.h>
#include <Instructions/DefineStruct.h>
#include <Instructions/DefineVariable.h>
#include <Instructions/ForStatement.h>
#include <Instructions/GotoStatement.h>
#include <Instructions/IfStatement.h>
#include <Instructions/Instruction.h>
#include <Instructions/Return.h>
#include <Instructions/WhileStatement.h>
#include <LibraryGateway.h>
#include <LlvmContainer.h>
#include <llvm_fwd.h>
#include <Logger.h>
#include <macros.h>
#include <Operators/AssignmentOperator.h>
#include <Operators/BinaryOperator.h>
#include <Operators/Content.h>
#include <Operators/ArrayElementReference.h>
#include <Operators/MemberFieldReference.h>
#include <Operators/IdentifierReference.h>
#include <Operators/UnaryOperator.h>
#include <Operators/Size.h>
#include <ParsingHandlers/BuildParsingHandler.h>
#include <ParsingHandlers/DefParsingHandler.h>
#include <ParsingHandlers/DumpParsingHandler.h>
#include <ParsingHandlers/ModuleParsingHandler.h>
#include <ParsingHandlers/RunParsingHandler.h>
#include <typedefs.h>
#include <Types/ArrayType.h>
#include <Types/DoubleType.h>
#include <Types/FloatType.h>
#include <Types/IntegerType.h>
#include <Types/PointerType.h>
#include <Types/StringType.h>
#include <Types/StructType.h>
#include <Types/ValueType.h>
#include <Types/VoidType.h>
#include <Values/Constant.h>
#include <Values/DoubleConst.h>
#include <Values/FloatConst.h>
#include <Values/IntegerConst.h>
#include <Values/StringConst.h>
#include <Values/Value.h>
#include <Values/Variable.h>

#endif // __scg_h__
