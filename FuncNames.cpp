#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <iostream>

using namespace clang::tooling;
using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

static llvm::cl::OptionCategory MyToolCategory("my-tool options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::extrahelp MoreHelp("\nMore help text...\n");

DeclarationMatcher funcMatch = functionDecl().bind("ordinaryFunc");

class FuncPrinter : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result)
	{
		if (const FunctionDecl *fn = Result.Nodes.getNodeAs<clang::FunctionDecl>("ordinaryFunc"))
		{
			SourceRange range = fn->getSourceRange();
			std::string begStr = range.getBegin().printToString(Result.Context->getSourceManager());
			std::string endStr = range.getEnd().printToString(Result.Context->getSourceManager());
			std::string name = fn->getNameInfo().getAsString();
			std::cout << name << " | " << begStr << " | " << endStr << std::endl;
		}
	}
};

int main(int argc, const char **argv) {
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(),
		       OptionsParser.getSourcePathList());

	FuncPrinter Printer;
	MatchFinder Finder;
	Finder.addMatcher(funcMatch, &Printer);

	return Tool.run(newFrontendActionFactory(&Finder).get());
	// return Tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
}
