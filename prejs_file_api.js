if (typeof (Module) === "undefined") Module = {};
var reader;
self.addEventListener('message', function(e) {
    var files = e.data.files;
    //var reader = e.data.reader;
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
        reader = Module.getReader("/work/" + files[0].name);
        outgoingMessagePort.postMessage(reader);
    }
    else if (action === "getArticleCountFromReader") {
        var articleCount = reader.getArticleCount();
        outgoingMessagePort.postMessage(articleCount);
    }
    else if (action === "getEntryFromPath") {
        var content = reader.getEntryFromPath(url);
        outgoingMessagePort.postMessage(content);
    }

