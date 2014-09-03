package view;

import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JProgressBar;
import javax.swing.JScrollPane;
import javax.swing.JSpinner;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.SpinnerNumberModel;
import javax.swing.SwingWorker;
import javax.swing.border.TitledBorder;
import javax.swing.table.DefaultTableModel;

import testes.Teste;
import controller.ControladorPrincipal;

/**
 * Classe responsável por exibir a Janela que define quais testes devem ser
 * feitos
 */

@SuppressWarnings("serial")
public class GuiMain extends Gui {

	/*
	 * É o controlador que faz a ligação da interface grafica com o motor de
	 * testes. Isso permite deixar a interface gráfica apenas tomando conta da
	 * exibição e do disparo/interrupção dos testes.
	 */
	ControladorPrincipal controlador = new ControladorPrincipal(this);

	/*
	 * Vetores que são utilizados para preencher as listas de tamanhos de Matriz
	 * e Número de threads
	 */

	int tamanhosMatriz[] = { 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000 };
	int numerosDeThreads[] = { 1, 2, 4, 8, 16 };

	// Definição dos objetos que compõem a GUI ================================
	Gui panelTamanhosMatriz = new Gui();
	Gui panelNumeroThreads = new Gui();

	JLabel lblTitulo = new JLabel();
	JLabel lblAlunos = new JLabel();

	// Grupo de objetos para montar a lista de Tamanhos de Matrizes.
	DefaultListModel<Integer> modelTamanhosMatriz = new DefaultListModel<>();
	JList<Integer> listaTamanhosMatriz = new JList<Integer>(modelTamanhosMatriz);
	JScrollPane scrollTamanhosMatriz = new JScrollPane(listaTamanhosMatriz);
	JTextField txtNovoTamanhoMatriz = new JTextField(5);
	JButton botaoTamanhoMatrizAdd = new JButton("+");
	JButton botaoTamanhoMatrizDel = new JButton("-");

	// Grupo de objetos para montar a lista de Número de Threads.
	DefaultListModel<Integer> modelNumeroThreads = new DefaultListModel<>();
	JList<Integer> listaNumeroThreads = new JList<Integer>(modelNumeroThreads);
	JScrollPane scrollNumeroThreads = new JScrollPane(listaNumeroThreads);
	JTextField txtNovoNumeroThreads = new JTextField(5);
	JButton botaoNumeroThreadsAdd = new JButton("+");
	JButton botaoNumeroThreadsDel = new JButton("-");

	// Spinner para controlar o número vezes que cada teste será feito.
	JSpinner spinnerTestes = new JSpinner(new SpinnerNumberModel(5, 0, 100, 5));

	// Grupo de objetos para montar o campo de resultados.
	String[] camposTabela = { "Size", "Threads", "Run #", "Start", "End", "Run Time" };
	DefaultTableModel modeloResultados = new DefaultTableModel();
	JTable tabelaResultados = new JTable(modeloResultados);

	JScrollPane scrollResultados = new JScrollPane(tabelaResultados);

	JProgressBar progresso = new JProgressBar();

	// Botões de controle da interface
	JButton botaoStart = new JButton();
	JButton botaoStop = new JButton();
	JButton botaoSalvar = new JButton();
	JButton botaoLimpar = new JButton();

	// Construtor da Classe.
	public GuiMain() {
		inicializarComponentes();
		definirListeners();
		montarLayout();

	}

