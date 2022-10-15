#pragma once

namespace CppUtils::Language::Lexer::Grammar
{
	using namespace std::literals;

	constexpr auto lowLevelGrammarSrc = R"(
		main_{ token_{decls_} optional_{token_{spaces_}} }

		decls_{ optional_{token_{spaces_}} or_{ token_{comment_} token_{decl_} } optional_{token_{decls_}} }
		decl_{ token_{token_} optional_{token_{spaces_}} string_{:} optional_{sub_{token_{lexemes_}}} optional_{token_{spaces_}} string_{;} }

		comment_{ string_{#} token_{commentContent_} optional_{string_{\n}} }
		commentContent_{ not_{or_{ end_ string_{\n} }}+ optional_{token_{commentContent_}} }

		lexemes_{ repeat_{token_{lexeme_}} }
		lexemesWithSeparator_{ token_{lexeme_} optional_{token_{spaces_}} optional_{parenthesis_{ string_{,} token_{lexemesWithSeparator_} }} }
		lexeme_{ optional_{token_{spaces_}} or_{
			token_{quote_}
			token_{not_}
			token_{optional_}
			token_{range_}
			token_{increment_}
			token_{comma_}
			token_{read_}
			token_{add_}
			token_{hash_}
			token_{or_}
			token_{sub_}
			token_{repeat_}
			token_{end_}
			token_{parenthesis_}
			token_{tokenLexeme_}
		}}
		tokenLexeme_{ add_{token_} sub_{token_{token_}} }

		not_{ string_{!} add_{not_} sub_{token_{lexeme_}} }
		optional_{ string_{~} add_{optional_} sub_{token_{lexeme_}} }
		range_{ string_{[} optional_{token_{spaces_}} add_{parenthesis_} sub_{ add_{>=_} sub_{token_{char_}} optional_{token_{spaces_}} string_{,} optional_{token_{spaces_}} add_{<=_} sub_{token_{char_}} } optional_{token_{spaces_}} string_{]} }
		increment_{ string_{+} add_{+} }
		comma_{ string_{,} add_{,} }
		read_{ string_{read} not_{token_{alphaNumChar_}} add_{read_} }
		parenthesis_{ string_{(} add_{parenthesis_} sub_{token_{lexemes_}} optional_{token_{spaces_}} string_{)} }
		add_{ string_{add(} add_{add_} or_{ token_{rawQuote_} sub_{token_{token_}} } optional_{token_{spaces_}} string_{)} }
		hash_{ string_{hash(} add_{hash_} sub_{token_{lexeme_}} optional_{token_{spaces_}} string_{)} }
		or_{ string_{or(} add_{or_} sub_{token_{lexemesWithSeparator_}} optional_{token_{spaces_}} string_{)} }
		sub_{ string_{sub(} add_{sub_} sub_{token_{lexemes_}} optional_{token_{spaces_}} string_{)} }
		repeat_{ string_{repeat(} add_{repeat_} sub_{token_{lexeme_} optional_{parenthesis_{optional_{token_{spaces_}} string_{,} token_{lexeme_}}}} optional_{token_{spaces_}} string_{)} }
		end_{ string_{end} not_{token_{alphaNumChar_}} add_{end_} }

		quote_{ or_{ token_{quote1_} token_{quote2_} } }
		quote1_{ string_{'} add_{string_} sub_{ optional_{token_{nonQuote1_}} } string_{'} }
		quote2_{ string_{"} add_{string_} sub_{ optional_{token_{nonQuote2_}} } string_{"} }
		rawQuote_{ or_{ token_{rawQuote1_} token_{rawQuote2_} } }
		rawQuote1_{ string_{'} sub_{ optional_{token_{nonQuote1_}} } string_{'} }
		rawQuote2_{ string_{"} sub_{ optional_{token_{nonQuote2_}} } string_{"} }
		nonQuote1_{ not_{string_{'}} token_{char_} optional_{token_{nonQuote1_}} }
		nonQuote2_{ not_{string_{"}} token_{char_} optional_{token_{nonQuote2_}} }

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

		spaces_{ repeat_{token_{space_}} }
		space_{ or_{ string_{\ } string_{\n} string_{\t} string_{\r} } }
	)"sv;
	
	constexpr auto highLevelGrammarSrc = R"(
		main: decls ~spaces;
		decls: ~spaces or(comment, decl) ~decls;
		decl: token ~spaces ':' sub(~lexemes) ~spaces ';';

		comment: '#' commentContent ~'\n';
		commentContent: !or(end, '\n')+ ~commentContent;

		lexemes: repeat(lexeme);
		lexemesWithSeparator: lexeme ~spaces ~(',' lexemesWithSeparator);
		lexeme: ~spaces or(
			quote,
			not,
			optional,
			range,
			increment,
			comma,
			readLexeme,
			addLexeme,
			hash,
			or,
			sub,
			repeat,
			endLexeme,
			recurrence,
			parenthesis,
			tokenLexeme
		);
		tokenLexeme: add(token) sub(token);

		not: '!' add(not) sub(lexeme);
		optional: '~' add(optional) sub(lexeme);
		range: '[' ~spaces add(parenthesis) sub( add(\>\=) sub(char) ~spaces ',' ~spaces add(\<\=) sub(char) ) ~spaces ']';
		increment: '+' add('+');
		# comma: ',' add(',');
		readLexeme: "read" !alphaNumChar add(read);
		parenthesis: '(' add(parenthesis) sub(lexemes) ~spaces ')';
		addLexeme: "add(" add(add) or(rawQuote, sub(token)) ~spaces ')';
		hash: "hash(" add(hash) sub(lexeme) ~spaces ')';
		or: "or(" add(or) sub(lexemesWithSeparator) ~spaces ')';
		sub: "sub(" add(sub) sub(lexemes) ~spaces ')';
		repeat: "repeat(" add(repeat) sub(lexeme ~(~spaces ',' lexeme)) ~spaces ')';
		endLexeme: "end" !alphaNumChar add(end);

		quote: or(quote1, quote2);
		quote1: '\'' add(string) sub(~nonQuote1) '\'';
		quote2: '"' add(string) sub(~nonQuote2) '"';
		rawQuote: or(rawQuote1, rawQuote2);
		rawQuote1: '\'' sub(~nonQuote1) '\'';
		rawQuote2: '"' sub(~nonQuote2) '"';
		nonQuote1: !'\'' char ~nonQuote1;
		nonQuote2: !'"' char ~nonQuote2;

		token: hash(keyword);
		keyword: firstKeywordChar ~keywordContinuation;
		keywordContinuation: keywordChar ~keywordContinuation;
		firstKeywordChar: alphaChar;
		keywordChar: alphaNumChar;
		alphaChar: or(escapeChar, (or([a, z], [A, Z]) read+));
		alphaNumChar: or(alphaChar, ([0, 9] read+));

		char: or(escapeChar, (read+));
		escapeChar: '\\' or(
			('0' add('\0')),
			('a' add('\a')),
			('b' add('\b')),
			('f' add('\f')),
			('n' add('\n')),
			('r' add('\r')),
			('t' add('\t')),
			('v' add('\v')),
			(read+)
		);

		spaces: repeat(space);
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
