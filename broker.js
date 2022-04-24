const zmq = require('zeromq')
let cli=[], req=[], workers=[]
let sc = zmq.socket('router') // frontend
let sw = zmq.socket('router') // backend

let portFrontend = process.argv[2];
let portBackend = process.argv[3];

let contadorResp = 0;
let contadorPeticiones = 0;
let contadorWorker =[];

sc.bind('tcp://*: '+ portFrontend)
sw.bind('tcp://*: '+ portBackend)
sc.on('message',(c,sep,m)=> {
	if (workers.length==0) { 
		cli.push(c); req.push(m) //SI NO HAY WORKERS DISPONIBLES AGREGO QUIEN Y SU MENSAJE PENDIENTE
	} else {
        
		sw.send([workers.shift(),'',c,'',m])
        
	}
})
sw.on('message',(w,sep,c,sep2,r)=> {
    if (c=='') {workers.push(w); return} //HAY UN WORKER DISPONIBLE
	if (cli.length>0) { //si hay mensajes pendientes, extraigo ese user y su mensaje pendiente y lo enviamos de nuevo
		sw.send([w,'',
			cli.shift(),'',req.shift()])
        
	} else {
		workers.push(w)   //Hay workers disponibles, pero no hay mensajes pendientes. Ponemos el worker a la cola
	}
	contadorPeticiones++;  //Número de peticiones que vamos respondiendo
    //CONTADOR DE WORKERS QUE ATENDIENDEN UNA PETICION
    if(contadorWorker[w]){  //Si ya tiene valor lo incrementamos
    contadorWorker[w]++;
    }
    else contadorWorker[w]=1; //Su primer valor será 1
      
      
	//MODIFICAMOS LA RESPUESTA QUE VIENE DEL BROKER PARA ENVIAR AL CLIENTE EL CONTADOR DE RESPUESTAS TOTALES ATENDIDAS
	sc.send([c,'',r+' '+ ++contadorResp])   
    
})

setInterval(()=> {console.log('Número de peticiones totales procesadas: ' + contadorPeticiones + '\n');
    for (let i in contadorWorker) { console.log('Worker ' + i + ' ha atendido: ' +contadorWorker[i] + ' peticiones\n');}
    console.log('----------------------------------------\'');
	}, 5000)
