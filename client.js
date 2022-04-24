const zmq = require('zeromq')
let req = zmq.socket('req');

let urlFrontend = process.argv[2];
let nickClient = process.argv[3];
let txtPeticion = process.argv[4];

req.identity = nickClient;
req.connect(urlFrontend)
req.on('message', (msg)=> {
	console.log('respuesta: ' +msg)
	process.exit(0);
})
req.send(txtPeticion)
