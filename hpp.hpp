#ifndef _H_REWRITE
#define _H_REWRITE

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

struct Sym {
	string tag,val;
	Sym(string,string); Sym(string);
	vector<Sym*> nest; void push(Sym*);
	virtual string tagval();
	virtual string dump(int=0); string pad(int);
	virtual string str();
	virtual Sym* eval();
	virtual Sym* eq(Sym*);
	virtual Sym* add(Sym*);
	virtual Sym* div(Sym*);
};

extern map<string,Sym*> env;
extern void env_init();

struct Str: Sym { Str(string); string tagval(); };

struct List: Sym { List(); Sym*div(Sym*); };

struct Op: Sym { Op(string); Sym*eval(); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_REWRITE
