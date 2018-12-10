#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <memory>

using namespace std;

struct Token {
  char _operand;
  double _val;
  Token(char operand, double val);
};
typedef struct Token Token;

Token::Token(char operand, double val) {
  _operand=operand;
  _val=val;
}

struct Node {
  char _operand;
  double _val;
  std::unique_ptr<struct Node> _leftptr;
  std::unique_ptr<struct Node> _rightptr;
  Node(char operand, double val);
  Node(char operand, double val, std::unique_ptr<struct Node>& leftptr, std::unique_ptr<struct Node>& rightptr);
  ~Node();
};
typedef struct Node Node;

Node::Node(char operand, double val) {
  _operand=operand;
  _val=val;
  _leftptr=nullptr;
  _rightptr=nullptr;
}

Node::~Node() {
  //cout << "DELETE NODE" << endl;
}

Node::Node(char operand, double val, std::unique_ptr<struct Node>& leftptr, std::unique_ptr<struct Node>& rightptr) {
  _operand=operand;
  _val=val;
  _leftptr=std::move(leftptr);
  _rightptr=std::move(rightptr);
}



//Test cases
std::vector<std::string> teststrings = {
"1",
"7.0",
"3+7",
"5-10",
"3*7",
"81/9",
"35/5+2",
"2+35/5",
"7*7+35/5+2",
"8/3*2+2",
"1+1+1+1+1+2/4"};

std::vector<double> results = {
1.0,
7.0,
10.0,
-5.0,
21.0,
9.0,
9.0,
9.0,
49.0+7.0+2.0,
8.0/3.0*2.0+2.0,
1+1+1+1+1+2.0/4.0
};

bool isOperand(char val) {
  if (val=='*' || val=='/' || val=='+' || val=='-')
    return true;
  else 
    return false;
}

bool isHighPri(char val) {
  if (val=='*' || val=='/')
    return true;
  else 
    return false;
}

bool isLowPri(char val) {
  if (val=='+' || val=='-')
    return true;
  else 
    return false;
}

int opPri(char operand) {
  switch (operand) {
    case '+' : return 1;
    case '-' : return 1;
    case '*' : return 2;
    case '/' : return 2;
  }
  return 0;
}

double handleNewNumber(char operand, double prevNum, double num) {
    if (operand != 0) {
      switch (operand) {
        case '+' : return (prevNum+num);
        case '-' : return (prevNum-num);
        case '*' : return (prevNum*num);
        case '/' : return (prevNum/num);
      }
    } else
      return num;
  return 0;
}

void parseMathString(const std::string& mathstr, std::list<Token>& mathlist) {
  std::string tmpNum;
  int idx=0;
  std::string operand;

  while (idx < mathstr.size()) {
    if (!isOperand(mathstr[idx])) {
      tmpNum+=mathstr[idx];
      //cout << tmpNum << endl;
    } else {
      mathlist.push_back( Token(0x00, atof(tmpNum.c_str())) );
      mathlist.push_back( Token(mathstr[idx], 0.0) );
      tmpNum="";
    }
    idx++;
  }
  if (tmpNum.size() > 0)
    mathlist.push_back( Token(0x00, atof(tmpNum.c_str())) );
}

void parseTokens(std::list<Token>& tokens, std::unique_ptr<Node>& root) {
  std::list<Token>::iterator it;
  std::stack<Token> opstack;
  std::stack<std::unique_ptr<Node>> outstack;
  std::unique_ptr<Node> leftptr;
  std::unique_ptr<Node> rightptr;

  cout << "TOKENS: ";  
  for (it=tokens.begin();it!=tokens.end();++it) {
    if (it->_operand==0x00)
      cout << it->_val << ' ';
    else
      cout << it->_operand << ' ';
  }
  cout << endl;


  for (it=tokens.begin();it!=tokens.end();++it) {
    if (it->_operand==0x00) {
      outstack.push( std::unique_ptr<Node>(new Node(0x00, it->_val)) );
      //cout << "N(" << it->_val << ")" << endl;
    } else {
      //cout << "OPERAND" << endl;
      while ( (!opstack.empty()) && (opPri(it->_operand) <= opPri(opstack.top()._operand)) ) {
        //cout << "N(" << outstack.top().get()->_val << ") ";
        rightptr=std::move(outstack.top());outstack.pop();
        //cout << "N(" << outstack.top().get()->_val << ")" << endl;
        leftptr=std::move(outstack.top());outstack.pop();
        outstack.push( std::unique_ptr<Node>(new Node(opstack.top()._operand, 0.0, leftptr, rightptr)) );
        opstack.pop();
      }
      //cout << "PUSH OP " << it->_operand << endl;
      opstack.push(*it);
    }
  }
  while (!opstack.empty()) {
    rightptr=std::move(outstack.top());outstack.pop();
    leftptr=std::move(outstack.top());outstack.pop();
    outstack.push( std::unique_ptr<Node>(new Node(opstack.top()._operand, 0.0, leftptr, rightptr)) );
    opstack.pop();
  }
  root=std::move(outstack.top());
}

