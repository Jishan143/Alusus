/**
 * @file Scg/Instructions/CallFunction.cpp
 *
 * @copyright Copyright (C) 2014 Rafid Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <http://alusus.net/alusus_license_1_0>.
 */
//==============================================================================

#include <prerequisites.h>

// STL header files
#include <sstream>

// LLVM header files
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

// Scg header files
#include <Containers/Block.h>
#include <Containers/List.h>
#include <Containers/Module.h>
#include <Containers/Program.h>
#include <Instructions/CallFunction.h>
#include <Instructions/DeclareExtFunction.h>
#include <Instructions/DefineFunction.h>
#include <Functions/Function.h>

using namespace llvm;

namespace Scg
{
const ValueType *CallFunction::GetValueType() const
{
  if (this->function == nullptr) {
    THROW_EXCEPTION(InvalidOperationException, "This function should only "
        "be called after the pre-code generation step has finished, because "
        "we don't have a reference to the function before that.");
  }
  auto funcRetType = this->function->GetReturnType();
  if (funcRetType == nullptr) {
    return GetModule()->GetValueTypeByName("void");
  }
  return funcRetType->ToValueType(*GetModule());
}

//------------------------------------------------------------------------------

Expression::CodeGenerationStage CallFunction::PreGenerateCode()
{
  // Ensures that all children have finished pre-code generation step so that
  // we can grab their types.
  for (auto i = 0; i < this->args->GetElementCount(); i++) {
    auto stage = this->args->GetElement(i)->GetCodeGenerationStage();
    if (stage < CodeGenerationStage::CodeGeneration
        || stage == CodeGenerationStage::None) {
      return CodeGenerationStage::PreCodeGeneration;
    }
  }

  // Finds the types of the arguments.
  if (argTypes.size() != this->args->GetElementCount()) {
    argTypes.resize(this->args->GetElementCount());
    for (auto i = 0; i < this->args->GetElementCount(); i++) {
      argTypes[i] = const_cast<ValueTypeSpec*>(
      		this->args->GetElement(i)->GetValueType()->GetValueTypeSpec());
    }
  }

  // Try to find the function in the same module.
  this->function = GetModule()->MatchFunction(this->funcName, argTypes);
  if (this->function == nullptr) {
  	// We couldn't find it in the current module, search the whole program.
  	auto matches = GetModule()->GetProgram()->MatchFunction(
  			this->funcName, argTypes);
  	switch (matches.size()) {
  		case 0:
  			// We couldn't find the function in the whole program, but it might
  			// not have been generated yet.
  			if (GetModule()->HasFunction(this->funcName, argTypes) ||
  					GetProgram()->HasFunction(this->funcName, argTypes)) {
  				// Yes, wait until it is generated.
          return CodeGenerationStage::PreCodeGeneration;
  			} else {
  				// No match, throw a CompilationErrorException.
          THROW_EXCEPTION(CompilationErrorException,
          		"Calling undefined function " + this->funcName);
  			}
  			break;

  		case 1:
  			// Found the function, use it.
  			this->function = matches[0];
  			break;

  		default:
  			// Found more than one match, throw a CompilationErrorException.
  			// TODO: Improve the exception so that it shows the candidate functions.
  			THROW_EXCEPTION(CompilationErrorException,
  					"Found multiple matches for " + this->funcName);
  	}
  }

  return CodeGenerationStage::CodeGeneration;
}

//------------------------------------------------------------------------------

Expression::CodeGenerationStage CallFunction::GenerateCode()
{
  MODULE_CHECK;
  BLOCK_CHECK;

  // Generate code for the arguments.
  std::vector<llvm::Value*> args;

  // If argument mismatch error.
  // TODO: Check the types of arguments as well.
  if ((this->function->IsVarArgs() &&
          this->function->GetArgumentCount() > GetArguments()->GetElementCount()) ||
      (!this->function->IsVarArgs() &&
          this->function->GetArgumentCount() != GetArguments()->GetElementCount()))
  {
    std::stringstream str;
    str << "Function " << this->funcName << " expects "
        << this->function->GetArgumentCount() << " arguments, but got "
        << GetArguments()->GetElementCount();
    THROW_EXCEPTION(ArgumentMismatchException, str.str());
  }

  for (size_t i = 0, e = GetArguments()->GetElementCount(); i != e; ++i)
  {
    auto expectedArgType = i < this->function->GetArgumentCount() ?
        this->function->GetArgumentTypes()[i]->ToValueType(*GetModule()) :
        nullptr;
    auto argType = GetArguments()->GetElement(i)->GetValueType();
    auto argValue = GetArguments()->GetElement(i)->GetGeneratedLlvmValue();
    if (argValue == nullptr)
      THROW_EXCEPTION(EvaluationException,
          "Expression doesn't evaluate to a value: "
              + GetArguments()->GetElement(i)->ToString());

    if (expectedArgType != nullptr &&
        argType->Compare(expectedArgType) != TypeComparisonResult::Equivalent) {
      // Need to cast the type before sending it to the function.
      args.push_back(argType->CreateCastInst(GetBlock()->GetIRBuilder(),
          argValue, expectedArgType));
    } else {
      args.push_back(argValue);
    }
  }

  // Generate code for calling the function.
  this->generatedLlvmValue = this->callInst =
  		GetBlock()->GetIRBuilder()->CreateCall(this->function->GetLlvmFunction(), args);

  return Expression::GenerateCode();
}

//------------------------------------------------------------------------------

Expression::CodeGenerationStage CallFunction::PostGenerateCode()
{
  this->function = nullptr;
  SAFE_DELETE_LLVM_INST(this->callInst);
  return CodeGenerationStage::None;
}

//------------------------------------------------------------------------------

std::string CallFunction::ToString()
{
  return this->funcName + GetArguments()->ToString();
}
}
