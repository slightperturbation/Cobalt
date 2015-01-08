#include <cstdint>
#include <cstdio>
#include <cmath>
#include <string>

#include "minijson.h"

typedef std::vector<IString> StringVec;

//==================
// Globals
//==================

Ref doc, extraInfo;

IString TOPLEVEL("toplevel"),
        DEFUN("defun"),
        BLOCK("block"),
        STAT("stat"),
        ASSIGN("assign"),
        NAME("name"),
        VAR("var"),
        CONDITIONAL("conditional"),
        BINARY("binary"),
        RETURN("return"),
        IF("if"),
        WHILE("while"),
        DO("do"),
        FOR("for"),
        SEQ("seq"),
        SUB("sub"),
        CALL("call"),
        NUM("num"),
        LABEL("label"),
        BREAK("break"),
        CONTINUE("continue"),
        SWITCH("switch"),
        STRING("string"),
        INF("inf"),
        NaN("nan"),
        TEMP_RET0("tempRet0"),
        UNARY_PREFIX("unary-prefix"),
        UNARY_POSTFIX("unary-postfix"),
        MATH_FROUND("Math_fround"),
        SIMD_FLOAT32X4("SIMD_float32x4"),
        SIMD_INT32X4("SIMD_int32x4"),
        PLUS("+"),
        MINUS("-"),
        OR("|"),
        AND("&"),
        XOR("^"),
        L_NOT("!"),
        B_NOT("~"),
        LT("<"),
        GE(">="),
        LE("<="),
        GT(">"),
        EQ("=="),
        NE("!="),
        DIV("/"),
        MOD("%"),
        RSHIFT(">>"),
        LSHIFT("<<"),
        TRSHIFT(">>>"),
        TEMP_DOUBLE_PTR("tempDoublePtr"),
        HEAP8("HEAP8"),
        HEAP16("HEAP16"),
        HEAP32("HEAP32"),
        HEAPF32("HEAPF32"),
        HEAPU8("HEAPU8"),
        HEAPU16("HEAPU16"),
        HEAPU32("HEAPU32"),
        HEAPF64("HEAPF64"),
        F0("f0"),
        EMPTY("");

//==================
// Infrastructure
//==================

template<class T, class V>
int indexOf(T list, V value) {
  for (int i = 0; i < list.size(); i++) {
    if (list[i] == value) return i;
  }
  return -1;
}

int jsD2I(double x) {
  return (int)((int64_t)x);
}

char *strdupe(const char *str) {
  char *ret = (char *)malloc(strlen(str)+1); // leaked!
  strcpy(ret, str);
  return ret;
}

int parseInt(const char *str) {
  int ret = *str - '0';
  while (*(++str)) {
    ret *= 10;
    ret += *str - '0';
  }
  return ret;
}

IString getHeapStr(int x, bool unsign) {
  switch (x) {
    case 8: return unsign ? HEAPU8 : HEAP8;
    case 16: return unsign ? HEAPU16 : HEAP16;
    case 32: return unsign ? HEAPU32 : HEAP32;
  }
  assert(0);
  return ":(";
}

// Traversals

struct TraverseInfo {
  Ref node;
  int index;
};

template <class T, int init>
struct StackedStack { // a stack, on the stack
  T stackStorage[init];
  T* storage;
  int used, available; // used amount, available amount
  bool alloced;

  StackedStack() : used(0), available(init), alloced(false) {
    storage = stackStorage;
  }
  ~StackedStack() {
    if (alloced) free(storage);
  }

  int size() { return used; }

  void push_back(const T& t) {
    assert(used <= available);
    if (used == available) {
      available *= 2;
      if (!alloced) {
        T* old = storage;
        storage = (T*)malloc(sizeof(T)*available);
        memcpy(storage, old, sizeof(T)*used);
        alloced = true;
      } else {
        storage = (T*)realloc(storage, sizeof(T)*available);
      }
    }
    assert(used < available);
    assert(storage);
    storage[used++] = t;
  }

  T& back() {
    assert(used > 0);
    return storage[used-1];
  }

  void pop_back() {
    assert(used > 0);
    used--;
  }
};

#define visitable(node) (node->isArray() and node->size() > 0)

#define TRAV_STACK 40

// Traverse, calling visit before the children
void traversePre(Ref node, std::function<void (Ref)> visit) {
  if (!visitable(node)) return;
  visit(node);
  StackedStack<TraverseInfo, TRAV_STACK> stack;
  stack.push_back({ node, 0 });
  while (stack.size() > 0) {
    TraverseInfo& top = stack.back();
    if (top.index < top.node->size()) {
      Ref sub = top.node[top.index];
      top.index++;
      if (visitable(sub)) {
        visit(sub);
        stack.push_back({ sub, 0 });
      }
    } else {
      stack.pop_back();
    }
  }
}

// Traverse, calling visitPre before the children and visitPost after
void traversePrePost(Ref node, std::function<void (Ref)> visitPre, std::function<void (Ref)> visitPost) {
  if (!visitable(node)) return;
  visitPre(node);
  StackedStack<TraverseInfo, TRAV_STACK> stack;
  stack.push_back({ node, 0 });
  while (stack.size() > 0) {
    TraverseInfo& top = stack.back();
    if (top.index < top.node->size()) {
      Ref sub = top.node[top.index];
      top.index++;
      if (visitable(sub)) {
        visitPre(sub);
        stack.push_back({ sub, 0 });
      }
    } else {
      visitPost(top.node);
      stack.pop_back();
    }
  }
  visitPost(node);
}

// Traverse, calling visitPre before the children and visitPost after. If pre returns false, do not traverse children
void traversePrePostConditional(Ref node, std::function<bool (Ref)> visitPre, std::function<void (Ref)> visitPost) {
  if (!visitable(node)) return;
  if (!visitPre(node)) return;
  StackedStack<TraverseInfo, TRAV_STACK> stack;
  stack.push_back({ node, 0 });
  while (stack.size() > 0) {
    TraverseInfo& top = stack.back();
    if (top.index < top.node->size()) {
      Ref sub = top.node[top.index];
      top.index++;
      if (visitable(sub)) {
        if (visitPre(sub)) {
          stack.push_back({ sub, 0 });
        }
      }
    } else {
      visitPost(top.node);
      stack.pop_back();
    }
  }
  visitPost(node);
}

// Traverses all the top-level functions in the document
void traverseFunctions(Ref ast, std::function<void (Ref)> visit) {
  if (!ast || ast->size() == 0) return;
  if (ast[0] == TOPLEVEL) {
    Ref stats = ast[1];
    for (int i = 0; i < stats->size(); i++) {
      Ref curr = stats[i];
      if (curr[0] == DEFUN) visit(curr);
    }
  } else if (ast[0] == DEFUN) {
    visit(ast);
  }
}

Ref deStat(Ref node) {
  if (node[0] == STAT) return node[1];
  return node;
}

Ref getStatements(Ref node) {
  if (node[0] == DEFUN) {
    return node[3];
  } else if (node[0] == BLOCK) {
    return node->size() > 1 ? node[1] : nullptr;
  } else {
    return arena.alloc();
  }
}

// Types

enum AsmType {
  ASM_INT = 0,
  ASM_DOUBLE = 1,
  ASM_FLOAT = 2,
  ASM_FLOAT32X4 = 3,
  ASM_INT32X4 = 4,
  ASM_NONE = 5 // number of types
};

// forward decls
struct AsmData;
AsmType detectType(Ref node, AsmData *asmData=nullptr, bool inVarDef=false);
Ref makeEmpty();
bool isEmpty(Ref node);
Ref makeAsmVarDef(const IString& v, AsmType type);
Ref makeArray();
Ref makeNum(double x);
Ref makeName(IString str);
Ref makeAsmCoercion(Ref node, AsmType type);
Ref make1(IString type, Ref a);
Ref make3(IString type, Ref a, Ref b, Ref c);

struct AsmData {
  struct Local {
    AsmType type;
    bool param; // false if a var
  };
  typedef std::unordered_map<IString, Local> Locals;

  Locals locals;
  std::vector<IString> params; // in order
  std::vector<IString> vars; // in order
  AsmType ret;

  Ref func;

  AsmType getType(const IString& name) {
    auto ret = locals.find(name);
    if (ret != locals.end()) return ret->second.type;
    return ASM_NONE;
  }
  void setType(const IString& name, AsmType type) {
    locals[name].type = type;
  }

  bool isLocal(const IString& name) {
    return locals.count(name) > 0;
  }
  bool isParam(const IString& name) {
    return isLocal(name) && locals[name].param;
  }
  bool isVar(const IString& name) {
    return isLocal(name) && !locals[name].param;
  }

  AsmData(Ref f) {
    func = f;

    // process initial params
    Ref stats = func[3];
    int i = 0;
    while (i < stats->size()) {
      Ref node = stats[i];
      if (node[0] != STAT || node[1][0] != ASSIGN || node[1][2][0] != NAME) break;
      node = node[1];
      Ref name = node[2][1];
      int index = func[2]->indexOf(name);
      if (index < 0) break; // not an assign into a parameter, but a global
      IString& str = name->getIString();
      if (locals.count(str) > 0) break; // already done that param, must be starting function body
      locals[str] = { detectType(node[3]), true };
      params.push_back(str);
      stats[i] = makeEmpty();
      i++;
    }
    // process initial variable definitions
    while (i < stats->size()) {
      Ref node = stats[i];
      if (node[0] != VAR) break;
      for (int j = 0; j < node[1]->size(); j++) {
        Ref v = node[1][j];
        IString& name = v[0]->getIString();
        Ref value = v[1];
        if (locals.count(name) == 0) {
          locals[name] = { detectType(value, nullptr, true), false };
          vars.push_back(name);
          v->setSize(1); // make an un-assigning var
        } else {
          assert(j == 0); // cannot break in the middle
          goto outside;
        }
      }
      i++;
    }
    outside:
    // look for other var definitions and collect them
    while (i < stats->size()) {
      traversePre(stats[i], [&](Ref node) {
        Ref type = node[0];
        if (type == VAR) {
          dump("bad, seeing a var in need of fixing", func);
          assert(0); //, 'should be no vars to fix! ' + func[1] + ' : ' + JSON.stringify(node));
        }
      });
      i++;
    }
    // look for final RETURN statement to get return type.
    Ref retStmt = stats->back();
    if (!!retStmt && retStmt[0] == RETURN && !!retStmt[1]) {
      ret = detectType(retStmt[1]);
    } else {
      ret = ASM_NONE;
    }
  }

  void denormalize() {
    Ref stats = func[3];
    // Remove var definitions, if any
    for (int i = 0; i < stats->size(); i++) {
      if (stats[i][0] == VAR) {
        stats[i] = makeEmpty();
      } else {
        if (!isEmpty(stats[i])) break;
      }
    }
    // calculate variable definitions
    Ref varDefs = makeArray();
    for (auto v : vars) {
      varDefs->push_back(makeAsmVarDef(v, locals[v].type));
    }
    // each param needs a line; reuse emptyNodes as much as we can
    int numParams = params.size();
    int emptyNodes = 0;
    while (emptyNodes < stats->size()) {
      if (!isEmpty(stats[emptyNodes])) break;
      emptyNodes++;
    }
    int neededEmptyNodes = numParams + (varDefs->size() ? 1 : 0); // params plus one big var if there are vars
    if (neededEmptyNodes > emptyNodes) {
      stats->insert(0, neededEmptyNodes - emptyNodes);
    } else if (neededEmptyNodes < emptyNodes) {
      stats->splice(0, emptyNodes - neededEmptyNodes);
    }
    // add param coercions
    int next = 0;
    for (auto param : func[2]->getArray()) {
      IString str = param->getIString();
      assert(locals.count(str) > 0);
      stats[next++] = make1(STAT, make3(ASSIGN, &(arena.alloc())->setBool(true), makeName(str.c_str()), makeAsmCoercion(makeName(str.c_str()), locals[str].type)));
    }
    if (varDefs->size()) {
      stats[next] = make1(VAR, varDefs);
    }
    /*
    if (inlines->size() > 0) {
      var i = 0;
      traverse(func, function(node, type) {
        if (type == CALL && node[1][0] == NAME && node[1][1] == 'inlinejs') {
          node[1] = inlines[i++]; // swap back in the body
        }
      });
    }
    */
    // ensure that there's a final RETURN statement if needed.
    if (ret != ASM_NONE) {
      Ref retStmt = stats->back();
      if (!retStmt || retStmt[0] != RETURN) {
        Ref retVal = makeNum(0);
        if (ret != ASM_INT) {
          retVal = makeAsmCoercion(retVal, ret);
        }
        stats->push_back(make1(RETURN, retVal));
      }
    }
    //printErr('denormalized \n\n' + astToSrc(func) + '\n\n');
  }

  void addParam(IString name, AsmType type) {
    locals[name] = { type, true };
    params.push_back(name);
  }
  void addVar(IString name, AsmType type) {
    locals[name] = { type, false };
    vars.push_back(name);
  }

  void deleteVar(IString name) {
    locals.erase(name);
    for (int i = 0; i < vars.size(); i++) {
      if (vars[i] == name) {
        vars.erase(vars.begin() + i);
        break;
      }
    }
  }
};

