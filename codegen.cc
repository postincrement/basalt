#include <ostream>

using namespace std;

#include "ast.h"
#include "codegen.h"

using namespace AST;

CodeGeneratorBase::CodeGeneratorBase(const std::string & genType, const std::string & fn, SourceFileExprList & tree)
  : Visitor(tree)
  , m_inputFilename(fn)
  , m_genType(genType)
  , m_currentScope(nullptr)
{
}

bool CodeGeneratorBase::Open()
{
  m_globalScope  = CreateScope(*this);
  m_currentScope = nullptr;
  return true;
}

std::string CodeGeneratorBase::GetTempName(const std::string & prefix)
{
  std::stringstream strm;
  strm << prefix << "_" << m_tempCounter++;
  return strm.str();
}

///////////////////////////////////////////////////////////////////////

VarDefExprBase * ScopeBase::FindVar(const std::string & name)
{
  auto r = m_vars.find(name);
  if (r != m_vars.end())
    return r->second.get();

  if (m_parent == nullptr)
    return nullptr;
    
  return m_parent->FindVar(name);  
}

///////////////////////////////////////////////////////////////////////

bool CodeGeneratorBase::CallRuntimeFunction(const std::string & name ...)
{
#if 0  
  // always declare before calling
  if (!DeclareRuntimeFunction(name))
    return false;

  std::string funcName(g_runtimeDefPrefix);
  funcName += name;

  va_list argValues;
  va_start(argValues, name);

  int i = 0;
  while (g_runtimeDefs[i].m_name != nullptr) {
    RuntimeFunctionDef & funcDef = g_runtimeDefs[i];
    if (name == funcDef.m_name) {
      return VCreateFunctionCall(funcDef.m_returnType, funcName.c_str(), funcDef.m_args, argValues);
    }
    ++i;
  }

  InternalError("unknown runtime function '" << name << "'");
  return false;
#endif

  return true;  
}



#if 0

bool CodeGeneratorBase::DeclareExternalFunction(const FunctionDef & fn)
{
  auto r = m_externFunctionMap.find(fn.m_name);
  if (r != m_externFunctionMap.end())
    return true;
  m_externFunctionMap[fn.m_name] = fn;
  return OnDeclareExternalFunc(fn);
}

bool CodeGeneratorBase::DeclareRuntimeFunction(const std::string & name)
{
  std::string funcName(g_runtimeDefPrefix);
  funcName += name;

  int i = 0;
  while (g_runtimeDefs[i].m_name != nullptr) {
    RuntimeFunctionDef & funcDef = g_runtimeDefs[i];
    if (name == funcDef.m_name) {
      FunctionDef newDef(funcDef);
      newDef.m_name = funcName;
      return DeclareExternalFunction(newDef);
    }
    ++i;
  }
  
  InternalError("unknown runtime function '" << name << "'");
  return false;
}

bool CodeGeneratorBase::VCreateFunctionCall(
  const char * returnTypeStr_,
  const char * name,
  const char * argsStr_,
  va_list varg)
{
  std::vector<std::string> args;

  if (argsStr_ != nullptr) {    
    std::string argStr(argsStr_);
    std::vector<std::string> tokens;
    Tokenize(tokens, argStr, ',');
    for (size_t i = 0; i < tokens.size(); ++i) {
      const char * strVal = va_arg(varg, const char *);
      args.push_back(std::string(strVal));
    }
  }

  std::string returnType;
  if (returnTypeStr_ != nullptr)
    returnType = returnTypeStr_;

  return CallFunction(returnType, name, args);
}

#endif

///////////////////////////////////////////////////////////////////////

void CodeGeneratorBase::EnterScope()
{
  m_currentScope = CreateScope(*this, m_currentScope);  
  m_currentScope->Enter();
}

void CodeGeneratorBase::LeaveScope()
{
  if (m_currentScope == nullptr)
    InternalError("Attempt to pop past global scope");
  m_currentScope->Leave();
  ScopeBase * parent = m_currentScope->m_parent;
  delete m_currentScope;
  m_currentScope = parent;  
}


bool CodeGeneratorBase::Run(AST::SourceFileExprList & expr)
{
  m_currentLineMarkerExpr = nullptr;

  EnterScope();
  
  // call init function
  if (!CallRuntimeFunction("init"))
    return false;

  // output code
  for (auto & r : expr) {
    if (r != nullptr) {
      bool result = r->Generate(*this);
      if (!result)
        break;
    }
  }

  LeaveScope();

  return true;
}

#if 0

bool CodeGeneratorBase::Visit(LineMarkerExpr & expr)
{
  m_currentLineMarkerExpr = &expr;
  OnLineMarker(*m_currentLineMarkerExpr);
  return true;
}

bool CodeGeneratorBase::Visit(ConstantStringExpr & expr)
{
  std::string name;
  auto r = m_constStringMap.find(expr.m_value);
  if (r != m_constStringMap.end()) 
    name = r->second;
  else {  
    name = GetTempName("string");
    m_constStringMap[expr.m_value] = name;
    OnDeclareConstString(expr.m_value, name);
  }

  auto val = CreateValueDef(name, Variable::Type::eString);
  m_valueStack.push_back(val);

  return true;
}

