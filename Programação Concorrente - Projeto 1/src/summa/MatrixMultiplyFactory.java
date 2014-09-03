package summa;

/**
 * Gera Runnables para multiplicar uma matriz concorrentemente.
 */
public class MatrixMultiplyFactory {

	private final double[][] A;
	private final double[][] B;
	private final double[][] C;
	private final int N;
	// tamanho de um lado do bloco
	private int tam_blocos;

	// próxima coluna a iniciar mapeamento
	private int prox_i = 0;
	// próxima linha a iniciar mapeamento
	private int prox_j = 0;

	/**
	 * Gera Runnables para multiplicar uma matriz concorrentemente.
	 * 
	 * @param a
	 *            Matriz para multiplicação.
	 * @param b
	 *            Matriz para multiplicação.
	 * @param c
	 *            Matriz de resultados. Deve ser alocada previamente mas não
	 *            necessita ser inicializada.
	 * 
	 * @param nthreads
	 *            Número de threads a ser usado para multiplicar.
	 */
	public MatrixMultiplyFactory(double[][] a, double[][] b, double[][] c, int nthreads) {
		A = a;
		B = b;
		C = c;
		N = A.length;

		// calcula tamanho dos blocos de acordo com o número de threads
		int qtd_bloco = 1;

		while (qtd_bloco * qtd_bloco < nthreads) {
			qtd_bloco *= 2;
		}
		tam_blocos = N / qtd_bloco;
	}

	/**
	 * Gera o próximo bloco para ser processado por uma thread.
	 * 
	 * @return Retorna MatrixMultiply pronto para execução ou null ao término.
	 */
	public SummaMatrixMultiply getNextMatrixMultipy() {

		// Testa se todos os blocos foram calculados.
		// o próximo bloco é calculado no final da função
		if (prox_i >= N) {
			return null;
		}

		// variáveis para tratamento dos casos de canto
		int delta;
		int prox_tam_i = tam_blocos;
		int prox_tam_j = tam_blocos;

		SummaMatrixMultiply mult;

		// verifica se o bloco restante não está completo em linhas
		delta = N - prox_i;
		if (delta < tam_blocos) {
			prox_tam_i = delta;
		}

		// verifica se o bloco restante não está completo em colunas
		delta = N - prox_j;
		if (delta < tam_blocos) {
			prox_tam_j = delta;
		}

		// cria o runnable
		mult = new SummaMatrixMultiply(A, B, C, prox_j, prox_i, prox_tam_i, prox_tam_j, N);

		// calcula o próximo bloco
		prox_j += tam_blocos;
		if (prox_j >= N) {
			prox_j = 0;
			prox_i += tam_blocos;
		}

		return mult;
	}
}
