/***************************************
 * SIMPLESEM.cpp
 *
 * Student Name: Joowon Suh
 * Student ID: 44414081
 * UCI Net ID: joowons
 **************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cstring>

class SIMPLESEM {
public:
  SIMPLESEM(char *sourceFile);
  void runProgram();
  void parseProgram();

private:
  std::ifstream codeIn;
  std::ifstream inputIn;
  std::ofstream outFile;
  static int nextChar; //contains the character(or -1==EOF)
  const static int eof = -1; //int value for End of File
  int debug;

  void printRule(const char*);
  void parseStatement();
  void parseSet();
  void parseExpr();
  void parseTerm();
  void parseFactor();
  void parseNumber();
  void parseJumpJumpt();
  void parseJumpt();
  void parseJump();
  void parseHalt();

void removeSpaceAfterOperation();
  void findWhiteSpace();
  void moveOffset();
  void moveOffset(std::string str);
  void readOperation(std::string str);
  void parseReadAndWrite();
  void lookInto();
};

int SIMPLESEM::nextChar = 0;

SIMPLESEM::SIMPLESEM(char *sourceFile) {
  debug = 1;
  codeIn.open(sourceFile, std::fstream::in);
  if (codeIn.fail()) {
    std::cerr << "init: Errors accessing source file "
              << sourceFile << std::endl;
    exit(-2);
  }

  // inputIn.open("input.txt", std::fstream::in);
  // if (inputIn.fail()) {
  //   std::cerr << "init: Errors accessing input file input.txt" << std::endl;
  //   exit(-2);
  // }

  outFile.open((std::string(sourceFile) + ".out").c_str(), std::fstream::out);
  if (outFile.fail()) {
    std::cerr << "init: Errors accessing output file "
              << std::string(sourceFile) + ".out" << std::endl;
    exit(-2);
  }
}

void SIMPLESEM::printRule(const char* rule)
{
	outFile << rule << std::endl;
}
void SIMPLESEM::moveOffset()
{
  nextChar = codeIn.get();
  if(debug == 1){
    std::cout << (char)nextChar;
  }
  nextChar = codeIn.peek();
}
void SIMPLESEM::moveOffset(std::string str){
  nextChar = codeIn.get();
  if(debug == 1){
    std::cout << " " << str << " [ " <<(char)nextChar << " ]"<< std::endl;;
  }
  nextChar = codeIn.peek();
}
void SIMPLESEM::readOperation(std::string str){
  while(nextChar != ' '){
    nextChar = codeIn.get();
    if(debug == 1){
      std::cout << " " << str << " [ " <<(char)nextChar << " ]"<< std::endl;;
    }
    nextChar = codeIn.peek();
  }
  nextChar = codeIn.get();
  if(debug == 1){
    std::cout << " " << str << " [ " <<(char)nextChar << " ] " << std::endl;
  }
  nextChar = codeIn.peek();
}

void SIMPLESEM::parseProgram() {
	printRule("Program");

	parseStatement();
	while (nextChar != eof){
			parseStatement();
        // nextChar = codeIn.get();
        // std::cout << (char)nextChar << std::endl;
	}
}

void SIMPLESEM::parseStatement() {
	printRule("Statement");
    // debug = 0;
    // std::cout << nextChar << std::endl;
    // if(nextChar == 'h')
    // {
    //     std::cin >> debug;
    // }
  //Complete productions here
  nextChar = codeIn.peek();
  if(nextChar == '\r'){
    moveOffset("Statement");
  }
  if(nextChar == '\n' ){
    moveOffset("Statement");
  }

  if( nextChar == 's' ){
    parseSet();
  }
  else if( nextChar == 'j' ){
    // moveOffset("Statement");
    parseJumpJumpt();
  }
  else{
      parseHalt();
  }
}
void SIMPLESEM::parseReadAndWrite(){
  while( nextChar != ',' && nextChar != '\n' && nextChar != '\r'&& nextChar != eof){
      moveOffset("reaandwrite");
  }
}
void SIMPLESEM::parseSet()
{
  printRule("Set");
  readOperation("Set");
  // std::cout << "dfdfdfd" << std::endl;
  while( nextChar != '\n' && nextChar != '\r' && nextChar != eof){

    switch(nextChar){
      case 'r':
      case 'w':
        parseReadAndWrite();
        break;
      case ',':
        moveOffset("set");
        break;
      case ' ':
        moveOffset("set");
        break;
      default:
        parseExpr();
    }
    // debug = 0;
    // std::cout << (char)nextChar << std::endl;
  }
}

void SIMPLESEM::parseExpr()
{
  //nextChar has first letter of expression.
  printRule("Expr");
  parseTerm();
  while( nextChar != ',' && nextChar != '\n'&& nextChar != '\r'&& nextChar != eof){
      // debug = 0;
      // std::cout << nextChar << std::endl;
      // if(nextChar == 'h')
      // {
      //     std::cin >> debug;
      // }
    switch(nextChar){
        case ' ':
            lookInto();
            break;
        case '+':
        case '-':
            removeSpaceAfterOperation();
            parseTerm();
            break;
        default:
            return;
    }
  }
}
void SIMPLESEM::removeSpaceAfterOperation(){
    moveOffset("removing");
    while( nextChar == ' '){
        moveOffset("removing");
    }
}
void SIMPLESEM::lookInto(){
    int count = 1;
    moveOffset("looking");
    while( nextChar != ' ' && nextChar != '\r' && nextChar !='n' ){
        if(nextChar == '+' || nextChar == '-')
            return;
        moveOffset("looking");
        count++;
    }
    while( count-- ){
        codeIn.unget();
    }
    nextChar = ',';
}

void SIMPLESEM::parseTerm()
{
  printRule("Term");
  parseFactor();
  while( nextChar != ',' && nextChar != '\n' && nextChar != ' ' && nextChar != '\r'&& nextChar != eof)
  {
    switch(nextChar){
      case '*':
      case '/':
      case '%':
        moveOffset("Term");
        parseFactor();
        break;
      default:
        return;
    }
  }
}
void SIMPLESEM::parseFactor()
{

  printRule("Factor");
  while( nextChar != ',' && nextChar != '\n' && nextChar != ' ' && nextChar != '\r'&& nextChar != eof)
  {

      // debug = 0;
      // std::cout << (char)nextChar << std::endl;
      // if(nextChar == '?')
      // {
      //     std::cin >> debug;
      // }
    switch(nextChar){
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
            parseNumber();
            break;
        case 'D':
            moveOffset("Factor");
            break;
        case '[':
            moveOffset("Factor");
            parseExpr();
            break;
        case '(':
            moveOffset("Factor");
            parseExpr();
            break;
        case ')':
            moveOffset("Factor");
            break;
        case ']':
            moveOffset("Factor");
            break;
        default:
            return;
    }
  }
}
void SIMPLESEM::parseNumber()
{
  printRule("Number");
  while( nextChar != ',' && nextChar != '\n'&& nextChar != ' ' && nextChar != '\r'&& nextChar != eof){
    if( isdigit((char)nextChar) ){
        moveOffset("Number");
    }else{
        return;
    }
  }
}
void SIMPLESEM::parseJumpt()
{
  printRule("Jumpt");
  //parse first expression
  while( nextChar != '\n' && nextChar != '\r'&& nextChar != eof)
  {

    switch(nextChar){
      case '!':
      case '=':
      case '>':
      case '<':
        moveOffset("Jumpt");
        break;
      case ',':
        moveOffset("Jumpt");
        break;
      case ' ':
        moveOffset("Jumpt");
        break;
      default:
        parseExpr();
    }
  }
}
////////////////////////// Next : jump
void SIMPLESEM::parseJumpJumpt()
{
  //move offset, read jump or jumpt
  while( nextChar != '\n' && nextChar != '\r'&& nextChar != eof)
  {
    moveOffset("parseJumpJumpt");
    if(nextChar == ' '){
        parseJump();

    }else if(nextChar == 't'){
      moveOffset("parseJumpJumpt");
      parseJumpt();
    }
  }
}

void SIMPLESEM::parseJump(){
    printRule("Jump");
    moveOffset("Jump");
    while( nextChar != '\r' && nextChar != '\n' && nextChar != eof ){
        parseExpr();
    }
}

void SIMPLESEM::parseHalt(){
    // printRule("Halt");
    while( nextChar != eof){
        moveOffset("Halt");
    }
}




int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "init: This program requires an input "
              << "filename as a parameter " << std::endl;
    exit(-2);
  }
  SIMPLESEM s(argv[1]);
  s.parseProgram();
}
