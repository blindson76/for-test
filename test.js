// var SegfaultHandler = require('segfault-handler');
 
// SegfaultHandler.registerHandler("crash.log"); 


const Wrapped = require('./build/Release/wrapped').Wrapped
let ins = new Wrapped(1024, 1000)

ins.start(data => {
    const view = new Int32Array(data);
    console.log(view.at(0))
})

setInterval(()=>{

},1000)