struct HeapInfo {
  bool valid, unsign, floaty;
  int bits;
  AsmType type;
};

HeapInfo parseHeap(const char *name) {
  HeapInfo ret;
  if (name[0] != 'H' || name[1] != 'E' || name[2] != 'A' || name[3] != 'P') {
    ret.valid = false;
    return ret;
  }
  ret.valid = true;
  ret.unsign = name[4] == 'U';
  ret.floaty = name[4] == 'F';
  ret.bits = parseInt(name + (ret.unsign || ret.floaty ? 5 : 4));
  ret.type = !ret.floaty ? ASM_INT : (ret.bits == 64 ? ASM_DOUBLE : ASM_FLOAT);
  return ret;
}

bool isInteger(double x) {
  return fmod(x, 1) == 0;
}

bool isInteger32(double x) {
  return isInteger(x) && (x == (int32_t)x || x == (uint32_t)x);
}

IString ASM_FLOAT_ZERO;

AsmType detectType(Ref node, AsmData *asmData, bool inVarDef) {
  switch (node[0]->getCString()[0]) {
    case 'n': {
      if (node[0] == NUM) {
        if (!isInteger(node[1]->getNumber())) return ASM_DOUBLE;
        return ASM_INT;
      } else if (node[0] == NAME) {
        if (asmData) {
          AsmType ret = asmData->getType(node[1]->getCString());
          if (ret != ASM_NONE) return ret;
        }
        if (!inVarDef) {
          if (node[1] == INF || node[1] == NaN) return ASM_DOUBLE;
          if (node[1] == TEMP_RET0) return ASM_INT;
          return ASM_NONE;
        }
        // We are in a variable definition, where Math_fround(0) optimized into a global constant becomes f0 = Math_fround(0)
        if (ASM_FLOAT_ZERO.isNull()) ASM_FLOAT_ZERO = node[1]->getIString();
        else assert(node[1] == ASM_FLOAT_ZERO);
        return ASM_FLOAT;
      }
      break;
    }
    case 'u': {
      if (node[0] == UNARY_PREFIX) {
        switch (node[1]->getCString()[0]) {
          case '+': return ASM_DOUBLE;
          case '-': return detectType(node[2], asmData, inVarDef);
          case '!': case '~': return ASM_INT;
        }
        break;
      }
      break;
    }
    case 'c': {
      if (node[0] == CALL) {
        if (node[1][0] == NAME) {
          IString name = node[1][1]->getIString();
          if (name == MATH_FROUND) return ASM_FLOAT;
          else if (name == SIMD_FLOAT32X4) return ASM_FLOAT32X4;
          else if (name == SIMD_INT32X4) return ASM_INT32X4;
        }
        return ASM_NONE;
      } else if (node[0] == CONDITIONAL) {
        return detectType(node[2], asmData, inVarDef);
      }
      break;
    }
    case 'b': {
      if (node[0] == BINARY) {
        switch (node[1]->getCString()[0]) {
          case '+': case '-':
          case '*': case '/': case '%': return detectType(node[2], asmData, inVarDef);
          case '|': case '&': case '^': case '<': case '>': // handles <<, >>, >>=, <=, >=
          case '=': case '!': { // handles ==, !=
            return ASM_INT;
          }
        }
      }
      break;
    }
    case 's': {
      if (node[0] == SEQ) {
        return detectType(node[2], asmData, inVarDef);
      } else if (node[0] == SUB) {
        assert(node[1][0] == NAME);
        HeapInfo info = parseHeap(node[1][1]->getCString());
        if (info.valid) return ASM_NONE;
        return info.floaty ? ASM_DOUBLE : ASM_INT; // XXX ASM_FLOAT?
      }
      break;
    }
  }
  dump("horrible", node);
  assert(0);
  return ASM_NONE;
}

// Constructions TODO: share common constructions, and assert they remain frozen

Ref makeArray() {
  return &arena.alloc()->setArray();
}

Ref makeString(const IString& s) {
  return &arena.alloc()->setString(s);
}

Ref makeEmpty() {
  Ref ret(makeArray());
  ret->push_back(makeString(TOPLEVEL));
  ret->push_back(makeArray());
  return ret;
}

Ref makePair(Ref x, Ref y) {
  Ref ret = makeArray();
  ret->push_back(x);
  ret->push_back(y);
  return ret;
};

Ref makeNum(double x) {
  Ref ret(makeArray());
  ret->push_back(makeString(NUM));
  ret->push_back(&arena.alloc()->setNumber(x));
  return ret;
}

Ref makeName(IString str) {
  Ref ret(makeArray());
  ret->push_back(makeString(NAME));
  ret->push_back(makeString(str));
  return ret;
}

Ref makeBlock() {
  Ref ret(makeArray());
  ret->push_back(makeString(BLOCK));
  ret->push_back(makeArray());
  return ret;
}

Ref make1(IString type, Ref a) {
  Ref ret(makeArray());
  ret->push_back(makeString(type));
  ret->push_back(a);
  return ret;
}

Ref make2(IString type, IString a, Ref b) {
  Ref ret(makeArray());
  ret->push_back(makeString(type));
  ret->push_back(makeString(a));
  ret->push_back(b);
  return ret;
}

Ref make2(IString type, Ref a, Ref b) {
  Ref ret(makeArray());
  ret->push_back(makeString(type));
  ret->push_back(a);
  ret->push_back(b);
  return ret;
}

Ref make3(IString type, IString a, Ref b, Ref c) {
  Ref ret(makeArray());
  ret->push_back(makeString(type));
  ret->push_back(makeString(a));
  ret->push_back(b);
  ret->push_back(c);
  return ret;
}

Ref make3(IString type, Ref a, Ref b, Ref c) {
  Ref ret(makeArray());
  ret->push_back(makeString(type));
  ret->push_back(a);
  ret->push_back(b);
  ret->push_back(c);
  return ret;
}

Ref makeAsmVarDef(const IString& v, AsmType type) {
  Ref val;
  switch (type) {
    case ASM_INT: val = makeNum(0); break;
    case ASM_DOUBLE: val = make2(UNARY_PREFIX, PLUS, makeNum(0)); break;
    case ASM_FLOAT: {
      if (!ASM_FLOAT_ZERO.isNull()) {
        val = makeName(ASM_FLOAT_ZERO);
      } else {
        val = make2(CALL, makeName(MATH_FROUND), &(makeArray())->push_back(makeNum(0)));
      }
      break;
    }
    case ASM_FLOAT32X4: {
      val = make2(CALL, makeName(SIMD_FLOAT32X4), &(makeArray())->push_back(makeNum(0)).push_back(makeNum(0)).push_back(makeNum(0)).push_back(makeNum(0)));
      break;
    }
    case ASM_INT32X4: {
      val = make2(CALL, makeName(SIMD_INT32X4), &(makeArray())->push_back(makeNum(0)).push_back(makeNum(0)).push_back(makeNum(0)).push_back(makeNum(0)));
      break;
    }
    default: assert(0);
  }
  return makePair(&(arena.alloc()->setString(v)), val);
}

Ref makeAsmCoercion(Ref node, AsmType type) {
  switch (type) {
    case ASM_INT: return make3(BINARY, OR, node, makeNum(0));
    case ASM_DOUBLE: return make2(UNARY_PREFIX, PLUS, node);
    case ASM_FLOAT: return make2(CALL, makeName(MATH_FROUND), &(makeArray())->push_back(node));
    case ASM_FLOAT32X4: return make2(CALL, makeName(SIMD_FLOAT32X4), &(makeArray())->push_back(node));
    case ASM_INT32X4: return make2(CALL, makeName(SIMD_INT32X4), &(makeArray())->push_back(node));
    case ASM_NONE:
    default: return node; // non-validating code, emit nothing XXX this is dangerous, we should only allow this when we know we are not validating
  }
}

// Checks

bool isEmpty(Ref node) {
  return node->size() == 2 && node[0] == TOPLEVEL && node[1]->size() == 0;
}

bool commable(Ref node) { // TODO: hashing
  IString type = node[0]->getIString();
  if (type == ASSIGN || type == BINARY || type == UNARY_PREFIX || type == NAME || type == NUM || type == CALL || type == SEQ || type == CONDITIONAL || type == SUB) return true;
  return false;
}

bool isMathFunc(const char *name) {
  static const char *Math_ = "Math_";
  static unsigned size = strlen(Math_);
  return strncmp(name, Math_, size) == 0;
}

bool isMathFunc(Ref value) {
  return value->isString() && isMathFunc(value->getCString());
}

bool callHasSideEffects(Ref node) { // checks if the call itself (not the args) has side effects (or is not statically known)
  return !(node[1][0] == NAME && isMathFunc(node[1][1]));
}

bool hasSideEffects(Ref node) { // this is 99% incomplete!
  IString type = node[0]->getIString();
  switch (type[0]) {
    case 'n':
      if (type == NUM || type == NAME) return false;
      break;
    case 's':
      if (type == STRING) return false;
      if (type == SUB) return hasSideEffects(node[1]) || hasSideEffects(node[2]);
      break;
    case 'u':
      if (type == UNARY_PREFIX) return hasSideEffects(node[2]);
      break;
    case 'b':
      if (type == BINARY) return hasSideEffects(node[2]) || hasSideEffects(node[3]);
      break;
    case 'c':
      if (type == CALL) {
        if (callHasSideEffects(node)) return true;
        // This is a statically known call, with no side effects. only args can side effect us
        for (auto arg : node[2]->getArray()) {
          if (hasSideEffects(arg)) return true;
        }
        return false;
      } else if (type == CONDITIONAL) return hasSideEffects(node[1]) || hasSideEffects(node[2]) || hasSideEffects(node[3]); 
      break;
  }
  return true;
}

// checks if a node has just basic operations, nothing with side effects nor that can notice side effects, which
// implies we can move it around in the code
bool triviallySafeToMove(Ref node, AsmData& asmData) {
  bool ok = true;
  traversePre(node, [&](Ref node) {
    Ref type = node[0];
    if (type == STAT || type == BINARY || type == UNARY_PREFIX || type == ASSIGN || type == NUM) return;
    else if (type == NAME) {
      if (!asmData.isLocal(node[1]->getIString())) ok = false;
    } else if (type == CALL) {
      if (callHasSideEffects(node)) ok = false;
    } else {
      ok = false;
    }  
  });
  return ok;
}

// Transforms

// We often have branchings that are simplified so one end vanishes, and
// we then get
//   if (!(x < 5))
// or such. Simplifying these saves space and time.
Ref simplifyNotCompsDirect(Ref node) {
  if (node[0] == UNARY_PREFIX && node[1] == L_NOT) {
    // de-morgan's laws do not work on floats, due to nans >:(
    if (node[2][0] == BINARY && (detectType(node[2][2]) == ASM_INT && detectType(node[2][3]) == ASM_INT)) {
      Ref op = node[2][1];
      switch(op->getCString()[0]) {
        case '<': {
          if (op == LT)  { op->setString(GE); break; }
          if (op == LE) { op->setString(GT); break; }
          return node;
        }
        case '>': {
          if (op == GT)  { op->setString(LE); break; }
          if (op == GE) { op->setString(LT); break; }
          return node;
        }
        case '=': {
          if (op == EQ) { op->setString(NE); break; }
          return node;
        }
        case '!': {
          if (op == NE) { op->setString(EQ); break; }
          return node;
        }
        default: return node;
      }
      return make3(BINARY, op, node[2][2], node[2][3]);
    } else if (node[2][0] == UNARY_PREFIX && node[2][1] == L_NOT) {
      return node[2][2];
    }
  }
  return node;
}

Ref flipCondition(Ref cond) {
  return simplifyNotCompsDirect(make2(UNARY_PREFIX, L_NOT, cond));
}

void safeCopy(Ref target, Ref source) { // safely copy source onto target, even if source is a subnode of target
  Ref temp = source; // hold on to source
  *target = *temp;
}

void clearEmptyNodes(Ref arr) {
  int skip = 0;
  for (int i = 0; i < arr->size(); i++) {
    if (skip) {
      arr[i-skip] = arr[i];
    }
    if (isEmpty(deStat(arr[i]))) {
      skip++;
    }
  }
  if (skip) arr->setSize(arr->size() - skip);
}
void clearUselessNodes(Ref arr) {
  int skip = 0;
  for (int i = 0; i < arr->size(); i++) {
    Ref curr = arr[i];
    if (skip) {
      arr[i-skip] = curr;
    }
    if (isEmpty(deStat(curr)) || (curr[0] == STAT && !hasSideEffects(curr[1]))) {
      skip++;
    }
  }
  if (skip) arr->setSize(arr->size() - skip);
}

void removeAllEmptySubNodes(Ref ast) {
  traversePre(ast, [](Ref node) {
    int index = -1;
    if (node[0] == DEFUN) {
      clearEmptyNodes(node[3]);
    } else if (node[0] == BLOCK && node->size() > 1 && !!node[1]) {
      clearEmptyNodes(node[1]);
    } else if (node[0] == SEQ && isEmpty(node[1])) {
      safeCopy(node, node[2]);
    }
  });
}
void removeAllUselessSubNodes(Ref ast) {
  traversePre(ast, [](Ref node) {
    int index = -1;
    if (node[0] == DEFUN) {
      clearUselessNodes(node[3]);
    } else if (node[0] == BLOCK && node->size() > 1 && !!node[1]) {
      clearUselessNodes(node[1]);
    } else if (node[0] == SEQ && isEmpty(node[1])) {
      safeCopy(node, node[2]);
    }
  });
}

