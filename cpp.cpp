#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { env_init(); return yyparse(); }

Sym::Sym(string T,string V) { tag=T; val=V; }
Sym::Sym(string V):Sym("",V) {}
void Sym::push(Sym*o) { nest.push_back(o); }

string Sym::tagval() { return "<"+val+">"; }
string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::dump(int depth) { string S = "\n"+pad(depth)+tagval();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Sym* Sym::eval() {
	Sym*E = env[val]; if (E) return E;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it) = (*it)->eval();
	return this; }

string Sym::str() { return val; }
Sym* Sym::eq(Sym*o) { env[str()]=o; return o; }
Sym* Sym::add(Sym*o) { return new Sym(str()+o->str()); }
Sym* Sym::div(Sym*o) { return new Sym(str()+"/"+o->str()); }

Str::Str(string V):Sym("str",V) {}
string Str::tagval() { string S = "'";
	for (int i=0;i<val.size();i++)
		switch (val[i]) {
		case '\n': S+="\\n"; break;
		default: S+=val[i]; }
	return S+"'"; }

List::List():Sym("") {}
Sym* List::div(Sym*o) {
	List*L = new List();
	if (nest.size()) {
		for (auto it=nest.begin(),e=nest.end();it!=e;it++) {
			L->push(*it); L->push(o); }
		L->nest.pop_back();
	}
	return L; }

Op::Op(string V):Sym("op",V) {}
Sym* Op::eval() {
	if (val=="~") return nest[0]; else Sym::eval();
	if (val=="=") return nest[0]->eq(nest[1]);
	if (val=="+") return nest[0]->add(nest[1]);
	if (val=="/") return nest[0]->div(nest[1]);
	return this; }

map<string,Sym*> env;
void env_init(){
	env["MODULE"] = new Sym(MODULE);
}
