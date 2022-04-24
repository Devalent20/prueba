/********************************
**        EJERCICIO 2.17        *
/*******************************/

int main(int argc, char *argv[]) 
{
	int i, j;
	double a, b, A[M][N], B[M][N], C[M][N];
	//VARIABLES NUEVAS
	double Alocal[M][N], Blocal[M][N], Clocal[M][N];
	int myid,nproc,k;
	MPI_Init(&argc, &argv)
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
	k=M/nproc;
	
	MPI_Datatype filas_ciclicas;
	MPI_Type_vector(k,N,N*nproc,MPI_DOUBLE,&filas_ciclicas);
	MPI_Commit(&filas_ciclicas);
		
	if (myid == 0)
		LeeOperandos(A, B, &a, &b);
		
	MPI_Bcast(&a,1,MPI_DOUBLE,0,MPI_COMM_WORLD)
	MPI_Bcast(&b,1,MPI_DOUBLE,0,MPI_COMM_WORLD)
	
	if (myid == 0){
		MPI_Sendrecv(A,1,filas_ciclicas,0,0,Alocal,k*N,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE)
		MPI_Sendrecv(B,1,filas_ciclicas,0,0,Blocal,k*N,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE)
		for (i=1; i<nproc; i++){
			MPI_Send(&A[i][0],1,filas_ciclicas,i,0,MPI_COMM_WORLD)			
		}
		for (i=1; i<nproc; i++){
			MPI_Send(&B[i][0],1,filas_ciclicas,i,0,MPI_COMM_WORLD)			
		}
	}
	else {
		MPI_Recv(Alocal,k*N,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(Blocal,k*N,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}
	
	/*REPARTO CICLICO	
	for(i=0;i<k;i++){
		MPI_Scatter(&A[nproc*i][0],N,MPI_DOUBLE,Alocal[i][0],N,MPI_DOUBLE,0,MPI_COMM_WORLD)
	}
	for(i=0;i<k;i++){
		MPI_Scatter(&B[nproc*i][0],N,MPI_DOUBLE,Blocal[i][0],N,MPI_DOUBLE,0,MPI_COMM_WORLD)
	}
	*/
	for (i=0; i<k; i++) {
		for (j=0; j<N; j++) {
			Clocal[i][j] = a*Alocal[i][j] + b*Blocal[i][j];
		}
	}
	
	if (myid == 0){
		MPI_Sendrecv(Clocal,k*N,MPI_DOUBLE,0,0,C,1,filas_ciclicas,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE)
		for (i=1; i<nproc; i++){
			MPI_Recv(&C[i][0],1,filas_ciclicas,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE)			
		}
	}
	else {
		MPI_Send(Clocal,k*N,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
	}
	
	/* RECOGIDA POR FILAS CÍCLICAS 
	for(i=0;i<k;i++){
		MPI_Gather(&Clocal[i][0],N,MPI_DOUBLE,&C[i*nproc][0],N,MPI_DOUBLE,0,MPI_COMM_WORLD);
	}
	*/
	if (myid == 0)
		EscribeMatriz(C);
	MPI_Finalize();
	return 0;
}

