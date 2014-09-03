package summa;

import java.util.concurrent.Executor;
import java.util.concurrent.Semaphore;
import java.util.concurrent.RejectedExecutionException;

/**
 * Classe para submeter trabalhos a um Executor que bloqueia ao atingir número
 * máximo de trabalhos especificados e continua quando há lugar na fila.
 */
public class BoundedExecutor {

	/**
	 * classe executor que vai receber os trabalhos
	 */
	private final Executor exec;

	/**
	 * semáforo que bloqueia quando a fila está cheia
	 */
	private final Semaphore semaphore;

	/**
	 * @param exec
	 *            Executor para submeter trabalhos
	 * @param bound
	 *            Número máximo de trabalhos ativos
	 */
	public BoundedExecutor(Executor exec, int bound) {
		this.exec = exec;
		this.semaphore = new Semaphore(bound);
	}

	/**
	 * @param command
	 *            Runnable a ser executado
	 * @throws InterruptedException
	 */
	public void submitTask(final Runnable command) throws InterruptedException {
		semaphore.acquire();
		try {
			// manda executar o Runnable passado
			exec.execute(new Runnable() {
				public void run() {
					try {
						command.run();
					} finally {
						semaphore.release();
					}
				}
			});
		} catch (RejectedExecutionException e) {
			semaphore.release();
		}
	}
}