double parseTree(std::unique_ptr<Node>& root) {
  //cout << "T " << root.get()->_operand << endl;
  switch (root.get()->_operand) {
    case 0x0 : return root.get()->_val;
    case '+' : return parseTree(root.get()->_leftptr)+parseTree(root.get()->_rightptr);
    case '-' : return parseTree(root.get()->_leftptr)-parseTree(root.get()->_rightptr);
    case '*' : return parseTree(root.get()->_leftptr)*parseTree(root.get()->_rightptr);
    case '/' : return parseTree(root.get()->_leftptr)/parseTree(root.get()->_rightptr);
    default : cout << "ERROR!" << endl; break;
  }
}


void printNode(Node* node) {
  if (node->_operand==0x00)
    cout << " " << node->_val << " ";
  else
    cout << node->_operand;

}

void DFS(std::unique_ptr<Node>& root) {
  if (root==nullptr)
    return;
  else {
    DFS(root.get()->_leftptr);
    printNode(root.get());
    DFS(root.get()->_rightptr);
  }
}

void BFS(std::unique_ptr<Node>& root) {
  std::queue<Node*> q;
  std::queue<int> level;
  int currentlevel=0;

  q.push( root.get() );
  level.push( currentlevel );

  while (!q.empty()) {
    printNode(q.front());
    currentlevel=level.front();



    if (q.front()->_leftptr != nullptr) {
      q.push(q.front()->_leftptr.get());
      level.push(currentlevel+1);
    }
    if (q.front()->_rightptr != nullptr) {
      q.push(q.front()->_rightptr.get());
      level.push(currentlevel+1);
    }


    q.pop();
    level.pop();

    if ( (!level.empty()) && (level.front() != currentlevel) )
      cout << endl;
  }
}

double parseMath(const std::string& mathstr) {
  std::list<Token> tokens;
  std::unique_ptr<Node> root;
  double result=0.0;

  parseMathString(mathstr, tokens);

  parseTokens(tokens, root);

  result=parseTree(root);
  cout << "FINISHED" << endl;
  DFS(root);cout << endl;
  BFS(root);cout << endl;

  return result;
}

void LOG_DOUBLE(const std::string& logMsg, double actual, double expected) {
  cout << logMsg << " ";
  cout << "actual: " << actual << " expected: " << expected << " ";
  if (actual==expected)
    cout << "PASS" << endl;
  else
    cout << "FAIL" << endl;
}

int main() {

  //parseMath(teststrings[8]);
  LOG_DOUBLE("TEST CONSTANT", parseMath(teststrings[0]), results[0]);
  LOG_DOUBLE("TEST CONSTANT", parseMath(teststrings[1]), results[1]);
  LOG_DOUBLE("TEST ADDITION", parseMath(teststrings[2]), results[2]);
  LOG_DOUBLE("TEST SUBTRACTION", parseMath(teststrings[3]), results[3]);
  LOG_DOUBLE("TEST MULTIPLICATION", parseMath(teststrings[4]), results[4]);
  LOG_DOUBLE("TEST DIVISION", parseMath(teststrings[5]), results[5]);
  for (int i=6;i<=10;i++) {
    LOG_DOUBLE("ADVANCED", parseMath(teststrings[i]), results[i]);
  }
	return 0;
}

