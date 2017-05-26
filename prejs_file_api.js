self.addEventListener('message', function(e) {
      var files = e.data;
      console.log(files[0].name);
      if (typeof(Module) === "undefined") Module = {};
      Module["arguments"] = ["/work/"+files[0].name];
      Module["preInit"] = function() {
        FS.mkdir("/work");
      	FS.mount(WORKERFS, {
    	  files: files // Array of File objects or FileList
    	}, '/work');
         //FS.createLazyFile('/', "tmp.zim", "wiktionary_en_all_2017-03.zim", true, false);
      };

