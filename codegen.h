#ifndef CODEGEN_H_
#define CODEGEN_H_

#include <cctype>
#include <deque>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "ast.h"
#include "errorcode.h"

#define CompilerError(code, ln, expr) \
do { std::stringstream strm; strm << expr; \
  CompilerErrorInternal(ErrorCode::code, ln, strm.str()); \
} while (0)

#define InternalError(expr) \
do { std::stringstream strm; strm << expr; \
  g_application.InternalErrorInternal(ErrorCode::eInternalError, strm.str()); \
} while (0)

class OutputGenerator;

// Walks the AST once and appends a flat list of nodes. Every backend renders
// that list; it does not walk the AST itself.
class CodeGenerator
{
  public:
    struct Node {
      virtual ~Node() = default;
      virtual int Generate(OutputGenerator & gen) = 0;
      std::string m_func;
    };

    struct BlockStart : Node { int Generate(OutputGenerator & gen) override; };
    struct BlockEnd : Node { int Generate(OutputGenerator & gen) override; };
    struct GotoTarget : Node {
      explicit GotoTarget(std::string label) : m_value(std::move(label)) {}
      int Generate(OutputGenerator & gen) override;
      std::string m_value;
    };
    struct Goto : Node {
      explicit Goto(std::string label) : m_value(std::move(label)) {}
      int Generate(OutputGenerator & gen) override;
      std::string m_value;
    };
    struct Gosub : Node {
      explicit Gosub(std::string line) : m_value(std::move(line)) {}
      int Generate(OutputGenerator & gen) override;
      std::string m_value;
    };
    struct Return : Node { int Generate(OutputGenerator & gen) override; };
    struct End : Node { int Generate(OutputGenerator & gen) override; End() { m_func = "end"; } };
    struct System : Node { int Generate(OutputGenerator & gen) override; System() { m_func = "end"; } };
    struct If : Node {
      If(std::string cond, VarType type) : m_cond(std::move(cond)), m_type(type) {}
      int Generate(OutputGenerator & gen) override;
      std::string m_cond;
      VarType m_type;
    };
    struct Else : Node { int Generate(OutputGenerator & gen) override; };
    struct EndIf : Node { int Generate(OutputGenerator & gen) override; };
    struct CreateTempVar : Node {
      CreateTempVar(VarType type, std::string name)
        : m_type(type), m_value(std::move(name)) {}
      int Generate(OutputGenerator & gen) override;
      VarType m_type;
      std::string m_value;
    };
    struct PrintNewLine : Node {
      PrintNewLine() { m_func = "print_newline"; }
      int Generate(OutputGenerator & gen) override;
    };
    struct PrintTab : Node {
      PrintTab() { m_func = "print_tab"; }
      int Generate(OutputGenerator & gen) override;
    };
    struct PrintStringConst : Node {
      explicit PrintStringConst(std::string value) : m_value(std::move(value)) { m_func = "print_string"; }
      int Generate(OutputGenerator & gen) override;
      std::string m_value;
    };
    struct PrintStringVar : Node {
      explicit PrintStringVar(std::string value) : m_value(std::move(value)) { m_func = "print_string"; }
      int Generate(OutputGenerator & gen) override;
      std::string m_value;
    };
    struct PrintNumber : Node {
      PrintNumber(VarType type, std::string value, bool literal)
        : m_type(type), m_value(std::move(value)), m_literal(literal)
      {
        switch (type) {
          case VarType::eInt32: m_func = "print_int32"; break;
          case VarType::eSingle: m_func = "print_single"; break;
          case VarType::eDouble: m_func = "print_double"; break;
          default: m_func = "print_int16"; break;
        }
      }
      int Generate(OutputGenerator & gen) override;
      VarType m_type;
      std::string m_value;
      bool m_literal;
    };
    struct UnaryOperator : Node {
      UnaryOperator(std::string func, VarType type, std::string ret, std::string arg)
        : m_type(type), m_ret(std::move(ret)), m_arg(std::move(arg))
      { m_func = std::move(func); }
      int Generate(OutputGenerator & gen) override;
      VarType m_type;
      std::string m_ret;
      std::string m_arg;
    };
    struct BinaryOperator : Node {
      BinaryOperator(std::string func, VarType type, std::string ret, std::string arg1, std::string arg2)
        : m_type(type), m_ret(std::move(ret)), m_arg1(std::move(arg1)), m_arg2(std::move(arg2))
      { m_func = std::move(func); }
      int Generate(OutputGenerator & gen) override;
      VarType m_type;
      std::string m_ret;
      std::string m_arg1;
      std::string m_arg2;
    };
    struct IndexOp : Node {
      IndexOp(bool store, VarType type, std::string array, std::vector<std::string> indexes, std::string value)
        : m_store(store), m_type(type), m_array(std::move(array)), m_indexes(std::move(indexes)), m_value(std::move(value))
      { m_func = "index"; }
      int Generate(OutputGenerator & gen) override;
      bool m_store;
      VarType m_type;
      std::string m_array;
      std::vector<std::string> m_indexes;
      std::string m_value;
    };
    struct Input : Node {
      Input(std::string name, VarType type, bool isString, bool line)
        : m_name(std::move(name)), m_type(type), m_string(isString), m_line(line)
      { m_func = "input"; }
      int Generate(OutputGenerator & gen) override;
      std::string m_name;
      VarType m_type;
      bool m_string;
      bool m_line;
    };
    struct OnGoto : Node {
      OnGoto(std::string index, VarType type, std::vector<std::string> lines)
        : m_index(std::move(index)), m_type(type), m_lines(std::move(lines))
      { m_func = "ongoto"; }
      int Generate(OutputGenerator & gen) override;
      std::string m_index;
      VarType m_type;
      std::vector<std::string> m_lines;
    };
    struct Clear : Node {
      Clear() { m_func = "clear"; }
      int Generate(OutputGenerator & gen) override;
    };
    struct Width : Node {
      explicit Width(int width) : m_width(width) { m_func = "width"; }
      int Generate(OutputGenerator & gen) override;
      int m_width;
    };

