self.addEventListener("message", function(e) {
    var files = e.data.files;
    var action = e.data.action;
    var url = e.data.url;
    var outgoingMessagePort = e.ports[0];
    if (action === "getContentByUrl") {
        var content = Module.getContentByUrl(url);
        outgoingMessagePort.postMessage(content);
    }
    else if (action === "getMimetypeByUrl") {
        var mimetype = Module.getMimetypeByUrl(url);
        outgoingMessagePort.postMessage(mimetype);
    }
    else if (action === "getArticleCount") {
        var articleCount = Module.getArticleCount();
        outgoingMessagePort.postMessage(articleCount);
    }
    else if (action === "init") {
        // When using split ZIM files, we need to remove the last two letters of the suffix (like .zimaa -> .zim)
        var baseZimFileName = files[0].name.replace(/\.zim..$/, '.zim');
        Module = {};
        Module["onRuntimeInitialized"] = function() {
            console.log("runtime initialized");
            Module.loadArchive("/work/" + baseZimFileName);
        };
        Module["arguments"] = [];
        for (let i = 0; i < files.length; i++) {
            Module["arguments"].push("/work/" + files[i].name);
        }
        Module["preRun"] = function() {
            FS.mkdir("/work");
            FS.mount(WORKERFS, {
                files: files
                }, "/work");
        };
        console.log("baseZimFileName = " + baseZimFileName);
        console.log('Module["arguments"] = ' + Module["arguments"])

