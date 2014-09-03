package summa;

/**
 * Multiplica duas matrizes quadradas usando produto externo. Pode ser usada
 * para multiplicar uma parte da matriz. Assim, vários threads podem ser usados
 * apenas variando os parâmetros.
 */
public class SummaMatrixMultiply implements Runnable {

	private final double[][] A;
	private final double[][] B;
	private final double[][] C;

	private final int j0;
	private final int i0;
	private final int m;
	private final int n;
	private final int n_total;

	/**
	 * @param a
	 *            Matriz a ser multiplicada.
	 * @param b
	 *            Matriz a ser multiplicada.
	 * @param c
	 *            Matriz de resultados. Deve ser alocada previamente, mas não
	 *            necessita de inicialização dos valores.
	 * @param j0
	 *            Coluna inicial a ser considerada. Use 0 para o início das
	 *            matrizes.
	 * @param i0
	 *            Linha inicial a ser consideradad. Use 0 para o início das
	 *            matrizes.
	 * @param m
	 *            Número de linhas a ser considerado.
	 * @param n
	 *            Número de colunas as ser considerado.
	 * @param n_total
	 *            Número de elementos de um lado da matriz toda
	 */
	public SummaMatrixMultiply(double[][] a, double[][] b, double[][] c, int j0, int i0, int m, int n, int n_total) {
		A = a;
		B = b;
		C = c;
		this.j0 = j0;
		this.i0 = i0;
		this.m = m;
		this.n = n;
		this.n_total = n_total;
	}

	public void run() {
		// inicializa com zeros a matriz de Resultados.
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				C[i0 + i][j0 + j] = 0.0;
			}
		}

		// realiza o cálculo
		for (int k = 0; k < n_total; k++) {
			for (int i = 0; i < m; i++) {
				for (int j = 0; j < n; j++) {
					C[i0 + i][j0 + j] += A[i0 + i][k] * B[k][j0 + j];
				}
			}
		}
	}
}
