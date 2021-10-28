self.addEventListener("message", function(e) {
    var files = e.data.files;
    var action = e.data.action;
    var url = e.data.url;
    var outgoingMessagePort = e.ports[0];
    if (action === "getContentByUrl") {
        var result = Module.getContentByUrl(url);
        var content = result.content;
        console.log("vectorsize=" + content.size());
        // TODO : it would more efficient to read the data directly from the buffer, instead of copying it
        var contentArray = new Uint8Array(new Array(content.size()).fill(0).map((_, id) => content.get(id)));
        outgoingMessagePort.postMessage({ content: contentArray, mimetype: result.mimetype});
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
            outgoingMessagePort.postMessage("runtime initialized");
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