Ref unVarify(Ref vars) { // transform var x=1, y=2 etc. into (x=1, y=2), i.e., the same assigns, but without a var definition
  Ref ret = makeArray();
  ret->push_back(makeString(STAT));
  if (vars->size() == 1) {
    ret->push_back(make3(ASSIGN, &(arena.alloc())->setBool(true), makeName(vars[0][0]->getIString()), vars[0][1]));
  } else {
    ret->push_back(makeArray());
    Ref curr = ret[1];
    for (int i = 0; i < vars->size()-1; i++) {
      curr->push_back(makeString(SEQ));
      curr->push_back(make3(ASSIGN, &(arena.alloc())->setBool(true), makeName(vars[i][0]->getIString()), vars[i][1]));
      if (i != vars->size()-2) {
        curr->push_back(makeArray());
        curr = curr[2];
      }
    }
    curr->push_back(make3(ASSIGN, &(arena.alloc())->setBool(true), makeName(vars->back()[0]->getIString()), vars->back()[1]));
  }
  return ret;
}

// Calculations

int measureCost(Ref ast) {
  int size = 0;
  traversePre(ast, [&size](Ref node) {
    Ref type = node[0];
    if (type == NUM || type == UNARY_PREFIX) size--;
    else if (type == BINARY) {
      if (node[3][0] == NUM && node[3][1]->getNumber() == 0) size--;
      else if (node[1] == DIV || node[1] == MOD) size += 2;
    }
    else if (type == CALL && !callHasSideEffects(node)) size -= 2;
    else if (type == SUB) size++;
    size++;
  });
  return size;
}

//==================
// Params
//==================

bool preciseF32 = false;

//=====================
// Optimization passes
//=====================

#define HASES \
  bool has(const IString& str) { \
    return count(str) > 0; \
  } \
  bool has(Ref node) { \
    return node->isString() && count(node->getIString()) > 0; \
  }

class StringSet : public std::unordered_set<IString> {
public:
  StringSet() {}
  StringSet(const char *init) { // comma-delimited list
    int size = strlen(init);
    char *curr = (char*)malloc(size+1); // leaked!
    strcpy(curr, init);
    while (1) {
      char *end = strchr(curr, ' ');
      if (end) *end = 0;
      insert(curr);
      if (!end) break;
      curr = end + 1;
    }
  }

  HASES

  void dump() {
    err("===");
    for (auto str : *this) {
      errv("%s", str.c_str());
    }
    err("===");
  }
};

StringSet USEFUL_BINARY_OPS("<< >> | & ^"),
          COMPARE_OPS("< <= > >= == == != !=="),
          BITWISE("| & ^"),
          SAFE_BINARY_OPS("+ -"), // division is unsafe as it creates non-ints in JS; mod is unsafe as signs matter so we can't remove |0's; mul does not nest with +,- in asm
          COERCION_REQUIRING_OPS("sub unary-prefix"), // ops that in asm must be coerced right away
          COERCION_REQUIRING_BINARIES("* / %"); // binary ops that in asm must be coerced

StringSet ASSOCIATIVE_BINARIES("+ * | & ^"),
          CONTROL_FLOW("do while for if switch"),
          LOOP("do while for"),
          NAME_OR_NUM("name num");

bool isFunctionTable(const char *name) {
  static const char *functionTable = "FUNCTION_TABLE";
  static unsigned size = strlen(functionTable);
  return strncmp(name, functionTable, size) == 0;
}

bool isFunctionTable(Ref value) {
  return value->isString() && isFunctionTable(value->getCString());
}

// Passes

// Eliminator aka Expressionizer
//
// The goal of this pass is to eliminate unneeded variables (which represent one of the infinite registers in the LLVM
// model) and thus to generate complex expressions where possible, for example
//
//  var x = a(10);
//  var y = HEAP[20];
//  print(x+y);
//
// can be transformed into
//
//  print(a(10)+HEAP[20]);
//
// The basic principle is to scan along the code in the order of parsing/execution, and keep a list of tracked
// variables that are current contenders for elimination. We must untrack when we see something that we cannot
// cross, for example, a write to memory means we must invalidate variables that depend on reading from
// memory, since if we change the order then we do not preserve the computation.
//
// We rely on some assumptions about emscripten-generated code here, which means we can do a lot more than
// a general JS optimization can. For example, we assume that SUB nodes (indexing like HEAP[..]) are
// memory accesses or FUNCTION_TABLE accesses, and in both cases that the symbol cannot be replaced although
// the contents can. So we assume FUNCTION_TABLE might have its contents changed but not be pointed to
// a different object, which allows
//
//  var x = f();
//  FUNCTION_TABLE[x]();
//
// to be optimized (f could replace FUNCTION_TABLE, so in general JS eliminating x is not valid).
//
// In memSafe mode, we are more careful and assume functions can replace HEAP and FUNCTION_TABLE, which
// can happen in ALLOW_MEMORY_GROWTH mode

StringSet ELIMINATION_SAFE_NODES("var assign call if toplevel do return label switch binary unary-prefix"); // do is checked carefully, however
StringSet IGNORABLE_ELIMINATOR_SCAN_NODES("num toplevel string break continue dot"); // dot can only be STRING_TABLE.*
StringSet ABORTING_ELIMINATOR_SCAN_NODES("new object function defun for while array throw"); // we could handle some of these, TODO, but nontrivial (e.g. for while, the condition is hit multiple times after the body)

bool isTempDoublePtrAccess(Ref node) { // these are used in bitcasts; they are not really affecting memory, and should cause no invalidation
  assert(node[0] == SUB);
  return (node[2][0] == NAME && node[2][1] == TEMP_DOUBLE_PTR) ||
         (node[2][0] == BINARY && ((node[2][2][0] == NAME && node[2][2][1] == TEMP_DOUBLE_PTR) ||
                                     (node[2][3][0] == NAME && node[2][3][1] == TEMP_DOUBLE_PTR)));
}

class StringIntMap : public std::unordered_map<IString, int> {
public:
  HASES
};

class StringStringMap : public std::unordered_map<IString, IString> {
public:
  HASES
};

class StringRefMap : public std::unordered_map<IString, Ref> {
public:
  HASES
};

class StringTypeMap : public std::unordered_map<IString, AsmType> {
public:
  HASES
};

