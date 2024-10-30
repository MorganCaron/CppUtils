rule("wayland.protocols")
do
	-- Traite des fichiers XML
	set_extensions(".xml")

	-- Support des modules C++20
	on_load(function(target)
		if target:rule("c++.build.modules.builder") then
			local rule = target:rule("c++.build.modules.builder"):clone()
			rule:add("deps", "wayland.protocols", { order = true })
			target:rule_add(rule)
		end
	end)

	on_config(function(target)
		-- Créé le répertoire de sortie
		local outputdir = path.join(target:autogendir(), "rules", "wayland.protocols")
		if not os.isdir(outputdir) then
			os.mkdir(outputdir)
		end

		-- Rend accessible les fichiers générés depuis le projet utilisant cette rule
		target:add("includedirs", outputdir, { public = true })
	end)

	before_buildcmd_file(function(target, batchcmds, sourcefile, opt) -- sourcefile est un fichier XML
		-- Créé le répertoire de sortie
		local outputdir = path.join(target:autogendir(), "rules", "wayland.protocols")

		-- Récupère les fichiers générés
		local header = path.join(outputdir, path.basename(sourcefile) .. ".h")
		local source = path.join(outputdir, path.basename(sourcefile) .. ".c")

		-- Génère le fichier .h uniquement s'il n'existe pas déjà
		if not os.exists(header) then
			batchcmds:show_progress(opt.progress, "Generating header for %s", path.basename(sourcefile))
			local result = batchcmds:vexecv("wayland-scanner", {"client-header", sourcefile, header})
			if result ~= 0 then
				print("Failed to generate header for:", sourcefile)
			end
		end

		-- Génère le fichier .c uniquement s'il n'existe pas déjà
		if not os.exists(source) then
			batchcmds:show_progress(opt.progress, "Generating source for %s", path.basename(sourcefile))
			local result = batchcmds:vexecv("wayland-scanner", {"private-code", sourcefile, source})
			if result ~= 0 then
				print("Failed to generate source for:", sourcefile)
			end
		end

		-- Ajout explicitement le fichier source pour garantir son inclusion
		target:add("files", source, { always_added = true })

		-- Si ce fichier xml est modifié, recompile ses .c/.h associés
		batchcmds:add_depfiles(sourcefile)
	end)
	
end