bool CodeGeneratorBase::Visit(VariableDefExpr & expr)
{
  ScopeBase * scope = expr.m_global ? m_globalScope : m_currentScope;
  if (scope->FindVar(expr.m_variable.m_normalizedName) == nullptr) {
    if (!OnDeclareVar(expr, *scope))
      return false;
  }

  return scope->ReferenceVar(expr);
}

bool CodeGeneratorBase::Visit(BinaryExpr & expr)
{
  if (expr.m_rhs == nullptr) {
    InternalError("RHS of assignment missing");
    return false;
  }
  if (expr.m_lhs == nullptr) {
    InternalError("LHS of assignment missing");
    return false;
  }

  // evalaute RHS first
  cout << "expr stack for " << expr.m_op << " starts with " << m_valueStack.size() << " values" << endl;
  if (!expr.m_rhs->Generate(*this)) {
    InternalError("RHS of binary expression did not evaluate");
    return false;
  }

  cout << "after eval of rhs of binary expression " << expr.m_op << " : stack has " << m_valueStack.size() << " entries" << endl;
  
  // get RHS
  std::unique_ptr<ValueDefBase> rhs(m_valueStack.back());  
  m_valueStack.pop_back();
  Variable::Type rhType = rhs->m_type;
  
  // evalaute LHS
  expr.m_lhs->Generate(*this);

  // get LHS
  std::unique_ptr<ValueDefBase> lhs(m_valueStack.back());
  m_valueStack.pop_back(); 
  Variable::Type lhType = lhs->m_type;

  // evaluate expression
  bool sameType = (lhType == rhType);
  if (!sameType) { 
    InternalError("not yet supporting mixed type expressions");
    return false;
  }
    
  if (expr.m_op == '=') {

    // LHS must be a variable ref
    AST::VariableRefExpr * lhRef = dynamic_cast<AST::VariableRefExpr *>(expr.m_lhs);
    if (lhRef == nullptr) {
      InternalError("LHS of assignment is not variable ref (" << typeid(lhRef).name());
      return false;
    }

    // strings need to be handled differently
    if (rhType == Variable::eString) {
      if (lhRef->m_variable.m_type != Variable::eString) {
        InternalError("cannot assign string to non-string");
        return false;
      }
      else if (lhRef->m_variable.m_normalizedName != rhs->m_name) {
        AssignString(lhRef->m_variable.m_normalizedName, rhs->m_name);
      }
    }
    else {
      AssignVar(*lhs, *rhs);
    }

    return true;
  }

  // handle string binary ops  
  if (lhType == Variable::eString) {
    switch (expr.m_op) {
      case '+':
        JoinStrings(lhs->m_name, rhs->m_name);
        break;
      default:
        SourceWarning(eWarning_UnsupportedStringOp, "unsupported string op '" << (int)expr.m_op << "'");
    }
    return true;
  }  

  //   handle scalar binary ops  
  std::string op;
  switch (expr.m_op) {
    case '+':
    case '-':
    case '*':
    case '/':
      op = expr.m_op;
      break;
    default:
      InternalError("RHS of binary expression '" << expr.m_op << "' did not evaluate");
      return false;
  }

  if (sameType)
    BinaryOp(*lhs, expr.m_op, *rhs);
  else {
    InternalError("mixed ops not supported");
    return false;
  }
  
  return true;
}

bool CodeGeneratorBase::Visit(BIFExpr & expr)
{
  if (
      (expr.m_name == "print_eol") || 
      (expr.m_name == "print_tab")
    ) {
      CallRuntimeFunction(expr.m_name);
  }
  
  else if (expr.m_name == "print_expr") {
    if ((expr.m_args == nullptr) || (expr.m_args->size() != 1))
      InternalError("print_expr has invalid args");
    else {      
      AST::Expr * printExpr = (*expr.m_args)[0];
      printExpr->Generate(*this);

      // string constant
      AST::ConstantStringExpr * constantString = dynamic_cast<AST::ConstantStringExpr *>(printExpr);
      if (constantString != nullptr) {
        auto r = m_constStringMap.find(constantString->m_value);
        if (r == m_constStringMap.end()) {
          InternalError("cannot find constant string def for '" << constantString->m_value << "'");
        }
        else { 
          CallRuntimeFunction("print_string", r->second.c_str());
        }
        return true;
      }

      // variable reference
      AST::VariableRefExpr * varRef = dynamic_cast<AST::VariableRefExpr *>(printExpr);
      if (varRef != nullptr) {
        std::string func;
        switch (varRef->m_variable.m_type) {
          case Variable::eString:
            func = "print_string";
            break;          
          case Variable::eInt16:
            func = "print_int16";
            break;          
          case Variable::eSingle:
            func = "print_single";
            break;          
          case Variable::eDouble:
            func = "print_double";
            break;          
          case Variable::eUntyped:
            InternalError("unknown print variable type " << varRef->m_variable.m_type);
            exit(1);
        }
        CallRuntimeFunction(func, varRef->m_variable.m_normalizedName.c_str());
        return true;
      }

      // unknown
      else {
        InternalError("unknown print expression type " << typeid(printExpr).name());
        return false;
      }
    }  
  } 
  
  else {
    Warning(eWarning_UnknownCXXBIF, "unknown BIF '" << expr.m_name << "'");
  }

  return true;
}

#endif