void eliminate(Ref ast, bool memSafe=false) {
  // Find variables that have a single use, and if they can be eliminated, do so
  traverseFunctions(ast, [&memSafe](Ref func) {
    AsmData asmData(func);

    // First, find the potentially eliminatable functions: that have one definition and one use

    StringIntMap definitions;
    StringIntMap uses;
    StringIntMap namings;
    StringRefMap values;
    StringIntMap varsToRemove; // variables being removed, that we can eliminate all 'var x;' of (this refers to VAR nodes we should remove)
                            // 1 means we should remove it, 2 means we successfully removed it
    StringSet varsToTryToRemove; // variables that have 0 uses, but have side effects - when we scan we can try to remove them

    // examine body and note locals
    traversePre(func, [&](Ref node) {
      Ref type = node[0];
      if (type == VAR) {
        Ref node1 = node[1];
        for (int i = 0; i < node1->size(); i++) {
          Ref node1i = node1[i];
          IString name = node1i[0]->getIString();
          Ref value;
          if (node1i->size() > 1 && !!(value = node1i[1])) {
            definitions[name]++;
            if (!values.has(name)) values[name] = value;
          }
          uses[name];
        }
      } else if (type == NAME) {
        IString& name = node[1]->getIString();
        uses[name]++;// = uses[name] + 1;
      } else if (type == ASSIGN) {
        Ref target = node[2];
        if (target[0] == NAME) {
          IString& name = target[1]->getIString();
          definitions[name]++;//= definitions[name] + 1;
          uses[name]; // zero if not there already
          if (!values.has(name)) values[name] = node[3];
          assert(node[1]->isBool(true)); // not +=, -= etc., just =
          uses[name]--; // because the name node will show up by itself in the previous case
          namings[name]++;// = namings[name] + 1; // offset it here, this tracks the total times we are named
        }
      }
    });

    for (auto used : uses) {
      namings[used.first] += used.second;
    }

    StringSet potentials; // local variables with 1 definition and 1 use
    StringSet sideEffectFree; // whether a local variable has no side effects in its definition. Only relevant when there are no uses

    auto unprocessVariable = [&](IString name) {
      potentials.erase(name);
      varsToRemove.erase(name);
      sideEffectFree.erase(name);
      varsToTryToRemove.erase(name);
    };
    std::function<void (IString)> processVariable = [&](IString name) {
      if (definitions[name] == 1 && uses[name] == 1) {
        potentials.insert(name);
      } else if (uses[name] == 0 && definitions[name] <= 1) { // no uses, no def or 1 def (cannot operate on phis, and the llvm optimizer will remove unneeded phis anyhow) (no definition means it is a function parameter, or a local with just |var x;| but no defining assignment)
        bool sideEffects = false;
        auto val = values.find(name);
        Ref value;
        if (val != values.end()) {
          value = val->second;
          // TODO: merge with other side effect code
          // First, pattern-match
          //  (HEAP32[((tempDoublePtr)>>2)]=((HEAP32[(($_sroa_0_0__idx1)>>2)])|0),HEAP32[(((tempDoublePtr)+(4))>>2)]=((HEAP32[((($_sroa_0_0__idx1)+(4))>>2)])|0),(+(HEAPF64[(tempDoublePtr)>>3])))
          // which has no side effects and is the special form of converting double to i64.
          if (!(value[0] == SEQ && value[1][0] == ASSIGN && value[1][2][0] == SUB && value[1][2][2][0] == BINARY && value[1][2][2][1] == RSHIFT &&
                value[1][2][2][2][0] == NAME && value[1][2][2][2][1] == TEMP_DOUBLE_PTR)) {
            // If not that, then traverse and scan normally.
            sideEffects = hasSideEffects(value);
          }
        }
        if (!sideEffects) {
          varsToRemove[name] = !definitions[name] ? 2 : 1; // remove it normally
          sideEffectFree.insert(name);
          // Each time we remove a variable with 0 uses, if its value has no
          // side effects and vanishes too, then we can remove a use from variables
          // appearing in it, and possibly eliminate again
          if (!!value) {
            traversePre(value, [&](Ref node) {
              if (node[0] == NAME) {
                IString name = node[1]->getIString();
                node[1]->setString(EMPTY); // we can remove this - it will never be shown, and should not be left to confuse us as we traverse
                if (asmData.isLocal(name)) {
                  uses[name]--; // cannot be infinite recursion since we descend an energy function
                  assert(uses[name] >= 0);
                  unprocessVariable(name);
                  processVariable(name);
                }
              }
            });
          }
        } else {
          varsToTryToRemove.insert(name); // try to remove it later during scanning
        }
      }
    };
    for (auto name : asmData.locals) {
      processVariable(name.first);
    }

    //printErr('defs: ' + JSON.stringify(definitions));
    //printErr('uses: ' + JSON.stringify(uses));
    //printErr('values: ' + JSON.stringify(values));
    //printErr('locals: ' + JSON.stringify(locals));
    //printErr('varsToRemove: ' + JSON.stringify(varsToRemove));
    //printErr('varsToTryToRemove: ' + JSON.stringify(varsToTryToRemove));
    values.clear();
    //printErr('potentials: ' + JSON.stringify(potentials));
    // We can now proceed through the function. In each list of statements, we try to eliminate
    struct Tracking {
      bool usesGlobals, usesMemory;
      Ref defNode;
      StringSet deps;
      bool doesCall;
    };
    class Tracked : public std::unordered_map<IString, Tracking> {
    public:
      HASES
    };
    Tracked tracked;
    #define dumpTracked() { errv("tracking %d", tracked.size()); for (auto t : tracked) errv("... %s", t.first.c_str()); }

    bool globalsInvalidated = false; // do not repeat invalidations, until we track something new
    bool memoryInvalidated = false;
    bool callsInvalidated = false;
    auto track = [&](IString name, Ref value, Ref defNode) { // add a potential that has just been defined to the tracked list, we hope to eliminate it
      Tracking& track = tracked[name];
      track.usesGlobals = false;
      track.usesMemory = false;
      track.defNode = defNode;
      track.doesCall = false;
      bool ignoreName = false; // one-time ignorings of names, as first op in sub and call
      traversePre(value, [&](Ref node) {
        Ref type = node[0];
        if (type == NAME) {
          if (!ignoreName) {
            IString name = node[1]->getIString();
            if (!asmData.isLocal(name)) {
              track.usesGlobals = true;
            }
            if (!potentials.has(name)) { // deps do not matter for potentials - they are defined once, so no complexity
              track.deps.insert(name);
            }
          } else {
            ignoreName = false;
          }
        } else if (type == SUB) {
          track.usesMemory = true;
          ignoreName = true;
        } else if (type == CALL) {
          track.usesGlobals = true;
          track.usesMemory = true;
          track.doesCall = true;        
          ignoreName = true;
        } else {
          ignoreName = false;
        }
      });
      globalsInvalidated = false;
      memoryInvalidated = false;
      callsInvalidated = false;
    };

    // TODO: invalidate using a sequence number for each type (if you were tracked before the last invalidation, you are cancelled). remove for.in loops
    #define INVALIDATE(what, check) \
    auto invalidate##what = [&]() { \
      std::vector<IString> temp; \
      for (auto t : tracked) { \
        IString name = t.first; \
        Tracking& info = tracked[name]; \
        if (check) { \
          temp.push_back(name); \
        } \
      } \
      for (int i = 0; i < temp.size(); i++) { \
        tracked.erase(temp[i]); \
      } \
    };
    INVALIDATE(Globals, info.usesGlobals);
    INVALIDATE(Memory, info.usesMemory);
    INVALIDATE(Calls, info.doesCall);

    auto invalidateByDep = [&](IString dep) {
      std::vector<IString> temp;
      for (auto t : tracked) {
        IString name = t.first;
        Tracking& info = tracked[name];
        if (info.deps.has(dep)) {
          temp.push_back(name);
        }
      }
      for (int i = 0; i < temp.size(); i++) {
        tracked.erase(temp[i]);
      }
    };

    std::function<void (IString name, Ref node)> doEliminate;

    // Generate the sequence of execution. This determines what is executed before what, so we know what can be reordered. Using
    // that, performs invalidations and eliminations
    auto scan = [&](Ref node) {
      bool abort = false;
      bool allowTracking = true; // false inside an if; also prevents recursing in an if
      std::function<void (Ref, bool, bool)> traverseInOrder = [&](Ref node, bool ignoreSub, bool ignoreName) {
        if (abort) return;
        Ref type = node[0];
        if (type == ASSIGN) {
          Ref target = node[2];
          Ref value = node[3];
          bool nameTarget = target[0] == NAME;
          traverseInOrder(target, true,  nameTarget); // evaluate left
          traverseInOrder(value,  false, false); // evaluate right
          // do the actual assignment
          if (nameTarget) {
            IString name = target[1]->getIString();
            if (!potentials.has(name)) {
              if (!varsToTryToRemove.has(name)) {
                // expensive check for invalidating specific tracked vars. This list is generally quite short though, because of
                // how we just eliminate in short spans and abort when control flow happens TODO: history numbers instead
                invalidateByDep(name); // can happen more than once per dep..
                if (!asmData.isLocal(name) && !globalsInvalidated) {
                  invalidateGlobals();
                  globalsInvalidated = true;
                }
                // if we can track this name (that we assign into), and it has 0 uses and we want to remove its VAR
                // definition - then remove it right now, there is no later chance
                if (allowTracking && varsToRemove.has(name) && uses[name] == 0) {
                  track(name, node[3], node);
                  doEliminate(name, node);
                }
              } else {
                // replace it in-place
                safeCopy(node, value);
                varsToRemove[name] = 2;
              }
            } else {
              if (allowTracking) track(name, node[3], node);
            }
          } else if (target[0] == SUB) {
            if (isTempDoublePtrAccess(target)) {
              if (!globalsInvalidated) {
                invalidateGlobals();
                globalsInvalidated = true;
              }
            } else if (!memoryInvalidated) {
              invalidateMemory();
              memoryInvalidated = true;
            }
          }
        } else if (type == SUB) {
          traverseInOrder(node[1], false, !memSafe); // evaluate inner
          traverseInOrder(node[2], false, false); // evaluate outer
          // ignoreSub means we are a write (happening later), not a read
          if (!ignoreSub && !isTempDoublePtrAccess(node)) {
            // do the memory access
            if (!callsInvalidated) {
              invalidateCalls();
              callsInvalidated = true;
            }
          }
        } else if (type == VAR) {
          Ref vars = node[1];
          for (int i = 0; i < vars->size(); i++) {
            IString name = vars[i][0]->getIString();
            Ref value;
            if (vars[i]->size() > 1 && !!(value = vars[i][1])) {
              traverseInOrder(value, false, false);
              if (potentials.has(name) && allowTracking) {
                track(name, value, node);
              } else {
                invalidateByDep(name);
              }
              if (vars->size() == 1 && varsToTryToRemove.has(name) && !!value) {
                // replace it in-place
                value = make1(STAT, value);
                safeCopy(node, value);
                varsToRemove[name] = 2;
              }
            }
          }
        } else if (type == BINARY) {
          bool flipped = false;
          if (ASSOCIATIVE_BINARIES.has(node[1]) && !NAME_OR_NUM.has(node[2][0]) && NAME_OR_NUM.has(node[3][0])) { // TODO recurse here?
            // associatives like + and * can be reordered in the simple case of one of the sides being a name, since we assume they are all just numbers
            Ref temp = node[2];
            node[2] = node[3];
            node[3] = temp;
            flipped = true;
          }
          traverseInOrder(node[2], false, false);
          traverseInOrder(node[3], false, false);
          if (flipped && NAME_OR_NUM.has(node[2][0])) { // dunno if we optimized, but safe to flip back - and keeps the code closer to the original and more readable
            Ref temp = node[2];
            node[2] = node[3];
            node[3] = temp;
          }
        } else if (type == NAME) {
          if (!ignoreName) { // ignoreName means we are the name of something like a call or a sub - irrelevant for us
            IString name = node[1]->getIString();
            if (tracked.has(name)) {
              doEliminate(name, node);
            } else if (!asmData.isLocal(name) && !callsInvalidated) {
              invalidateCalls();
              callsInvalidated = true;
            }
          }
        } else if (type == UNARY_PREFIX || type == UNARY_POSTFIX) {
          traverseInOrder(node[2], false, false);
        } else if (IGNORABLE_ELIMINATOR_SCAN_NODES.has(type)) {
        } else if (type == CALL) {
          traverseInOrder(node[1], false, true);
          Ref args = node[2];
          for (int i = 0; i < args->size(); i++) {
            traverseInOrder(args[i], false, false);
          }
          if (callHasSideEffects(node)) {
            // these two invalidations will also invalidate calls
            if (!globalsInvalidated) {
              invalidateGlobals();
              globalsInvalidated = true;
            }
            if (!memoryInvalidated) {
              invalidateMemory();
              memoryInvalidated = true;
            }
          }
        } else if (type == IF) {
          if (allowTracking) {
            traverseInOrder(node[1], false, false); // can eliminate into condition, but nowhere else
            if (!callsInvalidated) { // invalidate calls, since we cannot eliminate them into an if that may not execute!
              invalidateCalls();
              callsInvalidated = true;
            }
            allowTracking = false;
            traverseInOrder(node[2], false, false); // 2 and 3 could be 'parallel', really..
            if (!!node[3]) traverseInOrder(node[3], false, false);
            allowTracking = true;
          } else {
            tracked.clear();
          }
        } else if (type == BLOCK) {
          Ref stats = getStatements(node);
          if (!!stats) {
            for (int i = 0; i < stats->size(); i++) {
              traverseInOrder(stats[i], false, false);
            }
          }
        } else if (type == STAT) {
          traverseInOrder(node[1], false, false);
        } else if (type == LABEL) {
          traverseInOrder(node[2], false, false);
        } else if (type == SEQ) {
          traverseInOrder(node[1], false, false);
          traverseInOrder(node[2], false, false);
        } else if (type == DO) {
          if (node[1][0] == NUM && node[1][1]->getNumber() == 0) { // one-time loop
            traverseInOrder(node[2], false, false);
          } else {
            tracked.clear();
          }
        } else if (type == RETURN) {
          if (!!node[1]) traverseInOrder(node[1], false, false);
        } else if (type == CONDITIONAL) {
          if (!callsInvalidated) { // invalidate calls, since we cannot eliminate them into a branch of an LLVM select/JS conditional that does not execute
            invalidateCalls();
            callsInvalidated = true;
          }
          traverseInOrder(node[1], false, false);
          traverseInOrder(node[2], false, false);
          traverseInOrder(node[3], false, false);
        } else if (type == SWITCH) {
          traverseInOrder(node[1], false, false);
          Tracked originalTracked = tracked;
          Ref cases = node[2];
          for (int i = 0; i < cases->size(); i++) {
            Ref c = cases[i];
            assert(c[0]->isNull() || c[0][0] == NUM || (c[0][0] == UNARY_PREFIX && c[0][2][0] == NUM));
            Ref stats = c[1];
            for (int j = 0; j < stats->size(); j++) {
              traverseInOrder(stats[j], false, false);
            }
            // We cannot track from one switch case into another, undo all new trackings TODO: general framework here, use in if-else as well
            std::vector<IString> toDelete;
            for (auto t : tracked) {
              if (!originalTracked.has(t.first)) {
                Tracking& info = tracked[t.first];
                if (info.usesGlobals || info.usesMemory || info.deps.size() > 0) {
                  toDelete.push_back(t.first);
                }
              }
            }
            for (auto t : toDelete) {
              tracked.erase(t);
            }
          }
          tracked.clear(); // do not track from inside the switch to outside
        } else {
          assert(ABORTING_ELIMINATOR_SCAN_NODES.has(type));
          tracked.clear();
          abort = true;
        }
      };
      traverseInOrder(node, false, false);
    };
    //var eliminationLimit = 0; // used to debugging purposes
    doEliminate = [&](IString name, Ref node) {
      //if (eliminationLimit == 0) return;
      //eliminationLimit--;
      //printErr('elim!!!!! ' + name);
      // yes, eliminate!
      varsToRemove[name] = 2; // both assign and var definitions can have other vars we must clean up
      assert(tracked.has(name));
      Tracking& info = tracked[name];
      Ref defNode = info.defNode;
      assert(!!defNode);
      if (!sideEffectFree.has(name)) {
        assert(defNode[0] != VAR);
        // assign
        Ref value = defNode[3];
        // wipe out the assign
        safeCopy(defNode, makeEmpty());
        // replace this node in-place
        safeCopy(node, value);
      } else {
        // This has no side effects and no uses, empty it out in-place
        safeCopy(node, makeEmpty());
      }
      tracked.erase(name);
    };
    traversePre(func, [&](Ref block) {
      // Look for statements, including while-switch pattern
      Ref stats = getStatements(block);
      if (!stats && (block[0] == WHILE && block[2][0] == SWITCH)) {
        stats = &(makeArray()->push_back(block[2]));
      }
      if (!stats) return;
      tracked.clear();
      for (int i = 0; i < stats->size(); i++) {
        Ref node = deStat(stats[i]);
        Ref type = node[0];
        if (type == RETURN && i < stats->size()-1) {
          stats->setSize(i+1); // remove any code after a return
        }
        // Check for things that affect elimination
        if (ELIMINATION_SAFE_NODES.has(type)) {
          scan(node);
        } else {
          tracked.clear(); // not a var or assign, break all potential elimination so far
        }
      }
    });

    StringIntMap seenUses;
    StringStringMap helperReplacements; // for looper-helper optimization

    // clean up vars, and loop variable elimination
    traversePrePost(func, [&](Ref node) {
      // pre
      Ref type = node[0];
      /*if (type == VAR) {
        node[1] = node[1].filter(function(pair) { return !varsToRemove[pair[0]] });
        if (node[1]->size() == 0) {
          // wipe out an empty |var;|
          node[0] = 'toplevel';
          node[1] = [];
        }
      } else */
      if (type == ASSIGN && node[1]->isBool(true) && node[2][0] == NAME && node[3][0] == NAME && node[2][1] == node[3][1]) {
        // elimination led to X = X, which we can just remove
        safeCopy(node, makeEmpty());
      }
    }, [&](Ref node) {
      // post
      Ref type = node[0];
      if (type == NAME) {
        IString name = node[1]->getIString();
        if (helperReplacements.has(name)) {
          node[1]->setString(helperReplacements[name]);
          return; // no need to track this anymore, we can't loop-optimize more than once
        }
        // track how many uses we saw. we need to know when a variable is no longer used (hence we run this in the post)
        seenUses[name]++;
      } else if (type == WHILE) {
        // try to remove loop helper variables specifically
        Ref stats = node[2][1];
        Ref last = stats->back();
        if (!!last && last[0] == IF && last[2][0] == BLOCK && !!last[3] && last[3][0] == BLOCK) {
          Ref ifTrue = last[2];
          Ref ifFalse = last[3];
          clearEmptyNodes(ifTrue[1]);
          clearEmptyNodes(ifFalse[1]);
          bool flip = false;
          if (ifFalse[1]->size() > 0 && !!ifFalse[1][0] && !!ifFalse[1]->back() && ifFalse[1]->back()[0] == BREAK) { // canonicalize break in the if-true
            Ref temp = ifFalse;
            ifFalse = ifTrue;
            ifTrue = temp;
            flip = true;
          }
          if (ifTrue[1]->size() > 0 && !!ifTrue[1][0] && !!ifTrue[1]->back() && ifTrue[1]->back()[0] == BREAK) {
            Ref assigns = ifFalse[1];
            clearEmptyNodes(assigns);
            std::vector<IString> loopers, helpers;
            for (int i = 0; i < assigns->size(); i++) {
              if (assigns[i][0] == STAT && assigns[i][1][0] == ASSIGN) {
                Ref assign = assigns[i][1];
                if (assign[1]->isBool(true) && assign[2][0] == NAME && assign[3][0] == NAME) {
                  IString looper = assign[2][1]->getIString();
                  IString helper = assign[3][1]->getIString();
                  if (definitions[helper] == 1 && seenUses[looper] == namings[looper] &&
                      !helperReplacements.has(helper) && !helperReplacements.has(looper)) {
                    loopers.push_back(looper);
                    helpers.push_back(helper);
                  }
                }
              }
            }
            // remove loop vars that are used in the rest of the else
            for (int i = 0; i < assigns->size(); i++) {
              if (assigns[i][0] == STAT && assigns[i][1][0] == ASSIGN) {
                Ref assign = assigns[i][1];
                if (!(assign[1]->isBool(true) && assign[2][0] == NAME && assign[3][0] == NAME) || indexOf(loopers, assign[2][1]->getIString()) < 0) {
                  // this is not one of the loop assigns
                  traversePre(assign, [&](Ref node) {
                    if (node[0] == NAME) {
                      int index = indexOf(loopers, node[1]->getIString());
                      if (index < 0) index = indexOf(helpers, node[1]->getIString());
                      if (index >= 0) {
                        loopers.erase(loopers.begin() + index);
                        helpers.erase(helpers.begin() + index);
                      }
                    }
                  });
                }
              }
            }
            // remove loop vars that are used in the if
            traversePre(ifTrue, [&](Ref node) {
              if (node[0] == NAME) {
                int index = indexOf(loopers, node[1]->getIString());
                if (index < 0) index = indexOf(helpers, node[1]->getIString());
                if (index >= 0) {
                  loopers.erase(loopers.begin() + index);
                  helpers.erase(helpers.begin() + index);
                }
              }
            });
            if (loopers.size() == 0) return;
            for (int l = 0; l < loopers.size(); l++) {
              IString looper = loopers[l];
              IString helper = helpers[l];
              // the remaining issue is whether loopers are used after the assignment to helper and before the last line (where we assign to it)
              int found = -1;
              for (int i = stats->size()-2; i >= 0; i--) {
                Ref curr = stats[i];
                if (curr[0] == STAT && curr[1][0] == ASSIGN) {
                  Ref currAssign = curr[1];
                  if (currAssign[1]->isBool(true) && currAssign[2][0] == NAME) {
                    IString to = currAssign[2][1]->getIString();
                    if (to == helper) {
                      found = i;
                      break;
                    }
                  }
                }
              }
              if (found < 0) return;
              // if a loop variable is used after we assigned to the helper, we must save its value and use that.
              // (note that this can happen due to elimination, if we eliminate an expression containing the
              // loop var far down, past the assignment!)
              // first, see if the looper and helpers overlap. Note that we check for this looper, compared to
              // *ALL* the helpers. Helpers will be replaced by loopers as we eliminate them, potentially
              // causing conflicts, so any helper is a concern.
              int firstLooperUsage = -1;
              int lastLooperUsage = -1;
              int firstHelperUsage = -1;
              for (int i = found+1; i < stats->size(); i++) {
                Ref curr = i < stats->size()-1 ? stats[i] : last[1]; // on the last line, just look in the condition
                traversePre(curr, [&](Ref node) {
                  if (node[0] == NAME) {
                    if (node[1] == looper) {
                      if (firstLooperUsage < 0) firstLooperUsage = i;
                      lastLooperUsage = i;
                    } else if (indexOf(helpers, node[1]->getIString()) >= 0) {
                      if (firstHelperUsage < 0) firstHelperUsage = i;
                    }
                  }
                });
              }
              if (firstLooperUsage >= 0) {
                // the looper is used, we cannot simply merge the two variables
                if ((firstHelperUsage < 0 || firstHelperUsage > lastLooperUsage) && lastLooperUsage+1 < stats->size() && triviallySafeToMove(stats[found], asmData) &&
                    seenUses[helper] == namings[helper]) {
                  // the helper is not used, or it is used after the last use of the looper, so they do not overlap,
                  // and the last looper usage is not on the last line (where we could not append after it), and the
                  // helper is not used outside of the loop.
                  // just move the looper definition to after the looper's last use
                  stats->insert(lastLooperUsage+1, stats[found]);
                  stats->splice(found, 1);
                } else {
                  // they overlap, we can still proceed with the loop optimization, but we must introduce a
                  // loop temp helper variable
                  IString temp(strdupe((std::string(looper.c_str()) + "$looptemp").c_str()));
                  assert(!asmData.isLocal(temp));
                  for (int i = firstLooperUsage; i <= lastLooperUsage; i++) {
                    Ref curr = i < stats->size()-1 ? stats[i] : last[1]; // on the last line, just look in the condition

                    std::function<bool (Ref)> looperToLooptemp = [&](Ref node) {
                      if (node[0] == NAME) {
                        if (node[1] == looper) {
                          node[1]->setString(temp);
                        }
                      } else if (node[0] == ASSIGN && node[2][0] == NAME) {
                        // do not traverse the assignment target, phi assignments to the loop variable must remain
                        traversePrePostConditional(node[3], looperToLooptemp, [](Ref node){});
                        return false;
                      }
                      return true;
                    };
                    traversePrePostConditional(curr, looperToLooptemp, [](Ref node){});
                  }
                  asmData.addVar(temp, asmData.getType(looper));
                  stats->insert(found, make1(STAT, make3(ASSIGN, &(arena.alloc())->setBool(true), makeName(temp), makeName(looper))));
                }
              }
            }
            for (int l = 0; l < helpers.size(); l++) {
              for (int k = 0; k < helpers.size(); k++) {
                if (l != k && helpers[l] == helpers[k]) return; // it is complicated to handle a shared helper, abort
              }
            }
            // hurrah! this is safe to do
            for (int l = 0; l < loopers.size(); l++) {
              IString looper = loopers[l];
              IString helper = helpers[l];
              varsToRemove[helper] = 2;
              traversePre(node, [&](Ref node) { // replace all appearances of helper with looper
                if (node[0] == NAME && node[1] == helper) node[1]->setString(looper);
              });
              helperReplacements[helper] = looper; // replace all future appearances of helper with looper
              helperReplacements[looper] = looper; // avoid any further attempts to optimize looper in this manner (seenUses is wrong anyhow, too)
            }
            // simplify the if. we remove the if branch, leaving only the else
            if (flip) {
              last[1] = simplifyNotCompsDirect(make2(UNARY_PREFIX, L_NOT, last[1]));
              Ref temp = last[2];
              last[2] = last[3];
              last[3] = temp;
            }
            if (loopers.size() == assigns->size()) {
              last->pop_back();
            } else {
              Ref elseStats = getStatements(last[3]);
              for (int i = 0; i < elseStats->size(); i++) {
                Ref stat = deStat(elseStats[i]);
                if (stat[0] == ASSIGN && stat[2][0] == NAME) {
                  if (indexOf(loopers, stat[2][1]->getIString()) >= 0) {
                    elseStats[i] = makeEmpty();
                  }
                }
              }
            }
          }
        }
      }
    });

    for (auto v : varsToRemove) {
      if (v.second == 2 && asmData.isVar(v.first)) asmData.deleteVar(v.first);
    }

    asmData.denormalize();
  });

  removeAllEmptySubNodes(ast);
}

