#pragma once

namespace CppUtils::Language::Lexer::Grammar
{
	using namespace std::literals;

	constexpr auto lowLevelGrammarSrc = R"(
		main_{ token_{decls_} token_{spaces_} }

		decls_{ token_{spaces_} or_{ token_{comment_} token_{decl_} } optional_{token_{decls_}} }
		decl_{ token_{token_} token_{spaces_} string_{:} optional_{sub_{token_{lexemes_}}} token_{spaces_} comma_ string_{;} }

		comment_{ string_{#} token_{commentContent_} optional_{string_{\n}} }
		commentContent_{ not_{or_{ end_ string_{\n} }}+ optional_{token_{commentContent_}} }

		lexemes_{ repeat_{token_{lexeme_}} }
		lexemesWithSeparator_{ token_{lexeme_} token_{spaces_} optional_{parenthesis_{ string_{,} token_{lexemesWithSeparator_} }} }
		lexeme_{ token_{spaces_} or_{
			token_{comma_}
			token_{charLiteral_}
			token_{stringLiteral_}
			token_{not_}
			token_{optional_}
			token_{range_}
			token_{is_}
			token_{increment_}
			token_{read_}
			token_{add_}
			token_{hash_}
			token_{or_}
			token_{sub_}
			token_{repeat_}
			token_{push_}
			token_{pop_}
			token_{end_}
			token_{parenthesis_}
			token_{tokenLexeme_}
		}}
		tokenLexeme_{ add_{token_} sub_{token_{token_}} }

		comma_{ string_{,} add_{comma_} }
		not_{ string_{!} add_{not_} sub_{token_{lexeme_}} }
		optional_{ string_{~} add_{optional_} sub_{token_{lexeme_}} }
		range_{ string_{[} token_{spaces_} add_{parenthesis_} sub_{ add_{>=_} token_{rawCharLiteral_} token_{spaces_} string_{,} token_{spaces_} add_{<=_} token_{rawCharLiteral_} } token_{spaces_} string_{]} }
		is_{ string_{is(} add_{==_} token_{rawCharLiteral_} token_{spaces_} string_{)} }
		increment_{ string_{+} add_{+} }
		read_{ string_{read(} add_{read_} token_{spaces_} string_{)} }
		parenthesis_{ string_{(} add_{parenthesis_} sub_{token_{lexemes_}} token_{spaces_} string_{)} }
		add_{ string_{add(} add_{add_} or_{ token_{rawCharLiteral_} token_{rawStringLiteral_} sub_{token_{token_}} } token_{spaces_} string_{)} }
		hash_{ string_{hash(} add_{hash_} sub_{token_{lexemes_}} token_{spaces_} string_{)} }
		or_{ string_{or(} add_{or_} sub_{token_{lexemesWithSeparator_}} token_{spaces_} string_{)} }
		sub_{ string_{sub(} add_{sub_} sub_{token_{lexemes_}} token_{spaces_} string_{)} }
		repeat_{ string_{repeat(} add_{repeat_} sub_{token_{lexeme_} optional_{parenthesis_{token_{spaces_} string_{,} token_{lexeme_}}}} token_{spaces_} string_{)} }
		push_{ string_{push(} add_{push_} sub_{token_{lexeme_}} token_{spaces_} string_{)} }
		pop_{ string_{pop(} add_{pop_} token_{spaces_} string_{)} }
		end_{ string_{end(} add_{end_} token_{spaces_} string_{)} }

		charLiteral_{ string_{'} add_{string_} sub_{token_{charLiteralContent_}} string_{'} }
		rawCharLiteral_{ string_{'} sub_{token_{charLiteralContent_}} string_{'} }
		charLiteralContent_{ not_{string_{'}} token_{char_} }
		
		stringLiteral_{ string_{"} add_{string_} sub_{token_{stringLiteralContent_}} string_{"} }
		rawStringLiteral_{ string_{"} sub_{token_{stringLiteralContent_}} string_{"} }
		stringLiteralContent_{ optional_{repeat_{ parenthesis_{ not_{string_{"}} token_{char_} } }} }

		token_{ hash_{token_{keyword_}} }
		keyword_{ token_{firstKeywordChar_} optional_{token_{keywordContinuation_}} }
		keywordContinuation_{ token_{keywordChar_} optional_{token_{keywordContinuation_}} }
		firstKeywordChar_{ token_{alphaChar_} }
		keywordChar_{ token_{alphaNumChar_} }
		alphaChar_{ or_{ token_{escapeChar_} token_{min_} token_{maj_} } }
		alphaNumChar_{ or_{ token_{alphaChar_} token_{digit_} } }
		min_{ >=_{a} <=_{z} read_+ }
		maj_{ >=_{A} <=_{Z} read_+ }
		digit_{ >=_{0} <=_{9} read_+ }

		char_{ token_{escapeChar_} } char_{ read_+ }
		escapeChar_{ string_{\\} or_{
			parenthesis_{ string_{0} add_{\0} }
			parenthesis_{ string_{a} add_{\a} }
			parenthesis_{ string_{b} add_{\b} }
			parenthesis_{ string_{f} add_{\f} }
			parenthesis_{ string_{n} add_{\n} }
			parenthesis_{ string_{r} add_{\r} }
			parenthesis_{ string_{t} add_{\t} }
			parenthesis_{ string_{v} add_{\v} }
			parenthesis_{read_+}
		}}

		spaces_{ optional_{repeat_{token_{space_}}} }
		space_{ or_{ string_{\ } string_{\n} string_{\t} string_{\r} } }
	)"sv;

	constexpr auto highLevelGrammarSrc = R"(
		main: decls ~spaces;
		decls: ~spaces or(comment, decl) ~decls;
		decl: token ~spaces ':' sub(~lexemes) ~spaces ';';

		comment: '#' commentContent ~'\n';
		commentContent: !or(end(), '\n')+ ~commentContent;

		lexemes: repeat(lexeme);
		lexemesWithSeparator: lexeme ~spaces ~(',' lexemesWithSeparator);
		lexeme: ~spaces or(
			comma,
			charLiteral,
			stringLiteral,
			not,
			optional,
			range,
			is,
			increment,
			readLexeme,
			addLexeme,
			hash,
			or,
			sub,
			repeat,
			push,
			pop,
			endLexeme,
			recurrence,
			parenthesis,
			tokenLexeme
		);
		tokenLexeme: add(token) sub(token);

		comma: ',' add(comma);
		not: '!' add(not) sub(lexeme);
		optional: '~' add(optional) sub(lexeme);
		range: '[' spaces add(parenthesis) sub( add(\>\=) rawCharLiteral spaces ',' spaces add(\<\=) rawCharLiteral ) spaces ']';
		is: "is(" add(\=\=) rawCharLiteral spaces ')';
		increment: '+' add('+');
		readLexeme: "read(" add(read) spaces ')';
		parenthesis: '(' add(parenthesis) sub(lexemes) spaces ')';
		addLexeme: "add(" add(add) or(rawCharLiteral, rawStringLiteral, sub(token)) spaces ')';
		hash: "hash(" add(hash) sub(lexemes) spaces ')';
		or: "or(" add(or) sub(lexemesWithSeparator) spaces ')';
		sub: "sub(" add(sub) sub(lexemes) spaces ')';
		repeat: "repeat(" add(repeat) sub(lexeme ~(~spaces ',' lexeme)) spaces ')';
		push: "push(" add(push) sub(lexeme) spaces ')';
		pop: "pop(" add(pop) spaces ')';
		endLexeme: "end(" add(end) spaces ')';

		charLiteral: '\'' add(string) sub(charLiteralContent) '\'';
		rawCharLiteral: '\'' sub(charLiteralContent) '\'';
		charLiteralContent: !'\'' char;

		stringLiteral: '"' add(string) sub(stringLiteralContent) '"';
		rawStringLiteral: '"' sub(stringLiteralContent) '"';
		stringLiteralContent: ~repeat((!'"' char));

		token: hash(keyword);
		keyword: firstKeywordChar ~keywordContinuation;
		keywordContinuation: keywordChar ~keywordContinuation;
		firstKeywordChar: alphaChar;
		keywordChar: alphaNumChar;
		alphaChar: or(escapeChar, (or(['a', 'z'], ['A', 'Z']) read()+));
		alphaNumChar: or(alphaChar, (['0', '9'] read()+));

		char: or(escapeChar, (read()+));
		escapeChar: '\\' or(
			('0' add('\0')),
			('a' add('\a')),
			('b' add('\b')),
			('f' add('\f')),
			('n' add('\n')),
			('r' add('\r')),
			('t' add('\t')),
			('v' add('\v')),
			(read()+)
		);

		spaces: ~repeat(space);
		space: or(' ', '\n', '\t', '\r');
	)"sv;

	[[nodiscard]] auto parse(std::string_view src) -> Parser::Ast
	{
		const auto lowLevelGrammarAst = Parser::parseAst(lowLevelGrammarSrc);
		const auto highLevelGrammarAst = Lexer::parse(highLevelGrammarSrc, lowLevelGrammarAst);
		return Lexer::parse(src, highLevelGrammarAst);
	}

	namespace Literals
	{
		[[nodiscard]] auto operator"" _grammar(const char* cString, std::size_t) -> Parser::Ast
		{
			return parse(cString);
		}
	}
}
