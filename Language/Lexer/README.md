# [CppUtils](../../README.md) / [Language](../README.md) / Lexer

[![Back to Parent Page](https://img.shields.io/badge/-Back_to_Parent_Page-blue?style=for-the-badge)](../README.md)

## Lexer

The ``Lexer<Types...>`` class allows to parse a source code to generate an AST.
A grammar must be provided to know the syntax of the parsed language.
The ``Lexer<Types...>`` also needs to know the literal types recognized by the parsers to store them in the AST. These types are to be filled in the template of ``Lexer<Types...>``.

The ``Lexer<Types...>`` class provides a method ``Parser::Expression<Types...>& newExpression(const Type::Token& token, const bool isNode = true)`` to create new ``Expression``s in the desired language.
The argument ``isNode`` allows to determine if the expression must be present as a node in the generated AST.

The method ``Parser::ASTNode<Types...> parseString(const Type::Token& token, std::string_view src) const`` allows to generate an AST from the given string. The ``Expression`` representing the entry point must be indicated by its ``Token``.

---

## GrammarLexer

The ``GrammarLexer<Types...>`` class is an overlay on top of the ``Lexer<Types...>`` class. It is used to get rid of the "Expression "s and the "Lexeme "s.

Unlike ``Lexer<Types...>``, ``GrammarLexer<Types...>`` receives the grammar of the language in text format, which makes writing grammars much easier by making them more readable and concise.

Its method ``GrammarLexerTreeNode parseGrammar(std::string_view src)`` defines the grammar of the parsed language. The function returns the AST of the grammar, which is only useful for debugging purposes.

The method ``Parser::ASTNode<Types...> parseLanguage(const Type::Token& token, std::string_view src) const`` parses the language using the grammar and returns the generated AST.

---
