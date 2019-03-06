if (typeof (Module) === "undefined") Module = {};
self.addEventListener('message', function(e) {
    var files = e.data.files;
    var reader = e.data.reader;
    var action = e.data.action;
    var url = e.data.url;
    var outgoingMessagePort = e.ports[0];
    if (action === "init") {        
        console.log(files[0].name);
        Module["arguments"] = ["/work/" + files[0].name];
        Module["preInit"] = function() {
            FS.mkdir("/work");
            FS.mount(WORKERFS, {
                files: files // Array of File objects or FileList
                }, '/work');
            //FS.createLazyFile('/', "tmp.zim", "wiktionary_en_all_2017-03.zim", true, false);
        };
    }
    else if (action === "getArticleContentByUrl") {
        var content = Module.getArticleContentByUrl("/work/" + files[0].name, url);
        outgoingMessagePort.postMessage(content);
    }
    else if (action === "getArticleCount") {
        var articleCount = Module.getArticleCount("/work/" + files[0].name);
        outgoingMessagePort.postMessage(articleCount);
    }
    else if (action === "getReader") {
        var reader = Module.getReader("/work/" + files[0].name);
        outgoingMessagePort.postMessage(reader);
    }
    else if (action === "getArticleCountFromReader") {
        var articleCount = Module.getArticleCountFromReader(reader);
        outgoingMessagePort.postMessage(articleCount);
    }
    else if (action === "getEntryFromPath") {
        var content = Module.getEntryFromPath(reader, url);
        outgoingMessagePort.postMessage(content);
    }

