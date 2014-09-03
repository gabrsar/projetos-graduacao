package summa;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class SummaMultiply {

	double A[][];
	double B[][];
	double C[][];

	ExecutorService exec;

	BoundedExecutor bex;

	SummaMatrixMultiply bloco;

	MatrixMultiplyFactory factory;

	int threads;

	/**
	 * Implementa todo o mecanismo de multiplicação paralela do algoritmo SUMMA
	 * utilizando bag of tasks.
	 * 
	 * @param A
	 *            - Matriz de entrada A
	 * @param B
	 *            - Matriz de entrada B
	 * @param C
	 *            - Matriz de resultados
	 * @param threads
	 *            - Numero de threads que devem cooperar para gerar o resultado
	 */
	public SummaMultiply(double A[][], double B[][], double C[][], int threads) {

		this.A = A;
		this.B = B;
		this.C = C;
		this.threads = threads;
	}

	public void encerrar() {

		exec.shutdownNow();

	}

	public void multiplicar() throws InterruptedException {
		// Cria threadpool com numeroDeThreads threads
		exec = Executors.newFixedThreadPool(threads);
		bex = new BoundedExecutor(exec, threads);

		// Cria factory que faz o mapeamento dos threads
		// O factory tem que ser instanciado para cada testes
		// porque é um objeto que mantém o estado
		factory = new MatrixMultiplyFactory(A, B, C, threads);

		while ((bloco = factory.getNextMatrixMultipy()) != null) {

			try {
				bex.submitTask(bloco);
			} catch (InterruptedException e) {
				System.out.println("Erro ao submeter bloco para multiplicação");
			}

		}

		exec.shutdown();

		// Espera o término
		exec.awaitTermination(Long.MAX_VALUE, TimeUnit.MINUTES);
	}

}
