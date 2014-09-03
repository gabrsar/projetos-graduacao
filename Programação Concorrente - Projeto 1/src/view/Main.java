package view;

import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.UIManager;

public class Main {

	public static void main(String[] args) {

		// Define o tema para o Swing
		try {
			UIManager.setLookAndFeel("javax.swing.plaf.nimbus.NimbusLookAndFeel");
			JFrame.setDefaultLookAndFeelDecorated(true);
		} catch (Exception e) {
			String erro = "Não foi possivel aplicar o tema na janela";
			JOptionPane.showMessageDialog(null, erro);
			System.err.println(erro);
		}

		GuiMain gui = new GuiMain();

		JFrame frame = new JFrame();

		frame.setContentPane(gui);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
		frame.setMinimumSize(new Dimension(840, 570));
		frame.pack();

	}
}