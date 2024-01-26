enable_documentation = get_option('enable_documentation')

if enable_documentation

	standardese = find_program('standardese', required: true)
	if standardese.found()

		documentation = custom_target(
			'standardese',
			output: 'doc-markdown',
			command: [
				standardese,
				'--config=../resources/documentation/standardese.ini',
				'../include/',
				'../Chrono/include/',
				'../Container/include/',
				'../FileSystem/include/',
				'../Hash/include/',
				'../Language/ASM/Compiler/include/',
				'../Language/ASM/Lexer/include/',
				'../Language/ASM/VirtualMachine/include/',
				'../Language/Compiler/include/',
				'../Language/Ini/include/',
				'../Language/IR/Compiler/include/',
				'../Language/IR/Lexer/include/',
				'../Language/Json/include/',
				'../Language/Lexer/include/',
				'../Language/Parser/include/',
				'../Language/VirtualMachine/include/',
				'../Log/include/',
				'../Parameters/include/',
				'../Platform/include/',
				'../String/include/',
				'../Terminal/include/',
				'../Thread/include/',
				'../Type/include/',
				'../UnitTest/include/'
			],
			install: false,
			build_by_default: true
		)

#		mkdocs = find_program('mkdocs', required: true)
#		if mkdocs.found()
#			documentation = custom_target(
#				'mkdocs',
#				output: 'doc-html',
#				command: [
#					mkdocs,
#					'build',
#					'--config-file=../resources/documentation/mkdocs.yml'
#				],
#				install: false,
#				build_by_default: true
#			)
#		else
#			message('Mkdocs not found. The generation of the documentation is aborted.')
#		endif

	else
		message('Standardese not found. The generation of the documentation is aborted.')
	endif
endif