void eliminateMemSafe(Ref ast) {
  eliminate(ast, true);
}

void simplifyExpressions(Ref ast) {
  // Simplify common expressions used to perform integer conversion operations
  // in cases where no conversion is needed.
  auto simplifyIntegerConversions = [](Ref ast) {
    traversePre(ast, [](Ref node) {
      Ref type = node[0];
      if (type == BINARY       && node[1]    == RSHIFT && node[3][0] == NUM &&
          node[2][0] == BINARY && node[2][1] == LSHIFT && node[2][3][0] == NUM && node[3][1]->getNumber() == node[2][3][1]->getNumber()) {
        // Transform (x&A)<<B>>B to X&A.
        Ref innerNode = node[2][2];
        double shifts = node[3][1]->getNumber();
        if (innerNode[0] == BINARY && innerNode[1] == AND && innerNode[3][0] == NUM) {
          double mask = innerNode[3][1]->getNumber();
          if (isInteger32(mask) && isInteger32(shifts) && ((jsD2I(mask) << jsD2I(shifts)) >> jsD2I(shifts)) == jsD2I(mask)) {
            safeCopy(node, innerNode);
            return;
          }
        }
      } else if (type == BINARY && BITWISE.has(node[1])) {
        for (int i = 2; i <= 3; i++) {
          Ref subNode = node[i];
          if (subNode[0] == BINARY && subNode[1] == AND && subNode[3][0] == NUM && subNode[3][1]->getNumber() == 1) {
            // Rewrite (X < Y) & 1 to X < Y , when it is going into a bitwise operator. We could
            // remove even more (just replace &1 with |0, then subsequent passes could remove the |0)
            // but v8 issue #2513 means the code would then run very slowly in chrome.
            Ref input = subNode[2];
            if (input[0] == BINARY && COMPARE_OPS.has(input[1])) {
              safeCopy(node[i], input);
            }
          }
        }
      }
    });
  };

  // When there is a bunch of math like (((8+5)|0)+12)|0, only the external |0 is needed, one correction is enough.
  // At each node, ((X|0)+Y)|0 can be transformed into (X+Y): The inner corrections are not needed
  // TODO: Is the same is true for 0xff, 0xffff?
  // Likewise, if we have |0 inside a block that will be >>'d, then the |0 is unnecessary because some
  // 'useful' mathops already |0 anyhow.

  auto simplifyOps = [](Ref ast) {
    auto removeMultipleOrZero = [&ast] {
      bool rerun = true;
      while (rerun) {
        rerun = false;
        std::vector<int> stack;
        std::function<void (Ref)> process = [&stack, &rerun, &process, &ast](Ref node) {
          Ref type = node[0];
          if (type == BINARY && node[1] == OR) {
            if (node[2][0] == NUM && node[3][0] == NUM) {
              node[2][1]->setNumber(jsD2I(node[2][1]->getNumber()) | jsD2I(node[3][1]->getNumber()));
              stack.push_back(0);
              safeCopy(node, node[2]);
              return;
            }
            bool go = false;
            if (node[2][0] == NUM && node[2][1]->getNumber() == 0) {
              // canonicalize order
              Ref temp = node[3];
              node[3] = node[2];
              node[2] = temp;
              go = true;
            } else if (node[3][0] == NUM && node[3][1]->getNumber() == 0) {
              go = true;
            }
            if (!go) {
              stack.push_back(1);
              return;
            }
            // We might be able to remove this correction
            for (int i = stack.size()-1; i >= 0; i--) {
              if (stack[i] >= 1) {
                if (stack.back() < 2 && node[2][0] == CALL) break; // we can only remove multiple |0s on these
                if (stack.back() < 1 && (COERCION_REQUIRING_OPS.has(node[2][0]) ||
                                                 (node[2][0] == BINARY && COERCION_REQUIRING_BINARIES.has(node[2][1])))) break; // we can remove |0 or >>2
                // we will replace ourselves with the non-zero side. Recursively process that node.
                Ref result = node[2][0] == NUM && node[2][1]->getNumber() == 0 ? node[3] : node[2], other;
                // replace node in-place
                safeCopy(node, result);
                rerun = true;
                process(result);
                return;
              } else if (stack[i] == -1) {
                break; // Too bad, we can't
              }
            }
            stack.push_back(2); // From here on up, no need for this kind of correction, it's done at the top
                           // (Add this at the end, so it is only added if we did not remove it)
          } else if (type == BINARY && USEFUL_BINARY_OPS.has(node[1])) {
            stack.push_back(1);
          } else if ((type == BINARY && SAFE_BINARY_OPS.has(node[1])) || type == NUM || type == NAME) {
            stack.push_back(0); // This node is safe in that it does not interfere with this optimization
          } else if (type == UNARY_PREFIX && node[1] == B_NOT) {
            stack.push_back(1);
          } else {
            stack.push_back(-1); // This node is dangerous! Give up if you see this before you see '1'
          }
        };

        traversePrePost(ast, process, [&stack](Ref Node) {
          stack.pop_back();
        });
      }
    };

    removeMultipleOrZero();

    // & and heap-related optimizations

    bool hasTempDoublePtr = false, rerunOrZeroPass = false;

    traversePrePostConditional(ast, [](Ref node) {
      // Detect trees which should not
      // be simplified.
      if (node[0] == SUB && node[1][0] == NAME && isFunctionTable(node[1][1])) {
        return false; // do not traverse subchildren here, we should not collapse 55 & 126.
      }
      return true;
    }, [&hasTempDoublePtr, &rerunOrZeroPass](Ref node) {
      // Simplifications are done now so
      // that we simplify a node's operands before the node itself. This allows
      // optimizations to cascade.
      Ref type = node[0];
      if (type == NAME) {
        if (node[1] == TEMP_DOUBLE_PTR) hasTempDoublePtr = true;
      } else if (type == BINARY && node[1] == AND && node[3][0] == NUM) {
        if (node[2][0] == NUM) {
          safeCopy(node, makeNum(jsD2I(node[2][1]->getNumber()) & jsD2I(node[3][1]->getNumber())));
          return;
        }
        Ref input = node[2];
        double amount = node[3][1]->getNumber();
        if (input[0] == BINARY && input[1] == AND && input[3][0] == NUM) {
          // Collapse X & 255 & 1
          node[3][1]->setNumber(jsD2I(amount) & jsD2I(input[3][1]->getNumber()));
          node[2] = input[2];
        } else if (input[0] == SUB && input[1][0] == NAME) {
          // HEAP8[..] & 255 => HEAPU8[..]
          HeapInfo hi = parseHeap(input[1][1]->getCString());
          if (hi.valid) {
            if (isInteger32(amount) && amount == powl(2, hi.bits)-1) {
              if (!hi.unsign) {
                input[1][1]->setString(getHeapStr(hi.bits, true)); // make unsigned
              }
              // we cannot return HEAPU8 without a coercion, but at least we do HEAP8 & 255 => HEAPU8 | 0
              node[1]->setString(OR);
              node[3][1]->setNumber(0);
              return;
            }
          }
        }
      } else if (type == BINARY && node[1] == XOR) {
        // LLVM represents bitwise not as xor with -1. Translate it back to an actual bitwise not.
        if (node[3][0] == UNARY_PREFIX && node[3][1] == MINUS && node[3][2][0] == NUM &&
            node[3][2][1]->getNumber() == 1 &&
            !(node[2][0] == UNARY_PREFIX && node[2][1] == B_NOT)) { // avoid creating ~~~ which is confusing for asm given the role of ~~
          safeCopy(node, make2(UNARY_PREFIX, B_NOT, node[2]));
          return;
        }
      } else if (type       == BINARY && node[1]    == RSHIFT && node[3][0]    == NUM &&
                 node[2][0] == BINARY && node[2][1] == LSHIFT && node[2][3][0] == NUM &&
                 node[2][2][0] == SUB && node[2][2][1][0] == NAME) {
        // collapse HEAPU?8[..] << 24 >> 24 etc. into HEAP8[..] | 0
        double amount = node[3][1]->getNumber();
        if (amount == node[2][3][1]->getNumber()) {
          HeapInfo hi = parseHeap(node[2][2][1][1]->getCString());
          if (hi.valid && hi.bits == 32 - amount) {
            node[2][2][1][1]->setString(getHeapStr(hi.bits, false));
            node[1]->setString(OR);
            node[2] = node[2][2];
            node[3][1]->setNumber(0);
            rerunOrZeroPass = true;
            return;
          }
        }
      } else if (type == ASSIGN) {
        // optimizations for assigning into HEAP32 specifically
        if (node[1]->isBool(true) && node[2][0] == SUB && node[2][1][0] == NAME) {
          if (node[2][1][1] == HEAP32) {
            // HEAP32[..] = x | 0 does not need the | 0 (unless it is a mandatory |0 of a call)
            if (node[3][0] == BINARY && node[3][1] == OR) {
              if (node[3][2][0] == NUM && node[3][2][1]->getNumber() == 0 && node[3][3][0] != CALL) {
                node[3] = node[3][3];
              } else if (node[3][3][0] == NUM && node[3][3][1]->getNumber() == 0 && node[3][2][0] != CALL) {
                node[3] = node[3][2];
              }
            }
          } else if (node[2][1][1] == HEAP8) {
            // HEAP8[..] = x & 0xff does not need the & 0xff
            if (node[3][0] == BINARY && node[3][1] == AND && node[3][3][0] == NUM && node[3][3][1]->getNumber() == 0xff) {
              node[3] = node[3][2];
            }
          } else if (node[2][1][1] == HEAP16) {
            // HEAP16[..] = x & 0xffff does not need the & 0xffff
            if (node[3][0] == BINARY && node[3][1] == AND && node[3][3][0] == NUM && node[3][3][1]->getNumber() == 0xffff) {
              node[3] = node[3][2];
            }
          }
        }
        Ref value = node[3];
        if (value[0] == BINARY && value[1] == OR) {
          // canonicalize order of |0 to end
          if (value[2][0] == NUM && value[2][1]->getNumber() == 0) {
            Ref temp = value[2];
            value[2] = value[3];
            value[3] = temp;
          }
          // if a seq ends in an |0, remove an external |0
          // note that it is only safe to do this in assigns, like we are doing here (return (x, y|0); is not valid)
          if (value[2][0] == SEQ && value[2][2][0] == BINARY && USEFUL_BINARY_OPS.has(value[2][2][1])) {
            node[3] = value[2];
          }
        }
      } else if (type == BINARY && node[1] == RSHIFT && node[2][0] == NUM && node[3][0] == NUM) {
        // optimize num >> num, in asm we need this since we do not run optimizeShifts
        node[0]->setString(NUM);
        node[1]->setNumber(jsD2I(node[2][1]->getNumber()) >> jsD2I(node[3][1]->getNumber()));
        node->setSize(2);
        return;
      } else if (type == BINARY && node[1] == PLUS) {
        // The most common mathop is addition, e.g. in getelementptr done repeatedly. We can join all of those,
        // by doing (num+num) ==> newnum, and (name+num)+num = name+newnum
        if (node[2][0] == NUM && node[3][0] == NUM) {
          node[2][1]->setNumber(jsD2I(node[2][1]->getNumber()) + jsD2I(node[3][1]->getNumber()));
          safeCopy(node, node[2]);
          return;
        }
        for (int i = 2; i <= 3; i++) {
          int ii = 5-i;
          for (int j = 2; j <= 3; j++) {
            if (node[i][0] == NUM && node[ii][0] == BINARY && node[ii][1] == PLUS && node[ii][j][0] == NUM) {
              node[ii][j][1]->setNumber(jsD2I(node[ii][j][1]->getNumber()) + jsD2I(node[i][1]->getNumber()));
              safeCopy(node, node[ii]);
              return;
            }
          }
        }
      }
    });

    if (rerunOrZeroPass) removeMultipleOrZero();

    if (hasTempDoublePtr) {
      AsmData asmData(ast);
      traversePre(ast, [](Ref node) {
        Ref type = node[0];
        if (type == ASSIGN) {
          if (node[1]->isBool(true) && node[2][0] == SUB && node[2][1][0] == NAME && node[2][1][1] == HEAP32) {
            // remove bitcasts that are now obviously pointless, e.g.
            // HEAP32[$45 >> 2] = HEAPF32[tempDoublePtr >> 2] = ($14 < $28 ? $14 : $28) - $42, HEAP32[tempDoublePtr >> 2] | 0;
            Ref value = node[3];
            if (value[0] == SEQ && value[1][0] == ASSIGN && value[1][2][0] == SUB && value[1][2][1][0] == NAME && value[1][2][1][1] == HEAPF32 &&
                value[1][2][2][0] == BINARY && value[1][2][2][2][0] == NAME && value[1][2][2][2][1] == TEMP_DOUBLE_PTR) {
              // transform to HEAPF32[$45 >> 2] = ($14 < $28 ? $14 : $28) - $42;
              node[2][1][1]->setString(HEAPF32);
              node[3] = value[1][3];
            }
          }
        } else if (type == SEQ) {
          // (HEAP32[tempDoublePtr >> 2] = HEAP32[$37 >> 2], +HEAPF32[tempDoublePtr >> 2])
          //   ==>
          // +HEAPF32[$37 >> 2]
          if (node[0] == SEQ && node[1][0] == ASSIGN && node[1][2][0] == SUB && node[1][2][1][0] == NAME &&
              (node[1][2][1][1] == HEAP32 || node[1][2][1][1] == HEAPF32) &&
              node[1][2][2][0] == BINARY && node[1][2][2][2][0] == NAME && node[1][2][2][2][1] == TEMP_DOUBLE_PTR &&
              node[1][3][0] == SUB && node[1][3][1][0] == NAME && (node[1][3][1][1] == HEAP32 || node[1][3][1][1] == HEAPF32) &&
              node[2][0] != SEQ) { // avoid (x, y, z) which can be used for tempDoublePtr on doubles for alignment fixes
            if (node[1][2][1][1] == HEAP32) {
              node[1][3][1][1]->setString(HEAPF32);
              safeCopy(node, makeAsmCoercion(node[1][3], detectType(node[2])));
              return;
            } else {
              node[1][3][1][1]->setString(HEAP32);
              safeCopy(node, make3(BINARY, OR, node[1][3], makeNum(0)));
              return;
            }
          }
        }
      });

      // finally, wipe out remaining ones by finding cases where all assignments to X are bitcasts, and all uses are writes to
      // the other heap type, then eliminate the bitcast
      struct BitcastData {
        int define_HEAP32, define_HEAPF32, use_HEAP32, use_HEAPF32, namings;
        bool ok;
        std::vector<Ref> defines, uses;

        BitcastData() : define_HEAP32(0), define_HEAPF32(0), use_HEAP32(0), use_HEAPF32(0), namings(0), ok(false) {}
      };
      std::unordered_map<IString, BitcastData> bitcastVars;
      traversePre(ast, [&bitcastVars](Ref node) {
        if (node[0] == ASSIGN && node[1]->isBool(true) && node[2][0] == NAME) {
          Ref value = node[3];
          if (value[0] == SEQ && value[1][0] == ASSIGN && value[1][2][0] == SUB && value[1][2][1][0] == NAME &&
              (value[1][2][1][1] == HEAP32 || value[1][2][1][1] == HEAPF32) &&
              value[1][2][2][0] == BINARY && value[1][2][2][2][0] == NAME && value[1][2][2][2][1] == TEMP_DOUBLE_PTR) {
            IString name = node[2][1]->getIString();
            IString heap = value[1][2][1][1]->getIString();
            if (heap == HEAP32) {
              bitcastVars[name].define_HEAP32++;
            } else {
              assert(heap == HEAPF32);
              bitcastVars[name].define_HEAPF32++;
            }
            bitcastVars[name].defines.push_back(node);
            bitcastVars[name].ok = true;
          }
        }
      });
      traversePre(ast, [&bitcastVars](Ref node) {
        Ref type = node[0];
        if (type == NAME && bitcastVars[node[1]->getCString()].ok) {
          bitcastVars[node[1]->getCString()].namings++;
        } else if (type == ASSIGN && node[1]->isBool(true)) {
          Ref value = node[3];
          if (value[0] == NAME) {
            IString name = value[1]->getIString();
            if (bitcastVars[name].ok) {
              Ref target = node[2];
              if (target[0] == SUB && target[1][0] == NAME && (target[1][1] == HEAP32 || target[1][1] == HEAPF32)) {
                if (target[1][1] == HEAP32) {
                  bitcastVars[name].use_HEAP32++;
                } else {
                  bitcastVars[name].use_HEAPF32++;
                }
                bitcastVars[name].uses.push_back(node);
              }
            }
          }
        }
      });
      for (auto iter : bitcastVars) {
        const IString& v = iter.first;
        BitcastData& info = iter.second;
        // good variables define only one type, use only one type, have definitions and uses, and define as a different type than they use
        if (info.define_HEAP32*info.define_HEAPF32 == 0 && info.use_HEAP32*info.use_HEAPF32 == 0 &&
            info.define_HEAP32+info.define_HEAPF32 > 0  && info.use_HEAP32+info.use_HEAPF32 > 0 &&
            info.define_HEAP32*info.use_HEAP32 == 0 && info.define_HEAPF32*info.use_HEAPF32 == 0 &&
            asmData.isLocal(v.c_str()) && info.namings == info.define_HEAP32+info.define_HEAPF32+info.use_HEAP32+info.use_HEAPF32) {
          IString& correct = info.use_HEAP32 ? HEAPF32 : HEAP32;
          for (auto define : info.defines) {
            define[3] = define[3][1][3];
            if (correct == HEAP32) {
              define[3] = make3(BINARY, OR, define[3], makeNum(0));
            } else {
              assert(correct == HEAPF32);
              define[3] = makeAsmCoercion(define[3], preciseF32 ? ASM_FLOAT : ASM_DOUBLE);
            }
            // do we want a simplifybitops on the new values here?
          }
          for (auto use : info.uses) {
            use[2][1][1]->setString(correct.c_str());
          }
          AsmType correctType;
          switch(asmData.getType(v.c_str())) {
            case ASM_INT: correctType = preciseF32 ? ASM_FLOAT : ASM_DOUBLE; break;
            case ASM_FLOAT: case ASM_DOUBLE: correctType = ASM_INT; break;
            default: {} // pass
          }
          asmData.setType(v.c_str(), correctType);
        }
      }
      asmData.denormalize();
    }
  };

  std::function<bool (Ref)> emitsBoolean = [&emitsBoolean](Ref node) {
    Ref type = node[0];
    if (type == NUM) {
      return node[1]->getNumber() == 0 || node[1]->getNumber() == 1;
    }
    if (type == BINARY) return COMPARE_OPS.has(node[1]);
    if (type == UNARY_PREFIX) return node[1] == L_NOT;
    if (type == CONDITIONAL) return emitsBoolean(node[2]) && emitsBoolean(node[3]);
    return false;
  };

  //   expensive | expensive can be turned into expensive ? 1 : expensive, and
  //   expensive | cheap     can be turned into cheap     ? 1 : expensive,
  // so that we can avoid the expensive computation, if it has no side effects.
  auto conditionalize = [&emitsBoolean](Ref ast) {
    const int MIN_COST = 7;
    traversePre(ast, [&emitsBoolean](Ref node) {
      if (node[0] == BINARY && (node[1] == OR || node[1] == AND) && node[3][0] != NUM && node[2][0] != NUM) {
        // logical operator on two non-numerical values
        Ref left = node[2];
        Ref right = node[3];
        if (!emitsBoolean(left) || !emitsBoolean(right)) return;
        bool leftEffects = hasSideEffects(left);
        bool rightEffects = hasSideEffects(right);
        if (leftEffects && rightEffects) return; // both must execute
        // canonicalize with side effects, if any, happening on the left
        if (rightEffects) {
          if (measureCost(left) < MIN_COST) return; // avoidable code is too cheap
          Ref temp = left;
          left = right;
          right = temp;
        } else if (leftEffects) {
          if (measureCost(right) < MIN_COST) return; // avoidable code is too cheap
        } else {
          // no side effects, reorder based on cost estimation
          int leftCost = measureCost(left);
          int rightCost = measureCost(right);
          if (std::max(leftCost, rightCost) < MIN_COST) return; // avoidable code is too cheap
          // canonicalize with expensive code on the right
          if (leftCost > rightCost) {
            Ref temp = left;
            left = right;
            right = temp;
          }
        }
        // worth it, perform conditionalization
        Ref ret;
        if (node[1] == OR) {
          ret = make3(CONDITIONAL, left, makeNum(1), right);
        } else { // &
          ret = make3(CONDITIONAL, left, right, makeNum(0));
        }
        if (left[0] == UNARY_PREFIX && left[1] == L_NOT) {
          ret[1] = flipCondition(left);
          Ref temp = ret[2];
          ret[2] = ret[3];
          ret[3] = temp;
        }
        safeCopy(node, ret);
        return;
      }
    });
  };

  traverseFunctions(ast, [&](Ref func) {
    simplifyIntegerConversions(func);
    simplifyOps(func);
    traversePre(func, [](Ref node) {
      Ref ret = simplifyNotCompsDirect(node);
      if (ret.get() != node.get()) { // if we received a different pointer in return, then we need to copy the new value
        safeCopy(node, ret);
      }
    });
    conditionalize(func);
  });
}

