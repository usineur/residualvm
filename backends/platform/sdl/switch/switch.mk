residualvm.nro: $(EXECUTABLE)
	mkdir -p ./switch_release/residualvm
	nacptool --create "ResidualVM" "usineur" "$(VERSION)" ./switch_release/residualvm.nacp
	elf2nro $(EXECUTABLE) ./switch_release/residualvm/residualvm.nro --icon=$(srcdir)/dists/switch/icon.jpg --nacp=./switch_release/residualvm.nacp

residualvm_switch.zip: residualvm.nro
	rm -f ./switch_release/residualvm.nacp
	mkdir -p ./switch_release/residualvm/data
	mkdir -p ./switch_release/residualvm/doc
	cp $(srcdir)/backends/vkeybd/packs/vkeybd_default.zip ./switch_release/residualvm/data
	cp $(srcdir)/backends/vkeybd/packs/vkeybd_small.zip ./switch_release/residualvm/data
	cp $(DIST_FILES_THEMES) ./switch_release/residualvm/data
ifdef DIST_FILES_ENGINEDATA
	cp $(DIST_FILES_ENGINEDATA) ./switch_release/residualvm/data
endif
ifdef DIST_FILES_SHADERS
	mkdir -p ./switch_release/residualvm/data/shaders
	cp $(DIST_FILES_SHADERS) ./switch_release/residualvm/data/shaders
endif
	cp $(DIST_FILES_DOCS) ./switch_release/residualvm/doc/
	cd ./switch_release && zip -r ../residualvm_switch.zip . && cd ..

.PHONY: residualvm.nro residualvm_switch.zip

