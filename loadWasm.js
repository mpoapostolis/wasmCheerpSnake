window.Module = {
  canvas: (function() {
    var canvas = document.getElementById("canvas");
    canvas.height = window.innerHeight
    canvas.width = window.innerWidth
    return canvas;
  })(),
};

(function() {
  var memoryInitializer = "index.js.mem";
  if (typeof Module["locateFile"] === "function") {
    memoryInitializer = Module["locateFile"](memoryInitializer);
  } else if (Module["memoryInitializerPrefixURL"]) {
    memoryInitializer = Module["memoryInitializerPrefixURL"] + memoryInitializer;
  }
  var xhr = (Module["memoryInitializerRequest"] = new XMLHttpRequest());
  xhr.open("GET", memoryInitializer, true);
  xhr.responseType = "arraybuffer";
  xhr.send(null);
})();

var script = document.createElement("script");
script.src = "index.js";
document.body.appendChild(script);
