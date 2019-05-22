#include <liblangutil/Token.h>
#include <stdio.h>

using namespace langutil;

int main()
{
	printf("{\n");
	bool first = true;
	for ( int i = int(Token::EOS); i < int(Token::Whitespace); i++)
		{
			Token t = Token(i);
			if (!first) printf(",");
			printf("\n\t{\"%s\": [\"%s\", \"%s\", %d]}",
				TokenTraits::name(t),
				TokenTraits::friendlyName(t).c_str(),
				TokenTraits::isReservedKeyword(t) ? "true" : "false",
				TokenTraits::precedence(t));
		}
	printf("\n}\n");
}