    CodeGenerator() = default;

    bool Build(const AST::Program & program);

    const std::set<std::string> & GetFuncsUsed() const { return m_funcsUsed; }
    bool IsPrintUsed() const { return m_printUsed; }

    int Generate(const AST::Node & node);
    int Evaluate(const AST::Node & expr, std::string & result);
    int Print(const AST::Node & node);

    int Generate(const AST::SourceLine & line);
    int Generate(const AST::Statement & statement);
    int Generate(const AST::Print & expr);
    int Generate(const AST::NumericAssign & expr);
    int Generate(const AST::StringAssign & expr);
    int Generate(const AST::GotoStatement & expr);
    int Generate(const AST::End & expr);
    int Generate(const AST::System & expr);
    int Generate(const AST::Rem & expr);
    int Generate(const AST::AssignStatement & expr);
    int Generate(const AST::IfStatement & expr);
    int Generate(const AST::ForStatement & expr);
    int Generate(const AST::NextStatement & expr);
    int Generate(const AST::GosubStatement & expr);
    int Generate(const AST::ReturnStatement & expr);
    int Generate(const AST::DimStatement & expr);
    int Generate(const AST::InputStatement & expr);
    int Generate(const AST::OnGotoStatement & expr);
    int Generate(const AST::DefStatement & expr);
    int Generate(const AST::ClearStatement & expr);
    int Generate(const AST::WidthStatement & expr);

