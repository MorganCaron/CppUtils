#pragma once

namespace CppUtils::Language::Lexer::Grammar
{
	using namespace std::literals;

	constexpr auto lowLevelGrammar = R"(
		main_{ token_{decls_} optional_{token_{spaces_}} }

		decls_{ token_{decl_} optional_{token_{decls_}} }
		decl_{ optional_{token_{spaces_}} token_{token_} optional_{token_{spaces_}} string_{:} optional_{sub_{token_{lexemes_}}} optional_{token_{spaces_}} string_{;} }

		lexemes_{ token_{lexeme_} optional_{token_{lexemes_}} }
		lexeme_{ optional_{token_{spaces_}} or_{ token_{quote_} token_{not_} token_{optional_} token_{range_} token_{increment_} token_{comma_} token_{read_} token_{()_} token_{add_} token_{hash_} token_{or_} token_{sub_} token_{tokenLexeme_} } }
		tokenLexeme_{ add_{token_} sub_{token_{token_}} }

		not_{ string_{!} add_{not_} sub_{token_{lexeme_}} }
		optional_{ string_{~} add_{optional_} sub_{token_{lexeme_}} }
		range_{ string_{[} optional_{token_{spaces_}} add_{()_} sub_{ add_{>=_} sub_{token_{char_}} optional_{token_{spaces_}} string_{,} optional_{token_{spaces_}} add_{<=_} sub_{token_{char_}} optional_{token_{spaces_}} } string_{]} }
		increment_{ string_{+} add_{+} }
		comma_{ string_{,} add_{,} }
		read_{ string_{read} add_{read_} }
		()_{ string_{(} add_{sub_} sub_{token_{lexemes_}} string_{)} }
		add_{ string_{add(} add_{add_} sub_{token_{lexeme_}} string_{)} }
		hash_{ string_{hash(} add_{hash_} sub_{token_{lexeme_}} string_{)} }
		or_{ string_{or(} add_{or_} sub_{token_{lexemes_}} string_{)} }
		sub_{ string_{sub(} add_{sub_} sub_{token_{lexemes_}} string_{)} }

		quote_{ or_{ token_{quote1_} token_{quote2_} } }
		quote1_{ string_{'} add_{string_} sub_{ optional_{token_{nonQuote1_}} } string_{'} }
		quote2_{ string_{"} add_{string_} sub_{ optional_{token_{nonQuote2_}} } string_{"} }
		nonQuote1_{ not_{string_{'}} token_{char_} optional_{token_{nonQuote1_}} }
		nonQuote2_{ not_{string_{"}} token_{char_} optional_{token_{nonQuote2_}} }

		token_{ hash_{token_{keyword_}} }
		keyword_{ token_{keywordChar_} optional_{token_{keyword_}} }
		keywordChar_{ or_{ token_{min_} token_{maj_} } read_+ }
		min_{ >=_{a} <=_{z} }
		maj_{ >=_{A} <=_{Z} }
		char_{ string_{\\} token_{escapeChar_} } char_{ optional_{string_{\\}} read_+ }
		escapeChar_{ or_{ token_{escape0_} token_{escapeA_} token_{escapeB_} token_{escapeF_} token_{escapeN_} token_{escapeR_} token_{escapeT_} token_{escapeV_} } }
		escape0_{ string_{0} add_{\0} }
		escapeA_{ string_{a} add_{\a} }
		escapeB_{ string_{b} add_{\b} }
		escapeF_{ string_{f} add_{\f} }
		escapeN_{ string_{n} add_{\n} }
		escapeR_{ string_{r} add_{\r} }
		escapeT_{ string_{t} add_{\t} }
		escapeV_{ string_{v} add_{\v} }

		spaces_{ token_{space_} optional_{token_{spaces_}} }
		space_{ or_{ string_{\ } string_{\n} string_{\t} string_{\r} } }
	)"sv;
	
	constexpr auto highLevelGrammar = R"(
		main: decls ~spaces;
		decls: decl ~decls;
		decl: ~spaces token ~spaces ':' ~sub(lexemes) ~spaces ';';
		token: keyword;
		lexemes: lexeme ~lexemes;
		lexeme:  ~spaces keyword;

		keyword: hash(chars);
		chars: char ~chars;
		char: or([a, z] [A, Z]) read+;

		spaces: space ~spaces;
		space: or(' ' '\n' '\t' '\r');
	)"sv;
}
