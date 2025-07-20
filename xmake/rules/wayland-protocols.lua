rule("wayland.protocols")
do
	-- Spécifie que la rule traite les fichiers XML
	set_extensions(".xml")

	-- Support des modules C++20
	on_load(function(target)
		local wayland_protocols_package = target:pkg("wayland-protocols")
		if not wayland_protocols_package then
			os.raise("wayland-protocols package not found")
		end

		local wayland_protocols_dir = path.join(wayland_protocols_package:installdir() or "/usr", "share", "wayland-protocols")
		assert(os.isdir(wayland_protocols_dir), "wayland-protocols directory not found")

		local protocols = {
			{ "stable/xdg-shell/xdg-shell.xml" },
			{ "unstable/xdg-decoration/xdg-decoration-unstable-v1.xml" },
			{ "unstable/pointer-constraints/pointer-constraints-unstable-v1.xml" },
			{ "unstable/relative-pointer/relative-pointer-unstable-v1.xml" },
		}

		for _, protocol in ipairs(protocols) do
			local full = path.join(wayland_protocols_dir, protocol[1])
			if os.exists(full) then
				target:add("files", full, { rule = "wayland.protocols" })
			else
				print("Protocol not found: " .. protocol[1])
			end
		end

		if target:rule("c++.build.modules.builder") then
			local rule = target:rule("c++.build.modules.builder"):clone()
			rule:add("deps", "wayland.protocols", { order = true })
			target:rule_add(rule)
		end
	end)

	on_config(function(target)
		-- Créé le répertoire de sortie
		local outputDirectory = path.join(target:autogendir(), "rules", "wayland.protocols")
		if not os.isdir(outputDirectory) then
			os.mkdir(outputDirectory)
		end

		local sourcebatches = target:sourcebatches()
		if sourcebatches["wayland.protocols"] and sourcebatches["wayland.protocols"].sourcefiles then
			for _, sourcefile in ipairs(sourcebatches["wayland.protocols"].sourcefiles) do
				-- Noms des fichiers
				local header = path.join(outputDirectory, path.basename(sourcefile) .. ".h")
				local source = path.join(outputDirectory, path.basename(sourcefile) .. ".c")

				-- Génère des headers vides pour la découverte de dépendances des modules
				if not os.exists(header) then
					os.touch(header)
				end

				-- Ajout explicitement le fichier source pour garantir son inclusion
				target:add("files", source, { always_added = true })
			end
		end

		-- Rend accessible les fichiers générés depuis le projet utilisant cette rule
		target:add("includedirs", outputDirectory, { public = true }) -- todo: retirer le public ?
	end)

	before_buildcmd_file(function(target, batchcmds, sourcefile, opt) -- sourcefile est un fichier XML
		import("lib.detect.find_tool")
		local wayland_scanner = find_tool("wayland-scanner")
		assert(wayland_scanner, "wayland-scanner not found")

		-- Créé le répertoire de sortie
		local outputDirectory = path.join(target:autogendir(), "rules", "wayland.protocols")

		-- Récupère les fichiers générés
		local header = path.join(outputDirectory, path.basename(sourcefile) .. ".h")
		local source = path.join(outputDirectory, path.basename(sourcefile) .. ".c")

		-- Génère le fichier .h
		batchcmds:show_progress(opt.progress, "Generating header for %s", path.basename(sourcefile))
		batchcmds:vexecv(wayland_scanner.program, {"client-header", sourcefile, header})

		-- Génère le fichier .c
		batchcmds:show_progress(opt.progress, "Generating source for %s", path.basename(sourcefile))
		batchcmds:vexecv(wayland_scanner.program, {"private-code", sourcefile, source})

		-- Si ce fichier xml est modifié, recompile ses .c/.h associés
		batchcmds:add_depfiles(sourcefile)
		batchcmds:set_depmtime(os.mtime(header))
		batchcmds:set_depcache(target:dependfile(header))
	end)
	
end