	private void montarLayout() {

		// Monta o painel de tamanho de Matrizes.
		panelTamanhosMatriz.add(scrollTamanhosMatriz, 0, 0, 3, 1);
		panelTamanhosMatriz.add(botaoTamanhoMatrizDel, 1, 0);
		panelTamanhosMatriz.add(txtNovoTamanhoMatriz, 1, 1);
		panelTamanhosMatriz.add(botaoTamanhoMatrizAdd, 1, 2);

		// Monta o painel de número de threads.
		panelNumeroThreads.add(scrollNumeroThreads, 0, 0, 3, 1);
		panelNumeroThreads.add(botaoNumeroThreadsDel, 1, 0);
		panelNumeroThreads.add(txtNovoNumeroThreads, 1, 1);
		panelNumeroThreads.add(botaoNumeroThreadsAdd, 1, 2);

		// / Insere os componentes no painel principal.

		this.add(lblTitulo, 0, 0, 4, 1);
		this.add(lblAlunos, 1, 0, 4, 1);

		this.add(scrollResultados, 2, 3, 1, 9, 0.7, 1.0);

		this.add(panelNumeroThreads, 2, 1);
		this.add(panelTamanhosMatriz, 2, 0);

		this.add(new JLabel("Number of tests"), 3, 0);
		this.add(spinnerTestes, 3, 1);

		this.add(botaoSalvar, 5, 0);
		this.add(botaoLimpar, 5, 1);

		this.add(progresso, 6, 0, 2, 1);

		this.add(botaoStart, 7, 0);
		this.add(botaoStop, 7, 1);

	}

