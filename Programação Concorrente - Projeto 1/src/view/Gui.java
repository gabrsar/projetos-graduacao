package view;

import java.awt.Component;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.JPanel;

import controller.Controlador;

@SuppressWarnings("serial")
public class Gui extends JPanel {

	Controlador controlador = null;

	GridBagLayout layout = new GridBagLayout();

	GridBagConstraints regras = new GridBagConstraints();

	public Gui() {

		this.setLayout(layout);

		regras.insets = new Insets(2, 2, 2, 2);

		regras.fill = GridBagConstraints.BOTH;
	}

	public Component add(Component comp, int linha, int coluna) {

		regras.gridy = linha;
		regras.gridx = coluna;
		regras.weightx = 1;
		regras.weightx = 1;
		regras.gridwidth = 1;
		regras.gridheight = 1;

		super.add(comp, regras);

		return this;
	}

	public Component add(Component comp, int linha, int coluna, int w, int h) {

		regras.gridy = linha;
		regras.gridx = coluna;
		regras.gridwidth = w;
		regras.gridheight = h;
		regras.weightx = 1;
		regras.weightx = 1;

		super.add(comp, regras);

		return this;
	}

	public Component add(Component comp, int linha, int coluna, double w, double h) {

		regras.gridy = linha;
		regras.gridx = coluna;
		regras.weightx = w;
		regras.weighty = h;

		super.add(comp, regras);

		return this;
	}

	public Component add(Component comp, int linha, int coluna, int tamanho_colunas, int tamanho_linhas, double w,
			double h) {

		regras.gridy = linha;
		regras.gridx = coluna;
		regras.weightx = w;
		regras.weighty = h;
		regras.gridwidth = tamanho_colunas;
		regras.gridheight = tamanho_linhas;

		super.add(comp, regras);

		return this;
	}

	public void setControlador(Controlador c) {
		controlador = c;
	}

	public Controlador getControlador() {
		return controlador;
	}

}
