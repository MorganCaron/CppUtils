enable_documentation = get_option('enable_documentation')

if enable_documentation

	standardese = find_program('standardese', required: enable_documentation)
	if standardese.found()

		documentation = custom_target(
			'docs',
			output: 'documentation',
			command: [
				standardese,
				'--config=../resources/documentation/standardese.ini',
				'../include/',
				'../Chrono/include/',
				'../Container/include/',
				'../External/include/',
				'../FileSystem/include/',
				'../Functional/include/',
				'../Graph/include/',
				'../Hash/include/',
				'../Language/ASM/Compiler/include/',
				'../Language/ASM/Lexer/include/',
				'../Language/ASM/VirtualMachine/include/',
				'../Language/Compiler/include/',
				'../Language/Ini/include/',
				'../Language/IR/include/',
				'../Language/IR/Compiler/include/',
				'../Language/IR/Lexer/include/',
				'../Language/Json/include/',
				'../Language/Lexer/include/',
				'../Language/Parser/include/',
				'../Language/StringTree/include/',
				'../Language/VirtualMachine/include/',
				'../Log/include/',
				'../Parameters/include/',
				'../Platform/include/',
				'../String/include/',
				'../Terminal/include/',
				'../Terminal/TextModifier/include/',
				'../Thread/include/',
				'../Type/include/',
				'../UnitTest/include/'
			],
			install: false,
			build_by_default: true
		)

	elif enable_documentation
		message('Standardese not found. The generation of the documentation is aborted.')
	endif
endif
