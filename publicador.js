const zmq = require('zeromq')
let pub = zmq.socket('pub')

let args = process.argv.slice(2); //Empiezo a tener en cuenta lo sargs ignorando el node y nombre
let puerto = args[0];
let numMensajes = args[1];  //Contador
let temas = process.argv.slice(4); //Un vector con todos los temas


let cont = 1;
let contadorLocal=1


pub.bind('tcp://*:'+puerto)

function emite() {
    if (cont <= numMensajes) {
	let m=temas[0]
	pub.send(cont+': '+m + ' ' +contadorLocal)
    if(cont % temas.length == 0) {contadorLocal++}
    cont++
	temas.shift(); //Extrae el primer valor
    temas.push(m) // Rotatorio porque lo envia al final del vector
    }
    else process.exit();
}
setInterval(emite,1000) // every second


