package controller;

import java.util.Date;
import java.util.Random;
import java.util.concurrent.Semaphore;

import javax.swing.SwingUtilities;

import summa.SummaMultiply;
import testes.CaixaDeTestes;
import testes.Teste;
import view.GuiMain;

/**
 * Classe que liga a interface gráfica com o motor de testes
 */
public class ControladorPrincipal implements Controlador {

	GuiMain gui;

	int[] tamanhosMatriz;
	int[] numeroDeThreads;
	int numeroDeTestes;

	boolean abortarTestes = false;

	public boolean isTestesAbortados() {
		return abortarTestes;
	}

	Semaphore mutex = new Semaphore(1);

	SummaMultiply multiplicador;

	boolean testesTravados = false;

	CaixaDeTestes caixaDeTestes = null;

	public CaixaDeTestes getCaixaDeTestes() {
		return caixaDeTestes;
	}

	public void setTestes(CaixaDeTestes testes) {
		this.caixaDeTestes = testes;
	}

	public void setGui(GuiMain gui) {
		this.gui = gui;
	}

	public GuiMain getGui() {
		return gui;
	}

	public boolean isTestesIniciados() {
		return testesIniciados;
	}

	boolean testesIniciados = false;

	public ControladorPrincipal(GuiMain g) {
		gui = g;
		g.setControlador(this);
	}

	public void notificaInicioDeTeste(final Teste t) {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				gui.notificaInicioDeTeste(t);
			}
		});

	}

	public void notificaConclusaoDeTeste(final Teste t) {

		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				gui.notificaConclusaoDeTeste(t);
			}
		});
	}

	public void notificaConclusao() {
		testesIniciados = false;
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				gui.notificaConclusao();
			}
		});
	}

	public void encerrarTestes() {
		if (testesIniciados) {
			multiplicador.encerrar();
			abortarTestes = true;
		}

	}

	public void pausarTestes() {

		testesTravados = true;

	}

	public void liberarTestes() {

		testesTravados = false;
		mutex.release();
	}

	public void executarTestes() throws InterruptedException {
		testesIniciados = true;

		tamanhosMatriz = gui.getTamanhosDeMatrizParaTestar();
		numeroDeThreads = gui.getNumeroDeThreadsParaTestar();
		numeroDeTestes = gui.getNumeroDeTestes();

		int total = tamanhosMatriz.length * numeroDeTestes * numeroDeThreads.length;

		caixaDeTestes = new CaixaDeTestes(total);

		exeutarBateriaDeTestes();
	}

	/**
	 * Função que executa todos os testes solicitados na interface gráfica
	 * 
	 * @throws InterruptedException
	 */
	private void exeutarBateriaDeTestes() throws InterruptedException {

		double A[][];
		double B[][];
		double C[][];

		for (int tamanho : tamanhosMatriz) {

			A = new double[tamanho][tamanho];
			B = new double[tamanho][tamanho];
			C = new double[tamanho][tamanho];

			// Popula as matrizes com valores aleatórios
			Random rg = new Random();
			for (int i = 0; i < tamanho; i++) {
				for (int j = 0; j < tamanho; j++) {
					A[i][j] = rg.nextDouble();
					B[i][j] = rg.nextDouble();
				}
			}

			for (int threads : numeroDeThreads) {
				for (int teste = 0; teste < numeroDeTestes; teste++) {
					if (abortarTestes) {
						notificarAbortagem();
						return;
					}
					mutex.acquire();

					multiplicador = new SummaMultiply(A, B, C, threads);

					Teste novoTeste = new Teste(tamanho, threads, teste, null, null);

					novoTeste.setInicio(new Date());
					notificaInicioDeTeste(novoTeste);

					multiplicador.multiplicar();

					novoTeste.setConclusao(new Date());

					caixaDeTestes.gravarResultadosDeTeste(novoTeste);

					if (abortarTestes == false) {
						notificaConclusaoDeTeste(novoTeste);
					}

					if (testesTravados) {
						gui.sinalizaTrava();
						mutex.acquire();
					}
					mutex.release();
				}
			}

		}

		if (abortarTestes == false) {
			notificaConclusao();
		}

	}

	public boolean isTestesBloqueados() {
		return testesTravados;
	}

	public void notificarAbortagem() {

		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				gui.notificaAbortagemDosTestes();

			}
		});

	}

	public String getResults() {

		String resultado = "";

		while (!caixaDeTestes.getResultados().empty()) {

			Teste t = caixaDeTestes.getUltimoResultadoDeTeste();
			caixaDeTestes.getResultados().pop();
			resultado += t.toString() + "\n";

		}
		return resultado;

	}

}