    int Evaluate(const AST::StringConstant & expr, std::string & result);
    int Evaluate(const AST::StringVarRef & expr, std::string & result);
    int Evaluate(const AST::StrFunction & expr, std::string & result);
    int Evaluate(const AST::Int16Constant & expr, std::string & result);
    int Evaluate(const AST::Int32Constant & expr, std::string & result);
    int Evaluate(const AST::SingleConstant & expr, std::string & result);
    int Evaluate(const AST::DoubleConstant & expr, std::string & result);
    int Evaluate(const AST::NumericVarRef & expr, std::string & result);
    int Evaluate(const AST::NumericAddition & expr, std::string & result);
    int Evaluate(const AST::Subtraction & expr, std::string & result);
    int Evaluate(const AST::Multiplication & expr, std::string & result);
    int Evaluate(const AST::Division & expr, std::string & result);
    int Evaluate(const AST::NumericEquality & expr, std::string & result);
    int Evaluate(const AST::NumericNotEquality & expr, std::string & result);
    int Evaluate(const AST::NumericGreaterThan & expr, std::string & result);
    int Evaluate(const AST::NumericGreaterThanEqual & expr, std::string & result);
    int Evaluate(const AST::NumericLessThan & expr, std::string & result);
    int Evaluate(const AST::NumericLessThanEqual & expr, std::string & result);
    int Evaluate(const AST::LogicalAnd & expr, std::string & result);
    int Evaluate(const AST::LogicalOr & expr, std::string & result);
    int Evaluate(const AST::NumericSubscript & expr, std::string & result);
    int Evaluate(const AST::StringCompare & expr, std::string & result);
    int Evaluate(const AST::RndFunction & expr, std::string & result);
    int Evaluate(const AST::AbsFunction & expr, std::string & result);
    int Evaluate(const AST::Negation & expr, std::string & result);
    int Evaluate(const AST::Power & expr, std::string & result);
    int Evaluate(const AST::NumericCast & expr, std::string & result);
    int Evaluate(const AST::IntFunction & expr, std::string & result);
    int Evaluate(const AST::SqrFunction & expr, std::string & result);
    int Evaluate(const AST::LenFunction & expr, std::string & result);
    int Evaluate(const AST::TabFunction & expr, std::string & result);
    int Evaluate(const AST::LeftFunction & expr, std::string & result);
    int Evaluate(const AST::MidFunction & expr, std::string & result);
    int Evaluate(const AST::RightFunction & expr, std::string & result);
    int Evaluate(const AST::ChrFunction & expr, std::string & result);
    int Evaluate(const AST::StringAddition & expr, std::string & result);

    int Print(const AST::NumericExpr & expr);
    int Print(const AST::StringConstant & expr);
    int Print(const AST::Int16Constant & expr);
    int Print(const AST::Int32Constant & expr);
    int Print(const AST::SingleConstant & expr);
    int Print(const AST::DoubleConstant & expr);
    int Print(const AST::PrintComma & expr);
    int Print(const AST::PrintSemiColon & expr);
    int Print(const AST::NumericVarRef & expr);
    int Print(const AST::StringVarRef & expr);
    int Print(const AST::NumericSubscript & expr);
    int Print(const AST::StringExpr & expr);

    std::deque<std::shared_ptr<Node>> m_code;

  protected:
    void CompilerErrorInternal(ErrorCode code, unsigned ln, const std::string & msg);
    bool CheckVars();
    bool CheckJumps();

    template <typename T, typename... Args>
    void Add(Args &&... args)
    {
      auto node = std::make_shared<T>(std::forward<Args>(args)...);
      if (!node->m_func.empty() && std::isalpha(static_cast<unsigned char>(node->m_func[0])))
        m_funcsUsed.insert(node->m_func);
      m_code.push_back(node);
    }

    std::string GetTempName();
    std::string NumberText(const AST::NumericConstant & expr) const;
    int BinaryNumeric(const std::string & op, const AST::NumericBinaryOperation & expr, std::string & result);
    int CompareNumeric(const std::string & op, const AST::NumericBinaryOperation & expr, std::string & result);
    int UnaryNumeric(const std::string & op, VarType type, const AST::NumericExpr * arg, std::string & result);
    void EmitPrint(VarType type, const std::string & value, bool literal);
    bool FoldBinary(const std::string & op, VarType type, const std::string & lhs, const std::string & rhs, std::string & result);

    struct ForInfo {
      std::string m_index;
      VarType m_type;
      std::string m_to;
      std::string m_step;
      std::string m_body;
      std::string m_check;
    };

    std::vector<ForInfo> m_forStack;
    std::set<std::string> m_funcsUsed;
    bool m_printUsed = false;
    unsigned m_tempIndex = 1;
    unsigned m_forIndex = 1;
    unsigned m_lineNumber = 0;
    const AST::Program * m_program = nullptr;
};

#endif
