if (typeof (Module) === "undefined") Module = {};
Module['onRuntimeInitialized'] = function() { console.log("runtime initialized"); };
self.addEventListener('message', function(e) {
    var files = e.data.files;
    var action = e.data.action;
    var url = e.data.url;
    var outgoingMessagePort = e.ports[0];
    var baseZimFileName = files[0].name.replace(/\.zim..$/, '.zim');
    if (action === "init") {        
        console.log(files[0].name);
        Module["arguments"] = [];
        for (let i = 0; i < files.length; i++) {
            Module["arguments"].push("/work/" + files[i].name);
        }
        Module["preInit"] = function() {
            FS.mkdir("/work");
            FS.mount(WORKERFS, {
                files: files // Array of File objects or FileList
                }, '/work');
            //FS.createLazyFile('/', "tmp.zim", "wiktionary_en_all_2017-03.zim", true, false);
        };
    }
    else if (action === "getArticleContentByUrl") {
        var content = Module.getArticleContentByUrl("/work/" + baseZimFileName, url);
        outgoingMessagePort.postMessage(content);
    }
    else if (action === "getArticleCount") {
        console.log("baseZimFileName=" + baseZimFileName);
        console.log('Module["arguments"]=' + Module["arguments"])
        var articleCount = Module.getArticleCount("/work/" + baseZimFileName);
        outgoingMessagePort.postMessage(articleCount);
    }