void simplifyIfs(Ref ast) {
  traverseFunctions(ast, [](Ref func) {
    bool simplifiedAnElse = false;

    traversePre(func, [&simplifiedAnElse](Ref node) {
      // simplify   if (x) { if (y) { .. } }   to   if (x ? y : 0) { .. }
      if (node[0] == IF) {
        Ref body = node[2];
        // recurse to handle chains
        while (body[0] == BLOCK) {
          Ref stats = body[1];
          if (stats->size() == 0) break;
          Ref other = stats->back();
          if (other[0] != IF) {
            // our if block does not end with an if. perhaps if have an else we can flip
            if (node->size() > 3 && !!node[3] && node[3][0] == BLOCK) {
              stats = node[3][1];
              if (stats->size() == 0) break;
              other = stats->back();
              if (other[0] == IF) {
                // flip node
                node[1] = flipCondition(node[1]);
                node[2] = node[3];
                node[3] = body;
                body = node[2];
              } else break;
            } else break;
          }
          // we can handle elses, but must be fully identical
          if (!!node[3] || !!other[3]) {
            if (!node[3]) break;
            if (!node[3]->deepCompare(other[3])) {
              // the elses are different, but perhaps if we flipped a condition we can do better
              if (node[3]->deepCompare(other[2])) {
                // flip other. note that other may not have had an else! add one if so; we will eliminate such things later
                if (!other[3]) other[3] = makeBlock();
                other[1] = flipCondition(other[1]);
                Ref temp = other[2];
                other[2] = other[3];
                other[3] = temp;
              } else break;
            }
          }
          if (stats->size() > 1) {
            // try to commaify - turn everything between the ifs into a comma operator inside the second if
            bool ok = true;
            for (int i = 0; i < stats->size()-1; i++) {
              Ref curr = deStat(stats[i]);
              if (!commable(curr)) ok = false;
            }
            if (!ok) break;
            for (int i = stats->size()-2; i >= 0; i--) {
              Ref curr = deStat(stats[i]);
              other[1] = make2(SEQ, curr, other[1]);
            }
            Ref temp = makeArray();
            temp->push_back(other);
            stats = body[1] = temp;
          }
          if (stats->size() != 1) break;
          if (!!node[3]) simplifiedAnElse = true;
          node[1] = make3(CONDITIONAL, node[1], other[1], makeNum(0));
          body = node[2] = other[2];
        }
      }
    });

    if (simplifiedAnElse) {
      // there may be fusing opportunities

      // we can only fuse if we remove all uses of the label. if there are
      // other ones - if the label check can be reached from elsewhere -
      // we must leave it
      bool abort = false;

      std::unordered_map<int, int> labelAssigns;

      traversePre(func, [&labelAssigns, &abort](Ref node) {
        if (node[0] == ASSIGN && node[2][0] == NAME && node[2][1] == LABEL) {
          if (node[3][0] == NUM) {
            int value = node[3][1]->getNumber();
            labelAssigns[value] = labelAssigns[value] + 1;
          } else {
            // label is assigned a dynamic value (like from indirectbr), we cannot do anything
            abort = true;
          }
        }
      });
      if (abort) return;

      std::unordered_map<int, int> labelChecks;

      traversePre(func, [&labelChecks, &abort](Ref node) {
        if (node[0] == BINARY && node[1] == EQ && node[2][0] == BINARY && node[2][1] == OR &&
            node[2][2][0] == NAME && node[2][2][1] == LABEL) {
          if (node[3][0] == NUM) {
            int value = node[3][1]->getNumber();
            labelChecks[value] = labelChecks[value] + 1;
          } else {
            // label is checked vs a dynamic value (like from indirectbr), we cannot do anything
            abort = true;
          }
        }
      });
      if (abort) return;

      int inLoop = 0; // when in a loop, we do not emit   label = 0;   in the relooper as there is no need
      traversePrePost(func, [&inLoop, &labelAssigns, &labelChecks](Ref node) {
        if (node[0] == WHILE) inLoop++;
        Ref stats = getStatements(node);
        if (!!stats && stats->size() > 0) {
          for (int i = 0; i < stats->size()-1; i++) {
            Ref pre = stats[i];
            Ref post = stats[i+1];
            if (pre[0] == IF && pre->size() > 3 && !!pre[3] && post[0] == IF && (post->size() <= 3 || !post[3])) {
              Ref postCond = post[1];
              if (postCond[0] == BINARY && postCond[1] == EQ &&
                  postCond[2][0] == BINARY && postCond[2][1] == OR &&
                  postCond[2][2][0] == NAME && postCond[2][2][1] == LABEL &&
                  postCond[2][3][0] == NUM && postCond[2][3][1]->getNumber() == 0 &&
                  postCond[3][0] == NUM) {
                double postValue = postCond[3][1]->getNumber();
                Ref preElse = pre[3];
                if (labelAssigns[postValue] == 1 && labelChecks[postValue] == 1 && preElse[0] == BLOCK && preElse->size() >= 2 && preElse[1]->size() == 1) {
                  Ref preStat = preElse[1][0];
                  if (preStat[0] == STAT && preStat[1][0] == ASSIGN &&
                      preStat[1][1]->isBool(true) && preStat[1][2][0] == NAME && preStat[1][2][1] == LABEL &&
                      preStat[1][3][0] == NUM && preStat[1][3][1]->getNumber() == postValue) {
                    // Conditions match, just need to make sure the post clears label
                    if (post[2][0] == BLOCK && post[2]->size() >= 2 && post[2][1]->size() > 0) {
                      Ref postStat = post[2][1][0];
                      bool haveClear =
                        postStat[0] == STAT && postStat[1][0] == ASSIGN &&
                        postStat[1][1]->isBool(true) && postStat[1][2][0] == NAME && postStat[1][2][1] == LABEL &&
                        postStat[1][3][0] == NUM && postStat[1][3][1]->getNumber() == 0;
                      if (!inLoop || haveClear) {
                        // Everything lines up, do it
                        pre[3] = post[2];
                        if (haveClear) pre[3][1]->splice(0, 1); // remove the label clearing
                        stats->splice(i+1, 1); // remove the post entirely
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }, [&inLoop](Ref node) {
        if (node[0] == WHILE) inLoop--;
      });
      assert(inLoop == 0);
    }
  });
}

void optimizeFrounds(Ref ast) {
  // collapse fround(fround(..)), which can happen due to elimination
  // also emit f0 instead of fround(0) (except in returns)
  bool inReturn = false, currIsReturn = false;
  traversePrePost(ast, [&](Ref node) {
    currIsReturn = node[0] == RETURN;
    if (currIsReturn) inReturn = true;
  }, [&](Ref node) {
    if (currIsReturn) inReturn = false;
    if (node[0] == CALL && node[1][0] == NAME && node[1][1] == MATH_FROUND) {
      Ref arg = node[2][0];
      if (arg[0] == NUM) {
        if (!inReturn && arg[1]->getNumber() == 0) *node = *makeName(F0);
      } else if (arg[0] == CALL && arg[1][0] == NAME && arg[1][1] == MATH_FROUND) {
        safeCopy(node, arg);
      }
    }
  });
}

// Very simple 'registerization', coalescing of variables into a smaller number.
void registerize(Ref ast) {
  traverseFunctions(ast, [](Ref fun) {
    AsmData asmData(fun);
    // Add parameters as a first (fake) var (with assignment), so they get taken into consideration
    // note: params are special, they can never share a register between them (see later)
    Ref fake;
    if (!!fun[2] && fun[2]->size()) {
      Ref assign = makeNum(0);
      // TODO: will be an isEmpty here, can reuse it.
      fun[3]->insert(0, make1(VAR, fun[2]->map([&assign](Ref param) {
        return &(makeArray()->push_back(param).push_back(assign));
      })));
    }
    // Replace all var definitions with assignments; we will add var definitions at the top after we registerize
    StringSet allVars;
    traversePre(fun, [&](Ref node) {
      Ref type = node[0];
      if (type == VAR) {
        Ref vars = node[1]->filter([](Ref varr) { return varr->size() > 1; });
        if (vars->size() >= 1) {
          safeCopy(node, unVarify(vars));
        } else {
          safeCopy(node, makeEmpty());
        }
      } else if (type == NAME) {
        allVars.insert(node[1]->getIString());
      }
    });
    removeAllUselessSubNodes(fun); // vacuum?
    StringTypeMap regTypes; // reg name -> type
    auto getNewRegName = [&](int num, IString name) {
      const char *str;
      AsmType type = asmData.getType(name);
      switch (type) {
        case ASM_INT:       str = "i"; break;
        case ASM_DOUBLE:    str = "d"; break;
        case ASM_FLOAT:     str = "f"; break;
        case ASM_FLOAT32X4: str = "F4"; break;
        case ASM_INT32X4:   str = "I4"; break;
        case ASM_NONE:      str = "Z"; break;
        default: assert(0); // type doesn't have a name yet
      }
      int size = strlen(str) + int(ceil(log10(num))) + 3;
      char *temp = (char*)malloc(size);
      int written = sprintf(temp, "%s%d", str, num);
      assert(written < size);
      temp[written] = 0;
      IString ret(temp); // likely interns a new string; leaks if not XXX FIXME
      regTypes[ret] = type;
      assert(!allVars.has(ret) || asmData.isLocal(ret)); // register must not shadow non-local name
      return ret;
    };
    // Find the # of uses of each variable.
    // While doing so, check if all a variable's uses are dominated in a simple
    // way by a simple assign, if so, then we can assign its register to it
    // just for its definition to its last use, and not to the entire toplevel loop,
    // we call such variables "optimizable"
    StringIntMap varUses;
    int level = 1;
    std::unordered_map<int, StringSet> levelDominations; // level => set of dominated variables XXX vector?
    StringIntMap varLevels;
    StringSet possibles;
    StringSet unoptimizables;
    auto purgeLevel = [&]() {
      // Invalidate all dominating on this level, further users make it unoptimizable
      for (auto name : levelDominations[level]) {
        varLevels[name] = 0;
      }
      levelDominations[level].clear();
      level--;
    };
    std::function<bool (Ref node)> possibilifier = [&](Ref node) {
      Ref type = node[0];
      if (type == NAME) {
        IString name = node[1]->getIString();
        if (asmData.isLocal(name)) {
          varUses[name]++;
          if (possibles.has(name) && !varLevels[name]) unoptimizables.insert(name); // used outside of simple domination
        }
      } else if (type == ASSIGN && node[1]->isBool(true)) {
        if (!!node[2] && node[2][0] == NAME) {
          IString name = node[2][1]->getIString();
          // if local and not yet used, this might be optimizable if we dominate
          // all other uses
          if (asmData.isLocal(name) && !varUses[name] && !varLevels[name]) {
            possibles.insert(name);
            varLevels[name] = level;
            levelDominations[level].insert(name);
          }
        }
      } else if (CONTROL_FLOW.has(type)) {
        // recurse children, in the context of a loop
        if (type == WHILE || type == DO) {
          traversePrePostConditional(node[1], possibilifier, [](Ref node){});
          level++;
          traversePrePostConditional(node[2], possibilifier, [](Ref node){});
          purgeLevel();
        } else if (type == FOR) {
          traversePrePostConditional(node[1], possibilifier, [](Ref node){});
          for (int i = 2; i <= 4; i++) {
            level++;
            traversePrePostConditional(node[i], possibilifier, [](Ref node){});
            purgeLevel();
          }
        } else if (type == IF) {
          traversePrePostConditional(node[1], possibilifier, [](Ref node){});
          level++;
          traversePrePostConditional(node[2], possibilifier, [](Ref node){});
          purgeLevel();
          if (node->size() > 3 && !!node[3]) {
            level++;
            traversePrePostConditional(node[3], possibilifier, [](Ref node){});
            purgeLevel();
          }
        } else if (type == SWITCH) {
          traversePrePostConditional(node[1], possibilifier, [](Ref node){});
          Ref cases = node[2];
          for (int i = 0; i < cases->size(); i++) {
            level++;
            traversePrePostConditional(cases[i][1], possibilifier, [](Ref node){});
            purgeLevel();
          }
        } else assert(0);;
        return false; // prevent recursion into children, which we already did
      }
      return true;
    };
    traversePrePostConditional(fun, possibilifier, [](Ref node){});

    StringSet optimizables;
    for (auto possible : possibles) {
      if (!unoptimizables.has(possible)) optimizables.insert(possible);
    }

    // Go through the function's code, assigning 'registers'.
    // The only tricky bit is to keep variables locked on a register through loops,
    // since they can potentially be returned to. Optimizable variables lock onto
    // loops that they enter, unoptimizable variables lock in a conservative way
    // into the topmost loop.
    // Note that we cannot lock onto a variable in a loop if it was used and free'd
    // before! (then they could overwrite us in the early part of the loop). For now
    // we just use a fresh register to make sure we avoid this, but it could be
    // optimized to check for safe registers (free, and not used in this loop level).
    StringStringMap varRegs; // maps variables to the register they will use all their life
    std::vector<StringVec> freeRegsClasses;
    freeRegsClasses.resize(ASM_NONE);
    int nextReg = 1;
    StringVec fullNames;
    fullNames.push_back(EMPTY); // names start at 1
    std::vector<StringVec> loopRegs; // for each loop nesting level, the list of bound variables
    int loops = 0; // 0 is toplevel, 1 is first loop, etc
    StringSet activeOptimizables;
    StringIntMap optimizableLoops;
    StringSet paramRegs; // true if the register is used by a parameter (and so needs no def at start of function; also cannot
                         // be shared with another param, each needs its own)
    auto decUse = [&](IString name) {
      if (!varUses[name]) return false; // no uses left, or not a relevant variable
      if (optimizables.has(name)) activeOptimizables.insert(name);
      IString reg = varRegs[name];
      assert(asmData.isLocal(name));
      StringVec& freeRegs = freeRegsClasses[asmData.getType(name)];
      if (!reg) {
        // acquire register
        if (optimizables.has(name) && freeRegs.size() > 0 &&
            !(asmData.isParam(name) && paramRegs.has(freeRegs.back()))) { // do not share registers between parameters
          reg = freeRegs.back();
          freeRegs.pop_back();
        } else {
          assert(fullNames.size() == nextReg);
          reg = getNewRegName(nextReg++, name);
          fullNames.push_back(reg);
          if (asmData.isParam(name)) paramRegs.insert(reg);
        }
        varRegs[name] = reg;
      }
      varUses[name]--;
      assert(varUses[name] >= 0);
      if (varUses[name] == 0) {
        if (optimizables.has(name)) activeOptimizables.erase(name);
        // If we are not in a loop, or we are optimizable and not bound to a loop
        // (we might have been in one but left it), we can free the register now.
        if (loops == 0 || (optimizables.has(name) && !optimizableLoops.has(name))) {
          // free register
          freeRegs.push_back(reg);
        } else {
          // when the relevant loop is exited, we will free the register
          int relevantLoop = optimizables.has(name) ? (optimizableLoops[name] ? optimizableLoops[name] : 1) : 1;
          if (loopRegs.size() <= relevantLoop+1) loopRegs.resize(relevantLoop+1);
          loopRegs[relevantLoop].push_back(reg);
        }
      }
      return true;
    };
    traversePrePost(fun, [&](Ref node) { // XXX we rely on traversal order being the same as execution order here
      Ref type = node[0];
      if (type == NAME) {
        IString name = node[1]->getIString();
        if (decUse(name)) {
          node[1]->setString(varRegs[name]);
        }
      } else if (LOOP.has(type)) {
        loops++;
        // Active optimizables lock onto this loop, if not locked onto one that encloses this one
        for (auto name : activeOptimizables) {
          if (!optimizableLoops[name]) {
            optimizableLoops[name] = loops;
          }
        }
      }
    }, [&](Ref node) {
      Ref type = node[0];
      if (LOOP.has(type)) {
        // Free registers that were locked to this loop
        if (loopRegs.size() > loops && loopRegs[loops].size() > 0) {
          for (auto loopReg : loopRegs[loops]) {
            freeRegsClasses[regTypes[loopReg]].push_back(loopReg);
          }
          loopRegs[loops].clear();
        }
        loops--;
      }
    });
    if (!!fun[2] && fun[2]->size()) {
      fun[2]->setSize(0); // clear params, we will fill with registers
      fun[3]->splice(0, 1); // remove fake initial var
    }

    asmData.locals.clear();
    asmData.params.clear();
    asmData.vars.clear();
    for (int i = 1; i < nextReg; i++) {
      IString reg = fullNames[i];
      AsmType type = regTypes[reg];
      if (!paramRegs.has(reg)) {
        asmData.addVar(reg, type);
      } else {
        asmData.addParam(reg, type);
        fun[2]->push_back(makeString(reg));
      }
    }
    asmData.denormalize();
  });
}

// minified names generation
StringSet RESERVED("do if in for new try var env let");
const char *VALID_MIN_INITS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$";
const char *VALID_MIN_LATERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$0123456789";

StringVec minifiedNames;
std::vector<int> minifiedState;

void ensureMinifiedNames(int n) { // make sure the nth index in minifiedNames exists. done 100% deterministically
  static int VALID_MIN_INITS_LEN = strlen(VALID_MIN_INITS);
  static int VALID_MIN_LATERS_LEN = strlen(VALID_MIN_LATERS);

  while (minifiedNames.size() < n+1) {
    // generate the current name
    std::string name;
    name += VALID_MIN_INITS[minifiedState[0]];
    for (int i = 1; i < minifiedState.size(); i++) {
      name += VALID_MIN_LATERS[minifiedState[i]];
    }
    IString str(strdupe(name.c_str())); // leaked!
    if (!RESERVED.has(str)) minifiedNames.push_back(str);
    // increment the state
    int i = 0;
    while (1) {
      minifiedState[i]++;
      if (minifiedState[i] < (i == 0 ? VALID_MIN_INITS_LEN : VALID_MIN_LATERS_LEN)) break;
      // overflow
      minifiedState[i] = 0;
      i++;
      if (i == minifiedState.size()) minifiedState.push_back(-1); // will become 0 after increment in next loop head
    }
  }
}

void minifyLocals(Ref ast) {
  assert(!!extraInfo);
  IString GLOBALS("globals");
  assert(extraInfo->has(GLOBALS));
  Ref globals = extraInfo[GLOBALS];

  if (minifiedState.size() == 0) minifiedState.push_back(0);

  traverseFunctions(ast, [&globals](Ref fun) {
    // Analyse the asmjs to figure out local variable names,
    // but operate on the original source tree so that we don't
    // miss any global names in e.g. variable initializers.
    AsmData asmData(fun);
    asmData.denormalize(); // TODO: we can avoid modifying at all here - we just need a list of local vars+params

    StringStringMap newNames;
    StringSet usedNames;

    // Find all the globals that we need to minify using
    // pre-assigned names.  Don't actually minify them yet
    // as that might interfere with local variable names.
    traversePre(fun, [&](Ref node) {
      if (node[0] == NAME) {
        IString name = node[1]->getIString();
        if (!asmData.isLocal(name)) {
          if (globals->has(name)) {
            IString minified = globals[name]->getIString();
            assert(!!minified);
            newNames[name] = minified;
            usedNames.insert(minified);
          }
        }
      }
    });

    // The first time we encounter a local name, we assign it a
    // minified name that's not currently in use.  Allocating on
    // demand means they're processed in a predictable order,
    // which is very handy for testing/debugging purposes.
    int nextMinifiedName = 0;
    auto getNextMinifiedName = [&]() {
      IString minified;
      while (1) {
        ensureMinifiedNames(nextMinifiedName);
        minified = minifiedNames[nextMinifiedName++];
        // TODO: we can probably remove !isLocalName here
        if (!usedNames.has(minified) && !asmData.isLocal(minified)) {
          return minified;
        }
      }
    };

    // We can also minify loop labels, using a separate namespace
    // to the variable declarations.
    StringStringMap newLabels;
    int nextMinifiedLabel = 0;
    auto getNextMinifiedLabel = [&]() {
      ensureMinifiedNames(nextMinifiedLabel);
      return minifiedNames[nextMinifiedLabel++];
    };

    // Traverse and minify all names.
    if (globals->has(fun[1]->getIString())) {
      fun[1]->setString(globals[fun[1]->getIString()]->getIString());
      assert(!!fun[1]);
    }
    if (!!fun[2]) {
      for (int i = 0; i < fun[2]->size(); i++) {
        IString minified = getNextMinifiedName();
        newNames[fun[2][i]->getIString()] = minified;
        fun[2][i]->setString(minified);
      }
    }
    traversePre(fun[3], [&](Ref node) {
      Ref type = node[0];
      if (type == NAME) {
        IString name = node[1]->getIString();
        IString minified = newNames[name];
        if (!!minified) {
          node[1]->setString(minified);
        } else if (asmData.isLocal(name)) {
          minified = getNextMinifiedName();
          newNames[name] = minified;
          node[1]->setString(minified);
        }
      } else if (type == VAR) {
        for (int i = 0; i < node[1]->size(); i++) {
          Ref defn = node[1][i];
          IString name = defn[0]->getIString();
          if (!(newNames.has(name))) {
            newNames[name] = getNextMinifiedName();
          }
          defn[0]->setString(newNames[name]);
        }
      } else if (type == LABEL) {
        IString name = node[1]->getIString();
        if (!newLabels.has(name)) {
          newLabels[name] = getNextMinifiedLabel();
        }
        node[1]->setString(newLabels[name]);
      } else if (type == BREAK || type == CONTINUE) {
        if (node->size() > 1 && !!node[1]) {
          node[1]->setString(newLabels[node[1]->getIString()]);
        }
      }
    });
  });
}

//==================
// Main
//==================

#include <string.h> // only use this for param checking

int main(int argc, char **argv) {
  // Read input file
  FILE *f = fopen(argv[1], "r");
  assert(f);
  fseek(f, 0, SEEK_END);
  int size = ftell(f);
  char *json = new char[size+1];
  rewind(f);
  int num = fread(json, 1, size, f);
  assert(num == size);
  fclose(f);
  json[size] = 0;

  char *comment = strstr(json, "//");
  char *extraInfoStart = strstr(json, "// EXTRA_INFO:");
  if (comment) *comment = 0; // drop off the comments; TODO: parse extra info
  if (extraInfoStart) {
    extraInfo = arena.alloc();
    extraInfo->parse(extraInfoStart + 14);
  }

  // Parse JSON source into the document
  doc = arena.alloc();
  doc->parse(json);
  // do not free json, it's contents are used as strings

  // Run passes on the Document
  for (int i = 2; i < argc; i++) {
    std::string str(argv[i]);
    if (str == "asm") {} // the default for us
    else if (str == "asmPreciseF32") preciseF32 = true;
    else if (str == "receiveJSON" || str == "emitJSON") {} // the default for us
    else if (str == "eliminate") eliminate(doc);
    else if (str == "eliminateMemSafe") eliminateMemSafe(doc);
    else if (str == "simplifyExpressions") simplifyExpressions(doc);
    else if (str == "optimizeFrounds") optimizeFrounds(doc);
    else if (str == "simplifyIfs") simplifyIfs(doc);
    else if (str == "registerize") registerize(doc);
    else if (str == "minifyLocals") minifyLocals(doc);
    else if (str == "minifyWhitespace") {}
    else {
      fprintf(stderr, "unrecognized argument: %s\n", str.c_str());
      assert(0);
    }
  }

  // Emit JSON of modified Document
  doc->stringify(std::cout);
  std::cout << std::endl;

  return 0;
}