	/**
	 * Função apenas define os listeners dos objetos que respondem a eventos -
	 * Não deve ser inserido o código de tratar eventos aqui
	 */
	private void definirListeners() {

		botaoTamanhoMatrizAdd.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				acaoBotaoTamanhoMatrizAdd(e);
			}
		});

		botaoTamanhoMatrizDel.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				acaoBotaoTamanhoMatrizDel(e);
			}
		});

		botaoNumeroThreadsAdd.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				acaoBotaoNumeroThreadsAdd(e);
			}
		});

		botaoNumeroThreadsDel.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				acaoBotaoNumeroThreadsDel(e);
			}
		});

		botaoSalvar.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					acaoBotaoSalvar(e);
				} catch (IOException ex) {
					Logger.getLogger(GuiMain.class.getName()).log(Level.SEVERE, null, ex);
				}
			}
		});

		botaoLimpar.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				acaoBotaoLimpar(false);
			}
		});

		botaoStart.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				acaoBotaoStart(e);
			}
		});

		botaoStop.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				acaoBotaoStop(e);
			}
		});

	}

	private void acaoBotaoStop(ActionEvent e) {
		controlador.encerrarTestes();

	}

	private void acaoBotaoStart(ActionEvent e) {

		if (controlador.isTestesAbortados()) {
			controlador = new ControladorPrincipal(this);
		}

		if (controlador.isTestesIniciados()) {

			if (controlador.isTestesBloqueados()) {

				controlador.liberarTestes();
				botaoStart.setText("Pause");

			} else {
				progresso.setString("Pausing Test...");
				controlador.pausarTestes();
				botaoStart.setText("Continue");

			}

		} else {

			botaoStart.setText("Pause");

			new Thread(new Runnable() {
				public void run() {
					try {

						controlador.executarTestes();
					} catch (Exception e) {
						e.printStackTrace();
						JOptionPane.showMessageDialog(null, "Problem when starting tests!");

					}
				}
			}).start();

		}
	}

	private void acaoBotaoSalvar(ActionEvent e) throws IOException {
		JFileChooser salvandoArquivo = new JFileChooser();

		int resultado = salvandoArquivo.showSaveDialog(this);
		if (resultado != JFileChooser.APPROVE_OPTION) {
			return;
		}

		File salvarArquivoEscolhido = salvandoArquivo.getSelectedFile();

		PrintWriter pw = new PrintWriter(new FileWriter(salvarArquivoEscolhido + ".txt"));
		pw.println(controlador.getResults());
		pw.close();

	}

	/** Adiciona o conteúdo do textfield a lista de Tamanhos de Matrizes */
	private void acaoBotaoTamanhoMatrizAdd(ActionEvent e) {
		try {
			int valor = Integer.parseInt(txtNovoTamanhoMatriz.getText().trim());
			if (valor < 1) {
				throw new Exception();
			}
			modelTamanhosMatriz.addElement(valor);
			int lastIndex = listaTamanhosMatriz.getModel().getSize() - 1;
			if (lastIndex >= 0) {
				listaTamanhosMatriz.ensureIndexIsVisible(lastIndex);
			}
			txtNovoTamanhoMatriz.setText("");
		} catch (Exception ex) {
			JOptionPane.showMessageDialog(null, "You ta the brinqueichon uite me, cara?", "Invalid Value!",
					JOptionPane.WARNING_MESSAGE);
		}
	}

	/** Remove os tamanhos de matriz selecionados */
	private void acaoBotaoTamanhoMatrizDel(ActionEvent e) {

		int[] indices = listaTamanhosMatriz.getSelectedIndices();

		for (int i = 0; i < indices.length; i++) {
			modelTamanhosMatriz.remove(indices[i] - i);
		}
	}

	/** Adiciona o conteúdo do textfield a lista de Numero de Threads */
	private void acaoBotaoNumeroThreadsAdd(ActionEvent e) {
		try {
			int valor = Integer.parseInt(txtNovoNumeroThreads.getText().trim());
			if (valor < 1) {
				throw new Exception();
			}
			modelNumeroThreads.addElement(valor);
			int lastIndex = listaTamanhosMatriz.getModel().getSize() - 1;
			if (lastIndex >= 0) {
				listaNumeroThreads.ensureIndexIsVisible(lastIndex);
			}
			txtNovoNumeroThreads.setText("");
		} catch (Exception ex) {
			JOptionPane.showMessageDialog(null, "You ta the brinqueichon uite me, cara?", "Invalid Value!",
					JOptionPane.WARNING_MESSAGE);
		}
	}

	/** Remove os numéro de threads selecionados */
	private void acaoBotaoNumeroThreadsDel(ActionEvent e) {
		int[] indices = listaNumeroThreads.getSelectedIndices();

		for (int i = 0; i < indices.length; i++) {
			modelNumeroThreads.remove(indices[i] - i);
		}
	}

	/** Limpa os resultados */
	private void acaoBotaoLimpar(boolean bypass) {

		boolean clear = false;

		if (!bypass) {
			int opc = JOptionPane.showConfirmDialog(this, "This will abort tests. Are you sure?", "Warning!",
					JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE);
			if (opc == JOptionPane.YES_OPTION) {

				clear = true;

			}
		} else {
			clear = true;
		}

		if (clear) {
			progresso.setString("Aborting Tests...");
			acaoBotaoStop(null);

			resetGui();
			controlador = new ControladorPrincipal(this);
		}

	}

	/*
	 * Worker que isola a execução dos testes em outra thread, permitindo a GUI
	 * manter-se responsiva mesmo durante os testes.
	 */
	SwingWorker<Object, Object> worker = new SwingWorker<Object, Object>() {

		@Override
		protected Integer doInBackground() throws Exception {

			controlador.executarTestes();
			return null;

		}

	};

	/*
	 * Função responsável por definir os aspéctos gráficos dos objetos que
	 * compõem a GUI.
	 */
	public void inicializarComponentes() {

		// Define o titulo
		lblTitulo.setFont(new Font("Sans Serif", Font.BOLD, 35));
		lblTitulo.setHorizontalAlignment(JLabel.CENTER);
		lblTitulo.setText("Summa Paralell Matrix Multiply");

		lblAlunos.setFont(new Font("SansSerif", Font.BOLD, 15));
		lblAlunos.setHorizontalAlignment(JLabel.CENTER);
		lblAlunos
				.setText("<html><br>Professor: <b>Dr. Aleardo Manacero Jr</b> - Alunos: <b>Gabriel Saraiva</b> - <b>Leandro Barbosa</b> - <b>Leonardo Santos</b><br><br></html>");

		// Configura a lista de Tamanhos de Matrizes.
		modelTamanhosMatriz.clear();
		listaTamanhosMatriz.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
		for (Integer i : tamanhosMatriz) {
			modelTamanhosMatriz.addElement(i);
		}

		// Configura a lista de Número de Threads.
		modelNumeroThreads.clear();
		listaNumeroThreads.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
		for (Integer i : numerosDeThreads) {
			modelNumeroThreads.addElement(i);
		}

		// Configura os paineis de Thread e tamanho de matrizes.
		panelNumeroThreads.setBorder(new TitledBorder("Number of Threads "));
		panelTamanhosMatriz.setBorder(new TitledBorder("Size of Matrices"));

		// Configura os componentes de exibição de resultados

		while (modeloResultados.getRowCount() > 0) {
			modeloResultados.removeRow(0);
		}

		modeloResultados.setColumnCount(0);
		for (String coluna : camposTabela) {
			modeloResultados.addColumn(coluna);
		}

		progresso.setPreferredSize(new Dimension(100, 100));
		progresso.setValue(0);

		progresso.setStringPainted(true);
		progresso.setFont(new Font("Verdana", Font.BOLD, 20));
		progresso.setString("Press Start to begin");

		scrollResultados.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

		botaoStart.setText("Start");
		botaoStop.setText("Stop");
		botaoSalvar.setText("Save Results");
		botaoLimpar.setText("Clear Results");
	}

	public void sinalizaTrava() {
		progresso.setString("Paused");
	}

	public void resetGui() {

		inicializarComponentes();

	}

	// Funções que fazem inteface com o controlador ===========================

	public int[] getTamanhosDeMatrizParaTestar() {
		int[] tamanhosMatriz = new int[modelTamanhosMatriz.getSize()];

		for (int i = 0; i < modelTamanhosMatriz.getSize(); i++) {
			tamanhosMatriz[i] = modelTamanhosMatriz.elementAt(i);
		}

		return tamanhosMatriz;

	}

	public int[] getNumeroDeThreadsParaTestar() {
		int[] numerosDeThreads = new int[modelNumeroThreads.getSize()];

		for (int i = 0; i < modelNumeroThreads.getSize(); i++) {
			numerosDeThreads[i] = modelNumeroThreads.elementAt(i);
		}

		return numerosDeThreads;

	}

	public int getNumeroDeTestes() {
		return Integer.parseInt(spinnerTestes.getValue().toString());

	}

	public void atualizaProgresso() {
		if (controlador.isTestesBloqueados()) {
			return;
		}
		DecimalFormat df = new DecimalFormat("0.##");
		int concluidos = controlador.getCaixaDeTestes().getConcluidos();
		int total = controlador.getCaixaDeTestes().getTotal();

		double porcentagem = (concluidos * 100.0) / total;

		progresso.setMaximum(total);
		if (total > concluidos) {
			progresso.setString("Running test " + (concluidos + 1) + " of " + total + " (" + df.format(porcentagem)
					+ "%)");

		} else {
			progresso.setString("All the " + total + " tests done");

		}
		progresso.setValue(controlador.getCaixaDeTestes().getConcluidos());
	}

	public String formataHora(Date d) {
		SimpleDateFormat formatador = new SimpleDateFormat("HH:mm:ss");
		return formatador.format(d);
	}

	public void notificaInicioDeTeste(Teste t) {
		atualizaProgresso();

		String[] resultado = { t.getTamanhoMatriz() + "", t.getNumeroThreads() + "",
				(t.getNumeroDeSequencia() + 1) + "", formataHora(t.getInicio()), "Running", "..." };

		modeloResultados.addRow(resultado);

		tabelaResultados
				.scrollRectToVisible(tabelaResultados.getCellRect(tabelaResultados.getRowCount() + 1, 0, false));

	}

	public void notificaConclusaoDeTeste(Teste t) {
		atualizaProgresso();
		int linha = modeloResultados.getRowCount();

		modeloResultados.setValueAt(formataHora(t.getConclusao()), linha - 1, 4);

		long duracao = t.getConclusao().getTime() - t.getInicio().getTime();

		modeloResultados.setValueAt(duracao, linha - 1, 5);

	}

	public void notificaAbortagemDosTestes() {
		progresso.setString("Tests aborted!");

	}

	public void notificaConclusao() {
		atualizaProgresso();
		botaoStart.setText("Start");

	}

}
