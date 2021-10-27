if (typeof (Module) === "undefined") Module = {};
Module['onRuntimeInitialized'] = function() { console.log("runtime initialized"); };
self.addEventListener('message', function(e) {
    var files = e.data.files;
    var action = e.data.action;
    var url = e.data.url;
    var outgoingMessagePort = e.ports[0];
    // When using split ZIM files, we need to remove the last two letters of the suffix (like .zimaa -> .zim)
    var baseZimFileName = files[0].name.replace(/\.zim..$/, '.zim');
    if (action === "init") {        
        Module["arguments"] = [];
        for (let i = 0; i < files.length; i++) {
            Module["arguments"].push("/work/" + files[i].name);
        }
        Module["preRun"] = function() {
            FS.mkdir("/work");
            FS.mount(WORKERFS, {
                files: files
                }, '/work');
        };
        console.log("baseZimFileName = " + baseZimFileName);
        console.log('Module["arguments"] = ' + Module["arguments"])
    }
    else if (action === "getArticleContentByUrl") {
        var content = Module.getArticleContentByUrl("/work/" + baseZimFileName, url);
        outgoingMessagePort.postMessage(content);
    }
    else if (action === "getArticleCount") {
        var articleCount = Module.getArticleCount("/work/" + baseZimFileName);
        outgoingMessagePort.postMessage(articleCount);
    }

